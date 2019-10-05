#pragma once

#include <Banks/BankableMIDIOutput.hpp>
#include <Def/Def.hpp>
#include <Hardware/Button.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   An abstract class for momentary push buttons that send MIDI events.
 *
 * The button is debounced.
 *
 * @see     Button
 */
template <class BankAddress, class Sender>
class MIDIButton : public MIDIOutputElement {
  public:
    /**
     * @brief   Construct a new bankable MIDIButton.
     *
     * @param   pin
     *          The digital input pin with the button connected.
     *          The internal pull-up resistor will be enabled.
     */
    MIDIButton(const BankAddress &bankAddress, pin_t pin, const Sender &sender)
        : address{bankAddress}, button{pin}, sender{sender} {}

    void begin() final override { button.begin(); }
    void update() final override {
        Button::State state = button.getState();
        if (state == Button::Falling) {
            address.lock();
            sender.sendOn(address.getActiveAddress());
        } else if (state == Button::Rising) {
            sender.sendOff(address.getActiveAddress());
            address.unlock();
        }
    }

#ifdef INDIVIDUAL_BUTTON_INVERT
    void invert() { button.invert(); }
#endif

  private:
    BankAddress address;
    Button button;

  public:
    Sender sender;
};

} // namespace Bankable

END_CS_NAMESPACE