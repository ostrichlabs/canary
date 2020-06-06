/*
==========================================
Copyright (c) 2020 Ostrich Labs

The EventQueue and its wrappers
==========================================
*/

#ifndef EVENTQUEUE_H_
#define EVENTQUEUE_H_

#include <queue>
#include <utility>

#include "message.h"

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

    auto getQueueLength() const noexcept { return m_MessageQueue.size(); }

    void Push(const Message &msg);
    std::pair<Message, bool> Pop();

private:

    std::queue<Message> m_MessageQueue;
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

    void Send(const Message &msg) { if (m_Parent) m_Parent->Push(msg); }

private:

    EventQueue *m_Parent;
};


} // namespace ostrich

#endif /* EVENTQUEUE_H_ */
