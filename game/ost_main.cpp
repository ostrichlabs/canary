/*
==========================================
Copyright (c) 2020 Ostrich Labs

Main game object
==========================================
*/

#include "ost_main.h"
#include "ost_version.h"
#include "../common/datetime.h"
#include "../common/error.h"
#include "../common/signal.h"
#include "../game/msg_info.h"
#include "../game/msg_system.h"

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::Main::Main() noexcept :
m_isActive(false), m_Input(nullptr), m_Display(nullptr), m_Renderer(nullptr) {

}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::Main::~Main() {
    if (m_isActive)
        this->Destroy();
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::Main::Start(ostrich::IDisplay *display, ostrich::IRenderer *renderer,
    ostrich::IInput *input) {
    if ((display == nullptr) || (renderer == nullptr) || (input == nullptr))
        throw ostrich::Exception(u8"Catastrophic failure in ostrich::Main::Start()");

    m_Display = display;
    m_Renderer = renderer;
    m_Input = input;

    int initresult = this->Initialize();
    if (initresult == 0) {
        m_isActive = true;
        this->Run();
    }

    return initresult;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::Main::Initialize() {
    m_Console.Initialize();
    m_ConsolePrinter = m_Console.CreatePrinter();

    m_ConsolePrinter.WriteMessage(ostrich::g_GameName);
    m_ConsolePrinter.WriteMessage(u8"Started at: %", { ostrich::datetime::timestamp() });
    m_ConsolePrinter.WriteMessage(ost_version::g_String);
    m_ConsolePrinter.WriteMessage(u8"Platform: %", { ostrich::g_PlatformString });

    int initresult = 0;

    try {

        m_ConsolePrinter.WriteMessage(u8"Initializing Display");
        initresult = m_Display->Initialize(m_Console.CreatePrinter());

        if (initresult == 0) {
            m_ConsolePrinter.WriteMessage(u8"Initializing Renderer");
            initresult = m_Renderer->Initialize(m_Console.CreatePrinter());
        }

        if (initresult == 0) {
            m_ConsolePrinter.WriteMessage(u8"Initializing Event Queue");
            initresult = m_EventQueue.Initialize();
        }

        if (initresult == 0) {
            m_ConsolePrinter.WriteMessage(u8"Initializing signal handler");
            initresult = ostrich::signal::Initialize(m_EventQueue.CreateSender());
        }

        if (initresult == 0) {
            m_ConsolePrinter.WriteMessage(u8"Initializing input handler");
            initresult = m_Input->Initialize(m_Console.CreatePrinter(), m_EventQueue.CreateSender());
        }
    }
    catch (const ostrich::ProxyException &e) {
        m_ConsolePrinter.WriteMessage(u8"% at %", { e.what(), e.where() });
        throw e;
    }
    catch (const ostrich::InitException &e) {
        m_ConsolePrinter.WriteMessage(u8"% at %, reported error code %", { e.what(), e.where(), std::to_string(e.code()) });
        throw e;
    }
    catch (const ostrich::Exception &e) {
        m_ConsolePrinter.WriteMessage(u8"% from unknown location", { e.what() });
        throw e;
    }

    if (initresult == 0)
        m_ConsolePrinter.WriteMessage(u8"Initialization complete");
    else
        m_ConsolePrinter.WriteMessage(u8"Bizzare initialization failure, code: %", { std::to_string(initresult) });

    return initresult;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::Main::Destroy() {
    if (m_isActive) {
        m_Console.WriteMessage(u8"Shutting down...");
        if (m_Input)
            m_Input->Destroy();
        if (m_Renderer)
            m_Renderer->Destroy();
        if (m_Display)
            m_Display->Destroy();
        m_Console.Destroy();
        m_isActive = false;
    }
}

/////////////////////////////////////////////////
// game loop
/////////////////////////////////////////////////
void ostrich::Main::Run() {
    int32_t lag = 0;
    const double updatespersec = 60.0;
    const int32_t msperupdate = static_cast<int32_t>((1.0 / updatespersec) * 1000.0);

    auto prevtick = ostrich::timer::now();
    bool done = false;
    while (!done) {
        auto currtick = ostrich::timer::now();
        int32_t elapsedtime = ostrich::timer::interval(prevtick, currtick);
        prevtick = currtick;
        lag += elapsedtime;
        this->ProcessInput();
        while ((lag >= msperupdate) && (!done)) { // no need to update state if done
            done = this->UpdateState();
            lag -= msperupdate;
        }
        this->RenderScene(lag / msperupdate);
    }
}

/////////////////////////////////////////////////
// collects key/mouse input from the EventQueue
/////////////////////////////////////////////////
void ostrich::Main::ProcessInput() {
    if (m_Input) {
        m_Input->ProcessOSMessages();
        m_Input->ProcessKBM();
    }
}

/////////////////////////////////////////////////
// sends events to state manager to update game state
// returns true if the game should stop running
/////////////////////////////////////////////////
bool ostrich::Main::UpdateState() {
    if (m_EventQueue.isPending()) {
        std::pair<std::shared_ptr<IMessage>, bool> queuemsg = m_EventQueue.Pop();
        if (queuemsg.second == true) {
            auto msgptr = queuemsg.first;
            if (msgptr->getMessageType() == ostrich::MessageType::MSG_INFO) {
                auto infomsg = std::static_pointer_cast<ostrich::InfoMessage>(msgptr);
                m_ConsolePrinter.WriteMessage(infomsg->toString());
            }
            else if (msgptr->getMessageType() == ostrich::MessageType::MSG_SYSTEM) {
                auto sysmsg = std::static_pointer_cast<ostrich::SystemMessage>(msgptr);
                if (sysmsg->getType() == ostrich::SystemMsgType::SYS_QUIT) {
                    m_ConsolePrinter.WriteMessage(u8"SYS_QUIT received");
                    return true;
                }
            }
            else {
                m_ConsolePrinter.WriteMessage(u8"Unhandled message type % sent by %",
                    { std::to_string(static_cast<int32_t>(msgptr->getMessageType())),
                      std::string(msgptr->getSenderMethod()) });
            }
        }
    }
    return false;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::Main::RenderScene(int32_t extrapolation) {
    UNUSED_PARAMETER(extrapolation);
    if (m_Renderer && m_Display) {
        m_Renderer->RenderScene();
        m_Display->SwapBuffers();
    }
}
