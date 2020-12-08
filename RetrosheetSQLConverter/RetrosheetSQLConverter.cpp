// RetrosheetSQLConverter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Game.h"

std::vector<std::vector<std::string>> Data;
std::vector<Game> Games;

void HandleInfoRow(std::vector<std::string> Row, Game& Game);
void HandleStartRow(std::vector<std::string> Row, Game& Game);
void HandlePlayRow(std::vector<std::string> Row, Game& Game);
void HandleSubRow(std::vector<std::string> Row, Game& Game);

std::vector<std::string> SplitString(std::string RawString);

int main()
{
    // Import File and separate into games
    std::string filename = "2019ANA.EVA";
    //std::cout << "Enter file name for import: ";
    //std::cin >> filename;

    std::ifstream file;
    file.open(filename);
    if (file.is_open())
    {
        std::string line;

        while (std::getline(file, line))
        {
            if (line.substr(0, 2) == "id") { Games.push_back(Game()); }
            //else if (line.substr(0, 3) == "com") { Games[Games.size() - 1].RawData.push_back(SplitString(line)); }
            else if (line.substr(0, 3) == "sub") { Games[Games.size() - 1].RawData.push_back(SplitString(line)); }
            else if (line.substr(0, 4) == "info") { Games[Games.size() - 1].RawData.push_back(SplitString(line)); }
            else if (line.substr(0, 4) == "play") { Games[Games.size() - 1].RawData.push_back(SplitString(line)); }
            else if (line.substr(0, 4) == "data") { Games[Games.size() - 1].RawData.push_back(SplitString(line)); }
            else if (line.substr(0, 5) == "start") { Games[Games.size() - 1].RawData.push_back(SplitString(line)); }
        }
    }

    // Parse data per game
    for (auto& Game : Games)
    {
        for (auto Row : Game.RawData)
        {
            if (Row[0] == "info") { HandleInfoRow(Row, Game); }
            else if (Row[0] == "start") { HandleStartRow(Row, Game); }
            else if (Row[0] == "play") { HandlePlayRow(Row, Game); }
            else if (Row[0] == "sub") { HandleSubRow(Row, Game); }
        }
    }

    //Print some game info for Debug
    for (int i = 0; i < Games.size(); i++)
    {
        if (i == i)
        {
            //std::cout << "Game: " << i + 1 << ": " << Games[i].Teams[1].Abbreviation << " @ " << Games[i].Teams[0].Abbreviation << " on " << Games[i].GameDate << "\n";
            //std::cout << "WP: " << Games[i].WinningPitcherID << "\n";
            //std::cout << "LP: " << Games[i].LosingPitcherID << "\n";
            //std::cout << "Save: " << Games[i].SavePitcherID << "\n\n";

            //for (auto Team : Games[i].Teams)
            //{
            //    for (auto Player : Team.Roster)
            //    {
            //        std::cout << Player.second.FirstName << " " << Player.second.LastName << "\n";
            //    }
            //    std::cout << "\n";
            //}

            for (auto Play : Games[i].Plays)
            {
                //std::string PlayText = Play.getPlay();
                //if (PlayText != "")
                if (Play.PlayCode != "")
                {
                    std::cout << i+1 << ": " << Games[i].Teams[Play.isBottomInning].Roster[Play.BatterID].LastName << ": " << Play.PlayCode << " | " << Play.RawPlay << "\n";
                }
            }
            //for (auto Line : Games[i].RawData)
            //{
            //    if (Line[0] == "info")
            //    {
            //        for (auto Info : Line)
            //        {
            //            std::cout << Info << " | ";
            //        }
            //        std::cout << "\n";
            //    }
            //}
            //std::cout << "\n";
        }
    }

    file.close();
}

// Load retrosheet file from folder

// Populates needed info from info rows into the game object
void HandleInfoRow(std::vector<std::string> Row, Game& Game)
{
    if (Row[1] == "visteam") { Game.Teams[0].Abbreviation = Row[2]; }
    else if (Row[1] == "hometeam") { Game.Teams[1].Abbreviation = Row[2]; }
    else if (Row[1] == "wp") { Game.WinningPitcherID = Row[2]; }
    else if (Row[1] == "lp") { Game.LosingPitcherID = Row[2]; }
    else if (Row[1] == "save" && Row.size() > 2) { Game.SavePitcherID = Row[2]; }
    else if (Row[1] == "date" && Row.size() > 2) { Game.GameDate = Row[2]; }
    else if (Row[1] == "number" && Row.size() > 2) { Game.GameNumber = Row[2]; }
}


// Populates needed info from start rows into the game object
void HandleStartRow(std::vector<std::string> Row, Game& Game)
{
    Game.Teams[std::stoi(Row[3])].Roster[Row[1]] = (Player(Row));
}


// Handle PLAY rows
void HandlePlayRow(std::vector<std::string> Row, Game& Game)
{
    Game.Inning = std::stoi(Row[1]);
    Game.isBottomInning = std::stoi(Row[2]);

    Game.Plays.push_back(Play(Row));

    // Create an event object for each play row
    // Record each play's inning, home/away, playerID
    // Parse the event description for play details
    // Get result type, number of Runs, number of Outs, if its a double play, if bases were stolen or runners were caught, etc.
    // Record the defensive plays - putouts, assists, and errors
}


// Populates needed info from start rows into the game object
void HandleSubRow(std::vector<std::string> Row, Game& Game)
{
    Game.Teams[std::stoi(Row[3])].Roster[Row[1]] = (Player(Row));
}


// Handle DATA rows
    // Do nothing with these for now


// Export data to DB


// Close file



std::vector<std::string> SplitString(std::string RawString)
{
    std::vector<std::string> Strings;
    for (int i = 0; i < RawString.size(); i++)
    {
        if (RawString[i] == ',')
        {
            if (i == 0)
            {
                Strings.push_back(" ");
                RawString = RawString.substr(i + 1);
                i = -1;
            }
            else
            {
                Strings.push_back(RawString.substr(0, i));
                RawString = RawString.substr(i + 1);
                i = -1;
            }
        }
        else if (RawString[i] == '"')
        {
            RawString = RawString.substr(1);
            for (int j = 0; j < RawString.size(); j++)
            {
                if (RawString[j] == '"')
                {
                    Strings.push_back(RawString.substr(0, j));
                    RawString = RawString.substr(j + 2);
                    i = -1;
                    break;
                }
            }
        }
    }

    Strings.push_back(RawString);

    return Strings;
}