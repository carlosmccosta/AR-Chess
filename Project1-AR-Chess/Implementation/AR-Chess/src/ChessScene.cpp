#include "ChessScene.h"

ChessScene::ChessScene() {
	_rootNode = new osgShadow::ShadowedScene();
	_rootNode->setReceivesShadowTraversalMask(RECEIVE_SHADOW_MASK);
	_rootNode->setCastsShadowTraversalMask(CAST_SHADOW_MASK);

	_boardScene = new osgART::Scene();
	_lightGroup = new Group();
}

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
	_viewer.setSceneData(_rootNode);
	
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
	if (_boardScene->addVideo(VIDEO_PLUGIN, VIDEO_SOURCE, VIDEO_CONFIG) == NULL) {
		osg::notify(osg::FATAL) << "Could not initialize video plug-in!" << std::endl;
		exit(-1);
	}

	if (_boardScene->addVisualTracker(VIDEO_PLUGIN, TRACKER_PLUGIN, CAMERA_CONFIG) == NULL) {
		osg::notify(osg::FATAL) << "Could not initialize tracker plug-in!" << std::endl;
		exit(-2);
	}
	
	//or for being able to further add/modify the target transformation:
	_boardTrackerMT = _boardScene->addTrackedTransform(TRACKER_BOARD_CONFIG);
	_selectorTrackerMT = _boardScene->addTrackedTransform(TRACKER_SELECTOR_CONFIG);	

	osgGA::TrackballManipulator* tbManipulator = new osgGA::TrackballManipulator();
	tbManipulator->setHomePosition(Vec3f(0, -100, -100), Vec3f(0, 0, 0), Vec3f(0, 0, 1));
	_viewer.setCameraManipulator(tbManipulator);
}


void ChessScene::setupBoard() {
	MatrixTransform* boardMT = _gameBoard.setupBoard();	

	/*_specularHighlights = new osgFX::SpecularHighlights();
	_specularHighlights->setTextureUnit(0);
	_specularHighlights->setLightNumber(0);
	_specularHighlights->setSpecularColor(osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f));
	_specularHighlights->setSpecularExponent(16.0f);
	_specularHighlights->addChild(boardMT);*/

	/*_anisotropicLighting = new osgFX::AnisotropicLighting();
	_anisotropicLighting->setLightNumber(0);
	_anisotropicLighting->setLightingMap(new osg::Image());
	_anisotropicLighting->addChild(boardMT);*/

	_boardTrackerMT->addChild(boardMT);
}


void ChessScene::setupSelector() {
}


void ChessScene::setupLights() {	
	StateSet* stateSet = _boardTrackerMT->getOrCreateStateSet();
	//stateSet->setMode(GL_LIGHT0, osg::StateAttribute::ON);
	
	_mainLightSource = ChessUtils::createLightSource(stateSet, 0, Vec4(30, 0, 25, 1.0), Vec3(0, 0, -1));

	/*osg::Geode* geode = new osg::Geode();
	osg::ShapeDrawable* sd = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0, 0, 10), 10));
	sd->setColor(Vec4(1, 1, 0, 1));
	geode->addDrawable(sd);
	geode->setNodeMask(CAST_SHADOW_MASK);
	_mainLightSource->addChild(geode);*/

	float supportLightsOffset = BOARD_SIZE * 0.75;
	float supportLightsHeight = 10;
	_lightGroup->addChild(_mainLightSource);	
	_lightGroup->addChild(ChessUtils::createLightSource(stateSet, 1, Vec4(-supportLightsOffset, -supportLightsOffset, supportLightsHeight, 1.0), Vec3(supportLightsOffset, supportLightsOffset, -supportLightsHeight)));
	_lightGroup->addChild(ChessUtils::createLightSource(stateSet, 2, Vec4(-supportLightsOffset, supportLightsOffset, supportLightsHeight, 1.0), Vec3(supportLightsOffset, -supportLightsOffset, -supportLightsHeight)));
	_lightGroup->addChild(ChessUtils::createLightSource(stateSet, 3, Vec4(supportLightsOffset, supportLightsOffset, supportLightsHeight, 1.0), Vec3(-supportLightsOffset, -supportLightsOffset, -supportLightsHeight)));
	_lightGroup->addChild(ChessUtils::createLightSource(stateSet, 4, Vec4(supportLightsOffset, -supportLightsOffset, supportLightsHeight, 1.0), Vec3(-supportLightsOffset, supportLightsOffset, -supportLightsHeight)));
	_boardTrackerMT->addChild(_lightGroup);	

	_rootNode->addChild(_boardScene);

	osgShadow::ShadowMap* shadowMap = new osgShadow::ShadowMap();
	shadowMap->setLight(_mainLightSource->getLight());
	shadowMap->setTextureSize(osg::Vec2s(1024, 1024));
	shadowMap->setTextureUnit(1);

	//osgShadow::ViewDependentShadowMap* shadowMap = new osgShadow::ViewDependentShadowMap();	
	_rootNode->setShadowTechnique(shadowMap);	
}
