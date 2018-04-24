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

SoccerPlayer::SoccerPlayer()
{
}


SoccerPlayer::~SoccerPlayer()
{
}
