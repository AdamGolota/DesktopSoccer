#include "StaticCircle.h"



StaticCircle::StaticCircle()
{
}

StaticCircle::StaticCircle(float R, sf::Vector2f center) : R(R), center(center)
{
}

sf::Vector2f StaticCircle::getCenter()
{
	return this->center;
}


StaticCircle::~StaticCircle()
{
}
