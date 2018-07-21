#pragma once

#ifndef Encoder_h_
#error                                                                         \
    "PJRC Encoder library required (https://github.com/PaulStoffregen/Encoder)"
#endif

#include "Abstract/RelativeCCOut.h"

/** A normal rotary encoder sends four pulses per physical 'click'. */
constexpr uint8_t NORMAL_ENCODER = 4;
/** For jog wheels, you want the highest possible resolution. */
constexpr uint8_t JOG = 1;

/**
 * @brief   A class for rotary encoders that send relative MIDI Controller 
 *          Change events.
 * 
 * @note    This class requires the [PJRC Encoder library]
 *          (https://github.com/PaulStoffregen/Encoder) to be included into
 *          the main sketch _before_ the Control Surface library. 
 */
class RotaryEncoder : public RelativeCCOut {
  public:
    /**
     * @brief   Construct a new RotaryEncoder.
     * 
     * @param   pinA
     *          The first switch pin of the encoder.  
     *          The internal pull-up resistor will be enabled.
     * @param   pinB
     *          The second switch pin of the encoder.  
     *          The internal pull-up resistor will be enabled.
     * @param   controllerNumber
     *          The MIDI Controller number. [0, 119]
     * @param   channel
     *          The MIDI Channel. [1, 16]
     * @param   speedMultiply
     *          A multiplyier to make the control go faster
     * @param   pulsesPerStep
     *          The number of electrical pulses sent out by the encoder on each
     *          physical tick.  
     *          Either NORMAL_ENCODER or JOG, or an arbitrary unsigned integer.
     * @param   mode
     *          The encoding for sending negative values over MIDI.  
     *          Either `#TWOS_COMPLEMENT`, `#BINARY_OFFSET` or `#SIGN_MAGNITUDE`,
     *          or one of the following aliases:
     *          - `#REAPER_RELATIVE_1`
     *          - `#REAPER_RELATIVE_2`
     *          - `#REAPER_RELATIVE_3`
     *          - `#TRACKTION_RELATIVE`
     *          - `#MACKIE_CONTROL_RELATIVE`
     */
    RotaryEncoder(uint8_t pinA, uint8_t pinB, uint8_t controllerNumber,
                  uint8_t channel, int speedMultiply = 1,
                  uint8_t pulsesPerStep = NORMAL_ENCODER,
                  relativeCCmode mode = TWOS_COMPLEMENT)
        : RelativeCCOut(controllerNumber, channel, mode),
          speedMultiply(speedMultiply),
          pulsesPerStep(pulsesPerStep), enc{pinA, pinB} {}

  private:
    // Check if the encoder position has changed since last time, if so, send
    // the relative movement over MIDI
    void refresh() override {
        long currentPosition = enc.read();
        long difference = (currentPosition - previousPosition) / pulsesPerStep;
        if (difference) {
            send(difference * speedMultiply);
            previousPosition += difference * pulsesPerStep;
        }
    }

    const int speedMultiply;
    const uint8_t pulsesPerStep;

    Encoder enc;
    long previousPosition = 0;
};