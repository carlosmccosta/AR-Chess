#pragma once

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// OSG includes
#include <osg/MatrixTransform>
#include <osg/LightSource>
#include <osg/ComputeBoundsVisitor>
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>


// namespace specific imports to avoid namespace pollution
using std::string;
using osg::Vec4;
using osg::Vec3f;
using osg::Matrix;
using osg::MatrixTransform;
using osg::Node;
using osg::Light;
using osg::LightSource;
using osg::Group;
using osg::StateSet;
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


namespace ChessUtils {
	MatrixTransform* loadOSGModel(string name, float modelSize,
		Vec3f position = Vec3f(0.0, 0.0, 0.0),
		double rotationAngle = 0, Vec3f rotationAxis = Vec3f(0.0, 0.0, 1.0),
		bool centerX = true, bool centerY = true, bool centerZ = false);

	LightSource* createLightSource(StateSet* stateSet,
		int lightNumber = 1,
		Vec4 position = Vec4(0.0f, 0.0f, 0.0f, 0.0f),
		Vec4 ambientLight = Vec4(0.0f, 0.0f, 0.0f, 0.0f),
		Vec4 diffuseLight = Vec4(0.1f, 0.1f, 0.1f, 0.1f),
		Vec4 specularLight = Vec4(0.6f, 0.6f, 0.6f, 0.6f),
		float constanteAttenuation = 1.0f);
};

