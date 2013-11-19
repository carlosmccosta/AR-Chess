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


Vec3i ChessUtils::computePieceBoardPosition(Vec3 scenePosition) {
	Vec3i boardPosition;
	boardPosition.x() = scenePosition.x() / BOARD_SQUARE_SIZE;
	boardPosition.y() = scenePosition.y() / BOARD_SQUARE_SIZE;
	boardPosition.z() = scenePosition.z() / BOARD_SQUARE_SIZE;

	if (scenePosition.x() < 0) {
		--boardPosition.x();
	} else {
		++boardPosition.x();
	}

	if (scenePosition.y() < 0) {
		--boardPosition.y();
	}
	else {
		++boardPosition.y();
	}

	if (scenePosition.z() < 0) {
		--boardPosition.z();
	}
	else {
		++boardPosition.z();
	}

	return boardPosition;
}


MatrixTransform* ChessUtils::loadOSGModel(string name, float modelSize, Material* material, bool overrideMaterial,
	Vec3 modelCenterShift, double rotationAngle, Vec3 rotationAxis,
	float xModelCenterOffsetPercentage, float yModelCenterOffsetPercentage, float zModelCenterOffsetPercentage) {
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
				
		modelCenterShift.x() += (boundingBox.xMax() - boundingBox.xMin()) * xModelCenterOffsetPercentage;
		modelCenterShift.y() += (boundingBox.yMax() - boundingBox.yMin()) * yModelCenterOffsetPercentage;
		modelCenterShift.z() += (boundingBox.zMax() - boundingBox.zMin()) * zModelCenterOffsetPercentage;

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

	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
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



