/*
==========================================
Copyright (c) 2020 Ostrich Labs

Minesweeper state machine
==========================================
*/

#include "ms_statemachine.h"
#include "../common/error.h"
#include "../game/msg_system.h"

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ms::StateMachine::Initialize(ostrich::ConsolePrinter consoleprinter, ostrich::EventSender eventsender) {
    m_ConsolePrinter = consoleprinter;
    m_EventSender = eventsender;

    if (!m_ConsolePrinter.isValid() || !m_EventSender.isValid())
        throw ostrich::ProxyException(OST_FUNCTION_SIGNATURE);

    // any initialization of game-specific state should go here

    return 0;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ms::StateMachine::Destroy() {

}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
const std::shared_ptr<char *> ms::StateMachine::Serialize() {
    return nullptr;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ms::StateMachine::ProcessInput(std::shared_ptr<ostrich::InputMessage> msg) {
    if (!msg) {
        return; // probably should do some actual error handling here
    }

    if (msg->getType() == ostrich::KeyType::KEYTYPE_KB) {
        m_InputStates.m_Keys[msg->getKey()] = msg->isKeyDown();
    }
    else if (msg->getType() == ostrich::KeyType::KEYTYPE_MOUSE) {
        m_InputStates.m_XPos = msg->getXPos();
        m_InputStates.m_YPos = msg->getYPos();
        m_InputStates.m_MouseButtons[0] = (msg->getKey() & OST_MOUSE_LBUTTON);
        m_InputStates.m_MouseButtons[1] = (msg->getKey() & OST_MOUSE_RBUTTON);
        m_InputStates.m_MouseButtons[2] = (msg->getKey() & OST_MOUSE_MBUTTON);
    }
    else {
        m_ConsolePrinter.WriteMessage(u8"Unknown input type passed to StateMachine: %",
            { std::to_string(static_cast<int32_t>(msg->getType())) });
        return;
    }

    this->UpdateGameState();
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ms::StateMachine::UpdateGameState() {
    if (m_InputStates.m_Keys[int(u8' ')])
        m_EventSender.Send(ostrich::SystemMessage::Construct(ostrich::SystemMsgType::SYS_QUIT, u8"ms::StateMachine::UpdateGameState()"));
}