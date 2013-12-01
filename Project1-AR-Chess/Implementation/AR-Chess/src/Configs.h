#pragma once


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <constants definitions> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#define VIDEO_CONFIG "patterns/artoolkit/WDM_camera.xml"
#define VIDEO_PLUGIN "osgart_artoolkit"
#define VIDEO_SOURCE "osgart_video_artoolkit"
#define CAMERA_CONFIG "patterns/artoolkit/camera_para.dat"
#define TRACKER_PLUGIN "osgart_tracker_artoolkit"
#define TRACKER_BOARD_CONFIG "multi;patterns/artoolkit/multi/board.dat"
#define TRACKER_SELECTOR_CONFIG "single;patterns/artoolkit/patt.selector;50;0;0"

#define TRACKER_SELECTOR_PARTICLE_IMG "images/selector_particle.png"
#define TRACKER_SELECTOR_PARTICLE_HEIGHT_OFFSET 0.5

#define BOARD_SIZE 200
#define BOARD_MODEL "3d_models/board.ive"
#define BOARD_SQUARE_SIZE 24
#define BOARD_SQUARE_SHRINK_RATIO_FOR_OUTSIDE_PIECES 0.74
#define BOARD_HIGHLIGHTS_HEIGHT_OFFSET 0.02
#define BOARD_POSSIBLE_MOVES_HEIGHT_OFFSET 0.015
#define BOARD_SELECTIONS_HEIGHT_OFFSET 0.01

#define PIECE_HEIGHT_OFFSET -0.01
#define PIECE_OUTSIDE_OFFSET 12
#define PIECE_MOVE_ANIMATION_TRAVEL_SPEED 48
#define PIECE_MOVE_MAX_ANIMATION_DURATION_SECONDS (BOARD_SQUARE_SIZE * 2 / PIECE_MOVE_ANIMATION_TRAVEL_SPEED)
#define PIECE_MOVE_ANIMATION_HEIGHT_JUMP 90
#define PIECE_KING_SIZE 27
#define PIECE_QUEEN_SIZE 25
#define PIECE_ROOK_SIZE 20
#define PIECE_KNIGHT_SIZE 17
#define PIECE_BISHOP_SIZE 17
#define PIECE_PAWN_SIZE 11
#define PIECE_WHITE_KING_MODEL "3d_models/king_white.ive"
#define PIECE_WHITE_QUEEN_MODEL "3d_models/queen_white.ive"
#define PIECE_WHITE_ROOK_MODEL "3d_models/tower_white.ive"
#define PIECE_WHITE_KNIGHT_MODEL "3d_models/knight_white.ive"
#define PIECE_WHITE_BISHOP_MODEL "3d_models/bishop_white.ive"
#define PIECE_WHITE_PAWN_MODEL "3d_models/pawn_white.ive"
#define PIECE_BLACK_KING_MODEL "3d_models/king_black.ive"
#define PIECE_BLACK_QUEEN_MODEL "3d_models/queen_black.ive"
#define PIECE_BLACK_ROOK_MODEL "3d_models/tower_black.ive"
#define PIECE_BLACK_KNIGHT_MODEL "3d_models/knight_black.ive"
#define PIECE_BLACK_BISHOP_MODEL "3d_models/bishop_black.ive"
#define PIECE_BLACK_PAWN_MODEL "3d_models/pawn_black.ive"

#define PADDLE_SELECTOR_IMG "images/blue_circle.png"
#define PADDLE_SELECTED_IMG "images/blue_and_orange_circle.png"
#define POSSIBLE_MOVE_IMG "images/green_circle.png"
#define PADDLE_TIME_TO_SELECT_POSITION 2000 

#define GAME_STATUS_TEXT_SIZE 12
#define GAME_STATUS_TEXT_DEPTH 4
#define GAME_STATUS_NEW_GAME_H_H "Started new game (Human vs Human)"
#define GAME_STATUS_NEW_GAME_H_C "Started new game (Human vs Computer)"
#define GAME_STATUS_NEW_GAME_C_C "Started new game (Computer vs Computer)"

#define GAME_STATUS_TEXT_WHITE_IN_CHECK "White king in check!"
#define GAME_STATUS_TEXT_BLACK_IN_CHECK "Black king in check!"
#define GAME_STATUS_TEXT_DRAW "Draw!"
#define GAME_STATUS_TEXT_WHITE_WON "White player won!"
#define GAME_STATUS_TEXT_BLACK_WON "Black player won!"

#define WHITE_PLAYER_STATUS_IMG "images/white_player_status.png"
#define BLACK_PLAYER_STATUS_IMG "images/black_player_status.png"
#define PLAYER_STATUS_WIDTH 65
#define PLAYER_STATUS_HEIGHT 12
#define PLAYER_STATUS_Z_OFFSET -1
#define PLAYER_STATUS_BOARD_BORDER_OFFSET 10
#define PLAYER_STATUS_TEXT_LEFT_OFFSET -18
#define PLAYER_STATUS_TEXT_UP_OFFSET 0.5
#define PLAYER_STATUS_TEXT_Z_OFFSET 0
#define PLAYER_STATUS_TEXT_SIZE 8
#define PLAYER_STATUS_TEXT_DEPTH 1
#define PLAYER_STATUS_TEXT_ACTIVE_COLOR Vec4(0.9, 0.8, 0.05, 0.8)
#define PLAYER_STATUS_TEXT_INACTIVE_COLOR Vec4(0.4, 0.4, 0.4, 0.8)

#define TEXT_FONT "fonts/timerFont.ttf"

#define AUXILIARY_SELECTORS_X_OFFSET 6
#define AUXILIARY_SELECTORS_Y_OFFSET 0
#define AUXILIARY_SELECTORS_Z_OFFSET 0.01
#define AUXILIARY_SELECTORS_HIGHLIGHT_Z_OFFSET 0.015

#define NEW_GAME_IMGS_SCALE 1.0
#define NEW_GAME_H_H_IMG "images/new_game_h_h.png"
#define NEW_GAME_H_H_SELECTED_IMG "images/new_game_h_h_selected.png"
#define NEW_GAME_H_C_IMG "images/new_game_h_c.png"
#define NEW_GAME_H_C_SELECTED_IMG "images/new_game_h_c_selected.png"
#define NEW_GAME_C_C_IMG "images/new_game_c_c.png"
#define NEW_GAME_C_C_SELECTED_IMG "images/new_game_c_c_selected.png"

#define HISTORY_IMGS_SCALE 0.85
#define HISTORY_PREVIOUS_MOVE_IMG "images/arrow_left.png"
#define HISTORY_PREVIOUS_MOVE_SELECTED_IMG "images/arrow_left_selected.png"
#define HISTORY_NEXT_MOVE_IMG "images/arrow_right.png"
#define HISTORY_NEXT_MOVE_SELECTED_IMG "images/arrow_right_selected.png"

#define PROMOTION_SCALE_ANIMATION_DURATION_SECONDS 2
#define PROMOTION_PIECES_BOARD_OFFSET 2.6

#define RECEIVE_SHADOW_MASK 0x1
#define CAST_SHADOW_MASK 0x2


#define CHESS_ENGINE_FILE_PATH "chessengines/chessEngine.exe"
#define CHESS_ENGINE_WORKING_DIRECTORY "chessengines/"
#define CHESS_ENGINE_ARGS ""
#define CHESS_ENGINE_LOGFILE "chessengines/chessEngineCommunications.log"
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </constants definitions> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<