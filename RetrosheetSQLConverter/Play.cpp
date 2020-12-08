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
	ProcessBaserunners();
}

Play::~Play()
{
}




std::string Play::getPlay()
{
	if (OutcomeType == "Hit") { return ""; }//"Hits a " + Outcome + " on a " + Trajectory + " fielded by the " + Fielder + " | " + getBaserunnerText(); }
	else if (OutcomeType == "Out") { return ""; } //"Is out on a " + Trajectory + " to " + Location + " fielded by the " + GetFielder(Fielders[0]) + " | " + getBaserunnerText() ;}
	else if (OutcomeType == "Homerun") { return ""; } //"Hits a " + Trajectory + " to " + Location + " for a homerun! | " + getBaserunnerText() + "\n"; }
	else if (Outcome == "No play") { return ""; }
	else if (OutcomeType == "Strikeout") { return Outcome + " " + getBaserunnerText() + " | " + RawPlay + "\n"; }
	//else { return Outcome + " - " + PlayCode + " | " + BaserunnerCode + " || " + RawPlay + "\n"; }



	//if (OutcomeType == "Hit") { return "Hits a " + Outcome + " on a " + Trajectory + " fielded by the " + Fielder + " | " + getBaserunnerText(); }
	//else if (OutcomeType == "Out") { return "Is out on a " + Trajectory + " to " + Location + " fielded by the " + GetFielder(Fielders[0]) + " | " + getBaserunnerText() ;}
	//else if (OutcomeType == "Homerun") { return "Hits a " + Trajectory + " to " + Location + " for a homerun! | " + getBaserunnerText() + "\n"; }
	// else if (OutcomeType == "Homerun") { return "Hits a " + Trajectory + " to " + Location + " for a homerun! | " + getBaserunnerText() + "\n"; }
	//else { return Outcome + " - " + PlayCode + " | " + BaserunnerCode + " || " + RawPlay + "\n"; }
}

std::string Play::getBaserunnerText()
{
	std::string Text;
	std::string Result = ">";
	for (auto Event : BaserunnerActivity)
	{
		std::string Result = ">";
		if (!Event.isSafe) { Result = "x"; }
		Text += Event.startingBase + Result + Event.endingBase + ";";
	}
	return Text;
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
	else if (PlayCode.size() >= 2 && PlayCode.substr(0, 2) == "SB") { ProcessSB(); }
	else if (PlayCode.size() >= 2 && PlayCode.substr(0, 2) == "BK") { Outcome = "Balk"; PlayCode = PlayCode.substr(2); }
	else if (PlayCode.size() >= 2 && PlayCode.substr(0, 2) == "CS") { Outcome = "Caught Stealing"; }
	else if (PlayCode.size() >= 2 && PlayCode.substr(0, 2) == "DI") { Outcome = "Defensive Indifference"; PlayCode = PlayCode.substr(2); }
	else if (PlayCode.size() >= 2 && PlayCode.substr(0, 2) == "OA") { Outcome = "Other Advance"; }
	else if (PlayCode.size() >= 2 && PlayCode.substr(0, 2) == "PB") { Outcome = "Passed Ball"; }
	else if (PlayCode.size() >= 2 && PlayCode.substr(0, 2) == "WP") { Outcome = "Wild Pitch"; PlayCode = PlayCode.substr(2); }
	else if (PlayCode.size() >= 2 && PlayCode.substr(0, 2) == "PO") { Outcome = "Pickoff"; }
	else if (PlayCode[0] == 'C') { Outcome = "Catcher Interference"; }
	else if (PlayCode[0] == 'S' || PlayCode[0] == 'D' || PlayCode[0] == 'T') { ProcessHit(); }
	else if (PlayCode[0] == 'E') { ProcessE(); }
	else if (PlayCode.size() >= 3 && PlayCode.substr(0, 3) == "DGR") { Outcome = "Ground Rule Double"; }
	else if (PlayCode.size() >= 2 && PlayCode.substr(0, 2) == "FC") { Outcome = "Fielder's Choice"; }
	else if (PlayCode.size() >= 3 && PlayCode.substr(0, 3) == "FLE") { Outcome = "Foul Fly Error"; }
	else if (PlayCode.size() >= 2 && PlayCode.substr(0, 2) == "HP") { Outcome = "Hit by pitch"; PlayCode = PlayCode.substr(2); }
	else if (PlayCode[0] == 'K') { ProcessK(); }
	else if (PlayCode.size() >= 2 && PlayCode.substr(0, 2) == "NP") { Outcome = "No play"; PlayCode = PlayCode.substr(2); }
	else if (PlayCode[0] == 'I') { Outcome = "Intentional Walk"; PlayCode = PlayCode.substr(2); }
	else if (PlayCode[0] == 'W') { ProcessW(); }
	else if (PlayCode[0] == 'H') { ProcessHR(); }
}


void Play::ProcessHit()
{
	OutcomeType = "Hit";

	while (PlayCode.size() > 0)
	{
		if (PlayCode[0] == 'S') { Outcome = "Single"; PlayCode = PlayCode.substr(1); }
		else if (PlayCode[0] == 'D') 
		{ 
			if(PlayCode.size() > 2 && PlayCode.substr(0, 3) == "DGR") { Outcome = "Ground rule double"; PlayCode = PlayCode.substr(3); }
			else { Outcome = "Double"; PlayCode = PlayCode.substr(1); }
		}
		else if (PlayCode[0] == 'T') { Outcome = "Triple"; PlayCode = PlayCode.substr(1); }
		else if (PlayCode[0] == 'B') { Outcome = "Bunt single"; PlayCode = PlayCode.substr(1); }
		else if (isdigit(PlayCode[0])) { Location = PlayCode[0]; Fielder = GetFielder((int)PlayCode[0] - '0'); PlayCode = PlayCode.substr(1); }
		else if (PlayCode.size() > 3 && PlayCode.substr(0, 4) == "FINT") { Outcome = "Fan interference"; PlayCode = PlayCode.substr(4); }
		else if (PlayCode[0] == 'G' || PlayCode[0] == 'L' || PlayCode[0] == 'F' || PlayCode[0] == 'P') { Trajectory = GetTrajectory(PlayCode[0]); }
		else if (PlayCode[0] == '/' || PlayCode[0] == '+' || PlayCode[0] == '-') { PlayCode = PlayCode.substr(1); }
	}
}


void Play::ProcessHR()
{
	OutcomeType = "Homerun";

	if (PlayCode.substr(0, 2) == "HR") { PlayCode = PlayCode.substr(2); }
	else if (PlayCode[0] == 'H') { PlayCode = PlayCode.substr(1); }
	if (isdigit(PlayCode[0])) { Fielders.push_back(PlayCode[0]); PlayCode = PlayCode.substr(1); }

	while (PlayCode.size() > 0)
	{
		if (isalpha(PlayCode[0])) { Trajectory = GetTrajectory(PlayCode[0]); }
		else if (isdigit(PlayCode[0])) { Location = GetLocation(); }
		else { PlayCode = PlayCode.substr(1); }
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
			Fielders.push_back(PlayCode[0]);
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
				Outcome = "Double Play"; Trajectory = GetTrajectory(PlayCode[0]); PlayCode = PlayCode.substr(2);
			}
			else if (PlayCode.size() > 2 && (PlayCode.substr(0, 3) == "GTP" || PlayCode.substr(0, 3) == "LTP"))
			{ 
				Outcome = "Triple Play"; Trajectory = GetTrajectory(PlayCode[0]); PlayCode = PlayCode.substr(2);
			}
			else if (isdigit(PlayCode[0])) { Location = GetLocation(); }
			else { Trajectory = GetTrajectory(PlayCode[0]); }
			break;
		}
		if (PlayCode.size() > 0) { PlayCode = PlayCode.substr(1); }
	}
	if (Location == "") { Location = GetLocation(); }
}


void Play::ProcessK()
{
	OutcomeType = "Strikeout";
	Outcome = "Strikeout";
	PlayCode = PlayCode.substr(1);
	if (PlayCode[0] == '+')
	{
		PlayCode = PlayCode.substr(1);
		while (PlayCode.size() > 0)
		{
			if (PlayCode[0] == 'E')
			{
				std::string Fielder = GetFielder(PlayCode[1]);
				Outcome += " plus error by the " + Fielder;
				PlayCode = PlayCode.substr(2);
			}
			else if (PlayCode.substr(0, 2) == "SB")
			{
				Outcome += " but runner steals base";
				PlayCode = PlayCode.substr(3);
			}
			else if (PlayCode.substr(0, 2) == "CS")
			{
				Outcome += " and runner is caught attempting to steal";
				PlayCode = PlayCode.substr(2);
				BaserunnerAdvance Pickoff = BaserunnerAdvance();
				Pickoff.startingBase = PlayCode[0];
				Pickoff.endingBase = PlayCode[0];
				while (PlayCode[0] != ')')
				{
					if (isdigit(PlayCode[0])) { Fielders.push_back(PlayCode[0] - '0'); }
					std::cout << PlayCode[0];
					PlayCode = PlayCode.substr(1);
				}
				PlayCode = PlayCode.substr(1);
			}
			else if (PlayCode.substr(0, 2) == "PO")
			{
				Outcome += " and runner is picked off!";
				BaserunnerAdvance Pickoff = BaserunnerAdvance();
				Pickoff.startingBase = PlayCode[2];
				Pickoff.endingBase = PlayCode[2];
				PlayCode = PlayCode.substr(3);
				while (PlayCode[0] != ')')
				{
					if (isdigit(PlayCode[0])) { Fielders.push_back(PlayCode[0] - '0'); }
					std::cout << PlayCode[0];
					PlayCode = PlayCode.substr(1);
				}
				PlayCode = PlayCode.substr(1);
				Outcome += " and runner is picked off at " + Pickoff.startingBase + "!";
			}
			else if (PlayCode.substr(0, 2) == "PB") 
			{
				Outcome = "Passed ball on strike three!";
				PlayCode = PlayCode.substr(2);
			}
			else if (PlayCode.substr(0, 2) == "WP")
			{
				Outcome = "Wild pitch on strike three!";
				PlayCode = PlayCode.substr(2);
			}
			std::cout << PlayCode << " | " << RawPlay << "\n";
			if (PlayCode.size() > 0) { PlayCode = PlayCode.substr(1); }
		}
	}
	else if (PlayCode.substr(0, 2) == "23")
	{
		Fielders.push_back(2);
		Fielders.push_back(3);
		PlayCode = PlayCode.substr(2);
	}
}


void Play::ProcessE()
{
	OutcomeType = "Error";
	Fielders.push_back(PlayCode[1]);
	PlayCode = PlayCode.substr(2);
	while (PlayCode.size() > 0)
	{
		if (PlayCode.size() == 1)
		{
			if (PlayCode[0] == 'G' || PlayCode[0] == 'F' || PlayCode[0] == 'L' || PlayCode[0] == 'P') { Trajectory = GetTrajectory(PlayCode[0]); }
			else if (PlayCode[0] == '-') { PlayCode = PlayCode.substr(1); }
		}
		else
		{
			if (PlayCode[0] == '/') { PlayCode = PlayCode.substr(1); }
			else if (PlayCode.substr(0, 2) == "TH") { ErrorType = "Throwing"; PlayCode = PlayCode.substr(2); }
			else if (PlayCode.substr(0, 2) == "FO") { ErrorType = "Force"; PlayCode = PlayCode.substr(2); }
			else if (PlayCode.substr(0, 2) == "BG") { ErrorType = "Bunt"; PlayCode = PlayCode.substr(2); }
			else if (PlayCode.substr(0, 2) == "SH") { PlayCode = PlayCode.substr(2); }
			else if (PlayCode[0] == 'G' || PlayCode[0] == 'S') { Trajectory = GetTrajectory(PlayCode[0]); }
		}
	}
}


void Play::ProcessW()
{
	OutcomeType = "Walk";
	PlayCode = PlayCode.substr(1);
	if (PlayCode.size() > 0)
	{

	}
}

void Play::ProcessSB()
{
	OutcomeType = "Stolen Base";
	while (PlayCode.size() > 0)
	{
		if (PlayCode.substr(0, 2) == "SB")
		{
			PlayCode = PlayCode.substr(2);
		}
		else if(isdigit(PlayCode[0])) 
		{  
			BaserunnerAdvance StolenBase = BaserunnerAdvance();
			StolenBase.endingBase = PlayCode[0];
			StolenBase.startingBase = std::to_string(std::stoi(std::string(1, PlayCode[0])) - 1);
			StolenBase.isSafe = true;
			BaserunnerActivity.push_back(StolenBase);
			PlayCode = PlayCode.substr(1);
		}
		else { PlayCode = PlayCode.substr(1); }
	}
}


std::string Play::GetTrajectory(char BallCode)
{
	std::string Trajectory;

	switch (BallCode) 
	{
		case 'G': Trajectory = "groundball";
		case 'F': Trajectory = "flyball";
		case 'P': Trajectory = "popup";
		case 'L': Trajectory = "line drive";
		case 'B': Trajectory = "bunt";
		default: Trajectory = "";
	}
	PlayCode = PlayCode.substr(1);
	return Trajectory;
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


std::string Play::GetLocation()
{
	if (PlayCode.size() == 0) { PlayCode = Fielders[0]; }
	std::string Zone = ""; // General area the ball is hit to, roughly corresponds to a fielding position
	std::string Depth = ""; // Deep, Mid, Shallow
	std::string Radius = ""; // Where "around the circle" it is - foul, down the line, up the middle, between zones

	while (PlayCode.size() > 0)
	{
		if (isdigit(PlayCode[0])) { Zone += PlayCode[0]; }
		else 
		{
			switch (PlayCode[0])
			{
			case 'L': Radius += 'L';
			case 'F': Radius += 'F';
			case 'M': Radius += 'M';
			case 'S': Depth += 'S';
			case 'D': Radius += 'D';
			case 'X': Radius += 'X';
			}
		}
		PlayCode = PlayCode.substr(1);
	}
	return Radius + "-" + Depth + "-" + Zone;
}


//Outcome Part 2 (after .) | Indicates baserunner activity
void Play::ProcessBaserunners()
{
	BaserunnerAdvance CurrentRunner = BaserunnerAdvance();
	if (BaserunnerCode.size() > 0)
	{
		while (BaserunnerCode.size() > 0)
		{
			if (isdigit(BaserunnerCode[0]) || BaserunnerCode[0] == 'B')
			{
				CurrentRunner.startingBase = BaserunnerCode[0];
				if (BaserunnerCode[1] == '-') { CurrentRunner.isSafe = true; }
				else { CurrentRunner.isSafe = false; }
				CurrentRunner.endingBase = BaserunnerCode[2];
				BaserunnerCode = BaserunnerCode.substr(3);
			}
			else if (BaserunnerCode[0] == '(')
			{
				BaserunnerCode = BaserunnerCode.substr(1);
				while (BaserunnerCode[0] != ')')
				{
					if (isdigit(BaserunnerCode[0])) { CurrentRunner.Fielders.push_back(BaserunnerCode[0]); }
					BaserunnerCode = BaserunnerCode.substr(1);
				}
				BaserunnerCode = BaserunnerCode.substr(1);
			}
			else if (BaserunnerCode[0] == ';')
			{
				BaserunnerActivity.push_back(CurrentRunner);
				CurrentRunner = BaserunnerAdvance();
				BaserunnerCode = BaserunnerCode.substr(1);
			}
		}
		BaserunnerActivity.push_back(CurrentRunner);
	}
}