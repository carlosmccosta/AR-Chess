#include "ChessPiece.h"


ChessPiece::ChessPiece(ChessPieceType chessPieceType, ChessPieceColor chessPieceColor, int xPosition, int yPosition, Material* material, Vec3 scaleFactor, Vec3 translateOffset) :
	_chessPieceType(chessPieceType),
	_chessPieceColor(chessPieceColor),
	_xPosition(xPosition),
	_yPosition(yPosition),
	_chessPieceInitialType(chessPieceType),
	_xInitialPosition(xPosition),
	_yInitialPosition(yPosition),
	_piecePlayable(true),
	_pieceMovedPreviously(false),
	_pawnMakeDoubleStep(false),
	_playNumberOfLastMove(-1),
	_pieceMaterial(material) {

	_pieceMatrixTransform = new MatrixTransform();
	_pieceCurrentScenePosition = ChessUtils::computePieceScenePosition(xPosition, yPosition);
	loadPieceModel(chessPieceType, scaleFactor, translateOffset);
}

ChessPiece::~ChessPiece() {}


void ChessPiece::loadPieceModel(ChessPieceType chessPieceType, Vec3 scaleFactor, Vec3 translateOffset) {
	_chessPieceType = chessPieceType;

	// necessary parameters to position piece in board
	string name = getPieceModelPath(chessPieceType, _chessPieceColor);
	float modelSize = getPieceModelSize(chessPieceType);	
	_pieceCurrentScenePosition.x() += translateOffset.x();
	_pieceCurrentScenePosition.y() += translateOffset.y();
	_pieceCurrentScenePosition.z() += translateOffset.z();

	double rotationAngle = 0;
	Vec3f rotationAxis = Vec3f(0.0, 0.0, 1.0);

	// rotate knights to face each other
	if (chessPieceType == KNIGHT && _xPosition < 0) {
		rotationAngle = osg::PI;
	}

	if (chessPieceType == BISHOP && _xPosition > 0) {
		rotationAngle = osg::PI;
	}
	
	clearMatrixTransform();
	_pieceMatrixTransformModel = ChessUtils::loadOSGModel(name, modelSize, _pieceMaterial, false, _pieceCurrentScenePosition, rotationAngle, rotationAxis);
	_pieceMatrixTransformModel->postMult(Matrix::scale(scaleFactor));
	_pieceMatrixTransform->addChild(_pieceMatrixTransformModel);
	_pieceMatrixTransform->setNodeMask(CAST_SHADOW_MASK);
}


void ChessPiece::shrinkPiece() {
	osgAnimation::Motion* scaleEaseMotion = new osgAnimation::InBackMotion(1.0, PROMOTION_SCALE_ANIMATION_DURATION_SECONDS, -1.0);
	osg::AnimationPathCallback* animationPathCallback = new osg::AnimationPathCallback(ChessUtils::createScaleAnimationPath(_pieceCurrentScenePosition, scaleEaseMotion, PROMOTION_SCALE_ANIMATION_DURATION_SECONDS));
	_pieceMatrixTransformModel->setUpdateCallback(animationPathCallback);
}


void ChessPiece::unshrinkPiece() {
	osgAnimation::Motion* scaleEaseMotion = new osgAnimation::OutBackMotion(0, PROMOTION_SCALE_ANIMATION_DURATION_SECONDS, 1.0);
	osg::AnimationPathCallback* animationPathCallback = new osg::AnimationPathCallback(ChessUtils::createScaleAnimationPath(_pieceCurrentScenePosition, scaleEaseMotion, PROMOTION_SCALE_ANIMATION_DURATION_SECONDS));
	_pieceMatrixTransformModel->setUpdateCallback(animationPathCallback);
}


string ChessPiece::getPieceModelPath(ChessPieceType chessPieceType, ChessPieceColor chessPieceColor) {
	string pieceModelPath;
	
	if (chessPieceColor == WHITE) {
		switch (chessPieceType) {
			case KING:
				pieceModelPath = PIECE_WHITE_KING_MODEL;
				break;
			case QUEEN:
				pieceModelPath = PIECE_WHITE_QUEEN_MODEL;
				break;
			case ROOK:
				pieceModelPath = PIECE_WHITE_ROOK_MODEL;
				break;
			case KNIGHT:
				pieceModelPath = PIECE_WHITE_KNIGHT_MODEL;
				break;
			case BISHOP:
				pieceModelPath = PIECE_WHITE_BISHOP_MODEL;
				break;
			case PAWN:
				pieceModelPath = PIECE_WHITE_PAWN_MODEL;
				break;
			default:
				break;
		}
	} else {
		switch (chessPieceType) {
			case KING:
				pieceModelPath = PIECE_BLACK_KING_MODEL;
				break;
			case QUEEN:
				pieceModelPath = PIECE_BLACK_QUEEN_MODEL;
				break;
			case ROOK:
				pieceModelPath = PIECE_BLACK_ROOK_MODEL;
				break;
			case KNIGHT:
				pieceModelPath = PIECE_BLACK_KNIGHT_MODEL;
				break;
			case BISHOP:
				pieceModelPath = PIECE_BLACK_BISHOP_MODEL;
				break;
			case PAWN:
				pieceModelPath = PIECE_BLACK_PAWN_MODEL;
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


void ChessPiece::changePosition(int xPosition, int yPosition, int playNumber) {
	_xPosition = xPosition;
	_yPosition = yPosition;

	if (!_pieceMovedPreviously && _chessPieceType == PAWN) {
		if ((_chessPieceColor == WHITE && yPosition == -1) || (_chessPieceColor == BLACK && yPosition == 1)) {
			_pawnMakeDoubleStep = true;
		}
	}

	_pieceMovedPreviously = true;

	if (playNumber != -1) {
		_playNumberOfLastMove = playNumber;
	}

	Vec3f finalPieceScenePosition = ChessUtils::computePieceScenePosition(xPosition, yPosition);

	osg::AnimationPathCallback* animationPathCallback = new osg::AnimationPathCallback(ChessUtils::createChessPieceMoveAnimationPath(_pieceCurrentScenePosition, finalPieceScenePosition));
	_pieceMatrixTransformModel->setUpdateCallback(animationPathCallback);
	_pieceCurrentScenePosition = finalPieceScenePosition;
}


void ChessPiece::resetPosition() {
	if (_chessPieceType != _chessPieceInitialType) {
		loadPieceModel(_chessPieceInitialType);
	}

	changePosition(_xInitialPosition, _yInitialPosition);
	_pieceMovedPreviously = false;
	_piecePlayable = true;
	_pawnMakeDoubleStep = false;
	_playNumberOfLastMove = -1;	
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

	osg::AnimationPathCallback* animationPathCallback = new osg::AnimationPathCallback(ChessUtils::createChessPieceMoveAnimationPath(_pieceCurrentScenePosition, finalPieceScenePosition, -osg::PI_2));
	_pieceMatrixTransformModel->setUpdateCallback(animationPathCallback);
	_pieceCurrentScenePosition = finalPieceScenePosition;
}


void ChessPiece::reinsertPieceOnBoard() {
	_piecePlayable = true;
	changePosition(_xPosition, _yPosition);
}


void ChessPiece::clearMatrixTransform() {
	_pieceMatrixTransform->removeChildren(0, _pieceMatrixTransform->getNumChildren());
}


ChessPieceColor ChessPiece::getOpponentChessPieceColor(ChessPieceColor chessPieceColor) {
	ChessPieceColor opponentPieceColor = BLACK;
	if (chessPieceColor == BLACK) {
		opponentPieceColor = WHITE;
	}

	return opponentPieceColor;
}

