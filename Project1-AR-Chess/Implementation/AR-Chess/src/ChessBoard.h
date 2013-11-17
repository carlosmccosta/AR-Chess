#pragma once

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// OSG includes
#include <osg/MatrixTransform>

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
		MatrixTransform* setupBoard();

	private:
		vector<ChessPiece> _whiteChessPieces;
		vector<ChessPiece> _blackChessPieces;
		MatrixTransform* _boardMatrixTransform;
};

