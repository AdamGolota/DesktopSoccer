#include <SFML/System/Vector2.hpp>
#include "StaticCircle.h"
#include "StaticRect.h"
#pragma once
class DynamicCircle
{
public:

	DynamicCircle();
	DynamicCircle(
		const sf::Vector2f& center, 
		const float& R, 
		const float& M, 
		const float& frictionCoefficient, 
		const float & maxVelocity);
	~DynamicCircle();
	void move();
	void hit(StaticCircle& barrier);
	void hit(StaticRect& barrier);
	void hit(DynamicCircle& target);
	void slowDown();
	void pushInDirection(sf::Vector2f point);
	int setFrictionCoefficient(const float& fa);
	int setCenter(const sf::Vector2f& center);
	int setVelocity(const sf::Vector2f& velocity);
	float getRadius();
	sf::Vector2f getCenter();
	sf::Vector2f distance(DynamicCircle& target);
	sf::Vector2f distance(StaticCircle& target);
	sf::Vector2f distance(StaticRect& target);
	sf::Vector2f distance(sf::Vector2f& point);
	sf::Vector2f postStaticHitVelocity(sf::Vector2f distance);
	sf::Vector2f normalVelocity(sf::Vector2f distance);
	bool inRange(sf::Vector2f point);
	bool checkCollision(DynamicCircle target);
	bool checkCollision(StaticCircle& barrier);
	bool checkCollision(StaticRect& barrier);
	sf::Vector2f postHitVelocity( DynamicCircle& target);
private:
	float maxVelocity;
	float frictionCoefficient;
	sf::Vector2f center;
	float R;
	float M;
	sf::Vector2f velocity;

};


float length(const sf::Vector2f& v);
float angle(const sf::Vector2f&, const sf::Vector2f&);
float angle(const sf::Vector2f&);