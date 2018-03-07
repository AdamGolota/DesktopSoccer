#include <SFML/System/Vector2.hpp>
#include "StaticCircle.h"
#include "StaticRect.h"
#pragma once
class RoundSolidObject
{
public:

	RoundSolidObject();
	RoundSolidObject(const sf::Vector2f& center, const int& R, const int& M);
	~RoundSolidObject();
	void move();
	void hit(RoundSolidObject& target);
	void hit(StaticCircle& barrier);
	void hit(StaticRect& barrier);
	void slowDown();
	int setFrictionAcceleraton(const float& fa);
	int setCenter(const sf::Vector2f& center);
	sf::Vector2f getCenter();
	sf::Vector2f distance(RoundSolidObject& target);
	sf::Vector2f distance(StaticCircle& target);
	sf::Vector2f distance(StaticRect& target);
	friend sf::Vector2f postHitVelocity(RoundSolidObject& object, RoundSolidObject& target);
private:
	float frictionAcceleration;
	sf::Vector2f center;
	int R;
	int M;
	sf::Vector2f velocity;

};


float length(const sf::Vector2f& v);
sf::Vector2f postStaticHitVelocity(const sf::Vector2f& velocity, const float& contactAngle);
