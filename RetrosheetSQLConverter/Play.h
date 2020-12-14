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
	std::string advanceType;
};

struct Error
{
	std::string ErrorType;
	std::string Fielder;
	int BaseThrownTo;
};

class Play
{
public:
	Play();
	Play(std::vector<std::string> Row);
	~Play();

	int Inning;
	bool isBottomInning;

	std::string PlayType = "";

	std::vector<Error> Errors;
	std::string BatterID;
	std::string RawPlay;
	std::string Outcome;
	std::string OutcomeType;
	std::string Fielder;
	std::string Location;
	std::string Trajectory;

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
	void ProcessFieldOut();
	void ProcessFC();
	void ProcessK();
	void ProcessError(bool mainPlay);
	void ProcessW();
	void ProcessSB(bool mainPlay);
	void ProcessCS(bool mainPlay);
	void ProcessPO(bool mainPlay);

	void AddFielders();

	void GetTrajectory();
	std::string GetFielder(int FielderCode);
	std::string GetLocation();
	char OutBase;

private:
	void ReducePlayCode(int numChars);
};

