/*
==========================================
Copyright (c) 2020 Ostrich Labs
==========================================
*/

#include "eventqueue.h"


/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::EventQueue::Initialize() {
    return 0;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::EventQueue::Push(const Message &msg) {
    m_MessageQueue.push(msg);
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
std::pair<ostrich::Message, bool> ostrich::EventQueue::Pop() {
    if (!m_MessageQueue.empty()) {
        ostrich::Message msg = m_MessageQueue.front();
        m_MessageQueue.pop();
        return std::make_pair(msg, true);
    }
    return std::make_pair(ostrich::Message::CreateNullMessage(), false);
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::EventSender ostrich::EventQueue::CreateSender() noexcept {
    return ostrich::EventSender(this);
}
