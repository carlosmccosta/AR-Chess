#pragma once


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// OSG includes
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osg/Referenced>
#include <osg/LightModel>
#include <osg/Camera>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgFX/SpecularHighlights>
#include <osgFX/AnisotropicLighting>

// osgART includes
#include <osgART/Scene>

// std includes
#include <string>

// project includes
#include "ChessBoard.h"
#include "ChessUtils.h"
#include "CloudParticleSystem.h"


// namespace specific imports to avoid namespace pollution
using std::string;
using osg::Group;
using osg::Node;
using osg::Vec3;
using osg::Vec3f;
using osg::MatrixTransform;
using osg::Camera;
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <ChessScene> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
class ChessScene : public osg::Referenced {
	public:
		ChessScene();
		virtual ~ChessScene();

		void startGame();
		void endGame();

		void setupScene();
		void setupViewer();
		void setupARTrackers();

		void setupBoard();		
		void setupSelector();
		void setupLights();

		void updateScene();
		
	private:		
		osgViewer::Viewer _viewer;		
		Group* _lightGroup;
		LightSource* _mainLightSource;
		osgART::Scene* _boardScene;		
		MatrixTransform* _boardTrackerMT;
		MatrixTransform* _selectorTrackerMT;				
		osgShadow::ShadowedScene* _boardShadowedScene;
		ChessBoard _gameBoard;

		CloudParticleSystem* _selectorParticleSystem;

		//Camera* _gameStatusHUD;
		//osgFX::AnisotropicLighting* _anisotropicLighting;
		//osgFX::SpecularHighlights* _specularHighlights;
		//Font3D* _font3D;
		//unsigned int _screenWidth;
		//unsigned int _screenHeight;
};


class ChessSceneNodeCallback : public osg::NodeCallback {
	public:
		virtual void operator()(osg::Node* node, osg::NodeVisitor* nodeVisitor);
};
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </ChessScene> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
