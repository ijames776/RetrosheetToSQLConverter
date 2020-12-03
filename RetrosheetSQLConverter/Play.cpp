#include "Play.h"

Play::Play()
{
}

Play::Play(std::vector<std::string> Row)
{
	Inning = std::stoi(Row[1]);
	isBottomInning = std::stoi(Row[2]);
	BatterID = Row[3];
	//Row[4] = count
	//Row[5] = pitches
	RawPlay = Row[6];

	//Outcome = ProcessPlayOne();
	ProcessPlay();
}

Play::~Play()
{
}




std::string Play::getPlay()
{
	if (OutcomeType == "Hit") { return "Hits a " + Outcome + " on a " + Trajectory + " | " + PartTwo; }
	return Outcome + " - " + PartOne + " | " + PartTwo;
}

void Play::ProcessPlay()
{
	std::string PlayCode = RawPlay;
	//std::string PartOne; // Type of hit or fielder making play
	//std::string PartTwo; // Trajectory of hit and location fielded

	int PeriodIndex = -1;
	for (int i = 0; i < RawPlay.size(); i++) { if (RawPlay[i] == '.') { PeriodIndex = i; break; } }

	if(PeriodIndex == -1) 
	{ 
		PartOne = PlayCode;
		PartTwo = "";
	}
	else
	{
		PartOne = PlayCode.substr(0, PeriodIndex);
		PartTwo = PlayCode.substr(PeriodIndex + 1);
	}

	ProcessPlayOne(PartOne);
	//if (PartTwo != "") { ProcessPlayTwo(PartTwo); }
}


//Outcome Part 1 (before /) | Generally denotes a hit or a fielder who makes the out
void Play::ProcessPlayOne(std::string PartOne)
{
	if (std::isdigit(PartOne[0])) { Outcome = "Out"; }
	else if (PartOne[0] == 'C') { Outcome = "Catcher Interference"; }
	else if (PartOne[0] == 'S' || PartOne[0] == 'D' || PartOne[0] == 'T') { ProcessHit(PartOne); }
	else if (PartOne[0] == 'E') { Outcome = "Error"; }
	else if (PartOne.size() >= 3 && PartOne.substr(0, 3) == "DGR") { Outcome = "Ground Rule Double"; }
	else if (PartOne.size() >= 2 && PartOne.substr(0, 2) == "FC") { Outcome = "Fielder's Choice"; }
	else if (PartOne.size() >= 3 && PartOne.substr(0, 3) == "FLE") { Outcome = "Foul Fly Error"; }
	else if (PartOne.size() >= 2 && PartOne.substr(0, 2) == "HP") { Outcome = "Hit by pitch"; }
	else if (PartOne[0] == 'K') { Outcome = "Strikeout"; }
	else if (PartOne.size() >= 2 && PartOne.substr(0, 2) == "NP") { Outcome = "No play"; }
	else if (PartOne[0] == 'K') { Outcome = "Intentional Walk"; }
	else if (PartOne[0] == 'W') { Outcome = "Walk"; }
	else if (PartOne[0] == 'H') { Outcome = "Homerun"; }
}


//Ball trajectory and location
void Play::ProcessPlayTwo(std::string PartTwo)
{
	if (PartTwo[0] == 'G') { Trajectory = "Groundball"; }
	if (PartTwo[0] == 'F') { Outcome = "Flyball"; }
	if (PartTwo[0] == 'P') { Outcome = "Popup"; }
	if (PartTwo[0] == 'L') { Outcome = "Line Drive"; }
	if (PartTwo.substr(0, 3) == "GDP") { Outcome = "Double Play"; }

}


void Play::ProcessHit(std::string HitCode)
{
	OutcomeType = "Hit";

	if (HitCode[0] == 'S') { Outcome = "Single"; }
	else if (HitCode[0] == 'D') { Outcome = "Double"; }
	else if (HitCode[0] == 'T') { Outcome = "Triple"; }

	Trajectory = GetTrajectory(HitCode[3]);
	Location = HitCode[1];
}


std::string Play::GetTrajectory(char BallCode)
{
	switch (BallCode) 
	{
		case 'G': return "groundball";
		case 'F': return "flyball";
		case 'P': return "popup";
		case 'L': return "line drive";
		default: return "";
	}
}

std::string Play::GetFielder(int FielderCode)
{
	switch (FielderCode)
	{
		case 1: return "pitcher";
		case 2: return "catcher";
		case 3: return "firstbaseman";
		case 4: return "secondbaseman";
		case 5: return "shortstop";
		case 6: return "thirdbaseman";
		case 7: return "leftfielder";
		case 8: return "centerfielder";
		case 9: return "rightfielder";
		default: return "";
	}
}


std::string Play::GetLocation(std::string LocationCode, char trajectory)
{
	std::string Zone = ""; // General area the ball is hit to, roughly corresponds to a fielding position
	std::string Depth = ""; // Deep, Mid, Shallow
	std::string Radius = ""; // Where "around the circle" it is - foul, down the line, up the middle, between zones

	if


	if (LocationCode == "2F") {}
	else if (LocationCode == "25F") {}
	else if (LocationCode == "5F") {}
	else if (LocationCode == "5DF") {}
	else if (LocationCode == "7LSF") {}
	else if (LocationCode == "7LF") {}
	else if (LocationCode == "7LDF") {}
	else if (LocationCode == "2") {}
	else if (LocationCode == "25") {}
	else if (LocationCode == "5S") {}
	else if (LocationCode == "5") {}
	else if (LocationCode == "5D") {}
	else if (LocationCode == "7LS") {}
	else if (LocationCode == "7L") {}
	else if (LocationCode == "7LD") {}
	else if (LocationCode == "56S") {}
	else if (LocationCode == "56") {}
	else if (LocationCode == "56D") {}
	else if (LocationCode == "7S") {}
	else if (LocationCode == "7") {}
	else if (LocationCode == "7D") {}
	else if (LocationCode == "1S") {}
	else if (LocationCode == "15") {}
	else if (LocationCode == "6S") {}
	else if (LocationCode == "6") {}
	else if (LocationCode == "6D") {}
	else if (LocationCode == "78S") {}
	else if (LocationCode == "78") {}
	else if (LocationCode == "78D") {}
	else if (LocationCode == "78XD") {}
	else if (LocationCode == "1") {}
	else if (LocationCode == "6MS") {}
	else if (LocationCode == "6M") {}
	else if (LocationCode == "4MS") {}
	else if (LocationCode == "4M") {}
	else if (LocationCode == "4MD") {}
	else if (LocationCode == "8S") {}
	else if (LocationCode == "8") {}
	else if (LocationCode == "8D") {}
	else if (LocationCode == "8XD") {}
	else if (LocationCode == "7S") {}
	else if (LocationCode == "7") {}
	else if (LocationCode == "7D") {}
	else if (LocationCode == "1S") {}
	else if (LocationCode == "15") {}
	else if (LocationCode == "6S") {}
	else if (LocationCode == "6") {}
	else if (LocationCode == "6D") {}
	else if (LocationCode == "78S") {}
	else if (LocationCode == "78") {}
	else if (LocationCode == "78D") {}
	else if (LocationCode == "78XD") {}
	else if (LocationCode == "13") {}
	else if (LocationCode == "4S") {}
	else if (LocationCode == "4") {}
	else if (LocationCode == "4D") {}
	else if (LocationCode == "89S") {}
	else if (LocationCode == "89") {}
	else if (LocationCode == "89D") {}
	else if (LocationCode == "89XD") {}
	else if (LocationCode == "23") {}
	else if (LocationCode == "34S") {}
	else if (LocationCode == "34") {}
	else if (LocationCode == "34D") {}
	else if (LocationCode == "9S") {}
	else if (LocationCode == "9") {}
	else if (LocationCode == "9D") {}
	else if (LocationCode == "3S") {}
	else if (LocationCode == "3") {}
	else if (LocationCode == "3D") {}
	else if (LocationCode == "9LS") {}
	else if (LocationCode == "9L") {}
	else if (LocationCode == "9LD") {}
	else if (LocationCode == "23F") {}
	else if (LocationCode == "3F") {}
	else if (LocationCode == "3DF") {}
	else if (LocationCode == "9LSF") {}
	else if (LocationCode == "9LF") {}
	else if (LocationCode == "9LDF") {}
}


//Outcome Part 2 (after .) | Indicates baserunner activity