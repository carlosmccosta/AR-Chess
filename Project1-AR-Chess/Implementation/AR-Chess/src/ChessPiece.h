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
		void removePieceFromBoard();

		static ChessPieceColor getOpponentChessPieceColor(ChessPieceColor chessPieceColor);

		// ------------------------------------------------------------------------------  <gets / sets> -------------------------------------------------------------------------------
		ChessPieceType getChessPieceType() const { return _chessPieceType; }
		void setChessPieceType(ChessPieceType val) { _chessPieceType = val; }
		ChessPieceColor getChessPieceColor() const { return _chessPieceColor; }
		void setChessPieceColor(ChessPieceColor val) { _chessPieceColor = val; }
		int getXPosition() const { return _xPosition; }
		void setXPosition(int val) { _xPosition = val; }
		int getYPosition() const { return _yPosition; }
		void setYPosition(int val) { _yPosition = val; }
		bool isPiecePlayable() const { return _piecePlayable; }
		void setPiecePlayable(bool val) { _piecePlayable = val; }
		MatrixTransform* getPieceMatrixTransform() const { return _pieceMatrixTransform; }
		void setPieceMatrixTransform(MatrixTransform* val) { _pieceMatrixTransform = val; }
		// ------------------------------------------------------------------------------  </gets / sets> ------------------------------------------------------------------------------

	private:
		ChessPieceType _chessPieceType;		
		ChessPieceColor _chessPieceColor;		
		int _xPosition;
		int _yPosition;
		int _xInitialPosition;
		int _yInitialPosition;
		bool _piecePlayable;		
		MatrixTransform* _pieceMatrixTransform;
};

