#include "DynamicCircle.h"
#include "cmath"

const float PI = 3.14159265359;

DynamicCircle::DynamicCircle() : 
	R(100), center(sf::Vector2f(0, 0)), velocity(sf::Vector2f(0,0)), M(100), frictionCoefficient(0)
{
}

DynamicCircle::DynamicCircle(const sf::Vector2f & center, const int & R, const int & M, const float & fc) : 
	R(R), center(center), velocity(sf::Vector2f(0, 0)), M(M), frictionCoefficient(fc)
{
}


DynamicCircle::~DynamicCircle()
{
}

void DynamicCircle::move()
{
	this->center += this->velocity;
	slowDown();
}

void DynamicCircle::hit(DynamicCircle& target)
{
	this->velocity = postHitVelocity(*this, target);
}

void DynamicCircle::hit(StaticCircle & barrier)
{
	sf::Vector2f d = this->distance(barrier);
	float ca = angle(this->distance(barrier)) * (d.y > 0 ? -1 : 1); // contact angle
	this->velocity = postStaticHitVelocity(this->velocity, ca);
}

void DynamicCircle::hit(StaticRect & barrier)
{
	float ca = angle(this->distance(barrier)); // contact angle
	this->velocity = postStaticHitVelocity(this->velocity, ca);
}

void DynamicCircle::slowDown()
{	
	float acceleration = this->frictionCoefficient*this->M;
	if (length(this->velocity) > acceleration)
	{
		float angle = acos(this->velocity.x / length(this->velocity));
		this->velocity.x += acceleration * cos(angle);
		this->velocity.y += acceleration * sin(angle);
	}
	else
		this->velocity = sf::Vector2f(0,0);
}

int DynamicCircle::setFrictionCoefficient(const float & fa)
{
	if (fa <= 0)
		this->frictionCoefficient = fa;
	else return 1;
	return 0;
}


int DynamicCircle::setCenter(const sf::Vector2f & center)
{
	if (center.x > 0 && center.y > 0)
		this->center = center;
	else return 1;
	return 0;
}

int DynamicCircle::setVelocity(const sf::Vector2f & velocity)
{
	this->velocity = velocity;
	return 0;
}

float DynamicCircle::getRadius()
{
	return this->R;
}

sf::Vector2f DynamicCircle::getCenter()
{
	return this->center;
}

sf::Vector2f DynamicCircle::distance(DynamicCircle & target)
{
	sf::Vector2f distance = target.getCenter() - this->getCenter();
	return distance;
}

sf::Vector2f DynamicCircle::distance(StaticCircle & target)
{
	sf::Vector2f distance = target.getCenter() - this->getCenter();
	return distance;
}

sf::Vector2f DynamicCircle::distance(StaticRect & target)
{
	sf::Vector2f corner = target.getPosition();
	sf::Vector2f dtc(corner - this->center); // Distance To Corner
	sf::Vector2f point;
	if (abs(dtc.x) > target.getX())
		dtc.x = target.getX() * (dtc.x / abs(dtc.x));
	if (abs(dtc.y) > target.getY())
		dtc.y = target.getY() * (dtc.y / abs(dtc.y));
	point.x = corner.x + (abs(dtc.x) - dtc.x) / 2;
	point.y = corner.y + (abs(dtc.y) - dtc.y) / 2;
	sf::Vector2f distance(point - this->center);
	return distance;
}

sf::Vector2f DynamicCircle::distance(sf::Vector2f & point)
{
	sf::Vector2f distance = point - this->getCenter();
	return distance;
}

bool DynamicCircle::inRange(sf::Vector2f point)
{
	bool inRange = length(this->distance(point)) < this->R;
	return inRange;
}

bool DynamicCircle::checkCollision(DynamicCircle target)
{
	float tr = target.getRadius();
	bool collide = length(this->distance(target)) - tr < this->R;
	return collide;
}

bool DynamicCircle::checkCollision(StaticCircle & barrier)
{
	float br = barrier.getRadius();
	bool collide = length(this->distance(barrier)) - br < this->R;
	return collide;
}

bool DynamicCircle::checkCollision(StaticRect & barrier)
{
	bool collide = length(this->distance(barrier)) < this->R;
	return collide;
}

sf::Vector2f postHitVelocity(DynamicCircle & object, DynamicCircle & target)
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
	float va = angle(velocity) * (velocity.y > 0 ? -1 : 1);
	float ca = contactAngle;
	sf::Vector2f constVelocity;
	constVelocity.x = v * cos(PI / 2 - ca) * sin(ca + va);
	constVelocity.y = v * sin(PI / 2 - ca) * sin(ca + va);
	sf::Vector2f normalVelocity = velocity - constVelocity;
	sf::Vector2f newVelocity = constVelocity - normalVelocity;
	return newVelocity;
}


float angle(const sf::Vector2f & v)
{
	
	float angle = acos(v.x / length(v));
	return angle;
}
