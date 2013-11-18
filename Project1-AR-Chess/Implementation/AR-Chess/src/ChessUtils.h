#pragma once

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// OSG includes
#include <osg/MatrixTransform>
#include <osg/LightSource>
#include <osg/Material>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Texture2D>
#include <osg/TexEnv>
#include <osg/BlendFunc>
#include <osg/ComputeBoundsVisitor>
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>

// project includes
#include "ModelsConfigs.h"

// namespace specific imports to avoid namespace pollution
using std::string;
using osg::Vec2;
using osg::Vec3;
using osg::Vec3f;
using osg::Vec4;
using osg::Vec2Array;
using osg::Vec3Array;
using osg::Matrix;
using osg::MatrixTransform;
using osg::Node;
using osg::Geometry;
using osg::Texture2D;
using osg::Geode;
using osg::Image;
using osg::Light;
using osg::LightSource;
using osg::Group;
using osg::StateSet;
using osg::Material;
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


namespace ChessUtils {
	Vec3f computePieceScenePosition(int xPosition, int yPosition);

	MatrixTransform* loadOSGModel(string name, float modelSize, Material* material = NULL, bool overrideMaterial = false,
		Vec3 modelCenterShift = Vec3(0.0, 0.0, 0.0),
		double rotationAngle = 0, Vec3 rotationAxis = Vec3(0.0, 0.0, 1.0),
		float xModelCenterOffsetPercentage = 0, float yModelCenterOffsetPercentage = 0, float zModelCenterOffsetPercentage = 0.5);

	LightSource* createLightSource(StateSet* stateSet,
		int lightNumber = 1,		
		Vec4 position = Vec4(0.0f, 0.0f, 50.0f, 1.0f),
		Vec3 direction = Vec3(0.0, 0.0, -1.0),		
		float spotCutoff = 180, float spotExponent = 128,
		float constanteAttenuation = 1.0f,
		Vec4 ambientLight = Vec4(0.0f, 0.0f, 0.0f, 0.0f),
		Vec4 diffuseLight = Vec4(0.2f, 0.2f, 0.2f, 0.2f),
		Vec4 specularLight = Vec4(1.0f, 1.0f, 1.0f, 1.0f));


	Material* createMaterial(float shininess = 128.0,
		Vec4 emission = Vec4(0.05f, 0.05f, 0.05f, 1.0f),
		Vec4 specular = Vec4(0.6f, 0.6f, 0.6f, 1.0f),
		Vec4 diffuse = Vec4(0.4f, 0.4f, 0.4f, 1.0f),
		Vec4 ambient = Vec4(0.1f, 0.1f, 0.1f, 1.0f));

	Geode* createRectangleWithTexture(Vec3 centerPosition = Vec3(0, 0, 0), Image* image = NULL, int width = BOARD_SQUARE_SIZE, int height = BOARD_SQUARE_SIZE, Vec4 color = Vec4(1.0f, 1.0f, 1.0f, 0.9f));
};

