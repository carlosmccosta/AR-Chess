#pragma once


#define VIDEO_CONFIGURATION "Data/WDM_camera.xml"
#define VIDEO_PLUGIN "osgart_artoolkit"
#define TRACKER_PLUGIN "osgart_artoolkit_tracker"

#define TRACKER_MARKERS_CONFIG "Data/markers_board.dat"

#define BOARD_SIZE 200
#define BOARD_HEIGHT 4
#define BOARD_MODEL "Models/board.osg"
#define BOARD_BORDER_LEFT_RIGHT 20
#define BOARD_BORDER_TOP_BOTTOM 10
#define BOARD_SQUARE_SIZE 27

#define PIECE_SIZE 25
#define PIECE_HEIGHT 40
#define PIECE_WHITE_KING "Models/king_white.osg"
#define PIECE_WHITE_QUEEN "Models/queen_white.osg"
#define PIECE_WHITE_ROOK "Models/tower_white.osg"
#define PIECE_WHITE_KNIGHT "Models/knight_white.osg"
#define PIECE_WHITE_BISHOP "Models/bishop_white.osg"
#define PIECE_WHITE_PAWN "Models/pawn_white.osg"
#define PIECE_BLACK_KING "Models/king_black.osg"
#define PIECE_BLACK_QUEEN "Models/queen_black.osg"
#define PIECE_BLACK_ROOK "Models/tower_black.osg"
#define PIECE_BLACK_KNIGHT "Models/knight_black.osg"
#define PIECE_BLACK_BISHOP "Models/bishop_black.osg"
#define PIECE_BLACK_PAWN "Models/pawn_black.osg"

#include <Producer/RenderSurface>
#include <osgProducer/Viewer>
#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Projection>
#include <osg/AutoTransform>
#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osg/LightModel>
#include <osg/LightSource>
#include <osgDB/ReadFile>
#include <osgART/VideoManager>
#include <osgART/ARTTransform>
#include <osgART/TrackerManager>
#include <osgART/VideoBackground>
#include <osgART/VideoPlane>

#include <string>

#include "Board.h"

using std::string;
using osg::Node;
using osg::StateSet;
using osg::Matrix;
using osg::MatrixTransform;
using osg::BoundingSphere;
using osg::Vec3f;
using osg::Vec4;
using osg::Group;
using osg::Light;
using osg::LightSource;
using osg::StateAttribute;
using osg::ref_ptr;
using osg::Projection;
using osg::Group;


class Scene {
	public:
		Scene();
		virtual ~Scene();

		void startGame();
		void endGame();

		void setupScene();

		void setupBoard();
		void setupPieces(ref_ptr<MatrixTransform> boardMarkerTransform);
		void setupSelector();
		Node* setupLights(StateSet* rootStateSet);
		Node* createLightSource(StateSet* rootStateSet, Vec4 position = Vec4(), int lightNumber = 1); 

		Node* createOSGModel(string name, float modelSize, float shiftX = 0, float shiftY = 0, float shiftZ = 0, double rotationAngle = 0, Vec3f rotationAxis = Vec3f(1.0, 0.0, 0.0), bool centerX = true, bool centerY = true, bool centerZ = false);
		
	private:
		Board gameBoard;
		osgProducer::Viewer viewer;
		osg::ref_ptr<osgART::GenericVideo> videoPlugin;
		osg::ref_ptr<osgART::GenericTracker> trackerPlugin;
};

