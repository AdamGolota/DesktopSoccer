#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include "DesktopSoccer.h"
Parameters p("parameters.txt");

const float PI = p["PI"];
const int TPM = p["TPM"];				// Time Per Move (ms)
const int X = p["X"];
const int Y = p["Y"];
const int GL = X / 12;				// Goal length
const int GW = Y / 45;				// Goal width
const int GP = Y / 2.75 - GW / 2;	// Goal position
const int PR = p["PR"];					// Player Radius
const float PM = p["PM"];					// Player Mass
const float MV = p["MV"];				// Max Velocity
const float FC = p["FC"];				// Friction Coefficient
const float BR = p["BR"];				// Ball radius
const float BM = p["BM"];				// Ball mass



int main()
{

	// SOLID OBJECTS

	std::vector<StaticRect> staticRects = {
		// Borders
		StaticRect(sf::Vector2f(0,		0),		X, 5),
		StaticRect(sf::Vector2f(0,		0),		5, Y),
		StaticRect(sf::Vector2f(0,		Y - 5), X, 5),
		StaticRect(sf::Vector2f(X - 5,	0 ),	5, Y),
		
		// Goals
		StaticRect(sf::Vector2f(0,		GP),		GL, GW),
		StaticRect(sf::Vector2f(0,		Y - GP),	GL, GW),
		StaticRect(sf::Vector2f(X - GL, GP),		GL, GW),
		StaticRect(sf::Vector2f(X - GL, Y - GP),	GL, GW)
	};


	std::vector<StaticCircle> staticCircles = {

		// Goal posts
		StaticCircle(GW / 2, sf::Vector2f(GL,		GP + GW / 2)),
		StaticCircle(GW / 2, sf::Vector2f(GL,		Y - GP + GW / 2)),
		StaticCircle(GW / 2, sf::Vector2f(X - GL,	GP + GW / 2)),
		StaticCircle(GW / 2, sf::Vector2f(X - GL,	Y - GP + GW / 2)),
	};


	std::vector<SoccerPlayer> soccerPlayers = {
		// PLAYERS
		SoccerPlayer(sf::Vector2f(X / 4,		Y / 5), PR, PM, FC, MV),
		SoccerPlayer(sf::Vector2f(X * 2 / 5,	Y * 1 / 2), PR, PM, FC, MV),
		SoccerPlayer(sf::Vector2f(X / 4,		Y * 4 / 5), PR, PM, FC, MV),
		SoccerPlayer(sf::Vector2f(X - X / 4,		Y / 5), PR, PM, FC, MV),
		SoccerPlayer(sf::Vector2f(X - X * 2 / 5,	Y * 1 / 2), PR, PM, FC, MV),
		SoccerPlayer(sf::Vector2f(X - X / 4,		Y * 4 / 5), PR, PM, FC, MV),
	};

	DynamicCircle ball(sf::Vector2f(X * 1 / 2, Y * 1 / 2), BR, BM, FC, MV);


	std::vector<DynamicCircle*> dynamicCircles;
	dynamicCircles.reserve(6);
	for (int i = 0; i < soccerPlayers.size(); i++)
	{
		dynamicCircles.push_back(&soccerPlayers[i]);
	}
	dynamicCircles.push_back(&ball);


	// TEAMS
	Team teams[2];
	teams[0].setNumber(0);
	teams[1].setNumber(1);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			teams[i] += soccerPlayers[i*3 + j];
		}
	}

	sf::RenderWindow window(sf::VideoMode(X, Y), "DesktopSoccer");

	sf::Clock clock;
	int time = 0;
	bool currentTeam = 0;
	bool staticHit;
	DynamicCircle* capturedPlayerPointer = NULL;

	// GAME TIMER

	while (window.isOpen())
	{
		time += clock.restart().asMilliseconds();
		if (time > TPM)
		{
			staticHit = 0;
			time = 0;
			for (int i = 0; i < dynamicCircles.size(); i++)
			{
				dynamicCircles[i]->move();

				for (int j = 0; j < staticCircles.size(); j++)
				{
					bool collision = dynamicCircles[i]->checkCollision(staticCircles[j]);
					if (collision)
					{
						dynamicCircles[i]->hit(staticCircles[j]);
						staticHit = true;
					}

				}

				for (int j = 0; j < staticRects.size(); j++)
				{
					if (staticHit) 
						break;
					bool collision = dynamicCircles[i]->checkCollision(staticRects[j]);
					if (collision)
					{
						dynamicCircles[i]->hit(staticRects[j]);
					}
				}

				for (int j = i + 1; j < dynamicCircles.size(); j++)
				{
					bool collision = dynamicCircles[i]->checkCollision(*dynamicCircles[j]);
					if (collision)
					{
						dynamicCircles[i]->hit(*dynamicCircles[j]);
					}
				}
			}
		}


		{
			// EVENT HANDLING


			sf::Event event;

			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();

				if (event.type == sf::Event::MouseButtonPressed)
				{
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						sf::Vector2f clickPoint(event.mouseButton.x, event.mouseButton.y);
						for (int i = 0; i < soccerPlayers.size(); i++)
						{
							if (soccerPlayers[i].inRange(clickPoint) &&
								soccerPlayers[i].team->getNumber() == Team::getCurrentTeam())
							{
								capturedPlayerPointer = &soccerPlayers[i];
							}
						}
					}
				}
				if (event.type == sf::Event::MouseButtonReleased)
				{
					sf::Vector2f releasePoint(event.mouseButton.x, event.mouseButton.y);
					if (capturedPlayerPointer)
					{
						capturedPlayerPointer->pushInDirection(releasePoint);
						capturedPlayerPointer = NULL;
						Team::currentTeam = !Team::currentTeam;
					}
				}

			}

		}

		window.clear();


		// DRAWING 

		for (int i = 0; i < staticCircles.size(); i++)
		{
			float r = staticCircles[i].getRadius();
			sf::CircleShape shape(r);
			shape.setOrigin(sf::Vector2f(r, r));
			shape.setPosition(staticCircles[i].getCenter());
			window.draw(shape);
		}

		for (int i = 0; i < staticRects.size(); i++)
		{
			sf::RectangleShape shape(sf::Vector2f(staticRects[i].getX(), staticRects[i].getY()));
			shape.setPosition(staticRects[i].getPosition());
			window.draw(shape);
		}

		for (int i = 0; i < soccerPlayers.size(); i++)
		{
			float r = soccerPlayers[i].getRadius();
			sf::CircleShape shape(r);
			shape.setOrigin(sf::Vector2f(r, r));
			shape.setPosition(soccerPlayers[i].getCenter());
			shape.setFillColor(soccerPlayers[i].team->getNumber() ? sf::Color::Red : sf::Color::Blue);
			window.draw(shape);
		}

		float r = ball.getRadius();
		sf::CircleShape shape(r);
		shape.setOrigin(sf::Vector2f(r, r));
		shape.setPosition(ball.getCenter());
		window.draw(shape);

		window.display();
	}

	return 0;
}