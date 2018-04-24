#include "Team.h"

const unsigned scoreToWin = 5;

bool Team::currentTeam = 0;

Team::Team()
{
}


Team::~Team()
{
}

Team Team::operator+(SoccerPlayer& player)
{
	Team team = *this;
	team.players.push_back(player);
	return team;
}

Team Team::operator+=(SoccerPlayer& player)
{
	*this = *this + player;
	return *this;
}

bool Team::getNumber()
{
	return this->number;
}

std::vector<SoccerPlayer> Team::getPlayers()
{
	return this->players;
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
	Team::currentTeam = !Team::currentTeam;
	return 0;
}
