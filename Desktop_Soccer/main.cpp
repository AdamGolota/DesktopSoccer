#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include "DesktopSoccer.h"
#include "string"
#include "thread"
#include "sstream"
#include "Windows.h"

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

std::vector<StaticRect> staticRects;
std::vector<StaticCircle> staticCircles;
std::vector<SoccerPlayer> soccerPlayers;
DynamicCircle ball(sf::Vector2f(X * 1 / 2, Y * 1 / 2), BR, BM, FC, MV);

int inGoal(DynamicCircle&);
void setInitPos();


int main()
{
	staticRects.resize(8);
	staticCircles.resize(4);
	soccerPlayers.resize(6);
	for (int i = 0; i < 6; i++)
	{
		soccerPlayers[i] = SoccerPlayer(sf::Vector2f(), PR, PM, FC, MV);
	}
	setInitPos();

	std::vector<DynamicCircle*> dynamicCircles;
	dynamicCircles.reserve(7);
	for (int i = 0; i < soccerPlayers.size(); i++)
	{
		dynamicCircles.push_back(&soccerPlayers[i]);
	}
	dynamicCircles.push_back(&ball);

	//SOLID OBJECTS
	std::vector<SolidObject*> solidObjects;
	solidObjects.reserve(15);
	for (int i = 0; i < dynamicCircles.size(); i++)
	{
		solidObjects.push_back(dynamicCircles[i]);
	}
	for (int i = 0; i < staticRects.size(); i++)
	{
		solidObjects.push_back(&staticRects[i]);
	}
	for (int i = 0; i < staticCircles.size(); i++)
	{
		solidObjects.push_back(&staticCircles[i]);
	}


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
	sf::Clock moveTimer;
	bool currentTeam = 0;
	bool staticHit;
	DynamicCircle* capturedPlayerPointer = NULL;
	std::string fontname = "C:\\Users\\Adam\\source\\repos\\Desktop_Soccer\\Desktop_Soccer\\arial_narrow_7.ttf";
	std::stringstream score;
	score << 0 << " : " << 0;
	sf::Font font;
	font.loadFromFile(fontname);
	sf::Text text;
	text.setPosition(20, 20);
	text.setFont(font);
	text.setCharacterSize(30);
	text.setString(score.str());
	// GAME TIMER

	while (window.isOpen())
	{
		time += clock.restart().asMilliseconds();
		Team::moveTime += moveTimer.restart().asMilliseconds();
		if (time > TPM)
		{
			staticHit = 0;
			time = 0;
			for (int i = 0; i < dynamicCircles.size(); i++)
			{
				dynamicCircles[i]->move();
				
				if (Team::moveTime > 5000)
				{
					Team::switchTeam();
				}


				int goal = inGoal(ball);
				bool endOfGame;
				if (goal)
				{
					if (goal == -1) {
						endOfGame = teams[0].goal();
						Team::currentTeam = teams[0].getNumber();
					}
					if (goal == 1) {
						endOfGame = teams[1].goal();
						Team::currentTeam = teams[1].getNumber();
					}
					if (endOfGame)
					{
						teams[0].dropScore();
						teams[1].dropScore();
					}
					score.str("");
					score << teams[0].getScore() << " : " << teams[1].getScore();
					text.setString(score.str());
					setInitPos();
					sf::sleep(sf::seconds(1));
					break;
				}

				for (auto object : solidObjects)
				{
					if (object == dynamicCircles[i])
						continue;
					bool collision = object->checkCollision(*dynamicCircles[i]);
					if (collision)
					{
						object->hit(*dynamicCircles[i]);
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
						Team::switchTeam();
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
			if (soccerPlayers[i].team->getNumber() == Team::getCurrentTeam())
			{
				shape.setOutlineThickness(3);
				shape.setOutlineColor(sf::Color::White);
			}
			window.draw(shape);
		}

		float r = ball.getRadius();
		sf::CircleShape shape(r);
		shape.setOrigin(sf::Vector2f(r, r));
		shape.setPosition(ball.getCenter());
		window.draw(shape);

		window.draw(text);

		window.display();
	}
	return 0;
}


int inGoal(DynamicCircle& ball)
{
	bool team1Goal = ball.getCenter().x > X - GL &&
		ball.getCenter().y > GP + GW / 2 &&
		ball.getCenter().y < Y - GP + GW / 2;
	bool team2Goal = ball.getCenter().x < GL &&
		ball.getCenter().y > GP + GW / 2 &&
		ball.getCenter().y < Y - GP + GW / 2;
	if (team1Goal)
		return -1;
	if (team2Goal)
		return 1;
	return 0;
}

void setInitPos()
{
	// SOLID OBJECTS
	// Borders

	staticRects[0] = StaticRect(sf::Vector2f(0, 0), X, 5);
	staticRects[1] = StaticRect(sf::Vector2f(0, 0), 5, Y);
	staticRects[2] = StaticRect(sf::Vector2f(0, Y - 5), X, 5);
	staticRects[3] = StaticRect(sf::Vector2f(X - 5, 0), 5, Y);

	// Goals
	staticRects[4] = StaticRect(sf::Vector2f(0, GP), GL, GW);
	staticRects[5] = StaticRect(sf::Vector2f(0, Y - GP), GL, GW);
	staticRects[6] = StaticRect(sf::Vector2f(X - GL, GP), GL, GW);
	staticRects[7] = StaticRect(sf::Vector2f(X - GL, Y - GP), GL, GW);

	// Goal posts
	staticCircles[0] = StaticCircle(GW / 2, sf::Vector2f(GL, GP + GW / 2));
	staticCircles[1] = StaticCircle(GW / 2, sf::Vector2f(GL, Y - GP + GW / 2));
	staticCircles[2] = StaticCircle(GW / 2, sf::Vector2f(X - GL, GP + GW / 2));
	staticCircles[3] = StaticCircle(GW / 2, sf::Vector2f(X - GL, Y - GP + GW / 2));


		//DYNAMIC CIRCLES


		// PLAYERS
	soccerPlayers[0].setCenter(sf::Vector2f(X / 4, Y / 5));
	soccerPlayers[1].setCenter(sf::Vector2f(X * 2 / 5, Y * 1 / 2));
	soccerPlayers[2].setCenter(sf::Vector2f(X / 4, Y * 4 / 5));
	soccerPlayers[3].setCenter(sf::Vector2f(X - X / 4, Y / 5));
	soccerPlayers[4].setCenter(sf::Vector2f(X - X * 2 / 5, Y * 1 / 2));
	soccerPlayers[5].setCenter(sf::Vector2f(X - X / 4, Y * 4 / 5));
	for (int i = 0; i < 6; i++)
	{
		soccerPlayers[i].setVelocity(sf::Vector2f(0.f, 0.f));
	}

	ball.setCenter(sf::Vector2f(X * 1 / 2, Y * 1 / 2));
	ball.setVelocity(sf::Vector2f(0,0));
}

//void displayScore(int s1, int s2, sf::RenderWindow& window)
//{
//	std::string fontname = "C:\\Users\\Adam\\source\\repos\\Desktop_Soccer\\Desktop_Soccer\\arial_narrow_7.ttf";
//	std::string score = s1 + " : " + s2;
//	sf::Font font;
//	font.loadFromFile(fontname);
//	sf::Text text(score, font, 60);
//	window.draw(text);
//	window.display();
//	sf::Clock clock;
//	int time = 0;
//	sf::sleep(sf::seconds(3));
//}
