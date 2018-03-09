#include <SFML/System/Vector2.hpp>
#pragma once
class StaticRect
{
public:
	StaticRect();
	StaticRect(sf::Vector2f position, float x, float y);
	sf::Vector2f getPosition();
	float getX();
	float getY();
	~StaticRect();
private:
	sf::Vector2f position;
	float y;
	float x;
};