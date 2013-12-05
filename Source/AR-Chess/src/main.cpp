#include "ChessScene.h"


int main(int argc, char* argv[]) {	
	// ChessScene smart pointer to control game flow
	ChessScene* scene = new ChessScene();
	scene->startGame();
	scene->endGame();
}
