#pragma once


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <constants definitions> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#define VIDEO_CONFIG "patterns/artoolkit/WDM_camera.xml"
#define VIDEO_PLUGIN "osgart_artoolkit"
#define VIDEO_SOURCE "osgart_video_artoolkit"
#define CAMERA_CONFIG "patterns/artoolkit/camera_para.dat"
#define TRACKER_PLUGIN "osgart_tracker_artoolkit"
#define TRACKER_BOARD_CONFIG "multi;patterns/artoolkit/multi/board.dat"
#define TRACKER_SELECTOR_CONFIG "single;patterns/artoolkit/patt.selector;50;0;0"

#define TRACKER_SELECTOR_PARTICLE "images/selector_particle.png"
#define TRACKER_SELECTOR_PARTICLE_HEIGHT_OFFSET 0.5

#define BOARD_SIZE 200
#define BOARD_MODEL "3d_models/board.ive"
#define BOARD_SQUARE_SIZE 24
#define BOARD_SQUARE_SHRINK_RATIO_FOR_OUTSIDE_PIECES 0.74
#define BOARD_OVERLAYS_HEIGHT_OFFSET 0.01

#define PIECE_HEIGHT_OFFSET -0.01
#define PIECE_OUTSIDE_OFFSET 12
#define PIECE_MOVE_ANIMATION_TRAVEL_SPEED 48
#define PIECE_MOVE_MAX_ANIMATION_DURATION_SECONDS (BOARD_SQUARE_SIZE * 8 / PIECE_MOVE_ANIMATION_TRAVEL_SPEED)
#define PIECE_MOVE_ANIMATION_HEIGHT_JUMP 90
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

#define PADDLE_SELECTOR "images/blue_circle.png"
#define PADDLE_SELECTED "images/blue_and_orange_circle.png"
#define PADDLE_TIME_TO_SELECT_POSITION 2000 

#define WHITE_PLAYER_STATUS_IMG "images/white_player_status.png"
#define BLACK_PLAYER_STATUS_IMG "images/black_player_status.png"
#define PLAYER_STATUS_WIDTH 58
#define PLAYER_STATUS_HEIGHT 12
#define PLAYER_STATUS_Z_OFFSET -1
#define PLAYER_STATUS_BOARD_BORDER_OFFSET 10
#define PLAYER_STATUS_TEXT_LEFT_OFFSET -15
#define PLAYER_STATUS_TEXT_UP_OFFSET 0.01
#define PLAYER_STATUS_TEXT_Z_OFFSET 0
#define PLAYER_STATUS_TEXT_SIZE 7
#define PLAYER_STATUS_TEXT_DEPTH 1

#define AUXILIARY_SELECTORS_IMAGE_SCALE 0.9
#define AUXILIARY_SELECTORS_X_OFFSET 6
#define AUXILIARY_SELECTORS_Y_OFFSET 0
#define AUXILIARY_SELECTORS_Z_OFFSET 0.01
#define AUXILIARY_SELECTORS_HIGHLIGHT_Z_OFFSET 0.015
#define NEW_GAME_H_H "images/new_game_h_h.png"
#define NEW_GAME_H_H_SELECTED "images/new_game_h_h_selected.png"
#define NEW_GAME_H_M "images/new_game_h_m.png"
#define NEW_GAME_H_M_SELECTED "images/new_game_h_m_selected.png"

#define TEXT_FONT "fonts/Harrington.ttf"

#define RECEIVE_SHADOW_MASK 0x1
#define CAST_SHADOW_MASK 0x2
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </constants definitions> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<