#include "ChessScene.h"

ChessScene::ChessScene() {
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
	_boardScene->setDataVariance(osg::Object::DYNAMIC);
	_boardScene->setUserData(this);
	_boardScene->setUpdateCallback(new ChessSceneNodeCallback());
	_viewer.setSceneData(_boardScene);
	
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
	osgShadow::ShadowedScene* boardShadowedScene = _gameBoard.setupBoard();
	
	osgShadow::ShadowMap* shadowMap = new osgShadow::ShadowMap();
	int textureMapResolution = 1024 * 10;
	shadowMap->setTextureSize(osg::Vec2s(textureMapResolution, textureMapResolution));
	//shadowMap->setTextureUnit(1);
	shadowMap->setLight(_mainLightSource->getLight());
	boardShadowedScene->setShadowTechnique(shadowMap);

	/*osg::LightModel* lightModel = new osg::LightModel();
	lightModel->setColorControl(osg::LightModel::ColorControl::SEPARATE_SPECULAR_COLOR);
	boardShadowedScene->getOrCreateStateSet()->setAttributeAndModes(lightModel, osg::StateAttribute::ON);*/
	
	/*_specularHighlights = new osgFX::SpecularHighlights();
	_specularHighlights->setTextureUnit(2);
	_specularHighlights->setLightNumber(0);
	_specularHighlights->setSpecularColor(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	_specularHighlights->setSpecularExponent(16.0f);
	_specularHighlights->addChild(boardShadowedScene);
	_boardTrackerMT->addChild(_specularHighlights);*/

	/*_anisotropicLighting = new osgFX::AnisotropicLighting();
	_anisotropicLighting->setLightNumber(0);
	_anisotropicLighting->setLightingMap(new osg::Image());
	_anisotropicLighting->addChild(boardShadowedScene);
	_boardTrackerMT->addChild(_anisotropicLighting);*/

	_boardTrackerMT->addChild(boardShadowedScene);
}


void ChessScene::setupSelector() {
}


void ChessScene::setupLights() {	
	StateSet* stateSet = _boardTrackerMT->getOrCreateStateSet();
	//stateSet->setMode(GL_LIGHT0, osg::StateAttribute::ON);
	
	_mainLightSource = ChessUtils::createLightSource(stateSet, 0, Vec4(0, 0, BOARD_SIZE * 0.8, 1.0), Vec3(0, 0, -1));
	//_mainLightSource = ChessUtils::createLightSource(stateSet, 0, Vec4(BOARD_SIZE * 0.5, 0, BOARD_SIZE * 0.15, 1.0), Vec3(-BOARD_SIZE * 0.5, 0, -BOARD_SIZE * 0.08));

	/*osg::Geode* geode = new osg::Geode();
	osg::ShapeDrawable* sd = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0, 0, 10), 10));
	sd->setColor(Vec4(1, 1, 0, 1));
	geode->addDrawable(sd);
	geode->setNodeMask(CAST_SHADOW_MASK);
	_mainLightSource->addChild(geode);*/

	float supportLightsOffset = BOARD_SIZE * 0.75;
	float supportLightsHeight = 10;
	_lightGroup->addChild(_mainLightSource);
	_lightGroup->addChild(ChessUtils::createLightSource(stateSet, 1, Vec4(0, 0, BOARD_SIZE * 0.8, 1.0), Vec3(0, 0, -1), 22.5, 128, 1.0, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	_lightGroup->addChild(ChessUtils::createLightSource(stateSet, 1, Vec4(-supportLightsOffset, -supportLightsOffset, supportLightsHeight, 1.0), Vec3(supportLightsOffset, supportLightsOffset, -supportLightsHeight)));
	_lightGroup->addChild(ChessUtils::createLightSource(stateSet, 2, Vec4(-supportLightsOffset, supportLightsOffset, supportLightsHeight, 1.0), Vec3(supportLightsOffset, -supportLightsOffset, -supportLightsHeight)));
	_lightGroup->addChild(ChessUtils::createLightSource(stateSet, 3, Vec4(supportLightsOffset, supportLightsOffset, supportLightsHeight, 1.0), Vec3(-supportLightsOffset, -supportLightsOffset, -supportLightsHeight)));
	_lightGroup->addChild(ChessUtils::createLightSource(stateSet, 4, Vec4(supportLightsOffset, -supportLightsOffset, supportLightsHeight, 1.0), Vec3(-supportLightsOffset, supportLightsOffset, -supportLightsHeight)));
	/*_lightGroup->addChild(ChessUtils::createLightSource(stateSet, 1, Vec4(-supportLightsOffset, 0, supportLightsHeight, 1.0), Vec3(supportLightsOffset, 0, -supportLightsHeight)));
	_lightGroup->addChild(ChessUtils::createLightSource(stateSet, 2, Vec4(0, supportLightsOffset, supportLightsHeight, 1.0), Vec3(0, -supportLightsOffset, -supportLightsHeight)));
	_lightGroup->addChild(ChessUtils::createLightSource(stateSet, 3, Vec4(supportLightsOffset, 0, supportLightsHeight, 1.0), Vec3(-supportLightsOffset, 0, -supportLightsHeight)));
	_lightGroup->addChild(ChessUtils::createLightSource(stateSet, 4, Vec4(0, -supportLightsOffset, supportLightsHeight, 1.0), Vec3(0, supportLightsOffset, -supportLightsHeight)));*/

	_boardTrackerMT->addChild(_lightGroup);		
}


void ChessScene::updateScene() {
	Vec3 boardScenePosition = _boardTrackerMT->getMatrix().getTrans();
	Vec3 selectorScenePosition = _selectorTrackerMT->getMatrix().getTrans();	

	// only update if board and selector are visible
	/*if (boardScenePosition.x() != 0 && boardScenePosition.y() != 0 && boardScenePosition.z() != 0 &&
		selectorScenePosition.x() != 0 && selectorScenePosition.y() != 0 && selectorScenePosition.z() != 0) {*/
		Vec3 offset = selectorScenePosition - boardScenePosition;
		Vec3i selectorBoardPosition = ChessUtils::computePieceBoardPosition(offset);
		_gameBoard.updateBoard(selectorBoardPosition);
	/*} else {
		_gameBoard.clearHighlights();
	}*/
}


void ChessSceneNodeCallback::operator()(osg::Node* node, osg::NodeVisitor* nodeVisitor) {
	ChessScene* chessScene = dynamic_cast<ChessScene*> (node->getUserData());
	chessScene->updateScene();

	traverse(node, nodeVisitor);
}
