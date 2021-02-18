/*
==========================================
Copyright (c) 2020-2021 Ostrich Labs

The EventQueue and its wrappers
==========================================
*/

#ifndef OSTRICH_EVENTQUEUE_H_
#define OSTRICH_EVENTQUEUE_H_

#include <queue>
#include <utility>
#include "message.h"
#include "../common/filesystem.h"

namespace ostrich {

/////////////////////////////////////////////////
// forward declarations
class EventSender;

/////////////////////////////////////////////////
//
class EventQueue {
public:

    /////////////////////////////////////////////////
    // Constructor is effectively default; all data has default constructors
    // Destructor can do nothing because all data has its own destructors
    // Copy/move constructors/operators are deleted for performance reasons
    EventQueue() noexcept { }
    virtual ~EventQueue() { }
    EventQueue(EventQueue &&) = delete;
    EventQueue(const EventQueue &) = delete;
    EventQueue &operator=(EventQueue &&) = delete;
    EventQueue &operator=(const EventQueue &) = delete;

    /////////////////////////////////////////////////
    // Initialize the object
    // Any open files or data will be destroyed first
    //
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    int Initialize();

    /////////////////////////////////////////////////
    // Create an EventSender object using this
    //
    // returns:
    //      An EventSender object containing a pointer to the current object
    EventSender CreateSender() noexcept;

    /////////////////////////////////////////////////
    // Check if there are any pending messages in the queue.
    //
    // returns:
    //      true if there is one or more messages in the queue
    bool isPending() const noexcept { return !m_MessageQueue.empty(); }

    /////////////////////////////////////////////////
    // Get the number of messages in the queue
    //
    // returns:
    //      The number of messages in the queue; return type depends on internal representation of the queue
    auto getQueueLength() const noexcept { return m_MessageQueue.size(); }

    /////////////////////////////////////////////////
    // Add a new Message to the back of the queue
    //
    // in:
    //      msg - a reference to a Message to push
    // returns:
    //      void
    void Push(const Message &msg);

    /////////////////////////////////////////////////
    // Pop a Message from the front of the queue and remove it
    // Maybe removing the message should be separate, or there should be a Peek(), but for now this works
    //
    // returns:
    //      A pair containing a Message object and a flag which is false if there is no message in the queue at the time of the call
    std::pair<Message, bool> Pop();

private:

    /////////////////////////////////////////////////
    // Writes details about a Message to a message log for debugging/auditing purposes
    //
    // in:
    //      msg - a Message object to write
    // returns:
    //      void
    void WriteToJournal(const Message &msg);

    std::queue<Message> m_MessageQueue;
    ostrich::File m_MessageJournal;
};

/////////////////////////////////////////////////
// A proxy class only allowing adding a message to an EventQueue
class EventSender {
public:

    /////////////////////////////////////////////////
    // Default constructor creates a broken object
    // Destructor can do nothing because data is not owned by the object
    // Copy/move constructors/operators are default; there's no harm in allowing it
    EventSender() noexcept : m_Parent(nullptr) {}
    virtual ~EventSender() {}
    EventSender(EventSender &&) = default;
    EventSender(const EventSender &) = default;
    EventSender &operator=(EventSender &&) = default;
    EventSender &operator=(const EventSender &) = default;

    /////////////////////////////////////////////////
    // Explicit constructor attaching an EventQueue
    explicit EventSender(EventQueue *parent) noexcept : m_Parent(parent) {}

    /////////////////////////////////////////////////
    // Mimics the explicit constructor
    // Allows changing the target of the object - though I don't precisely know why you'd do this
    //
    // in:
    //      parent - A pointer to an EventQueue object
    // returns:
    //      true/false depending on whether the passed pointer is null
    bool AttachParent(EventQueue *parent) noexcept { m_Parent = parent; return this->isValid(); }

    /////////////////////////////////////////////////
    // Check if the object has a valid EventQueue pointer
    //
    // returns:
    //      true/false depending on whether the EventQueue pointer is null
    bool isValid() const noexcept { if (m_Parent) return true; return false; }

    /////////////////////////////////////////////////
    // Add a new Message to the back of the queue
    //
    // in:
    //      msg - a reference to a Message to push
    // returns:
    //      void
    void Send(const Message &msg) { if (m_Parent) m_Parent->Push(msg); }

private:

    EventQueue *m_Parent;
};


} // namespace ostrich

#endif /* OSTRICH_EVENTQUEUE_H_ */
