/*
==========================================
Copyright (c) 2020 Ostrich Labs

Minesweeper state machine

The game state is simple for games, but a little too complicated to put in this file header
Check docs for more details
==========================================
*/

#ifndef MS_STATEMACHINE_H_
#define MS_STATEMACHINE_H_

#include <cstring>
#include <list>
#include <memory>
#include <vector>
#include "../common/console.h"
#include "../common/ost_common.h"
#include "../game/eventqueue.h"
#include "../game/scenedata.h"

namespace ms {

/////////////////////////////////////////////////
//
class StateMachine {
public:

    StateMachine() noexcept : m_isActive(false) { }
    virtual ~StateMachine() { m_isActive = false; }
    StateMachine(StateMachine &&) = delete;
    StateMachine(const StateMachine &) = delete;
    StateMachine &operator=(StateMachine &&) = delete;
    StateMachine &operator=(const StateMachine &) = delete;

    bool isActive() const noexcept { return m_isActive; }

    int Initialize(ostrich::ConsolePrinter consoleprinter, ostrich::EventSender eventsender);

    void Destroy();

    const std::shared_ptr<char *> Serialize();

    void ProcessInput(const ostrich::Message &msg);

    // force a game state update based on current InputState
    void UpdateGameState();

    // returns a pointer to scene data for the renderer
    // this should be a curated list taken from the master list of entities
    const ostrich::SceneData *GetSceneData() const noexcept;

private:

    /////////////////////////////////////////////////
    //
    struct InputStates {

        InputStates() : m_XPos(0), m_YPos(0)
        {
            ::memset(m_Keys, 0, (NUMKEYS * sizeof(m_Keys[0])));
            ::memset(m_MouseButtons, 0, (NUMMOUSEBUTTONS * sizeof(m_MouseButtons[0])));
        }
        ~InputStates() {}

        static const int NUMKEYS = 256;
        bool m_Keys[NUMKEYS];
        static const int NUMMOUSEBUTTONS = 5;
        bool m_MouseButtons[NUMMOUSEBUTTONS];
        int32_t m_XPos;
        int32_t m_YPos;
    };

    const char *const m_Classname = u8"ms::StateMachine";

    bool m_isActive;

    ostrich::ConsolePrinter m_ConsolePrinter;
    ostrich::EventSender m_EventSender;

    InputStates m_InputStates;

    ostrich::SceneData m_SceneData;

    std::list<ostrich::IEntity> m_MasterEntityList;

    /////////////////////////////////////////////////
    // all of the above state could be considered generic stuff for any game
    // the below is specific to Minesweeper
    /////////////////////////////////////////////////

    enum class TileState : int32_t {
        STATE_UNREVEALED,   // state is unknown to player
        STATE_REVEALED,     // state is known to the player
        STATE_FLAGGED       // player planted a flag on the tile
    };

    struct Tile {
        TileState m_State;
        bool m_isAMine;
    };

    // board represented by a single-dimension vector of power-of-two size
    std::vector<ostrich::IEntity> m_Board;
};

} // namespace ms

#endif /* MS_STATEMACHINE_H_ */
