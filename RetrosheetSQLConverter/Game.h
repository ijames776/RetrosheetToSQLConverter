#pragma once
#include <vector>
#include <string>
#include "Team.h"
#include "Play.h"

class Game
{
public:
	Game();
	~Game();

	std::vector<std::vector<std::string>> RawData;
	std::vector<Play> Plays;

	int Inning = 0;
	bool isBottomInning = 0;

	Team Teams[2];

	std::string GameDate;
	std::string GameNumber;
	std::string WinningPitcherID;
	std::string LosingPitcherID;
	std::string SavePitcherID;

	std::map<int, Player*> Bases = { {1, nullptr}, {2, nullptr}, {3, nullptr} };

	void UpdateInning(int Inning, bool isBottomInning);
};