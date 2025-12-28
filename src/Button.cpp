#include "Button.h"

Button::Button(uint8_t p) : pin(p), previousState(HIGH) {}

void Button::init()
{
    pinMode(pin, INPUT_PULLUP);
}

bool Button::pressed()
{
    bool currentState = digitalRead(pin);
    bool result = false;

    if (currentState != previousState && (millis() - lastDebounceTime) > debounceTime)
    {
        if (currentState == LOW)
        { // El botón BOOT conecta a GND
            result = true;
        }
        lastDebounceTime = millis();
        previousState = currentState;
    }
    return result;
}
