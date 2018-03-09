#include "StaticRect.h"



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
