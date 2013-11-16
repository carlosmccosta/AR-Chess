#pragma once


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <constants definitions> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#define VIDEO_CONFIG "data/artoolkit/WDM_camera.xml"
#define VIDEO_PLUGIN "osgart_artoolkit"
#define VIDEO_SOURCE "osgart_video_artoolkit"
#define CAMERA_CONFIG "data/artoolkit/camera_para.dat"
#define TRACKER_PLUGIN "osgart_tracker_artoolkit"
#define TRACKER_BOARD_CONFIG "multi;data/artoolkit/multi/marker.dat"
#define TRACKER_SELECTOR_CONFIG "single;data/artoolkit/patt.kanji;80;0;0"

#define BOARD_SIZE 200
#define BOARD_HEIGHT 4
#define BOARD_MODEL "models/board.osg"
#define BOARD_BORDER_LEFT_RIGHT 20
#define BOARD_BORDER_TOP_BOTTOM 10
#define BOARD_SQUARE_SIZE 27

#define PIECE_SIZE 25
#define PIECE_HEIGHT 40
#define PIECE_WHITE_KING "models/king_white.osg"
#define PIECE_WHITE_QUEEN "models/queen_white.osg"
#define PIECE_WHITE_ROOK "models/tower_white.osg"
#define PIECE_WHITE_KNIGHT "models/knight_white.osg"
#define PIECE_WHITE_BISHOP "models/bishop_white.osg"
#define PIECE_WHITE_PAWN "models/pawn_white.osg"
#define PIECE_BLACK_KING "models/king_black.osg"
#define PIECE_BLACK_QUEEN "models/queen_black.osg"
#define PIECE_BLACK_ROOK "models/tower_black.osg"
#define PIECE_BLACK_KNIGHT "models/knight_black.osg"
#define PIECE_BLACK_BISHOP "models/bishop_black.osg"
#define PIECE_BLACK_PAWN "models/pawn_black.osg"
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </constants definitions> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// OSG includes
#include <osg/MatrixTransform>
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>

// osgART includes
#include <osgART/Scene>

// std includes
#include <string>

// project includes
#include "ChessBoard.h"


// namespace specific imports to avoid namespace pollution
using std::string;
using osg::Vec4;
using osg::Vec3f;
using osg::ref_ptr;
using osg::Matrix;
using osg::MatrixTransform;
using osg::Node;
using osg::Light;
using osg::LightSource;
using osg::Group;
using osg::StateSet;
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <ChessScene> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
class ChessScene {
	public:
		ChessScene();
		virtual ~ChessScene();

		void startGame();
		void endGame();

		void setupScene();
		void setupViewer();
		void setupARTrackers();

		void setupBoard();
		void setupPieces();
		void setupSelector();
		Node* setupLights();
		Node* createLightSource(StateSet* rootStateSet, Vec4 position = Vec4(), int lightNumber = 1); 

		Node* createOSGModel(string name, float modelSize, float shiftX = 0, float shiftY = 0, float shiftZ = 0,
			double rotationAngle = 0, Vec3f rotationAxis = Vec3f(1.0, 0.0, 0.0),
			bool centerX = true, bool centerY = true, bool centerZ = false);
		
	private:		
		osgViewer::Viewer viewer;
		osgART::Scene* scene;
		ref_ptr<MatrixTransform> boardTrackerMT;
		ref_ptr<MatrixTransform> selectorTrackerMT;
		ChessBoard gameBoard;
};
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </ChessScene> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
