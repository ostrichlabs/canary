/*
==========================================
ost_events.h

Copyright (c) 2019 Ostrich Labs

The EventQueue and its wrappers
==========================================
*/

#ifndef OST_EVENTS_H_
#define OST_EVENTS_H_

#include <deque>
#include <utility>

#include "ost_message.h"

namespace ostrich {

/////////////////////////////////////////////////
// forward declarations
class EventSender;

/////////////////////////////////////////////////
//
class EventQueue {
public:

    EventQueue() noexcept {}
    virtual ~EventQueue() {}

    int Initialize();

    EventSender CreateSender() noexcept;

    bool isPending() const noexcept { return !m_MessageQueue.empty(); }

    void Push(const Message &msg);
    std::pair<Message,bool> Pop();

private:

    std::deque<Message> m_MessageQueue;
};

/////////////////////////////////////////////////
//
class EventSender {
public:

    EventSender() noexcept : m_Parent(nullptr) {}
    virtual ~EventSender() {}
    EventSender(EventSender &&) = default;
    EventSender(const EventSender &) = default;
    EventSender &operator=(EventSender &&) = default;
    EventSender &operator=(const EventSender &) = default;

    explicit EventSender(EventQueue *parent) noexcept : m_Parent(parent) {}

    bool AttachParent(EventQueue *parent) noexcept { m_Parent = parent; return this->isValid(); }

    bool isValid() const noexcept { if (m_Parent) return true; return false; }

    void Push(const Message &msg) { if (m_Parent) m_Parent->Push(msg); }

private:

    EventQueue *m_Parent;
};


} // namespace ostrich

#endif /* OST_EVENTS_H_ */
