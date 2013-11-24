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
		ChessMoveHistory(ChessPiece* pieceMoved, Vec2i pieceMovedOriginPosition, ChessPiece* pieceRemoved = NULL);
		virtual ~ChessMoveHistory();

		// ------------------------------------------------------------------------------  <gets | sets> -------------------------------------------------------------------------------
		ChessPiece* isPieceMoved() const { return _pieceMoved; }
		void setPieceMoved(ChessPiece* val) { _pieceMoved = val; }
		Vec2i getPieceMovedOriginPosition() const { return _pieceMovedOriginPosition; }
		void setPieceMovedOriginPosition(Vec2i val) { _pieceMovedOriginPosition = val; }
		ChessPiece* getPieceRemoved() const { return _pieceRemoved; }
		void setPieceRemoved(ChessPiece* val) { _pieceRemoved = val; }
		// ------------------------------------------------------------------------------  </gets | sets> ------------------------------------------------------------------------------


	private:
		ChessPiece* _pieceMoved;		
		Vec2i _pieceMovedOriginPosition;		
		ChessPiece* _pieceRemoved;		
};

