/*
==========================================
Copyright (c) 2020 Ostrich Labs

Engine version defines
==========================================
*/

#ifndef OSTRICH_OST_VERSION_H_
#define OSTRICH_OST_VERSION_H_

namespace ostrich {

constexpr char g_EngineName[] = u8"Canary";

namespace version {

/////////////////////////////////////////////////
// Project version defines
// DEV = In development, feature incomplete
// ALPHA = Playable, feature incomplete
// BETA = Playable, mostly feature complete, working on bugs/balance
// RELEASE = Playable, feature complete
constexpr char g_EngineVersion[] = u8"0.01.0001 DEV";
constexpr int g_EngineMajor = 0;
constexpr int g_EngineMinor = 1;
constexpr int g_EngineBuild = 1;

} // namespace version

} // namespace ostrich

namespace ost_version = ostrich::version;

#endif /* OSTRICH_OST_VERSION_H_ */
