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
	_viewer.setSceneData(_scene);	
	
	// MAIN LOOP & EXIT CLEANUP
	_viewer.run();
}


void ChessScene::setupViewer() {	
	//setup default threading mode
	_viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);

	// add relevant handlers to the viewer
	_viewer.addEventHandler(new osgViewer::StatsHandler);		//stats, press 's'
	_viewer.addEventHandler(new osgViewer::WindowSizeHandler);	//resize, full screen 'f'
	_viewer.addEventHandler(new osgViewer::ThreadingHandler);	//threading mode, press 't'
	_viewer.addEventHandler(new osgViewer::HelpHandler);		//help menu, press 'h'	
}


void ChessScene::setupARTrackers() {
	//AR SCENEGRAPH INIT
	_scene = new osgART::Scene();
	if (_scene->addVideo(VIDEO_PLUGIN, VIDEO_SOURCE, VIDEO_CONFIG) == NULL) {
		osg::notify(osg::FATAL) << "Could not initialize video plug-in!" << std::endl;
		exit(-1);
	}

	if (_scene->addVisualTracker(VIDEO_PLUGIN, TRACKER_PLUGIN, CAMERA_CONFIG) == NULL) {
		osg::notify(osg::FATAL) << "Could not initialize tracker plug-in!" << std::endl;
		exit(-2);
	}
	
	//or for being able to further add/modify the target transformation:
	_boardTrackerMT = _scene->addTrackedTransform(TRACKER_BOARD_CONFIG);
	_selectorTrackerMT = _scene->addTrackedTransform(TRACKER_SELECTOR_CONFIG);

	osgGA::TrackballManipulator* tbManipulator = new osgGA::TrackballManipulator();
	tbManipulator->setHomePosition(Vec3f(0, -100, -100), Vec3f(0, 0, 0), Vec3f(0, 0, 1));
	_viewer.setCameraManipulator(tbManipulator);
}


void ChessScene::setupBoard() {			
	_boardTrackerMT->addChild(_gameBoard.setupBoard());	
}


void ChessScene::setupSelector() {
}


Group* ChessScene::setupLights() {
	Group* lightGroup = new Group();
	double offset = BOARD_SIZE;
	double boardHalfSize = BOARD_SIZE / 2.0;

	lightGroup->addChild(ChessUtils::createLightSource(_scene->getOrCreateStateSet(), 1, Vec4(offset, 0, boardHalfSize, 0)));
	
	return lightGroup;	
}
