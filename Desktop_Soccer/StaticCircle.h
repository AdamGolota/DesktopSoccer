#include <SFML/System/Vector2.hpp>
#pragma once
class StaticCircle
{
public:
	StaticCircle();
	StaticCircle(float R, sf::Vector2f center);
	sf::Vector2f getCenter();
	~StaticCircle();
private:
	float R;
	sf::Vector2f center;
};

