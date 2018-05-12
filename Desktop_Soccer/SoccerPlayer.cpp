#include "SoccerPlayer.h"



SoccerPlayer::SoccerPlayer(
	const sf::Vector2f & center,
	const float & R,
	const float & M,
	const float & fc,
	const float & mv) :
	DynamicCircle(center, R, M, fc, mv)
{
}

SoccerPlayer::SoccerPlayer(DynamicCircle c) :
	DynamicCircle(c.center, c.R, c.M, c.frictionCoefficient, c.maxVelocity)
{
}

SoccerPlayer::SoccerPlayer()
{
}


SoccerPlayer::~SoccerPlayer()
{
}

void SoccerPlayer::setTeam(Team *team)
{
	this->team = team;
}
