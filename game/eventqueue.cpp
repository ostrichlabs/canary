/*
==========================================
ost_events.cpp

Copyright (c) 2020 Ostrich Labs
==========================================
*/

#include "ost_events.h"


/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::EventQueue::Initialize() {
    return 0;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::EventQueue::Push(const ostrich::Message &msg) {
    m_MessageQueue.push_back(msg);
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
std::pair<ostrich::Message,bool> ostrich::EventQueue::Pop() {
    if (!m_MessageQueue.empty()) {
        ostrich::Message msg = m_MessageQueue.front();
        m_MessageQueue.pop_front();
        return { msg, true };
    }
    return { ostrich::Message(ostrich::MessageType::MSG_NULL, ostrich::SubMessageType::MSG_NULL, -1, u8"ostrich::EventQueue"), false };
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::EventSender ostrich::EventQueue::CreateSender() noexcept {
    return ostrich::EventSender(this);
}
