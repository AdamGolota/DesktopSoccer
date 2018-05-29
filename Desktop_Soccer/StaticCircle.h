#include <SFML/System/Vector2.hpp>
#pragma once
#include "SolidObject.h"
class StaticCircle : public SolidObject
{
public:
	StaticCircle();
	StaticCircle(float R, sf::Vector2f center);
	sf::Vector2f getCenter();
	void hit(DynamicCircle&);
	bool checkCollision(DynamicCircle &);
	float getRadius();
	~StaticCircle();
private:
	float R;
	sf::Vector2f center;
};

