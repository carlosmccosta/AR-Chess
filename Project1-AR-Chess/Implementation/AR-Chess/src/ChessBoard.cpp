#include "ChessBoard.h"


ChessBoard::ChessBoard() {
	_paddleSelectorImage = osgDB::readImageFile(PADDLE_SELECTOR);
	_paddleSelectedImage = osgDB::readImageFile(PADDLE_SELECTED);
	_newGameHH = osgDB::readImageFile(NEW_GAME_H_H);
	_newGameHHSelected = osgDB::readImageFile(NEW_GAME_H_H_SELECTED);
	_newGameHM = osgDB::readImageFile(NEW_GAME_H_M);
	_newGameHMSelected = osgDB::readImageFile(NEW_GAME_H_M_SELECTED);

	_font3D = osgText::readFont3DFile(TEXT_FONT);
	_lastSelectorBoardPosition = Vec2i(0, 0);

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


	// new game h vs h selectors
	_boardShadowedScene->addChild(setupAuxiliarySelector(Vec2i(5, -4), _newGameHH));
	_boardShadowedScene->addChild(setupAuxiliarySelector(Vec2i(-5, 4), _newGameHH, Vec3(-AUXILIARY_SELECTORS_X_OFFSET, -AUXILIARY_SELECTORS_Y_OFFSET, 0), osg::PI));
	_boardShadowedScene->addChild(setupAuxiliarySelector(Vec2i(6, -4), _newGameHM));
	_boardShadowedScene->addChild(setupAuxiliarySelector(Vec2i(-6, 4), _newGameHM, Vec3(-AUXILIARY_SELECTORS_X_OFFSET, -AUXILIARY_SELECTORS_Y_OFFSET, 0), osg::PI));

	_playersPieces = new Group();
	_playersTimers = new Group();
	_boardSquareSelectorHighlights = new Group();
	_boardSquareSelections = new Group();	
	
	setupPlayersTimers();

	_boardShadowedScene->addChild(boardModelMT);
	_boardShadowedScene->addChild(_playersPieces);
	_boardShadowedScene->addChild(_playersTimers);
	_boardShadowedScene->addChild(_boardSquareSelectorHighlights);
	_boardShadowedScene->addChild(_boardSquareSelections);	
}

ChessBoard::~ChessBoard() {}


osgShadow::ShadowedScene* ChessBoard::setupBoard() {
	// reset board
	_whiteChessPieces.clear();
	_blackChessPieces.clear();
	clearPlayersPieces();

	_currentPlayer = WHITE;
	_moveOriginPosition = Vec2(0, 0);
	_moveDestinationPosition = Vec2(0, 0);
	
	_selectorTimer = new ElapsedTime();
	_whitePlayerGameTimer = NULL;
	_blackPlayerGameTimer = NULL;	
	_animationInProgress = false;
	_animationDelayBetweenMoves = new ElapsedTime();
	_whitePlayerGameTimerD = 0;
	_blackPlayerGameTimerD = 0;

	updatePlayerStatus(_whitePlayerGameTimerText, _blackPlayerGameTimerText);
	
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

	return _boardShadowedScene;
}


void ChessBoard::setupPlayersTimers() {
	float playersStatusXOffsetToBoardBorder = 4 * BOARD_SQUARE_SIZE + PLAYER_STATUS_BOARD_OFFSET;
	_whitePlayerStatus = ChessUtils::createRectangleWithTexture(
		Vec3(-PLAYER_STATUS_TEXT_LEFT_OFFSET, -PLAYER_STATUS_TEXT_UP_OFFSET, BOARD_OVERLAYS_HEIGHT_OFFSET),
		osgDB::readImageFile(WHITE_PLAYER_STATUS_IMG), PLAYER_STATUS_WIDTH, PLAYER_STATUS_HEIGHT);

	_blackPlayerStatus = ChessUtils::createRectangleWithTexture(
		Vec3(-PLAYER_STATUS_TEXT_LEFT_OFFSET, -PLAYER_STATUS_TEXT_UP_OFFSET, BOARD_OVERLAYS_HEIGHT_OFFSET),
		osgDB::readImageFile(BLACK_PLAYER_STATUS_IMG), PLAYER_STATUS_WIDTH, PLAYER_STATUS_HEIGHT);

	_whitePlayerGameTimerText = ChessUtils::createText3D(ChessUtils::formatSecondsToDate(0), _font3D);
	_blackPlayerGameTimerText = ChessUtils::createText3D(ChessUtils::formatSecondsToDate(0), _font3D);

	setupPlayerTimer(_whitePlayerStatus, _whitePlayerGameTimerText, osg::PI_2, -playersStatusXOffsetToBoardBorder);
	setupPlayerTimer(_blackPlayerStatus, _blackPlayerGameTimerText, -osg::PI_2, playersStatusXOffsetToBoardBorder);
}


void ChessBoard::setupPlayerTimer(Geode* backgroundImage, Text3D* timerText, float rotationAngle, float playersStatusXOffsetToBoardBorder) {
	MatrixTransform* playerTimerMT = new MatrixTransform();	
	playerTimerMT->addChild(backgroundImage);
	
	Geode* timerTextGeode = new Geode();
	timerTextGeode->addDrawable(timerText);		

	playerTimerMT->addChild(timerTextGeode);
	playerTimerMT->postMult(Matrix::rotate(rotationAngle, osg::Z_AXIS));
	playerTimerMT->postMult(Matrix::translate(playersStatusXOffsetToBoardBorder, 0, 0));
	_playersTimers->addChild(playerTimerMT);
}


MatrixTransform* ChessBoard::setupAuxiliarySelector(Vec2i position, Image* image, Vec3 translateOffset, float rotationAngle, Vec3 scale) {
	Vec3f newGameHHTopScenePosition = ChessUtils::computePieceScenePosition(position.x(), position.y());
	newGameHHTopScenePosition.x() += translateOffset.x();
	newGameHHTopScenePosition.y() += translateOffset.y();
	newGameHHTopScenePosition.z() = BOARD_OVERLAYS_HEIGHT_OFFSET + translateOffset.z();

	Geode* newGameHHTopSceneGeode = ChessUtils::createRectangleWithTexture(Vec3(0, 0, 0), new Image(*image));
	MatrixTransform* newGameHHTopSceneMT = new MatrixTransform();
	newGameHHTopSceneMT->addChild(newGameHHTopSceneGeode);
	newGameHHTopSceneMT->postMult(Matrix::rotate(rotationAngle, osg::Z_AXIS));
	newGameHHTopSceneMT->postMult(Matrix::scale(scale));
	newGameHHTopSceneMT->postMult(Matrix::translate(newGameHHTopScenePosition));
	
	return newGameHHTopSceneMT;
}


bool ChessBoard::updateBoard(Vec2i selectorBoardPosition) {
	if (_animationInProgress) {
		if (_animationDelayBetweenMoves->elapsedTime() < PIECE_MOVE_MAX_ANIMATION_DURATION_SECONDS) {
			return false;
		} else {
			_animationInProgress = false;
			clearSelections();
			_currentPlayer = ChessPiece::getOpponentChessPieceColor(_currentPlayer);

			if (_currentPlayer == WHITE) {
				updatePlayerStatus(_whitePlayerGameTimerText, _blackPlayerGameTimerText);
			} else {
				updatePlayerStatus(_blackPlayerGameTimerText, _whitePlayerGameTimerText);
			}
		}
	}

	bool positionIsAnAuxiliarySelector = isPositionAnAuxiliarySelector(selectorBoardPosition);

	if ((isPositionValid(selectorBoardPosition.x()) && isPositionValid(selectorBoardPosition.y())) || positionIsAnAuxiliarySelector) {
		// start timers when player moves paddle inside board for first time in the game
		if (_currentPlayer == WHITE) {			
			if (_whitePlayerGameTimer == NULL) {
				_whitePlayerGameTimer = new ElapsedTime();
			}

			_whitePlayerGameTimerText->setText(ChessUtils::formatSecondsToDate(_whitePlayerGameTimerD + _whitePlayerGameTimer->elapsedTime()));
		}

		if (_currentPlayer == BLACK) {
			if (_blackPlayerGameTimer == NULL) {
				_blackPlayerGameTimer = new ElapsedTime();
			}

			_blackPlayerGameTimerText->setText(ChessUtils::formatSecondsToDate(_blackPlayerGameTimerD + _blackPlayerGameTimer->elapsedTime()));
		}



		if (selectorBoardPosition.x() != _lastSelectorBoardPosition.x() || selectorBoardPosition.y() != _lastSelectorBoardPosition.y()) {
			// selector in new board position
			clearHighlights();
			_selectorTimer->reset();
			
			_lastSelectorBoardPosition.x() = selectorBoardPosition.x();
			_lastSelectorBoardPosition.y() = selectorBoardPosition.y();

			return hightLighPosition(selectorBoardPosition.x(), selectorBoardPosition.y());
		} else {
			if (_selectorTimer->elapsedTime_m() > PADDLE_TIME_TO_SELECT_POSITION) {
				if (positionIsAnAuxiliarySelector) {
					if ((selectorBoardPosition.x() == 5 && selectorBoardPosition.y() == -4) ||
						(selectorBoardPosition.x() == -5 && selectorBoardPosition.y() == 4)) {
						setupBoard();
						clearSelections();
					}					
					_selectorTimer->reset();
					return true;
				}


				// first selection (0 is an invalid position)
				if (_moveOriginPosition.x() == 0) {	
					_pieceToMove = selectPosition(selectorBoardPosition.x(), selectorBoardPosition.y(), _currentPlayer, true);
					
					// mark origin position if a valid piece was selected
					if (_pieceToMove != NULL) {
						_moveOriginPosition.x() = selectorBoardPosition.x();
						_moveOriginPosition.y() = selectorBoardPosition.y();
						clearHighlights();
						_selectorTimer->reset();
						return true;
					}
				} else {					
					MovePositionStatus movePositionStatus = isPositionAvailableToReceiveMove(selectorBoardPosition.x(), selectorBoardPosition.y(), _currentPlayer);

					if (movePositionStatus == SAME_POSITION_AS_ORIGIN) {
						_moveOriginPosition.x() = 0;
						_moveOriginPosition.y() = 0;				
						clearSelections();				
						hightLighPosition(selectorBoardPosition.x(), selectorBoardPosition.y());
						_selectorTimer->reset();
						return false;
					}

					if (movePositionStatus == POSITION_WITH_PLAYER_PIECE) {
						_selectorTimer->reset();
						return false;
					}

					// second selection (if not defined yet)
					if (_moveDestinationPosition.x() == 0 && (movePositionStatus == POSITION_AVAILABLE || movePositionStatus == POSITION_WITH_OPONENT_PIECE)) {
						_moveDestinationPosition.x() = selectorBoardPosition.x();
						_moveDestinationPosition.y() = selectorBoardPosition.y();

						// remove opponent piece if necessary
						if (movePositionStatus == POSITION_WITH_OPONENT_PIECE) {
							removeChessPieceWithAnimation(_moveDestinationPosition, _currentPlayer);
						}
						
						// animate piece movement to final destination
						clearHighlights();
						clearSelections();
						//selectPosition(selectorBoardPosition.x(), selectorBoardPosition.y(), _currentPlayer);
						moveChessPieceWithAnimation(_pieceToMove, _moveDestinationPosition);
																
						if (_currentPlayer == WHITE) {
							_whitePlayerGameTimerD += _whitePlayerGameTimer->elapsedTime();
							_whitePlayerGameTimer->finish();
							_whitePlayerGameTimer = NULL;																				
						} else {
							_blackPlayerGameTimerD += _blackPlayerGameTimer->elapsedTime();
							_blackPlayerGameTimer->finish();
							_blackPlayerGameTimer = NULL;
						}
						
						_selectorTimer->reset();
						_animationDelayBetweenMoves->reset();
						_animationInProgress = true;

						return true;
					}
				}

				_selectorTimer->reset();
			}
		}
	} else {
		// selector outside board bounds
		clearHighlights();
	}
	
	return false;
}


void ChessBoard::updatePlayerStatus(Text3D* makeActive, Text3D* makeInactive) {
	makeActive->setColor(Vec4(0.3, 0.5, 0.3, 0.8));
	makeInactive->setColor(Vec4(0.4, 0.4, 0.4, 0.8));	
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


bool ChessBoard::hightLighPosition(int xBoardPosition, int yBoardPosition) {
	bool positionAnAuxiliarySelector = isPositionAnAuxiliarySelector(Vec2i(xBoardPosition, yBoardPosition));
	if ((!isPositionValid(xBoardPosition) || !isPositionValid(yBoardPosition)) && !positionAnAuxiliarySelector) {
		return false;
	}

	if (positionAnAuxiliarySelector) {
		// new game h vs h selectors
		if (xBoardPosition == 5 && yBoardPosition == -4) {
			_boardSquareSelectorHighlights->addChild(setupAuxiliarySelector(Vec2i(5, -4), _newGameHHSelected, Vec3(AUXILIARY_SELECTORS_X_OFFSET, AUXILIARY_SELECTORS_Y_OFFSET, BOARD_OVERLAYS_HEIGHT_OFFSET)));			
		}

		if (xBoardPosition == -5 && yBoardPosition == 4) {
			_boardSquareSelectorHighlights->addChild(setupAuxiliarySelector(Vec2i(-5, 4), _newGameHHSelected, Vec3(-AUXILIARY_SELECTORS_X_OFFSET, -AUXILIARY_SELECTORS_Y_OFFSET, BOARD_OVERLAYS_HEIGHT_OFFSET), osg::PI));			
		}

		if (xBoardPosition == 6 && yBoardPosition == -4) {
			_boardSquareSelectorHighlights->addChild(setupAuxiliarySelector(Vec2i(6, -4), _newGameHMSelected, Vec3(AUXILIARY_SELECTORS_X_OFFSET, AUXILIARY_SELECTORS_Y_OFFSET, BOARD_OVERLAYS_HEIGHT_OFFSET)));			
		}

		if (xBoardPosition == -6 && yBoardPosition == 4) {
			_boardSquareSelectorHighlights->addChild(setupAuxiliarySelector(Vec2i(-6, 4), _newGameHMSelected, Vec3(-AUXILIARY_SELECTORS_X_OFFSET, -AUXILIARY_SELECTORS_Y_OFFSET, BOARD_OVERLAYS_HEIGHT_OFFSET), osg::PI));			
		}

		return true;
	}


	Vec3f scenePosition = ChessUtils::computePieceScenePosition(xBoardPosition, yBoardPosition);
	scenePosition.z() = BOARD_OVERLAYS_HEIGHT_OFFSET;
	_boardSquareSelectorHighlights->addChild(ChessUtils::createRectangleWithTexture(scenePosition, new Image(*_paddleSelectorImage)));
	return true;
}


ChessPiece* ChessBoard::selectPosition(int xBoardPosition, int yBoardPosition, ChessPieceColor chessPieceColor, bool selectOnlyIfExistsPiece) {
	if ((!isPositionValid(xBoardPosition) || !isPositionValid(yBoardPosition)) && !isPositionAnAuxiliarySelector(Vec2i(xBoardPosition, yBoardPosition))) {
		return NULL;
	}

	ChessPiece* chessPieceAtPosition = getChessPieceAtBoardPosition(xBoardPosition, yBoardPosition, chessPieceColor);
	if (!selectOnlyIfExistsPiece || (selectOnlyIfExistsPiece && chessPieceAtPosition != NULL)) {
		Vec3f scenePosition = ChessUtils::computePieceScenePosition(xBoardPosition, yBoardPosition);
		scenePosition.z() = BOARD_OVERLAYS_HEIGHT_OFFSET;
		_boardSquareSelections->addChild(ChessUtils::createRectangleWithTexture(scenePosition, new Image(*_paddleSelectedImage)));
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
			return POSITION_WITH_OPONENT_PIECE;
		}

		return POSITION_AVAILABLE;
	} else {
		return SAME_POSITION_AS_ORIGIN;
	}
}



void ChessBoard::moveChessPieceWithAnimation(ChessPiece* chessPiece, Vec2 finalPosition) {
	// reset positions to next piece move
	_moveOriginPosition.x() = 0;
	_moveOriginPosition.y() = 0;
	_moveDestinationPosition.x() = 0;
	_moveDestinationPosition.y() = 0;
	
	chessPiece->changePosition(finalPosition.x(), finalPosition.y());
}

void ChessBoard::removeChessPieceWithAnimation(Vec2 boardPositionOfPieceToRemove, ChessPieceColor currentPlayerChessPieceColor) {
	getChessPieceAtBoardPosition(boardPositionOfPieceToRemove.x(), boardPositionOfPieceToRemove.y(), ChessPiece::getOpponentChessPieceColor(currentPlayerChessPieceColor))->removePieceFromBoard();
}


bool ChessBoard::isPositionValid(int position) {
	if ((position > -5 && position < 0) || (position > 0 && position < 5)) {
		return true;
	}
	else {
		return false;
	}
}


bool ChessBoard::isPositionAnAuxiliarySelector(Vec2i position) {
	if ((position.y() == -4 || position.y() == 4) &&
		((position.x() > -7 && position.x() < -4) || (position.x() > 4 && position.x() < 7))) {
		return true;
	}
	else {
		return false;
	}
}


ChessPiece* ChessBoard::getChessPieceAtBoardPosition(int xBoardPosition, int yBoardPosition, ChessPieceColor chessPieceColor) {
	if (!isPositionValid(xBoardPosition) || !isPositionValid(yBoardPosition)) {
		return NULL;
	}
	
	if (chessPieceColor == WHITE) {
		for (size_t i = 0; i < _whiteChessPieces.size(); ++i) {
			if (_whiteChessPieces[i]->getXPosition() == xBoardPosition && _whiteChessPieces[i]->getYPosition() == yBoardPosition) {
				return _whiteChessPieces[i];
			}
		}
	} else {
		for (size_t i = 0; i < _blackChessPieces.size(); ++i) {
			if (_blackChessPieces[i]->getXPosition() == xBoardPosition && _blackChessPieces[i]->getYPosition() == yBoardPosition) {
				return _blackChessPieces[i];
			}
		}
	}	

	return NULL;
}

