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
	std::string Fielder;
	std::string Location;
	std::string Trajectory;

	std::vector<int> Fielders;
	std::vector<char> RunnersOut;

	std::string PlayCode;
	std::string BaserunnerCode;

	std::string getPlay();

	void SplitPlayCode();
	void ProcessPlay();
	void ProcessBaserunners(std::string PartTwo);

	void ProcessHit();
	void ProcessOut();

	std::string GetTrajectory(char BallCode);
	std::string GetFielder(int FielderCode);
	std::string GetLocation(std::string LocationCode);
};

