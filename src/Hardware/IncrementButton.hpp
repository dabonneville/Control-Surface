#pragma once

#include "Button.h"

class IncrementButton {
    // TODO: move to settings
    static constexpr unsigned long longPressDelay = 450;       // milliseconds
    static constexpr unsigned long longPressRepeatDelay = 350; // milliseconds

  public:
    IncrementButton(const Button &button) : button(button) {}

    void begin() { button.begin(); }

    enum State { Nothing = 0, Increment };

    State getState() {
        Button::State incrState = button.getState();

        if (incrState == Button::Released) {
            // Button released, don't do anything
            // This one is first to minimize overhead
            // because most of the time, the button will
            // be released
        } else if (incrState == Button::Rising) {
            longPressState = Initial;
        } else if (incrState == Button::Falling) {
            return Increment;
        } else { // if (incrState == Button::Pressed)
            if (longPressState == LongPress) {
                if (millis() - longPressRepeat >= longPressRepeatDelay) {
                    longPressRepeat += longPressRepeatDelay;
                    return Increment;
                }
            } else if (button.stableTime() >= longPressDelay) {
                longPressState = LongPress;
                longPressRepeat = millis();
                return Increment;
            }
        }
        return Nothing;
    }

  private:
    Button button;

    enum {
        Initial,
        LongPress,
    } longPressState = Initial;
    unsigned long longPressRepeat;
};