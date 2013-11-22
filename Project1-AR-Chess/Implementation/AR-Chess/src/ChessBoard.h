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

// project includes
#include "ChessUtils.h"
#include "ChessPiece.h"
#include "ModelsConfigs.h"


// namespace specific imports to avoid namespace pollution
using std::vector;
using osg::MatrixTransform;
using osg::Vec2i;
using osg::Vec2Array;
using osg::Vec3;
using osg::Vec3f;
using osg::Image;
using osg::ElapsedTime;
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


enum MovePositionStatus {
	POSITION_AVAILABLE,
	POSITION_WITH_OPONENT_PIECE,
	POSITION_WITH_PLAYER_PIECE,
	SAME_POSITION_AS_ORIGIN
};


class ChessBoard : public osg::Referenced {
	public:
		ChessBoard();
		virtual ~ChessBoard();
		osgShadow::ShadowedScene* setupBoard();
		osgShadow::ShadowedScene* resetBoard();
		void setupBoardPieces();
		void setupPlayersTimers();
		void setupPlayerTimer(Geode* backgroundImage, Text3D* timerText, float rotationAngle, Vec3 playersStatusOffsetToBoardBorder);
		MatrixTransform* setupAuxiliarySelector(Vec2i position, Image* image,
			Vec3 translateOffset = Vec3(AUXILIARY_SELECTORS_X_OFFSET, AUXILIARY_SELECTORS_Y_OFFSET, 0),
			float rotationAngle = 0,
			Vec3 scale = Vec3(AUXILIARY_SELECTORS_IMAGE_SCALE, AUXILIARY_SELECTORS_IMAGE_SCALE, AUXILIARY_SELECTORS_IMAGE_SCALE));

		bool updateBoard(Vec2i selectorBoardPosition);
		void updatePlayerStatus(Text3D* makeActive, Text3D* makeInactive);
		void clearPlayersPieces();
		void clearHighlights();
		void clearSelections();
		void clearPossibleMoves();

		bool hightLighPosition(int xBoardPosition, int yBoardPosition);		
		ChessPiece* selectPosition(int xBoardPosition, int yBoardPosition, ChessPieceColor chessPieceColor, bool selectOnlyIfExistsPiece = false);
		bool showPossibleMoves(ChessPiece* chessPiece);
		Vec2Array* computePossibleMovePositions(ChessPiece* chessPiece);
		bool updatePossibleMoves(int xBoardPosition, int yBoardPosition, ChessPieceColor chessPieceColor, Vec2Array* possibleMoves);		
		MovePositionStatus isPositionAvailableToReceiveMove(int xBoardPosition, int yBoardPosition, ChessPieceColor currentPlayer);

		void moveChessPieceWithAnimation(ChessPiece* chessPiece, Vec2 finalPosition);
		void removeChessPieceWithAnimation(Vec2 boardPositionOfPieceToRemove, ChessPieceColor chessPieceColor);

		bool isPositionValid(int position);
		bool isPositionAnAuxiliarySelector(Vec2i position);
		ChessPiece* getChessPieceAtBoardPosition(int xBoardPosition, int yBoardPosition, ChessPieceColor chessPieceColor);


	private:
		vector<ChessPiece*> _whiteChessPieces;
		vector<ChessPiece*> _blackChessPieces;
		ChessPieceColor _currentPlayer;
		ChessPiece* _pieceToMove;
		Vec2 _moveOriginPosition;
		Vec2 _moveDestinationPosition;

		osgShadow::ShadowedScene* _boardShadowedScene;
		Material* _boardMaterial;
		Material* _whitePiecesMaterial;
		Material* _blackPiecesMaterial;
		Image* _paddleSelectorImage;
		Image* _paddleSelectedImage;
		Vec2i _lastSelectorBoardPosition;
		Font3D* _font3D;
		
		Group* _playersPieces;
		Group* _playersTimers;
		Group* _boardSquareSelectorHighlights;
		Group* _boardSquareSelections;
		Group* _boardSquarePossibleMoves;

		ElapsedTime* _selectorTimer;
		ElapsedTime* _animationDelayBetweenMoves;
		bool _animationInProgress;

		ElapsedTime* _whitePlayerGameTimer;
		double _whitePlayerGameTimerD;
		ElapsedTime* _blackPlayerGameTimer;
		double _blackPlayerGameTimerD;
		Geode* _whitePlayerStatus;
		Geode* _blackPlayerStatus;
		Text3D* _whitePlayerGameTimerText;		
		Text3D* _blackPlayerGameTimerText;

		Image* _newGameHH;
		Image* _newGameHHSelected;
		Image* _newGameHM;
		Image* _newGameHMSelected;
};

