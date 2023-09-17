#include "Game.h"



//=== Constructors & Destructors ===//
Game::Game() {
	this->initVariables();
	this->initWindow();
	this->initEnemies();
	this->initFonts();
	this->initText();
}

Game::~Game() {
	delete this->window;
}



//=== Initializations [Constructor] ===//
void Game::initVariables() {
	/* Initialize private variables */
	this->window = nullptr;
	this->points = 0;
	this->enemySpawnTimerMax = 20.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 10;
	this->mouseHeld = false;
	this->health = 20;
	this->endgame = false;
	this->intro = true;
}

void Game::initWindow() {
	/* Initialize Window Settings */
	this->video.width = 800;
	this->video.height = 600;

	//Create window with width+height, title, and a window that can minimize, close, etc
	this->window = new sf::RenderWindow(this->video, "Sinking Shapes", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(60);
}

void Game::initEnemies() {
	/* Sets Enemies to Default */
	this->enemy.setPosition(10.f, 10.f); //ensures enemies fit on screen
	this->enemy.setSize(sf::Vector2f(100.f, 100.f)); //.f == "float value"
	this->enemy.setScale(sf::Vector2f(0.5f, 0.5f));
	this->enemy.setFillColor(sf::Color(146, 134, 148));
}

void Game::initFonts() {
	/* Grabs Font from System File */
	if (!this->font.loadFromFile("Fonts/SFCartoonistHand.ttf")) {
		std::cout << "ERROR | GAME::INITFONTS | Failed to load font!" << std::endl;
	}
}

void Game::initText() {
	/* Sets Font for Game */
	this->uiText.setFont(this->font);
	this->introText.setFont(this->font);
	this->uiText.setCharacterSize(24);
	this->introText.setCharacterSize(60);
	this->uiText.setFillColor(sf::Color::White);
	this->introText.setFillColor(sf::Color::Black);
	this->uiText.setString("default_text");
}



//=== Public Functions ===//
void Game::update() {
	/* Polls for Events & Runs the Game */
	//this->pollEvents();
	if (!this->endgame) {
		this->updateMousePositions();
		this->updateText();
		this->updateEnemies();
	}
	if (this->health <= 0) {
		this->endgame = true;
	}
}

void Game::render() {
	/* Renders the Game onto the Screen
		  1) Clear old frame
		  2) render in objects
		  3) display frames in window  */

	this->window->clear(sf::Color(101, 122, 165));
	this->renderEnemies(*this->window);
	this->renderText(*this->window);
	this->window->display();
}

void Game::pollEvents() {
	/* Polls for events relating to the game window */

	while (this->window->pollEvent(this->ev)) { //Poll for events while the game is running
		switch (this->ev.type) { //Check the type of event detected

			//If the 'X' close button is pressed, close the window
		case sf::Event::Closed:
			this->window->close();
			break;
			//If esc is pressed, close the window 
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			else if (this->ev.key.code == sf::Keyboard::Enter)
				this->intro = false;
			else if (this->ev.key.code == sf::Keyboard::R) {//Reset all values for 2nd game
				//reset game
				this->reset();
			}
				
			break;
		}
	}
}

void Game::reset() {
	this->points = 0;
	this->enemySpawnTimerMax = 20.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 10;
	this->mouseHeld = false;
	this->health = 20;
	this->endgame = false;
	this->intro = true;

	this->initText();
	this->initEnemies();
	this->enemies.clear();
}

void Game::spawnEnemy() {
	/* Spawns enemies
	   1) sets a random posiiton
	   2) sets a random color & difficulty
	   3) adds enemy to vector
	*/

	//Sets Random Enemy Position from the Top
	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
		0.f
	);

	//Randomize Enemy Type
	int type = rand() % 5;
	switch (type) {
	case 0: // Hardest Enemy
		this->enemy.setSize(sf::Vector2f(50.f, 50.f));
		this->enemy.setFillColor(sf::Color::Red);
		// different  speed
		break;
	case 1:
		this->enemy.setSize(sf::Vector2f(70.f, 70.f));
		this->enemy.setFillColor(sf::Color(75, 0, 130));
		break;
	case 2: // Medium Enemy
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		this->enemy.setFillColor(sf::Color::Blue);
		break;
	case 3:
		this->enemy.setSize(sf::Vector2f(150.f, 150.f));
		this->enemy.setFillColor(sf::Color::Yellow);
		break;
	case 4: // Easiest Enemy
		this->enemy.setSize(sf::Vector2f(200.f, 200.f));
		this->enemy.setFillColor(sf::Color::Green);
		break;

	default:
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		this->enemy.setFillColor(sf::Color::White);
		break;
	}
	this->enemies.push_back(this->enemy);
}

void Game::updateMousePositions() {
	/* Updates Mouse Positions relative to the window */
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateEnemies() {
	/* Holds Enemy Game Logic */

	//Enemy Spawn Timer
	if (this->enemies.size() < this->maxEnemies) { //keep adding enemies to keep max enemies on screen 24/7

		if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else {
			this->enemySpawnTimer += 1.f;
		}
	}

	//Move the enemies
	for (int i = 0; i < this->enemies.size(); i++) {
		bool deleted = false;
		this->enemies[i].move(0.f, 3.f); //Moves enemies downwards with this speed
		
		//If Enemy Reaches Bottom Screen
		if (this->enemies[i].getPosition().y > this->window->getSize().y) { //'>' bc 0,0 is top left

			//Lose Health
			if (this->enemies[i].getFillColor() == sf::Color::Red) {
				this->health -= 5;
			}
			else if (this->enemies[i].getFillColor() == sf::Color(75, 0, 130)) {
				this->health -= 4;
			}
			else if (this->enemies[i].getFillColor() == sf::Color::Blue) {
				this->health -= 3;
			}
			else if (this->enemies[i].getFillColor() == sf::Color::Yellow) {
				this->health -= 2;
			}
			else if (this->enemies[i].getFillColor() == sf::Color::Green) {
				this->health -= 1;
			}

			//Delete Enemy
			this->enemies.erase(this->enemies.begin() + i);
		}
	}

	//If You click the mouse
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

		//If the mouse is not being held down
		if (!this->mouseHeld) {
			this->mouseHeld = true; //set by default to assume click is being held down & stops 1ce
			bool deleted = false;
			for (size_t i = 0; i < this->enemies.size() && deleted == false; i++) {
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView)) {
					
					//Gain Points
					if (this->enemies[i].getFillColor() == sf::Color::Red) {
						this->points += 10;
					}
					else if (this->enemies[i].getFillColor() == sf::Color(75, 0, 130)) {
						this->points += 8;
					}
					else if (this->enemies[i].getFillColor() == sf::Color::Blue) {
						this->points += 6;
					}
					else if (this->enemies[i].getFillColor() == sf::Color::Yellow) {
						this->points += 4;
					}
					else if (this->enemies[i].getFillColor() == sf::Color::Green) {
						this->points += 2;
					}

					//Delete the Enemy
					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);
				}
			}
		}
	}
	
	//If mouse isnt being clicked, then we know mouse isnt being held either
	else {
		this->mouseHeld = false;
	}
}

void Game::updateText() {
	/* Updates Points & Health Text in Top Left Screen */
	std::stringstream ss; //allows you to combine strings of ints, etc into 1 string via a stream
	ss << "Points: " << this->points << std::endl;
	ss << "Health: " << this->health << std::endl;

	this->uiText.setString(ss.str());
}

void Game::renderEnemies(sf::RenderTarget& target) {
	/* Renders all enemies onto the screen*/
	for (auto& e : this->enemies) { //range based for loop
		target.draw(e);
		//this->window->draw(e);
	}
}

void Game::renderText(sf::RenderTarget& target) {
	/* Renders text onto the screen	*/
	this->uiText.setPosition(0, 0);
	target.draw(this->uiText);
}



//=== Accessor Functions ===//
const bool Game::isRunning() const {
	/* Detects if Window is Open */
	return this->window->isOpen();
}

const bool Game::getEndgame() const {
	/* Detects if Game has ended */
	return this->endgame;
}

const void Game::getPoints() const {
	std::cout << "Total Points: " << this->points << std::endl;
}

void Game::renderGameOver() {
	this->window->clear(sf::Color::Black);
	this->GameOverText(); //Display Game Over with Total Points
	this->window->display();
}

void Game::GameOverText() {
	/* Prints Game Over & Total Points for User */
	std::stringstream ss;
	ss << "GAME OVER!!!" << std::endl;
	ss << "Total Points: " << this->points << std::endl << std::endl;
	ss << "[R] to Reset  |  [ESC] to Close";

	this->uiText.setFillColor(sf::Color::Red);
	this->uiText.setCharacterSize(60);
	this->uiText.setString(ss.str());
	//Sets top left of text in the center of the window, then shifts the text backwards by half of the text width and size ==> "Centered Text"
	this->uiText.setPosition((this->window->getSize().x / 2) - (this->uiText.getGlobalBounds().width / 2), (this->window->getSize().y / 2) - (this->uiText.getGlobalBounds().height / 1.5));
	this->window->draw(this->uiText);
}

void Game::renderIntro() {
	this->window->clear(sf::Color(177, 129, 138));
	
	//Set Title Screen
	std::stringstream ss;
	ss << "Sinking Shapes!!!" << std::endl;
	this->introText.setString(ss.str());

	//Set Game Controls
	ss.str("");
	ss << "RAHHHHH! Shapes are falling from the skyyy!" << std::endl;
	ss << "[CLICK] on the shapes before they reach the bottom to save yourself!" << std::endl;
	ss << "Be careful though, the smaller the runt, the bigger the punch." << std::endl;
	ss << "Good luck >:]" << std::endl << std::endl;
	ss << "To play, press [ENTER]" << std::endl << std::endl;
	ss << "By: VVV (Trufflz)" << std::endl;
	this->uiText.setCharacterSize(24);
	this->uiText.setString(ss.str());

	//Set Positions
	this->introText.setPosition((this->window->getSize().x / 2) - (this->introText.getGlobalBounds().width / 2), (this->window->getSize().y / 2) - (this->introText.getGlobalBounds().width / 2));
	this->uiText.setPosition((this->window->getSize().x / 2) - (this->uiText.getGlobalBounds().width / 2), (this->window->getSize().y / 2.5));

	//Render text onto screen
	this->window->draw(this->introText);
	this->window->draw(this->uiText);
	//Display
	this->window->display();
}

const bool Game::getIntro() const {
	return this->intro;
}