#pragma once
#include "SoccerPlayer.h"
#include "vector"

class Team
{
public:
	Team();
	~Team();
	Team operator+(SoccerPlayer&);
	Team operator+=(SoccerPlayer&);
	bool getNumber();
	std::vector<SoccerPlayer> getPlayers();
	static bool getCurrentTeam();
	unsigned getScore();
	int goal();
private:
	unsigned score;
	bool number;
	std::vector<SoccerPlayer> players;
	static bool currentTeam;
};

