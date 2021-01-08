/*
==========================================
Copyright (c) 2020 Ostrich Labs
==========================================
*/

#include "ost_main.h"
#include <csignal>
#include "ost_version.h"
#include "../common/datetime.h"
#include "../common/error.h"
#include "../game/errorcodes.h"
#include "../game/message.h"

// game-specific includes
#include "../minesweeper/ms_common.h"


/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::Main::Main() noexcept :
m_isActive(false), m_Input(nullptr), m_Display(nullptr), m_Renderer(nullptr) {

}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::Main::~Main() {
    //this->Destroy();
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

    m_ConsolePrinter.WriteMessage(u8"Running % version %", { ostrich::g_EngineName, ostrich::version::g_EngineVersion });
    m_ConsolePrinter.WriteMessage(u8"Platform: %", { ostrich::g_PlatformString });
    m_ConsolePrinter.WriteMessage(u8"Started at: %", { ostrich::datetime::timestamp() });

    int initresult = OST_ERROR_OK;

    auto start = ostrich::timer::now();

    try {

        if (initresult == OST_ERROR_OK) {
            m_ConsolePrinter.WriteMessage(u8"Initializing Event Queue");
            initresult = m_EventQueue.Initialize();
        }

        if (initresult == OST_ERROR_OK) {
            m_ConsolePrinter.WriteMessage(u8"Initializing Display");
            initresult = m_Display->Initialize(m_Console.CreatePrinter());
        }

        if (initresult == OST_ERROR_OK) {
            m_ConsolePrinter.WriteMessage(u8"Initializing Renderer");
            initresult = m_Renderer->Initialize(m_Console.CreatePrinter());
        }

        if (initresult == OST_ERROR_OK) {
            m_ConsolePrinter.WriteMessage(u8"Initializing Input Handler");
            initresult = m_Input->Initialize(m_Console.CreatePrinter(), m_EventQueue.CreateSender());
        }

        if (initresult == OST_ERROR_OK) {
            m_ConsolePrinter.WriteMessage(u8"Initializing State Machine");
            initresult = m_GameState.Initialize(m_Console.CreatePrinter(), m_EventQueue.CreateSender());
        }
    }
    catch (const ostrich::ProxyException &e) {
        m_ConsolePrinter.WriteMessage(u8"% at %", { e.what(), e.where() });
        initresult = OST_ERROR_EXCEPTPROXY;
    }
    catch (const ostrich::InitException &e) {
        m_ConsolePrinter.WriteMessage(u8"% at %, our error code %", { e.what(), e.where(), std::to_string(e.getOurCode()) });
        if (e.hasReturnedCode()) {
            m_ConsolePrinter.WriteMessage(u8"Additional code %", { std::to_string(e.getReturnedCode()) });
        }
        initresult = OST_ERROR_EXCEPTINIT;
    }
    catch (const ostrich::Exception &e) {
        m_ConsolePrinter.WriteMessage(u8"% from unknown location", { e.what() });
        initresult = OST_ERROR_EXCEPTGENERIC;
    }
    catch (const std::exception &e) {
        m_ConsolePrinter.WriteMessage(u8"std::exception: %", { e.what() });
        initresult = OST_ERROR_EXCEPTCPP;
    }

    if (initresult == OST_ERROR_OK) {
        auto finish = ostrich::timer::now();
        int32_t duration = ostrich::timer::interval(start, finish);
        m_ConsolePrinter.WriteMessage(u8"Initialization complete in % milliseconds", { std::to_string(duration) });
    }
    else {
        m_ConsolePrinter.WriteMessage(u8"Bizzare initialization failure, code: %", { std::to_string(initresult) });
    }

    return initresult;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::Main::Destroy() {
    if (m_isActive) {
        m_Console.WriteMessage(u8"Shutting down...");
        if (m_Input) {
            m_Input->Destroy();
            m_Input = nullptr;
        }
        if (m_Renderer) {
            m_Renderer->Destroy();
            m_Renderer = nullptr;
        }
        if (m_Display) {
            m_Display->Destroy();
            m_Display = nullptr;
        }
        m_Console.Destroy();
        m_isActive = false;
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::Main::Run() {
    int32_t lag = 0;
    const double updatespersec = 60.0;
    const int32_t msperupdate = static_cast<int32_t>((1.0 / updatespersec) * 1000.0);

    auto prevtick = ostrich::timer::now();
    auto currtick = prevtick;
    bool done = false;
    int32_t elapsedtime = 0;
    double fps = 0.0;
    while (!done) {
        currtick = ostrich::timer::now();
        elapsedtime = ostrich::timer::interval(prevtick, currtick);
        prevtick = currtick;
        lag += elapsedtime;

        if (elapsedtime > 0)
            fps = static_cast<double>(1.0 / static_cast<double>(elapsedtime)) * 1000.0;

        this->ProcessInput();
        while ((lag >= msperupdate) && (!done)) { // no need to update state if done
            done = this->UpdateState();
            lag -= msperupdate;
        }
        this->RenderScene(lag / msperupdate);
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::Main::ProcessInput() {
    if (m_Input) {
        m_Input->ProcessOSMessages();
        m_Input->ProcessKBM();
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool ostrich::Main::UpdateState() {
    while (m_EventQueue.isPending()) {
        auto queuemsg = m_EventQueue.Pop();
        if (queuemsg.second == true) {
            ostrich::Message msg = queuemsg.first;
            if ((msg.getType() >= ostrich::Message::Type::INPUT_START) &&
                (msg.getType() <= ostrich::Message::Type::INPUT_LAST)) {
                // update state based on input
                m_GameState.ProcessInput(msg);
            }
            else if (msg.getType() == ostrich::Message::Type::SYSTEM) {
                // process system-type messages
                return this->ProcessSystemMessage(msg);
            }
            else {
                m_ConsolePrinter.WriteMessage(u8"Unhandled message type % sent by %",
                    { std::to_string(msg.getTypeAsInt()),
                      msg.getSender() });
            }
        }
    }

    return false;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool ostrich::Main::ProcessSystemMessage(const ostrich::Message &msg) {
    switch (msg.getSystemCode()) {
        case OST_SYSTEMMSG_QUIT:
        {
            m_ConsolePrinter.DebugMessage(u8"Shutdown received from %",
                { msg.getSender() });
            return true;
        }
        case OST_SYSTEMMSG_SIGNAL:
        {
            int32_t signal = msg.getSystemAddlData();
            m_ConsolePrinter.DebugMessage(u8"Signal raised: %",
                { std::to_string(signal) });
            if ((signal == SIGINT) || (signal == SIGTERM) || (signal == SIGHUP)) {
                return true;
            }
            break;
        }
        case OST_SYSTEMMSG_NULL:
        default:
        {
            m_ConsolePrinter.DebugMessage(u8"Unknown system message %",
                { std::to_string(msg.getSystemCode()) });
        }
    }

    return false;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::Main::RenderScene(int32_t extrapolation) {
    UNUSED_PARAMETER(extrapolation);
    if (m_Renderer && m_Display) {
        auto scenedata = m_GameState.GetSceneData();
        if (scenedata == nullptr) {

        }
        else {
            m_Renderer->RenderScene(scenedata, extrapolation);
            m_Display->SwapBuffers();
        }
    }
}
