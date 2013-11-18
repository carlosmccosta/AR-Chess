#pragma once


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// OSG includes
#include <osg/MatrixTransform>
#include <osg/Material>

// project includes
#include "ChessUtils.h"
#include "ModelsConfigs.h"


// namespace specific imports to avoid namespace pollution
using osg::MatrixTransform;
using osg::Vec3f;
using osg::Material;
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



enum ChessPieceType {
	KING,
	QUEEN,
	ROOK,
	KNIGHT,
	BISHOP,
	PAWN
};

enum ChessPieceColor{
	WHITE,
	BLACK
};


class ChessPiece {
	public:		
		ChessPiece(ChessPieceType chessPieceType, ChessPieceColor chessPieceColor, int xPosition, int yPosition, Material* material);
		virtual ~ChessPiece();
		
		string getPieceModelPath(ChessPieceType chessPieceType, ChessPieceColor chessPieceColor);
		float getPieceModelSize(ChessPieceType chessPieceType);		

		bool changePosition(int xPosition, int yPosition);

		// ------------------------------------------------------------------------------  <gets / sets> -------------------------------------------------------------------------------
		int getXPosition() const { return _xPosition; }
		void setXPosition(int val) { _xPosition = val; }
		int getYPosition() const { return _yPosition; }
		void setYPosition(int val) { _yPosition = val; }
		MatrixTransform* getPieceMatrixTransform() const { return _pieceMatrixTransform; }
		void setPieceMatrixTransform(MatrixTransform* val) { _pieceMatrixTransform = val; }
		// ------------------------------------------------------------------------------  </gets / sets> ------------------------------------------------------------------------------

	private:
		ChessPieceType _chessPieceType;
		ChessPieceColor _chessPieceColor;
		int _xPosition;		
		int _yPosition;		
		bool _piecePlayable;
		MatrixTransform* _pieceMatrixTransform;
};

