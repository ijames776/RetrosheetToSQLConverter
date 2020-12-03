#pragma once
#include <vector>
#include <string>

class Play
{
public:
	Play();
	Play(std::vector<std::string> Row);
	~Play();

	int Inning;
	bool isBottomInning;
	std::string BatterID;
	std::string RawPlay;
	std::string Outcome;
	std::string OutcomeType;
	std::string Location;
	std::string Trajectory;

	std::vector<int> Fielders;

	std::string PartOne;
	std::string PartTwo;

	std::string getPlay();

	void ProcessPlay();
	void ProcessPlayOne(std::string PartOne);
	void ProcessPlayTwo(std::string PartTwo);
	void ProcessHit(std::string HitCode);
	std::string GetTrajectory(char BallCode);
	std::string GetFielder(int FielderCode);
	std::string GetLocation(std::string LocationCode, char trajectory);
};

