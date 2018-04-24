#pragma once
#include "DynamicCircle.h"
#include "SFML\Graphics\CircleShape.hpp"
class SoccerPlayer : public DynamicCircle
{
public:
	SoccerPlayer(
		const sf::Vector2f& center,
		const float& R,
		const float& M,
		const float& frictionCoefficient,
		const float & maxVelocity);
	SoccerPlayer();
	~SoccerPlayer();
};

