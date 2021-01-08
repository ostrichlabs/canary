/*
==========================================
Copyright (c) 2020 Ostrich Labs

Interface for retrieving input information from Windows
==========================================
*/

#ifndef OSTRICH_WIN_INPUT_H_
#define OSTRICH_WIN_INPUT_H_

#include "../game/eventqueue.h"
#include "../game/i_input.h"

namespace ostrich {

namespace windows {

/////////////////////////////////////////////////
// Translates VK_ values into internal/UTF-8 equivalents
// There are some questions about international keyboards for some keys
// But I will cross that bridge when I get to it
//
// in:
//      vkey - a Windows virtual key value
// returns:
//      an ostrich virtual key
int32_t TranslateKey(int32_t vkey);

} // namespace windows

/////////////////////////////////////////////////
//
class InputWindows : public IInput {
public:

    /////////////////////////////////////////////////
    // Constructor is simple.
    // Destructor is defined, but all it does is set m_isActive to false so I dunno how necessary it is.
    // Copy/move constructors/operators are deleted to prevent accidentally creating two input handlers
    // (if you want to make another, you can do it manually)
    InputWindows() noexcept : m_isActive(false) {}
    virtual ~InputWindows() { m_isActive = false; }
    InputWindows(InputWindows &&) = delete;
    InputWindows(const InputWindows &) = delete;
    InputWindows &operator=(InputWindows &&) = delete;
    InputWindows &operator=(const InputWindows &) = delete;

    /////////////////////////////////////////////////
    // Initialize the input handler.
    // Since Windows input almost entirely comes from Windows messages, this doesn't have to do much.
    //
    // in:
    //      consoleprinter - an initialized ConsolePrinter for logging
    //      eventsender - an initialized EventSender for passing input data to the queue
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    int Initialize(ConsolePrinter consoleprinter, EventSender eventsender) override;

    /////////////////////////////////////////////////
    // Cleans up input setup.
    // For Windows, this is releasing the mouse and little else.
    //
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    void Destroy() override;

    /////////////////////////////////////////////////
    // Check if the object is valid (by checking the m_isActive flag).
    // The flag should be false by default & after Destroy() and true after Initialize()
    //
    // returns:
    //      m_isActive flag
    bool isActive() const noexcept override { return m_isActive; }

    /////////////////////////////////////////////////
    // Process any keyboard/mouse input.
    // For Windows, this polls the mouse position and nothing else.
    //
    // returns:
    //      void
    void ProcessKBM() override;

    /////////////////////////////////////////////////
    // Process any miscellaneous messages from the OS.
    // Runs the Windows message pump, which includes most (but not all) KBM data.
    //
    // returns:
    //      void
    void ProcessOSMessages() override;

private:

    const char * const m_Classname = u8"ostrich::InputWindows";

    ConsolePrinter m_ConsolePrinter;
    EventSender m_EventSender;

    bool m_isActive;
};

} // namespace ostrich

#endif /* OSTRICH_WIN_INPUT_H */
