#include "ChessMoveHistory.h"


ChessMoveHistory::ChessMoveHistory(ChessPiece* pieceMoved, Vec2i pieceMovedOriginPosition, Vec2i pieceMovedDestinationPosition, bool pieceHasMovedPriviously,
	double whitePlayerGameTimerD, double whitePlayerGameTimerDEndMove, double blackPlayerGameTimerD, double blackPlayerGameTimerDEndMove,
	int playNumberOfLastMove, ChessPiece* pieceRemoved) :
	_pieceMoved(pieceMoved), _pieceMovedOriginPosition(pieceMovedOriginPosition), _pieceMovedDestinationPosition(pieceMovedDestinationPosition), _pieceHasMovedPriviously(pieceHasMovedPriviously),
	_whitePlayerGameTimerD(whitePlayerGameTimerD), _whitePlayerGameTimerDEndMove(whitePlayerGameTimerDEndMove),
	_blackPlayerGameTimerD(blackPlayerGameTimerD), _blackPlayerGameTimerDEndMove(blackPlayerGameTimerDEndMove),
	_pieceRemoved(pieceRemoved), _performedCastling(false), _performedPromotion(false), _playNumberOfLastMove(playNumberOfLastMove) {
	
	_uciMove = UCIMove::convertBoardMoveToUCIMove(pieceMovedOriginPosition, pieceMovedDestinationPosition);
}

ChessMoveHistory::~ChessMoveHistory() {}


void ChessMoveHistory::moveBackInHistory() {
	_pieceMoved->changePosition(_pieceMovedOriginPosition.x(), _pieceMovedOriginPosition.y(), _playNumberOfLastMove - 1);
	_pieceMoved->setPieceMovedPreviously(_pieceHasMovedPriviously);
	if (_pieceRemoved != NULL) {
		_pieceRemoved->reinsertPieceOnBoard();
	}
}


void ChessMoveHistory::moveFowardInHistory() {
	_pieceMoved->changePosition(_pieceMovedDestinationPosition.x(), _pieceMovedDestinationPosition.y(), _playNumberOfLastMove);

	if (_pieceRemoved != NULL) {
		_pieceRemoved->removePieceFromBoard();
	}
}



void ChessMoveHistory::setPromotionPieceType(ChessPieceType promotionRank) {
	_promotionPieceType = promotionRank;
	_performedPromotion = true;

	switch (promotionRank) {
		case QUEEN:		{ _uciMove.append("q"); break; }
		case ROOK:		{ _uciMove.append("r"); break; }
		case KNIGHT:	{ _uciMove.append("n"); break; }
		case BISHOP:	{ _uciMove.append("b"); break; }
		default:		{ _promotionPieceType = PAWN; _performedPromotion = false; break; }
	}
}