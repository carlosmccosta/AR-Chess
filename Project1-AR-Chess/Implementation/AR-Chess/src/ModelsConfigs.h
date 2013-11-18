#pragma once


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <constants definitions> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#define VIDEO_CONFIG "data/artoolkit/WDM_camera.xml"
#define VIDEO_PLUGIN "osgart_artoolkit"
#define VIDEO_SOURCE "osgart_video_artoolkit"
#define CAMERA_CONFIG "data/artoolkit/camera_para.dat"
#define TRACKER_PLUGIN "osgart_tracker_artoolkit"
#define TRACKER_BOARD_CONFIG "multi;data/artoolkit/multi/marker.dat"
#define TRACKER_SELECTOR_CONFIG "single;data/artoolkit/patt.kanji;80;0;0"

#define BOARD_SIZE 200
#define BOARD_HEIGHT 16
#define BOARD_MODEL "models/board.ive"
#define BOARD_BORDER_LEFT_RIGHT 20
#define BOARD_BORDER_TOP_BOTTOM 10
#define BOARD_SQUARE_SIZE 24

#define PIECE_HEIGHT_OFFSET -0.5
#define PIECE_KING_SIZE 27
#define PIECE_QUEEN_SIZE 25
#define PIECE_ROOK_SIZE 20
#define PIECE_KNIGHT_SIZE 17
#define PIECE_BISHOP_SIZE 17
#define PIECE_PAWN_SIZE 11
#define PIECE_WHITE_KING "models/king_white.ive"
#define PIECE_WHITE_QUEEN "models/queen_white.ive"
#define PIECE_WHITE_ROOK "models/tower_white.ive"
#define PIECE_WHITE_KNIGHT "models/knight_white.ive"
#define PIECE_WHITE_BISHOP "models/bishop_white.ive"
#define PIECE_WHITE_PAWN "models/pawn_white.ive"
#define PIECE_BLACK_KING "models/king_black.ive"
#define PIECE_BLACK_QUEEN "models/queen_black.ive"
#define PIECE_BLACK_ROOK "models/tower_black.ive"
#define PIECE_BLACK_KNIGHT "models/knight_black.ive"
#define PIECE_BLACK_BISHOP "models/bishop_black.ive"
#define PIECE_BLACK_PAWN "models/pawn_black.ive"

#define RECEIVE_SHADOW_MASK 0x1
#define CAST_SHADOW_MASK 0x2
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </constants definitions> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<