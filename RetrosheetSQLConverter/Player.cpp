#include "Player.h"

Player::Player()
{
}

Player::Player(std::vector<std::string> DataRow)
{
	PlayerID = DataRow[1];
	for (int i = 0; i < DataRow[2].size(); i++)
	{
		if (DataRow[2][i] == ' ') { FirstName = DataRow[2].substr(0, i); LastName = DataRow[2].substr(i + 1); }
	}
	BattingOrder = DataRow[3];
	Position = DataRow[4];
}

Player::~Player()
{
}
