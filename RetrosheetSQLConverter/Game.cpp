#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::UpdateInning(int Inning, bool isBottomInning)
{
	if (this->Inning != Inning || this->isBottomInning != isBottomInning)
	{
		this->Inning = Inning;
		this->isBottomInning = isBottomInning;

		Bases[1] = nullptr;
		Bases[2] = nullptr;
		Bases[3] = nullptr;
	}
}
