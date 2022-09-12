#include <rapidcheck.h>

#include <vector>
#include <algorithm>
#include <iostream>
// #include <iomanip>

using namespace std;


int main(void) {
    cout << "in test\n";

    rc::check("rc is working?", [](void) {

        const uint32_t a_word = *rc::gen::inRange(0u, 0x3u);
        cout << "value: " << a_word << "\n";

        RC_ASSERT(
            1 == 1
            );

    });
}