/*
==========================================
ost_version.h

Copyright (c) 2020 Ostrich Labs

Game/engine version defines
==========================================
*/

#ifndef OST_VERSION_H_
#define OST_VERSION_H_

namespace ostrich {

constexpr char g_GameName[] = u8"Canary";

namespace version {

/////////////////////////////////////////////////
// Project version defines
// DEV = In development, feature incomplete
// ALPHA = Playable, feature incomplete
// BETA = Playable, mostly feature complete, working on bugs/balance
// RELEASE = Playable, feature complete
constexpr char g_String[] = u8"0.01.0001 DEV";
constexpr int g_Major = 0;
constexpr int g_Minor = 1;
constexpr int g_Build = 1;

} // namespace version

} // namespace ostrich

namespace ost_version = ostrich::version;

#endif /* OST_VERSION_H_ */
