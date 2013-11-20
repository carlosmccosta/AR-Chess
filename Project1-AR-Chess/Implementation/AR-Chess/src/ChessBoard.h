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

		bool updateBoard(Vec2i selectorBoardPosition);
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
		
		Group* _boardSquareSelectorHighlights;
		Group* _boardSquareSelections;
		Group* _boardSquarePossibleMoves;

		ElapsedTime* _selectorTimer;
};

