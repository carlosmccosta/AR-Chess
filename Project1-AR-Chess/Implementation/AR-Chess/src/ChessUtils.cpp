#include "ChessUtils.h"



MatrixTransform* ChessUtils::loadOSGModel(string name, float modelSize, Vec3f position, double rotationAngle, Vec3f rotationAxis, bool centerX, bool centerY, bool centerZ) {
	// create a new node by reading in model from file
	Node* modelNode = osgDB::readNodeFile(name);
	if (modelNode != NULL) {
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
		
		if (!centerX) {
			position.x() += (boundingBox.xMax() - boundingBox.xMin()) / 2.0;
		}

		if (!centerY) {
			position.y() += (boundingBox.yMax() - boundingBox.yMin()) / 2.0;
		}

		if (!centerZ) {
			position.z() += (boundingBox.zMax() - boundingBox.zMin()) / 2.0;
		}

		MatrixTransform* modelPositionTransform = new MatrixTransform();
		Matrix shiftTranslation;
		shiftTranslation.makeTranslate(position);
		modelPositionTransform->postMult(shiftTranslation);
		modelPositionTransform->addChild(unitTransform);
		
		return modelPositionTransform;
	}

	return NULL;
}


LightSource* ChessUtils::createLightSource(StateSet* stateSet, int lightNumber, Vec4 position, Vec4 ambientLight, Vec4 diffuseLight, Vec4 specularLight, float constanteAttenuation) {
	Light* light = new Light();
	light->setLightNum(lightNumber);
	light->setPosition(position);
	light->setAmbient(ambientLight);
	light->setDiffuse(diffuseLight);
	light->setSpecular(specularLight);
	light->setConstantAttenuation(constanteAttenuation);

	LightSource* lightS = new LightSource();
	lightS->setLight(light);
	lightS->setLocalStateSetModes(osg::StateAttribute::ON);
	lightS->setStateSetModes(*stateSet, osg::StateAttribute::ON);

	return lightS;
}

