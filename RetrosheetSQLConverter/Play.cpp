#include <iostream>
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

	SplitPlayCode();
	ProcessPlay();
	//if (BaserunnerCode != "") { ProcessBaserunners(); }
}

Play::~Play()
{
}




std::string Play::getPlay()
{
	if (OutcomeType == "Hit") { return "Hits a " + Outcome + " on a " + Trajectory + " fielded by the " + Fielder + " | " + BaserunnerCode; }
	else if (OutcomeType == "Out") { return "Is out on a " + Trajectory + " to " + Location + " fielded by the " + GetFielder(Fielders[0]) + " | " + BaserunnerCode; }

	return Outcome + " - " + PlayCode + " | " + BaserunnerCode;
}

void Play::SplitPlayCode()
{
	int PeriodIndex = -1;
	for (int i = 0; i < RawPlay.size(); i++) { if (RawPlay[i] == '.') { PeriodIndex = i; break; } }

	if(PeriodIndex == -1) 
	{ 
		PlayCode = RawPlay;
		BaserunnerCode = "";
	}
	else
	{
		PlayCode = RawPlay.substr(0, PeriodIndex);
		BaserunnerCode = RawPlay.substr(PeriodIndex + 1);
	}
}


//Outcome Part 1 (before /) | Generally denotes a hit or a fielder who makes the out
void Play::ProcessPlay()
{
	if (std::isdigit(PlayCode[0])) { ProcessOut(); }
	else if (PlayCode.size() >= 2 && PlayCode.substr(0, 2) == "SB") { Outcome = "Stolen Base"; }
	else if (PlayCode.size() >= 2 && PlayCode.substr(0, 2) == "BK") { Outcome = "Balk"; }
	else if (PlayCode.size() >= 2 && PlayCode.substr(0, 2) == "CS") { Outcome = "Caught Stealing"; }
	else if (PlayCode.size() >= 2 && PlayCode.substr(0, 2) == "DI") { Outcome = "Defensive Indifference"; }
	else if (PlayCode.size() >= 2 && PlayCode.substr(0, 2) == "OA") { Outcome = "Other Advance"; }
	else if (PlayCode.size() >= 2 && PlayCode.substr(0, 2) == "PB") { Outcome = "Passed Ball"; }
	else if (PlayCode.size() >= 2 && PlayCode.substr(0, 2) == "WP") { Outcome = "Wild Pitch"; }
	else if (PlayCode.size() >= 2 && PlayCode.substr(0, 2) == "PO") { Outcome = "Pickoff"; }
	else if (PlayCode[0] == 'C') { Outcome = "Catcher Interference"; }
	else if (PlayCode[0] == 'S' || PlayCode[0] == 'D' || PlayCode[0] == 'T') { ProcessHit(); }
	else if (PlayCode[0] == 'E') { Outcome = "Error"; }
	else if (PlayCode.size() >= 3 && PlayCode.substr(0, 3) == "DGR") { Outcome = "Ground Rule Double"; }
	else if (PlayCode.size() >= 2 && PlayCode.substr(0, 2) == "FC") { Outcome = "Fielder's Choice"; }
	else if (PlayCode.size() >= 3 && PlayCode.substr(0, 3) == "FLE") { Outcome = "Foul Fly Error"; }
	else if (PlayCode.size() >= 2 && PlayCode.substr(0, 2) == "HP") { Outcome = "Hit by pitch"; }
	else if (PlayCode[0] == 'K') { Outcome = "Strikeout"; }
	else if (PlayCode.size() >= 2 && PlayCode.substr(0, 2) == "NP") { Outcome = "No play"; }
	else if (PlayCode[0] == 'K') { Outcome = "Intentional Walk"; }
	else if (PlayCode[0] == 'W') { Outcome = "Walk"; }
	else if (PlayCode[0] == 'H') { Outcome = "Homerun"; }
}


//Ball trajectory and location
void Play::ProcessBaserunners(std::string PartTwo)
{
	if (PartTwo[0] == 'G') { Trajectory = "Groundball"; }
	if (PartTwo[0] == 'F') { Outcome = "Flyball"; }
	if (PartTwo[0] == 'P') { Outcome = "Popup"; }
	if (PartTwo[0] == 'L') { Outcome = "Line Drive"; }
	if (PartTwo.substr(0, 3) == "GDP") { Outcome = "Double Play"; }

}


void Play::ProcessHit()
{
	OutcomeType = "Hit";

	while (PlayCode != "")
	{
		if (PlayCode[0] == 'S') { Outcome = "Single"; }
		else if (PlayCode[0] == 'D') { Outcome = "Double"; }
		else if (PlayCode[0] == 'T') { Outcome = "Triple"; }
		else if (isdigit(PlayCode[0])) { Location = PlayCode[0]; Fielder = GetFielder((int)PlayCode[0] - 48); }
		else if (PlayCode[0] == 'G' || PlayCode[0] == 'L' || PlayCode[0] == 'F') { Trajectory = GetTrajectory(PlayCode[0]); }
		PlayCode = PlayCode.substr(1);

	}
}


void Play::ProcessOut()
{
	OutcomeType = "Out";
	int OutCount = 0;

	while(PlayCode != "")
	{
		if (isdigit(PlayCode[0]))
		{
			Fielders.push_back((int)PlayCode[0] - 48);
		}
		else if (PlayCode[0] == '(')
		{
			RunnersOut.push_back(PlayCode[0]);
			PlayCode = PlayCode.substr(1);
			OutCount++;
		}
		else if (PlayCode[0] == '/')
		{
			PlayCode = PlayCode.substr(1);
			if (PlayCode.size() > 2 && (PlayCode.substr(0, 3) == "GDP" || PlayCode.substr(0, 3) == "LDP"))
			{ 
				Outcome = "Double Play"; Trajectory = GetTrajectory(PlayCode[0]); PlayCode = PlayCode.substr(3);
			}
			else if (PlayCode.size() > 2 && (PlayCode.substr(0, 3) == "GTP" || PlayCode.substr(0, 3) == "LTP"))
			{ 
				Outcome = "Triple Play"; Trajectory = GetTrajectory(PlayCode[0]); PlayCode = PlayCode.substr(3);
			}
			else if (isdigit(PlayCode[0])) { Location = GetLocation(PlayCode[0] + ""); }
			else { Trajectory = GetTrajectory(PlayCode[0]); }
			break;
		}
		PlayCode = PlayCode.substr(1);
	}
	if (Location == "") { Location = GetLocation(std::to_string(Fielders[0])); }
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


std::string Play::GetLocation(std::string LocationCode)
{
	std::string Zone = ""; // General area the ball is hit to, roughly corresponds to a fielding position
	std::string Depth = ""; // Deep, Mid, Shallow
	std::string Radius = ""; // Where "around the circle" it is - foul, down the line, up the middle, between zones

	while (LocationCode != "")
	{
		if (isdigit(LocationCode[0])) { Zone += LocationCode[0]; }
		else 
		{
			switch (LocationCode[0])
			{
			case 'L': Radius += 'L';
			case 'F': Radius += 'F';
			case 'M': Radius += 'M';
			case 'S': Depth += 'S';
			case 'D': Radius += 'D';
			case 'X': Radius += 'X';
			}
		}
		if (LocationCode.size() == 1) { LocationCode = ""; }
		else { LocationCode = LocationCode.substr(1); }
	}
	return Radius + "-" + Depth + "-" + Zone;
}


//Outcome Part 2 (after .) | Indicates baserunner activity