/*
==========================================
Copyright (c) 2020 Ostrich Labs

Minesweeper state machine

The game state is simple for games, but a little too complicated to put in this file header
Check docs for more details
==========================================
*/

#ifndef MS_STATEMACHINE_H_
#define MS_STATEMACHINE_H_

#include <cstring>
#include <memory>
#include "../common/console.h"
#include "../common/ost_common.h"
#include "../game/eventqueue.h"

namespace ms {

/////////////////////////////////////////////////
//
class StateMachine {
public:

    StateMachine() noexcept : m_isActive(false) { }
    virtual ~StateMachine() { m_isActive = false; }
    StateMachine(StateMachine &&) = delete;
    StateMachine(const StateMachine &) = delete;
    StateMachine &operator=(StateMachine &&) = delete;
    StateMachine &operator=(const StateMachine &) = delete;

    bool isActive() const noexcept { return m_isActive; }

    int Initialize(ostrich::ConsolePrinter consoleprinter, ostrich::EventSender eventsender);

    void Destroy();

    const std::shared_ptr<char *> Serialize();

    void ProcessInput(const ostrich::Message &msg);

    // force a game state update based on current InputState
    void UpdateGameState();

private:

    /////////////////////////////////////////////////
    //
    struct InputStates {

        InputStates() : m_XPos(0), m_YPos(0)
        {
            ::memset(m_Keys, 0, (NUMKEYS * sizeof(m_Keys[0])));
            ::memset(m_MouseButtons, 0, (NUMMOUSEBUTTONS * sizeof(m_MouseButtons[0])));
        }
        ~InputStates() {}

        static const int NUMKEYS = 256;
        bool m_Keys[NUMKEYS];
        static const int NUMMOUSEBUTTONS = 5;
        bool m_MouseButtons[NUMMOUSEBUTTONS];
        int32_t m_XPos;
        int32_t m_YPos;
    };

    const char *const m_Classname = u8"ms::StateMachine";

    bool m_isActive;

    ostrich::ConsolePrinter m_ConsolePrinter;
    ostrich::EventSender m_EventSender;

    InputStates m_InputStates;
};

} // namespace ms

#endif /* MS_STATEMACHINE_H_ */
