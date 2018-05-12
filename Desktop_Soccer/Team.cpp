#include "Team.h"
#include "SoccerPlayer.h"
const unsigned scoreToWin = 5;

bool Team::currentTeam = 0;

Team::Team() : score(0)
{
}


Team::~Team()
{
}

Team Team::operator+=(SoccerPlayer& player)
{
	player.setTeam(this);
	return *this;
}

bool Team::getNumber()
{
	return this->number;
}

void Team::setNumber(bool number)
{
	this->number = number;
}

bool Team::getCurrentTeam()
{

	return Team::currentTeam;
}

unsigned Team::getScore()
{
	return this->score;
}

int Team::goal()
{
	this->score++;
	if (this->score == scoreToWin)
		return 1;
	return 0;
}
