#include "DynamicCircle.h"
#include "cmath"

const float PI = 3.14159265359;

DynamicCircle::DynamicCircle() 
{
}

DynamicCircle::DynamicCircle(	
	const sf::Vector2f & center, 
	const float & R, 
	const float & M, 
	const float & fc, 
	const float & mv) : 
	R(R), center(center), velocity(sf::Vector2f(0, 0)), M(M), frictionCoefficient(fc), maxVelocity(mv)
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


void DynamicCircle::hit(StaticCircle & barrier)
{
	sf::Vector2f d = this->distance(barrier);
	float rs = this->getRadius() + barrier.getRadius();						//	Radius sum
	this->setCenter(this->center + d * ( 1 - rs / length(d)));
	this->setVelocity(this->postStaticHitVelocity(this->distance(barrier)));
}

void DynamicCircle::hit(StaticRect & barrier)
{
	sf::Vector2f d = this->distance(barrier);
	this->setCenter(this->center + d * (1 - this->getRadius() / length(d)));
	this->setVelocity(this->postStaticHitVelocity(this->distance(barrier)));
}

void DynamicCircle::hit(DynamicCircle & target)
{
	sf::Vector2f d = this->distance(target);
	float rs = this->getRadius() + target.getRadius();						//	Radius sum
	this->setCenter(this->center + d * (1 - rs / length(d)));
	sf::Vector2f ov = this->postHitVelocity( target);						// New object velocity
	sf::Vector2f tv = target.postHitVelocity(*this);						// New target velocity
	this->setVelocity(ov);
	target.setVelocity(tv);
}

void DynamicCircle::slowDown()
{	
	sf::Vector2f v = this->velocity;								// velocity
	float vl = length(this->velocity);
	float al = vl * this->frictionCoefficient*this->M;					// acceleration length

	sf::Vector2f a = v * (al / vl);
	if (vl > al)
	{
		this->setVelocity(sf::Vector2f(v - a));
	}
	else
		this->setVelocity(sf::Vector2f(0,0));
}

void DynamicCircle::pushInDirection(sf::Vector2f point)
{
	const float dk = 25.f;											// distance coefficient
	sf::Vector2f d = this->distance(point);				
	float dl = length(d) / dk;								
	float mv = this->maxVelocity;
	float vl = sqrt(dl > mv ? mv*mv: dl * mv);						//Velocity length		
	this->setVelocity(d / dk * (vl / dl));	
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

sf::Vector2f DynamicCircle::postStaticHitVelocity(sf::Vector2f distance)
{
	sf::Vector2f nv = this->normalVelocity(distance);
	sf::Vector2f newVelocity = this->velocity - (nv * 2.f);
	return newVelocity;
}

sf::Vector2f DynamicCircle::normalVelocity(sf::Vector2f distance)
{
	float dl = length(distance);
	// normal velocity length
	float nvl = (this->velocity.x * distance.x + this->velocity.y * distance.y) / length(distance);
	// normal velocity
	sf::Vector2f nv = distance * (nvl / dl);
	return nv;
}

bool DynamicCircle::inRange(sf::Vector2f point)
{
	bool inRange = length(this->distance(point)) < this->R;
	return inRange;
}

bool DynamicCircle::checkCollision(DynamicCircle& target)
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

sf::Vector2f DynamicCircle::postHitVelocity(DynamicCircle & target)
{
	sf::Vector2f distance = this->distance(target);
	sf::Vector2f onv = this->normalVelocity(distance);						// Object normal velosity
	sf::Vector2f tnv = target.normalVelocity(-distance); 					// Target normal velosity
																			// New normal velocity
	sf::Vector2f nnv = (2.f * target.M * tnv + onv * (this->M - target.M)) / (this->M + target.M);
	sf::Vector2f newVelocity = this->velocity - onv + nnv;
	return newVelocity;
}


float length(const sf::Vector2f& v) {
	float length = sqrt(pow(v.x, 2) + pow(v.y, 2));
	return length;
}


float angle(const sf::Vector2f & v)
{
	
	float angle = acos(v.x / length(v));
	return angle;
}
