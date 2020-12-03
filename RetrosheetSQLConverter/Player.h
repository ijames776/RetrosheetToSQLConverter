#pragma once
#include <string>
#include <vector>

class Player
{
public:
	Player();
	Player(std::vector<std::string> DataRow);
	~Player();

	std::string PlayerID;
	std::string FirstName;
	std::string LastName;
	std::string BattingOrder;
	std::string Position;

private:
};

