#include "ChessBoard.h"


ChessBoard::ChessBoard() {	
	setupImages();
	setupMaterials();

	_font3D = osgText::readFont3DFile(TEXT_FONT);
	_lastSelectorBoardPosition = Vec2i(0, 0);
		
	_boardReseting = false;
	_movedBackwards = false;

	_whitePlayerIsAI = false;
	_blackPlayerIsAI = false;
	_chessEngineSkillLevel = 20;
	_uciProtocol.startUCIChessEngine(CHESS_ENGINE_FILE_PATH, _chessEngineSkillLevel, CHESS_ENGINE_LOGFILE);


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
	_boardSquarePossibleMoves = new Group();
	_promotionPieces = new Group();
	_chessEngineSkillLevelSelectors = new Group();
	
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
	_boardShadowedScene->addChild(_boardSquarePossibleMoves);
	_boardShadowedScene->addChild(_promotionPieces);
	_boardShadowedScene->addChild(_chessEngineSkillLevelSelectors);

	
	_gameStatusTextWhiteSide = ChessUtils::createText3D("Game Status", _font3D, Vec3(0,0,0), GAME_STATUS_TEXT_SIZE, GAME_STATUS_TEXT_DEPTH);
	_gameStatusTextWhiteSide->setAlignment(osgText::TextBase::CENTER_CENTER);
	_gameStatusTextWhiteSide->setColor(GAME_STATUS_TEXT_COLOR);
	_gameStatusTextWhiteSideMT = wrapText(_gameStatusTextWhiteSide);
	_boardShadowedScene->addChild(wrapAndChangeMTPosition(_gameStatusTextWhiteSideMT, GAME_STATUS_TEXT_POSITION_WHITE_SIDE, Vec3(0, 0, 0), 0.0));

	_gameStatusTextBlackSide = ChessUtils::createText3D("Game Status", _font3D, Vec3(0, 0, 0), GAME_STATUS_TEXT_SIZE, GAME_STATUS_TEXT_DEPTH);
	_gameStatusTextBlackSide->setAlignment(osgText::TextBase::CENTER_CENTER);	
	_gameStatusTextBlackSide->setColor(GAME_STATUS_TEXT_COLOR);
	_gameStatusTextBlackSideMT = wrapText(_gameStatusTextBlackSide);	
	_boardShadowedScene->addChild(wrapAndChangeMTPosition(_gameStatusTextBlackSideMT, GAME_STATUS_TEXT_POSITION_BLACK_SIDE, Vec3(0, 0, 0), osg::PI));

	_gameStatusTextVisible = false;
	_chessEngineSkillLevelSelectionInProgress = false;

	setupAISkillLevelSelectors();
}

ChessBoard::~ChessBoard() {}


void ChessBoard::setupImages() {
	// images loading
	_paddleSelectorImg = osgDB::readImageFile(PADDLE_SELECTOR_IMG);
	_paddleSelectedImg = osgDB::readImageFile(PADDLE_SELECTED_IMG);
	_possibleMoveImg = osgDB::readImageFile(POSSIBLE_MOVE_IMG);
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
	_gameStatus = IN_PROGRESS;
	_currentPlayer = WHITE;
	_pieceToMove = NULL;
	_moveOriginPosition = Vec2i(0, 0);
	_moveDestinationPosition = Vec2i(0, 0);
	_currentPlayNumber = 0;

	_chessEngineThinking = false;

	_chessEngineMoveTimeInMilliseconds = (std::max)(CHESS_ENGINE_SKILL_LEVEL_TIME_ALLOWED_INCREASE_MS, _chessEngineSkillLevel * CHESS_ENGINE_SKILL_LEVEL_TIME_ALLOWED_INCREASE_MS);
	
	_whitePlayerGameTimer->finish();
	_blackPlayerGameTimer->finish();
	_selectorTimer->finish();
	_animationDelayBetweenMoves->finish();
	_promotionConversionTimer->finish();

	_whitePlayerGameTimerD = 0;
	_blackPlayerGameTimerD = 0;

	_animationInProgress = false;
	_piecePromotionInProgress = false;
	_piecePromotionInProgressAborted = false;
	_piecePromotionConversionInProgress = false;
	_piecePromotionConversionFromHistoryInProgress = false;
	_piecePromotionReversionInProgress = false;

	_rankOfPromotion = PAWN;

	_pieceMovesHistoryBackwardsStack.clear();
	_pieceMovesHistoryFowardStack.clear();
	updateHistoryManipulatorsVisibility();	

	updatePlayerStatus(_whitePlayerGameTimerText, _blackPlayerGameTimerText);	

	if (!_whitePlayerIsAI && !_blackPlayerIsAI) {
		setGameStatusText(GAME_STATUS_NEW_GAME_H_H);
	} else if (!_whitePlayerIsAI && _blackPlayerIsAI) {
		setGameStatusText(GAME_STATUS_NEW_GAME_H_C);
	} else if (_whitePlayerIsAI && _blackPlayerIsAI) {
		setGameStatusText(GAME_STATUS_NEW_GAME_C_C);
	}

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
	clearPossibleMoves();
	removePromotionPiecesOnBoad();

	_animationDelayBetweenMoves->reset();
	_whitePlayerGameTimerText->setText(getPlayerGameTimeFormated(0, WHITE));
	_blackPlayerGameTimerText->setText(getPlayerGameTimeFormated(0, BLACK));

	_boardReseting = true;

	return setupBoard();
}


void ChessBoard::setupBoardPieces() {
	//setup initial positions

	// kings
	_pieceKingWhite = new ChessPiece(KING, WHITE, 1, -4, _whitePiecesMaterial);
	_pieceKingBlack = new ChessPiece(KING, BLACK, 1, 4, _blackPiecesMaterial);
	_whiteChessPieces.push_back(_pieceKingWhite);
	_blackChessPieces.push_back(_pieceKingBlack);

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
	_whitePlayerGameTimerText = ChessUtils::createText3D(getPlayerGameTimeFormated(0, WHITE), _font3D, textPosition, PLAYER_STATUS_TEXT_SIZE, PLAYER_STATUS_TEXT_DEPTH);
	_blackPlayerGameTimerText = ChessUtils::createText3D(getPlayerGameTimeFormated(0, BLACK), _font3D, textPosition, PLAYER_STATUS_TEXT_SIZE, PLAYER_STATUS_TEXT_DEPTH);

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
	bool gameInProgress = isGameInProgress();

	if (gameInProgress) {
		updatePlayersGameTimers();

		if (_piecePromotionConversionFromHistoryInProgress && !_piecePromotionConversionInProgress) {
			if (_animationDelayBetweenMoves->elapsedTime() > _timeToWaitForPromotingPieceFromHistory) {
				_pieceToPromote->shrinkPiece();
				_promotionConversionTimer->finish();
				_animationDelayBetweenMoves->finish();
				_piecePromotionConversionInProgress = true;
			}
			else {
				return true;
			}
		}


		if (_piecePromotionConversionInProgress || _piecePromotionReversionInProgress) {
			managePromotionConversionAndReversion();
			return true;
		}
	}

	if (_animationInProgress) {
		if (_animationDelayBetweenMoves->elapsedTime() > PIECE_MOVE_MAX_ANIMATION_DURATION_SECONDS) {
			if (!_piecePromotionInProgress) {
				if (_boardReseting) {
					_animationInProgress = false;
					_boardReseting = false;
					clearPromotionPieces();
				} else {
					if (!_piecePromotionInProgressAborted) {
						switchPlayer();
					}
					else {
						_piecePromotionInProgressAborted = false;
						clearPromotionPieces();
					}


					// go back in history moves again if a human player is against a AI player and it wants to undo last move
					if (isCurrentPlayerAI() && !isOpponentPlayerAI() && _movedBackwards) {
						goToPreviousMoveInHistory();
					} else {
						_animationInProgress = false;
					}

					_movedBackwards = false;
				}
			}
		}
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

	bool currentPlayerIsAI = isCurrentPlayerAI();

	// update user input
	AuxiliarySelector positionIsAnAuxiliarySelector = isPositionAnAuxiliarySelector(selectorBoardPosition);
	if ((isPositionValid(selectorBoardPosition.x()) && isPositionValid(selectorBoardPosition.y())) || (positionIsAnAuxiliarySelector != SELECTOR_INVALID)) {
		if (selectorChangedPosition) {
			hightLighPosition(selectorBoardPosition.x(), selectorBoardPosition.y(), positionIsAnAuxiliarySelector);
		} else {
			if (_selectorTimer->elapsedTime_m() > PADDLE_TIME_TO_SELECT_POSITION) {
				_selectorTimer->reset();
				
				if (_chessEngineSkillLevelSelectionInProgress) {
					return manageAISkillLevelSelection(selectorBoardPosition);
				}

				if (positionIsAnAuxiliarySelector != SELECTOR_INVALID || _piecePromotionInProgress) {
					processAuxiliarySelector(positionIsAnAuxiliarySelector);
					return true;
				}

				if (gameInProgress && !currentPlayerIsAI && !_animationInProgress) {
					// first selection (0 is an invalid position)
					if (_moveOriginPosition.x() == 0) {
						return processFirstSelection(selectorBoardPosition);
					} else {						
						return processSecondSelection(selectorBoardPosition);					
					}
				}
			}
		}
	} else {
		// selector outside board bounds
		clearHighlights();
	}
	

	// update AI
	if (gameInProgress && currentPlayerIsAI && !_animationInProgress && !_piecePromotionInProgress) {
		return makeChessAIMove();
	}

	return false;
}


void ChessBoard::updatePlayersGameTimers(bool forceUpdate) {	
	if (_currentPlayer == WHITE || forceUpdate) {		
		_whitePlayerGameTimerText->setText(getPlayerGameTimeFormated(_whitePlayerGameTimerD + _whitePlayerGameTimer->elapsedTime(), WHITE));
	}

	if (_currentPlayer == BLACK || forceUpdate) {		
		_blackPlayerGameTimerText->setText(getPlayerGameTimeFormated(_blackPlayerGameTimerD + _blackPlayerGameTimer->elapsedTime(), BLACK));
	}
}


void ChessBoard::updatePlayersCumulativeGameTimers() {
	if (_currentPlayer == WHITE) {
		_whitePlayerGameTimerD += _whitePlayerGameTimer->elapsedTime();
	}
	else {
		_blackPlayerGameTimerD += _blackPlayerGameTimer->elapsedTime();
	}

	_whitePlayerGameTimer->finish();
	_blackPlayerGameTimer->finish();	
}


string ChessBoard::getPlayerGameTimeFormated(double timeInSeconds, ChessPieceColor playerColor) {	
	bool playerIsAI = false;
	if ((playerColor == WHITE && _whitePlayerIsAI) || (playerColor == BLACK && _blackPlayerIsAI)) {
		playerIsAI = true;
	}	

	string playerTag;
	if (playerIsAI) {
		playerTag = "C: ";
	} else {
		playerTag = "H: ";
	}

	string timeFormated = ChessUtils::formatSecondsToDate(timeInSeconds);

	return playerTag + timeFormated;
}


void ChessBoard::processAuxiliarySelector(AuxiliarySelector auxiliarySelector) {
	if (auxiliarySelector == SELECTOR_NEW_GAME_H_H_WHITE_SIDE || auxiliarySelector == SELECTOR_NEW_GAME_H_H_BLACK_SIDE) {
		_whitePlayerIsAI = false;
		_blackPlayerIsAI = false;
		resetBoard();
		_animationInProgress = true;
	} else if (auxiliarySelector == SELECTOR_NEW_GAME_H_C_WHITE_SIDE || auxiliarySelector == SELECTOR_NEW_GAME_H_C_BLACK_SIDE) {				
		showAISkillLevelSelectors();
		_chessEngineSkillLevelSelectionInProgress = true;
		_whitePlayerIsAI = false;
		_blackPlayerIsAI = true;		
		resetBoard();		
		_gameStatus = DRAW;
		_animationInProgress = true;
	} else if (auxiliarySelector == SELECTOR_NEW_GAME_C_C_WHITE_SIDE || auxiliarySelector == SELECTOR_NEW_GAME_C_C_BLACK_SIDE) {				
		showAISkillLevelSelectors();
		_chessEngineSkillLevelSelectionInProgress = true;
		_whitePlayerIsAI = true;
		_blackPlayerIsAI = true;
		resetBoard();
		_gameStatus = DRAW;
		_animationInProgress = true;
	} else if (auxiliarySelector == SELECTOR_PREVIOUS_MOVE_WHITE_SIDE || auxiliarySelector == SELECTOR_PREVIOUS_MOVE_BLACK_SIDE) {
		if (goToPreviousMoveInHistory()) {
			_animationDelayBetweenMoves->reset();
			_animationInProgress = true;
		}
	} else if (auxiliarySelector == SELECTOR_NEXT_MOVE_WHITE_SIDE || auxiliarySelector == SELECTOR_NEXT_MOVE_BLACK_SIDE) {
		if (goToNextMoveInHistory()) {
			_animationDelayBetweenMoves->reset();
			_animationInProgress = true;
		}
	} else if (_piecePromotionInProgress) {		
		if (auxiliarySelector == SELECTOR_PROMOTE_TO_QUEEN_WHITE_SIDE || auxiliarySelector == SELECTOR_PROMOTE_TO_QUEEN_BLACK_SIDE) {
			_rankOfPromotion = QUEEN;
			setupPiecePromotion();
		} else if (auxiliarySelector == SELECTOR_PROMOTE_TO_ROOK_WHITE_SIDE || auxiliarySelector == SELECTOR_PROMOTE_TO_ROOK_BLACK_SIDE) {
			_rankOfPromotion = ROOK;
			setupPiecePromotion();
		} else if (auxiliarySelector == SELECTOR_PROMOTE_TO_KNIGHT_WHITE_SIDE || auxiliarySelector == SELECTOR_PROMOTE_TO_KNIGHT_BLACK_SIDE) {
			_rankOfPromotion = KNIGHT;
			setupPiecePromotion();
		} else if (auxiliarySelector == SELECTOR_PROMOTE_TO_BISHOP_WHITE_SIDE || auxiliarySelector == SELECTOR_PROMOTE_TO_BISHOP_BLACK_SIDE) {
			_rankOfPromotion = BISHOP;
			setupPiecePromotion();
		}		
	}
}


void ChessBoard::setupPiecePromotion() {
	_piecePromotionConversionInProgress = true;
	_promotionConversionTimer->finish();
	_pieceToPromote->shrinkPiece();

	for (size_t i = 0; i < _promotionChessPieces.size(); ++i) {
		_promotionChessPieces[i]->shrinkPiece();
	}
}


bool ChessBoard::processFirstSelection(Vec2i selectorBoardPosition) {
	_pieceToMove = selectPosition(selectorBoardPosition.x(), selectorBoardPosition.y(), _currentPlayer, true);

	// mark origin position if a valid piece was selected
	if (_pieceToMove != NULL) {
		_moveOriginPosition.x() = selectorBoardPosition.x();
		_moveOriginPosition.y() = selectorBoardPosition.y();
		_moveDestinationPosition.x() = 0;
		_moveDestinationPosition.y() = 0;
		clearHighlights();
		showPossibleMoves(_pieceToMove);
		return true;
	} else {
		return false;
	}
}


bool ChessBoard::processSecondSelection(Vec2i selectorBoardPosition, bool playerIsAI) {
	if (_pieceToMove == NULL) {
		return false;
	}

	MovePositionStatus movePositionStatus = isPositionAvailableToReceiveMove(_moveOriginPosition, selectorBoardPosition, _currentPlayer);

	// invalid selection
	if (movePositionStatus == POSITION_WITH_PLAYER_PIECE || movePositionStatus == POSITION_INVALID) {
		return false;
	}

	// deselect position
	if (movePositionStatus == SAME_POSITION_AS_ORIGIN) {
		_moveOriginPosition.x() = 0;
		_moveOriginPosition.y() = 0;
		clearSelections();
		clearPossibleMoves();
		hightLighPosition(selectorBoardPosition.x(), selectorBoardPosition.y());
		return false;
	}	

	// second selection (if not defined yet)	
	if (_moveDestinationPosition.x() == 0 && (movePositionStatus == POSITION_AVAILABLE || movePositionStatus == POSITION_WITH_OPPONENT_PIECE)) {		
		if (!playerIsAI && !isPositionInPossibleMoves(selectorBoardPosition)) {
			return false;
		}

		_moveDestinationPosition.x() = selectorBoardPosition.x();
		_moveDestinationPosition.y() = selectorBoardPosition.y();		

		// remove opponent piece if necessary
		ChessPiece* pieceRemoved = NULL;
		if (movePositionStatus == POSITION_WITH_OPPONENT_PIECE) {
			pieceRemoved = removeChessPieceWithAnimation(_moveDestinationPosition, _currentPlayer);
		} else {
			pieceRemoved = isEnPassantPawnCapturePossible(_pieceToMove, _moveDestinationPosition, _currentPlayer);
			if (pieceRemoved != NULL) {
				pieceRemoved->removePieceFromBoard();
			}
		}

		_pieceMovesHistoryBackwardsStack.push_back(new ChessMoveHistory(_pieceToMove, _moveOriginPosition, _moveDestinationPosition,
			_pieceToMove->getPieceMovedPreviously(),
			_whitePlayerGameTimerD, _whitePlayerGameTimerD + _whitePlayerGameTimer->elapsedTime(),
			_blackPlayerGameTimerD, _blackPlayerGameTimerD + _blackPlayerGameTimer->elapsedTime(),
			_currentPlayNumber, pieceRemoved));
				
		
		// animate piece movement to final destination
		moveChessPieceWithAnimation(_pieceToMove, _moveDestinationPosition);
		//selectPosition(selectorBoardPosition.x(), selectorBoardPosition.y(), _currentPlayer);
				
		if (!playerIsAI) {
			clearHighlights();
			clearSelections();
			clearPossibleMoves();
			hightLighPosition(selectorBoardPosition.x(), selectorBoardPosition.y());			
		}
		

		_animationDelayBetweenMoves->reset();
		_animationInProgress = true;
		++_currentPlayNumber;
		updateGameStatusWithCheckOrCheckMate();

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
	_promotionChessPieces.clear();
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
				_boardSquareSelectorHighlights->addChild(ChessUtils::createRectangleWithTexture(scenePosition, new Image(*_paddleSelectorImg)));
				break;
			}
		}
		

		return true;
	}

	if (isPositionValid(xBoardPosition) && isPositionValid(yBoardPosition)) {
		Vec3f scenePosition = ChessUtils::computePieceScenePosition(xBoardPosition, yBoardPosition);
		scenePosition.z() = BOARD_HIGHLIGHTS_HEIGHT_OFFSET;
		_boardSquareSelectorHighlights->addChild(ChessUtils::createRectangleWithTexture(scenePosition, new Image(*_paddleSelectorImg)));
		return true;
	} else {
		return false;
	}
}


bool ChessBoard::hightLighPossibleMove(int xBoardPosition, int yBoardPosition) {
	if (isPositionValid(xBoardPosition) && isPositionValid(yBoardPosition)) {
		Vec3f scenePosition = ChessUtils::computePieceScenePosition(xBoardPosition, yBoardPosition);
		scenePosition.z() = BOARD_POSSIBLE_MOVES_HEIGHT_OFFSET;
		_boardSquarePossibleMoves->addChild(ChessUtils::createRectangleWithTexture(scenePosition, new Image(*_possibleMoveImg)));
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
		selectionMT->addChild(ChessUtils::createRectangleWithTexture(Vec3(0,0,0), new Image(*_paddleSelectedImg)));

		if (chessPieceColor == BLACK) {
			selectionMT->postMult(Matrix::rotate(osg::PI, osg::Z_AXIS));
		}

		selectionMT->postMult(Matrix::translate(scenePosition));
		_boardSquareSelections->addChild(selectionMT);
	}

	return chessPieceAtPosition;

}


bool ChessBoard::showPossibleMoves(ChessPiece* chessPiece) {
	clearPossibleMoves();

	ChessPieceColor chessPieceColor = chessPiece->getChessPieceColor();
	bool seeIfWhiteKingIsInCheck = (chessPieceColor == WHITE ? true : false);
	bool seeIfBlackKingIsInCheck = (chessPieceColor == BLACK ? true : false);

	_pieceSelectedPossibleMoves = computePossibleMovePositions(_pieceToMove, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck);

	if (!_pieceSelectedPossibleMoves->empty()) {		
		for (size_t i = 0; i < _pieceSelectedPossibleMoves->size(); ++i) {
			hightLighPossibleMove(_pieceSelectedPossibleMoves->at(i).x(), _pieceSelectedPossibleMoves->at(i).y());
		}

		return true;
	}

	return false;
}


bool ChessBoard::isPositionInPossibleMoves(Vec2i position) {
	if (_pieceSelectedPossibleMoves == NULL || _pieceSelectedPossibleMoves->empty()) {
		return false;
	}

	for (size_t i = 0; i < _pieceSelectedPossibleMoves->size(); ++i) {
		if (_pieceSelectedPossibleMoves->at(i) == position) {
			return true;
		}
	}	

	return false;
}


Vec2iArray* ChessBoard::computePossibleMovePositions(ChessPiece* chessPiece, bool seeIfWhiteKingIsInCheck, bool seeIfBlackKingIsInCheck) {
	Vec2iArray* possibleMoves = new Vec2iArray();
	
	if (!chessPiece->isPiecePlayable()) {
		return possibleMoves;
	}

	int xBoardPosition = chessPiece->getXPosition();
	int yBoardPosition = chessPiece->getYPosition();
	Vec2i pieceInitialPosition = Vec2i(xBoardPosition, yBoardPosition);
	ChessPieceType chessPieceType = chessPiece->getChessPieceType();
	ChessPieceColor chessPieceColor = chessPiece->getChessPieceColor();
	ChessPieceColor opponentChessPieceColor = ChessPiece::getOpponentChessPieceColor(chessPieceColor);	

	switch (chessPieceType) {		
		case KING: {
			// normal moves
			updatePossibleMoves(pieceInitialPosition, Vec2i((xBoardPosition ==  1 ? xBoardPosition - 2 : xBoardPosition - 1),  yBoardPosition),                                                  chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			updatePossibleMoves(pieceInitialPosition, Vec2i((xBoardPosition ==  1 ? xBoardPosition - 2 : xBoardPosition - 1), (yBoardPosition == -1 ? yBoardPosition + 2 : yBoardPosition + 1)), chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			updatePossibleMoves(pieceInitialPosition, Vec2i(xBoardPosition,                                                   (yBoardPosition == -1 ? yBoardPosition + 2 : yBoardPosition + 1)), chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			updatePossibleMoves(pieceInitialPosition, Vec2i((xBoardPosition == -1 ? xBoardPosition + 2 : xBoardPosition + 1), (yBoardPosition == -1 ? yBoardPosition + 2 : yBoardPosition + 1)), chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			updatePossibleMoves(pieceInitialPosition, Vec2i((xBoardPosition == -1 ? xBoardPosition + 2 : xBoardPosition + 1),  yBoardPosition),                                                  chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			updatePossibleMoves(pieceInitialPosition, Vec2i((xBoardPosition == -1 ? xBoardPosition + 2 : xBoardPosition + 1), (yBoardPosition == 1 ? yBoardPosition - 2 : yBoardPosition - 1)),  chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			updatePossibleMoves(pieceInitialPosition, Vec2i(xBoardPosition,                                                   (yBoardPosition == 1 ? yBoardPosition - 2 : yBoardPosition - 1)),  chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			updatePossibleMoves(pieceInitialPosition, Vec2i((xBoardPosition == 1 ? xBoardPosition - 2 : xBoardPosition - 1),  (yBoardPosition == 1 ? yBoardPosition - 2 : yBoardPosition - 1)),  chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);

			// left castling
			Vec2i leftCastlingPosition = Vec2i(-2, chessPiece->getYPosition());
			if (isCastlingPossible(chessPiece, leftCastlingPosition, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck)) {
				possibleMoves->push_back(leftCastlingPosition);
			}

			// right castling
			Vec2i rightCastlingPosition = Vec2i(3, chessPiece->getYPosition());
			if (isCastlingPossible(chessPiece, rightCastlingPosition, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck)) {
				possibleMoves->push_back(rightCastlingPosition);
			}
			
			break;
		}

		case QUEEN: {
			// vertical moves
			updatePossibleMovesAlongLine(pieceInitialPosition,  0,  1, chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			updatePossibleMovesAlongLine(pieceInitialPosition,  0, -1, chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			
			// horizontal moves
			updatePossibleMovesAlongLine(pieceInitialPosition, -1,  0, chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			updatePossibleMovesAlongLine(pieceInitialPosition,  1,  0, chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);

			// diagonal moves
			updatePossibleMovesAlongLine(pieceInitialPosition, -1,  1, chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			updatePossibleMovesAlongLine(pieceInitialPosition,  1,  1, chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			updatePossibleMovesAlongLine(pieceInitialPosition,  1, -1, chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			updatePossibleMovesAlongLine(pieceInitialPosition, -1, -1, chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);

			break;
		}

		case ROOK: {
			// vertical moves
			updatePossibleMovesAlongLine(pieceInitialPosition, 0, 1, chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			updatePossibleMovesAlongLine(pieceInitialPosition, 0, -1, chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);

			// horizontal moves
			updatePossibleMovesAlongLine(pieceInitialPosition, -1, 0, chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			updatePossibleMovesAlongLine(pieceInitialPosition, 1, 0, chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);

			break;
		}

		case KNIGHT: {
			updatePossibleMoves(pieceInitialPosition, Vec2i(((xBoardPosition ==  1 || xBoardPosition ==  2) ? xBoardPosition - 3 : xBoardPosition - 2), ( yBoardPosition == -1                          ? yBoardPosition + 2 : yBoardPosition + 1)), chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			updatePossibleMoves(pieceInitialPosition, Vec2i(( xBoardPosition ==  1                          ? xBoardPosition - 2 : xBoardPosition - 1), ((yBoardPosition == -1 || yBoardPosition == -2) ? yBoardPosition + 3 : yBoardPosition + 2)), chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			updatePossibleMoves(pieceInitialPosition, Vec2i(( xBoardPosition == -1                          ? xBoardPosition + 2 : xBoardPosition + 1), ((yBoardPosition == -1 || yBoardPosition == -2) ? yBoardPosition + 3 : yBoardPosition + 2)), chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			updatePossibleMoves(pieceInitialPosition, Vec2i(((xBoardPosition == -1 || xBoardPosition == -2) ? xBoardPosition + 3 : xBoardPosition + 2), ( yBoardPosition == -1                          ? yBoardPosition + 2 : yBoardPosition + 1)), chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			updatePossibleMoves(pieceInitialPosition, Vec2i(((xBoardPosition == -1 || xBoardPosition == -2) ? xBoardPosition + 3 : xBoardPosition + 2), ( yBoardPosition ==  1                          ? yBoardPosition - 2 : yBoardPosition - 1)), chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			updatePossibleMoves(pieceInitialPosition, Vec2i(( xBoardPosition == -1                          ? xBoardPosition + 2 : xBoardPosition + 1), ((yBoardPosition ==  1 || yBoardPosition ==  2) ? yBoardPosition - 3 : yBoardPosition - 2)), chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			updatePossibleMoves(pieceInitialPosition, Vec2i(( xBoardPosition ==  1                          ? xBoardPosition - 2 : xBoardPosition - 1), ((yBoardPosition ==  1 || yBoardPosition ==  2) ? yBoardPosition - 3 : yBoardPosition - 2)), chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			updatePossibleMoves(pieceInitialPosition, Vec2i(((xBoardPosition ==  1 || xBoardPosition ==  2) ? xBoardPosition - 3 : xBoardPosition - 2), ( yBoardPosition ==  1                          ? yBoardPosition - 2 : yBoardPosition - 1)), chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			break;
		}

		case BISHOP: {
			// diagonal moves
			updatePossibleMovesAlongLine(pieceInitialPosition, -1, 1, chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			updatePossibleMovesAlongLine(pieceInitialPosition, 1, 1, chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			updatePossibleMovesAlongLine(pieceInitialPosition, 1, -1, chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			updatePossibleMovesAlongLine(pieceInitialPosition, -1, -1, chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);

			break;
		}

		case PAWN: {
			// no piece in front of pawn
			int pawnFrontPositionX = xBoardPosition;
			int pawnFrontPositionY = (yBoardPosition == -1 ? yBoardPosition + 2 : yBoardPosition + 1);
			
			if (chessPieceColor == BLACK) {
				pawnFrontPositionY = (yBoardPosition == 1 ? yBoardPosition - 2 : yBoardPosition - 1);
			}
			
			Vec2i pawnFrontPosition = Vec2i(pawnFrontPositionX, pawnFrontPositionY);
			bool positionInFrontFree = false;
			if (getChessPieceAtBoardPosition(pawnFrontPositionX, pawnFrontPositionY, chessPieceColor) == NULL &&
				getChessPieceAtBoardPosition(pawnFrontPositionX, pawnFrontPositionY, opponentChessPieceColor) == NULL) {
				updatePossibleMoves(pieceInitialPosition, pawnFrontPosition, chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
				positionInFrontFree = true;
			}

			// double move
			if (!chessPiece->getPieceMovedPreviously()) {
				int pawnDoubleMoveFrontPositionX = xBoardPosition;
				int pawnDoubleMoveFrontPositionY = (chessPieceColor == WHITE ? -1 : 1);

				Vec2i pawnDoubleMoveFrontPosition = Vec2i(pawnDoubleMoveFrontPositionX, pawnDoubleMoveFrontPositionY);
				if (positionInFrontFree &&
					getChessPieceAtBoardPosition(pawnDoubleMoveFrontPositionX, pawnDoubleMoveFrontPositionY, chessPieceColor) == NULL &&
					getChessPieceAtBoardPosition(pawnDoubleMoveFrontPositionX, pawnDoubleMoveFrontPositionY, opponentChessPieceColor) == NULL) {
					updatePossibleMoves(pieceInitialPosition, pawnDoubleMoveFrontPosition, chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
				}
			}

			// capture left
			int pawnLeftPositionX = (xBoardPosition == 1 ? xBoardPosition - 2 : xBoardPosition - 1);
			int pawnLeftPositionY = (yBoardPosition == -1 ? yBoardPosition + 2 : yBoardPosition + 1);

			if (chessPieceColor == BLACK) {
				pawnLeftPositionY = (yBoardPosition == 1 ? yBoardPosition - 2 : yBoardPosition - 1);
			}

			Vec2i pawnLeftPosition = Vec2i(pawnLeftPositionX, pawnLeftPositionY);
			if (getChessPieceAtBoardPosition(pawnLeftPositionX, pawnLeftPositionY, opponentChessPieceColor) != NULL ||
				isEnPassantPawnCapturePossible(chessPiece, pawnLeftPosition, chessPieceColor)) {
				updatePossibleMoves(pieceInitialPosition, pawnLeftPosition, chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			}


			// capture right
			int pawnRightPositionX = (xBoardPosition == -1 ? xBoardPosition + 2 : xBoardPosition + 1);
			int pawnRightPositionY = (yBoardPosition == -1 ? yBoardPosition + 2 : yBoardPosition + 1);

			if (chessPieceColor == BLACK) {
				pawnRightPositionY = (yBoardPosition == 1 ? yBoardPosition - 2 : yBoardPosition - 1);
			}

			Vec2i pawnRightPosition = Vec2i(pawnRightPositionX, pawnRightPositionY);
			if (getChessPieceAtBoardPosition(pawnRightPositionX, pawnRightPositionY, opponentChessPieceColor) != NULL ||
				isEnPassantPawnCapturePossible(chessPiece, pawnRightPosition, chessPieceColor)) {
				updatePossibleMoves(pieceInitialPosition, pawnRightPosition, chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck, possibleMoves);
			}

			break;
		}

		default:
			break;
	}


	return possibleMoves;
}


bool ChessBoard::isMoveValid(Vec2i initialPosition, Vec2i finalPosition, ChessPieceColor chessPieceColor, bool seeIfWhiteKingIsInCheck, bool seeIfBlackKingIsInCheck) {
	bool moveValid = false;

	MovePositionStatus movePositionStatus = isPositionAvailableToReceiveMove(initialPosition, finalPosition, chessPieceColor);
	if (movePositionStatus == POSITION_AVAILABLE || movePositionStatus == POSITION_WITH_OPPONENT_PIECE) {
		if ((chessPieceColor == WHITE && seeIfWhiteKingIsInCheck) || (chessPieceColor == BLACK && seeIfBlackKingIsInCheck)) {
			ChessPiece* pieceGoingToBeMoved = getChessPieceAtBoardPosition(initialPosition.x(), initialPosition.y(), chessPieceColor);
			ChessPiece* pieceGoingToBeCaptured = getChessPieceAtBoardPosition(finalPosition.x(), finalPosition.y(), ChessPiece::getOpponentChessPieceColor(chessPieceColor));

			if (pieceGoingToBeMoved != NULL) {
				pieceGoingToBeMoved->setPosition(finalPosition.x(), finalPosition.y());
			}

			if (pieceGoingToBeCaptured != NULL) {
				pieceGoingToBeCaptured->setPiecePlayable(false);
			}

			Vec2i kingPosition;
			if (pieceGoingToBeMoved != NULL && pieceGoingToBeMoved->getChessPieceType() == KING) {
				kingPosition = finalPosition;
			} else {
				kingPosition = (chessPieceColor == WHITE ? Vec2i(_pieceKingWhite->getXPosition(), _pieceKingWhite->getYPosition()) : Vec2i(_pieceKingBlack->getXPosition(), _pieceKingBlack->getYPosition()));
			}

			if (!isKingInCheck(kingPosition, chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck)) {
				moveValid = true;
			}

			if (pieceGoingToBeMoved != NULL) {
				pieceGoingToBeMoved->setPosition(initialPosition.x(), initialPosition.y());
			}

			if (pieceGoingToBeCaptured != NULL) {
				pieceGoingToBeCaptured->setPiecePlayable(true);
			}
		} else {			
			moveValid = true;
		}
	}

	return moveValid;
}


bool ChessBoard::updatePossibleMoves(Vec2i initialPosition, Vec2i finalPosition, ChessPieceColor chessPieceColor, bool seeIfWhiteKingIsInCheck, bool seeIfBlackKingIsInCheck, Vec2iArray* possibleMoves) {
	if (possibleMoves != NULL && isMoveValid(initialPosition, finalPosition, chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck)) {
		possibleMoves->push_back(finalPosition);
		return true;
	}

	return false;
}


ChessPiece* ChessBoard::updatePossibleMovesAlongLine(Vec2i initialPosition, int xIncrement, int yIncrement, ChessPieceColor chessPieceColor, bool seeIfWhiteKingIsInCheck, bool seeIfBlackKingIsInCheck, Vec2iArray* possibleMoves) {
	Vec2i finalPosition = initialPosition;
	ChessPieceColor opponentChessPieceColor = ChessPiece::getOpponentChessPieceColor(chessPieceColor);
	ChessPiece* pieceInFinalPosition = NULL;

	do {		
		finalPosition.x() += xIncrement;
		finalPosition.y() += yIncrement;

		if (finalPosition.x() == 0) {
			finalPosition.x() += xIncrement;
		}

		if (finalPosition.y() == 0) {
			finalPosition.y() += yIncrement;
		}

		// reach board border
		if (!isPositionValid(finalPosition.x()) || !isPositionValid(finalPosition.y())) {
			return NULL;
		}

		// found piece with same color
		pieceInFinalPosition = getChessPieceAtBoardPosition(finalPosition.x(), finalPosition.y(), chessPieceColor);		
		if (pieceInFinalPosition != NULL) {
			return pieceInFinalPosition;
		}

		// found opponent piece or empty square
		pieceInFinalPosition = getChessPieceAtBoardPosition(finalPosition.x(), finalPosition.y(), opponentChessPieceColor);


		if (isMoveValid(initialPosition, finalPosition, chessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck)) {
			possibleMoves->push_back(Vec2i(finalPosition.x(), finalPosition.y()));
		}
	} while (pieceInFinalPosition == NULL);

	return pieceInFinalPosition;
}


MovePositionStatus ChessBoard::isPositionAvailableToReceiveMove(Vec2i initialPosition, Vec2i finalPosition, ChessPieceColor currentPlayer) {
	if (!isPositionValid(finalPosition.x()) || !isPositionValid(finalPosition.y())) {
		return POSITION_INVALID;
	}

	if (initialPosition.x() != finalPosition.x() || initialPosition.y() != finalPosition.y()) {
		if (getChessPieceAtBoardPosition(finalPosition.x(), finalPosition.y(), currentPlayer) != NULL) {
			return POSITION_WITH_PLAYER_PIECE;
		}

		if (getChessPieceAtBoardPosition(finalPosition.x(), finalPosition.y(), ChessPiece::getOpponentChessPieceColor(currentPlayer)) != NULL) {
			return POSITION_WITH_OPPONENT_PIECE;
		}

		return POSITION_AVAILABLE;
	} else {
		return SAME_POSITION_AS_ORIGIN;
	}
}



void ChessBoard::moveChessPieceWithAnimation(ChessPiece* chessPieceToMove, Vec2i finalPosition) {			
	if (!checkAndPerformCastling(chessPieceToMove, finalPosition)) {
		chessPieceToMove->changePositionWithAnimation(finalPosition.x(), finalPosition.y(), _currentPlayNumber);
	} else {
		_pieceMovesHistoryBackwardsStack.back()->setPerformedCastling(true);
	}	

	checkAndPerformPromotion(chessPieceToMove, finalPosition);	

	_pieceMovesHistoryFowardStack.clear();
	updateHistoryManipulatorsVisibility();
}

ChessPiece* ChessBoard::removeChessPieceWithAnimation(Vec2i boardPositionOfPieceToRemove, ChessPieceColor currentPlayerChessPieceColor) {
	ChessPiece* pieceToRemove = getChessPieceAtBoardPosition(boardPositionOfPieceToRemove.x(), boardPositionOfPieceToRemove.y(), ChessPiece::getOpponentChessPieceColor(currentPlayerChessPieceColor));
	pieceToRemove->removePieceFromBoard();

	return pieceToRemove;
}


bool ChessBoard::checkAndPerformCastling(ChessPiece* king, Vec2i finalPosition) {
	bool seeIfWhiteKingIsInCheck = (king->getChessPieceColor() == WHITE ? true : false);
	bool seeIfBlackKingIsInCheck = (king->getChessPieceColor() == BLACK ? true : false);

	if (isCastlingPossible(king, finalPosition, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck)) {
		ChessPiece* rook = getChessPieceAtBoardPosition((finalPosition.x() == 3 ? 4 : -4), king->getYPosition(), king->getChessPieceColor());

		// move king
		king->changePositionWithAnimation(finalPosition.x(), finalPosition.y(), _currentPlayNumber);

		// move rook
		rook->changePositionWithAnimation((finalPosition.x() == 3 ? 2 : -1), finalPosition.y(), _currentPlayNumber);
		return true;
	}

	return false;
}


bool ChessBoard::isCastlingPossible(ChessPiece* king, Vec2i kingFinalPosition, bool seeIfWhiteKingIsInCheck, bool seeIfBlackKingIsInCheck) {
	// castling can only be done by the king and a rook if none of then has moved, and there is no pieces between then
	if (king->getChessPieceType() == KING && !king->getPieceMovedPreviously()) {		
		if (kingFinalPosition.x() == 3) {
			// check if there are pieces blocking the castling
			if (getChessPieceAtBoardPosition(2, king->getYPosition(), WHITE) != NULL || getChessPieceAtBoardPosition(2, king->getYPosition(), BLACK) != NULL ||
				getChessPieceAtBoardPosition(3, king->getYPosition(), WHITE) != NULL || getChessPieceAtBoardPosition(3, king->getYPosition(), BLACK) != NULL) {
				return false;
			}

			// check if the rook hasn't move
			ChessPiece* rookRightSide = getChessPieceAtBoardPosition(4, king->getYPosition(), king->getChessPieceColor());
			if (rookRightSide == NULL || rookRightSide->getPieceMovedPreviously()) {
				return false;
			}
						

			// castling is only possible if none of the positions the king will cross are attacked			
			if (king->getChessPieceColor() == WHITE && seeIfWhiteKingIsInCheck || king->getChessPieceColor() == BLACK && seeIfBlackKingIsInCheck) { // to avoid circular calls between white and black possible moves
				// king can't castle when is in check
				if (isKingInCheck(Vec2i(king->getXPosition(), king->getYPosition()), king->getChessPieceColor(), seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck)) {
					return false;
				}

				if (isKingInCheck(Vec2i(2, kingFinalPosition.y()), king->getChessPieceColor(), seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck) ||
					isKingInCheck(Vec2i(3, kingFinalPosition.y()), king->getChessPieceColor(), seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck)) {
					return false;
				}
			}
			
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
			if (rookRightSide == NULL || rookRightSide->getPieceMovedPreviously()) {
				return false;
			}			

			// castling is only possible if none of the positions the king will cross are attacked
			if (king->getChessPieceColor() == WHITE && seeIfWhiteKingIsInCheck || king->getChessPieceColor() == BLACK && seeIfBlackKingIsInCheck) { // to avoid circular calls between white and black possible moves
				// king can't castle when is in check
				if (isKingInCheck(Vec2i(king->getXPosition(), king->getYPosition()), king->getChessPieceColor(), seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck)) {
					return false;
				}

				if (isKingInCheck(Vec2i(-1, kingFinalPosition.y()), king->getChessPieceColor(), seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck) ||
					isKingInCheck(Vec2i(-2, kingFinalPosition.y()), king->getChessPieceColor(), seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck)) {
					return false;
				}
			}

			return true;
		}
	}

	return false;
}


ChessPiece* ChessBoard::isEnPassantPawnCapturePossible(ChessPiece* chessPieceToMove, Vec2i finalPosition, ChessPieceColor currentPlayerPieceColor) {
	if (chessPieceToMove->getChessPieceType() == PAWN) {
		ChessPiece* pieceToCheck = getChessPieceAtBoardPosition(finalPosition.x(), (currentPlayerPieceColor == WHITE ? finalPosition.y() - 1 : finalPosition.y() + 1), ChessPiece::getOpponentChessPieceColor(currentPlayerPieceColor));
		if (pieceToCheck != NULL &&
			pieceToCheck->getChessPieceType() == PAWN &&
			pieceToCheck->getPawnMakeDoubleStep() &&
			pieceToCheck->getPlayNumberOfLastMove() == (_currentPlayNumber - 1)) {
			return pieceToCheck;
		}
	}

	return NULL;
}


bool ChessBoard::checkAndPerformPromotion(ChessPiece* chessPieceMoved, Vec2i finalPosition) {
	if (chessPieceMoved->getChessPieceType() == PAWN) {
		if ((chessPieceMoved->getChessPieceColor() == WHITE && chessPieceMoved->getYPosition() == 4) ||
			(chessPieceMoved->getChessPieceColor() == BLACK && chessPieceMoved->getYPosition() == -4)) {
			_pieceToPromote = chessPieceMoved;			

			_pieceMovesHistoryBackwardsStack.back()->setPerformedPromotion(true);
			setupPromotionPiecesOnBoad(chessPieceMoved->getChessPieceColor());
			_piecePromotionInProgress = true;
			return true;
		}
	}

	return false;
}


void ChessBoard::setupPromotionPiecesOnBoad(ChessPieceColor chessPieceColor) {
	_promotionChessPieces.clear();	
	
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
				_pieceMovesHistoryBackwardsStack.back()->setOriginalPieceType(_pieceToPromote->getChessPieceType());
				_pieceMovesHistoryBackwardsStack.back()->setPromotionPieceType(_rankOfPromotion);
			}

			_pieceToPromote->loadPieceModel(_rankOfPromotion, Vec3(0.0, 0.0, 0.0));
			_pieceToPromote->unshrinkPiece();
		}
	}

	// piece promotion done
	if (_promotionConversionTimer->elapsedTime() > 2.0 * PROMOTION_SCALE_ANIMATION_DURATION_SECONDS) {		
		_piecePromotionInProgress = false;
		_piecePromotionConversionInProgress = false;
		_piecePromotionConversionFromHistoryInProgress = false;

		if (_piecePromotionReversionInProgress) {
			ChessMoveHistory* moveBackInfo = _pieceMovesHistoryFowardStack.back();
			
			// move piece to original position
			moveBackInfo->moveBackInHistory();
			_piecePromotionReversionInProgress = false;
		}		

		//switchPlayer();
	}
}


void ChessBoard::removePromotionPiecesOnBoad() {
	for (size_t i = 0; i < _promotionChessPieces.size(); ++i) {
		_promotionChessPieces[i]->shrinkPiece();	
	}
}


bool ChessBoard::isKingInCheck(Vec2i kingPosition, ChessPieceColor kingColor, bool seeIfWhiteKingIsInCheck, bool seeIfBlackKingIsInCheck) {
	vector<ChessPiece*>& opponentPieces = (kingColor == WHITE ? _blackChessPieces : _whiteChessPieces);
	ChessPieceColor opponentPieceColor = ChessPiece::getOpponentChessPieceColor(kingColor);	
		
	for (size_t opponentPieceIndex = 0; opponentPieceIndex < opponentPieces.size(); ++opponentPieceIndex) {
		Vec2iArray* opponentPieceAttackPositions = computePossibleMovePositions(opponentPieces[opponentPieceIndex], seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck);

		for (size_t attackPositionIndex = 0; attackPositionIndex < opponentPieceAttackPositions->size(); ++attackPositionIndex) {
			Vec2i attackPosition = opponentPieceAttackPositions->at(attackPositionIndex);
			if (attackPosition == kingPosition) {
				return true;
			}
		}
	}

	return false;
}


bool ChessBoard::isKingInCheckMate(Vec2i kingCurrentPosition, ChessPieceColor kingColor) {
	// see if king is in check or can move out of check
	/*if (isKingInCheck(kingCurrentPosition, kingColor)) {
		ChessPiece* kingPiece = (kingColor == WHITE ? _pieceKingWhite : _pieceKingBlack);
		Vec2iArray* kingPossibleMoves = computePossibleMovePositions(kingPiece);

		for (size_t kingPossibleMoveIndex = 0; kingPossibleMoveIndex < kingPossibleMoves->size(); ++kingPossibleMoveIndex) {
			Vec2i kingPossibleMove = kingPossibleMoves->at(kingPossibleMoveIndex);
			if (!isKingInCheck(kingPossibleMove, kingColor)) {
				return false;
			}			
		}
	}*/

	// see if king can avoid check mate by moving one of its pieces
	return _uciProtocol.isBoardPositionInCheckMate(computeBoardUCIMoves());
}


void ChessBoard::updateGameStatusWithCheckOrCheckMate() {
	clearGameStatusText();

	Vec2i opponentKingPosition;
	ChessPieceColor opponentChessPieceColor = ChessPiece::getOpponentChessPieceColor(_currentPlayer);
	if (opponentChessPieceColor == WHITE) {
		opponentKingPosition.x() = _pieceKingWhite->getXPosition();
		opponentKingPosition.y() = _pieceKingWhite->getYPosition(); 
	} else {
		opponentKingPosition.x() = _pieceKingBlack->getXPosition();
		opponentKingPosition.y() = _pieceKingBlack->getYPosition();
	}

	// when is a AI player the end game is automatically checked so there is no need to check it here
	if (!isCurrentPlayerAI()) {
		// see if there is a check mate
		if (isKingInCheckMate(opponentKingPosition, opponentChessPieceColor)) {
			if (opponentChessPieceColor == WHITE) {
				_gameStatus = BLACK_WON;
			} else {
				_gameStatus = WHITE_WON;
			}

			showGameStatus(_gameStatus);
			return;
		}
	}

	bool seeIfWhiteKingIsInCheck = (opponentChessPieceColor == WHITE ? true : false);
	bool seeIfBlackKingIsInCheck = (opponentChessPieceColor == BLACK ? true : false);

	if (isKingInCheck(opponentKingPosition, opponentChessPieceColor, seeIfWhiteKingIsInCheck, seeIfBlackKingIsInCheck)) {
		if (opponentChessPieceColor == WHITE) {
			_gameStatus = WHITE_IN_CHECK;
		} else {
			_gameStatus = BLACK_IN_CHECK;
		}

		showGameStatus(_gameStatus);
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
				rook->changePositionWithAnimation((king->getXPosition() == 3 ? 4 : -4), king->getYPosition());
				rook->setPieceMovedPreviously(false);
			}
		}

		_pieceMovesHistoryFowardStack.push_back(moveBackInfo);
		_pieceMovesHistoryBackwardsStack.pop_back();

		if (moveBackInfo->hasPerformedPromotion()) {			
			_pieceToPromote = moveBackInfo->getPieceMoved();
			_rankOfPromotion = moveBackInfo->getOriginalPieceType();

			if (_rankOfPromotion != _pieceToPromote->getChessPieceType()) {
				_pieceToPromote->shrinkPiece();
				_promotionConversionTimer->finish();
				_piecePromotionReversionInProgress = true;
			} else {
				moveBackInfo->moveBackInHistory();
			}
		} else {			
			moveBackInfo->moveBackInHistory();
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

		if (_piecePromotionInProgress) {
			removePromotionPiecesOnBoad();
			_piecePromotionInProgress = false;
			_piecePromotionInProgressAborted = true;
		}


		_animationDelayBetweenMoves->reset();
		--_currentPlayNumber;
		_animationInProgress = true;
		_gameStatus = IN_PROGRESS;	
		_movedBackwards = true;
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
				rook->changePositionWithAnimation((moveFowardInfo->getPieceMovedDestinationPosition().x() == 3 ? 2 : -1), king->getYPosition());
			}
		}

		// move king to destination position
		moveFowardInfo->moveFowardInHistory();

		_pieceMovesHistoryBackwardsStack.push_back(moveFowardInfo);
		_pieceMovesHistoryFowardStack.pop_back();


		// cancel current piece promotion
		if (_piecePromotionInProgress) {
			removePromotionPiecesOnBoad();
			_piecePromotionInProgress = false;
			_piecePromotionInProgressAborted = true;
		}

		if (moveFowardInfo->hasPerformedPromotion()) {			
			_pieceToPromote = moveFowardInfo->getPieceMoved();
			_rankOfPromotion = moveFowardInfo->getPromotionPieceType();
			_animationDelayBetweenMoves->finish();

			if (_pieceToPromote->getChessPieceType() == _rankOfPromotion) {
				// move was canceled before promotion rank was chosen
				setupPromotionPiecesOnBoad(_pieceToPromote->getChessPieceColor());
				_piecePromotionInProgress = true;
			} else {				
				Vec2i pieceMovedOriginPosition = moveFowardInfo->getPieceMovedOriginPosition();
				Vec2i pieceMovedDestinationPosition = moveFowardInfo->getPieceMovedDestinationPosition();

				Vec3f initialPieceScenePosition = ChessUtils::computePieceScenePosition(pieceMovedOriginPosition.x(), pieceMovedOriginPosition.y());
				Vec3f finalPieceScenePosition = ChessUtils::computePieceScenePosition(pieceMovedDestinationPosition.x(), pieceMovedDestinationPosition.y());

				Vec3 positionsOffset = finalPieceScenePosition - initialPieceScenePosition;
				float positionsDistance = positionsOffset.length();
				float pieceTravelSpeed = PIECE_MOVE_ANIMATION_TRAVEL_SPEED;
				_timeToWaitForPromotingPieceFromHistory = (std::max)(positionsDistance / pieceTravelSpeed, 2.0f);

				_piecePromotionConversionInProgress = false;
				_piecePromotionConversionFromHistoryInProgress = true;
			}
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

		_animationDelayBetweenMoves->reset();
		_animationInProgress = true;
		++_currentPlayNumber;
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
	// reset positions to next piece move
	_moveOriginPosition.x() = 0;
	_moveOriginPosition.y() = 0;
	_moveDestinationPosition.x() = 0;
	_moveDestinationPosition.y() = 0;

	updatePlayersCumulativeGameTimers();
	
	clearSelections();
	clearPossibleMoves();
	clearPromotionPieces();	

	if (_pieceMovesHistoryBackwardsStack.empty()) {
		_currentPlayer = ChessPiece::getOpponentChessPieceColor(_currentPlayer);
	} else {
		// to allow correct history moves manipulation
		_currentPlayer = ChessPiece::getOpponentChessPieceColor(_pieceMovesHistoryBackwardsStack.back()->getPieceMoved()->getChessPieceColor());
	}	

	if (_currentPlayer == WHITE) {
		updatePlayerStatus(_whitePlayerGameTimerText, _blackPlayerGameTimerText);
	} else {
		updatePlayerStatus(_blackPlayerGameTimerText, _whitePlayerGameTimerText);
	}	
}


void ChessBoard::showGameStatus(GameStatus gameStatus) {
	switch (gameStatus) {	
		case WHITE_IN_CHECK: {
			setGameStatusText(GAME_STATUS_TEXT_WHITE_IN_CHECK);
			break;
		}

		case BLACK_IN_CHECK: {
			setGameStatusText(GAME_STATUS_TEXT_BLACK_IN_CHECK);
			break;
		}

		case WHITE_WON: {
			setGameStatusText(GAME_STATUS_TEXT_WHITE_WON);
			break;
		}

		case BLACK_WON: {
			setGameStatusText(GAME_STATUS_TEXT_BLACK_WON);
			break;
		}

		case DRAW: {
			setGameStatusText(GAME_STATUS_TEXT_DRAW);
			break;
		}

		default: {
			break;
		}
	}
}


bool ChessBoard::isGameInProgress() {
	if (_gameStatus == IN_PROGRESS || _gameStatus == WHITE_IN_CHECK || _gameStatus == BLACK_IN_CHECK) {
		return true;
	}

	return false;
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


MatrixTransform* ChessBoard::wrapAndChangeMTPosition(MatrixTransform* mt, Vec3 position, Vec3 scale, float rotationAngle) {
	mt->postMult(Matrix::scale(scale));	

	MatrixTransform* positionMT = new MatrixTransform();
	positionMT->postMult(Matrix::rotate(rotationAngle, osg::Z_AXIS));
	positionMT->postMult(Matrix::translate(position));
	positionMT->addChild(mt);

	return positionMT;	
}


MatrixTransform* ChessBoard::wrapText(Text3D* gameStatusText) {
	MatrixTransform* gameStatusMT = new MatrixTransform();		

	Geode* gameStatusGeode = new Geode();
	gameStatusGeode->addDrawable(gameStatusText);

	gameStatusMT->addChild(gameStatusGeode);	

	return gameStatusMT;
}


void ChessBoard::setGameStatusText(string text) {
	_gameStatusTextWhiteSide->setText(text);
	_gameStatusTextBlackSide->setText(text);

	_gameStatusTextWhiteSideMT->postMult(Matrix::scale(0, 0, 0));
	_gameStatusTextBlackSideMT->postMult(Matrix::scale(0, 0, 0));

	osgAnimation::Motion* scaleEaseMotion = new osgAnimation::OutBackMotion(0, GAME_STATUS_TEXT_SCALE_ANIMATION_DURATION_SECONDS, 1.0);
	osg::AnimationPathCallback* animationPathCallback = new osg::AnimationPathCallback(ChessUtils::createScaleAnimationPath(Vec3(0,0,0), scaleEaseMotion, GAME_STATUS_TEXT_SCALE_ANIMATION_DURATION_SECONDS));
	
	_gameStatusTextWhiteSideMT->setUpdateCallback(animationPathCallback);
	_gameStatusTextBlackSideMT->setUpdateCallback(animationPathCallback);

	_gameStatusTextVisible = true;
}


void ChessBoard::clearGameStatusText() {	
	if (_gameStatusTextVisible) {
		osgAnimation::Motion* scaleEaseMotion = new osgAnimation::OutBackMotion(1.0, GAME_STATUS_TEXT_SCALE_ANIMATION_DURATION_SECONDS, -1.0);
		osg::AnimationPathCallback* animationPathCallback = new osg::AnimationPathCallback(ChessUtils::createScaleAnimationPath(Vec3(0, 0, 0), scaleEaseMotion, GAME_STATUS_TEXT_SCALE_ANIMATION_DURATION_SECONDS));

		_gameStatusTextWhiteSideMT->setUpdateCallback(animationPathCallback);
		_gameStatusTextBlackSideMT->setUpdateCallback(animationPathCallback);

		_gameStatusTextVisible = false;
	}
}


void ChessBoard::setupAISkillLevelSelectors() {
	vector<string> aiSkillLevels;
	aiSkillLevels.push_back("0");
	aiSkillLevels.push_back("2");
	aiSkillLevels.push_back("4");
	aiSkillLevels.push_back("8");
	aiSkillLevels.push_back("12");
	aiSkillLevels.push_back("16");
	aiSkillLevels.push_back("18");
	aiSkillLevels.push_back("20");
	aiSkillLevels.push_back("Skill level");

	_chessEngineSkillLevelSelectorsMTs.clear();
	_chessEngineSkillLevelSelectors->addChild(setupAISkillLevelSelectorsOneSide(aiSkillLevels, BOARD_SQUARE_SIZE, 0.0));
	_chessEngineSkillLevelSelectors->addChild(setupAISkillLevelSelectorsOneSide(aiSkillLevels, BOARD_SQUARE_SIZE, osg::PI));
}


MatrixTransform* ChessBoard::setupAISkillLevelSelectorsOneSide(const vector<string>& skillLevels, int sceneYPositionIncrement, float rotationAngle) {
	MatrixTransform* positionMT = new MatrixTransform();
	positionMT->postMult(Matrix::rotate(rotationAngle, osg::Z_AXIS));		
	
	for (size_t i = 0; i < skillLevels.size(); ++i) {
		float textXPosition;
		float textYPosition;
		if (i == skillLevels.size() - 1) {
			textXPosition = 0;
			textYPosition = -sceneYPositionIncrement * 0.5;
		} else {			
			textXPosition = ((int)i - 4) * BOARD_SQUARE_SIZE + BOARD_SQUARE_SIZE * 0.5;
			textYPosition = -sceneYPositionIncrement * 1.5;
		}

		Text3D* text = ChessUtils::createText3D(skillLevels[i], _font3D, Vec3(textXPosition, textYPosition, PLAYER_STATUS_Z_OFFSET), GAME_STATUS_TEXT_SIZE, GAME_STATUS_TEXT_DEPTH, osgText::TextBase::CENTER_CENTER);
		
		if (i == skillLevels.size() - 1) {
			text->setColor(SKILL_LEVEL_SELECTOR_TITLE_COLOR);
		} else {
			text->setColor(SKILL_LEVEL_SELECTOR_NUMBER_COLOR);
		}
		
		MatrixTransform* wrapedText = wrapText(text);
		wrapedText->postMult(Matrix::scale(Vec3(0, 0, 0)));

		_chessEngineSkillLevelSelectorsMTs.push_back(wrapedText);
		positionMT->addChild(wrapedText);
	}

	return positionMT;
}


void ChessBoard::showAISkillLevelSelectors() {
	for (size_t i = 0; i < _chessEngineSkillLevelSelectorsMTs.size(); ++i) {
		osgAnimation::Motion* scaleEaseMotion = new osgAnimation::OutBackMotion(0, GAME_STATUS_TEXT_SCALE_ANIMATION_DURATION_SECONDS, 1.0);
		osg::AnimationPathCallback* animationPathCallback = new osg::AnimationPathCallback(ChessUtils::createScaleAnimationPath(Vec3(0, 0, 0), scaleEaseMotion, GAME_STATUS_TEXT_SCALE_ANIMATION_DURATION_SECONDS));
		_chessEngineSkillLevelSelectorsMTs[i]->setUpdateCallback(animationPathCallback);
	}
}


void ChessBoard::hideAISkillLevelSelectors() {
	for (size_t i = 0; i < _chessEngineSkillLevelSelectorsMTs.size(); ++i) {
		osgAnimation::Motion* scaleEaseMotion = new osgAnimation::OutBackMotion(1.0, GAME_STATUS_TEXT_SCALE_ANIMATION_DURATION_SECONDS, -1.0);
		osg::AnimationPathCallback* animationPathCallback = new osg::AnimationPathCallback(ChessUtils::createScaleAnimationPath(Vec3(0, 0, 0), scaleEaseMotion, GAME_STATUS_TEXT_SCALE_ANIMATION_DURATION_SECONDS));
		_chessEngineSkillLevelSelectorsMTs[i]->setUpdateCallback(animationPathCallback);
	}
}


bool ChessBoard::manageAISkillLevelSelection(Vec2i selectorBoardPosition) {
	if (selectorBoardPosition.y() == -2) {
		switch (selectorBoardPosition.x()) {
			case -4: { _chessEngineSkillLevel =  0; break; }
			case -3: { _chessEngineSkillLevel =  2; break; }
			case -2: { _chessEngineSkillLevel =  4; break; }
			case -1: { _chessEngineSkillLevel =  8; break; }
			case  1: { _chessEngineSkillLevel = 12; break; }
			case  2: { _chessEngineSkillLevel = 16; break; }
			case  3: { _chessEngineSkillLevel = 18; break; }
			case  4: { _chessEngineSkillLevel = 20; break; }
			default: return false;
		}		
	} else if (selectorBoardPosition.y() == 2) {
		switch (selectorBoardPosition.x()) {
			case -4: { _chessEngineSkillLevel = 20; break; }
			case -3: { _chessEngineSkillLevel = 18; break; }
			case -2: { _chessEngineSkillLevel = 16; break; }
			case -1: { _chessEngineSkillLevel = 12; break; }
			case  1: { _chessEngineSkillLevel =  8; break; }
			case  2: { _chessEngineSkillLevel =  4; break; }
			case  3: { _chessEngineSkillLevel =  2; break; }
			case  4: { _chessEngineSkillLevel =  0; break; }
			default: return false;
		}		
	} else {
		return false;
	}

	_uciProtocol.startNewChessGame(_chessEngineSkillLevel);
	_gameStatus = IN_PROGRESS;
	hideAISkillLevelSelectors();
	_chessEngineSkillLevelSelectionInProgress = false;

	return true;
}


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <chess AI> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
bool ChessBoard::isCurrentPlayerAI() {
	if ((_currentPlayer == WHITE && _whitePlayerIsAI) || (_currentPlayer == BLACK && _blackPlayerIsAI)) {
		return true;
	}

	return false;
}


bool ChessBoard::isOpponentPlayerAI() {
	if ((_currentPlayer == WHITE && _blackPlayerIsAI) || (_currentPlayer == BLACK && _whitePlayerIsAI)) {
		return true;
	}

	return false;
}


string ChessBoard::computeBoardUCIMoves() {
	if (_pieceMovesHistoryBackwardsStack.empty()) {
		return "";
	}
	
	stringstream boardMoves;
	boardMoves << _pieceMovesHistoryBackwardsStack[0]->getUciMove();

	for (size_t i = 1; i < _pieceMovesHistoryBackwardsStack.size(); ++i) {
		boardMoves << " " << _pieceMovesHistoryBackwardsStack[i]->getUciMove();
	}

	return boardMoves.str();
}


void ChessBoard::restartChessEngine(string enginePath) {
	_uciProtocol.startUCIChessEngine(enginePath, _chessEngineSkillLevel, CHESS_ENGINE_LOGFILE);
}

bool ChessBoard::makeChessAIMove() {
	if (!_uciProtocol.isChessEngineReady()) {
		return false;
	}

	// update engine board
	/*if (_pieceMovesHistoryBackwardsStack.empty()) {
		_uciProtocol.startNewChessGame(_engineSkillLevel);
	}*/

	if (!_chessEngineThinking) {
		_chessEngineThinking = true;
		string boardMoves = computeBoardUCIMoves();
		_uciProtocol.setChessEngineBoardPosition(boardMoves);
		_uciProtocol.startEngineMoveSearch(_chessEngineMoveTimeInMilliseconds);
	}

	string chessAIBestMove = _uciProtocol.receiveBestMoveFromChessEngine();

	if (chessAIBestMove == "") {
		// engine still thinking
		return false;
	}

	_chessEngineThinking = false;

	// see if there is a check mate
	if (chessAIBestMove == UCI_BESTMOVE_CHECK_MATE) {
		if (_currentPlayer == WHITE) {
			_gameStatus = BLACK_WON;
		} else {
			_gameStatus = WHITE_WON;
		}

		showGameStatus(_gameStatus);
		return false;
	}

	UCIMove uciMove;
	Vec2iArray* boardMove = uciMove.convertUCIMoveToBoardMove(chessAIBestMove);

	if (boardMove == NULL || boardMove->size() != 2) {
		return false;
	}

	if (uciMove.isMoveWithPromotion()) {
		_rankOfPromotion = uciMove.getPromotionRank();
	}

	_moveOriginPosition = boardMove->at(0);
	Vec2i chessAIMoveFinalPosition = boardMove->at(1);
	_moveDestinationPosition.x() = 0;
	_moveDestinationPosition.y() = 0;

	if (_moveOriginPosition.x() == 0 || _moveOriginPosition.y() == 0 || chessAIMoveFinalPosition.x() == 0 || chessAIMoveFinalPosition.y() == 0) {
		return false;
	}

	_pieceToMove = getChessPieceAtBoardPosition(_moveOriginPosition.x(), _moveOriginPosition.y(), _currentPlayer);
	
	if (_pieceToMove == NULL) {
		return false;
	}	

	_moveDestinationPosition.x() = 0;
	_moveDestinationPosition.y() = 0;
	bool moveResult = processSecondSelection(chessAIMoveFinalPosition, true);


	if (uciMove.isMoveWithPromotion()) {
		_pieceToPromote = _pieceToMove;
		setupPiecePromotion();
	}

	return moveResult;
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </chess AI> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
