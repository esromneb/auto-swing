# FSM Critique and Development Plan

## Overview

`myfsm_t` is a template-based, event-driven Finite State Machine (FSM) for embedded C++. It accepts three type parameters — a state enum (`ST_T`), an event enum (`EV_T`), and a user-data struct (`VP_T`) — and dispatches posted events to per-state `process` callbacks, firing `enter` callbacks on transitions. The design is clean for a prototype but has several embedded-specific pitfalls and missing abstractions that would need to be addressed before this can be used as a production library.

---

## 1. Critical Bugs and Pitfalls

### 1.1 Enum-as-Array-Index with No Bounds Checking

`init()` casts each state enum value to `size_t` and uses it as a direct array index:

```cpp
states.resize(max+1);
states[sint] = { ... };
```

And in `tick()`:

```cpp
const size_t sint = (size_t)state;
const state_record_t<...> record = states[sint];
```

If a state value is ever outside the range `[0, max]` (possible via a bug, an uninitialized FSM, or a cast from an integer), this is an **out-of-bounds access with undefined behaviour**. There is no `assert`, no range check, and no graceful error path. On a microcontroller this silently corrupts RAM.

Additionally, sparse enums cause wasted heap allocation. An enum with values `{STATE_A=0, STATE_B=100}` allocates a vector of 101 entries, 99 of which are uninitialised. Calling `process` on one of those ghost states (which have a NULL process pointer) causes a null function-pointer dereference.

### 1.2 `std::vector` Used as an Event Queue with O(n) Dequeue

```cpp
ev_q.erase(ev_q.begin());
```

`std::vector::erase` from the front is O(n) — it shifts every remaining element down. For a queue this is the wrong data structure. On a fast host machine this is harmless, but on an 8/16 MHz microcontroller processing high-frequency events it is measurably expensive. A `std::deque` or a fixed-size circular buffer is the correct choice.

### 1.3 `std::vector` Heap Use on Bare-Metal Targets

Both `states` and `ev_q` are `std::vector` members, meaning every `init()` call and every `post()` call touches the heap. On AVR (Arduino Uno) and similar targets:

- The heap is small (2 kB total SRAM on the Uno).
- Heap fragmentation accumulates over long run times.
- `malloc` / `free` are not interrupt-safe.
- There is no out-of-memory exception — a failed allocation returns a null pointer that is silently dereferenced.

For a library targeting resource-constrained MCUs, capacity should be fixed at compile time (template integer parameters) and backed by a statically-allocated array.

### 1.4 Initial State Entry Callback Is Never Fired

The FSM initialises its state to `(ST_T)0` but never calls `enter` for that initial state. Any setup that the initial state's `enter` callback is expected to perform (initialising hardware, logging, etc.) is silently skipped. The developer must remember to call `fsm.go(INITIAL_STATE)` after `init()` to trigger the first `enter`, which is non-obvious.

### 1.5 `postTick()` Is Not Re-entrant

`postTick()` posts an event and then immediately calls `tick()`. If `postTick()` is called from *inside* a `process` or `enter` callback, the inner `tick()` call will start consuming the event queue while the outer `tick()` loop is still iterating over it. The queue state seen by the outer loop after the inner call returns is unpredictable in terms of ordering. `control_t::tick()` calls `fsm.postTick(EV_RUN)` on every main-loop iteration, so any callback that also calls `postTick()` would trigger this problem.

### 1.6 `go()` Called Outside a Callback Leaves FSM in a Pending-Transition Limbo

```cpp
void myfsm_t::go(const ST_T st) {
    state = st;
    changing = true;
}
```

`go()` is `public`. If user code calls `go()` directly without subsequently calling `tick()`, the FSM is left with `changing == true` but `enter` has not fired. The FSM's internal invariant (enter always runs on transition) is broken silently.

### 1.7 Cascading `go()` Calls in `enter` with No Cycle Detection

If an `enter` callback calls `go()` to an intermediate state, and *that* state's `enter` calls `go()` again, the while-loop in `tick()` will keep iterating. With a logic bug this becomes an infinite loop that blocks the MCU forever. There is no depth limit, watchdog integration point, or cycle detection.

---

## 2. Design Limitations

### 2.1 No Exit Callback

Only `enter` and `process` callbacks exist. There is no `exit` callback called when a state is *left*. This forces cleanup logic to be split across the next state's `enter` and the outgoing state's `process`, making it harder to reason about resource cleanup (e.g. stopping a timer, de-asserting a pin).

### 2.2 No Guard Conditions as First-Class Citizens

Transition logic (guards) is entirely embedded inside `process` callbacks as ad-hoc `if` statements. There is no transition table, no declarative guard function, and no way to inspect the set of valid transitions programmatically. This makes the FSM opaque — you cannot tell which states are reachable from which without reading all the callback bodies.

### 2.3 No Timeout Transitions

Timed transitions (e.g. "leave this state after 500 ms if no event arrives") are manually implemented by storing a timestamp in the user-data struct and polling in the `process` callback with `EV_RUN`. This is what `STATE_DELAY_AFTER_CHAR` does. It works, but it is boilerplate that every state needing a timeout must repeat, and the resolution depends entirely on how often `EV_RUN` is posted.

### 2.4 No Hierarchical States

There is no support for nested/hierarchical states (HSM). All states are flat. Shared behaviour between related states (e.g. a global `EV_RESET` that always returns to `STATE_IDLE`) must be duplicated in every `process` callback that needs it.

### 2.5 No State History

There is no shallow or deep history pseudo-state. Returning to a compound state after an interruption always starts at the initial sub-state.

### 2.6 Internal State and Callbacks Are Public

`states`, `ev_q`, `state`, `changing`, and `vp` are all public struct members. Any code with a reference to the FSM can corrupt its internals. For a library, these should be private with a controlled public interface.

---

## 3. Application-Level Issues in `mycontrol.cpp`

### 3.1 Static Mutable State Inside Callbacks

```cpp
static bool swingleft = true;   // inside idle_process
static int bumpers = 55;        // file scope
static int dafter  = 480;       // file scope
static int enable  = 0;         // file scope
```

These statics are module-level globals, not members of `control_t`. This makes it impossible to have two independent `control_t` instances, and makes unit testing fragile (state leaks between test runs).

### 3.2 `handleChar2()` Uses Blocking `delay()`

`handleChar2()` is a member of `control_t` that calls `delay()` (blocking). It is not wired into the FSM at all in the current `#if 1` block. It is dead code whose presence is confusing, and its blocking semantics are incompatible with a non-blocking FSM design.

### 3.3 `#if 0` / `#if 1` Conditional Blocks as Version Control

Two complete alternative implementations exist in the same `.cpp` file gated by `#if 1` / `#if 0`. This is fragile, makes the file hard to read, and defeats diff-based version control. The inactive block should be deleted, with history preserved in git.

### 3.4 `mycontrol.hpp` Does Not Include `myfsm.hpp`

`mycontrol.hpp` references `myfsm_t<>` and the `using fsm_t = ...` alias but does not `#include "myfsm.hpp"`. It compiles only because the translation units that include `mycontrol.hpp` happen to include `myfsm.hpp` first. This is a latent include-order dependency.

### 3.5 `STATE_CHAR` Is Functionally Dead

`char_enter` and `char_process` exist but `char_process` does nothing, and the state transitions straight to `STATE_DELAY_AFTER_CHAR` from `char_enter`. The state adds complexity without contributing behaviour.

---

## 4. Test Suite Observations

### 4.1 Acknowledged Fragility in `test_control_fsm.cpp`

```cpp
// Including or excluding this breaks the tests
// fsm.go(STATE_IDLE);
```

This comment highlights that the test author does not fully understand whether `go()` should be called inside `delay_process`. This uncertainty about FSM semantics is a symptom of the unclear entry/event ordering documented in §1.5 above and indicates the need for a precise written specification of the dispatch order.

### 4.2 Global Test State Makes Tests Order-Dependent

`count_delay_enter`, `last_char_time`, and `fns` are file-scope globals reset manually at the start of each test. If a future test is added without remembering to reset them, it will produce false results silently. Test state should be local to each test fixture.

---

## 5. Plan to Complete This as a Usable Library

### Phase 1 — Stabilise the Core Engine

| Task | Detail |
|------|--------|
| Fix bounds checking | Add a static assert or runtime assert that `(size_t)state < states.size()` before every access |
| Add NULL process guard | Make a NULL process pointer a hard compile-time or init-time error, not a printf |
| Fix initial state entry | After `init()`, call `enter` for the initial state (or require the caller to call `go(initial)`) |
| Make `go()` private / friend-only | Expose only `post()` and `tick()` to external callers; restrict `go()` to callbacks |
| Define re-entrancy rules | Document and enforce that `postTick()` must not be called from within a callback |

### Phase 2 — Replace Heap Allocations with Fixed-Size Storage

```cpp
// Target API:
template <typename ST_T, typename EV_T, typename VP_T,
          size_t MAX_STATES, size_t QUEUE_DEPTH>
struct myfsm_t {
    state_record_t<ST_T, EV_T, VP_T> states[MAX_STATES];
    EV_T ev_q[QUEUE_DEPTH];
    uint8_t q_head, q_tail, q_count;
    ...
};
```

- Replace `std::vector<state_record_t>` with a fixed array sized by template parameter `MAX_STATES`.
- Replace `std::vector<EV_T>` with a fixed-size power-of-two ring buffer sized by `QUEUE_DEPTH`.
- Remove all heap allocation; all storage is on the stack or in BSS.

### Phase 3 — Add Missing Callbacks and Transition Table

- Add an `exit` callback field to `state_record_t`, called when `go()` is invoked *from* that state.
- Consider a declarative transition table:
  ```cpp
  { FROM_STATE, EVENT, guard_fn, TO_STATE }
  ```
  so that `process` callbacks only need to emit events and the engine resolves transitions.
- Add a timeout field to `state_record_t` (duration + destination state) to eliminate manual timestamp polling.

### Phase 4 — Harden the Application Layer (`mycontrol`)

- Move `bumpers`, `dafter`, `enable`, and `swingleft` into `control_t` struct fields.
- Delete `handleChar2()` or rewrite it as a non-blocking FSM event source.
- Delete the `#if 0` block and commit to one implementation.
- Add `#include "myfsm.hpp"` to `mycontrol.hpp`.
- Remove or flesh out `STATE_CHAR`.

### Phase 5 — Test Suite Improvements

- Resolve and document the `go()` / event ordering semantics with an explicit sequence diagram.
- Replace global test state with per-test local state or a test fixture struct.
- Add property-based tests (rapidcheck is already present) for:
  - Queuing N events always processes exactly N events.
  - `enter` is called exactly once per transition, always before the next `process`.
  - Posting to a full queue is handled gracefully (once Phase 2 is in place).
  - No transition fires if no `go()` is called.

### Phase 6 — Documentation and API Contract

- Write a state machine specification document defining:
  - Dispatch order: `process` → `exit` → `enter`.
  - Event queue overflow behaviour.
  - Re-entrancy guarantees.
  - Thread / ISR safety guarantees (or lack thereof).
- Provide a minimal worked example independent of the servo application.
- Add Doxygen-style comments to all public API functions.

---

## Summary

The core FSM mechanism — template dispatch, enum-indexed state storage, and the `post`/`tick` interface — is a solid starting point. The main risks are the heap-based dynamic allocation (fatal on low-SRAM MCUs under load), the absence of bounds checking (silent UB), the missing initial `enter` call, and the re-entrancy hazard in `postTick`. The application code in `mycontrol` has secondary issues around global state and dead code that would need to be cleaned up before the system can be reliably tested or extended.
