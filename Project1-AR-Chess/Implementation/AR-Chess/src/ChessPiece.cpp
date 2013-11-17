#include "ChessPiece.h"


ChessPiece::ChessPiece() {}
ChessPiece::ChessPiece(ChessPieceType chessPieceType, ChessPieceColor chessPieceColor, int xPosition, int yPosition) :
	_chessPieceType(chessPieceType),
	_chessPieceColor(chessPieceColor),
	_xPosition(xPosition),
	_yPosition(yPosition),
	_piecePlayable(true) {

	// necessary parameters to position piece in board
	string name = getPieceModelPath(chessPieceType, chessPieceColor);
	float modelSize = getPieceModelSize(chessPieceType);
	Vec3f pieceScenePosition = computePieceScenePosition(xPosition, yPosition);
	double rotationAngle = 0;
	Vec3f rotationAxis = Vec3f(0.0, 0.0, 1.0);
	
	// rotate knights to face each other
	if (chessPieceType == KNIGHT) {
		if (xPosition < 0) {
			rotationAngle = osg::PI;
		}
	}

	_pieceMatrixTransform = ChessUtils::loadOSGModel(name, modelSize, pieceScenePosition, rotationAngle, rotationAxis);
}

ChessPiece::~ChessPiece() {}


string ChessPiece::getPieceModelPath(ChessPieceType chessPieceType, ChessPieceColor chessPieceColor) {
	string pieceModelPath;
	
	if (chessPieceColor == WHITE) {
		switch (chessPieceType) {
			case KING:
				pieceModelPath = PIECE_WHITE_KING;
				break;
			case QUEEN:
				pieceModelPath = PIECE_WHITE_QUEEN;
				break;
			case ROOK:
				pieceModelPath = PIECE_WHITE_ROOK;
				break;
			case KNIGHT:
				pieceModelPath = PIECE_WHITE_KNIGHT;
				break;
			case BISHOP:
				pieceModelPath = PIECE_WHITE_BISHOP;
				break;
			case PAWN:
				pieceModelPath = PIECE_WHITE_PAWN;
				break;
			default:
				break;
		}
	} else {
		switch (chessPieceType) {
			case KING:
				pieceModelPath = PIECE_BLACK_KING;
				break;
			case QUEEN:
				pieceModelPath = PIECE_BLACK_QUEEN;
				break;
			case ROOK:
				pieceModelPath = PIECE_BLACK_ROOK;
				break;
			case KNIGHT:
				pieceModelPath = PIECE_BLACK_KNIGHT;
				break;
			case BISHOP:
				pieceModelPath = PIECE_BLACK_BISHOP;
				break;
			case PAWN:
				pieceModelPath = PIECE_BLACK_PAWN;
				break;
			default:
				break;
		}
	}

	return pieceModelPath;
}


float ChessPiece::getPieceModelSize(ChessPieceType chessPieceType) {
	float pieceModelSize = 0;
	switch (chessPieceType) {
		case KING:
			pieceModelSize = PIECE_KING_SIZE;
			break;
		case QUEEN:
			pieceModelSize = PIECE_QUEEN_SIZE;
			break;
		case ROOK:
			pieceModelSize = PIECE_ROOK_SIZE;
			break;
		case KNIGHT:
			pieceModelSize = PIECE_KNIGHT_SIZE;
			break;
		case BISHOP:
			pieceModelSize = PIECE_BISHOP_SIZE;
			break;
		case PAWN:
			pieceModelSize = PIECE_PAWN_SIZE;
			break;
		default:
			break;
	}

	return pieceModelSize;
}


Vec3f ChessPiece::computePieceScenePosition(int xPosition, int yPosition) {
	float shiftX = xPosition * BOARD_SQUARE_SIZE;
	float shiftY = yPosition * BOARD_SQUARE_SIZE;
	float shiftZ = BOARD_HEIGHT / 2.0;

	// adjust position to middle of square
	double halfSquareSize = BOARD_SQUARE_SIZE / 2.0;
	if (shiftX < 0) {
		shiftX += halfSquareSize;
	} else {
		shiftX -= halfSquareSize;
	}

	if (shiftY < 0) {
		shiftY += halfSquareSize;
	} else {
		shiftY -= halfSquareSize;
	}

	return Vec3f(shiftX, shiftY, shiftZ);
}


bool ChessPiece::changePosition(int xPosition, int yPosition) {
	return false;
}
