#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include "DesktopSoccer.h"

const float PI = 3.14159265359;
const float SV = 1.0f;				// Standard Velocity
const int TPM = 10;					// Time Per Move (ms)
const int X = 800;
const int Y = 500;
const int GL = X / 10;				// Goal length
const int GW = Y / 20;				// Goal width
const int GP = Y / 2.75 - GW / 2;	// Goal position
const int PR = 35;					// Player Radius
const float PM = 1;					// Player Mass
const float PMV = 15;				// Player Max Velocity
const float PFC = 0.025;			// Player Friction Coefficient

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

	std::vector<DynamicCircle> dynamicCircles = {
		DynamicCircle(sf::Vector2f(X / 2, Y / 2), PR, PM, PFC, PMV),
		DynamicCircle(sf::Vector2f(X / 2 + 100, Y / 2 - 100), PR, PM, PFC, PMV)
	};
	dynamicCircles[0].setVelocity(sf::Vector2f(0, 0));





	sf::RenderWindow window(sf::VideoMode(X, Y), "DesktopSoccer");

	sf::Clock clock;
	int time = 0;
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
				dynamicCircles[i].move();

				for (int j = 0; j < staticCircles.size(); j++)
				{
					bool collision = dynamicCircles[i].checkCollision(staticCircles[j]);
					if (collision)
					{
						dynamicCircles[i].hit(staticCircles[j]);
						staticHit = true;
					}

				}

				for (int j = 0; j < staticRects.size(); j++)
				{
					if (staticHit) 
						break;
					bool collision = dynamicCircles[i].checkCollision(staticRects[j]);
					if (collision)
					{
						dynamicCircles[i].hit(staticRects[j]);
					}
				}

				for (int j = i + 1; j < dynamicCircles.size(); j++)
				{
					bool collision = dynamicCircles[i].checkCollision(dynamicCircles[j]);
					if (collision)
					{
						hit(dynamicCircles[i], dynamicCircles[j]);
					}
				}
			}
		}


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
					for (int i = 0; i < dynamicCircles.size(); i++)
					{
						if (dynamicCircles[i].inRange(clickPoint))
						{
							capturedPlayerPointer = &dynamicCircles[i];
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

		for (int i = 0; i < dynamicCircles.size(); i++)
		{
			float r = dynamicCircles[i].getRadius();
			sf::CircleShape shape(r);
			shape.setOrigin(sf::Vector2f(r, r));
			shape.setPosition(dynamicCircles[i].getCenter());
			window.draw(shape);
		}

		window.display();
	}

	return 0;
}