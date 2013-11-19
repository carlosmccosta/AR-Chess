#pragma once


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <constants definitions> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#define VIDEO_CONFIG "patterns/artoolkit/WDM_camera.xml"
#define VIDEO_PLUGIN "osgart_artoolkit"
#define VIDEO_SOURCE "osgart_video_artoolkit"
#define CAMERA_CONFIG "patterns/artoolkit/camera_para.dat"
#define TRACKER_PLUGIN "osgart_tracker_artoolkit"
#define TRACKER_BOARD_CONFIG "multi;patterns/artoolkit/multi/board.dat"
#define TRACKER_SELECTOR_CONFIG "single;patterns/artoolkit/patt.selector;50;0;0"

#define BOARD_SIZE 200
#define BOARD_MODEL "3d_models/board.ive"
#define BOARD_SQUARE_SIZE 24

#define PIECE_HEIGHT_OFFSET -0.01
#define PIECE_KING_SIZE 27
#define PIECE_QUEEN_SIZE 25
#define PIECE_ROOK_SIZE 20
#define PIECE_KNIGHT_SIZE 17
#define PIECE_BISHOP_SIZE 17
#define PIECE_PAWN_SIZE 11
#define PIECE_WHITE_KING "3d_models/king_white.ive"
#define PIECE_WHITE_QUEEN "3d_models/queen_white.ive"
#define PIECE_WHITE_ROOK "3d_models/tower_white.ive"
#define PIECE_WHITE_KNIGHT "3d_models/knight_white.ive"
#define PIECE_WHITE_BISHOP "3d_models/bishop_white.ive"
#define PIECE_WHITE_PAWN "3d_models/pawn_white.ive"
#define PIECE_BLACK_KING "3d_models/king_black.ive"
#define PIECE_BLACK_QUEEN "3d_models/queen_black.ive"
#define PIECE_BLACK_ROOK "3d_models/tower_black.ive"
#define PIECE_BLACK_KNIGHT "3d_models/knight_black.ive"
#define PIECE_BLACK_BISHOP "3d_models/bishop_black.ive"
#define PIECE_BLACK_PAWN "3d_models/pawn_black.ive"

#define PADDLE_OVERLAYS_HEIGHT_OFFSET 0.01
#define PADDLE_SELECTOR "images/blue_circle.png"
#define PADDLE_SELECTED "images/blue_and_orange_circle.png"

#define RECEIVE_SHADOW_MASK 0x1
#define CAST_SHADOW_MASK 0x2
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </constants definitions> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<