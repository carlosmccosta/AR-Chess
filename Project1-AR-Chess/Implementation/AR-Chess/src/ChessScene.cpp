#include "ChessScene.h"

ChessScene::ChessScene() {}
ChessScene::~ChessScene() {}


void ChessScene::startGame() {
	setupScene();
}


void ChessScene::endGame() {
}


void ChessScene::setupScene() {
	// OSGART INIT
	setupViewer();
	setupARTrackers();
	setupLights();

	//APPLICATION INIT
	setupBoard();
	setupSelector();

	//BOOTSTRAP INIT
	viewer.setSceneData(scene);	


	//-- SOLUTION 1
	// MAIN LOOP & EXIT CLEANUP
	viewer.run();


	//-- SOLUTION 2	
	//create,display the view
	//viewer.realize();

	// MAIN LOOP
	// viewer.done exit on a press key
	//while (!viewer.done()) {		
	//	// update, culling, and draw traversal
	//	viewer.frame();
	//}

	//EXIT CLEANUP
}


void ChessScene::setupViewer() {	
	//setup default threading mode
	viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);

	// add relevant handlers to the viewer
	viewer.addEventHandler(new osgViewer::StatsHandler);		//stats, press 's'
	viewer.addEventHandler(new osgViewer::WindowSizeHandler);	//resize, full screen 'f'
	viewer.addEventHandler(new osgViewer::ThreadingHandler);	//threading mode, press 't'
	viewer.addEventHandler(new osgViewer::HelpHandler);			//help menu, press 'h'	
}


void ChessScene::setupARTrackers() {
	//AR SCENEGRAPH INIT
	scene = new osgART::Scene();
	if (scene->addVideo(VIDEO_PLUGIN, VIDEO_SOURCE, VIDEO_CONFIG) == NULL) {
		osg::notify(osg::FATAL) << "Could not initialize video plug-in!" << std::endl;
		exit(-1);
	}

	if (scene->addVisualTracker(VIDEO_PLUGIN, TRACKER_PLUGIN, CAMERA_CONFIG) == NULL) {
		osg::notify(osg::FATAL) << "Could not initialize tracker plug-in!" << std::endl;
		exit(-2);
	}
	
	//or for being able to further add/modify the target transformation:
	boardTrackerMT = scene->addTrackedTransform(TRACKER_BOARD_CONFIG);
	selectorTrackerMT = scene->addTrackedTransform(TRACKER_SELECTOR_CONFIG);

	osgGA::TrackballManipulator* tbManipulator = new osgGA::TrackballManipulator();
	tbManipulator->setHomePosition(Vec3f(0, -100, -100), Vec3f(0, 0, 0), Vec3f(0, 0, 1));
	viewer.setCameraManipulator(tbManipulator);
}


void ChessScene::setupBoard() {			
	boardTrackerMT->addChild(createOSGModel(BOARD_MODEL, BOARD_SIZE));
	setupPieces();
}


void ChessScene::setupPieces() {
	double halfSquareSize = BOARD_SQUARE_SIZE / 2.0;	
	double halfPieceSize = PIECE_HEIGHT / 2.0;
	double heightOffset = BOARD_HEIGHT;

	// kings
	boardTrackerMT->addChild(createOSGModel(PIECE_BLACK_KING, PIECE_SIZE, halfSquareSize,  3 * BOARD_SQUARE_SIZE + halfSquareSize, heightOffset));
	boardTrackerMT->addChild(createOSGModel(PIECE_WHITE_KING, PIECE_SIZE, halfSquareSize, -3 * BOARD_SQUARE_SIZE - halfSquareSize, heightOffset));
	
	//queens
	boardTrackerMT->addChild(createOSGModel(PIECE_BLACK_QUEEN, PIECE_SIZE, -halfSquareSize,  3 * BOARD_SQUARE_SIZE + halfSquareSize, heightOffset));
	boardTrackerMT->addChild(createOSGModel(PIECE_WHITE_QUEEN, PIECE_SIZE, -halfSquareSize, -3 * BOARD_SQUARE_SIZE - halfSquareSize, heightOffset));

	// rooks
	boardTrackerMT->addChild(createOSGModel(PIECE_BLACK_ROOK, PIECE_SIZE, -3 * BOARD_SQUARE_SIZE - halfSquareSize,  3 * BOARD_SQUARE_SIZE + halfSquareSize, heightOffset));
	boardTrackerMT->addChild(createOSGModel(PIECE_BLACK_ROOK, PIECE_SIZE,  3 * BOARD_SQUARE_SIZE + halfSquareSize,  3 * BOARD_SQUARE_SIZE + halfSquareSize, heightOffset));
	boardTrackerMT->addChild(createOSGModel(PIECE_WHITE_ROOK, PIECE_SIZE, -3 * BOARD_SQUARE_SIZE - halfSquareSize, -3 * BOARD_SQUARE_SIZE - halfSquareSize, heightOffset));
	boardTrackerMT->addChild(createOSGModel(PIECE_WHITE_ROOK, PIECE_SIZE,  3 * BOARD_SQUARE_SIZE + halfSquareSize, -3 * BOARD_SQUARE_SIZE - halfSquareSize, heightOffset));

	// knights
	boardTrackerMT->addChild(createOSGModel(PIECE_BLACK_KNIGHT, PIECE_SIZE, -2 * BOARD_SQUARE_SIZE - halfSquareSize,  3 * BOARD_SQUARE_SIZE + halfSquareSize, heightOffset, osg::PI, Vec3f(0,0,1)));
	boardTrackerMT->addChild(createOSGModel(PIECE_BLACK_KNIGHT, PIECE_SIZE,  2 * BOARD_SQUARE_SIZE + halfSquareSize,  3 * BOARD_SQUARE_SIZE + halfSquareSize, heightOffset));
	boardTrackerMT->addChild(createOSGModel(PIECE_WHITE_KNIGHT, PIECE_SIZE, -2 * BOARD_SQUARE_SIZE - halfSquareSize, -3 * BOARD_SQUARE_SIZE - halfSquareSize, heightOffset, osg::PI, Vec3f(0,0,1)));
	boardTrackerMT->addChild(createOSGModel(PIECE_WHITE_KNIGHT, PIECE_SIZE,  2 * BOARD_SQUARE_SIZE + halfSquareSize, -3 * BOARD_SQUARE_SIZE - halfSquareSize, heightOffset));

	// bishops
	boardTrackerMT->addChild(createOSGModel(PIECE_BLACK_BISHOP, PIECE_SIZE, -BOARD_SQUARE_SIZE - halfSquareSize,  3 * BOARD_SQUARE_SIZE + halfSquareSize, heightOffset));
	boardTrackerMT->addChild(createOSGModel(PIECE_BLACK_BISHOP, PIECE_SIZE,  BOARD_SQUARE_SIZE + halfSquareSize,  3 * BOARD_SQUARE_SIZE + halfSquareSize, heightOffset));
	boardTrackerMT->addChild(createOSGModel(PIECE_WHITE_BISHOP, PIECE_SIZE, -BOARD_SQUARE_SIZE - halfSquareSize, -3 * BOARD_SQUARE_SIZE - halfSquareSize, heightOffset));
	boardTrackerMT->addChild(createOSGModel(PIECE_WHITE_BISHOP, PIECE_SIZE,  BOARD_SQUARE_SIZE + halfSquareSize, -3 * BOARD_SQUARE_SIZE - halfSquareSize, heightOffset));

	double pawnsSize = PIECE_SIZE * 0.65;

	// paws
	for (size_t pawnPosition = 0; pawnPosition < 4; ++pawnPosition) {
		boardTrackerMT->addChild(createOSGModel(PIECE_BLACK_PAWN, pawnsSize, -(pawnPosition * BOARD_SQUARE_SIZE + halfSquareSize),  2 * BOARD_SQUARE_SIZE + halfSquareSize, heightOffset));
		boardTrackerMT->addChild(createOSGModel(PIECE_BLACK_PAWN, pawnsSize,   pawnPosition * BOARD_SQUARE_SIZE + halfSquareSize,   2 * BOARD_SQUARE_SIZE + halfSquareSize, heightOffset));
		boardTrackerMT->addChild(createOSGModel(PIECE_WHITE_PAWN, pawnsSize, -(pawnPosition * BOARD_SQUARE_SIZE + halfSquareSize), -2 * BOARD_SQUARE_SIZE - halfSquareSize, heightOffset));
		boardTrackerMT->addChild(createOSGModel(PIECE_WHITE_PAWN, pawnsSize,   pawnPosition * BOARD_SQUARE_SIZE + halfSquareSize,  -2 * BOARD_SQUARE_SIZE - halfSquareSize, heightOffset));
	}
}


void ChessScene::setupSelector() {
}


Node* ChessScene::setupLights() {
	Group* lightGroup = new Group();
	double offset = BOARD_SIZE / 2.0;
	double boardHalfSize = BOARD_SIZE / 2.0;

	lightGroup->addChild(createLightSource(scene->getOrCreateStateSet(), Vec4(BOARD_SIZE, 0, boardHalfSize, 0), 1));

	/*lightGroup->addChild(createLightSource(rootStateSet, Vec4(-offset, -offset, heightPosition, 0), 1));
	lightGroup->addChild(createLightSource(rootStateSet, Vec4(-offset, BOARD_SIZE + offset, heightPosition, 0), 2));
	lightGroup->addChild(createLightSource(rootStateSet, Vec4(BOARD_SIZE + offset, -offset, heightPosition, 0), 3));
	lightGroup->addChild(createLightSource(rootStateSet, Vec4(BOARD_SIZE + offset, BOARD_SIZE + offset, heightPosition, 0), 4));*/
	
	return lightGroup;	
}


Node* ChessScene::createLightSource(StateSet* rootStateSet, Vec4 position, int lightNumber) {
	Light* light = new Light();
	light->setLightNum(lightNumber);
	light->setPosition(position);
	light->setAmbient(Vec4(0.0f, 0.0f, 0.0f, 0.0f));
	light->setDiffuse(Vec4(0.1f, 0.1f, 0.1f, 0.1f));
	light->setSpecular(Vec4(0.6f, 0.6f, 0.6f, 0.6f));
	light->setConstantAttenuation(1.0f);

	LightSource* lightS = new LightSource();
	lightS->setLight(light);
	lightS->setLocalStateSetModes(osg::StateAttribute::ON); 
	lightS->setStateSetModes(*rootStateSet, osg::StateAttribute::ON);

	return lightS;
}


Node* ChessScene::createOSGModel(string name, float modelSize, float shiftX, float shiftY, float shiftZ, double rotationAngle, Vec3f rotationAxis, bool centerX, bool centerY, bool centerZ) {
	// create a new node by reading in model from file
	Node* modelNode = osgDB::readNodeFile(name);
	if (modelNode != NULL) {
		//put model in origin
		Matrix modelScale;
		Matrix modelRotate;
		Matrix modelTranslate;		

		MatrixTransform* unitTransform = new MatrixTransform();
		osg::BoundingSphere bound = modelNode->getBound();
		double scaleRatio = modelSize / bound.radius();
		modelScale.makeScale(scaleRatio, scaleRatio, scaleRatio);
		modelRotate.makeRotate(rotationAngle, rotationAxis);
		modelTranslate.makeTranslate((centerX? -bound.center().x() : 0), (centerY? -bound.center().y() : 0), (centerZ? -bound.center().z() : 0));

		unitTransform->postMult(modelTranslate);
		unitTransform->postMult(modelRotate);
		unitTransform->postMult(modelScale);
		unitTransform->addChild(modelNode);
	
		// put model in specified location
		MatrixTransform* trackerCenterTransform = new MatrixTransform();
		Matrix shiftTranslation;
		shiftTranslation.makeTranslate(shiftX, shiftY, shiftZ);
		trackerCenterTransform->postMult(shiftTranslation);
		trackerCenterTransform->addChild(unitTransform);

		MatrixTransform* modelMatrixTransform = new MatrixTransform();
		modelMatrixTransform->addChild(trackerCenterTransform);
		return modelMatrixTransform;
	}

	return NULL;
}
