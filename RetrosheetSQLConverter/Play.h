#pragma once
#include <vector>
#include <string>
#include <tuple>

struct BaserunnerAdvance
{
	bool isSafe = true;
	std::string startingBase;
	std::string endingBase;
	std::vector<char> Fielders;
	std::vector<std::string> ExtraEvents;
};

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
	std::string ErrorType;

	std::vector<int> Fielders;
	std::vector<char> RunnersOut;
	std::vector<BaserunnerAdvance> BaserunnerActivity;

	std::string PlayCode;
	std::string BaserunnerCode;

	std::string getPlay();
	std::string getBaserunnerText();

	void SplitPlayCode();
	void ProcessPlay();
	void ProcessBaserunners();

	void ProcessHit();
	void ProcessHR();
	void ProcessOut();
	void ProcessK();
	void ProcessE();
	void ProcessW();
	void ProcessSB();

	std::string GetTrajectory(char BallCode);
	std::string GetFielder(int FielderCode);
	std::string GetLocation();
};

