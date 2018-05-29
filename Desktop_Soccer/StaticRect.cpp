#include "StaticRect.h"
#include "DynamicCircle.h"


StaticRect::StaticRect()
{
}

StaticRect::StaticRect(sf::Vector2f position,  float x, float y ) :
	position(position), y(y), x(x)
{

}

sf::Vector2f StaticRect::getPosition()
{
	return this->position;
}

void StaticRect::hit(DynamicCircle& circle)
{
	sf::Vector2f d = circle.distance(*this);
	circle.setCenter(circle.getCenter() + d * (1 - circle.getRadius() / length(d)));
	circle.setVelocity(circle.postStaticHitVelocity(circle.distance(*this)));
}

bool StaticRect::checkCollision(DynamicCircle & circle)
{
	bool collide = length(circle.distance(*this)) < circle.getRadius();
	return collide;
}

float StaticRect::getX()
{
	return this->x;
}


float StaticRect::getY()
{
	return this->y;
}

StaticRect::~StaticRect()
{
}
