/*
==========================================
Copyright (c) 2020 Ostrich Labs

The EventQueue and its wrappers
==========================================
*/

#ifndef EVENTQUEUE_H_
#define EVENTQUEUE_H_

#include <deque>
#include <memory>
#include <utility>

#include "i_message.h"

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

    void Push(std::shared_ptr<IMessage> msg);
    std::pair<std::shared_ptr<IMessage>, bool> Pop();

private:

    std::deque<std::shared_ptr<IMessage>> m_MessageQueue;
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

    void Send(std::shared_ptr<IMessage> msg) { if (m_Parent) m_Parent->Push(msg); }

private:

    EventQueue *m_Parent;
};


} // namespace ostrich

#endif /* EVENTQUEUE_H_ */
