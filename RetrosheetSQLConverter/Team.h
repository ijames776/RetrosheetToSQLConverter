#pragma once
#include "string";
#include "vector";
#include "map";
#include "Player.h";

class Team
{
public:
	Team();
	~Team();

	std::string Abbreviation;
	std::map<std::string, Player> Roster;
	std::map<std::string, Player*> Lineup;


private:
};

