#pragma once
#include "map"
#include "string"
class Parameters
{
public:
	float operator[](std::string);
	Parameters(std::string filename);
	~Parameters();
private:
	void load();
	void save();
	std::map<std::string, float> parameters;
	std::string filename;
};

