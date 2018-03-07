#include "RoundSolidObject.h"
#include "cmath"
#define PI 3.14159265359
RoundSolidObject::RoundSolidObject() : 
	R(100), center(sf::Vector2f(0, 0)), velocity(sf::Vector2f(0,0)), M(100), frictionAcceleration(0)
{
}

RoundSolidObject::RoundSolidObject(const sf::Vector2f & center, const int & R, const int & M) : 
	R(R), center(center), velocity(sf::Vector2f(0, 0)), M(M), frictionAcceleration(0)
{
}


RoundSolidObject::~RoundSolidObject()
{
}

void RoundSolidObject::move()
{
	this->center += this->velocity;
	slowDown();
}

void RoundSolidObject::hit(RoundSolidObject& target)
{
	this->velocity = postHitVelocity(*this, target);
}

void RoundSolidObject::hit(StaticCircle & barrier)
{
	float ca = acos(this->distance(barrier).x / length(this->distance(barrier))); // contact angle
	this->velocity = postStaticHitVelocity(this->velocity, ca);
}

void RoundSolidObject::hit(StaticRect & barrier)
{
	float ca = acos(this->distance(barrier).x / length(this->distance(barrier))); // contact angle
	this->velocity = postStaticHitVelocity(this->velocity, ca);
}

void RoundSolidObject::slowDown()
{	
	if (this->frictionAcceleration < 0)
	{
		float angle = acos(this->velocity.x / length(this->velocity));
		this->velocity.x += this->frictionAcceleration * cos(angle);
		this->velocity.y += this->frictionAcceleration * sin(angle);
	}
}

int RoundSolidObject::setFrictionAcceleraton(const float & fa)
{
	if (fa <= 0)
		this->frictionAcceleration = fa;
	else return 1;
	return 0;
}


int RoundSolidObject::setCenter(const sf::Vector2f & center)
{
	if (center.x > 0 && center.y > 0)
		this->center = center;
	else return 1;
	return 0;
}

sf::Vector2f RoundSolidObject::getCenter()
{
	return this->center;
}

sf::Vector2f RoundSolidObject::distance(RoundSolidObject & target)
{
	sf::Vector2f distance = target.getCenter() - this->getCenter();
	return distance;
}

sf::Vector2f RoundSolidObject::distance(StaticCircle & target)
{
	sf::Vector2f distance = target.getCenter() - this->getCenter();
	return distance;
}

sf::Vector2f RoundSolidObject::distance(StaticRect & target)
{
	float x0 = target.getPosition().x;
	float y0 = target.getPosition().y;
	float x10 = this->getCenter().x;
	float y10 = this->getCenter().y;
	float k = tan(target.getAngle());
	float x = (y10 - y0 + k * x0 + 1 / k * x10) / (k + 1 / k);
	float y = k * x + y0 - k * x0;
	sf::Vector2f distance = sf::Vector2f(x, y) - this->getCenter();
	return distance;
}

sf::Vector2f postHitVelocity(RoundSolidObject & object, RoundSolidObject & target)
{

	float ova = acos(object.velocity.x / length(object.velocity)); // object velocity angle
	float tva = acos(target.velocity.x / length(target.velocity)); // target velocity angle
	float ca = acos(object.distance(target).x / length(object.distance(target))); // contact angle
	float v1 = length(object.velocity);
	float v2 = length(target.velocity);
	float m1 = object.M;
	float m2 = target.M;
	sf::Vector2f newVelocity;
	newVelocity.x = (v1*cos(ova - ca)*(m1 - m2) + 2 * m2*v2*cos(tva - ca))*cos(ca) / (m1 + m2) +
		v1 * sin(ova - ca) * cos(ca + PI / 2);
	newVelocity.y = (v1*cos(ova - ca)*(m1 - m2) + 2 * m2*v2*cos(tva - ca))*sin(ca) / (m1 + m2) +
		v1 * sin(ova - ca) * sin(ca + PI / 2);

	return newVelocity;
}


float length(const sf::Vector2f& v) {
	float length = sqrt(pow(v.x, 2) + pow(v.y, 2));
	return length;
}

sf::Vector2f postStaticHitVelocity(const sf::Vector2f & velocity, const float & contactAngle)
{
	float v = length(velocity);
	float va = acos(velocity.x / length(velocity)); // velocity angle
	float ca = contactAngle;
	sf::Vector2f constVelocity;
	constVelocity.x = v * sin(va + ca)*cos(90 - ca);
	constVelocity.y = v * sin(va + ca)*sin(90 - ca);
	sf::Vector2f normalVelocity = velocity - constVelocity;
	sf::Vector2f newVelocity = constVelocity - normalVelocity;
	return newVelocity;
}
