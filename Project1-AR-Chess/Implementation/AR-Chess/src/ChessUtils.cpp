#include "ChessUtils.h"



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



