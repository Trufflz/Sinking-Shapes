#include <iostream>
#include "Game.h"

using namespace std;

int main() {
	//Initializing the Game & Rand
	Game game;
	srand(static_cast<unsigned>(time(NULL)));

	// Game Loop
	while (game.isRunning()) {
		game.pollEvents();

		//intro
		if (!game.getEndgame() && game.getIntro()) {
			game.renderIntro();
		}
		
		//during game
		else if (!game.getEndgame()){
			game.update(); // Update the Game
			game.render(); // Render the Game onto the Screen
		}

		//endgame
		else {
			game.renderGameOver();
		}
	}

	return 0;
}