/*
==========================================
Copyright (c) 2020-2021 Ostrich Labs
==========================================
*/

#include "eventqueue.h"
#include "errorcodes.h"
#include "../common/filesystem.h"

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::EventQueue::Initialize() {
    if (!m_MessageJournal.Open(u8"message.log", ostrich::FileMode::OPEN_WRITETRUNCATE)) {
        return OST_ERROR_JOURNAL;
    }

    return OST_ERROR_OK;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::EventQueue::Push(const Message &msg) {
    m_MessageQueue.push(msg);
    this->WriteToJournal(msg);
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
std::optional<ostrich::Message> ostrich::EventQueue::Pop() {
    if (!m_MessageQueue.empty()) {
        ostrich::Message msg = m_MessageQueue.front();
        m_MessageQueue.pop();
        return std::make_optional(msg);
    }
    return std::nullopt;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::EventSender ostrich::EventQueue::CreateSender() noexcept {
    return ostrich::EventSender(this);
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::EventQueue::WriteToJournal(const Message &msg) {
    if (!m_MessageJournal.isOpen())
        return;

    std::string message = u8"[";
    message += msg.getTimestamp();
    message += u8"] Message type >";
    message += std::to_string(msg.getTypeAsInt());
    message += u8"< Data1: >";
    message += std::to_string(msg.getData1());
    message += u8"< Data2: >";
    message += std::to_string(msg.getData2());
    message += u8"< Sender: >";
    message += msg.getSender();
    message += u8"<";
    std::fstream &handle = m_MessageJournal.getFStream();
    handle.write(message.c_str(), message.length());
    handle.put(ost_char::g_NewLine);
}