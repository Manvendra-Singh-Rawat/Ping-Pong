#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <SFML/Audio.hpp>
#include <time.h>

using namespace std;

int main()
{
	srand(time(0)); // To randomize the number

	sf::RenderWindow Window(sf::VideoMode(900, 900), "PONG"); //To create a window
	Window.setFramerateLimit(60); //Set the framelimit of 60Hz
	Window.setKeyRepeatEnabled(false); //To not repeat the same input

	sf::Event event;

	//States
	bool Menu = true; // To check if the game is in Menu
	bool play = true; //Play state for the main game loop
	bool RoundStart = true; //To check if the player is playing the game
	bool RoundAfter = false; // To check if the player is in the exit screen
	bool Quit = false; // To quit the game
	
	bool Up = false;
	bool Down = false;

	//Variables
	int yVelocityPad_1 = 0; // Player Velocity
	int yVelocityPad_2 = 0; // AI Velocity
	int xVelocityBall = -7; // Ball Velocity X
	int yVelocityBall = -7; // Ball Velocity Y

	int PlayerScore = 0, AIScore = 0; // Scores in the game
	int WhoWon = -1; // To check who won the game

	//Font
	sf::Font font_Consolas;
	if (font_Consolas.loadFromFile("Data/consola.ttf") == 0)
		return 2;

	//Text
	sf::Text Score;
	Score.setFont(font_Consolas);
	Score.setCharacterSize(30);
	Score.setPosition(400, 0);

	sf::Text Winning;
	Winning.setFont(font_Consolas);
	Winning.setCharacterSize(50);
	Winning.setPosition(400, 0);

	sf::Text MenuText;
	MenuText.setFont(font_Consolas);
	MenuText.setCharacterSize(50);
	MenuText.setString("Welcome to \nPONG\nPress 'P' to Start\nPress 'Q' to exit anytime");
	MenuText.setPosition(50, 600);

	//Images
	sf::Texture Tex_Pad_1; //Texture for Player
	sf::Texture Tex_Pad_2; //Texture for AI
	sf::Texture Background_Tex; //Texture for Background
	sf::Texture Ball_Tex; //Texture for Ball

	if (Tex_Pad_1.loadFromFile("Data/Slab_1.png") == 0)
		return 1;
	
	if (Tex_Pad_2.loadFromFile("Data/Slab_2.png") == 0)
		return 1;

	if (Background_Tex.loadFromFile("Data/Background.png") == 0)
		return 1;

	if (Ball_Tex.loadFromFile("Data/Ball.png") == 0)
		return 1;

	//Sounds
	sf::SoundBuffer Buff_Hit;
	if(Buff_Hit.loadFromFile("Data/Hit.wav") == 0)
		return 2;

	sf::Sound Hit;
	Hit.setBuffer(Buff_Hit);

	//Shapes
	sf::RectangleShape Background;
	Background.setSize(sf::Vector2f(900, 900));
	Background.setPosition(0, 0);
	Background.setTexture(&Background_Tex);

	sf::RectangleShape Pad_1;
	Pad_1.setSize(sf::Vector2f(50, 100));
	Pad_1.setPosition(50, 450);
	Pad_1.setTexture(&Tex_Pad_1);

	sf::RectangleShape Pad_2;
	Pad_2.setSize(sf::Vector2f(50, 100));
	Pad_2.setPosition(800, 450);
	Pad_2.setTexture(&Tex_Pad_2);

	sf::RectangleShape Ball;
	Ball.setSize(sf::Vector2f(50, 50));
	Ball.setPosition(425, 425);
	Ball.setTexture(&Ball_Tex);

	while (play)
	{
		//Event
		if (RoundStart == true) //To randomize the location of ball at the start of each round
		{
			switch (rand() % 2)
			{
			case 0: xVelocityBall = -7;
					yVelocityBall = -7;
					RoundStart = false;
					break;
			case 1: xVelocityBall = -7;
					yVelocityBall = 7;
					RoundStart = false;
					break;
			}
		}
			
		while (Window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || Quit == true) //To check if the player wants to exit or not
			{
				play = false;
				return 9;
			}
			if (event.type == sf::Event::KeyPressed) //To check if the player presses the key
			{
				if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) Up = true;
				if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) Down = true;
				if (event.key.code == sf::Keyboard::Q) Quit = true;
				if (event.key.code == sf::Keyboard::P && Menu == true) Menu = false;
			}
			if (event.type == sf::Event::KeyReleased) //To check if the player releases the key
			{
				if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) Up = false;
				if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) Down = false;
			}		
		}
		
		//Logic
		if (RoundAfter == false && Menu == false)
		{
			//Player Movement//
			/////////////////////////////////////////////////////////////////////
			if (Up == true)
			{
				yVelocityPad_1 = -7;
			}
			if (Down == true)
			{
				yVelocityPad_1 = 7;
			}

			if (Up == true && Down == true)
			{
				yVelocityPad_1 = 0;
			}
			if (Up == false && Down == false)
			{
				yVelocityPad_1 = 0;
			}
			
			//Move
			Pad_1.move(0, yVelocityPad_1);
			/////////////////////////////////////////////////////////////////////

			//Out of Bounds checking For Player//
			/////////////////////////////////////////////////////////////////////
			if (Pad_1.getPosition().y < 0)
				Pad_1.setPosition(Pad_1.getPosition().x, 0);
		
			if (Pad_1.getPosition().y > 800)
				Pad_1.setPosition(Pad_1.getPosition().x, 800);
			/////////////////////////////////////////////////////////////////////

			//AI Movement//
			/////////////////////////////////////////////////////////////////////
			if (Ball.getPosition().y < Pad_2.getPosition().y)
				yVelocityPad_2 = -6;

			if (Ball.getPosition().y > Pad_2.getPosition().y)
				yVelocityPad_2 = 6;

			//Move
			Pad_2.move(0, yVelocityPad_2);
			/////////////////////////////////////////////////////////////////////

			//Ball Movement//
			/////////////////////////////////////////////////////////////////////
			//Out of Bounds checking
			if (Ball.getPosition().y < 0)
			{
				yVelocityBall = -yVelocityBall;
			}
			if (Ball.getPosition().y > 850)
			{
				yVelocityBall = -yVelocityBall;
			}

			//Move
			Ball.move(xVelocityBall, yVelocityBall);
			/////////////////////////////////////////////////////////////////////

			//Ball Colission//
			/////////////////////////////////////////////////////////////////////
			if (Ball.getGlobalBounds().intersects(Pad_1.getGlobalBounds()) == true || Ball.getGlobalBounds().intersects(Pad_2.getGlobalBounds()) == true)
			{
				Hit.play(); //To play the hitting sound if Player/AI collides with Ball
				xVelocityBall = -xVelocityBall;

				switch (rand() % 3) //To  randomize the ball velocity after every colission
				{
				case 0: xVelocityBall++;
						yVelocityBall--;
						break;
				case 1: xVelocityBall--;
						yVelocityBall++;
						break;
				case 2: xVelocityBall++;
						yVelocityBall++;
						break;
				default: break;
				}

				if (xVelocityBall < 5 && xVelocityBall > 0)
					xVelocityBall++;
				if (xVelocityBall > -5 && xVelocityBall < 0)
					xVelocityBall--;
				if (xVelocityBall > 8)
					xVelocityBall--;
				if (xVelocityBall < -8)
					xVelocityBall++;

				if (yVelocityBall < 5 && yVelocityBall > 0)
					yVelocityBall++;
				if (yVelocityBall > -5 && yVelocityBall < 0)
					yVelocityBall--;
				if (yVelocityBall > 9)
					yVelocityBall--;
				if (yVelocityBall < -9)
					yVelocityBall++;
			}
			/////////////////////////////////////////////////////////////////////

			//Ball Side Wining//
			/////////////////////////////////////////////////////////////////////
			if (Ball.getPosition().x <= 0)
			{
				AIScore++;
				Ball.setPosition(425, 425);
				RoundStart = true;
				Pad_1.setPosition(50, 450);
				Pad_2.setPosition(800, 450);
			}
			if (Ball.getPosition().x >= 900)
			{
				PlayerScore++;
				Ball.setPosition(425, 425);
				RoundStart = true;
				Pad_1.setPosition(50, 450);
				Pad_2.setPosition(800, 450);
			}
			/////////////////////////////////////////////////////////////////////

			//Updating Score//
			/////////////////////////////////////////////////////////////////////
			if (PlayerScore == 7)
			{
				WhoWon = 1;
				RoundAfter = true;
			}
			else if (AIScore == 7)
			{
				WhoWon = 2;
				RoundAfter = true;
			}
			/////////////////////////////////////////////////////////////////////
		}

		//Rendering//
		Window.clear();

		Window.draw(Background); // Display the background

		if (Menu == true)
			Window.draw(MenuText); //Display menu only when we are in the menu screen
		
		if (Menu == false)
		{
			Window.draw(Pad_1);
			Window.draw(Pad_2);
			Window.draw(Ball);
			
			stringstream text;
			text << PlayerScore << " : " << AIScore;
			Score.setString(text.str());
			Window.draw(Score);
		}
			
		if (RoundAfter == true)
		{
			if (WhoWon == 1)
			{
				Winning.setPosition(150, 500);
				Winning.setString("Player Won the match!!");
			}
			else if (WhoWon == 2)
			{
				Winning.setPosition(200, 500);
				Winning.setString("AI Won the match!!");
			}
			Window.draw(Winning);
		}
		Window.display();
		/////////////////////////////////////////////////////////////////////
	}
	//Clean up
	Window.close();

	return 0;
}