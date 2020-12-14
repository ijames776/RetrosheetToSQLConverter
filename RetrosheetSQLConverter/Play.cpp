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


void Play::ProcessPlay()
{
	for(int i = 0; i < PlayCode.size(); i++)
	{
		if (PlayType == "")
		{
			if (isdigit(PlayCode[i])) { ProcessFieldOut(); }
			else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "DI") { PlayType = "defensiveInterference"; ReducePlayCode(2); }
			else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "NP") { PlayType = "noPlay"; ReducePlayCode(2); }
			else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "IW") { PlayType = "intentionalWalk"; ReducePlayCode(2); }
			else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "HP") { PlayType = "hitByPitch"; ReducePlayCode(2); }
			else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "WP") { PlayType = "wildPitch"; ReducePlayCode(2); }
			else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "FC") { PlayType = "fieldersChoice"; ReducePlayCode(2); AddFielders(); }
			else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "PB") { PlayType = "passedBall"; ReducePlayCode(2); }
			else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "BK") { PlayType = "balk"; ReducePlayCode(2); }
			else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "PO") { ProcessPO(true); }
			else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "OA") { PlayType = "otherAdvance"; ReducePlayCode(2); }
			else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "HR") { PlayType = "homerun"; ReducePlayCode(2); }
			else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "SB") { ProcessSB(true); }
			else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "CS") { ProcessCS(true); }
			else if (PlayCode.size() > 2 && PlayCode.substr(0, 3) == "DGR") { PlayType = "groundRuleDouble"; ReducePlayCode(3); GetLocation(); }
			else if (PlayCode.size() > 2 && PlayCode.substr(0, 3) == "FLE") { GetTrajectory(); ReducePlayCode(1); ProcessError(true); }
			else if (PlayCode[0] == 'K' || PlayCode[0] == 'C') { PlayType = "strikeout"; ReducePlayCode(1); }
			else if (PlayCode[0] == 'S' || PlayCode[0] == 'D' || PlayCode[0] == 'T') { ProcessHit(); }
			else if (PlayCode[0] == 'W') { PlayType = "walk"; ReducePlayCode(1); }
			else if (PlayCode[0] == 'E') { ProcessError(true); }
			else { PlayType = "Other"; }
			i = -1;
		}
		else if (PlayCode[0] == '+' || PlayCode[0] == '-' || PlayCode[0] == '!' || PlayCode[0] == ';') { ReducePlayCode(1); i = -1; }
		else if (PlayCode[0] == '/')
		{
			ReducePlayCode(1);
			if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "FO") { ReducePlayCode(2); }
			else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "SH") { PlayType = "sacrificeHit"; ReducePlayCode(2); }
			else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "AP") { ReducePlayCode(2); }
			else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "FL") { ReducePlayCode(2); }
			else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "BR") { ReducePlayCode(2); }
			else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "SF") { PlayType = "sacrificeFly"; ReducePlayCode(2); }
			else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "BG" || PlayCode.substr(0, 2) == "BL" || PlayCode.substr(0, 2) == "BP" || PlayCode.substr(0, 2) == "BF") { ReducePlayCode(1); GetTrajectory(); }
			else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "IF") { PlayType = "infieldFly"; ReducePlayCode(2); }
			else if (PlayCode.size() > 2 && PlayCode.substr(0, 3) == "FDP" || PlayCode.substr(0, 3) == "LDP" || PlayCode.substr(0, 3) == "GDP") { PlayType = "doublePlay"; ReducePlayCode(3); }
			else if (PlayCode.size() > 2 && PlayCode.substr(0, 3) == "FTP" || PlayCode.substr(0, 3) == "LTP" || PlayCode.substr(0, 3) == "GTP") { PlayType = "triplePlay"; ReducePlayCode(3); }
			else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "DP") { PlayType = "doublePlay"; ReducePlayCode(2); }
			else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "TP") { PlayType = "triplePlay"; ReducePlayCode(2); }
			else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "TH") { ReducePlayCode(2); if (isdigit(PlayCode[0])) { ReducePlayCode(1); } }
			else if (PlayCode.size() > 3 && PlayCode.substr(0, 4) == "UREV") { ReducePlayCode(4); }
			else if (PlayCode.size() > 3 && PlayCode.substr(0, 4) == "BOOT") { PlayType = "batterOutOfTurn"; ReducePlayCode(4); }
			else if (PlayCode.size() > 2 && PlayCode.substr(0, 3) == "OBS") { PlayType = "obstruction"; ReducePlayCode(3); }
			else if (PlayCode.size() > 3 && PlayCode.substr(0, 4) == "BINT") { PlayType = "batterInterference"; ReducePlayCode(4); }
			else if (PlayCode.size() > 3 && PlayCode.substr(0, 4) == "FINT") { PlayType = "fanInterference"; ReducePlayCode(4); }
			else if (PlayCode.size() > 3 && PlayCode.substr(0, 4) == "UINT") { PlayType = "umpireInterference"; ReducePlayCode(4); }
			else if (PlayCode.size() > 3 && PlayCode.substr(0, 4) == "RINT") { PlayType = "runnerInterference"; ReducePlayCode(4); }
			else if (PlayCode.size() > 2 && PlayCode.substr(0, 3) == "INT") { PlayType = "interference"; ReducePlayCode(3); }
			else if (PlayCode.size() > 3 && PlayCode.substr(0, 4) == "PASS") { ReducePlayCode(4); }
			else if (PlayCode.size() > 2 && PlayCode.substr(0, 3) == "NDP") { ReducePlayCode(3); }
			else if (PlayCode[0] == 'C') { ReducePlayCode(1); }
			else if (PlayCode[0] == 'R') { ReducePlayCode(2); }
			else if (PlayCode[0] == 'E') { ProcessError(false); }
			else if (Trajectory == "" && PlayCode[0] == 'G' || PlayCode[0] == 'F' || PlayCode[0] == 'L' || PlayCode[0] == 'P')
			{
				GetTrajectory();
			}
			else if (isdigit(PlayCode[0])) { GetLocation(); }
			i = -1;
		}
		else if (PlayCode[0] == '(')
		{
			if (PlayCode.size() > 1 && PlayCode.substr(1, 2) == "UR") { ReducePlayCode(4); }
			else if (PlayCode.size() > 2 && PlayCode.substr(1, 3) == "TUR") { ReducePlayCode(5); }
			i = -1;
		}
		else if (isdigit(PlayCode[0])) { GetLocation(); i = -1; }
		else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "SB") { ProcessSB(false); i = -1;}
		else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "CS") { ProcessCS(false); i = -1; }
		else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "WP") { ReducePlayCode(2); i = -1; }
		else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "PB") { ReducePlayCode(2); i = -1; }
		else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "OA") { ReducePlayCode(2); i = -1; }
		else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "DI") { ReducePlayCode(2); i = -1; }
		else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "PO") { ProcessPO(false); i = -1; }
		else if (PlayCode.size() > 2 && PlayCode.substr(0, 3) == "TUR") { ProcessSB(false); i = -1; }
		else if (PlayCode.size() > 1 && PlayCode.substr(0, 2) == "DP") { PlayType = "doublePlay"; ReducePlayCode(2); }
		else if (PlayCode.size() > 3 && PlayCode.substr(0, 4) == "MREV") { ReducePlayCode(4); i = -1;}
		else if (PlayCode[0] == 'E') { ProcessError(false); i = -1; }
		else if (PlayCode[0] == '!') { ReducePlayCode(1); i = -1; }
	}
}


void Play::ProcessHit()
{
	PlayType = "Hit";
	if (PlayCode[0] == 'S') { OutcomeType = "single"; }
	else if (PlayCode[0] == 'D') { OutcomeType = "double"; }
	else if (PlayCode[0] == 'T') { OutcomeType = "triple"; }
	ReducePlayCode(1);
	AddFielders();
}


void Play::ProcessFieldOut()
{
	PlayType = "FieldOut";
	AddFielders();
}


void Play::ProcessError(bool mainPlay)
{
	if (mainPlay) { PlayType = "error"; }

	Error PlayError;
	PlayError.Fielder = "" + PlayCode[1];
	ReducePlayCode(2);

	if (PlayCode.size() > 2 && PlayCode.substr(0, 3) == "/TH") 
	{ 
		PlayError.ErrorType = "Throw"; 
		ReducePlayCode(3);
		if (isdigit(PlayCode[0]) || PlayCode[0] == 'H') { PlayError.BaseThrownTo = PlayCode[3]; ReducePlayCode(1); }
	}
	else { PlayError.ErrorType = "Catch"; }

	Errors.push_back(PlayError);
}


void Play::ProcessSB(bool mainPlay)
{
	if (mainPlay) { PlayType = "stolenBase"; }

	ReducePlayCode(3);
}


void Play::ProcessCS(bool mainPlay)
{
	if (mainPlay) { PlayType = "caughtStealing"; }

	OutBase = PlayCode[2];
	ReducePlayCode(4);
	AddFielders();
	ReducePlayCode(1);
}


void Play::ProcessPO(bool mainPlay)
{
	if (mainPlay) { PlayType = "pickoff"; }
	ReducePlayCode(2);
	if (PlayCode[0] == 'C') { ReducePlayCode(2); }

	OutBase = PlayCode[0];
	ReducePlayCode(2);
	AddFielders();
	ReducePlayCode(1);
}


void Play::AddFielders()
{
	bool fieldersComplete = false;
	while (!fieldersComplete)
	{
		if (isdigit(PlayCode[0]))
		{
			Fielders.push_back(PlayCode[0]);
			ReducePlayCode(1);
		}
		else if (PlayCode[0] == '!') { ReducePlayCode(1); }
		else if (PlayCode[0] == '(')
		{
			OutBase == PlayCode[1];
			ReducePlayCode(3);
		}
		else if (PlayCode[0] == 'E') { ProcessError(false); }
		else { fieldersComplete = true; }
	}
}


void Play::GetTrajectory()
{
	switch (PlayCode[0]) 
	{
		case 'G': Trajectory = "groundball"; break;
		case 'F': Trajectory = "flyball"; break;
		case 'P': Trajectory = "popup"; break;
		case 'L': Trajectory = "line drive"; break;
		default: Trajectory = "";
	}
	PlayCode = PlayCode.substr(1);
	if (PlayCode[0] == '+' || PlayCode[0] == '-' || PlayCode[0] == '!') { PlayCode = PlayCode.substr(1); }
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
	std::string Zone = ""; // General area the ball is hit to, roughly corresponds to a fielding position
	std::string Depth = ""; // Deep, Mid, Shallow
	std::string Radius = ""; // Where "around the circle" it is - foul, down the line, up the middle, between zones

	if (PlayCode.size() == 0) { Zone += Fielders[0]; }
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
		ReducePlayCode(1);
	}
	return Radius + "-" + Depth + "-" + Zone;
}

void Play::ReducePlayCode(int numChars)
{
	PlayCode = PlayCode.substr(numChars);
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