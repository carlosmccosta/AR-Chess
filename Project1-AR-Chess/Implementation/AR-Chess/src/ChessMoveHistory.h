#pragma once
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// OSG includes
#include <osg/Vec2i>
#include <osg/Referenced>

// project includes
#include "ChessPiece.h"

// namespace specific imports to avoid namespace pollution
using osg::Vec2i;
using osg::Referenced;

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

class ChessMoveHistory : public Referenced {
	public:
		ChessMoveHistory(ChessPiece* pieceMoved, Vec2i pieceMovedOriginPosition, Vec2i pieceMovedDestinationPosition, bool pieceHasMovedPriviously,
			double whitePlayerGameTimerD = 0, double blackPlayerGameTimerD = 0, ChessPiece* pieceRemoved = NULL);
		virtual ~ChessMoveHistory();

		void moveBackInHistory();
		void moveFowardInHistory();

		// ------------------------------------------------------------------------------  <gets | sets> -------------------------------------------------------------------------------
		ChessPiece* getPieceMoved() const { return _pieceMoved; }		
		Vec2i getPieceMovedOriginPosition() const { return _pieceMovedOriginPosition; }		
		Vec2i getPieceMovedDestinationPosition() const { return _pieceMovedDestinationPosition; }
		double getWhitePlayerGameTimerD() const { return _whitePlayerGameTimerD; }
		double getBlackPlayerGameTimerD() const { return _blackPlayerGameTimerD; }
		ChessPiece* getPieceRemoved() const { return _pieceRemoved; }
		bool hasPerformedCastling() const { return _performedCastling; }
		void setPerformedCastling(bool val) { _performedCastling = val; }
		// ------------------------------------------------------------------------------  </gets | sets> ------------------------------------------------------------------------------


	private:
		ChessPiece* _pieceMoved;		
		Vec2i _pieceMovedOriginPosition;
		Vec2i _pieceMovedDestinationPosition;		
		bool _pieceHasMovedPriviously;
		double _whitePlayerGameTimerD;				
		double _blackPlayerGameTimerD;		
		ChessPiece* _pieceRemoved;
		bool _performedCastling;		
};

