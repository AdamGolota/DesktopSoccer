#pragma once
#include "DynamicCircle.h"
#include "SFML\Graphics\CircleShape.hpp"
#include "Team.h"


class SoccerPlayer : public DynamicCircle
{
public:
	SoccerPlayer(
		const sf::Vector2f& center,
		const float& R,
		const float& M,
		const float& frictionCoefficient,
		const float & maxVelocity);
	SoccerPlayer(DynamicCircle);
	SoccerPlayer();
	~SoccerPlayer();
	void setTeam(Team *team);

	Team *team;
};

