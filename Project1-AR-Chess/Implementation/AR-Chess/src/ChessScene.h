#pragma once


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// OSG includes
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osg/Referenced>
#include <osg/LightModel>
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


// namespace specific imports to avoid namespace pollution
using std::string;
using osg::Group;
using osg::Node;
using osg::Vec3;
using osg::Vec3f;
using osg::MatrixTransform;
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
		osgFX::AnisotropicLighting* _anisotropicLighting;
		osgFX::SpecularHighlights* _specularHighlights;
		MatrixTransform* _boardTrackerMT;
		MatrixTransform* _selectorTrackerMT;
		ChessBoard _gameBoard;
};


class ChessSceneNodeCallback : public osg::NodeCallback {
	public:
		virtual void operator()(osg::Node* node, osg::NodeVisitor* nodeVisitor);
};
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </ChessScene> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
