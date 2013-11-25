#pragma once


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// OSG includes
#include <osg/MatrixTransform>
#include <osg/Material>
#include <osg/AnimationPath>

// project includes
#include "ChessUtils.h"
#include "Configs.h"


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

enum ChessPieceColor {
	WHITE,
	BLACK
};


class ChessPiece : public osg::Referenced {
	public:		
		ChessPiece(ChessPieceType chessPieceType, ChessPieceColor chessPieceColor, int xPosition, int yPosition, Material* material);
		virtual ~ChessPiece();
		
		string getPieceModelPath(ChessPieceType chessPieceType, ChessPieceColor chessPieceColor);
		float getPieceModelSize(ChessPieceType chessPieceType);		

		void changePosition(int xPosition, int yPosition);
		void resetPosition();
		void removePieceFromBoard();
		void reinsertPieceOnBoard();

		static ChessPieceColor getOpponentChessPieceColor(ChessPieceColor chessPieceColor);

		// ------------------------------------------------------------------------------  <gets | sets> -------------------------------------------------------------------------------
		ChessPieceType getChessPieceType() const { return _chessPieceType; }		
		ChessPieceColor getChessPieceColor() const { return _chessPieceColor; }		
		int getXPosition() const { return _xPosition; }		
		int getYPosition() const { return _yPosition; }		
		bool isPiecePlayable() const { return _piecePlayable; }		
		MatrixTransform* getPieceMatrixTransform() const { return _pieceMatrixTransform; }
		bool hasPieceMovedPreviously() const { return _pieceMovedPreviously; }
		void setPieceMovedPreviously(bool val) { _pieceMovedPreviously = val; }
		// ------------------------------------------------------------------------------  </gets | sets> ------------------------------------------------------------------------------

	private:
		ChessPieceType _chessPieceType;		
		ChessPieceColor _chessPieceColor;		
		int _xPosition;
		int _yPosition;
		Vec3f _pieceCurrentScenePosition;
		int _xInitialPosition;
		int _yInitialPosition;
		bool _piecePlayable;
		bool _pieceMovedPreviously;
		MatrixTransform* _pieceMatrixTransform;
};

