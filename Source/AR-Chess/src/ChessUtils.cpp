#include "ChessUtils.h"


Vec3f ChessUtils::computePieceScenePosition(int boardXPosition, int boardYPosition) {
	float shiftX = boardXPosition * BOARD_SQUARE_SIZE;
	float shiftY = boardYPosition * BOARD_SQUARE_SIZE;
	float shiftZ = PIECE_HEIGHT_OFFSET;

	// adjust position to middle of square
	double halfSquareSize = BOARD_SQUARE_SIZE / 2.0;
	if (shiftX < 0) {
		shiftX += halfSquareSize;
	}
	else {
		shiftX -= halfSquareSize;
	}

	if (shiftY < 0) {
		shiftY += halfSquareSize;
	}
	else {
		shiftY -= halfSquareSize;
	}

	return Vec3f(shiftX, shiftY, shiftZ);
}


Vec2i ChessUtils::computePieceBoardPosition(Vec3 scenePosition) {
	Vec2i boardPosition;
	boardPosition.x() = scenePosition.x() / BOARD_SQUARE_SIZE;
	boardPosition.y() = scenePosition.y() / BOARD_SQUARE_SIZE;	

	if (scenePosition.x() < 0) {
		--boardPosition.x();
	} else {
		++boardPosition.x();
	}

	if (scenePosition.y() < 0) {
		--boardPosition.y();
	} else {
		++boardPosition.y();
	}

	return boardPosition;
}


MatrixTransform* ChessUtils::loadOSGModel(string name, float modelSize, Material* material, bool overrideMaterial,
	Vec3 modelCenterShift, double rotationAngle, Vec3 rotationAxis,
	Vec3 modelCenterOffsetPercentage) {
	// create a new node by reading in model from file
	Node* modelNode = osgDB::readNodeFile(name);
	if (modelNode != NULL) {
		// apply material
		if (material != NULL) {
			if (overrideMaterial) {
				modelNode->getOrCreateStateSet()->setAttributeAndModes(material, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
			} else {
				modelNode->getOrCreateStateSet()->setAttributeAndModes(material, osg::StateAttribute::ON);
			}
		}

		//put model in origin			
		//osg::BoundingSphere bound = modelNode->getBound();
		osg::ComputeBoundsVisitor cbVisitorOrigin;
		modelNode->accept(cbVisitorOrigin);
		osg::BoundingBox bound = cbVisitorOrigin.getBoundingBox();
		
		double scaleRatio = modelSize / bound.radius();

		MatrixTransform* unitTransform = new MatrixTransform();		
		unitTransform->postMult(Matrix::translate(-bound.center().x(), -bound.center().y(), -bound.center().z()));
		unitTransform->postMult(Matrix::rotate(rotationAngle, rotationAxis));
		unitTransform->postMult(Matrix::scale(scaleRatio, scaleRatio, scaleRatio));
		unitTransform->addChild(modelNode);		

		// put model in specified location
		osg::ComputeBoundsVisitor cbVisitor;
		unitTransform->accept(cbVisitor);
		osg::BoundingBox boundingBox = cbVisitor.getBoundingBox();
				
		float modelXOffset = (boundingBox.xMax() - boundingBox.xMin()) * modelCenterOffsetPercentage.x();
		float modelYOffset = (boundingBox.yMax() - boundingBox.yMin()) * modelCenterOffsetPercentage.y();
		float modelZOffset = (boundingBox.zMax() - boundingBox.zMin()) * modelCenterOffsetPercentage.z();
		unitTransform->postMult(Matrix::translate(modelXOffset, modelYOffset, modelZOffset));		

		MatrixTransform* modelPositionTransform = new MatrixTransform();		
		modelPositionTransform->postMult(Matrix::translate(modelCenterShift));
		modelPositionTransform->addChild(unitTransform);		

		return modelPositionTransform;
	}

	return NULL;
}


LightSource* ChessUtils::createLightSource(StateSet* stateSet, int lightNumber,	
	Vec4 position, Vec3 direction,	
	float spotCutoff, float spotExponent,
	float constanteAttenuation,
	Vec4 ambientLight, Vec4 diffuseLight, Vec4 specularLight) {
	Light* light = new Light();
	light->setLightNum(lightNumber);
	light->setPosition(position);
	light->setDirection(direction);
	light->setAmbient(ambientLight);
	light->setDiffuse(diffuseLight);
	light->setSpecular(specularLight);
	light->setConstantAttenuation(constanteAttenuation);	
	light->setSpotCutoff(spotCutoff);
	light->setSpotExponent(spotExponent);

	LightSource* lightS = new LightSource();
	lightS->setLight(light);
	lightS->setLocalStateSetModes(osg::StateAttribute::ON);
	lightS->setStateSetModes(*stateSet, osg::StateAttribute::ON);	

	return lightS;
}


Material* ChessUtils::createMaterial(float shininess, Vec4 emission, Vec4 specular, Vec4 diffuse, Vec4 ambient) {
	Material* material = new Material();
	material->setColorMode(Material::DIFFUSE);
	material->setShininess(Material::FRONT, shininess);
	material->setEmission(Material::FRONT, emission);
	material->setSpecular(Material::FRONT, specular);
	material->setDiffuse(Material::FRONT, diffuse);
	material->setAmbient(Material::FRONT, ambient);
	
	return material;
}


Geode* ChessUtils::createRectangleWithTexture(Vec3 centerPosition, Image* image, int width, int height, Vec4 color) {
	int halfWidth = width / 2;
	int halfHeight = height / 2;

	Vec3Array* vertices = new Vec3Array();	
	vertices->push_back(Vec3(centerPosition.x() - halfWidth, centerPosition.y() - halfHeight, centerPosition.z()));
	vertices->push_back(Vec3(centerPosition.x() + halfWidth, centerPosition.y() - halfHeight, centerPosition.z()));
	vertices->push_back(Vec3(centerPosition.x() + halfWidth, centerPosition.y() + halfHeight, centerPosition.z()));
	vertices->push_back(Vec3(centerPosition.x() - halfWidth, centerPosition.y() + halfHeight, centerPosition.z()));

	Vec3Array* normals = new Vec3Array();
	normals->push_back(Vec3(0.0f, 0.0f, 1.0f));

	Vec2Array* texcoords = new Vec2Array();
	texcoords->push_back(Vec2(0.0f, 0.0f));
	texcoords->push_back(Vec2(1.0f, 0.0f));
	texcoords->push_back(Vec2(1.0f, 1.0f));
	texcoords->push_back(Vec2(0.0f, 1.0f));

	Vec4Array* colors = new Vec4Array();
	colors->push_back(color);

	Geometry* quad = new Geometry();
	quad->setVertexArray(vertices);
	quad->setNormalArray(normals);
	quad->setNormalBinding(osg::Geometry::BIND_OVERALL);	
	quad->setColorArray(colors);
	quad->setColorBinding(osg::Geometry::BIND_OVERALL);
	quad->setTexCoordArray(0, texcoords);
	quad->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));

	Texture2D* texture = new Texture2D();
	if (image != NULL) {
		texture->setImage(image);
	}			

	Geode* geode = new Geode();
	geode->addDrawable(quad);

	osg::BlendFunc* blendFunc = new osg::BlendFunc();
	blendFunc->setFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	osg::TexEnv* blendTexEnv = new osg::TexEnv();
	blendTexEnv->setMode(osg::TexEnv::BLEND);

	osg::StateSet* geodeStateset = geode->getOrCreateStateSet();
	geodeStateset->setAttributeAndModes(blendFunc);
	geodeStateset->setTextureAttribute(0, blendTexEnv);
	geodeStateset->setTextureAttributeAndModes(0, texture);
	geodeStateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	
	return geode;
}


Geometry* ChessUtils::createQuadWithTexture(string filename, const Vec3& corner, const Vec3& widthVec, const Vec3& heightVec) {
	Image* image = osgDB::readImageFile(filename);
	Texture2D* texture = new Texture2D();
	texture->setImage(image);

	osg::BlendFunc* blendFunc = new osg::BlendFunc();
	blendFunc->setFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	osg::TexEnv* blendTexEnv = new osg::TexEnv();
	blendTexEnv->setMode(osg::TexEnv::BLEND);

	Geometry* quad = osg::createTexturedQuadGeometry(corner, widthVec, heightVec);

	osg::StateSet* geometryStateset = quad->getOrCreateStateSet();
	geometryStateset->setAttributeAndModes(blendFunc);
	geometryStateset->setTextureAttribute(0, blendTexEnv);
	geometryStateset->setTextureAttributeAndModes(0, texture);
	geometryStateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

	return quad;
}


Geometry* ChessUtils::createHUDQuadWithTexture(string filename, int width, int height, Vec3 corner) {
	return createQuadWithTexture(filename, corner, Vec3(width, 0, 0), Vec3(0, 0, height));
}


Camera* ChessUtils::createHUDCamera(unsigned int screenWidth, unsigned int screenHeight) {
	Camera* camera = new Camera();
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	camera->setRenderOrder(Camera::POST_RENDER);
	camera->setAllowEventFocus(false);
	camera->setProjectionMatrix(Matrix::ortho2D(0, screenWidth, 0, screenHeight));
	//camera->setViewMatrix(osg::Matrix::identity());
	camera->setViewMatrixAsLookAt(Vec3(0, 0, 100), Vec3(0, 0, 0), Vec3(0, 1, 0));
	camera->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	return camera;
}


Text3D* ChessUtils::createText3D(const string& content, Font3D* font3D, const Vec3& position, float size, float depth, osgText::TextBase::AlignmentType textAligment) {
	Text3D* text = new Text3D();
	text->setFont(font3D);
	text->setCharacterSize(size);
	text->setCharacterDepth(depth);
	text->setAlignment(textAligment);
	text->setAxisAlignment(osgText::TextBase::XY_PLANE);
	text->setPosition(position);
	text->setText(content);	

	return text;
}



AnimationPath* ChessUtils::createChessPieceMoveAnimationPath(Vec3f initialPosition, Vec3f finalPosition, float rotationAngle, Vec3f rotationAxis, float pieceTravelSpeed, size_t numberSamplesInPath) {
	AnimationPath* animationPath = new AnimationPath();
	animationPath->setLoopMode(osg::AnimationPath::NO_LOOPING);

	Vec3 positionsOffset = finalPosition - initialPosition;
	float positionsDistance = positionsOffset.length();	
	float jumpHeight = positionsDistance / 2.0;
	float maxHeightPosition = initialPosition.z() + jumpHeight;

	float animationTimeSeconds = std::max(positionsDistance / pieceTravelSpeed, 2.0f);
	float numberSamplesInPathF = (float)numberSamplesInPath;
	float halfAnimationTime = animationTimeSeconds / 2.0;
	
	float deltaTime = animationTimeSeconds / numberSamplesInPathF;

	// cross product to obtain a perpendicular vector in relation to the animation direction
	//Vec3 rotationAxis = positionsOffset ^ osg::Z_AXIS;
	
	float fullRotation = osg::PI * 2.0;
	osgAnimation::Motion* rotationEaseMotion = new osgAnimation::InOutCubicMotion(0, animationTimeSeconds, rotationAngle);
	osgAnimation::Motion* xMovingEaseMotion = new osgAnimation::InOutCubicMotion(initialPosition.x(), animationTimeSeconds, positionsOffset.x());
	osgAnimation::Motion* yMovingEaseMotion = new osgAnimation::InOutCubicMotion(initialPosition.y(), animationTimeSeconds, positionsOffset.y());
	osgAnimation::Motion* zClimbingEaseMotion = new osgAnimation::InQuadMotion(initialPosition.z(), halfAnimationTime, jumpHeight);
	osgAnimation::Motion* zFallingEaseMotion = new osgAnimation::OutBounceMotion(maxHeightPosition, halfAnimationTime, -jumpHeight);

	size_t halfNumberSamplesInPath = numberSamplesInPath / 2;
	double currenTime = 0;
	for (size_t currentSamplePosition = 0; currentSamplePosition < numberSamplesInPath; ++currentSamplePosition) {
		float sampleXPosition = xMovingEaseMotion->getValue();
		xMovingEaseMotion->update(deltaTime);

		float sampleYPosition = yMovingEaseMotion->getValue();
		yMovingEaseMotion->update(deltaTime);

		float sampleZPosition;		
		if (currentSamplePosition < halfNumberSamplesInPath) {
			sampleZPosition = zClimbingEaseMotion->getValue();
			zClimbingEaseMotion->update(deltaTime);
		} else {
			sampleZPosition = zFallingEaseMotion->getValue();
			zFallingEaseMotion->update(deltaTime);
		}

		float sampleRotationAngle = rotationEaseMotion->getValue();
		rotationEaseMotion->update(deltaTime);		

		Vec3 position(sampleXPosition, sampleYPosition, sampleZPosition);
		Quat rotationQuad(sampleRotationAngle, rotationAxis);
		animationPath->insert(currenTime, AnimationPath::ControlPoint(position, rotationQuad));
		currenTime += deltaTime;
	}

	return animationPath;
}


AnimationPath* ChessUtils::createScaleAnimationPath(Vec3f position, osgAnimation::Motion* scaleEaseMotion, float animationTimeSeconds, size_t numberSamplesInPath) {
	AnimationPath* animationPath = new AnimationPath();
	animationPath->setLoopMode(osg::AnimationPath::NO_LOOPING);	
			
	float deltaTime = animationTimeSeconds / (float)numberSamplesInPath;	
	
	double currenTime = 0;
	while (currenTime <= animationTimeSeconds) {		
		float scaleFactor = scaleEaseMotion->getValue();
		scaleEaseMotion->update(deltaTime);
		
		Quat rotationQuad(0, osg::Z_AXIS);
		Vec3d scale(scaleFactor, scaleFactor, scaleFactor);
		animationPath->insert(currenTime, AnimationPath::ControlPoint(position, rotationQuad, scale));
		currenTime += deltaTime;
	}

	return animationPath;
}


string ChessUtils::formatSecondsToDate(double seconds) {
	unsigned short secondsFinal = (unsigned short)fmod(seconds, 60);

	double minutes = seconds / 60.0;
	unsigned short minutesFinal = (unsigned short)fmod(minutes, 60.0);

	double hours = minutes / 60;
	unsigned short hoursFinal = (unsigned short)fmod(hours, 24.0);

	unsigned short daysFinal = (unsigned short)(hours / 24.0);

	stringstream timeFormated;
	if (daysFinal != 0) {
		timeFormated << daysFinal << "d ";
	}

	if (hoursFinal < 10) {
		timeFormated << 0;
	}
	timeFormated << hoursFinal << "h ";	
	
	if (minutesFinal < 10) {
		timeFormated << 0;
	}
	timeFormated << minutesFinal << "m ";
	
	if (secondsFinal < 10) {
		timeFormated << 0;
	}			
	timeFormated << secondsFinal << "s";

	return timeFormated.str();
}