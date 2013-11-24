#include "ChessMoveHistory.h"


ChessMoveHistory::ChessMoveHistory(ChessPiece* pieceMoved, Vec2i pieceMovedOriginPosition, ChessPiece* pieceRemoved) :
	_pieceMoved(pieceMoved), _pieceMovedOriginPosition(pieceMovedOriginPosition), _pieceRemoved(pieceRemoved) {}

ChessMoveHistory::~ChessMoveHistory() {}
