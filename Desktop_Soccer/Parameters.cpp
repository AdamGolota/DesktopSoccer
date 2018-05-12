#include "Parameters.h"
#include "fstream"

float Parameters::operator[](std::string key)
{
	return this->parameters[key];
}

Parameters::Parameters(std::string filename)
{
	this->filename = filename;
	this->load();
}

void Parameters::load()
{
	std::string key;
	std::fstream fs(this->filename, std::fstream::in);
	while (!fs.eof())
	{
		fs >> key;
		fs >> this->parameters[key];
	}
	fs.close();
}

void Parameters::save()
{
	std::fstream fs(this->filename, std::fstream::out | std::fstream::trunc);
	
	for (const auto &pair : this->parameters)
	{
		fs << pair.first << " ";
		fs << pair.second << std::endl;
	}
	fs.close();
}



Parameters::~Parameters()
{
	this->save();
}
