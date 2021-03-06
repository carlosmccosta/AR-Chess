#pragma once


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// OSG includes
#include <osg/MatrixTransform>
#include <osg/Material>
#include <osg/AnimationPath>
#include <osgAnimation/EaseMotion>

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
		ChessPiece(ChessPieceType chessPieceType, ChessPieceColor chessPieceColor, int xPosition, int yPosition, Material* material, Vec3 scaleFactor = Vec3(1.0, 1.0, 1.0), Vec3 translateOffset = Vec3(0.0, 0.0, 0.0));
		virtual ~ChessPiece();
		
		void loadPieceModel(ChessPieceType chessPieceType, Vec3 scaleFactor = Vec3(1.0, 1.0, 1.0), Vec3 translateOffset = Vec3(0.0, 0.0, 0.0));
		void shrinkPiece();
		void unshrinkPiece();

		string getPieceModelPath(ChessPieceType chessPieceType, ChessPieceColor chessPieceColor);
		float getPieceModelSize(ChessPieceType chessPieceType);		

		void setPosition(int xPosition, int yPosition);
		void changePositionWithAnimation(int xPosition, int yPosition, int playNumber = -1);
		
		void resetPosition();
		void removePieceFromBoard();
		void reinsertPieceOnBoard();		

		void clearMatrixTransform();

		static ChessPieceColor getOpponentChessPieceColor(ChessPieceColor chessPieceColor);

		// ------------------------------------------------------------------------------  <gets | sets> -------------------------------------------------------------------------------
		ChessPieceType getChessPieceType() const { return _chessPieceType; }		
		ChessPieceColor getChessPieceColor() const { return _chessPieceColor; }		
		int getXPosition() const { return _xPosition; }		
		int getYPosition() const { return _yPosition; }		
		bool isPiecePlayable() const { return _piecePlayable; }		
		void setPiecePlayable(bool val) { _piecePlayable = val; }
		MatrixTransform* getPieceMatrixTransform() const { return _pieceMatrixTransform; }
		bool getPieceMovedPreviously() const { return _pieceMovedPreviously; }
		void setPieceMovedPreviously(bool val) { _pieceMovedPreviously = val; }
		int getPlayNumberOfLastMove() const { return _playNumberOfLastMove; }
		void setPlayNumberOfLastMove(int val) { _playNumberOfLastMove = val; }		
		bool getPawnMakeDoubleStep() const { return _pawnMakeDoubleStep; }
		void setPawnMakeDoubleStep(bool val) { _pawnMakeDoubleStep = val; }
		// ------------------------------------------------------------------------------  </gets | sets> ------------------------------------------------------------------------------

	private:
		ChessPieceType _chessPieceType;
		ChessPieceColor _chessPieceColor;		
		int _xPosition;
		int _yPosition;
		Vec3f _pieceCurrentScenePosition;

		ChessPieceType _chessPieceInitialType;
		int _xInitialPosition;
		int _yInitialPosition;
		bool _piecePlayable;
		bool _pieceMovedPreviously;

		// en passant pawn capture fields
		int _playNumberOfLastMove;		
		bool _pawnMakeDoubleStep;		
		
		Material* _pieceMaterial;
		MatrixTransform* _pieceMatrixTransform;
		MatrixTransform* _pieceMatrixTransformModel;
};

