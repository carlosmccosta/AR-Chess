#pragma once

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// OSG includes
#include <osg/Vec2i>
#include <osg/Array>
#include <osg/Referenced>

// std includes
#include <string>
#include <sstream>


// project includes
#include "ChessPiece.h"

// namespace specific imports to avoid namespace pollution
using std::string;
using std::stringstream;
using osg::Vec2i;
using osg::Vec2iArray;
using osg::Referenced;
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


class UCIMove : public Referenced {
	public:
		UCIMove();
		virtual ~UCIMove();

		Vec2iArray* convertUCIMoveToBoardMove(string uciPosition);

		static string convertBoardMoveToUCIMove(Vec2i pieceMovedOriginPosition, Vec2i pieceMovedDestinationPosition);
		static string convertBoardPositionToUCIPosition(Vec2i piecePosition);		
		static Vec2i convertUCIPositionToBoardPosition(string uciPosition);
		static ChessPieceType getRankOfPromotion(char uciRankChar);


		// ------------------------------------------------------------------------------  <gets | sets> -------------------------------------------------------------------------------
		bool isMoveWithPromotion() const { return _moveWithPromotion; }
		ChessPieceType getPromotionRank() const { return _promotionRank; }
		// ------------------------------------------------------------------------------  </gets | sets> ------------------------------------------------------------------------------

	private:
		bool _moveWithPromotion;		
		ChessPieceType _promotionRank;		
};

