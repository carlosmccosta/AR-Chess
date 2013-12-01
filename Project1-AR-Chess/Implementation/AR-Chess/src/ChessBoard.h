#pragma once

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// OSG includes
#include <osg/MatrixTransform>
#include <osg/Image>
#include <osg/TexEnv>
#include <osgDB/ReadFile>
#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowMap>

// std includes
#include <vector>
#include <ios>
#include <fstream>
#include <string>
#include <sstream>

// project includes
#include "ChessUtils.h"
#include "ChessPiece.h"
#include "Configs.h"
#include "ChessMoveHistory.h"

#include "UCIMove.h"
#include "UCIProtocol.h"

// namespace specific imports to avoid namespace pollution
using std::vector;
using std::string;
using std::stringstream;
using osg::MatrixTransform;
using osg::Vec2i;
using osg::Vec2iArray;
using osg::Vec3;
using osg::Vec3f;
using osg::Vec4Array;
using osg::Image;
using osg::ElapsedTime;
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

enum GameStatus {
	IN_PROGRESS,
	WHITE_IN_CHECK,
	BLACK_IN_CHECK,
	DRAW,
	WHITE_WON,
	BLACK_WON
};


enum MovePositionStatus {
	POSITION_AVAILABLE,
	POSITION_WITH_OPPONENT_PIECE,
	POSITION_WITH_PLAYER_PIECE,
	SAME_POSITION_AS_ORIGIN,
	POSITION_INVALID
};

enum AuxiliarySelector {
	SELECTOR_NEW_GAME_H_H_WHITE_SIDE,
	SELECTOR_NEW_GAME_H_H_BLACK_SIDE,
	SELECTOR_NEW_GAME_H_C_WHITE_SIDE,
	SELECTOR_NEW_GAME_H_C_BLACK_SIDE,
	SELECTOR_NEW_GAME_C_C_WHITE_SIDE,
	SELECTOR_NEW_GAME_C_C_BLACK_SIDE,
	SELECTOR_PREVIOUS_MOVE_WHITE_SIDE,
	SELECTOR_PREVIOUS_MOVE_BLACK_SIDE,
	SELECTOR_NEXT_MOVE_WHITE_SIDE,
	SELECTOR_NEXT_MOVE_BLACK_SIDE,
	SELECTOR_PROMOTE_TO_QUEEN_WHITE_SIDE,
	SELECTOR_PROMOTE_TO_QUEEN_BLACK_SIDE,
	SELECTOR_PROMOTE_TO_ROOK_WHITE_SIDE,
	SELECTOR_PROMOTE_TO_ROOK_BLACK_SIDE,
	SELECTOR_PROMOTE_TO_KNIGHT_WHITE_SIDE,
	SELECTOR_PROMOTE_TO_KNIGHT_BLACK_SIDE,
	SELECTOR_PROMOTE_TO_BISHOP_WHITE_SIDE,
	SELECTOR_PROMOTE_TO_BISHOP_BLACK_SIDE,
	SELECTOR_INVALID
};


class ChessBoard : public osg::Referenced {
	public:
		ChessBoard();
		virtual ~ChessBoard();

		void setupImages();
		void setupMaterials();
		void setupNewGameSelectors();
		void setupGameHistoryManipulators();

		osgShadow::ShadowedScene* setupBoard();
		osgShadow::ShadowedScene* resetBoard();
		void setupBoardPieces();
		void setupPlayersTimers();
		void setupPlayerTimer(Geode* backgroundImage, Text3D* timerText, float rotationAngle, Vec3 playersStatusOffsetToBoardBorder);
		MatrixTransform* setupAuxiliarySelector(Vec2i position, Image* image,
			Vec3 translateOffset = Vec3(AUXILIARY_SELECTORS_X_OFFSET, AUXILIARY_SELECTORS_Y_OFFSET, 0),
			float rotationAngle = 0,
			Vec3 scale = Vec3(NEW_GAME_IMGS_SCALE, NEW_GAME_IMGS_SCALE, NEW_GAME_IMGS_SCALE));		

		bool updateBoard(Vec2i selectorBoardPosition);						
		void updatePlayersGameTimers(bool forceUpdate = false);
		void updatePlayersCumulativeGameTimers();
		string getPlayerGameTimeFormated(double timeInSeconds, ChessPieceColor playerColor);
		void processAuxiliarySelector(AuxiliarySelector auxiliarySelector);
		void setupPiecePromotion();
		bool processFirstSelection(Vec2i selectorBoardPosition);
		bool processSecondSelection(Vec2i selectorBoardPosition, bool playerIsAI = false);

		void updatePlayerStatus(Text3D* makeActive, Text3D* makeInactive);
		void clearPlayersPieces();
		void clearHighlights();
		void clearSelections();
		void clearPossibleMoves();
		void clearPromotionPieces();

		bool hightLighPosition(int xBoardPosition, int yBoardPosition, AuxiliarySelector auxiliarySelector = SELECTOR_INVALID);
		bool hightLighPossibleMove(int xBoardPosition, int yBoardPosition);
		ChessPiece* selectPosition(int xBoardPosition, int yBoardPosition, ChessPieceColor chessPieceColor, bool selectOnlyIfExistsPiece = false, AuxiliarySelector auxiliarySelector = SELECTOR_INVALID);
		bool showPossibleMoves(ChessPiece* chessPiece);
		Vec2iArray* computePossibleMovePositions(ChessPiece* chessPiece);
		bool updateKingPossibleMoves(Vec2i initialPosition, Vec2i finalPosition, ChessPieceColor chessPieceColor, Vec2iArray* possibleMoves);
		bool updateKnightPossibleMoves(Vec2i initialPosition, Vec2i finalPosition, ChessPieceColor chessPieceColor, Vec2iArray* possibleMoves);
		ChessPiece* updatePossibleMovesAlongLine(Vec2i initialPosition, int xIncrement, int yIncrement, ChessPieceColor chessPieceColor, Vec2iArray* possibleMoves);
		MovePositionStatus isPositionAvailableToReceiveMove(Vec2i initialPosition, Vec2i finalPosition, ChessPieceColor currentPlayer);

		void moveChessPieceWithAnimation(ChessPiece* chessPieceToMove, Vec2i finalPosition);
		ChessPiece* removeChessPieceWithAnimation(Vec2i boardPositionOfPieceToRemove, ChessPieceColor chessPieceColor);
		bool checkAndPerformCastling(ChessPiece* chessPieceToMove, Vec2i finalPosition);
		bool isCastlingPossible(ChessPiece* king, Vec2i kingFinalPosition);
		ChessPiece*  isEnPassantPawnCapturePossible(ChessPiece* chessPieceToMove, Vec2i finalPosition, ChessPieceColor currentPlayerPieceColor);
		bool checkAndPerformPromotion(ChessPiece* chessPieceMoved, Vec2i finalPosition);
		void setupPromotionPiecesOnBoad(ChessPieceColor chessPieceColor);
		void managePromotionConversionAndReversion();
		void removePromotionPiecesOnBoad();

		bool isKingInCheck(Vec2i kingPosition, ChessPieceColor kingColor);
		bool isKingInCheckMate(Vec2i kingPosition, ChessPieceColor kingColor);

		bool goToPreviousMoveInHistory();
		bool goToNextMoveInHistory();
		void updateHistoryManipulatorsVisibility();

		void switchPlayer();
		void showGameStatus(GameStatus gameStatus);
		bool isGameInProgress();

		bool isPositionValid(int position);
		AuxiliarySelector isPositionAnAuxiliarySelector(Vec2i position);
		ChessPiece* getChessPieceAtBoardPosition(int xBoardPosition, int yBoardPosition, ChessPieceColor chessPieceColor);	

		void setupGameStatusText(Text3D* gameStatusText, float rotationAngle);
		void setGameStatusText(string text);
		void clearGameStatusText();

		// chess AI
		bool isCurrentPlayerAI();
		bool isOpponentPlayerAI();
		string computeBoardUCIMoves();
		void restartChessEngine(string enginePath = CHESS_ENGINE_FILE_PATH);
		bool makeChessAIMove();


	private:
		GameStatus _gameStatus;
		Text3D* _gameStatusTextWhiteSide;
		Text3D* _gameStatusTextBlackSide;
		vector<ChessPiece*> _whiteChessPieces;
		vector<ChessPiece*> _blackChessPieces;
		vector<ChessPiece*> _promotionChessPieces;
		vector<ChessMoveHistory*> _pieceMovesHistoryBackwardsStack;
		vector<ChessMoveHistory*> _pieceMovesHistoryFowardStack;
		ChessPieceColor _currentPlayer;
		ChessPiece* _pieceToMove;
		ChessPiece* _pieceKingWhite;
		ChessPiece* _pieceKingBlack;

		ChessPiece* _pieceToPromote;
		ChessPieceType _rankOfPromotion;
		ElapsedTime* _promotionConversionTimer;
		
		Vec2i _moveOriginPosition;
		Vec2i _moveDestinationPosition;
		int _currentPlayNumber;

		osgShadow::ShadowedScene* _boardShadowedScene;
		Material* _boardMaterial;
		Material* _whitePiecesMaterial;
		Material* _blackPiecesMaterial;
		Image* _paddleSelectorImg;
		Image* _paddleSelectedImg;
		Image* _possibleMoveImg;
		Vec2i _lastSelectorBoardPosition;
		Font3D* _font3D;
		
		Group* _playersPieces;
		Group* _playersTimers;
		Group* _boardSquareSelectorHighlights;
		Group* _boardSquareSelections;
		Group* _boardSquarePossibleMoves;
		Group* _promotionPieces;

		ElapsedTime* _selectorTimer;
		ElapsedTime* _animationDelayBetweenMoves;
		bool _boardReseting;
		bool _animationInProgress;
		bool _piecePromotionInProgress;
		bool _piecePromotionInProgressAborted;
		bool _piecePromotionConversionInProgress;
		bool _piecePromotionConversionFromHistoryInProgress;
		double _timeToWaitForPromotingPieceFromHistory;
		bool _piecePromotionReversionInProgress;

		ElapsedTime* _whitePlayerGameTimer;
		double _whitePlayerGameTimerD;
		ElapsedTime* _blackPlayerGameTimer;
		double _blackPlayerGameTimerD;
		Geode* _whitePlayerStatus;
		Geode* _blackPlayerStatus;
		Text3D* _whitePlayerGameTimerText;
		Text3D* _blackPlayerGameTimerText;
		Geometry* _moveBackwardsInHistorySelectorWhiteSide;
		Geometry* _moveBackwardsInHistorySelectorBlackSide;
		Geometry* _moveFowardInHistorySelectorWhiteSide;
		Geometry* _moveFowardInHistorySelectorBlackSide;		

		Vec2iArray* _pieceSelectedPossibleMoves;

		Image* _newGameHHImg;
		Image* _newGameHHSelectedImg;
		Image* _newGameHCImg;
		Image* _newGameHCSelectedImg;
		Image* _newGameCCImg;
		Image* _newGameCCSelectedImg;
		Image* _previousMoveImg;
		Image* _previousMoveSelectedImg;
		Image* _nextMoveImg;
		Image* _nextMoveSelectedImg;

		// chess AI
		//chessinterfaceengine* _chessEngine;
		UCIProtocol _uciProtocol;		
		bool _whitePlayerIsAI;
		bool _blackPlayerIsAI;
		int _engineSkillLevel;
};

