#include "ChessMoveHistory.h"


ChessMoveHistory::ChessMoveHistory(ChessPiece* pieceMoved, Vec2i pieceMovedOriginPosition, Vec2i pieceMovedDestinationPosition,
	double whitePlayerGameTimerD, double blackPlayerGameTimerD, ChessPiece* pieceRemoved) :
	_pieceMoved(pieceMoved), _pieceMovedOriginPosition(pieceMovedOriginPosition), _pieceMovedDestinationPosition(pieceMovedDestinationPosition),
	_whitePlayerGameTimerD(whitePlayerGameTimerD), _blackPlayerGameTimerD(blackPlayerGameTimerD), _pieceRemoved(pieceRemoved) {}

ChessMoveHistory::~ChessMoveHistory() {}


void ChessMoveHistory::moveBackInHistory() {
	_pieceMoved->changePosition(_pieceMovedOriginPosition.x(), _pieceMovedOriginPosition.y());

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