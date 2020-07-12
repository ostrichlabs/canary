/*
==========================================
Copyright (c) 2020 Ostrich Labs

Common definitions
==========================================
*/

#ifndef MS_COMMON_H_
#define MS_COMMON_H_

namespace ms {

constexpr char g_GameName[] = u8"Minesweeper";

namespace version {

/////////////////////////////////////////////////
// Project version defines
// DEV = In development, feature incomplete
// ALPHA = Playable, feature incomplete
// BETA = Playable, mostly feature complete, working on bugs/balance
// RELEASE = Playable, feature complete
constexpr char g_Version[] = u8"0.01.0001 DEV";
constexpr int g_Major = 0;
constexpr int g_Minor = 1;
constexpr int g_Build = 1;

} // namespace version

} // namespace ms

#endif /* MS_COMMON_H_ */