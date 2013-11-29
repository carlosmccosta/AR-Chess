#include "UCIMove.h"


UCIMove::UCIMove() : _moveWithPromotion(false), _promotionRank(PAWN) {}
UCIMove::~UCIMove() {}



string UCIMove::convertBoardMoveToUCIMove(Vec2i pieceMovedOriginPosition, Vec2i pieceMovedDestinationPosition) {
	return convertBoardPositionToUCIPosition(pieceMovedOriginPosition) + convertBoardPositionToUCIPosition(pieceMovedDestinationPosition);
}


string UCIMove::convertBoardPositionToUCIPosition(Vec2i piecePosition) {
	stringstream uciPosition;

	switch (piecePosition.x()) {
		case -4: { uciPosition << "a"; break; }
		case -3: { uciPosition << "b"; break; }
		case -2: { uciPosition << "c"; break; }
		case -1: { uciPosition << "d"; break; }
		case  1: { uciPosition << "e"; break; }
		case  2: { uciPosition << "f"; break; }
		case  3: { uciPosition << "g"; break; }
		case  4: { uciPosition << "h"; break; }
		default: { uciPosition << "a"; break; }
	}

	switch (piecePosition.y()) {
		case -4: { uciPosition << "1"; break; }
		case -3: { uciPosition << "2"; break; }
		case -2: { uciPosition << "3"; break; }
		case -1: { uciPosition << "4"; break; }
		case  1: { uciPosition << "5"; break; }
		case  2: { uciPosition << "6"; break; }
		case  3: { uciPosition << "7"; break; }
		case  4: { uciPosition << "8"; break; }
		default: { uciPosition << "1"; break; }
	}

	return uciPosition.str();
}


Vec2iArray* UCIMove::convertUCIMoveToBoardMove(string uciPosition) {
	if (uciPosition.size() < 4) {
		return NULL;
	}

	Vec2iArray* boardPosition = new Vec2iArray();
	boardPosition->push_back(convertUCIPositionToBoardPosition(uciPosition.substr(0, 2)));
	boardPosition->push_back(convertUCIPositionToBoardPosition(uciPosition.substr(2, 2)));

	if (uciPosition.size() > 4) {
		// piece promotion
		_promotionRank = getRankOfPromotion(uciPosition[4]);

		if (_promotionRank != PAWN && _promotionRank != KING) {
			_moveWithPromotion = true;
		} else {
			_moveWithPromotion = false;
		}
	}

	return boardPosition;
}


Vec2i UCIMove::convertUCIPositionToBoardPosition(string uciPosition) {
	Vec2i boardPosition(0, 0);

	if (uciPosition.size() != 2) {
		return boardPosition;
	}

	switch (uciPosition[0]) {
		case 'a': { boardPosition.x() = -4; break; }
		case 'b': { boardPosition.x() = -3; break; }
		case 'c': { boardPosition.x() = -2; break; }
		case 'd': { boardPosition.x() = -1; break; }
		case 'e': { boardPosition.x() =  1; break; }
		case 'f': { boardPosition.x() =  2; break; }
		case 'g': { boardPosition.x() =  3; break; }
		case 'h': { boardPosition.x() =  4; break; }
		default : { boardPosition.x() =  0; break; }
	}

	switch (uciPosition[1]) {
		case '1': { boardPosition.y() = -4; break; }
		case '2': { boardPosition.y() = -3; break; }
		case '3': { boardPosition.y() = -2; break; }
		case '4': { boardPosition.y() = -1; break; }
		case '5': { boardPosition.y() =  1; break; }
		case '6': { boardPosition.y() =  2; break; }
		case '7': { boardPosition.y() =  3; break; }
		case '8': { boardPosition.y() =  4; break; }
		default : { boardPosition.y() =  0; break; }
	}

	return boardPosition;
}


ChessPieceType UCIMove::getRankOfPromotion(char uciRankChar) {
	if (uciRankChar == 'q' || uciRankChar == 'Q') {
		return QUEEN;
	}

	if (uciRankChar == 'r' || uciRankChar == 'R') {
		return ROOK;
	}

	if (uciRankChar == 'n' || uciRankChar == 'N') {
		return KNIGHT;
	}

	if (uciRankChar == 'b' || uciRankChar == 'B') {
		return BISHOP;
	}

	return PAWN;
}
