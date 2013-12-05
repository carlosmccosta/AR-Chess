#pragma once
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// OSG includes
#include <osg/Vec2i>
#include <osg/Referenced>

// std includes
#include <string>

// project includes
#include "ChessPiece.h"
#include "UCIMove.h"

// namespace specific imports to avoid namespace pollution
using std::string;
using osg::Vec2i;
using osg::Vec2iArray;
using osg::Referenced;
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

class ChessMoveHistory : public Referenced {
	public:
		ChessMoveHistory(ChessPiece* pieceMoved, Vec2i pieceMovedOriginPosition, Vec2i pieceMovedDestinationPosition, bool pieceHasMovedPriviously,
			double whitePlayerGameTimerD = 0, double whitePlayerGameTimerDEndMove = 0, double blackPlayerGameTimerD = 0, double blackPlayerGameTimerDEndMove = 0,
			int playNumberOfLastMove = -1, ChessPiece* pieceRemoved = NULL);
		virtual ~ChessMoveHistory();

		void moveBackInHistory();
		void moveFowardInHistory();		

		// ------------------------------------------------------------------------------  <gets | sets> -------------------------------------------------------------------------------
		ChessPiece* getPieceMoved() const { return _pieceMoved; }		
		Vec2i getPieceMovedOriginPosition() const { return _pieceMovedOriginPosition; }		
		Vec2i getPieceMovedDestinationPosition() const { return _pieceMovedDestinationPosition; }
		string getUciMove() const { return _uciMove; }
		double getWhitePlayerGameTimerD() const { return _whitePlayerGameTimerD; }
		double getWhitePlayerGameTimerDEndMove() const { return _whitePlayerGameTimerDEndMove; }
		double getBlackPlayerGameTimerD() const { return _blackPlayerGameTimerD; }
		double getBlackPlayerGameTimerDEndMove() const { return _blackPlayerGameTimerDEndMove; }
		ChessPiece* getPieceRemoved() const { return _pieceRemoved; }
		bool hasPerformedCastling() const { return _performedCastling; }
		void setPerformedCastling(bool val) { _performedCastling = val; }
		bool hasPerformedPromotion() const { return _performedPromotion; }
		void setPerformedPromotion(bool val) { _performedPromotion = val; }
		ChessPieceType getOriginalPieceType() const { return _originalPieceType; }
		void setOriginalPieceType(ChessPieceType val) { _originalPieceType = val; }
		ChessPieceType getPromotionPieceType() const { return _promotionPieceType; }
		void setPromotionPieceType(ChessPieceType promotionRank);
		// ------------------------------------------------------------------------------  </gets | sets> ------------------------------------------------------------------------------


	private:
		ChessPiece* _pieceMoved;		
		Vec2i _pieceMovedOriginPosition;
		Vec2i _pieceMovedDestinationPosition;
		string _uciMove;				
		bool _pieceHasMovedPriviously;
		double _whitePlayerGameTimerD;
		double _whitePlayerGameTimerDEndMove;				
		double _blackPlayerGameTimerD;
		double _blackPlayerGameTimerDEndMove;								
		ChessPiece* _pieceRemoved;
		bool _performedCastling;
		bool _performedPromotion;		

		// en passant pawn capture fields
		int _playNumberOfLastMove;

		ChessPieceType _originalPieceType;
		ChessPieceType _promotionPieceType;		
};

