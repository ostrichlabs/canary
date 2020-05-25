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
void ostrich::EventQueue::Push(std::shared_ptr<IMessage> msg) {
    m_MessageQueue.push_back(msg);
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
std::pair<std::shared_ptr<ostrich::IMessage>, bool> ostrich::EventQueue::Pop() {
    if (!m_MessageQueue.empty()) {
        std::shared_ptr<IMessage> msg = m_MessageQueue.front();
        m_MessageQueue.pop_front();
        return { msg, true };
    }
    return { nullptr, false };
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::EventSender ostrich::EventQueue::CreateSender() noexcept {
    return ostrich::EventSender(this);
}
