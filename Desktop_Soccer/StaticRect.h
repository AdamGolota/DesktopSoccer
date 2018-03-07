#include <SFML/System/Vector2.hpp>
#pragma once
class StaticRect
{
public:
	StaticRect();
	StaticRect(sf::Vector2f position, float length, float width);
	sf::Vector2f getPosition();
	float getAngle();
	~StaticRect();
private:
	sf::Vector2f position;
	float length;
	float width;
	float angle;
};