#include <osgART/Foundation>

#include "Scene.h"


int main(int argc, char* argv[]) {
	osgARTInit(&argc, argv);

	Scene scene;
	scene.startGame();
	scene.endGame();
}
