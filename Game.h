#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

/*
	The Game Class: Allows a User to Play the "Sinking Shapes" Game
	- Creates Enemies for the user to interact with
	- Renders the graphics & texts onto a window
*/

class Game {
	private:
		//-Initializations [Constructor]-//
		void initVariables();
		void initWindow();
		void initEnemies();
		void initFonts(); 
		void initText();

		//-Variables-//
		   //Window Vars
		sf::RenderWindow* window;
		sf::VideoMode video;
		sf::Event ev;

		   //Game Object Vars
		sf::RectangleShape enemy;
		std::vector<sf::RectangleShape> enemies;

		   //Mouse Position Vars
		sf::Vector2i mousePosWindow;
		sf::Vector2f mousePosView;

		  //Game Logic Vars
		float enemySpawnTimer;
		float enemySpawnTimerMax;
		int maxEnemies;
		unsigned points;
		bool mouseHeld;
		int health;
		bool endgame;
		bool intro;

		  //Font & Text
		sf::Font font;
		sf::Text uiText;
		sf::Text introText;



	public:
		//-Constructor & Destructor-//
		Game();
		~Game();

		//-Public Functions-//
		void update(); //The Game Logic (inputs, win conditions, enemy behavior, etc)
		void render(); //Visualizing the Game onto the screen
		void pollEvents(); //Receiving Window Updates
		void spawnEnemy();
		void GameOverText();
		void reset();

		void updateMousePositions();
		void updateEnemies();
		void updateText();

		void renderIntro();
		void renderEnemies(sf::RenderTarget& target); //allows you to control how you WANT to render enemies specifically
		void renderText(sf::RenderTarget& target);  //target: allows you to control WHERE to render stuff, doesnt have to all be on the window directly
		void renderGameOver();


		//-Accessor Functions-//  [Called in main's game loop]
		const bool isRunning() const; //A const function that returns a const bool
		const bool getEndgame() const;
		const void getPoints() const;
		const bool getIntro() const;
};