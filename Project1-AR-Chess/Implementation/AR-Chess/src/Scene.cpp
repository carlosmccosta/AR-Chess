#include "Scene.h"


Scene::Scene() {}
Scene::~Scene() {}


void Scene::startGame() {
	setupScene();	
	videoPlugin->start();

	while (!viewer.done()) {
		viewer.sync();	
		videoPlugin->update();

		trackerPlugin->setImage(videoPlugin.get());
		trackerPlugin->update();

		viewer.update();
		viewer.frame();
	}
}


void Scene::endGame() {
	viewer.sync();
	viewer.cleanup_frame();
	viewer.sync();

	videoPlugin->stop();
	videoPlugin->close();
}


void Scene::setupScene() {
	osg::setNotifyLevel(osg::ALWAYS);	
	
	viewer.setUpViewer(osgProducer::Viewer::ESCAPE_SETS_DONE);
	viewer.getCullSettings().setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);

#ifndef __linux	
	viewer.getCamera(0)->getRenderSurface()->fullScreen(false);
#endif
	
	videoPlugin = osgART::VideoManager::createVideoFromPlugin(VIDEO_PLUGIN);
	if (!videoPlugin.valid()) {
		// without video an AR application can not work
		osg::notify(osg::FATAL) << "Could not initialize video!" << std::endl;
		exit(1);
	}
	
	osgART::VideoConfiguration* _config = videoPlugin->getVideoConfiguration();	
	if (_config) {
		_config->deviceconfig = VIDEO_CONFIGURATION;
	}
	
	trackerPlugin = osgART::TrackerManager::createTrackerFromPlugin(TRACKER_PLUGIN);	
	videoPlugin->open();
	trackerPlugin->init(videoPlugin->getWidth(), videoPlugin->getHeight(), TRACKER_MARKERS_CONFIG);
	
	setupBoard();
	setupSelector();	

	viewer.realize();
}


void Scene::setupBoard() {		
	osgART::VideoBackground* videoBackground = new osgART::VideoBackground(videoPlugin.get());
	videoBackground->setTextureMode(osgART::GenericVideoObject::USE_TEXTURE_RECTANGLE);
	videoBackground->init();

	Group* foregroundGroup	= new Group();
	foregroundGroup->addChild(videoBackground);
	foregroundGroup->getOrCreateStateSet()->setRenderBinDetails(2, "RenderBin");

	Projection* projectionMatrix = new Projection(Matrix(trackerPlugin->getProjectionMatrix()));

	// create marker with id number '0'
	ref_ptr<osgART::Marker> boardMarker = trackerPlugin->getMarker(0);	
	if (!boardMarker.valid()) {
		osg::notify(osg::FATAL) << "No Marker defined!" << std::endl;
		exit(-1);
	}
	
	boardMarker->setActive(true);

	// create a matrix transform related to the marker
	ref_ptr<MatrixTransform> boardMarkerTransform = new osgART::ARTTransform(boardMarker.get());
	boardMarkerTransform->addChild(createOSGModel(BOARD_MODEL, 200));
	setupPieces(boardMarkerTransform);

	Group* sceneGroup = new Group();
	sceneGroup->getOrCreateStateSet()->setRenderBinDetails(5, "RenderBin");
	sceneGroup->addChild(boardMarkerTransform.get());
	foregroundGroup->addChild(sceneGroup);

	//normalize for environment mapping
	sceneGroup->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);

	//activate lighting
	sceneGroup->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);

	//add a light model
	sceneGroup->getOrCreateStateSet()->setAttribute(new osg::LightModel());

	//add the directional light to the scene
	sceneGroup->addChild(setupLights(sceneGroup->getOrCreateStateSet()));

	MatrixTransform* modelViewMatrix = new osg::MatrixTransform();
	modelViewMatrix->addChild(foregroundGroup);
	projectionMatrix->addChild(modelViewMatrix);

	Group* root = new Group();
	root->addChild(projectionMatrix);

	viewer.setSceneData(root);
}


void Scene::setupPieces( ref_ptr<MatrixTransform> boardMarkerTransform ) {
	double halfSquareSize = BOARD_SQUARE_SIZE / 2.0;	
	double halfPieceSize = PIECE_HEIGHT / 2.0;
	double heightOffset = BOARD_HEIGHT;

	// kings
	boardMarkerTransform->addChild(createOSGModel(PIECE_BLACK_KING, PIECE_SIZE, halfSquareSize,  3 * BOARD_SQUARE_SIZE + halfSquareSize, heightOffset));
	boardMarkerTransform->addChild(createOSGModel(PIECE_WHITE_KING, PIECE_SIZE, halfSquareSize, -3 * BOARD_SQUARE_SIZE - halfSquareSize, heightOffset));
	
	//queens
	boardMarkerTransform->addChild(createOSGModel(PIECE_BLACK_QUEEN, PIECE_SIZE, -halfSquareSize,  3 * BOARD_SQUARE_SIZE + halfSquareSize, heightOffset));
	boardMarkerTransform->addChild(createOSGModel(PIECE_WHITE_QUEEN, PIECE_SIZE, -halfSquareSize, -3 * BOARD_SQUARE_SIZE - halfSquareSize, heightOffset));

	// rooks
	boardMarkerTransform->addChild(createOSGModel(PIECE_BLACK_ROOK, PIECE_SIZE, -3 * BOARD_SQUARE_SIZE - halfSquareSize,  3 * BOARD_SQUARE_SIZE + halfSquareSize, heightOffset));
	boardMarkerTransform->addChild(createOSGModel(PIECE_BLACK_ROOK, PIECE_SIZE,  3 * BOARD_SQUARE_SIZE + halfSquareSize,  3 * BOARD_SQUARE_SIZE + halfSquareSize, heightOffset));
	boardMarkerTransform->addChild(createOSGModel(PIECE_WHITE_ROOK, PIECE_SIZE, -3 * BOARD_SQUARE_SIZE - halfSquareSize, -3 * BOARD_SQUARE_SIZE - halfSquareSize, heightOffset));
	boardMarkerTransform->addChild(createOSGModel(PIECE_WHITE_ROOK, PIECE_SIZE,  3 * BOARD_SQUARE_SIZE + halfSquareSize, -3 * BOARD_SQUARE_SIZE - halfSquareSize, heightOffset));

	// knights
	boardMarkerTransform->addChild(createOSGModel(PIECE_BLACK_KNIGHT, PIECE_SIZE, -2 * BOARD_SQUARE_SIZE - halfSquareSize,  3 * BOARD_SQUARE_SIZE + halfSquareSize, heightOffset, osg::PI, Vec3f(0,0,1)));
	boardMarkerTransform->addChild(createOSGModel(PIECE_BLACK_KNIGHT, PIECE_SIZE,  2 * BOARD_SQUARE_SIZE + halfSquareSize,  3 * BOARD_SQUARE_SIZE + halfSquareSize, heightOffset));
	boardMarkerTransform->addChild(createOSGModel(PIECE_WHITE_KNIGHT, PIECE_SIZE, -2 * BOARD_SQUARE_SIZE - halfSquareSize, -3 * BOARD_SQUARE_SIZE - halfSquareSize, heightOffset, osg::PI, Vec3f(0,0,1)));
	boardMarkerTransform->addChild(createOSGModel(PIECE_WHITE_KNIGHT, PIECE_SIZE,  2 * BOARD_SQUARE_SIZE + halfSquareSize, -3 * BOARD_SQUARE_SIZE - halfSquareSize, heightOffset));

	// bishops
	boardMarkerTransform->addChild(createOSGModel(PIECE_BLACK_BISHOP, PIECE_SIZE, -BOARD_SQUARE_SIZE - halfSquareSize,  3 * BOARD_SQUARE_SIZE + halfSquareSize, heightOffset));
	boardMarkerTransform->addChild(createOSGModel(PIECE_BLACK_BISHOP, PIECE_SIZE,  BOARD_SQUARE_SIZE + halfSquareSize,  3 * BOARD_SQUARE_SIZE + halfSquareSize, heightOffset));
	boardMarkerTransform->addChild(createOSGModel(PIECE_WHITE_BISHOP, PIECE_SIZE, -BOARD_SQUARE_SIZE - halfSquareSize, -3 * BOARD_SQUARE_SIZE - halfSquareSize, heightOffset));
	boardMarkerTransform->addChild(createOSGModel(PIECE_WHITE_BISHOP, PIECE_SIZE,  BOARD_SQUARE_SIZE + halfSquareSize, -3 * BOARD_SQUARE_SIZE - halfSquareSize, heightOffset));

	double pawnsSize = PIECE_SIZE * 0.65;

	// paws
	for (size_t pawnPosition = 0; pawnPosition < 4; ++pawnPosition) {
		boardMarkerTransform->addChild(createOSGModel(PIECE_BLACK_PAWN, pawnsSize, -(pawnPosition * BOARD_SQUARE_SIZE + halfSquareSize),  2 * BOARD_SQUARE_SIZE + halfSquareSize, heightOffset));
		boardMarkerTransform->addChild(createOSGModel(PIECE_BLACK_PAWN, pawnsSize,   pawnPosition * BOARD_SQUARE_SIZE + halfSquareSize,   2 * BOARD_SQUARE_SIZE + halfSquareSize, heightOffset));
		boardMarkerTransform->addChild(createOSGModel(PIECE_WHITE_PAWN, pawnsSize, -(pawnPosition * BOARD_SQUARE_SIZE + halfSquareSize), -2 * BOARD_SQUARE_SIZE - halfSquareSize, heightOffset));
		boardMarkerTransform->addChild(createOSGModel(PIECE_WHITE_PAWN, pawnsSize,   pawnPosition * BOARD_SQUARE_SIZE + halfSquareSize,  -2 * BOARD_SQUARE_SIZE - halfSquareSize, heightOffset));
	}	
}


void Scene::setupSelector() {

}


Node* Scene::setupLights(StateSet* rootStateSet) {
	Group* lightGroup = new Group();
	double offset = BOARD_SIZE / 2.0;
	double heightPosition = BOARD_SIZE / 2.0;

	lightGroup->addChild(createLightSource(rootStateSet, Vec4(-offset, -offset, heightPosition, 0), 1));
	lightGroup->addChild(createLightSource(rootStateSet, Vec4(-offset, BOARD_SIZE + offset, heightPosition, 0), 2));
	lightGroup->addChild(createLightSource(rootStateSet, Vec4(BOARD_SIZE + offset, -offset, heightPosition, 0), 3));
	lightGroup->addChild(createLightSource(rootStateSet, Vec4(BOARD_SIZE + offset, BOARD_SIZE + offset, heightPosition, 0), 4));
	
	return lightGroup;
}



Node* Scene::createLightSource(StateSet* rootStateSet, Vec4 position, int lightNumber) {
	Light* light = new Light();
	light->setLightNum(lightNumber);
	light->setPosition(position);
	light->setAmbient(Vec4(0.0f, 0.0f, 0.0f, 1.0f));
	light->setDiffuse(Vec4(0.1f, 0.1f, 0.1f, 1.0f));
	light->setSpecular(Vec4(0.6f, 0.6f, 0.6f, 1.0f));
	light->setConstantAttenuation(1.0f);

	LightSource* lightS = new LightSource();
	lightS->setLight(light);
	lightS->setLocalStateSetModes(StateAttribute::ON); 
	lightS->setStateSetModes(*rootStateSet, StateAttribute::ON);

	return lightS;
}




Node* Scene::createOSGModel(string name, float modelSize, float shiftX, float shiftY, float shiftZ, double rotationAngle, Vec3f rotationAxis, bool centerX, bool centerY, bool centerZ) {
	// create a new node by reading in model from file
	Node* modelNode = osgDB::readNodeFile(name);
	if (modelNode != NULL) {
		//put model in origin
		Matrix modelScale;
		Matrix modelTranslate;
		Matrix modelRotate;

		MatrixTransform* unitTransform = new MatrixTransform();
		BoundingSphere bound = modelNode->getBound();
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

	return modelNode;
}
