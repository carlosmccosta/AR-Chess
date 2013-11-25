#include "ChessBoard.h"


ChessBoard::ChessBoard() {
	setupImages();
	setupMaterials();

	_font3D = osgText::readFont3DFile(TEXT_FONT);
	_lastSelectorBoardPosition = Vec2i(0, 0);


	// board setup
	Vec3 modelCenterShift = Vec3(0.0, 0.0, 0.0);
	double rotationAngle = 0;
	Vec3 rotationAxis = Vec3(0.0, 0.0, 1.0);
	Vec3 modelCenterOffsetPercentage = Vec3(0.0, 0.0, -0.5);

	_boardShadowedScene = new osgShadow::ShadowedScene();
	_boardShadowedScene->setReceivesShadowTraversalMask(RECEIVE_SHADOW_MASK);
	_boardShadowedScene->setCastsShadowTraversalMask(CAST_SHADOW_MASK);

	osg::TexEnv* blendTexEnv = new osg::TexEnv();
	blendTexEnv->setMode(osg::TexEnv::BLEND);
	_boardShadowedScene->getOrCreateStateSet()->setTextureAttribute(0, blendTexEnv);

	MatrixTransform* boardModelMT = ChessUtils::loadOSGModel(BOARD_MODEL, BOARD_SIZE, _boardMaterial, false, modelCenterShift, rotationAngle, rotationAxis, modelCenterOffsetPercentage);
	boardModelMT->setNodeMask(RECEIVE_SHADOW_MASK);
	
	setupNewGameSelectors();
	setupGameHistoryManipulators();
	updateHistoryManipulatorsVisibility();

	// osg setup
	_playersPieces = new Group();
	_playersTimers = new Group();
	_boardSquareSelectorHighlights = new Group();
	_boardSquareSelections = new Group();	
	_promotionPieces = new Group();
	
	setupBoardPieces();
	setupPlayersTimers();

	_whitePlayerGameTimer = new ElapsedTime();
	_blackPlayerGameTimer = new ElapsedTime();
	_selectorTimer = new ElapsedTime();
	_animationDelayBetweenMoves = new ElapsedTime();
	_promotionConversionTimer = new ElapsedTime();	

	_boardShadowedScene->addChild(boardModelMT);
	_boardShadowedScene->addChild(_playersPieces);
	_boardShadowedScene->addChild(_playersTimers);
	_boardShadowedScene->addChild(_boardSquareSelectorHighlights);
	_boardShadowedScene->addChild(_boardSquareSelections);
	_boardShadowedScene->addChild(_promotionPieces);
}

ChessBoard::~ChessBoard() {}


void ChessBoard::setupImages() {
	// images loading
	_paddleSelectorImage = osgDB::readImageFile(PADDLE_SELECTOR_IMG);
	_paddleSelectedImage = osgDB::readImageFile(PADDLE_SELECTED_IMG);
	_newGameHHImg = osgDB::readImageFile(NEW_GAME_H_H_IMG);
	_newGameHHSelectedImg = osgDB::readImageFile(NEW_GAME_H_H_SELECTED_IMG);
	_newGameHCImg = osgDB::readImageFile(NEW_GAME_H_C_IMG);
	_newGameHCSelectedImg = osgDB::readImageFile(NEW_GAME_H_C_SELECTED_IMG);
	_newGameCCImg = osgDB::readImageFile(NEW_GAME_C_C_IMG);
	_newGameCCSelectedImg = osgDB::readImageFile(NEW_GAME_C_C_SELECTED_IMG);
	_previousMoveImg = osgDB::readImageFile(HISTORY_PREVIOUS_MOVE_IMG);
	_previousMoveSelectedImg = osgDB::readImageFile(HISTORY_PREVIOUS_MOVE_SELECTED_IMG);
	_nextMoveImg = osgDB::readImageFile(HISTORY_NEXT_MOVE_IMG);
	_nextMoveSelectedImg = osgDB::readImageFile(HISTORY_NEXT_MOVE_SELECTED_IMG);
}


void ChessBoard::setupMaterials() {
	// materials setup
	float boardShininess = 128.0;
	Vec4 boardEmission = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
	Vec4 boardSpecular = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Vec4 boardDiffuse = Vec4(0.4f, 0.4f, 0.4f, 0.5f);
	Vec4 boardAmbient = Vec4(0.05f, 0.05f, 0.05f, 0.05f);
	_boardMaterial = ChessUtils::createMaterial(boardShininess, boardEmission, boardSpecular, boardDiffuse, boardAmbient);

	float whitePieceShininess = 128.0;
	Vec4 whitePieceEmission = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
	Vec4 whitePieceSpecular = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Vec4 whitePieceDiffuse = Vec4(0.05f, 0.05f, 0.05f, 0.5f);
	Vec4 whitePieceAmbient = Vec4(0.02f, 0.02f, 0.02f, 0.2f);
	_whitePiecesMaterial = ChessUtils::createMaterial(whitePieceShininess, whitePieceEmission, whitePieceSpecular, whitePieceDiffuse, whitePieceAmbient);

	float blackPieceShininess = 128.0;
	Vec4 blackPieceEmission = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
	Vec4 blackPieceSpecular = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Vec4 blackPieceDiffuse = Vec4(0.05f, 0.05f, 0.05f, 0.5f);
	Vec4 blackPieceAmbient = Vec4(0.02f, 0.02f, 0.02f, 0.2f);
	_blackPiecesMaterial = ChessUtils::createMaterial(blackPieceShininess, blackPieceEmission, blackPieceSpecular, blackPieceDiffuse, blackPieceAmbient);	
}


void ChessBoard::setupNewGameSelectors() {
	// new game selectors
	float halfBoardSquareSize = BOARD_SQUARE_SIZE / 2.0;
	_boardShadowedScene->addChild(setupAuxiliarySelector(Vec2i(5, -4), _newGameHHImg, Vec3(AUXILIARY_SELECTORS_X_OFFSET + halfBoardSquareSize, AUXILIARY_SELECTORS_Y_OFFSET, AUXILIARY_SELECTORS_Z_OFFSET)));
	_boardShadowedScene->addChild(setupAuxiliarySelector(Vec2i(-5, 4), _newGameHHImg, Vec3(-AUXILIARY_SELECTORS_X_OFFSET - halfBoardSquareSize, -AUXILIARY_SELECTORS_Y_OFFSET, AUXILIARY_SELECTORS_Z_OFFSET), osg::PI));
	_boardShadowedScene->addChild(setupAuxiliarySelector(Vec2i(5, -5), _newGameHCImg));
	_boardShadowedScene->addChild(setupAuxiliarySelector(Vec2i(-5, 5), _newGameHCImg, Vec3(-AUXILIARY_SELECTORS_X_OFFSET, -AUXILIARY_SELECTORS_Y_OFFSET, AUXILIARY_SELECTORS_Z_OFFSET), osg::PI));
	_boardShadowedScene->addChild(setupAuxiliarySelector(Vec2i(6, -5), _newGameCCImg));
	_boardShadowedScene->addChild(setupAuxiliarySelector(Vec2i(-6, 5), _newGameCCImg, Vec3(-AUXILIARY_SELECTORS_X_OFFSET, -AUXILIARY_SELECTORS_Y_OFFSET, AUXILIARY_SELECTORS_Z_OFFSET), osg::PI));
}


void ChessBoard::setupGameHistoryManipulators() {
	// game history manipulators selectors
	MatrixTransform* goBackwardsSelectorWhiteSideMT = setupAuxiliarySelector(Vec2i(-6, -4), _previousMoveImg,
		Vec3(-AUXILIARY_SELECTORS_X_OFFSET, -AUXILIARY_SELECTORS_Y_OFFSET, AUXILIARY_SELECTORS_Z_OFFSET), 0.0,
		Vec3(HISTORY_IMGS_SCALE, HISTORY_IMGS_SCALE, HISTORY_IMGS_SCALE));
	_boardShadowedScene->addChild(goBackwardsSelectorWhiteSideMT);
	_moveBackwardsInHistorySelectorWhiteSide = goBackwardsSelectorWhiteSideMT->getChild(0)->asGeode()->getDrawable(0)->asGeometry();

	MatrixTransform* goBackwardsSelectorBlackSideMT = setupAuxiliarySelector(Vec2i(6, 4), _previousMoveImg,
		Vec3(AUXILIARY_SELECTORS_X_OFFSET, AUXILIARY_SELECTORS_Y_OFFSET, AUXILIARY_SELECTORS_Z_OFFSET), osg::PI,
		Vec3(HISTORY_IMGS_SCALE, HISTORY_IMGS_SCALE, HISTORY_IMGS_SCALE));
	_boardShadowedScene->addChild(goBackwardsSelectorBlackSideMT);
	_moveBackwardsInHistorySelectorBlackSide = goBackwardsSelectorBlackSideMT->getChild(0)->asGeode()->getDrawable(0)->asGeometry();

	MatrixTransform* goFowardSelectorWhiteSideMT = setupAuxiliarySelector(Vec2i(-5, -4), _nextMoveImg,
		Vec3(-AUXILIARY_SELECTORS_X_OFFSET, -AUXILIARY_SELECTORS_Y_OFFSET, AUXILIARY_SELECTORS_Z_OFFSET), 0.0,
		Vec3(HISTORY_IMGS_SCALE, HISTORY_IMGS_SCALE, HISTORY_IMGS_SCALE));
	_boardShadowedScene->addChild(goFowardSelectorWhiteSideMT);
	_moveFowardInHistorySelectorWhiteSide = goFowardSelectorWhiteSideMT->getChild(0)->asGeode()->getDrawable(0)->asGeometry();

	MatrixTransform* goFowardSelectorBlackSideMT = setupAuxiliarySelector(Vec2i(5, 4), _nextMoveImg,
		Vec3(AUXILIARY_SELECTORS_X_OFFSET, AUXILIARY_SELECTORS_Y_OFFSET, AUXILIARY_SELECTORS_Z_OFFSET), osg::PI,
		Vec3(HISTORY_IMGS_SCALE, HISTORY_IMGS_SCALE, HISTORY_IMGS_SCALE));
	_boardShadowedScene->addChild(goFowardSelectorBlackSideMT);
	_moveFowardInHistorySelectorBlackSide = goFowardSelectorBlackSideMT->getChild(0)->asGeode()->getDrawable(0)->asGeometry();
}


osgShadow::ShadowedScene* ChessBoard::setupBoard() {
	_currentPlayer = WHITE;
	_pieceToMove = NULL;
	_moveOriginPosition = Vec2i(0, 0);
	_moveDestinationPosition = Vec2i(0, 0);
	
	_whitePlayerGameTimer->finish();
	_blackPlayerGameTimer->finish();
	_selectorTimer->finish();
	_animationDelayBetweenMoves->finish();
	_promotionConversionTimer->finish();

	_whitePlayerGameTimerD = 0;
	_blackPlayerGameTimerD = 0;

	_animationInProgress = false;
	_piecePromotionInProgress = false;
	_piecePromotionConversionInProgress = false;
	_piecePromotionConversionFromHistoryInProgress = false;
	_piecePromotionReversionInProgress = false;
	

	updatePlayerStatus(_whitePlayerGameTimerText, _blackPlayerGameTimerText);	

	return _boardShadowedScene;
}


osgShadow::ShadowedScene* ChessBoard::resetBoard() {
	// move pieces to their original position
	for (size_t whitePiecePos = 0; whitePiecePos < _whiteChessPieces.size(); ++whitePiecePos) {
		_whiteChessPieces[whitePiecePos]->resetPosition();
	}

	for (size_t blackPiecePos = 0; blackPiecePos < _blackChessPieces.size(); ++blackPiecePos) {
		_blackChessPieces[blackPiecePos]->resetPosition();
	}

	clearSelections();
	_animationDelayBetweenMoves->reset();
	_whitePlayerGameTimerText->setText(ChessUtils::formatSecondsToDate(0));
	_blackPlayerGameTimerText->setText(ChessUtils::formatSecondsToDate(0));

	return setupBoard();
}


void ChessBoard::setupBoardPieces() {
	//setup initial positions

	// kings
	_whiteChessPieces.push_back(new ChessPiece(KING, WHITE, 1, -4, _whitePiecesMaterial));
	_blackChessPieces.push_back(new ChessPiece(KING, BLACK, 1, 4, _blackPiecesMaterial));

	//queens
	_whiteChessPieces.push_back(new ChessPiece(QUEEN, WHITE, -1, -4, _whitePiecesMaterial));
	_blackChessPieces.push_back(new ChessPiece(QUEEN, BLACK, -1, 4, _blackPiecesMaterial));

	// rooks
	_whiteChessPieces.push_back(new ChessPiece(ROOK, WHITE, -4, -4, _whitePiecesMaterial));
	_whiteChessPieces.push_back(new ChessPiece(ROOK, WHITE, 4, -4, _whitePiecesMaterial));
	_blackChessPieces.push_back(new ChessPiece(ROOK, BLACK, -4, 4, _blackPiecesMaterial));
	_blackChessPieces.push_back(new ChessPiece(ROOK, BLACK, 4, 4, _blackPiecesMaterial));

	// knights
	_whiteChessPieces.push_back(new ChessPiece(KNIGHT, WHITE, -3, -4, _whitePiecesMaterial));
	_whiteChessPieces.push_back(new ChessPiece(KNIGHT, WHITE, 3, -4, _whitePiecesMaterial));
	_blackChessPieces.push_back(new ChessPiece(KNIGHT, BLACK, -3, 4, _blackPiecesMaterial));
	_blackChessPieces.push_back(new ChessPiece(KNIGHT, BLACK, 3, 4, _blackPiecesMaterial));

	// bishops
	_whiteChessPieces.push_back(new ChessPiece(BISHOP, WHITE, -2, -4, _whitePiecesMaterial));
	_whiteChessPieces.push_back(new ChessPiece(BISHOP, WHITE, 2, -4, _whitePiecesMaterial));
	_blackChessPieces.push_back(new ChessPiece(BISHOP, BLACK, -2, 4, _blackPiecesMaterial));
	_blackChessPieces.push_back(new ChessPiece(BISHOP, BLACK, 2, 4, _blackPiecesMaterial));

	// paws
	for (int pawnPosition = -4; pawnPosition < 5; ++pawnPosition) {
		if (pawnPosition != 0) {
			_whiteChessPieces.push_back(new ChessPiece(PAWN, WHITE, pawnPosition, -3, _whitePiecesMaterial));
			_blackChessPieces.push_back(new ChessPiece(PAWN, BLACK, pawnPosition, 3, _blackPiecesMaterial));
		}
	}


	// add pieces to the board
	for (size_t whitePiecePos = 0; whitePiecePos < _whiteChessPieces.size(); ++whitePiecePos) {
		_playersPieces->addChild(_whiteChessPieces[whitePiecePos]->getPieceMatrixTransform());
	}

	for (size_t blackPiecePos = 0; blackPiecePos < _blackChessPieces.size(); ++blackPiecePos) {
		_playersPieces->addChild(_blackChessPieces[blackPiecePos]->getPieceMatrixTransform());
	}
}


void ChessBoard::setupPlayersTimers() {
	float playersStatusXOffsetToBoardBorder = 4 * BOARD_SQUARE_SIZE + PLAYER_STATUS_BOARD_BORDER_OFFSET;
	_whitePlayerStatus = ChessUtils::createRectangleWithTexture(Vec3(0, 0, 0), osgDB::readImageFile(WHITE_PLAYER_STATUS_IMG), PLAYER_STATUS_WIDTH, PLAYER_STATUS_HEIGHT);
	_blackPlayerStatus = ChessUtils::createRectangleWithTexture(Vec3(0, 0, 0), osgDB::readImageFile(BLACK_PLAYER_STATUS_IMG), PLAYER_STATUS_WIDTH, PLAYER_STATUS_HEIGHT);

	Vec3 textPosition = Vec3(PLAYER_STATUS_TEXT_LEFT_OFFSET, PLAYER_STATUS_TEXT_UP_OFFSET, PLAYER_STATUS_TEXT_Z_OFFSET);
	_whitePlayerGameTimerText = ChessUtils::createText3D(ChessUtils::formatSecondsToDate(0), _font3D, textPosition, PLAYER_STATUS_TEXT_SIZE, PLAYER_STATUS_TEXT_DEPTH);
	_blackPlayerGameTimerText = ChessUtils::createText3D(ChessUtils::formatSecondsToDate(0), _font3D, textPosition, PLAYER_STATUS_TEXT_SIZE, PLAYER_STATUS_TEXT_DEPTH);

	setupPlayerTimer(_whitePlayerStatus, _whitePlayerGameTimerText, osg::PI_2, Vec3(-playersStatusXOffsetToBoardBorder, 0, PLAYER_STATUS_Z_OFFSET));
	setupPlayerTimer(_blackPlayerStatus, _blackPlayerGameTimerText, -osg::PI_2, Vec3(playersStatusXOffsetToBoardBorder, 0, PLAYER_STATUS_Z_OFFSET));
}


void ChessBoard::setupPlayerTimer(Geode* backgroundImage, Text3D* timerText, float rotationAngle, Vec3 playersStatusOffsetToBoardBorder) {
	MatrixTransform* playerTimerMT = new MatrixTransform();	
	playerTimerMT->addChild(backgroundImage);
	
	Geode* timerTextGeode = new Geode();
	timerTextGeode->addDrawable(timerText);		

	playerTimerMT->addChild(timerTextGeode);
	playerTimerMT->postMult(Matrix::rotate(rotationAngle, osg::Z_AXIS));
	playerTimerMT->postMult(Matrix::translate(playersStatusOffsetToBoardBorder));
	_playersTimers->addChild(playerTimerMT);
}


MatrixTransform* ChessBoard::setupAuxiliarySelector(Vec2i position, Image* image, Vec3 translateOffset, float rotationAngle, Vec3 scale) {
	Vec3f newGameHHTopScenePosition = ChessUtils::computePieceScenePosition(position.x(), position.y());
	newGameHHTopScenePosition.x() += translateOffset.x();
	newGameHHTopScenePosition.y() += translateOffset.y();
	newGameHHTopScenePosition.z() = BOARD_HIGHLIGHTS_HEIGHT_OFFSET + translateOffset.z();

	Geode* newGameHHTopSceneGeode = ChessUtils::createRectangleWithTexture(Vec3(0, 0, 0), new Image(*image));
	MatrixTransform* newGameHHTopSceneMT = new MatrixTransform();
	newGameHHTopSceneMT->addChild(newGameHHTopSceneGeode);
	newGameHHTopSceneMT->postMult(Matrix::rotate(rotationAngle, osg::Z_AXIS));
	newGameHHTopSceneMT->postMult(Matrix::scale(scale));
	newGameHHTopSceneMT->postMult(Matrix::translate(newGameHHTopScenePosition));
	
	return newGameHHTopSceneMT;
}


bool ChessBoard::updateBoard(Vec2i selectorBoardPosition) {
	updatePlayersGameTimers();

	if (_piecePromotionConversionFromHistoryInProgress && !_piecePromotionConversionInProgress) {
		if (_animationDelayBetweenMoves->elapsedTime() > _timeToWaitForPromotingPieceFromHistory) {
			_pieceToPromote->shrinkPiece();
			_promotionConversionTimer->finish();
			_animationDelayBetweenMoves->finish();
			_piecePromotionConversionInProgress = true;						
		} else {
			return true;
		}
	}


	if (_piecePromotionConversionInProgress || _piecePromotionReversionInProgress) {
		managePromotionConversionAndReversion();
		return true;
	}	
	
	bool selectorChangedPosition = false;
	// reset selector timer when it changes position (even if it isn't inside the board squares)
	if (selectorBoardPosition.x() != _lastSelectorBoardPosition.x() || selectorBoardPosition.y() != _lastSelectorBoardPosition.y()) {
		// selector in new board position
		clearHighlights();
		_selectorTimer->reset();

		_lastSelectorBoardPosition.x() = selectorBoardPosition.x();
		_lastSelectorBoardPosition.y() = selectorBoardPosition.y();

		selectorChangedPosition = true;
	}

	AuxiliarySelector positionIsAnAuxiliarySelector = isPositionAnAuxiliarySelector(selectorBoardPosition);
	if ((isPositionValid(selectorBoardPosition.x()) && isPositionValid(selectorBoardPosition.y())) || (positionIsAnAuxiliarySelector != SELECTOR_INVALID)) {
		if (selectorChangedPosition) {
			return hightLighPosition(selectorBoardPosition.x(), selectorBoardPosition.y(), positionIsAnAuxiliarySelector);
		} else {
			if (_animationInProgress) {
				if (_animationDelayBetweenMoves->elapsedTime() < PIECE_MOVE_MAX_ANIMATION_DURATION_SECONDS) {
					return false;
				}
				else {
					_animationInProgress = false;
					clearSelections();

					if (_currentPlayer == WHITE) {
						updatePlayerStatus(_whitePlayerGameTimerText, _blackPlayerGameTimerText);
					} else {
						updatePlayerStatus(_blackPlayerGameTimerText, _whitePlayerGameTimerText);
					}
				}
			}

			if (_selectorTimer->elapsedTime_m() > PADDLE_TIME_TO_SELECT_POSITION) {
				_selectorTimer->reset();
				
				if (positionIsAnAuxiliarySelector != SELECTOR_INVALID || _piecePromotionInProgress) {
					processAuxiliarySelector(positionIsAnAuxiliarySelector);
					return true;
				}

				// first selection (0 is an invalid position)
				if (_moveOriginPosition.x() == 0) {	
					return processFirstSelection(selectorBoardPosition, positionIsAnAuxiliarySelector);
				} else {				
					return processSecondSelection(selectorBoardPosition, positionIsAnAuxiliarySelector);
				}				
			}
		}
	} else {
		// selector outside board bounds
		clearHighlights();
	}
	
	return false;
}


void ChessBoard::updatePlayersGameTimers(bool forceUpdate) {	
	if (_currentPlayer == WHITE || forceUpdate) {		
		_whitePlayerGameTimerText->setText(ChessUtils::formatSecondsToDate(_whitePlayerGameTimerD + _whitePlayerGameTimer->elapsedTime()));
	}

	if (_currentPlayer == BLACK || forceUpdate) {		
		_blackPlayerGameTimerText->setText(ChessUtils::formatSecondsToDate(_blackPlayerGameTimerD + _blackPlayerGameTimer->elapsedTime()));
	}
}


void ChessBoard::processAuxiliarySelector(AuxiliarySelector auxiliarySelector) {
	if (auxiliarySelector == SELECTOR_NEW_GAME_H_H_WHITE_SIDE || auxiliarySelector == SELECTOR_NEW_GAME_H_H_BLACK_SIDE) {
		resetBoard();
		_animationInProgress = true;
	}

	if (auxiliarySelector == SELECTOR_PREVIOUS_MOVE_WHITE_SIDE || auxiliarySelector == SELECTOR_PREVIOUS_MOVE_BLACK_SIDE) {
		if (goToPreviousMoveInHistory()) {
			_animationDelayBetweenMoves->reset();
			_animationInProgress = true;
		}
	}

	if (auxiliarySelector == SELECTOR_NEXT_MOVE_WHITE_SIDE || auxiliarySelector == SELECTOR_NEXT_MOVE_BLACK_SIDE) {
		if (goToNextMoveInHistory()) {
			_animationDelayBetweenMoves->reset();
			_animationInProgress = true;
		}
	}


	if (_piecePromotionInProgress) {		
		if (auxiliarySelector == SELECTOR_PROMOTE_TO_QUEEN_WHITE_SIDE || auxiliarySelector == SELECTOR_PROMOTE_TO_QUEEN_BLACK_SIDE) {
			_rankOfPromotion = QUEEN;
			_piecePromotionConversionInProgress = true;
		} else if (auxiliarySelector == SELECTOR_PROMOTE_TO_ROOK_WHITE_SIDE || auxiliarySelector == SELECTOR_PROMOTE_TO_ROOK_BLACK_SIDE) {
			_rankOfPromotion = ROOK;
			_piecePromotionConversionInProgress = true;
		} else if (auxiliarySelector == SELECTOR_PROMOTE_TO_KNIGHT_WHITE_SIDE || auxiliarySelector == SELECTOR_PROMOTE_TO_KNIGHT_BLACK_SIDE) {
			_rankOfPromotion = KNIGHT;
			_piecePromotionConversionInProgress = true;
		} else if (auxiliarySelector == SELECTOR_PROMOTE_TO_BISHOP_WHITE_SIDE || auxiliarySelector == SELECTOR_PROMOTE_TO_BISHOP_BLACK_SIDE) {
			_rankOfPromotion = BISHOP;
			_piecePromotionConversionInProgress = true;
		}

		if (_piecePromotionConversionInProgress) {
			_promotionConversionTimer->finish();
			_pieceToPromote->shrinkPiece();

			for (size_t i = 0; i < _promotionChessPieces.size(); ++i) {
				_promotionChessPieces[i]->shrinkPiece();				
			}
		}
	}
}


bool ChessBoard::processFirstSelection(Vec2i selectorBoardPosition, AuxiliarySelector auxiliarySelector) {
	_pieceToMove = selectPosition(selectorBoardPosition.x(), selectorBoardPosition.y(), _currentPlayer, true, auxiliarySelector);

	// mark origin position if a valid piece was selected
	if (_pieceToMove != NULL) {
		_moveOriginPosition.x() = selectorBoardPosition.x();
		_moveOriginPosition.y() = selectorBoardPosition.y();
		clearHighlights();
		_selectorTimer->reset();
		return true;
	} else {
		return false;
	}
}


bool ChessBoard::processSecondSelection(Vec2i selectorBoardPosition, AuxiliarySelector auxiliarySelector) {
	MovePositionStatus movePositionStatus = isPositionAvailableToReceiveMove(selectorBoardPosition.x(), selectorBoardPosition.y(), _currentPlayer);

	// deselect position
	if (movePositionStatus == SAME_POSITION_AS_ORIGIN) {
		_moveOriginPosition.x() = 0;
		_moveOriginPosition.y() = 0;
		clearSelections();
		hightLighPosition(selectorBoardPosition.x(), selectorBoardPosition.y(), auxiliarySelector);
		return false;
	}

	// invalid selection
	if (movePositionStatus == POSITION_WITH_PLAYER_PIECE) {
		return false;
	}

	// second selection (if not defined yet)
	if (_moveDestinationPosition.x() == 0 && (movePositionStatus == POSITION_AVAILABLE || movePositionStatus == POSITION_WITH_OPPONENT_PIECE)) {
		_moveDestinationPosition.x() = selectorBoardPosition.x();
		_moveDestinationPosition.y() = selectorBoardPosition.y();		

		// remove opponent piece if necessary
		ChessPiece* pieceRemoved = NULL;
		if (movePositionStatus == POSITION_WITH_OPPONENT_PIECE) {
			pieceRemoved = removeChessPieceWithAnimation(_moveDestinationPosition, _currentPlayer);						
		}

		_pieceMovesHistoryBackwardsStack.push_back(new ChessMoveHistory(_pieceToMove, _moveOriginPosition, _moveDestinationPosition,
			_pieceToMove->hasPieceMovedPreviously(),
			_whitePlayerGameTimerD, _whitePlayerGameTimerD + _whitePlayerGameTimer->elapsedTime(),
			_blackPlayerGameTimerD, _blackPlayerGameTimerD + _blackPlayerGameTimer->elapsedTime(),
			pieceRemoved));

		// animate piece movement to final destination
		clearHighlights();
		clearSelections();
		hightLighPosition(selectorBoardPosition.x(), selectorBoardPosition.y(), auxiliarySelector);
		//selectPosition(selectorBoardPosition.x(), selectorBoardPosition.y(), _currentPlayer);
		moveChessPieceWithAnimation(_pieceToMove, _moveDestinationPosition);		
		
		_animationDelayBetweenMoves->reset();
		_animationInProgress = true;		

		return true;
	}

	return false;
}


void ChessBoard::updatePlayerStatus(Text3D* makeActive, Text3D* makeInactive) {
	makeActive->setColor(PLAYER_STATUS_TEXT_ACTIVE_COLOR);
	makeInactive->setColor(PLAYER_STATUS_TEXT_INACTIVE_COLOR);
}


void ChessBoard::clearPlayersPieces() {
	_playersPieces->removeChildren(0, _playersPieces->getNumChildren());
}


void ChessBoard::clearHighlights() {
	_boardSquareSelectorHighlights->removeChildren(0, _boardSquareSelectorHighlights->getNumChildren());
}

void ChessBoard::clearSelections() {
	_boardSquareSelections->removeChildren(0, _boardSquareSelections->getNumChildren());
}


void ChessBoard::clearPossibleMoves() {
	_boardSquarePossibleMoves->removeChildren(0, _boardSquarePossibleMoves->getNumChildren());
}


void ChessBoard::clearPromotionPieces() {
	_promotionPieces->removeChildren(0, _promotionPieces->getNumChildren());
}


bool ChessBoard::hightLighPosition(int xBoardPosition, int yBoardPosition, AuxiliarySelector auxiliarySelector) {
	if (auxiliarySelector != SELECTOR_INVALID) {
		// new game h vs h selectors
		float halfBoardSquareSize = BOARD_SQUARE_SIZE / 2.0;

		switch (auxiliarySelector) {
			case SELECTOR_NEW_GAME_H_H_WHITE_SIDE: {
				_boardSquareSelectorHighlights->addChild(setupAuxiliarySelector(Vec2i(5, -4), _newGameHHSelectedImg,
					Vec3(AUXILIARY_SELECTORS_X_OFFSET + halfBoardSquareSize, AUXILIARY_SELECTORS_Y_OFFSET, AUXILIARY_SELECTORS_HIGHLIGHT_Z_OFFSET)));
				break;
			}

			case SELECTOR_NEW_GAME_H_H_BLACK_SIDE: {
				_boardSquareSelectorHighlights->addChild(setupAuxiliarySelector(Vec2i(-5, 4), _newGameHHSelectedImg,
					Vec3(-AUXILIARY_SELECTORS_X_OFFSET - halfBoardSquareSize, -AUXILIARY_SELECTORS_Y_OFFSET, AUXILIARY_SELECTORS_HIGHLIGHT_Z_OFFSET), osg::PI));
				break;
			}

			case SELECTOR_NEW_GAME_H_C_WHITE_SIDE: {
				_boardSquareSelectorHighlights->addChild(setupAuxiliarySelector(Vec2i(5, -5), _newGameHCSelectedImg,
					Vec3(AUXILIARY_SELECTORS_X_OFFSET, AUXILIARY_SELECTORS_Y_OFFSET, AUXILIARY_SELECTORS_HIGHLIGHT_Z_OFFSET)));
				break;
			}

			case SELECTOR_NEW_GAME_H_C_BLACK_SIDE: {
				_boardSquareSelectorHighlights->addChild(setupAuxiliarySelector(Vec2i(-5, 5), _newGameHCSelectedImg,
					Vec3(-AUXILIARY_SELECTORS_X_OFFSET, -AUXILIARY_SELECTORS_Y_OFFSET, AUXILIARY_SELECTORS_HIGHLIGHT_Z_OFFSET), osg::PI));
				break;
			}

			case SELECTOR_NEW_GAME_C_C_WHITE_SIDE: {
				_boardSquareSelectorHighlights->addChild(setupAuxiliarySelector(Vec2i(6, -5), _newGameCCSelectedImg,
					Vec3(AUXILIARY_SELECTORS_X_OFFSET, AUXILIARY_SELECTORS_Y_OFFSET, AUXILIARY_SELECTORS_HIGHLIGHT_Z_OFFSET)));
				break;
			}

			case SELECTOR_NEW_GAME_C_C_BLACK_SIDE: {
				_boardSquareSelectorHighlights->addChild(setupAuxiliarySelector(Vec2i(-6, 5), _newGameCCSelectedImg,
					Vec3(-AUXILIARY_SELECTORS_X_OFFSET, -AUXILIARY_SELECTORS_Y_OFFSET, AUXILIARY_SELECTORS_HIGHLIGHT_Z_OFFSET), osg::PI));
				break;
			}

			case SELECTOR_PREVIOUS_MOVE_WHITE_SIDE: {
				if (!_pieceMovesHistoryBackwardsStack.empty()) {
					_boardSquareSelectorHighlights->addChild(setupAuxiliarySelector(Vec2i(-6, -4), _previousMoveSelectedImg,
						Vec3(-AUXILIARY_SELECTORS_X_OFFSET, -AUXILIARY_SELECTORS_Y_OFFSET, AUXILIARY_SELECTORS_HIGHLIGHT_Z_OFFSET), 0.0,
						Vec3(HISTORY_IMGS_SCALE, HISTORY_IMGS_SCALE, HISTORY_IMGS_SCALE)));
				}
				break;
			}

			case SELECTOR_PREVIOUS_MOVE_BLACK_SIDE: {
				if (!_pieceMovesHistoryBackwardsStack.empty()) {
					_boardSquareSelectorHighlights->addChild(setupAuxiliarySelector(Vec2i(6, 4), _previousMoveSelectedImg,
						Vec3(AUXILIARY_SELECTORS_X_OFFSET, AUXILIARY_SELECTORS_Y_OFFSET, AUXILIARY_SELECTORS_HIGHLIGHT_Z_OFFSET), osg::PI,
						Vec3(HISTORY_IMGS_SCALE, HISTORY_IMGS_SCALE, HISTORY_IMGS_SCALE)));
				}
				break;
			}

			case SELECTOR_NEXT_MOVE_WHITE_SIDE: {
				if (!_pieceMovesHistoryFowardStack.empty()) {
					_boardSquareSelectorHighlights->addChild(setupAuxiliarySelector(Vec2i(-5, -4), _nextMoveSelectedImg,
						Vec3(-AUXILIARY_SELECTORS_X_OFFSET, -AUXILIARY_SELECTORS_Y_OFFSET, AUXILIARY_SELECTORS_HIGHLIGHT_Z_OFFSET), 0.0,
						Vec3(HISTORY_IMGS_SCALE, HISTORY_IMGS_SCALE, HISTORY_IMGS_SCALE)));
				}
				break;
			}

			case SELECTOR_NEXT_MOVE_BLACK_SIDE: {
				if (!_pieceMovesHistoryFowardStack.empty()) {
					_boardSquareSelectorHighlights->addChild(setupAuxiliarySelector(Vec2i(5, 4), _nextMoveSelectedImg,
						Vec3(AUXILIARY_SELECTORS_X_OFFSET, AUXILIARY_SELECTORS_Y_OFFSET, AUXILIARY_SELECTORS_HIGHLIGHT_Z_OFFSET), osg::PI,
						Vec3(HISTORY_IMGS_SCALE, HISTORY_IMGS_SCALE, HISTORY_IMGS_SCALE)));
				}
				break;
			}

			case SELECTOR_INVALID: {
				break;
			}

			// promotion selectors
			default: {
				Vec3f scenePosition = ChessUtils::computePieceScenePosition(xBoardPosition, yBoardPosition);
				scenePosition.y() += (_currentPlayer == WHITE ? PROMOTION_PIECES_BOARD_OFFSET : -PROMOTION_PIECES_BOARD_OFFSET);
				scenePosition.z() = BOARD_HIGHLIGHTS_HEIGHT_OFFSET;
				_boardSquareSelectorHighlights->addChild(ChessUtils::createRectangleWithTexture(scenePosition, new Image(*_paddleSelectorImage)));
				break;
			}
		}
		

		return true;
	}

	if (isPositionValid(xBoardPosition) && isPositionValid(yBoardPosition)) {
		Vec3f scenePosition = ChessUtils::computePieceScenePosition(xBoardPosition, yBoardPosition);
		scenePosition.z() = BOARD_HIGHLIGHTS_HEIGHT_OFFSET;
		_boardSquareSelectorHighlights->addChild(ChessUtils::createRectangleWithTexture(scenePosition, new Image(*_paddleSelectorImage)));
		return true;
	} else {
		return false;
	}
}


ChessPiece* ChessBoard::selectPosition(int xBoardPosition, int yBoardPosition, ChessPieceColor chessPieceColor, bool selectOnlyIfExistsPiece, AuxiliarySelector auxiliarySelector) {
	if ((!isPositionValid(xBoardPosition) || !isPositionValid(yBoardPosition)) && auxiliarySelector == SELECTOR_INVALID) {
		return NULL;
	}

	ChessPiece* chessPieceAtPosition = getChessPieceAtBoardPosition(xBoardPosition, yBoardPosition, chessPieceColor);
	if (!selectOnlyIfExistsPiece || (selectOnlyIfExistsPiece && chessPieceAtPosition != NULL)) {
		Vec3f scenePosition = ChessUtils::computePieceScenePosition(xBoardPosition, yBoardPosition);
		scenePosition.z() = BOARD_SELECTIONS_HEIGHT_OFFSET;

		MatrixTransform* selectionMT = new MatrixTransform();
		selectionMT->addChild(ChessUtils::createRectangleWithTexture(Vec3(0,0,0), new Image(*_paddleSelectedImage)));

		if (chessPieceColor == BLACK) {
			selectionMT->postMult(Matrix::rotate(osg::PI, osg::Z_AXIS));
		}

		selectionMT->postMult(Matrix::translate(scenePosition));
		_boardSquareSelections->addChild(selectionMT);
	}

	return chessPieceAtPosition;

}


bool ChessBoard::showPossibleMoves(ChessPiece* chessPiece) {

	return false;
}


Vec2Array* ChessBoard::computePossibleMovePositions(ChessPiece* chessPiece) {
	int xBoardPosition = chessPiece->getXPosition();
	int yBoardPosition = chessPiece->getYPosition();
	ChessPieceType chessPieceType = chessPiece->getChessPieceType();
	ChessPieceColor chessPieceColor = chessPiece->getChessPieceColor();
	ChessPieceColor opponentChessPieceColor = ChessPiece::getOpponentChessPieceColor(chessPieceColor);

	Vec2Array* possibleMoves = new Vec2Array();

	switch (chessPieceType) {		
		case KING: {
			updatePossibleMoves(xBoardPosition - 1, yBoardPosition,		chessPieceColor, possibleMoves);
			updatePossibleMoves(xBoardPosition - 1, yBoardPosition - 1, chessPieceColor, possibleMoves);
			updatePossibleMoves(xBoardPosition,		yBoardPosition + 1, chessPieceColor, possibleMoves);
			updatePossibleMoves(xBoardPosition + 1, yBoardPosition + 1,	chessPieceColor, possibleMoves);
			updatePossibleMoves(xBoardPosition + 1, yBoardPosition,		chessPieceColor, possibleMoves);
			updatePossibleMoves(xBoardPosition + 1, yBoardPosition - 1,	chessPieceColor, possibleMoves);
			updatePossibleMoves(xBoardPosition,		yBoardPosition - 1,	chessPieceColor, possibleMoves);
			updatePossibleMoves(xBoardPosition - 1, yBoardPosition - 1,	chessPieceColor, possibleMoves);
			break;
		}

		case QUEEN: {
			break;
		}

		case ROOK: {
			break;
		}

		case KNIGHT: {
			break;
		}

		case BISHOP: {
			break;
		}

		case PAWN: {
			break;
		}

		default:
			break;
	}


	return possibleMoves;
}


bool ChessBoard::updatePossibleMoves(int xBoardPosition, int yBoardPosition, ChessPieceColor chessPieceColor, Vec2Array* possibleMoves) {
	if (isPositionAvailableToReceiveMove(xBoardPosition, yBoardPosition, chessPieceColor)) {
		possibleMoves->push_back(Vec2(yBoardPosition, yBoardPosition));
		return true;
	}

	return false;
}


MovePositionStatus ChessBoard::isPositionAvailableToReceiveMove(int xBoardPosition, int yBoardPosition, ChessPieceColor currentPlayer) {
	if (xBoardPosition != _moveOriginPosition.x() || yBoardPosition != _moveOriginPosition.y()) {		
		if (getChessPieceAtBoardPosition(xBoardPosition, yBoardPosition, currentPlayer) != NULL) {
			return POSITION_WITH_PLAYER_PIECE;
		}

		if (getChessPieceAtBoardPosition(xBoardPosition, yBoardPosition, ChessPiece::getOpponentChessPieceColor(currentPlayer)) != NULL) {
			return POSITION_WITH_OPPONENT_PIECE;
		}

		return POSITION_AVAILABLE;
	} else {
		return SAME_POSITION_AS_ORIGIN;
	}
}



void ChessBoard::moveChessPieceWithAnimation(ChessPiece* chessPieceToMove, Vec2i finalPosition) {
	// reset positions to next piece move
	_moveOriginPosition.x() = 0;
	_moveOriginPosition.y() = 0;
	_moveDestinationPosition.x() = 0;
	_moveDestinationPosition.y() = 0;
		
	if (!checkAndPerformCastling(chessPieceToMove, finalPosition)) {
		chessPieceToMove->changePosition(finalPosition.x(), finalPosition.y());
	} else {
		_pieceMovesHistoryBackwardsStack.back()->setPerformedCastling(true);
	}	

	if (!checkAndPerformPromotion(chessPieceToMove, finalPosition)) {
		switchPlayer();
	}

	_pieceMovesHistoryFowardStack.clear();
	updateHistoryManipulatorsVisibility();
}

ChessPiece* ChessBoard::removeChessPieceWithAnimation(Vec2i boardPositionOfPieceToRemove, ChessPieceColor currentPlayerChessPieceColor) {
	ChessPiece* pieceToRemove = getChessPieceAtBoardPosition(boardPositionOfPieceToRemove.x(), boardPositionOfPieceToRemove.y(), ChessPiece::getOpponentChessPieceColor(currentPlayerChessPieceColor));
	pieceToRemove->removePieceFromBoard();

	return pieceToRemove;
}


bool ChessBoard::checkAndPerformCastling(ChessPiece* king, Vec2i finalPosition) {
	if (isCastlingPossible(king, finalPosition)) {
		ChessPiece* rook = getChessPieceAtBoardPosition((finalPosition.x() == 3 ? 4 : -4), king->getYPosition(), king->getChessPieceColor());

		// move king
		king->changePosition(finalPosition.x(), finalPosition.y());

		// move rook
		rook->changePosition((finalPosition.x() == 3 ? 2 : -1), finalPosition.y());
		return true;
	}

	return false;
}


bool ChessBoard::isCastlingPossible(ChessPiece* king, Vec2i kingFinalPosition) {
	// castling can only be done by the king and a rook if none of then has moved, and there is no pieces between then
	if (king->getChessPieceType() == KING && !king->hasPieceMovedPreviously()) {				
		if (kingFinalPosition.x() == 3) {
			// check if there are pieces blocking the castling
			if (getChessPieceAtBoardPosition(2, king->getYPosition(), WHITE) != NULL || getChessPieceAtBoardPosition(2, king->getYPosition(), BLACK) != NULL ||
				getChessPieceAtBoardPosition(3, king->getYPosition(), WHITE) != NULL || getChessPieceAtBoardPosition(3, king->getYPosition(), BLACK) != NULL) {
				return false;
			}

			// check if the rook hasn't move
			ChessPiece* rookRightSide = getChessPieceAtBoardPosition(4, king->getYPosition(), king->getChessPieceColor());
			if (rookRightSide == NULL || rookRightSide->hasPieceMovedPreviously()) {
				return false;
			}
				
			// TODO check if any position from the king to the final position is attacked
			
			return true;
		} else if (kingFinalPosition.x() == -2) {
			// check if there are pieces blocking the castling
			if (getChessPieceAtBoardPosition(-1, king->getYPosition(), WHITE) != NULL || getChessPieceAtBoardPosition(-1, king->getYPosition(), BLACK) != NULL ||
				getChessPieceAtBoardPosition(-2, king->getYPosition(), WHITE) != NULL || getChessPieceAtBoardPosition(-2, king->getYPosition(), BLACK) != NULL ||
				getChessPieceAtBoardPosition(-3, king->getYPosition(), WHITE) != NULL || getChessPieceAtBoardPosition(-3, king->getYPosition(), BLACK) != NULL) {
				return false;
			}

			// check if the rook hasn't move
			ChessPiece* rookRightSide = getChessPieceAtBoardPosition(-4, king->getYPosition(), king->getChessPieceColor());
			if (rookRightSide == NULL || rookRightSide->hasPieceMovedPreviously()) {
				return false;
			}

			// TODO check if any position from the king to the final position is attacked	

			return true;
		}
	}

	return false;
}


bool ChessBoard::checkAndPerformPromotion(ChessPiece* chessPieceMoved, Vec2i finalPosition) {
	if (chessPieceMoved->getChessPieceType() == PAWN) {
		if ((chessPieceMoved->getChessPieceColor() == WHITE && chessPieceMoved->getYPosition() == 4) ||
			(chessPieceMoved->getChessPieceColor() == BLACK && chessPieceMoved->getYPosition() == -4)) {
			_pieceToPromote = chessPieceMoved;
			setupPromotionPiecesOnBoad(chessPieceMoved->getChessPieceColor());
			return true;
		}
	}

	return false;
}


void ChessBoard::setupPromotionPiecesOnBoad(ChessPieceColor chessPieceColor) {
	_promotionChessPieces.clear();
	_piecePromotionInProgress = true;
	
	Vec3 scaleFactor = Vec3(0.0, 0.0, 0.0);
	Vec3 translateOffset = Vec3(0.0, (chessPieceColor == WHITE ? PROMOTION_PIECES_BOARD_OFFSET : -PROMOTION_PIECES_BOARD_OFFSET), 0.0);

	_promotionChessPieces.push_back(new ChessPiece(QUEEN, chessPieceColor, (chessPieceColor == WHITE ? -2 : 2), (chessPieceColor == WHITE ? 5 : -5), _whitePiecesMaterial, scaleFactor, translateOffset));
	_promotionChessPieces.push_back(new ChessPiece(ROOK, chessPieceColor, (chessPieceColor == WHITE ? -1 : 1), (chessPieceColor == WHITE ? 5 : -5), _whitePiecesMaterial, scaleFactor, translateOffset));
	_promotionChessPieces.push_back(new ChessPiece(KNIGHT, chessPieceColor, (chessPieceColor == WHITE ? 1 : -1), (chessPieceColor == WHITE ? 5 : -5), _whitePiecesMaterial, scaleFactor, translateOffset));
	_promotionChessPieces.push_back(new ChessPiece(BISHOP, chessPieceColor, (chessPieceColor == WHITE ? 2 : -2), (chessPieceColor == WHITE ? 5 : -5), _whitePiecesMaterial, scaleFactor, translateOffset));


	for (size_t i = 0; i < _promotionChessPieces.size(); ++i) {
		_promotionChessPieces[i]->unshrinkPiece();
		_promotionPieces->addChild(_promotionChessPieces[i]->getPieceMatrixTransform());
	}
}


void ChessBoard::managePromotionConversionAndReversion() {	
	if (_promotionConversionTimer->elapsedTime() > PROMOTION_SCALE_ANIMATION_DURATION_SECONDS) {
		// after original piece completes shrinking, replace the model
		if (_pieceToPromote->getChessPieceType() != _rankOfPromotion) {
			clearPromotionPieces();

			if (!_piecePromotionReversionInProgress && !_piecePromotionConversionFromHistoryInProgress) {
				_pieceMovesHistoryBackwardsStack.back()->setPerformedPromotion(true);
				_pieceMovesHistoryBackwardsStack.back()->setOriginalPieceType(_pieceToPromote->getChessPieceType());
				_pieceMovesHistoryBackwardsStack.back()->setPromotionPieceType(_rankOfPromotion);
			}

			_pieceToPromote->loadPieceModel(_rankOfPromotion, Vec3(0.0, 0.0, 0.0));
			_pieceToPromote->unshrinkPiece();
		}
	}

	// piece promotion done
	if (_promotionConversionTimer->elapsedTime() > 2.0 * PROMOTION_SCALE_ANIMATION_DURATION_SECONDS) {
		_selectorTimer->finish();
		_piecePromotionInProgress = false;
		_piecePromotionConversionInProgress = false;
		_piecePromotionConversionFromHistoryInProgress = false;

		if (_piecePromotionReversionInProgress) {
			ChessMoveHistory* moveBackInfo = _pieceMovesHistoryFowardStack.back();
			
			// move piece to original position
			moveBackInfo->moveBackInHistory();
			_piecePromotionReversionInProgress = false;
		}		

		switchPlayer();
	}
}


void ChessBoard::removePromotionPiecesOnBoad() {
	for (size_t i = 0; i < _promotionChessPieces.size(); ++i) {
		_promotionChessPieces[i]->shrinkPiece();	
	}
}


bool ChessBoard::goToPreviousMoveInHistory() {
	if (!_pieceMovesHistoryBackwardsStack.empty()) {
		ChessMoveHistory* moveBackInfo = _pieceMovesHistoryBackwardsStack.back();	
		
		// move rook to original position
		if (moveBackInfo->hasPerformedCastling()) {
			ChessPiece* king = moveBackInfo->getPieceMoved();
			ChessPiece* rook = getChessPieceAtBoardPosition((king->getXPosition() == 3 ? 2 : -1), king->getYPosition(), king->getChessPieceColor());

			if (rook != NULL) {
				rook->changePosition((king->getXPosition() == 3 ? 4 : -4), king->getYPosition());
			}
		}

		_pieceMovesHistoryFowardStack.push_back(moveBackInfo);
		_pieceMovesHistoryBackwardsStack.pop_back();

		if (moveBackInfo->hasPerformedPromotion()) {			
			_pieceToPromote = moveBackInfo->getPieceMoved();
			_rankOfPromotion = moveBackInfo->getOriginalPieceType();

			_pieceToPromote->shrinkPiece();
			_promotionConversionTimer->finish();
			_piecePromotionReversionInProgress = true;
		} else {
			// move king to original position
			moveBackInfo->moveBackInHistory();
			switchPlayer();
		}				
		
		_whitePlayerGameTimerD = moveBackInfo->getWhitePlayerGameTimerD();
		_blackPlayerGameTimerD = moveBackInfo->getBlackPlayerGameTimerD();
		_whitePlayerGameTimer->finish();
		_blackPlayerGameTimer->finish();
		updatePlayersGameTimers(true);

		updateHistoryManipulatorsVisibility();		

		if (_pieceMovesHistoryBackwardsStack.empty()) {
			clearHighlights();
		}

		return true;
	}

	return false;	
}


bool ChessBoard::goToNextMoveInHistory() {
	if (!_pieceMovesHistoryFowardStack.empty()) {
		ChessMoveHistory* moveFowardInfo = _pieceMovesHistoryFowardStack.back();				

		// move rook to destination position
		if (moveFowardInfo->hasPerformedCastling()) {
			ChessPiece* king = moveFowardInfo->getPieceMoved();
			ChessPiece* rook = getChessPieceAtBoardPosition((moveFowardInfo->getPieceMovedDestinationPosition().x() == 3 ? 4 : -4), king->getYPosition(), king->getChessPieceColor());

			if (rook != NULL) {
				rook->changePosition((moveFowardInfo->getPieceMovedDestinationPosition().x() == 3 ? 2 : -1), king->getYPosition());
			}
		}

		// move king to destination position
		moveFowardInfo->moveFowardInHistory();

		_pieceMovesHistoryBackwardsStack.push_back(moveFowardInfo);
		_pieceMovesHistoryFowardStack.pop_back();

		if (moveFowardInfo->hasPerformedPromotion()) {			
			_pieceToPromote = moveFowardInfo->getPieceMoved();
			_rankOfPromotion = moveFowardInfo->getPromotionPieceType();
			_animationDelayBetweenMoves->finish();

			Vec2i pieceMovedOriginPosition = moveFowardInfo->getPieceMovedOriginPosition();
			Vec2i pieceMovedDestinationPosition = moveFowardInfo->getPieceMovedDestinationPosition();

			Vec3f initialPieceScenePosition = ChessUtils::computePieceScenePosition(pieceMovedOriginPosition.x(), pieceMovedOriginPosition.y());
			Vec3f finalPieceScenePosition = ChessUtils::computePieceScenePosition(pieceMovedDestinationPosition.x(), pieceMovedDestinationPosition.y());

			Vec3 positionsOffset = finalPieceScenePosition - initialPieceScenePosition;
			float positionsDistance = positionsOffset.length();
			float pieceTravelSpeed = PIECE_MOVE_ANIMATION_TRAVEL_SPEED;
			_timeToWaitForPromotingPieceFromHistory = std::max(positionsDistance / pieceTravelSpeed, 2.0f);

			_piecePromotionConversionInProgress = false;
			_piecePromotionConversionFromHistoryInProgress = true;
		} else {			
			switchPlayer();
		}

		_whitePlayerGameTimerD = moveFowardInfo->getWhitePlayerGameTimerDEndMove();
		_blackPlayerGameTimerD = moveFowardInfo->getBlackPlayerGameTimerDEndMove();
		_whitePlayerGameTimer->finish();
		_blackPlayerGameTimer->finish();
		updatePlayersGameTimers(true);

		updateHistoryManipulatorsVisibility();
		if (_pieceMovesHistoryFowardStack.empty()) {
			clearHighlights();
		}

		return true;
	}

	return false;
}


void ChessBoard::updateHistoryManipulatorsVisibility() {	
	Vec4Array* activeColors = new Vec4Array();
	activeColors->push_back(Vec4(1.0f, 1.0f, 1.0f, 0.8f));

	Vec4Array* inactiveColors = new Vec4Array();
	inactiveColors->push_back(Vec4(0.5, 0.5, 0.5, 0.8));

	if (_pieceMovesHistoryBackwardsStack.empty()) {		
		_moveBackwardsInHistorySelectorWhiteSide->setColorArray(inactiveColors);
		_moveBackwardsInHistorySelectorBlackSide->setColorArray(inactiveColors);
		_moveBackwardsInHistorySelectorWhiteSide->setColorBinding(osg::Geometry::BIND_OVERALL);
		_moveBackwardsInHistorySelectorBlackSide->setColorBinding(osg::Geometry::BIND_OVERALL);
	} else {		
		_moveBackwardsInHistorySelectorWhiteSide->setColorArray(activeColors);
		_moveBackwardsInHistorySelectorBlackSide->setColorArray(activeColors);
		_moveBackwardsInHistorySelectorWhiteSide->setColorBinding(osg::Geometry::BIND_OVERALL);
		_moveBackwardsInHistorySelectorBlackSide->setColorBinding(osg::Geometry::BIND_OVERALL);
	}

	if (_pieceMovesHistoryFowardStack.empty()) {
		_moveFowardInHistorySelectorWhiteSide->setColorArray(inactiveColors);
		_moveFowardInHistorySelectorBlackSide->setColorArray(inactiveColors);
		_moveFowardInHistorySelectorWhiteSide->setColorBinding(osg::Geometry::BIND_OVERALL);
		_moveFowardInHistorySelectorBlackSide->setColorBinding(osg::Geometry::BIND_OVERALL);


	} else {
		_moveFowardInHistorySelectorWhiteSide->setColorArray(activeColors);
		_moveFowardInHistorySelectorBlackSide->setColorArray(activeColors);
		_moveFowardInHistorySelectorWhiteSide->setColorBinding(osg::Geometry::BIND_OVERALL);
		_moveFowardInHistorySelectorBlackSide->setColorBinding(osg::Geometry::BIND_OVERALL);
	}
}


void ChessBoard::switchPlayer() {
	if (_currentPlayer == WHITE) {
		_whitePlayerGameTimerD += _whitePlayerGameTimer->elapsedTime();
	} else {
		_blackPlayerGameTimerD += _blackPlayerGameTimer->elapsedTime();
	}
	
	_currentPlayer = ChessPiece::getOpponentChessPieceColor(_currentPlayer);

	_whitePlayerGameTimer->finish();
	_blackPlayerGameTimer->finish();
	_selectorTimer->finish();

	if (_currentPlayer == WHITE) {
		updatePlayerStatus(_whitePlayerGameTimerText, _blackPlayerGameTimerText);
	} else {
		updatePlayerStatus(_blackPlayerGameTimerText, _whitePlayerGameTimerText);
	}
}


bool ChessBoard::isPositionValid(int position) {
	if ((position > -5 && position < 0) || (position > 0 && position < 5)) {
		return true;
	}
	else {
		return false;
	}
}


AuxiliarySelector ChessBoard::isPositionAnAuxiliarySelector(Vec2i position) {
	// history manipulators
	if (position.x() == -6 && position.y() == -4) {
		return SELECTOR_PREVIOUS_MOVE_WHITE_SIDE;
	}

	if (position.x() == 6 && position.y() == 4) {
		return SELECTOR_PREVIOUS_MOVE_BLACK_SIDE;
	}

	if (position.x() == -5 && position.y() == -4) {
		return SELECTOR_NEXT_MOVE_WHITE_SIDE;
	}

	if (position.x() == 5 && position.y() == 4) {
		return SELECTOR_NEXT_MOVE_BLACK_SIDE;
	}


	// new game manipulators
	if ((position.x() == 5 || position.x() == 6) && position.y() == -4) {
		return SELECTOR_NEW_GAME_H_H_WHITE_SIDE;
	}

	if ((position.x() == -5 || position.x() == -6) && position.y() == 4) {
		return SELECTOR_NEW_GAME_H_H_BLACK_SIDE;
	}

	if (position.x() == 5 && position.y() == -5) {
		return SELECTOR_NEW_GAME_H_C_WHITE_SIDE;
	}

	if (position.x() == -5 && position.y() == 5) {
		return SELECTOR_NEW_GAME_H_C_BLACK_SIDE;
	}

	if (position.x() == 6 && position.y() == -5) {
		return SELECTOR_NEW_GAME_C_C_WHITE_SIDE;
	}

	if (position.x() == -6 && position.y() == 5) {
		return SELECTOR_NEW_GAME_C_C_BLACK_SIDE;
	}


	// piece promotion manipulators
	if (_piecePromotionInProgress) {
		if (_currentPlayer == WHITE) {
			if (position.x() == -2 && position.y() == 5) {
				return SELECTOR_PROMOTE_TO_QUEEN_BLACK_SIDE;
			}			

			if (position.x() == -1 && position.y() == 5) {
				return SELECTOR_PROMOTE_TO_ROOK_BLACK_SIDE;
			}
			
			if (position.x() == 1 && position.y() == 5) {
				return SELECTOR_PROMOTE_TO_KNIGHT_BLACK_SIDE;
			}

			if (position.x() == 2 && position.y() == 5) {
				return SELECTOR_PROMOTE_TO_BISHOP_BLACK_SIDE;
			}
		} else {
			if (position.x() == 2 && position.y() == -5) {
				return SELECTOR_PROMOTE_TO_QUEEN_WHITE_SIDE;
			}

			if (position.x() == 1 && position.y() == -5) {
				return SELECTOR_PROMOTE_TO_ROOK_WHITE_SIDE;
			}

			if (position.x() == -1 && position.y() == -5) {
				return SELECTOR_PROMOTE_TO_KNIGHT_WHITE_SIDE;
			}

			if (position.x() == -2 && position.y() == -5) {
				return SELECTOR_PROMOTE_TO_BISHOP_WHITE_SIDE;
			}
		}		
	}

	return SELECTOR_INVALID;	
}


ChessPiece* ChessBoard::getChessPieceAtBoardPosition(int xBoardPosition, int yBoardPosition, ChessPieceColor chessPieceColor) {
	if (!isPositionValid(xBoardPosition) || !isPositionValid(yBoardPosition)) {
		return NULL;
	}
	
	if (chessPieceColor == WHITE) {
		for (size_t i = 0; i < _whiteChessPieces.size(); ++i) {
			if (_whiteChessPieces[i]->isPiecePlayable() && _whiteChessPieces[i]->getXPosition() == xBoardPosition && _whiteChessPieces[i]->getYPosition() == yBoardPosition) {
				return _whiteChessPieces[i];
			}
		}
	} else {
		for (size_t i = 0; i < _blackChessPieces.size(); ++i) {
			if (_blackChessPieces[i]->isPiecePlayable() && _blackChessPieces[i]->getXPosition() == xBoardPosition && _blackChessPieces[i]->getYPosition() == yBoardPosition) {
				return _blackChessPieces[i];
			}
		}
	}	

	return NULL;
}

