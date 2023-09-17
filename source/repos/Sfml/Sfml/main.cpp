#include <iostream>
#include "Game.h"

using namespace std;

int main() {
	//Initializing the Game & Rand
	Game game;
	srand(static_cast<unsigned>(time(NULL)));

	// Game Loop
	while (game.isRunning() && !game.getEndgame()) {
		game.update(); // Update the Game
		game.render(); // Render the Game onto the Screen
	}

	return 0;
}