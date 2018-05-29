#pragma once
class DynamicCircle;
class SolidObject
{
public:
	SolidObject();
	virtual void hit(DynamicCircle&) = 0;
	virtual bool checkCollision(DynamicCircle&) = 0;
	~SolidObject();
};

