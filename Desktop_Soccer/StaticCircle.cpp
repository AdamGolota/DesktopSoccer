#include "StaticCircle.h"
#include "DynamicCircle.h"


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

void StaticCircle::hit(DynamicCircle& circle)
{
	sf::Vector2f d = circle.distance(*this);
	float rs = circle.getRadius() + this->getRadius();						//	Radius sum
	circle.setCenter(circle.center + d * (1 - rs / length(d)));
	circle.setVelocity(circle.postStaticHitVelocity(circle.distance(*this)));
}

bool StaticCircle::checkCollision(DynamicCircle & circle)
{
	float br = this->getRadius();
	bool collide = length(circle.distance(*this)) - br < circle.getRadius();
	return collide;
}

float StaticCircle::getRadius()
{
	return this->R;
}


StaticCircle::~StaticCircle()
{
}
