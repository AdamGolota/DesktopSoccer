#include <SFML/System/Vector2.hpp>
#include "StaticCircle.h"
#include "StaticRect.h"
#pragma once
class DynamicCircle
{
public:

	DynamicCircle();
	DynamicCircle(const sf::Vector2f& center, const int& R, const int& M, const float& frictionCoefficient);
	~DynamicCircle();
	void move();
	void hit(DynamicCircle& target);
	void hit(StaticCircle& barrier);
	void hit(StaticRect& barrier);
	void slowDown();
	int setFrictionCoefficient(const float& fa);
	int setCenter(const sf::Vector2f& center);
	int setVelocity(const sf::Vector2f& velocity);
	float getRadius();
	sf::Vector2f getCenter();
	sf::Vector2f distance(DynamicCircle& target);
	sf::Vector2f distance(StaticCircle& target);
	sf::Vector2f distance(StaticRect& target);
	sf::Vector2f distance(sf::Vector2f& point);
	bool inRange(sf::Vector2f point);
	bool checkCollision(DynamicCircle target);
	bool checkCollision(StaticCircle& barrier);
	bool checkCollision(StaticRect& barrier);
	friend sf::Vector2f postHitVelocity(DynamicCircle& object, DynamicCircle& target);
private:
	float frictionCoefficient;
	sf::Vector2f center;
	int R;
	int M;
	sf::Vector2f velocity;

};


float length(const sf::Vector2f& v);
sf::Vector2f postStaticHitVelocity(const sf::Vector2f& velocity, const float& contactAngle);
float angle(const sf::Vector2f&, const sf::Vector2f&);
float angle(const sf::Vector2f&);