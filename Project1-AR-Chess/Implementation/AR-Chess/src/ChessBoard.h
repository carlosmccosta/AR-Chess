#pragma once

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// OSG includes
#include <osg/MatrixTransform>
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
using osg::MatrixTransform;
using osg::Vec3f;
using std::vector;
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



class ChessBoard {
	public:
		ChessBoard();
		virtual ~ChessBoard();
		osgShadow::ShadowedScene* setupBoard();

	private:
		vector<ChessPiece> _whiteChessPieces;
		vector<ChessPiece> _blackChessPieces;
		osgShadow::ShadowedScene* _boardShadowedScene;
		Material* _boardMaterial;
		Material* _whitePiecesMaterial;
		Material* _blackPiecesMaterial;
};

