#include "ChessMoveHistory.h"


ChessMoveHistory::ChessMoveHistory(ChessPiece* pieceMoved, Vec2i pieceMovedOriginPosition, Vec2i pieceMovedDestinationPosition, bool pieceHasMovedPriviously,
	double whitePlayerGameTimerD, double whitePlayerGameTimerDEndMove, double blackPlayerGameTimerD, double blackPlayerGameTimerDEndMove, ChessPiece* pieceRemoved) :
	_pieceMoved(pieceMoved), _pieceMovedOriginPosition(pieceMovedOriginPosition), _pieceMovedDestinationPosition(pieceMovedDestinationPosition), _pieceHasMovedPriviously(pieceHasMovedPriviously),
	_whitePlayerGameTimerD(whitePlayerGameTimerD), _whitePlayerGameTimerDEndMove(whitePlayerGameTimerDEndMove),
	_blackPlayerGameTimerD(blackPlayerGameTimerD), _blackPlayerGameTimerDEndMove(blackPlayerGameTimerDEndMove),
	_pieceRemoved(pieceRemoved), _performedCastling(false), _performedPromotion(false) {}

ChessMoveHistory::~ChessMoveHistory() {}


void ChessMoveHistory::moveBackInHistory() {
	_pieceMoved->changePosition(_pieceMovedOriginPosition.x(), _pieceMovedOriginPosition.y());	
	_pieceMoved->setPieceMovedPreviously(_pieceHasMovedPriviously);
	if (_pieceRemoved != NULL) {
		_pieceRemoved->reinsertPieceOnBoard();
	}
}


void ChessMoveHistory::moveFowardInHistory() {
	_pieceMoved->changePosition(_pieceMovedDestinationPosition.x(), _pieceMovedDestinationPosition.y());

	if (_pieceRemoved != NULL) {
		_pieceRemoved->removePieceFromBoard();
	}
}