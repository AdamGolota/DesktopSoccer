#include <SFML/System/Vector2.hpp>
#pragma once
#include "SolidObject.h"
class StaticRect : public SolidObject
{
public:
	StaticRect();
	StaticRect(sf::Vector2f position, float x, float y);
	sf::Vector2f getPosition();
	void hit(DynamicCircle&);
	bool checkCollision(DynamicCircle&);
	float getX();
	float getY();
	~StaticRect();
private:
	sf::Vector2f position;
	float y;
	float x;
};