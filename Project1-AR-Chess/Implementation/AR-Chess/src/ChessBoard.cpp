#include "ChessBoard.h"


ChessBoard::ChessBoard() {
	_boardMatrixTransform = ChessUtils::loadOSGModel(BOARD_MODEL, BOARD_SIZE, Vec3f(0.0, 0.0, 0.0), 0, Vec3f(0.0, 0.0, 1.0), true, true, true);
}

ChessBoard::~ChessBoard() {}


MatrixTransform* ChessBoard::setupBoard() {
	// reset board
	_whiteChessPieces.clear();
	_blackChessPieces.clear();

	//setup initial positions

	// kings
	_whiteChessPieces.push_back(ChessPiece(KING, WHITE, 1, -4));
	_blackChessPieces.push_back(ChessPiece(KING, BLACK, 1, 4));

	//queens
	_whiteChessPieces.push_back(ChessPiece(QUEEN, WHITE, -1, -4));
	_blackChessPieces.push_back(ChessPiece(QUEEN, BLACK, -1, 4));

	// rooks
	_whiteChessPieces.push_back(ChessPiece(ROOK, WHITE, -4, -4));
	_whiteChessPieces.push_back(ChessPiece(ROOK, WHITE, 4, -4));
	_blackChessPieces.push_back(ChessPiece(ROOK, BLACK, -4, 4));
	_blackChessPieces.push_back(ChessPiece(ROOK, BLACK, 4, 4));

	// knights
	_whiteChessPieces.push_back(ChessPiece(KNIGHT, WHITE, -3, -4));
	_whiteChessPieces.push_back(ChessPiece(KNIGHT, WHITE, 3, -4));
	_blackChessPieces.push_back(ChessPiece(KNIGHT, BLACK, -3, 4));
	_blackChessPieces.push_back(ChessPiece(KNIGHT, BLACK, 3, 4));

	// bishops
	_whiteChessPieces.push_back(ChessPiece(BISHOP, WHITE, -2, -4));
	_whiteChessPieces.push_back(ChessPiece(BISHOP, WHITE, 2, -4));
	_blackChessPieces.push_back(ChessPiece(BISHOP, BLACK, -2, 4));
	_blackChessPieces.push_back(ChessPiece(BISHOP, BLACK, 2, 4));

	// paws
	for (int pawnPosition = -4; pawnPosition < 5; ++pawnPosition) {
		if (pawnPosition != 0) {
			_whiteChessPieces.push_back(ChessPiece(PAWN, WHITE, pawnPosition, -3));
			_blackChessPieces.push_back(ChessPiece(PAWN, BLACK, pawnPosition, 3));
		}
	}


	// add pieces to the board
	for (size_t whitePiecePos = 0; whitePiecePos < _whiteChessPieces.size(); ++whitePiecePos) {
		_boardMatrixTransform->addChild(_whiteChessPieces[whitePiecePos].getPieceMatrixTransform());
	}

	for (size_t blackPiecePos = 0; blackPiecePos < _blackChessPieces.size(); ++blackPiecePos) {
		_boardMatrixTransform->addChild(_blackChessPieces[blackPiecePos].getPieceMatrixTransform());
	}


	return _boardMatrixTransform;
}
