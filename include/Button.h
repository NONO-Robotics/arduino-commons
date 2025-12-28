#pragma once
#include <Arduino.h>

class Button {
  private:
    uint8_t pin;
    bool previousState;
    unsigned long lastDebounceTime = 0;
    const unsigned long debounceTime = 50; // milisegundos

  public:
    Button(uint8_t p);

    void init();

    bool pressed();
};