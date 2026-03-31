# auto-swing

Arduino firmware for an automatic baby carriage rocker.

## Hardware

An MPU6050 IMU detects the swing angle and a servo motor drives the swing back and forth.

| Component | Notes |
|-----------|-------|
| Arduino Uno (ATmega328P) | Target board |
| MPU6050 | 6-axis IMU (accelerometer + gyroscope) over I2C |
| Servo motor | Attached to Arduino pin 3 |

## Repository layout

```
blink.cpp          # Arduino sketch – setup() / loop() entry points
blob.cpp / .hpp    # Unity-build "blob" that pulls in all src/ files
src/
  myposition.cpp/hpp  # MPU6050 driver wrapper  (position_t)
  mycontrol.cpp/hpp   # Servo + FSM controller  (control_t)
  mydsp.cpp/hpp       # Swing-direction detector (dsp_t)
  myfsm.hpp           # Generic templated FSM    (myfsm_t<ST,EV,VP>)
  fixed_iir.cpp/hpp   # Fixed-point IIR filter
  myprintf.cpp/hpp    # printf macro via Serial
  myutil.hpp          # Misc utilities (ARRAY_SIZE)
test/              # Host-side unit tests (RapidCheck)
libraries/         # Vendored Arduino libraries
3rd/               # Third-party test dependencies (RapidCheck)
doc/               # Development notes
py/                # Python data capture / plotting scripts
```

The `blob` pattern is used instead of a library so that all `src/` files share
a single compilation unit without the boilerplate of an Arduino library.
`blob.hpp` `#include`s every header and `blob.cpp` `#include`s every `.cpp`.
`blink.cpp` includes only `blob.hpp`.

## Flow

All runtime logic is split among three top-level objects declared as globals in
`blink.cpp`:

```cpp
position_t pos;     // reads raw IMU data each tick
control_t  control; // drives the servo via an FSM
dsp_t      dsp;     // detects swing peaks from IMU data
```

### setup()

```cpp
void setup() {
    Serial.begin(230400);
    pos.setup();          // initialise I2C + MPU6050
    control.setup();      // attach servo; register FSM states
    dsp.setup(&pos);      // store pointer to pos for data access
}
```

`control_t::setup()` is where states are **registered** with the FSM:

```cpp
fsm.init({
    {STATE_IDLE,             idle_enter,    idle_process},
    {STATE_CHAR,             char_enter,    char_process},
    {STATE_DELAY_AFTER_CHAR, delay_enter,   delay_process},
}, this);           // 'this' is stored as fsm.vp (void pointer)
```

Each entry is a `state_record_t` containing:
- the state enum value (used as an array index internally)
- an optional `enter` callback – called once when the FSM transitions **into** the state
- a required `process` callback – called for every event while in that state

The `this` pointer (`control_t*`) is stored in `fsm.vp` so callbacks can reach
back into the owning object without globals.

### loop()

```cpp
void loop() {
    pos.tick();       // poll MPU6050, update ax/ay/az/gx/gy/gz
    control.tick();   // post EV_RUN to FSM and drain event queue
    dsp.tick();       // run swing-peak detection on latest IMU data
}
```

### FSM tick flow

`control_t::tick()` calls `fsm.postTick(EV_RUN)`:

```
postTick(EV_RUN)
  └─ push EV_RUN onto ev_q
  └─ tick()
       └─ while ev_q not empty OR state change pending:
            ├─ dequeue event → call current state's process(fsm, ev)
            │    process() may call fsm.go(NEW_STATE)
            │      └─ sets state = NEW_STATE, changing = true
            └─ if changing:
                 └─ changing = false
                 └─ call new state's enter(fsm)   (if non-NULL)
                      enter() may call fsm.go() again → loop continues
```

The loop in `myfsm_t::tick()` keeps running until both the event queue is empty
**and** no state transition is pending, so a chain of `go()` calls inside
`enter` callbacks resolves in the same `tick()` call.

### DSP tick flow

`dsp_t::tick()` reads `pos->ax` and `pos->az`, computes
`angle = 10000 * atan2(ax, az)`, and feeds it into a simple three-sample
peak detector (`run()`) that prints a CSV tag when a local minimum is found
(i.e. the swing has passed through the bottom).

## Build

Requires [arduino-mk](https://github.com/sudar/Arduino-Makefile) installed to
`/usr/share/arduino`.

```bash
# compile
make

# compile and flash
make upload

# open serial monitor
make monitor
```

The board target is `uno`. See the `Makefile` for commented-out configurations
for Leonardo, Mega, Nano, and Pro Mini.

## Runtime tuning

While the firmware is running, the following keys are accepted over the serial
port (230400 baud) to adjust swing parameters live:

| Key | Effect |
|-----|--------|
| `q` / `a` | Increase / decrease servo end-stop margin (`bumpers`) by 5° |
| `w` / `s` | Increase / decrease post-swing delay (`dafter`) by 10 ms |
| `p` | Toggle swing enable on/off |

## Tests

Host-side tests use [RapidCheck](https://github.com/emil-e/rapidcheck) for
property-based testing of `myfsm_t`.

```bash
cd test/build
cmake .. && make
./test

# single run (no shrinking)
RC_PARAMS="max_success=1" ./test
```

Tests cover:
- `test_fsm.cpp` – basic FSM transitions, enter/process ordering, `go()` chaining
- `test_control_fsm.cpp` – control-specific state machine behaviour (idle → char → delay → idle cycle)
