#pragma once

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// OSG includes
#include <osg/MatrixTransform>
#include <osg/LightSource>
#include <osg/Material>
#include <osg/ComputeBoundsVisitor>
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>


// namespace specific imports to avoid namespace pollution
using std::string;
using osg::Vec4;
using osg::Vec3;
using osg::Matrix;
using osg::MatrixTransform;
using osg::Node;
using osg::Light;
using osg::LightSource;
using osg::Group;
using osg::StateSet;
using osg::Material;
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


namespace ChessUtils {
	MatrixTransform* loadOSGModel(string name, float modelSize, Material* material = NULL, bool overrideMaterial = false,
		Vec3 modelCenterShift = Vec3(0.0, 0.0, 0.0),
		double rotationAngle = 0, Vec3 rotationAxis = Vec3(0.0, 0.0, 1.0),
		float xModelCenterOffsetPercentage = 0, float yModelCenterOffsetPercentage = 0, float zModelCenterOffsetPercentage = 0.5);

	LightSource* createLightSource(StateSet* stateSet,
		int lightNumber = 1,
		Vec4 position = Vec4(0.0f, 0.0f, 50.0f, 1.0f),
		Vec3 direction = Vec3(0.0, 0.0, -1.0),
		Vec4 ambientLight = Vec4(0.0f, 0.0f, 0.0f, 0.0f),
		Vec4 diffuseLight = Vec4(0.2f, 0.2f, 0.2f, 0.2f),
		Vec4 specularLight = Vec4(1.0f, 1.0f, 1.0f, 1.0f),
		float constanteAttenuation = 1.0f,		
		float spotCutoff = 180, float spotExponent = 128);


	Material* createMaterial(float shininess = 128.0,
		Vec4 emission = Vec4(0.05f, 0.05f, 0.05f, 1.0f),
		Vec4 specular = Vec4(0.6f, 0.6f, 0.6f, 1.0f),
		Vec4 diffuse = Vec4(0.4f, 0.4f, 0.4f, 1.0f),
		Vec4 ambient = Vec4(0.1f, 0.1f, 0.1f, 1.0f));
};

