/*
==========================================
Copyright (c) 2020 Ostrich Labs
==========================================
*/

#include "eventqueue.h"
#include "errorcodes.h"
#include "../common/filesystem.h"

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::EventQueue::Initialize() {
    ostrich::OpenFile(u8"message.log", ost_filemode::g_WRITETRUNCATE, m_MessageJournal);
    if (!m_MessageJournal.is_open())
        return OST_ERROR_JOURNAL;

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

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::EventQueue::WriteToJournal(const Message &msg) {
    if (!m_MessageJournal.is_open())
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
    m_MessageJournal.write(message.c_str(), message.length());
    m_MessageJournal.put(ost_char::g_NewLine);
}