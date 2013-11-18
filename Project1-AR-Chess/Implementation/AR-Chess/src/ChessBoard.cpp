#include "ChessBoard.h"


ChessBoard::ChessBoard() {
	_paddleSelectorImage = osgDB::readImageFile(PADDLE_SELECTOR);
	_paddleSelectedImage = osgDB::readImageFile(PADDLE_SELECTED);

	// materials setup
	float boardShininess = 128.0;
	Vec4 boardEmission = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
	Vec4 boardSpecular = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Vec4 boardDiffuse = Vec4(0.4f, 0.4f, 0.4f, 0.5f);
	Vec4 boardAmbient = Vec4(0.05f, 0.05f, 0.05f, 0.05f);
	_boardMaterial = ChessUtils::createMaterial(boardShininess, boardEmission, boardSpecular, boardDiffuse, boardAmbient);
	//_boardMaterial = NULL;

	float whitePieceShininess = 128.0;
	Vec4 whitePieceEmission = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
	Vec4 whitePieceSpecular = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Vec4 whitePieceDiffuse = Vec4(0.05f, 0.05f, 0.05f, 0.5f);
	Vec4 whitePieceAmbient = Vec4(0.02f, 0.02f, 0.02f, 0.2f);
	_whitePiecesMaterial = ChessUtils::createMaterial(whitePieceShininess, whitePieceEmission, whitePieceSpecular, whitePieceDiffuse, whitePieceAmbient);
	//_whitePiecesMaterial = NULL;

	float blackPieceShininess = 128.0;
	Vec4 blackPieceEmission = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
	Vec4 blackPieceSpecular = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Vec4 blackPieceDiffuse = Vec4(0.05f, 0.05f, 0.05f, 0.5f);
	Vec4 blackPieceAmbient = Vec4(0.02f, 0.02f, 0.02f, 0.2f);
	_blackPiecesMaterial = ChessUtils::createMaterial(blackPieceShininess, blackPieceEmission, blackPieceSpecular, blackPieceDiffuse, blackPieceAmbient);
	//_blackPiecesMaterial = NULL;


	// board setup
	Vec3 modelCenterShift = Vec3(0.0, 0.0, 0.0);
	double rotationAngle = 0;
	Vec3 rotationAxis = Vec3(0.0, 0.0, 1.0);
	float xModelCenterOffsetPercentage = 0; 
	float yModelCenterOffsetPercentage = 0; 
	float zModelCenterOffsetPercentage = -0.5;

	_boardShadowedScene = new osgShadow::ShadowedScene();
	_boardShadowedScene->setReceivesShadowTraversalMask(RECEIVE_SHADOW_MASK);
	_boardShadowedScene->setCastsShadowTraversalMask(CAST_SHADOW_MASK);

	osg::TexEnv* blendTexEnv = new osg::TexEnv();
	blendTexEnv->setMode(osg::TexEnv::BLEND);
	_boardShadowedScene->getOrCreateStateSet()->setTextureAttribute(0, blendTexEnv);

	MatrixTransform* boardModelMT = ChessUtils::loadOSGModel(BOARD_MODEL, BOARD_SIZE, _boardMaterial, false, modelCenterShift, rotationAngle, rotationAxis,
		xModelCenterOffsetPercentage, yModelCenterOffsetPercentage, zModelCenterOffsetPercentage);
	boardModelMT->setNodeMask(RECEIVE_SHADOW_MASK);

	_overlays = new Group();
	_boardShadowedScene->addChild(boardModelMT);	
	_boardShadowedScene->addChild(_overlays);
}

ChessBoard::~ChessBoard() {}


osgShadow::ShadowedScene* ChessBoard::setupBoard() {
	// reset board
	_whiteChessPieces.clear();
	_blackChessPieces.clear();
	
	//setup initial positions

	// kings
	_whiteChessPieces.push_back(ChessPiece(KING, WHITE, 1, -4, _whitePiecesMaterial));
	_blackChessPieces.push_back(ChessPiece(KING, BLACK, 1, 4, _blackPiecesMaterial));

	//queens
	_whiteChessPieces.push_back(ChessPiece(QUEEN, WHITE, -1, -4, _whitePiecesMaterial));
	_blackChessPieces.push_back(ChessPiece(QUEEN, BLACK, -1, 4, _blackPiecesMaterial));

	// rooks
	_whiteChessPieces.push_back(ChessPiece(ROOK, WHITE, -4, -4, _whitePiecesMaterial));
	_whiteChessPieces.push_back(ChessPiece(ROOK, WHITE, 4, -4, _whitePiecesMaterial));
	_blackChessPieces.push_back(ChessPiece(ROOK, BLACK, -4, 4, _blackPiecesMaterial));
	_blackChessPieces.push_back(ChessPiece(ROOK, BLACK, 4, 4, _blackPiecesMaterial));

	// knights
	_whiteChessPieces.push_back(ChessPiece(KNIGHT, WHITE, -3, -4, _whitePiecesMaterial));
	_whiteChessPieces.push_back(ChessPiece(KNIGHT, WHITE, 3, -4, _whitePiecesMaterial));
	_blackChessPieces.push_back(ChessPiece(KNIGHT, BLACK, -3, 4, _blackPiecesMaterial));
	_blackChessPieces.push_back(ChessPiece(KNIGHT, BLACK, 3, 4, _blackPiecesMaterial));

	// bishops
	_whiteChessPieces.push_back(ChessPiece(BISHOP, WHITE, -2, -4, _whitePiecesMaterial));
	_whiteChessPieces.push_back(ChessPiece(BISHOP, WHITE, 2, -4, _whitePiecesMaterial));
	_blackChessPieces.push_back(ChessPiece(BISHOP, BLACK, -2, 4, _blackPiecesMaterial));
	_blackChessPieces.push_back(ChessPiece(BISHOP, BLACK, 2, 4, _blackPiecesMaterial));

	// paws
	for (int pawnPosition = -4; pawnPosition < 5; ++pawnPosition) {
		if (pawnPosition != 0) {
			_whiteChessPieces.push_back(ChessPiece(PAWN, WHITE, pawnPosition, -3, _whitePiecesMaterial));
			_blackChessPieces.push_back(ChessPiece(PAWN, BLACK, pawnPosition, 3, _blackPiecesMaterial));
		}
	}


	// add pieces to the board
	for (size_t whitePiecePos = 0; whitePiecePos < _whiteChessPieces.size(); ++whitePiecePos) {
		_boardShadowedScene->addChild(_whiteChessPieces[whitePiecePos].getPieceMatrixTransform());
	}

	for (size_t blackPiecePos = 0; blackPiecePos < _blackChessPieces.size(); ++blackPiecePos) {
		_boardShadowedScene->addChild(_blackChessPieces[blackPiecePos].getPieceMatrixTransform());
	}

	hightLighPosition(-1, -1);
	hightLighPosition(-1, -4);
	hightLighPosition(-1, -3);
	selectPosition(1, 1, WHITE);
	selectPosition(1, -3, WHITE);
	selectPosition(1, -4, WHITE);

	return _boardShadowedScene;
}


bool ChessBoard::selectPosition(int xBoardPosition, int yBoardPosition, ChessPieceColor chessPieceType) {
	if (!isPositionValid(xBoardPosition) || !isPositionValid(yBoardPosition)) {
		return false;
	}

	if (isBoardSquareWithPiece(xBoardPosition, yBoardPosition, chessPieceType)) {		
		Vec3f scenePosition = ChessUtils::computePieceScenePosition(xBoardPosition, yBoardPosition);		
		scenePosition.z() = PADDLE_OVERLAYS_HEIGHT_OFFSET;
		_overlays->addChild(ChessUtils::createRectangleWithTexture(scenePosition, _paddleSelectedImage));

		return true;
	}

	return false;	
}


bool ChessBoard::hightLighPosition(int xBoardPosition, int yBoardPosition) {
	if (!isPositionValid(xBoardPosition) || !isPositionValid(yBoardPosition)) {
		return false;
	}

	Vec3f scenePosition = ChessUtils::computePieceScenePosition(xBoardPosition, yBoardPosition);
	scenePosition.z() = PADDLE_OVERLAYS_HEIGHT_OFFSET;
	_overlays->addChild(ChessUtils::createRectangleWithTexture(scenePosition, _paddleSelectorImage));
	return true;
}


bool ChessBoard::isBoardSquareWithPiece(int xBoardPosition, int yBoardPosition, ChessPieceColor chessPieceType) {
	return true;

	if (!isPositionValid(xBoardPosition) || !isPositionValid(yBoardPosition)) {
		return false;
	}
	
	vector<ChessPiece>& piecesToCheck = _blackChessPieces;
	if (chessPieceType == WHITE) {
		piecesToCheck = _whiteChessPieces;
	}

	for (size_t i = 0; i < piecesToCheck.size(); ++i) {
		if (piecesToCheck[i].getXPosition() == xBoardPosition && piecesToCheck[i].getYPosition() == yBoardPosition) {
			return true;
		}
	}

	return false;
}


bool ChessBoard::isPositionValid(int position) {
	if ((position > -5 && position < 0) || (position > 0 && position < 5)) {
		return true;
	} else {
		return false;
	}
}

