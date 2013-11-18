#pragma once

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// OSG includes
#include <osg/MatrixTransform>
#include <osg/Image>
#include <osg/TexEnv>
#include <osgDB/ReadFile>
#include <osgSim/MultiSwitch>
#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowMap>
#include <osgShadow/ViewDependentShadowMap>

// std includes
#include <vector>

// project includes
#include "ChessUtils.h"
#include "ChessPiece.h"
#include "ModelsConfigs.h"


// namespace specific imports to avoid namespace pollution
using std::vector;
using osg::MatrixTransform;
using osg::Vec3f;
using osg::Image;
using osgSim::MultiSwitch;
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



class ChessBoard {
	public:
		ChessBoard();
		virtual ~ChessBoard();
		osgShadow::ShadowedScene* setupBoard();
		bool selectPosition(int xBoardPosition, int yBoardPosition, ChessPieceColor chessPieceType);
		bool hightLighPosition(int xBoardPosition, int yBoardPosition);
		bool isBoardSquareWithPiece(int xBoardPosition, int yBoardPosition, ChessPieceColor chessPieceType);
		bool isPositionValid(int position);

	private:
		vector<ChessPiece> _whiteChessPieces;
		vector<ChessPiece> _blackChessPieces;
		osgShadow::ShadowedScene* _boardShadowedScene;
		Material* _boardMaterial;
		Material* _whitePiecesMaterial;
		Material* _blackPiecesMaterial;
		Image* _paddleSelectorImage;
		Image* _paddleSelectedImage;
		Group* _overlays;
};

