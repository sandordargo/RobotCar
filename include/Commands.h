#include <wiringPi.h>

namespace Commands {

void goForward() {
    wiringPiSetup();
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
    pinMode(10, OUTPUT);
    digitalWrite(10, 1);
    digitalWrite(11, 0);
    digitalWrite(12, 0);
    digitalWrite(13, 1);
}

void goBackward() {
    wiringPiSetup();
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
    pinMode(10, OUTPUT);
    digitalWrite(10, 0);
    digitalWrite(11, 1);
    digitalWrite(12, 1);
    digitalWrite(13, 0);
}

}