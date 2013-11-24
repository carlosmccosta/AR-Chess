#include "ChessPiece.h"


ChessPiece::ChessPiece(ChessPieceType chessPieceType, ChessPieceColor chessPieceColor, int xPosition, int yPosition, Material* material) :
	_chessPieceType(chessPieceType),
	_chessPieceColor(chessPieceColor),
	_xPosition(xPosition),
	_yPosition(yPosition),
	_xInitialPosition(xPosition),
	_yInitialPosition(yPosition),
	_piecePlayable(true) {

	// necessary parameters to position piece in board
	string name = getPieceModelPath(chessPieceType, chessPieceColor);
	float modelSize = getPieceModelSize(chessPieceType);
	_pieceCurrentScenePosition = ChessUtils::computePieceScenePosition(xPosition, yPosition);
	double rotationAngle = 0;
	Vec3f rotationAxis = Vec3f(0.0, 0.0, 1.0);
	
	// rotate knights to face each other
	if (chessPieceType == KNIGHT && xPosition < 0) {		
		rotationAngle = osg::PI;
	}

	if (chessPieceType == BISHOP && xPosition > 0) {		
		rotationAngle = osg::PI;
	}

	_pieceMatrixTransform = ChessUtils::loadOSGModel(name, modelSize, material, false, _pieceCurrentScenePosition, rotationAngle, rotationAxis);
	_pieceMatrixTransform->setNodeMask(CAST_SHADOW_MASK);
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


void ChessPiece::changePosition(int xPosition, int yPosition) {
	_xPosition = xPosition;
	_yPosition = yPosition;

	Vec3f finalPieceScenePosition = ChessUtils::computePieceScenePosition(xPosition, yPosition);

	osg::AnimationPathCallback* animationPathCallback = new osg::AnimationPathCallback(ChessUtils::createChessPieceAnimationPath(_pieceCurrentScenePosition, finalPieceScenePosition));
	_pieceMatrixTransform->setUpdateCallback(animationPathCallback);
	_pieceCurrentScenePosition = finalPieceScenePosition;
}


void ChessPiece::resetPosition() {
	changePosition(_xInitialPosition, _yInitialPosition);
}


void ChessPiece::removePieceFromBoard() {
	/*_xPosition = 0;
	_yPosition = 0;*/
	_piecePlayable = false;

	Vec3f finalPieceScenePosition = ChessUtils::computePieceScenePosition(_yInitialPosition, -_xInitialPosition);
	if (_yInitialPosition < 0) {
		finalPieceScenePosition.x() -= (2 * BOARD_SQUARE_SIZE + PIECE_OUTSIDE_OFFSET);
		if (_yInitialPosition == -4) {
			finalPieceScenePosition.x() += BOARD_SQUARE_SIZE / 3.0;
		}
	} else {
		finalPieceScenePosition.x() += (2 * BOARD_SQUARE_SIZE + PIECE_OUTSIDE_OFFSET);
		if (_yInitialPosition == 4) {
			finalPieceScenePosition.x() -= BOARD_SQUARE_SIZE / 3.0;
		}
	}

	finalPieceScenePosition.y() *= BOARD_SQUARE_SHRINK_RATIO_FOR_OUTSIDE_PIECES;

	osg::AnimationPathCallback* animationPathCallback = new osg::AnimationPathCallback(ChessUtils::createChessPieceAnimationPath(_pieceCurrentScenePosition, finalPieceScenePosition, -osg::PI_2));
	_pieceMatrixTransform->setUpdateCallback(animationPathCallback);
	_pieceCurrentScenePosition = finalPieceScenePosition;
}


ChessPieceColor ChessPiece::getOpponentChessPieceColor(ChessPieceColor chessPieceColor) {
	ChessPieceColor opponentPieceColor = BLACK;
	if (chessPieceColor == BLACK) {
		opponentPieceColor = WHITE;
	}

	return opponentPieceColor;
}

