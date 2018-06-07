#pragma once
#include "vector"
class SoccerPlayer;

class Team
{
public:
	Team();
	~Team();
	Team operator+=(SoccerPlayer&);
	bool getNumber();
	void setNumber(bool);
	static bool getCurrentTeam();
	unsigned getScore();
	int goal();
	static void switchTeam();
	static int moveTime;
	void dropScore();

	static bool currentTeam;
private:
	unsigned score;
	bool number;
};

