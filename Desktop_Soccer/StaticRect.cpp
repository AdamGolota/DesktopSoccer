#include "StaticRect.h"



StaticRect::StaticRect()
{
}

StaticRect::StaticRect(sf::Vector2f position, float length, float width) :
	position(position), length(length), width(width)
{

}

sf::Vector2f StaticRect::getPosition()
{
	return this->position;
}

float StaticRect::getAngle()
{
	return this->angle;
}


StaticRect::~StaticRect()
{
}
