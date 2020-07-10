/*
==========================================
Copyright (c) 2020 Ostrich Labs

Minesweeper state machine
==========================================
*/

#include "ms_statemachine.h"
#include "../common/error.h"

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ms::StateMachine::Initialize(ostrich::ConsolePrinter consoleprinter, ostrich::EventSender eventsender) {
    m_ConsolePrinter = consoleprinter;
    m_EventSender = eventsender;

    if (!m_ConsolePrinter.isValid() || !m_EventSender.isValid())
        throw ostrich::ProxyException(OST_FUNCTION_SIGNATURE);

    // any initialization of game-specific state should go here

    m_isActive = true;
    return 0;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ms::StateMachine::Destroy() {
    m_isActive = false;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
const std::shared_ptr<char *> ms::StateMachine::Serialize() {
    return nullptr;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ms::StateMachine::ProcessInput(const ostrich::Message &msg) {
    if (!m_isActive) {
        return; // probably should do some actual error handling here
    }

    if (msg.getType() == ostrich::Message::Type::INPUT_KEY) {
        auto keydata = msg.getKeyStatus();
        m_InputStates.m_Keys[keydata.first] = keydata.second;
    }
    else if (msg.getType() == ostrich::Message::Type::INPUT_BUTTON) {
        int32_t buttons = msg.getButtonStatus();
        m_InputStates.m_MouseButtons[0] = (buttons & ostrich::Message::MOUSE_LBUTTON);
        m_InputStates.m_MouseButtons[1] = (buttons & ostrich::Message::MOUSE_RBUTTON);
        m_InputStates.m_MouseButtons[2] = (buttons & ostrich::Message::MOUSE_MBUTTON);
    }
    else if (msg.getType() == ostrich::Message::Type::INPUT_MOUSEPOS) {
        auto posdata = msg.getMouseCoords();
        m_InputStates.m_XPos = posdata.first;
        m_InputStates.m_YPos = posdata.second;
    }
    else {
        m_ConsolePrinter.WriteMessage(u8"Unknown input type passed to StateMachine: %",
            { std::to_string(msg.getTypeAsInt()) });
        return;
    }

    this->UpdateGameState();
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ms::StateMachine::UpdateGameState() {
    if (m_isActive) {
        if (m_InputStates.m_Keys[int(u8' ')])
            m_EventSender.Send(ostrich::Message::CreateSystemMessage(OST_SYSTEMMSG_QUIT, 0, m_Classname));
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
const ostrich::SceneData *ms::StateMachine::GetSceneData() const noexcept {
    return &m_SceneData;
}