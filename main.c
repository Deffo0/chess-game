<<<<<<< HEAD
// Authors:
// Youssef Ali Mohamed Bazina & Ahmed Adel Abudef

//#--------------------------------------------------------------------------------------------#
// Libraries:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_IMAGE.h>
#include <SDL_TTF.h>
#include <SDL_MIXER.h>
#include <windows.h>
//#--------------------------------------------------------------------------------------------#
#define  FPS 300
#define  FRAME_TARGET_TIME 1000/FPS

//                                -----------<Global variables>------------:

// Initialize a pointer for the window
SDL_Window *window = NULL;
// Initialize a pointers for the images
SDL_Surface *image[33], *valid_images[63], *not_valid_image, *undo_image, *redo_image, *wpromo_image, *bpromo_image, *icon, *white_turn_image , *black_turn_image;
SDL_Surface *check_image, *b_mate_image, *w_mate_image, *dead_pos_image, *stale_image, *moves_surf, *moves_prev_surf, *save_image, *load_image;
SDL_Surface *press_image[4];
// Initialize a pointers for the textures
SDL_Texture *texture[33], *valid_textures[63], *not_valid_texture, *undo_texture, *redo_texture, *wpromo_texture, *bpromo_texture, *check_texture, *white_turn_texture , *black_turn_texture;
SDL_Texture *b_mate_texture, *w_mate_texture, *dead_pos_texture, *stale_texture, *moves_texture, *moves_prev_texture, *save_texture, *load_texture;
SDL_Texture *press_texture[4];
TTF_Font *font;
// Initialize a pointers for the sounds
Mix_Music *sound[3];
// Initialize a pointer for the render
SDL_Renderer *renderer = NULL;
// Initialize save file location:
FILE *last_scene;
//-----------------------------------------------//

int Ready_To_start = 0;
int st = 0, dp = 0, enemy_line1 = 0, enemy_line2 = 0, enemy_line3 = 0, enemy_line4 = 0, enemy_line5 = 0, enemy_line6 = 0, enemy_line7 = 0, enemy_line8 = 0;
int flag = 0, sum = 0, chosen_piece = 0, dead_piece = 0, guide = 0, wrong = 0, f_save = 0, f_load = 0, f_undo = 0, f_redo = 0, temp_chosen, down;
int k1 = 0, k2 = 0, k3 = 0, k4 = 0, k5 = 0, k6 = 0, k7 = 0, k8 = 0, k9 = 0, k10 = 0, k11 = 0, k12 = 0;
int l1 = 0, l2 = 0, l3 = 0, l4 = 0, l5 = 0, l6 = 0, l7 = 0, l8 = 0;
int line_moves_x[63], line_moves_y[63], line_y = 0, line_x = 0;
int valid_line_x[63], valid_line_y[63], shield_x = 0, shield_y = 0;
int die_x[63], x, die_y[63], y, wdx = 0, wdy = 0, bdx = 0, bdy = 0, w = 0, s = 0, e = 0, P_king;

//-----------------------------------------------//
// Initialize structure to contain all dynamic properties of the game loop:
typedef struct game_properties{
    // Flag for Dead position and stalemate
    int d_pos;
    int stale;
    // Flags for promotion
    int wpromo;
    int bpromo;
    int bpr;
    int bpb;
    int bpn;
    int bpq;
    int wpr;
    int wpb;
    int wpn;
    int wpq;
    // Flag to toggle turn
    int w_o_b;
    // Counter for undo and redo times
    int do_count;
    // Flags for cover
    int cover;
    int covered;
    int cov_shield;
    // Flags for check and threaten piece
    int king_check;
    int check;
    int w_check;
    int b_check;
    // Threaten piece number
    int T_piece;
    int b_mate;
    int w_mate;
    // Flags for Castling
    int brc_w_k;
    int blc_w_k;
    int wrc_w_k;
    int wlc_w_k;
    // Counter, Arrays and Flags for Undo and Redo Features
    int prev;
    // Flags for castling
    int bcas;
    int wcas;
    // Flags for promotion
    int do_wpromo;
    int do_bpromo;
    // Save the chosen pieces along the game (Undo)
    int chosen_undo[5000];
    // Save the previous coordinates for the chosen pieces
    int do_moves_x[5000];
    int do_moves_y[5000];
    // Save the dead pieces along the game (Undo)
    int dead_undo[5000];
    // Save the previous coordinates for the dead pieces
    int dead_moves_x[5000];
    int dead_moves_y[5000];
    // Save the previous row and column numbers for the chosen pieces
    int prev_bcol[5000];
    int prev_brow[5000];
    // Save the previous row and column numbers for the dead pieces
    int prev_bcol_dead[5000];
    int prev_brow_dead[5000];
    // Save the chosen pieces along the game (Redo)
    int chosen_redo[5000];
    // Save the new coordinates for the chosen pieces
    int redo_moves_x[5000];
    int redo_moves_y[5000];
    // Save the dead pieces along the game (Redo)
    int dead_redo[5000];
    // Save the new coordinates for the dead pieces
    int re_dead_moves_x[5000];
    int re_dead_moves_y[5000];
    // Save all special moves (promotion, kill moves, castling)
    int compare[5000];
    // Save the new row and column numbers for the chosen pieces
    int next_bcol[5000];
    int next_brow[5000];
    // Save the new row and column numbers for the dead pieces
    int next_bcol_dead[5000];
    int next_brow_dead[5000];
    // Save the chosen piece's type for promotion (Redo)
    int redo_promo_type[5000];
    // Save check and stalemate and dead position along the game
    int comp_mate[63];
    // Save the first moves
    int first_move[63];
    // Flag and an array for en passant moves
    int en_passant;
    int passant[32];
}game_properties;
game_properties game_prop;
//-----------------------------------------------//
// Initialize a structure for the pieces to determine its coordinates etc..
typedef struct piece_pos{
    // X and Y coordinates
    int x;
    int y;
    int x_prev;
    int y_prev;
    int xtemp;
    int ytemp;
    // Piece position on the Board
    int brow;
    int bcol;
    int brow_prev;
    int temp_brow_prev;
    int bcol_prev;
    // Piece type
    char type;
    // Valid moves for the pieces
    int valid_moves_x[63];
    int valid_moves_y[63];
    // Cover moves for the pieces
    int valid_cover_x[63];
    int valid_cover_y[63];
    // Counter for the arrays
    int cx;
    int cy;
    int cover_cx;
    int cover_cy;
    // Determine if the piece is live or dead
    int l_o_d;
}pieces_pos;
pieces_pos piece[32];
//---------------------------------------------//
// Initialize a 2D array for the board
// Determine if the chosen piece is black or white
int board[8][8] = {
                {-1, -1, -1, -1, -1, -1, -1, -1},
                {-1, -1, -1, -1, -1, -1, -1, -1},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {1, 1, 1, 1, 1, 1, 1, 1},
                {1, 1, 1, 1, 1, 1, 1, 1}
};
int board_types[8][8] = {
                {-5, -4, -3, -2, -1, -3, -4, -5},
                {-6, -6, -6, -6, -6, -6, -6, -6},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {6, 6, 6, 6, 6, 6, 6, 6},
                {5, 4, 3, 2, 1, 3, 4, 5}
};
// Enumerating numbers to names which refers to piece's type
enum board_elements {br = -5, bn = -4, bb = -3, bq = -2, bk = -1, bp = -6, wp = 6, wk = 1, wq = 2, wb = 3, wn = 4, wr = 5};
// A Board to Determine the move of the piece in letters
char *board_moves[8][8] = {
                {"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"},
                {"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7"},
                {"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6"},
                {"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5"},
                {"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4"},
                {"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3"},
                {"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2"},
                {"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"}
};

//#-------------------------------------------------------------------------#

int initialize_basics(void){

    // Initialize SDL library
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("Error in initialization\n");
        return 0;
    }
    // Initialize SDL_ttf library
    TTF_Init();

    // Initialize the window of the game
    window = SDL_CreateWindow(
        "Chess",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1100,
        600,
        SDL_WINDOW_MINIMIZED
    );

    // Check if the window is running or not
    if(!window){
        printf("Error in window\n");
        return 0;
    }

    // Initialize renderer for the game
    renderer = SDL_CreateRenderer(window, -1, 0);

    // Check if the renderer is running or not
    if(!renderer){
        printf("Error in render\n");
        return 0;
    }
    // Return if all gone well
    return 1;
}

//#-------------------------------------------------------------------------#

// Setup the initial positions for all games elements (board and pieces)
void setup(void){
    int t, j = 0;
    // Load the font
    font = TTF_OpenFont("attach/Roboto-Bold.ttf", 100);

    // Load the images for the whole game:
    icon = IMG_Load("images/icon.png");
    image[0] = IMG_Load("images/board.png");
    image[1] = IMG_Load("images/bcas.png");
    image[2] = IMG_Load("images/bhor.png");
    image[3] = IMG_Load("images/bele.png");
    image[4] = IMG_Load("images/bque.png");
    image[5] = IMG_Load("images/bkin.png");
    image[6] = IMG_Load("images/bele.png");
    image[7] = IMG_Load("images/bhor.png");
    image[8] = IMG_Load("images/bcas.png");
    image[9] = IMG_Load("images/bsol.png");
    image[10] = IMG_Load("images/bsol.png");
    image[11] = IMG_Load("images/bsol.png");
    image[12] = IMG_Load("images/bsol.png");
    image[13] = IMG_Load("images/bsol.png");
    image[14] = IMG_Load("images/bsol.png");
    image[15] = IMG_Load("images/bsol.png");
    image[16] = IMG_Load("images/bsol.png");
    image[17] = IMG_Load("images/wsol.png");
    image[18] = IMG_Load("images/wsol.png");
    image[19] = IMG_Load("images/wsol.png");
    image[20] = IMG_Load("images/wsol.png");
    image[21] = IMG_Load("images/wsol.png");
    image[22] = IMG_Load("images/wsol.png");
    image[23] = IMG_Load("images/wsol.png");
    image[24] = IMG_Load("images/wsol.png");
    image[25] = IMG_Load("images/wcas.png");
    image[26] = IMG_Load("images/whor.png");
    image[27] = IMG_Load("images/wele.png");
    image[28] = IMG_Load("images/wque.png");
    image[29] = IMG_Load("images/wkin.png");
    image[30] = IMG_Load("images/wele.png");
    image[31] = IMG_Load("images/whor.png");
    image[32] = IMG_Load("images/wcas.png");

    press_image[0] = IMG_Load("images/load 2.png");
    press_image[1] = IMG_Load("images/save 2.png");
    press_image[2] = IMG_Load("images/undo 2.png");
    press_image[3] = IMG_Load("images/redo 2.png");

    // Create textures for images:
    for(j = 0; j < 33; j++){
        texture[j] = SDL_CreateTextureFromSurface(renderer, image[j]);
        if(!texture[j]){
            printf("Error in textures\n");
            return 0;
        }
    }
    //-----------------------------------------------//
    not_valid_image = IMG_Load("images/notvalid.png");
    for(t = 0; t < 63; t++){
        valid_images[t]=IMG_Load("images/valid.png");
        if(!valid_images[t]){
            printf("Error in valid_images");
            return 0;
        }
    }

    not_valid_texture = SDL_CreateTextureFromSurface(renderer, not_valid_image);
    for(t = 0; t < 63; t++){
        valid_textures[t] = SDL_CreateTextureFromSurface(renderer,valid_images[t]);
        if(!valid_textures[t]){
            printf("Error in valid_textures\n");
            return 0;
        }
    }

    for(j = 0; j < 4; j++){
        press_texture[j] = SDL_CreateTextureFromSurface(renderer, press_image[j]);
    }
    //-----------------------------------------------//
    wpromo_image = IMG_Load("images/wpromo.png");
    wpromo_texture = SDL_CreateTextureFromSurface(renderer, wpromo_image);

    bpromo_image = IMG_Load("images/bpromo.png");
    bpromo_texture = SDL_CreateTextureFromSurface(renderer, bpromo_image);
    //-----------------------------------------------//
    undo_image = IMG_Load("images/undo.png");
    undo_texture = SDL_CreateTextureFromSurface(renderer, undo_image);
    redo_image = IMG_Load("images/redo.png");
    redo_texture = SDL_CreateTextureFromSurface(renderer, redo_image);
    //-----------------------------------------------//
    check_image = IMG_Load("images/check.png");
    check_texture = SDL_CreateTextureFromSurface(renderer, check_image);
    //-----------------------------------------------//
    b_mate_image = IMG_Load("images/bmate.png");
    b_mate_texture = SDL_CreateTextureFromSurface(renderer, b_mate_image);

    w_mate_image = IMG_Load("images/wmate.png");
    w_mate_texture = SDL_CreateTextureFromSurface(renderer, w_mate_image);
    //-----------------------------------------------//
    save_image = IMG_Load("images/save.png");
    save_texture = SDL_CreateTextureFromSurface(renderer, save_image);

    load_image = IMG_Load("images/load.png");
    load_texture = SDL_CreateTextureFromSurface(renderer, load_image);
    //-----------------------------------------------//
    dead_pos_image = IMG_Load("images/dead.png");
    dead_pos_texture = SDL_CreateTextureFromSurface(renderer, dead_pos_image);

    stale_image = IMG_Load("images/stale.png");
    stale_texture = SDL_CreateTextureFromSurface(renderer, stale_image);
    //-----------------------------------------------//
    white_turn_image = IMG_Load("images/white turn.png");
    white_turn_texture = SDL_CreateTextureFromSurface(renderer, white_turn_image);

    black_turn_image = IMG_Load("images/black turn.png");
    black_turn_texture = SDL_CreateTextureFromSurface(renderer, black_turn_image);
    //-----------------------------------------------//
    // Set types for all pieces
    piece[0].type = 'r';
    piece[1].type = 'n';
    piece[2].type = 'b';
    piece[3].type = 'q';
    piece[4].type = 'k';
    piece[5].type = 'b';
    piece[6].type = 'n';
    piece[7].type = 'r';
    piece[8].type = 'p';
    piece[9].type = 'p';
    piece[10].type = 'p';
    piece[11].type = 'p';
    piece[12].type = 'p';
    piece[13].type = 'p';
    piece[14].type = 'p';
    piece[15].type = 'p';
    piece[16].type = 'p';
    piece[17].type = 'p';
    piece[18].type = 'p';
    piece[19].type = 'p';
    piece[20].type = 'p';
    piece[21].type = 'p';
    piece[22].type = 'p';
    piece[23].type = 'p';
    piece[24].type = 'r';
    piece[25].type = 'n';
    piece[26].type = 'b';
    piece[27].type = 'q';
    piece[28].type = 'k';
    piece[29].type = 'b';
    piece[30].type = 'n';
    piece[31].type = 'r';
    //-----------------------------------------------//
    // Enter initial values for the flags, counter and coordinates for the whole game:
    game_prop.w_o_b = 1;
    game_prop.do_count = 0;
    game_prop.cover = 0;
    game_prop.covered = 0;
    game_prop.wpromo = 0;
    game_prop.bpromo = 0;
    game_prop.do_wpromo = 0;
    game_prop.do_bpromo = 0;
    game_prop.brc_w_k = 0;
    game_prop.blc_w_k = 0;
    game_prop.wrc_w_k = 0;
    game_prop.wlc_w_k = 0;
    game_prop.bcas = 0;
    game_prop.wcas = 0;
    game_prop.prev = -1;
    game_prop.check = 0;
    game_prop.w_check = 0;
    game_prop.b_check = 0;
    game_prop.T_piece = -5;
    game_prop.b_mate = 0;
    game_prop.w_mate = 0;
    game_prop.bpr = 0;
    game_prop.bpb = 0;
    game_prop.bpn = 0;
    game_prop.bpq = 0;
    game_prop.wpr = 0;
    game_prop.wpb = 0;
    game_prop.wpn = 0;
    game_prop.wpq = 0;
    game_prop.d_pos = 0;
    game_prop.stale = 0;
    game_prop.cov_shield = 0;
    game_prop.en_passant = 0;
    //-----------------------------------------------//
    for(j = 0; j < 5000; j++){
        game_prop.compare[j] = -10;
    }
    //-----------------------------------------------//
    for (j = 0; j < 32; j++){
        piece[j].brow_prev = -1;
        piece[j].bcol_prev = -1;
        piece[j].l_o_d = 1;
        game_prop.passant[j] = 0;
    }
    //-----------------------------------------------//
    for (j = 0; j < 63; j++){
        line_moves_x[j] = -15;
        line_moves_x[y] = -15;
    }
    //-----------------------------------------------//
    // Set x and y coordinates for all pieces:
    for (j = 0; j < 8; j++){
        piece[j].y = 0;
        piece[j].x = sum ;
        piece[j].brow = piece[j].y / 75;
        piece[j].bcol = piece[j].x / 75;
        sum = sum + 75;
    }

    sum = 0;
    for (j = 8; j < 16; j++){
        piece[j].y = 75;
        piece[j].x = sum ;
        piece[j].brow = piece[j].y / 75;
        piece[j].bcol = piece[j].x / 75;
        sum = sum + 75;
    }

    sum = 0;
    for (j = 16; j < 24; j++){
        piece[j].y = 450;
        piece[j].x = sum ;
        piece[j].brow = piece[j].y / 75;
        piece[j].bcol = piece[j].x / 75;
        sum = sum + 75;
    }

    sum = 0;
    for (j = 24; j < 32; j++){
        piece[j].y = 525;
        piece[j].x = sum ;
        piece[j].brow = piece[j].y / 75;
        piece[j].bcol = piece[j].x / 75;
        sum = sum + 75;
    }
    //-----------------------------------------------//
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096);
    sound[0] = Mix_LoadMUS("attach/Erik Satie - Gnossienne No.1.mp3");
    Mix_PlayMusic(sound[0], 120);
    Mix_VolumeMusic(9);
}

//#-------------------------------------------------------------------------#

// Merge the moves which covers the king with normal valid moves for the piece
void append(int s){
    int i, j;
    for(i = piece[s].cx, j = 0; i < piece[s].cover_cx + piece[s].cx, j < piece[s].cover_cx; i++, j++){
        piece[s].valid_moves_x[i] = piece[s].valid_cover_x[j];
        piece[s].valid_moves_y[i] = piece[s].valid_cover_y[j];
    }
    piece[s].cx = piece[s].cx + piece[s].cover_cx;
    piece[s].cy = piece[s].cy + piece[s].cover_cy;
}

// Reset all counters and array to its initial values after every move
void clearing(void){
    for(int w = 0; w < 32; w++){
        for(int h = 0; h < 63; h++){
            valid_line_x[h] = -10;
            valid_line_y[h] = -10;
            line_moves_x[h] = -10;
            line_moves_y[h] = -10;
            piece[w].valid_moves_x[h] = -10;
            piece[w].valid_moves_y[h] = -10;
            piece[w].valid_cover_x[h] = -10;
            piece[w].valid_cover_y[h] = -10;
            piece[w].cx = 0;
            piece[w].cy = 0;
            piece[w].cover_cx = 0;
            piece[w].cover_cy = 0;
            die_x[w] = -10;
            die_y[w] = -10;
        }
    }
    shield_x = 0;
    shield_y = 0;
    line_x = 0;
    line_y = 0;
    x = 0;
    y = 0;
    game_prop.blc_w_k = 0;
    game_prop.brc_w_k = 0;
    game_prop.wlc_w_k = 0;
    game_prop.wrc_w_k = 0;
}

//#-------------------------------------------------------------------------#

// White Pawn Movement:
void wsol(int piece_moves, int ally, int enemy, int p, int l){
    // Upper Moves
    if (board[piece[piece_moves].brow - 1][piece[piece_moves].bcol] == 1){
        // Do nothing if there is an ally in front of it
    }if (board[piece[piece_moves].brow - 1][piece[piece_moves].bcol] == 0){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
        piece[piece_moves].cy++;
        if (piece[piece_moves].brow == 6 && board[piece[piece_moves].brow - 2][piece[piece_moves].bcol] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 2) * 75;
            piece[piece_moves].cy++;
        }
    }
    // Upper Right Moves if there is an enemy ahead to kill
    if((board[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 1] == -1 &&
            piece[piece_moves].brow -1 >= 0 && piece[piece_moves].bcol + 1 < 8)){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
        piece[piece_moves].cy++;
        die_x[x] = (piece[piece_moves].bcol + 1) * 75;
        x++;
        die_y[y] = (piece[piece_moves].brow - 1) * 75;
        y++;
    }
    // If there is Ally ahead, save it as cover position
    else if (board[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 1] == 1 &&
            piece[piece_moves].brow -1 >= 0 && piece[piece_moves].bcol + 1 < 8){
        piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol + 1) * 75;
        piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow - 1) * 75;
        piece[piece_moves].cover_cx++;
        piece[piece_moves].cover_cy++;
    }
    // Upper Left Moves if there is an enemy ahead to kill
    if((board[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 1] == -1 &&
            piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol - 1 >= 0)){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
        piece[piece_moves].cy++;
        die_x[x] = (piece[piece_moves].bcol - 1) * 75;
        x++;
        die_y[y] = (piece[piece_moves].brow - 1) * 75;
        y++;
    }
    // If there is Ally ahead, save it as cover position
    else if (board[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 1] == 1 &&
            piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol - 1 >= 0){
        piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol - 1) * 75;
        piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow - 1) * 75;
        piece[piece_moves].cover_cx++;
        piece[piece_moves].cover_cy++;
    }
    // Left Moves if there is an enemy to kill by en passant
    if(board_types[piece[piece_moves].brow][piece[piece_moves].bcol - 1] == bp &&
        game_prop.en_passant > 0 &&
        piece[piece_moves].bcol - 1 >= 0 && piece[piece_moves].brow == 3 &&
        board_types[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 1] == 0){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
        piece[piece_moves].cy++;
        die_x[x] = (piece[piece_moves].bcol - 1) * 75;
        x++;
        die_y[y] = (piece[piece_moves].brow) * 75;
        y++;
    }
    // Right Moves if there is an enemy to kill by en passant
    if(board_types[piece[piece_moves].brow][piece[piece_moves].bcol + 1] == bp &&
        game_prop.en_passant > 0 &&
        piece[piece_moves].bcol + 1 < 8 && piece[piece_moves].brow == 3 &&
        board_types[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 1] == 0){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
        piece[piece_moves].cy++;
        die_x[x] = (piece[piece_moves].bcol + 1) * 75;
        x++;
        die_y[y] = (piece[piece_moves].brow) * 75;
        y++;
    }
}
//-----------------------------------------------//
// Black Pawn Movement:
void bsol(int piece_moves, int ally, int enemy, int p, int l){
    // Lower Moves
    if (board[piece[piece_moves].brow + 1][piece[piece_moves].bcol] == -1){
        // Do nothing if there is an ally in front of it
    }if (board[piece[piece_moves].brow + 1][piece[piece_moves].bcol] == 0){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
        piece[piece_moves].cy++;
        if (piece[piece_moves].brow == 1 && board[piece[piece_moves].brow + 2][piece[piece_moves].bcol] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 2) * 75;
            piece[piece_moves].cy++;
        }
    }
    // Lower Right Moves if there is an enemy ahead to kill
    if((board[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 1] == 1 &&
            piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol + 1 < 8)){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
        piece[piece_moves].cy++;
        die_x[x] = (piece[piece_moves].bcol + 1) * 75;
        x++;
        die_y[y] = (piece[piece_moves].brow + 1) * 75;
        y++;
    }
    // If there is Ally ahead, save it as cover position
    else if (board[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 1] == -1 &&
            piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol + 1 < 8){
        piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol + 1) * 75;
        piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow + 1) * 75;
        piece[piece_moves].cover_cx++;
        piece[piece_moves].cover_cy++;
    }
    // Lower Left Moves if there is an enemy ahead to kill
    if((board[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 1] == 1 &&
            piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol - 1 >= 0)){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
        piece[piece_moves].cy++;
        die_x[x] = (piece[piece_moves].bcol-1) * 75;
        x++;
        die_y[y] = (piece[piece_moves].brow+1) * 75;
        y++;
    }
    // If there is Ally ahead, save it as cover position
    else if (board[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 1] == -1 &&
            piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol - 1 >= 0){
        piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol - 1) * 75;
        piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow + 1) * 75;
        piece[piece_moves].cover_cx++;
        piece[piece_moves].cover_cy++;
    }
    // Left Moves if there is an enemy to kill by en passant
    if(board_types[piece[piece_moves].brow][piece[piece_moves].bcol - 1] == wp &&
        game_prop.en_passant > 0 &&
        piece[piece_moves].bcol - 1 >= 0 && piece[piece_moves].brow == 4 &&
        board_types[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 1] == 0){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
        piece[piece_moves].cy++;
        die_x[x] = (piece[piece_moves].bcol - 1) * 75;
        x++;
        die_y[y] = (piece[piece_moves].brow) * 75;
        y++;
    }
    // Right Moves if there is an enemy to kill by en passant
    if(board_types[piece[piece_moves].brow][piece[piece_moves].bcol + 1] == wp &&
        game_prop.en_passant > 0 &&
        piece[piece_moves].bcol + 1 < 8 && piece[piece_moves].brow == 4 &&
        board_types[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 1] == 0){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
        piece[piece_moves].cy++;
        die_x[x] = (piece[piece_moves].bcol + 1) * 75;
        x++;
        die_y[y] = (piece[piece_moves].brow) * 75;
        y++;
    }
}
//-----------------------------------------------//
// Queen Movement:
void queen(int piece_moves, int ally, int enemy, int p, int l){
    // Positive Horizontal Movement
    for(p = (piece[piece_moves].bcol + 1); p < 8; p++){
        // If there is Ally ahead, save it as cover position
        if(board[piece[piece_moves].brow][p] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = p * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = piece[piece_moves].brow * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[piece[piece_moves].brow][p] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[piece[piece_moves].brow][p] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cy++;
            die_x[x] = p * 75;
            x++;
            die_y[y] = piece[piece_moves].brow * 75;
            y++;
            break;
        }
    }
    // Negative Horizontal Movement
    for(p = (piece[piece_moves].bcol - 1); p >= 0; p--){
        // If there is Ally ahead, save it as cover position
        if(board[piece[piece_moves].brow][p] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = p * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = piece[piece_moves].brow * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[piece[piece_moves].brow][p] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[piece[piece_moves].brow][p] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cy++;
            die_x[x] = p * 75;
            x++;
            die_y[y] = piece[piece_moves].brow * 75;
            y++;
            break;
        }
    }
    // Positive Vertical Movement
    for(p = (piece[piece_moves].brow + 1); p < 8; p++){
        // If there is Ally ahead, save it as cover position
        if(board[p][piece[piece_moves].bcol] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = piece[piece_moves].bcol * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = p * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[p][piece[piece_moves].bcol] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = p * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[p][piece[piece_moves].bcol] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = p * 75;
            piece[piece_moves].cy++;
            die_x[x] = piece[piece_moves].bcol * 75;
            x++;
            die_y[y] = p * 75;
            y++;
            break;
        }
    }
    // Negative Vertical Movement
    for(p = (piece[piece_moves].brow - 1); p >= 0; p--){
        // If there is Ally ahead, save it as cover position
        if(board[p][piece[piece_moves].bcol] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = piece[piece_moves].bcol * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = p * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[p][piece[piece_moves].bcol] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = piece[piece_moves].bcol * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = p * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[p][piece[piece_moves].bcol] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = piece[piece_moves].bcol * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = p * 75;
            piece[piece_moves].cy++;
            die_x[x] = piece[piece_moves].bcol * 75;
            x++;
            die_y[y] = p * 75;
            y++;
            break;
        }
    }
    // Upper Right Movement
    for(p = piece[piece_moves].bcol + 1, l = piece[piece_moves].brow - 1 ; p < 8 && l >= 0; p++, l--){
        // If there is Ally ahead, save it as cover position
        if(board[l][p] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = p * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = l * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[l][p] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[l][p] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
            die_x[x] = p * 75;
            x++;
            die_y[y] = l * 75;
            y++;
            break;
        }
    }
    // Upper Left Movement
    for(p = piece[piece_moves].bcol - 1, l = piece[piece_moves].brow - 1; p >= 0 && l >= 0; p--, l--){
        // If there is Ally ahead, save it as cover position
        if(board[l][p] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = p * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = l * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[l][p] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[l][p] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
            die_x[x] = p * 75;
            x++;
            die_y[y] = l * 75;
            y++;
            break;
        }
    }
    // Lower Right Movement
    for(p = piece[piece_moves].bcol + 1, l = piece[piece_moves].brow + 1; p < 8 && l < 8; p++, l++){
        // If there is Ally ahead, save it as cover position
        if(board[l][p] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = p * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = l * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[l][p] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[l][p] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
            die_x[x] = p * 75;
            x++;
            die_y[y] = l * 75;
            y++;
            break;
        }
    }
    // Lower Left Movement
    for(p = piece[piece_moves].bcol - 1,l = piece[piece_moves].brow + 1;p >= 0 && l < 8; p--, l++){
        // If there is Ally ahead, save it as cover position
        if(board[l][p] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = p * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = l * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[l][p] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[l][p] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
            die_x[x] = p * 75;
            x++;
            die_y[y] = l * 75;
            y++;
            break;
        }
    }
}
//-----------------------------------------------//
// Bishop Movement:
void bishop(int piece_moves, int ally, int enemy, int p, int l){
    // Upper Right Movement
    for(p = piece[piece_moves].bcol + 1, l = piece[piece_moves].brow - 1 ; p < 8 && l >= 0; p++, l--){
        // If there is Ally ahead, save it as cover position
        if(board[l][p] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = p * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = l * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[l][p] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[l][p] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
            die_x[x] = p * 75;
            x++;
            die_y[y] = l * 75;
            y++;
            break;
        }
    }
    // Upper Left Movement
    for(p = piece[piece_moves].bcol - 1, l = piece[piece_moves].brow - 1; p >= 0 && l >= 0; p--, l--){
        // If there is Ally ahead, save it as cover position
        if(board[l][p] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = p * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = l * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[l][p] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[l][p] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
            die_x[x] = p * 75;
            x++;
            die_y[y] = l * 75;
            y++;
            break;
        }
    }
    // Lower Right Movement
    for(p = piece[piece_moves].bcol + 1, l = piece[piece_moves].brow + 1; p < 8 && l < 8; p++, l++){
        // If there is Ally ahead, save it as cover position
        if(board[l][p] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = p * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = l * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[l][p] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[l][p] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
            die_x[x] = p * 75;
            x++;
            die_y[y] = l * 75;
            y++;
            break;
        }
    }
    // Lower Left Movement
    for(p = piece[piece_moves].bcol - 1,l = piece[piece_moves].brow + 1;p >= 0 && l < 8; p--, l++){
        // If there is Ally ahead, save it as cover position
        if(board[l][p] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = p * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = l * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[l][p] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[l][p] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
            die_x[x] = p * 75;
            x++;
            die_y[y] = l * 75;
            y++;
            break;
        }
    }
}
//-----------------------------------------------//
// Knight Movement:
void knight(int piece_moves, int ally, int enemy, int p, int l){
    // 2 Lower 1 Left Movement
    // If there is no one, save it as a valid move
    if ((board[piece[piece_moves].brow + 2][piece[piece_moves].bcol - 1] == 0
        || board[piece[piece_moves].brow + 2][piece[piece_moves].bcol - 1] == enemy) &&
        piece[piece_moves].brow + 2 < 8 && piece[piece_moves].bcol - 1 >= 0){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 2) * 75;
        piece[piece_moves].cy++;
        // If there is an enemy ahead, save it as a kill position
        if (board[piece[piece_moves].brow + 2][piece[piece_moves].bcol - 1] == enemy){
            die_x[x] = (piece[piece_moves].bcol - 1) * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow + 2) * 75;
            y++;
        }
    }
    // If there is Ally ahead, save it as cover position
    else if (board[piece[piece_moves].brow + 2][piece[piece_moves].bcol - 1] == ally){
        piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol - 1) * 75;
        piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow + 2) * 75;
        piece[piece_moves].cover_cx++;
        piece[piece_moves].cover_cy++;
    }
    // 2 Lower 1 Right Movement
    // If there is no one, save it as a valid move
    if ((board[piece[piece_moves].brow + 2][piece[piece_moves].bcol + 1] == 0
        || board[piece[piece_moves].brow + 2][piece[piece_moves].bcol + 1] == enemy) &&
        piece[piece_moves].brow + 2 < 8 && piece[piece_moves].bcol + 1 < 8){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 2) * 75;
        piece[piece_moves].cy++;
        // If there is an enemy ahead, save it as a kill position
        if (board[piece[piece_moves].brow + 2][piece[piece_moves].bcol + 1] == enemy){
            die_x[x] = (piece[piece_moves].bcol + 1) * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow + 2) * 75;
            y++;
        }
    }
    // If there is Ally ahead, save it as cover position
    else if (board[piece[piece_moves].brow + 2][piece[piece_moves].bcol + 1] == ally){
        piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol + 1) * 75;
        piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow + 2) * 75;
        piece[piece_moves].cover_cx++;
        piece[piece_moves].cover_cy++;
    }
    // 2 Upper 1 Left Movement
    // If there is no one, save it as a valid move
    if ((board[piece[piece_moves].brow - 2][piece[piece_moves].bcol - 1] == 0
        || board[piece[piece_moves].brow - 2][piece[piece_moves].bcol - 1] == enemy) &&
        piece[piece_moves].brow - 2 >= 0 && piece[piece_moves].bcol - 1 >= 0){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 2) * 75;
        piece[piece_moves].cy++;
        // If there is an enemy ahead, save it as a kill position
        if (board[piece[piece_moves].brow - 2][piece[piece_moves].bcol - 1] == enemy){
            die_x[x] = (piece[piece_moves].bcol - 1) * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow - 2) * 75;
            y++;
        }
    }
    // If there is Ally ahead, save it as cover position
    else if (board[piece[piece_moves].brow - 2][piece[piece_moves].bcol - 1] == ally){
        piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol - 1) * 75;
        piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow - 2) * 75;
        piece[piece_moves].cover_cx++;
        piece[piece_moves].cover_cy++;
    }
    // 2 Upper 1 Right Movement
    // If there is no one, save it as a valid move
    if ((board[piece[piece_moves].brow - 2][piece[piece_moves].bcol + 1] == 0
        || board[piece[piece_moves].brow - 2][piece[piece_moves].bcol + 1] == enemy) &&
        piece[piece_moves].brow - 2 >= 0 && piece[piece_moves].bcol + 1 < 8){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 2) * 75;
        piece[piece_moves].cy++;
        // If there is an enemy ahead, save it as a kill position
        if (board[piece[piece_moves].brow - 2][piece[piece_moves].bcol + 1] == enemy){
            die_x[x] = (piece[piece_moves].bcol + 1) * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow - 2) * 75;
            y++;
        }
    }
    // If there is Ally ahead, save it as cover position
    else if (board[piece[piece_moves].brow - 2][piece[piece_moves].bcol + 1] == ally){
        piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol + 1) * 75;
        piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow - 2) * 75;
        piece[piece_moves].cover_cx++;
        piece[piece_moves].cover_cy++;
    }
    // 2 Right 1 Lower Movement
    // If there is no one, save it as a valid move
    if ((board[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 2] == 0
        || board[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 2] == enemy) &&
        piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol + 2 < 8){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 2) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
        piece[piece_moves].cy++;
        // If there is an enemy ahead, save it as a kill position
        if (board[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 2] == enemy){
            die_x[x] = (piece[piece_moves].bcol + 2) * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow + 1) * 75;
            y++;
        }
    }
    // If there is Ally ahead, save it as cover position
    else if (board[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 2] == ally){
        piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol + 2) * 75;
        piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow + 1) * 75;
        piece[piece_moves].cover_cx++;
        piece[piece_moves].cover_cy++;
    }
    // 2 Right 1 Upper Movement
    // If there is no one, save it as a valid move
    if ((board[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 2] == 0
        || board[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 2] == enemy) &&
        piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol + 2 < 8){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 2) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
        piece[piece_moves].cy++;
        // If there is an enemy ahead, save it as a kill position
        if (board[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 2] == enemy){
            die_x[x] = (piece[piece_moves].bcol + 2) * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow - 1) * 75;
            y++;
        }
    }
    // If there is Ally ahead, save it as cover position
    else if (board[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 2] == ally){
        piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol + 2) * 75;
        piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow - 1) * 75;
        piece[piece_moves].cover_cx++;
        piece[piece_moves].cover_cy++;
    }
    // 2 Left 1 Lower Movement
    // If there is no one, save it as a valid move
    if ((board[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 2] == 0
        || board[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 2] == enemy) &&
        piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol - 2 >= 0){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 2) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
        piece[piece_moves].cy++;
        // If there is an enemy ahead, save it as a kill position
        if (board[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 2] == enemy){
            die_x[x] = (piece[piece_moves].bcol - 2) * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow + 1) * 75;
            y++;
        }
    }
    // If there is Ally ahead, save it as cover position
    else if (board[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 2] == ally){
        piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol - 2) * 75;
        piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow + 1) * 75;
        piece[piece_moves].cover_cx++;
        piece[piece_moves].cover_cy++;
    }
    // 2 Left 1 Upper Movement
    // If there is no one, save it as a valid move
    if ((board[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 2] == 0
        || board[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 2] == enemy) &&
        piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol - 2 >= 0){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 2) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
        piece[piece_moves].cy++;
        // If there is an enemy ahead, save it as a kill position
        if (board[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 2] == enemy){
            die_x[x] = (piece[piece_moves].bcol - 2) * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow - 1) * 75;
            y++;
        }
    }
    // If there is Ally ahead, save it as cover position
    else if (board[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 2] == ally){
        piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol - 2) * 75;
        piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow - 1) * 75;
        piece[piece_moves].cover_cx++;
        piece[piece_moves].cover_cy++;
    }
}
//-----------------------------------------------//
// Rook Movement:
void rook(int piece_moves, int ally, int enemy, int p, int l){
    // Right Direction Movement
    for(p = (piece[piece_moves].bcol + 1); p < 8; p++){
        // If there is Ally ahead, save it as cover position
        if(board[piece[piece_moves].brow][p] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = p * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = piece[piece_moves].brow * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[piece[piece_moves].brow][p] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[piece[piece_moves].brow][p] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cy++;
            die_x[x] = p * 75;
            x++;
            die_y[y] = piece[piece_moves].brow * 75;
            y++;
            break;
        }
    }
    // Left Direction Movement
    for(p = (piece[piece_moves].bcol - 1); p >= 0; p--){
        // If there is Ally ahead, save it as cover position
        if(board[piece[piece_moves].brow][p] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = p * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = piece[piece_moves].brow * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[piece[piece_moves].brow][p] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[piece[piece_moves].brow][p] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cy++;
            die_x[x] = p * 75;
            x++;
            die_y[y] = piece[piece_moves].brow * 75;
            y++;
            break;
        }
    }
    // Lower Movement
    for(p = (piece[piece_moves].brow + 1); p < 8; p++){
        // If there is Ally ahead, save it as cover position
        if(board[p][piece[piece_moves].bcol] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = piece[piece_moves].bcol * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = p * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[p][piece[piece_moves].bcol] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = p * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[p][piece[piece_moves].bcol] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = p * 75;
            piece[piece_moves].cy++;
            die_x[x] = piece[piece_moves].bcol * 75;
            x++;
            die_y[y] = p * 75;
            y++;
            break;
        }
    }
    // Upper Movement
    for(p = (piece[piece_moves].brow - 1); p >= 0; p--){
        // If there is Ally ahead, save it as cover position
        if(board[p][piece[piece_moves].bcol] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = piece[piece_moves].bcol * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = p * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[p][piece[piece_moves].bcol] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = piece[piece_moves].bcol * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = p * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[p][piece[piece_moves].bcol] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = piece[piece_moves].bcol * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = p * 75;
            piece[piece_moves].cy++;
            die_x[x] = piece[piece_moves].bcol * 75;
            x++;
            die_y[y] = p * 75;
            y++;
            break;
        }
    }
}
//-----------------------------------------------//
// King Movement:
void king(int piece_moves, int ally, int enemy, int p, int l){
    int j, temp_bcol, temp_brow, temp_b1, temp_b2;
    // Stopping the loop if the piece which the player chose equal to the same the king's piece
    if(piece_moves == chosen_piece){
        // Check all enemy's valid moves which attack the king's valid moves
        for(s; s < e; s++){
            // If the king was checked, consider this action
            if(game_prop.w_check || game_prop.b_check){
                temp_bcol = piece[chosen_piece].bcol;
                temp_brow = piece[chosen_piece].brow;
                temp_b1 = board[piece[chosen_piece].brow][piece[chosen_piece].bcol];
                temp_b2 = board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol];
                board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
            }
            // Get the valid moves the enemy piece
            get_valid_moves(s);
            // If there was a check, remove the action which was considered
            if(game_prop.w_check || game_prop.b_check){
                board[temp_brow][temp_bcol] = temp_b1;
                board_types[temp_brow][temp_bcol] = temp_b2;
            }
            // Merge the cover moves with valid moves
            append(s);
            // If the piece was a black pawn and white king, take the following action
            if(piece[s].type == 'p' && s < 16){
                // Check the intersection between the black pawn valid moves and white king valid moves
                // Then remove the intersected moves as the king will be checked
                // Right Direction Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol + 1 &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol + 1 &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow)){
                    k1 = 1;
                }
                // Left Direction Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol - 1 &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol - 1 &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow)){
                    k2 = 1;
                }
                // Lower Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol  &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow + 1) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow + 1)){
                    k3 = 1;
                }
                // Upper Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol  &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow - 1) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow - 1)){
                    k4 = 1;
                }
                // Upper Right Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol + 1 &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow - 1)||(((piece[s].x) / 75) + 1 == piece[piece_moves].bcol + 1 &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow - 1)){
                    k5 = 1;
                }
                // Upper Left Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol - 1 &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow - 1)||(((piece[s].x) / 75) + 1 == piece[piece_moves].bcol - 1 &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow - 1)){
                    k6 = 1;
                }
                // Lower Right Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol + 1 &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow + 1)||(((piece[s].x) / 75) + 1 == piece[piece_moves].bcol + 1 &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow + 1)){
                    k7 = 1;
                }
                // Lower Left Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol - 1 &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow + 1) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol - 1 &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow + 1)){
                    k8 = 1;
                }
                // 2 Left Direction Movement or 1 Left Direction Movement (White King Castling on The Left Side)
                if((((piece[s].x / 75) - 1 == piece[piece_moves].bcol - 1 ||
                (piece[s].x / 75) - 1 == piece[piece_moves].bcol - 2) || (((piece[s].x / 75) + 1 == piece[piece_moves].bcol - 1 ||
                (piece[s].x / 75) + 1 == piece[piece_moves].bcol - 2))) &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow){
                    k9 = 1;
                }
                // 2 Right Direction Movement or 1 Right Direction Movement (White King Castling on The Right Side)
                if((((piece[s].x / 75) - 1 == piece[piece_moves].bcol + 1 ||
                (piece[s].x / 75) - 1 == piece[piece_moves].bcol + 2) || (((piece[s].x / 75) + 1 == piece[piece_moves].bcol + 1 ||
                (piece[s].x / 75) + 1 == piece[piece_moves].bcol + 2))) &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow){
                    k11 = 1;
                }
            }
            // If the piece was a white pawn and black king, take the following action
            else if(piece[s].type == 'p' && s >= 16){
                // Check the intersection between the white pawn valid moves and black king valid moves
                // Then remove the intersected moves as the king will be checked
                // Right Direction Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol + 1 &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol + 1 &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow)){
                    k1 = 1;
                }
                // Left Direction Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol - 1 &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol - 1 &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow)){
                    k2 = 1;
                }
                // Lower Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol  &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow + 1) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow + 1)){
                    k3 = 1;
                }
                // Upper Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol  &&
                (piece[s].y/ 75) - 1 == piece[piece_moves].brow - 1) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol &&
                (piece[s].y/ 75) - 1 == piece[piece_moves].brow - 1)){
                    k4 = 1;
                }
                // Upper Right Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol + 1 &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow - 1) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol + 1 &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow - 1)){
                    k5 = 1;
                }
                // Upper Left Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol - 1 &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow - 1) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol - 1 &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow - 1)){
                    k6 = 1;
                }
                // Lower Right Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol + 1 &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow + 1) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol + 1 &&
                (piece[s].y/ 75) - 1 == piece[piece_moves].brow + 1)){
                    k7 = 1;
                }
                // Lower Left Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol - 1 &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow + 1) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol - 1 &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow + 1)){
                    k8 = 1;
                }
                // 2 Left Direction Movement or 1 Left Direction Movement (Black King Castling on The Left Side)
                if((((piece[s].x / 75) - 1 == piece[piece_moves].bcol - 1 ||
                (piece[s].x / 75) - 1 == piece[piece_moves].bcol - 2) || (((piece[s].x / 75) + 1 == piece[piece_moves].bcol - 1 ||
                (piece[s].x / 75) + 1 == piece[piece_moves].bcol - 2))) &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow){
                    k10 = 1;
                }
                // 2 Right Direction Movement or 1 Right Direction Movement (Black King Castling on The Right Side)
                if((((piece[s].x / 75) - 1 == piece[piece_moves].bcol + 1 ||
                (piece[s].x / 75) - 1 == piece[piece_moves].bcol + 2) || (((piece[s].x / 75) + 1 == piece[piece_moves].bcol + 1 ||
                (piece[s].x / 75) + 1 == piece[piece_moves].bcol + 2))) &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow){
                    k12 = 1;
                }
            }else{
                // Loop for the rest of the pieces
                for(j = 0; j < piece[s].cx; j++){
                    // Right Direction Movement
                    if((piece[s].valid_moves_x[j]) / 75 == piece[piece_moves].bcol + 1 &&
                    (piece[s].valid_moves_y[j] / 75) == piece[piece_moves].brow){
                        k1 = 1;
                    }
                    // Left Direction Movement
                    if((piece[s].valid_moves_x[j]) / 75 == piece[piece_moves].bcol - 1 &&
                    (piece[s].valid_moves_y[j] / 75) == piece[piece_moves].brow){
                        k2 = 1;
                    }
                    // Lower Movement
                    if((piece[s].valid_moves_x[j]) / 75 == piece[piece_moves].bcol &&
                    (piece[s].valid_moves_y[j] / 75) == piece[piece_moves].brow + 1){
                        k3 = 1;
                    }
                    // Upper Movement
                    if((piece[s].valid_moves_x[j]) / 75 == piece[piece_moves].bcol &&
                    (piece[s].valid_moves_y[j] / 75) == piece[piece_moves].brow - 1){
                        k4 = 1;
                    }
                    // Upper Right Movement
                    if((piece[s].valid_moves_x[j]) / 75 == piece[piece_moves].bcol + 1 &&
                    (piece[s].valid_moves_y[j] / 75) == piece[piece_moves].brow - 1){
                        k5 = 1;
                    }
                    // Upper Left Movement
                    if((piece[s].valid_moves_x[j]) / 75 == piece[piece_moves].bcol - 1 &&
                    (piece[s].valid_moves_y[j] / 75) == piece[piece_moves].brow - 1){
                        k6 = 1;
                    }
                    // Lower Right Movement
                    if((piece[s].valid_moves_x[j]) / 75 == piece[piece_moves].bcol + 1 &&
                    (piece[s].valid_moves_y[j] / 75) == piece[piece_moves].brow + 1){
                        k7 = 1;
                    }
                    // Lower Left Movement
                    if((piece[s].valid_moves_x[j]) / 75 == piece[piece_moves].bcol - 1 &&
                    (piece[s].valid_moves_y[j] / 75) == piece[piece_moves].brow + 1){
                        k8 = 1;
                    }
                    // 2 Left Direction Movement or 1 Left Direction Movement (White King Castling on The Left Side)
                    if((piece[s].valid_moves_x[j] / 75 == piece[piece_moves].bcol - 1 ||
                    (piece[s].valid_moves_x[j]) / 75 == piece[piece_moves].bcol - 2) &&
                    ((piece[s].valid_moves_y[j] / 75) == piece[piece_moves].brow && s < 16)){
                        k9 = 1;
                    }
                    // 2 Left Direction Movement or 1 Left Direction Movement (Black King Castling on The Left Side)
                    if((piece[s].valid_moves_x[j] / 75 == piece[piece_moves].bcol - 1 ||
                    (piece[s].valid_moves_x[j]) / 75 == piece[piece_moves].bcol - 2) &&
                    ((piece[s].valid_moves_y[j] / 75) == piece[piece_moves].brow && s >= 16)){
                        k10 = 1;
                    }
                    // 2 Right Direction Movement or 1 Right Direction Movement (White King Castling on The Right Side)
                    if((piece[s].valid_moves_x[j] / 75 == piece[piece_moves].bcol + 1 ||
                    (piece[s].valid_moves_x[j]) / 75 == piece[piece_moves].bcol + 2) &&
                    ((piece[s].valid_moves_y[j] / 75) == piece[piece_moves].brow && s < 16)){
                        k11 = 1;
                    }
                    // 2 Right Direction Movement or 1 Right Direction Movement (Black King Castling on The Right Side)
                    if((piece[s].valid_moves_x[j] / 75 == piece[piece_moves].bcol + 1 ||
                    (piece[s].valid_moves_x[j]) / 75 == piece[piece_moves].bcol + 2) &&
                    ((piece[s].valid_moves_y[j] / 75) == piece[piece_moves].brow && s >= 16)){
                        k12 = 1;
                    }
                }
            }
            // Reset the arrays' elements and counters
            for(j = 0; j < 63; j++){
                piece[s].valid_moves_x[j] = -10;
                piece[s].valid_moves_y[j] = -10;
                piece[s].valid_cover_x[j] = -10;
                piece[s].valid_cover_y[j] = -10;
            }
            piece[s].cx = 0;
            piece[s].cy = 0;
            piece[s].cover_cx = 0;
            piece[s].cover_cy = 0;
        }
        if(board[piece[piece_moves].brow][piece[piece_moves].bcol + 1] == ally){
            // Do nothing if there is an ally
        }
        // Check if the move is valid or not for every move
        if(!k1){
            // Right Direction Movement
            // If there is no one, save it as valid position
            if(board[piece[piece_moves].brow][piece[piece_moves].bcol + 1] == 0 &&
                piece[piece_moves].brow < 8 && piece[piece_moves].bcol + 1 < 8){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
                piece[piece_moves].cy++;
            }
            // If there is an enemy ahead, save it as a kill position
            else if(board[piece[piece_moves].brow][piece[piece_moves].bcol + 1] == enemy &&
                piece[piece_moves].brow < 8 && piece[piece_moves].bcol + 1 < 8){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
                piece[piece_moves].cy++;
                die_x[x] = (piece[piece_moves].bcol + 1) * 75;
                x++;
                die_y[y] = piece[piece_moves].brow * 75;
                y++;
            }
        }else{
            // Reset the flag value
            k1 = 0;
        }
        if(board[piece[piece_moves].brow][piece[piece_moves].bcol - 1] == ally){
            // Do nothing if there is an ally
        }
        if(!k2){
            // Left Direction Movement
            // If there is no one, save it as valid position
            if(board[piece[piece_moves].brow][piece[piece_moves].bcol - 1] == 0 &&
                piece[piece_moves].brow < 8 && piece[piece_moves].bcol - 1 >= 0){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
                piece[piece_moves].cy++;
            }
            // If there is an enemy ahead, save it as a kill position
            else if(board[piece[piece_moves].brow][piece[piece_moves].bcol - 1] == enemy &&
                piece[piece_moves].brow < 8 && piece[piece_moves].bcol - 1 >= 0){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
                piece[piece_moves].cy++;
                die_x[x] = (piece[piece_moves].bcol - 1) * 75;
                x++;
                die_y[y] = piece[piece_moves].brow * 75;
                y++;
            }
        }else{
            // Reset the flag value
            k2 = 0;
        }
        if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol] == ally){
            // Do nothing if there is an ally
        }
        if(!k3){
            // Lower Movement
            // If there is no one, save it as valid position
            if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol] == 0 &&
                piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol < 8){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
                piece[piece_moves].cy++;
            }
            // If there is an enemy ahead, save it as a kill position
            else if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol] == enemy &&
                piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol < 8){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
                piece[piece_moves].cy++;
                die_x[x] = piece[piece_moves].bcol * 75;
                x++;
                die_y[y] = (piece[piece_moves].brow + 1) * 75;
                y++;
            }
        }else{
            // Reset the flag value
            k3 = 0;
        }
        if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol] == ally){
            // Do nothing if there is an ally
        }
        if(!k4){
            // Upper Movement
            // If there is no one, save it as valid position
            if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol] == 0 &&
                piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol < 8){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = piece[piece_moves].bcol * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
                piece[piece_moves].cy++;
            }
            // If there is an enemy ahead, save it as a kill position
            else if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol] == enemy &&
                piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol < 8){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = piece[piece_moves].bcol * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
                piece[piece_moves].cy++;
                die_x[x] = piece[piece_moves].bcol * 75;
                x++;
                die_y[y] = (piece[piece_moves].brow - 1) * 75;
                y++;
            }
        }else{
            // Reset the flag value
            k4 = 0;
        }
        if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 1] == ally){
            // Do nothing if there is an ally
        }
        if(!k5){
            // Upper Right Movement
            // If there is no one, save it as valid position
            if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 1] == 0 &&
                piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol + 1 < 8){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
                piece[piece_moves].cy++;
            }
            // If there is an enemy ahead, save it as a kill position
            else if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 1] == enemy &&
                piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol + 1 < 8){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
                piece[piece_moves].cy++;
                die_x[x] = (piece[piece_moves].bcol + 1) * 75;
                x++;
                die_y[y] = (piece[piece_moves].brow - 1) * 75;
                y++;
            }
        }else{
            // Reset the flag value
            k5 = 0;
        }
        if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 1] == ally){
            // Do nothing if there is an ally
        }
        if(!k6){
            // Upper Left Movement
            // If there is no one, save it as valid position
            if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 1] == 0 &&
                piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol - 1 >= 0){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
                piece[piece_moves].cy++;
            }
            // If there is an enemy ahead, save it as a kill position
            else if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 1] == enemy &&
                piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol - 1 >= 0){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
                piece[piece_moves].cy++;
                die_x[x] = (piece[piece_moves].bcol - 1) * 75;
                x++;
                die_y[y] = (piece[piece_moves].brow - 1) * 75;
                y++;
            }
        }else{
            // Reset the flag value
            k6 = 0;
        }
        if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 1] == ally){
            // Do nothing if there is an ally
        }
        if(!k7){
            // Lower Right Movement
            // If there is no one, save it as valid position
            if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 1] == 0 &&
                piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol + 1 < 8){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
                piece[piece_moves].cy++;
            }
            // If there is an enemy ahead, save it as a kill position
            else if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 1] == enemy &&
                piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol + 1 < 8){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
                piece[piece_moves].cy++;
                die_x[x] = (piece[piece_moves].bcol + 1) * 75;
                x++;
                die_y[y] = (piece[piece_moves].brow + 1) * 75;
                y++;
            }
        }else{
            // Reset the flag value
            k7 = 0;
        }
        if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 1] == ally){
            // Do nothing if there is an ally
        }
        if(!k8){
            // Lower Left Movement
            // If there is no one, save it as valid position
            if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 1] == 0 &&
                piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol - 1 >= 0){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
                piece[piece_moves].cy++;
            }
            // If there is an enemy ahead, save it as a kill position
            else if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 1] == enemy &&
                piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol - 1 >= 0){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
                piece[piece_moves].cy++;
                die_x[x] = (piece[piece_moves].bcol - 1) * 75;
                x++;
                die_y[y] = (piece[piece_moves].brow + 1) * 75;
                y++;
            }
        }else{
            // Reset the flag value
            k8 = 0;
        }
        if(!k11){
            // White King Castling on The Right Side
            if(piece[piece_moves].brow == 7 && piece[piece_moves].bcol == 4
                && piece[piece_moves].brow_prev == -1 && piece[piece_moves].bcol_prev == -1
                && board_types[piece[piece_moves].brow][piece[piece_moves].bcol + 3] == wr
                && piece[31].brow_prev == -1 && piece[31].bcol_prev == -1 && board[7][6] == 0 && board[7][5] == 0){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 2) * 75;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].cy++;
                game_prop.wrc_w_k = 1;
            }
        }else{
            // Reset the flag value
            k11 = 0;
        }
        if(!k9){
            // White King Castling on The Left Side
            if(piece[piece_moves].brow == 7 && piece[piece_moves].bcol == 4
                && piece[piece_moves].brow_prev == -1 && piece[piece_moves].bcol_prev == -1
                && board_types[piece[piece_moves].brow][piece[piece_moves].bcol - 4] == wr
                && piece[24].brow_prev == -1 && piece[24].bcol_prev == -1 && board[7][1] == 0 && board[7][2] == 0 && board[7][3] == 0){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 2) * 75;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].cy++;
                game_prop.wlc_w_k = 1;
            }
        }else{
            // Reset the flag value
            k9 = 0;
        }
        if(!k12){
            // Black King Castling on The Right Side
            if(piece[piece_moves].brow == 0 && piece[piece_moves].bcol == 4
                && piece[piece_moves].brow_prev == -1 && piece[piece_moves].bcol_prev == -1
                && board_types[piece[piece_moves].brow][piece[piece_moves].bcol + 3] == br
                && piece[7].brow_prev == -1 && piece[7].bcol_prev == -1 && board[0][6] == 0 && board[0][5] == 0){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 2) * 75;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].cy++;
                game_prop.brc_w_k = 1;
            }
        }else{
            // Reset the flag value
            k12 = 0;
        }
        if(!k10){
            // Black King Castling on The Left Side
            if(piece[piece_moves].brow == 0 && piece[piece_moves].bcol == 4
                && piece[piece_moves].brow_prev == -1 && piece[piece_moves].bcol_prev == -1
                && board_types[piece[piece_moves].brow][piece[piece_moves].bcol - 4] == br
                && piece[0].brow_prev == -1 && piece[0].bcol_prev == -1 && board[0][1] == 0 && board[0][2] == 0 && board[0][3] == 0){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 2) * 75;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].cy++;
                game_prop.blc_w_k = 1;
            }
        }else{
            // Reset the flag value
            k10 = 0;
        }
    }
    // Checks the move of the king
    else{
        // Right Direction Movement
        // If there is Ally ahead, save it as cover position
        if(board[piece[piece_moves].brow][piece[piece_moves].bcol + 1] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol + 1) * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = piece[piece_moves].brow * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
        }
        // If there is no one, save it as valid position
        if(board[piece[piece_moves].brow][piece[piece_moves].bcol + 1] == 0 &&
            piece[piece_moves].brow < 8 && piece[piece_moves].bcol + 1 < 8){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[piece[piece_moves].brow][piece[piece_moves].bcol + 1] == enemy &&
            piece[piece_moves].brow < 8 && piece[piece_moves].bcol + 1 < 8){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cy++;
            die_x[x] = (piece[piece_moves].bcol + 1) * 75;
            x++;
            die_y[y] = piece[piece_moves].brow * 75;
            y++;
        }
        // Left Direction Movement
        // If there is Ally ahead, save it as cover position
        if(board[piece[piece_moves].brow][piece[piece_moves].bcol - 1] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol - 1) * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = piece[piece_moves].brow * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
        }
        // If there is no one, save it as valid position
        if(board[piece[piece_moves].brow][piece[piece_moves].bcol - 1] == 0 &&
            piece[piece_moves].brow < 8 && piece[piece_moves].bcol - 1 >= 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[piece[piece_moves].brow][piece[piece_moves].bcol - 1] == enemy &&
            piece[piece_moves].brow < 8 && piece[piece_moves].bcol - 1 >= 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cy++;
            die_x[x] = (piece[piece_moves].bcol - 1) * 75;
            x++;
            die_y[y] = piece[piece_moves].brow * 75;
            y++;
        }
        // Lower Movement
        // If there is Ally ahead, save it as cover position
        if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = piece[piece_moves].bcol * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow + 1) * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
        }
        // If there is no one, save it as valid position
        if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol] == 0 &&
            piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol < 8){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol] == enemy &&
            piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol < 8){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
            piece[piece_moves].cy++;
            die_x[x] = piece[piece_moves].bcol * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow + 1) * 75;
            y++;
        }
        // Upper Movement
        // If there is Ally ahead, save it as cover position
        if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = piece[piece_moves].bcol * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow - 1) * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
        }
        // If there is no one, save it as valid position
        if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol] == 0 &&
            piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol < 8){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = piece[piece_moves].bcol * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol] == enemy &&
            piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol < 8){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = piece[piece_moves].bcol * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
            piece[piece_moves].cy++;
            die_x[x] = piece[piece_moves].bcol * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow - 1) * 75;
            y++;
        }
        // Upper Right Movement
        // If there is Ally ahead, save it as cover position
        if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 1] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol + 1) * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow - 1) * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
        }
        // If there is no one, save it as valid position
        if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 1] == 0 &&
            piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol + 1 < 8){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 1] == enemy &&
            piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol + 1 < 8){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
            piece[piece_moves].cy++;
            die_x[x] = (piece[piece_moves].bcol + 1) * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow - 1) * 75;
            y++;
        }
        // Upper Left Movement
        // If there is Ally ahead, save it as cover position
        if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 1] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol - 1) * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow - 1) * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
        }
        // If there is no one, save it as valid position
        if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 1] == 0 &&
            piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol - 1 >= 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 1] == enemy &&
            piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol - 1 >= 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
            piece[piece_moves].cy++;
            die_x[x] = (piece[piece_moves].bcol - 1) * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow - 1) * 75;
            y++;
        }
        // Lower Right Movement
        // If there is Ally ahead, save it as cover position
        if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 1] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol + 1) * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow + 1) * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
        }
        // If there is no one, save it as valid position
        if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 1] == 0 &&
            piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol + 1 < 8){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 1] == enemy &&
            piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol + 1 < 8){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
            piece[piece_moves].cy++;
            die_x[x] = (piece[piece_moves].bcol + 1) * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow + 1) * 75;
            y++;
        }
        // Lower Left Movement
        // If there is Ally ahead, save it as cover position
        if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 1] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol - 1) * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow + 1) * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
        }
        // If there is no one, save it as valid position
        if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 1] == 0 &&
            piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol - 1 >= 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 1] == enemy &&
            piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol - 1 >= 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
            piece[piece_moves].cy++;
            die_x[x] = (piece[piece_moves].bcol - 1) * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow + 1) * 75;
            y++;
        }
        // Right King Castling on The Right Side
        if(piece[piece_moves].brow == 7 && piece[piece_moves].bcol == 4
            && piece[piece_moves].brow_prev == -1 && piece[piece_moves].bcol_prev == -1
            && board_types[piece[piece_moves].brow][piece[piece_moves].bcol + 3] == wr
            && piece[31].brow_prev == -1 && piece[31].bcol_prev == -1 && board[7][6] == 0 && board[7][5] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 2) * 75;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].cy++;
            game_prop.wrc_w_k = 1;
        }
        // Right King Castling on The Left Side
        if(piece[piece_moves].brow == 7 && piece[piece_moves].bcol == 4
            && piece[piece_moves].brow_prev == -1 && piece[piece_moves].bcol_prev == -1
            && board_types[piece[piece_moves].brow][piece[piece_moves].bcol - 4] == wr
            && piece[24].brow_prev == -1 && piece[24].bcol_prev == -1 && board[7][1] == 0 && board[7][2] == 0 && board[7][3] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 2) * 75;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].cy++;
            game_prop.wlc_w_k = 1;
        }
        // Black King Castling on The Right Side
        if(piece[piece_moves].brow == 0 && piece[piece_moves].bcol == 4
            && piece[piece_moves].brow_prev == -1 && piece[piece_moves].bcol_prev == -1
            && board_types[piece[piece_moves].brow][piece[piece_moves].bcol + 3] == br
            && piece[7].brow_prev == -1 && piece[7].bcol_prev == -1 && board[0][6] == 0 && board[0][5] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 2) * 75;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].cy++;
            game_prop.brc_w_k = 1;
        }
        // Black King Castling on The Left Side
        if(piece[piece_moves].brow == 0 && piece[piece_moves].bcol == 4
            && piece[piece_moves].brow_prev == -1 && piece[piece_moves].bcol_prev == -1
            && board_types[piece[piece_moves].brow][piece[piece_moves].bcol - 4] == br
            && piece[0].brow_prev == -1 && piece[0].bcol_prev == -1 && board[0][1] == 0 && board[0][2] == 0 && board[0][3] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 2) * 75;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].cy++;
            game_prop.blc_w_k = 1;
        }
    }
}

//#-------------------------------------------------------------------------#

// Check the validity of the player's move
int check_valid(void){
    int i1, i2;
    for (i1 = 0, i2 = 0; i1 < piece[chosen_piece].cx, i2 < piece[chosen_piece].cy; i1++, i2++){
        // If the move is valid return one
        if (i2 == i1 && piece[chosen_piece].valid_moves_x[i1] == piece[chosen_piece].xtemp &&
        piece[chosen_piece].valid_moves_y[i2] == piece[chosen_piece].ytemp){
            return 1;
        }
    }
    // If the move is not valid return one
    return 0;
}

//#-------------------------------------------------------------------------#

// Check If the player's move is resulting in a dead piece
int check_die(void){
    int i, j;
    for (i = 0, j = 0; i < x, j < y; i++, j++){
        // If There is a dead piece return 1
        if (i == j && die_x[i] == piece[chosen_piece].xtemp && die_y[j] == piece[chosen_piece].ytemp){
            return 1;
        }
        // If there is en passant kill return 1 (White Pawn Kill Black Pawn)
        else if (board_types[piece[chosen_piece].brow + 1][piece[chosen_piece].bcol] == bp
                    && board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] == wp){
            return 1;
        }
        // If there is en passant kill return 1 (Black Pawn Kill White Pawn)
        else if (board_types[piece[chosen_piece].brow - 1][piece[chosen_piece].bcol] == wp
                    && board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] == bp){
            return 1;
        }
    }
    // If there is no dead pieces return 0
    return 0;
}

//#-------------------------------------------------------------------------#

// find the line of the enemy which attacks the king
void check_enemy_line(void){
    int l, p;
    // Positive Horizontal Line
    if(l1){
        for(p = (piece[game_prop.king_check].bcol + 1); p < 8; p++){
            // If the enemy on this line, equal flag to one
            if(((piece[game_prop.king_check].brow == piece[game_prop.T_piece].brow) && (p == piece[game_prop.T_piece].bcol))){
                enemy_line1 = 1;
                break;
            }
            // If The enemy not on this line, equal flag to zero
            else{
                enemy_line1 = 0;
            }
        }
    }
    // Negative Horizontal Line
    if(l2){
        for(p = (piece[game_prop.king_check].bcol - 1); p >= 0; p--){
            // If the enemy on this line, equal flag to one
            if(((piece[game_prop.king_check].brow == piece[game_prop.T_piece].brow) && (p == piece[game_prop.T_piece].bcol))){
                enemy_line2 = 1;
                break;
            }
            // If The enemy not on this line, equal flag to zero
            else{
                enemy_line2 = 0;
            }
        }
    }
    // Positive Vertical Line
    if(l3){
        for(p = (piece[game_prop.king_check].brow + 1); p < 8; p++){
            // If the enemy on this line, equal flag to one
            if(((piece[game_prop.T_piece].brow == p) && (piece[game_prop.king_check].bcol == piece[game_prop.T_piece].bcol))){
                enemy_line3 = 1;
                break;
            }
            // If The enemy not on this line, equal flag to zero
            else{
                enemy_line3 = 0;
            }
        }
    }
    // Negative Vertical Line
    if(l4){
        for(p = (piece[game_prop.king_check].brow - 1); p >= 0; p--){
            // If the enemy on this line, equal flag to one
            if(((piece[game_prop.T_piece].brow == p) && (piece[game_prop.king_check].bcol == piece[game_prop.T_piece].bcol))){
                enemy_line4 = 1;
                break;
            }
            // If The enemy not on this line, equal flag to zero
            else{
                enemy_line4 = 0;
            }
        }
    }
    // Upper Right Diagonal Line
    if(l5){
        for(p = piece[game_prop.king_check].bcol + 1, l = piece[game_prop.king_check].brow - 1 ; p < 8 && l >= 0; p++, l--){
            // If the enemy on this line, equal flag to one
            if(((piece[game_prop.T_piece].brow == l) && (p == piece[game_prop.T_piece].bcol))){
                enemy_line5 = 1;
                break;
            }
            // If The enemy not on this line, equal flag to zero
            else{
                enemy_line5 = 0;
            }
        }
    }
    // Upper Left Diagonal Line
    if(l6){
        for(p = piece[game_prop.king_check].bcol - 1, l = piece[game_prop.king_check].brow - 1; p >= 0 && l >= 0; p--, l--){
            // If the enemy on this line, equal flag to one
            if(((piece[game_prop.T_piece].brow == l) && (p == piece[game_prop.T_piece].bcol))){
                enemy_line6 = 1;
                break;
            }
            // If The enemy not on this line, equal flag to zero
            else{
                enemy_line6 = 0;
            }
        }
    }
    // Lower Right Diagonal Line
    if(l7){
        for(p = piece[game_prop.king_check].bcol + 1, l = piece[game_prop.king_check].brow + 1; p < 8 && l < 8; p++, l++){
            // If the enemy on this line, equal flag to one
            if(((piece[game_prop.T_piece].brow == l) && (p == piece[game_prop.T_piece].bcol))){
                enemy_line7 = 1;
                break;
            }
            // If The enemy not on this line, equal flag to zero
            else{
                enemy_line7 = 0;
            }
        }
    }
    // Lower Left Diagonal Line
    if(l8){
        for(p = piece[game_prop.king_check].bcol - 1,l = piece[game_prop.king_check].brow + 1;p >= 0 && l < 8; p--, l++){
            // If the enemy on this line, equal flag to one
            if(((piece[game_prop.T_piece].brow == l) && (p == piece[game_prop.T_piece].bcol))){
                enemy_line8 = 1;
                break;
            }
            // If The enemy not on this line, equal flag to zero
            else{
                enemy_line8 = 0;
            }
        }
    }
}

//#-------------------------------------------------------------------------#

// Find the intersection of the king's line and enemy and record the valid moves to stop the attack
void check_king_line(int ally, int enemy){
    // Call the function to find the line of the enemy which attacks the king
    check_enemy_line();
    int l, p;
    // Positive Horizontal Line
    if(l1 && enemy_line1){
        for(p = (piece[game_prop.king_check].bcol + 1); p < 8; p++){
            // If there is an ally on the line move, stop this loop
            if(board[piece[game_prop.king_check].brow][p] == ally){
                break;
            }
            // If there is an enemy, confirm this line and save the valid moves on this line
            // If there is no one, keep searching until there is no valid moves on this line
            else if(board[piece[game_prop.king_check].brow][p] == 0 || board[piece[game_prop.king_check].brow][p] == enemy){
                line_moves_x[line_x] = p * 75;
                line_x++;
                line_moves_y[line_y] = (piece[game_prop.king_check].brow) * 75;
                line_y++;
                if(board[piece[game_prop.king_check].brow][p] == enemy){
                    break;
                }
            }
        }
        // Reset the flags Value
        l1 = 0;
        enemy_line1 = 0;
    }
    // Negative Horizontal Line
    if(l2 && enemy_line2){
        for(p = (piece[game_prop.king_check].bcol - 1); p >= 0; p--){
            // If there is an ally on the line move, stop this loop
            if(board[piece[game_prop.king_check].brow][p] == ally){
                break;
            }
            // If there is an enemy, confirm this line and save the valid moves on this line
            // If there is no one, keep searching until there is no valid moves on this line
            else if(board[piece[game_prop.king_check].brow][p] == 0 || board[piece[game_prop.king_check].brow][p] == enemy){
                line_moves_x[line_x] = p * 75;
                line_x++;
                line_moves_y[line_y] = (piece[game_prop.king_check].brow) * 75;
                line_y++;
                if(board[piece[game_prop.king_check].brow][p] == enemy){
                    break;
                }
            }
        }
        // Reset the flags Value
        l2 = 0;
        enemy_line2 = 0;
    }
    // Positive Vertical Line
    if(l3 && enemy_line3){
        for(p = (piece[game_prop.king_check].brow + 1); p < 8; p++){
            // If there is an ally on the line move, stop this loop
            if(board[p][piece[game_prop.king_check].bcol] == ally){
                break;
            }
            // If there is an enemy, confirm this line and save the valid moves on this line
            // If there is no one, keep searching until there is no valid moves on this line
            else if(board[p][piece[game_prop.king_check].bcol] == 0 || board[p][piece[game_prop.king_check].bcol] == enemy){
                line_moves_x[line_x] = (piece[game_prop.king_check].bcol) * 75;
                line_x++;
                line_moves_y[line_y] = p * 75;
                line_y++;
                if(board[p][piece[game_prop.king_check].bcol] == enemy){
                    break;
                }
            }
        }
        // Reset the flags Value
        l3 = 0;
        enemy_line3 = 0;
    }
    // Negative Vertical Line
    if(l4 && enemy_line4){
        for(p = (piece[game_prop.king_check].brow - 1); p >= 0; p--){
            // If there is an ally on the line move, stop this loop
            if(board[p][piece[game_prop.king_check].bcol] == ally){
                break;
            }
            // If there is an enemy, confirm this line and save the valid moves on this line
            // If there is no one, keep searching until there is no valid moves on this line
            else if(board[p][piece[game_prop.king_check].bcol] == 0 || board[p][piece[game_prop.king_check].bcol] == enemy){
                line_moves_x[line_x] = piece[game_prop.king_check].bcol * 75;
                line_x++;
                line_moves_y[line_y] = p * 75;
                line_y++;
                if(board[p][piece[game_prop.king_check].bcol] == enemy){
                    break;
                }
            }
        }
        // Reset the flags Value
        l4 = 0;
        enemy_line4 = 0;
    }
    // Upper Right Diagonal Line
    if(l5 && enemy_line5){
        for(p = piece[game_prop.king_check].bcol + 1, l = piece[game_prop.king_check].brow - 1 ; p < 8 && l >= 0; p++, l--){
            // If there is an ally on the line move, stop this loop
            if(board[l][p] == ally){
                break;
            }
            // If there is an enemy, confirm this line and save the valid moves on this line
            // If there is no one, keep searching until there is no valid moves on this line
            else if(board[l][p] == 0 || board[l][p] == enemy){
                line_moves_x[line_x] = p * 75;
                line_x++;
                line_moves_y[line_y] = l * 75;
                line_y++;
                if(board[l][p] == enemy){
                    break;
                }
            }
        }
        // Reset the flags Value
        l5 = 0;
        enemy_line5 = 0;
    }
    // Upper Left Diagonal Line
    if(l6 && enemy_line6){
        for(p = piece[game_prop.king_check].bcol - 1, l = piece[game_prop.king_check].brow - 1; p >= 0 && l >= 0; p--, l--){
            // If there is an ally on the line move, stop this loop
            if(board[l][p] == ally){
                break;
            }
            // If there is an enemy, confirm this line and save the valid moves on this line
            // If there is no one, keep searching until there is no valid moves on this line
            else if(board[l][p] == 0 || board[l][p] == enemy){
                line_moves_x[line_x] = p * 75;
                line_x++;
                line_moves_y[line_y] = l * 75;
                line_y++;
                if(board[l][p] == enemy){
                    break;
                }
            }
        }
        // Reset the flags Value
        l6 = 0;
        enemy_line6 = 0;
    }
    // Lower Right Diagonal Line
    if(l7 && enemy_line7){
        for(p = piece[game_prop.king_check].bcol + 1, l = piece[game_prop.king_check].brow + 1; p < 8 && l < 8; p++, l++){
            // If there is an ally on the line move, stop this loop
            if(board[l][p] == ally){
                break;
            }
            // If there is an enemy, confirm this line and save the valid moves on this line
            // If there is no one, keep searching until there is no valid moves on this line
            else if(board[l][p] == 0 || board[l][p] == enemy){
                line_moves_x[line_x] = p * 75;
                line_x++;
                line_moves_y[line_y] = l * 75;
                line_y++;
                if(board[l][p] == enemy){
                    break;
                }
            }
        }
        // Reset the flags Value
        l7 = 0;
        enemy_line7 = 0;
    }
    // Lower Left Diagonal Line
    if(l8 && enemy_line8){
        for(p = piece[game_prop.king_check].bcol - 1,l = piece[game_prop.king_check].brow + 1;p >= 0 && l < 8; p--, l++){
            // If there is an ally on the line move, stop this loop
            if(board[l][p] == ally){
                break;
            }
            // If there is an enemy, confirm this line and save the valid moves on this line
            // If there is no one, keep searching until there is no valid moves on this line
            else if(board[l][p] == 0 || board[l][p] == enemy){
                line_moves_x[line_x] = p * 75;
                line_x++;
                line_moves_y[line_y] = l * 75;
                line_y++;
                if(board[l][p] == enemy){
                    break;
                }
            }
        }
        // Reset the flags Value
        l8 = 0;
        enemy_line8 = 0;
    }
}

// Find the position of the knight which attack the king, and record it
void king_knight_att(void){
    int j;
    for(j = 0; j < piece[game_prop.T_piece].cx; j++){
        // Lower Right Direction
        if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow) &&
        (piece[game_prop.T_piece].bcol == piece[game_prop.king_check].bcol + 1 &&
        piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow + 2)){
            line_moves_x[line_x] = (piece[game_prop.king_check].bcol + 1) * 75;
            line_x++;
            line_moves_y[line_y] = (piece[game_prop.king_check].brow + 2) * 75;
            line_y++;
        }
        // Upper Right Direction
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow) &&
        (piece[game_prop.T_piece].bcol == piece[game_prop.king_check].bcol + 1 &&
        piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow - 2)){
            line_moves_x[line_x] = (piece[game_prop.king_check].bcol + 1) * 75;
            line_x++;
            line_moves_y[line_y] = (piece[game_prop.king_check].brow - 2) * 75;
            line_y++;
        }
        // Lower Left Direction
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow) &&
        (piece[game_prop.T_piece].bcol == piece[game_prop.king_check].bcol - 1 &&
        piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow + 2)){
            line_moves_x[line_x] = (piece[game_prop.king_check].bcol - 1) * 75;
            line_x++;
            line_moves_y[line_y] = (piece[game_prop.king_check].brow + 2) * 75;
            line_y++;
        }
        // Upper Left Direction
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow) &&
        (piece[game_prop.T_piece].bcol == piece[game_prop.king_check].bcol - 1 &&
        piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow - 2)){
            line_moves_x[line_x] = (piece[game_prop.king_check].bcol - 1) * 75;
            line_x++;
            line_moves_y[line_y] = (piece[game_prop.king_check].brow - 2) * 75;
            line_y++;
        }
        // Right Lower Direction
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow) &&
        (piece[game_prop.T_piece].bcol == piece[game_prop.king_check].bcol + 2 &&
        piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow + 1)){
            line_moves_x[line_x] = (piece[game_prop.king_check].bcol + 2) * 75;
            line_x++;
            line_moves_y[line_y] = (piece[game_prop.king_check].brow + 1) * 75;
            line_y++;
        }
        // Right Upper Direction
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow) &&
        (piece[game_prop.T_piece].bcol == piece[game_prop.king_check].bcol + 2 &&
        piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow - 1)){
            line_moves_x[line_x] = (piece[game_prop.king_check].bcol + 2) * 75;
            line_x++;
            line_moves_y[line_y] = (piece[game_prop.king_check].brow - 1) * 75;
            line_y++;
        }
        // Left Lower Direction
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow) &&
        (piece[game_prop.T_piece].bcol == piece[game_prop.king_check].bcol - 2 &&
        piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow + 1)){
            line_moves_x[line_x] = (piece[game_prop.king_check].bcol - 2) * 75;
            line_x++;
            line_moves_y[line_y] = (piece[game_prop.king_check].brow + 1) * 75;
            line_y++;
        }
        // Left Upper Direction
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow) &&
        (piece[game_prop.T_piece].bcol == piece[game_prop.king_check].bcol - 2 &&
        piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow - 1)){
            line_moves_x[line_x] = (piece[game_prop.king_check].bcol - 2) * 75;
            line_x++;
            line_moves_y[line_y] = (piece[game_prop.king_check].brow - 1) * 75;
            line_y++;
        }
    }
}

//#-------------------------------------------------------------------------#

// Find the Line Which the threaten piece attacks the king from it
void check_line(void){
    int j;
    // If the king on this line, equal line flag to one
    for(j = 0; j < piece[game_prop.T_piece].cx; j++){
        // Positive Horizontal Line
        if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol + 1 &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow) ||
        ((piece[game_prop.king_check].brow == piece[game_prop.T_piece].brow) && (piece[game_prop.king_check].bcol + 1 == piece[game_prop.T_piece].bcol))){
            l1 = 1;
        }
        // Negative Horizontal Line
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol - 1 &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow) ||
        ((piece[game_prop.king_check].brow == piece[game_prop.T_piece].brow) && (piece[game_prop.king_check].bcol - 1 == piece[game_prop.T_piece].bcol))){
            l2 = 1;
        }
        // Positive Vertical Line
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow + 1) ||
        ((piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow + 1) && (piece[game_prop.king_check].bcol == piece[game_prop.T_piece].bcol))){
            l3 = 1;
        }
        // Negative Vertical Line
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow - 1) ||
        ((piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow - 1) && (piece[game_prop.king_check].bcol == piece[game_prop.T_piece].bcol))){
            l4 = 1;
        }
        // Upper Right Diagonal Line
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol + 1 &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow - 1) ||
        ((piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow - 1) && (piece[game_prop.king_check].bcol + 1 == piece[game_prop.T_piece].bcol))){
            l5 = 1;
        }
        // Upper Left Diagonal Line
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol - 1 &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow - 1) ||
        ((piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow - 1) && (piece[game_prop.king_check].bcol - 1 == piece[game_prop.T_piece].bcol))){
            l6 = 1;
        }
        // Lower Right Diagonal Line
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol + 1 &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow + 1) ||
        ((piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow + 1) && (piece[game_prop.king_check].bcol + 1 == piece[game_prop.T_piece].bcol))){
            l7 = 1;
        }
        // Lower Left Diagonal Line
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol - 1 &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow + 1) ||
        ((piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow + 1) && (piece[game_prop.king_check].bcol - 1 == piece[game_prop.T_piece].bcol))){
            l8 = 1;
        }
    }
}

//#-------------------------------------------------------------------------#

// Compare piece's valid moves to the valid moves on king's line to cover it
void covered_matching(void){
    for(int o = 0; o < line_x; o++){
        for(int u = 0; u < piece[chosen_piece].cx; u++){
            // If there is equal values, save it
            if((piece[chosen_piece].valid_moves_x[u] == line_moves_x[o]) &&
            (piece[chosen_piece].valid_moves_y[u] == line_moves_y[o])){
                game_prop.covered = 1;
                valid_line_x[shield_x] = piece[chosen_piece].valid_moves_x[u];
                valid_line_y[shield_y] = piece[chosen_piece].valid_moves_y[u];
                shield_x++;
                shield_y++;
            }
        }
    }
    if(game_prop.covered == 1){
        // When there is equal values, record it to the piece's valid moves
        for(int q = 0; q < shield_x; q++){
            piece[chosen_piece].valid_moves_x[q] = valid_line_x[q];
            piece[chosen_piece].valid_moves_y[q] = valid_line_y[q];
        }
        game_prop.covered = 0;
    }
    // Modify array size of valid moves
    piece[chosen_piece].cx = shield_x;
    piece[chosen_piece].cy = shield_y;
}

//#-------------------------------------------------------------------------#

// Check if there is a piece which cover the king's piece from an attack
// So it stops its movement if it is exposing the king to a danger
void check_cover(void){
    int ally, enemy, set, end, temp_bcol, temp_brow, temp_b1, temp_b2, temp_T;
    if(chosen_piece == 4 || chosen_piece == 28){
        // Do nothing if the chosen piece is a king
    }else{
        // Determine the chosen piece and its enemies and allies
        if(16 <= chosen_piece && chosen_piece < 32){
            ally = 1;
            enemy = -1;
            game_prop.king_check = 28;
            set = 0;
            end = 16;
        }else if(0 <= chosen_piece && chosen_piece < 16){
            ally = -1;
            enemy = 1;
            game_prop.king_check = 4;
            set = 16;
            end = 32;
        }
        // Save the threaten piece to a temporary variable
        if(game_prop.w_check || game_prop.b_check){
            temp_T = game_prop.T_piece;
        }

        // Make the chosen piece invisible
        // To determine the line from which the threaten is coming
        temp_bcol = piece[chosen_piece].bcol;
        temp_brow = piece[chosen_piece].brow;
        temp_b1 = board[piece[chosen_piece].brow][piece[chosen_piece].bcol];
        temp_b2 = board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol];
        board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
        board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
        for(int c = set; c < end; c++){
            if (c == 4 || c == 28 || piece[c].l_o_d == 0 || c == temp_T){
                // If it is the king or the piece which checks the king or it's a dead piece, continue
                continue;
            }
            if(check_king_att(c)){
                if(piece[c].type == 'n'){
                    king_knight_att();
                }else{
                    // Get the line on which the king lie and it will be checked there
                    check_line();
                    // Get the line on which the enemy lie and check the king's piece from it
                    check_king_line(ally, enemy);
                }
                // Compare the valid moves and the covered moves
                covered_matching();
                // A flag to determine if there is a check and a piece covers another check
                game_prop.cov_shield = 1;
            }
        }
        // Reset the values of the chosen piece on the board
        board[temp_brow][temp_bcol] = temp_b1;
        board_types[temp_brow][temp_bcol] = temp_b2;
    }
}

//#-------------------------------------------------------------------------#

// Check if there is a piece which masks (shield) the enemy attack
void check_shield(void){
    int d = 0, j, r, p, l, ally, enemy;

    if(chosen_piece == 4 || chosen_piece == 28){
        // If the chosen piece is a king, check its valid moves
        get_valid_moves(chosen_piece);
    }
    // If other piece is chosen get its valid moves to mask the enemy attack
    else{
        // Determine the chosen piece and its enemies and allies
        if(16 <= chosen_piece && chosen_piece < 32){
            ally = 1;
            enemy = -1;
            game_prop.king_check = 28;
        }else if(0 <= chosen_piece && chosen_piece < 16){
            ally = -1;
            enemy = 1;
            game_prop.king_check = 4;
        }
        // Get the valid moves of the chosen piece and the threaten piece
        get_valid_moves(chosen_piece);
        get_valid_moves(game_prop.T_piece);
        if(piece[game_prop.T_piece].type == 'n'){
            king_knight_att();
        }else{
            // Get the line on which the king lie and it will be checked there
            check_line();
            // Get the line on which the enemy lie and check the king's piece from it
            check_king_line(ally, enemy);
        }

        // Get the intersection between the valid moves of the threaten piece and the chosen one
        for(j = 0; j < line_x; j++){
            for(r = 0; r < piece[chosen_piece].cx; r++){
                // If there is an equal moves, save it
                if((piece[chosen_piece].valid_moves_x[r] == line_moves_x[j]) &&
                (piece[chosen_piece].valid_moves_y[r] == line_moves_y[j])){
                    game_prop.cover = 1;
                    valid_line_x[shield_x] = piece[chosen_piece].valid_moves_x[r];
                    valid_line_y[shield_y] = piece[chosen_piece].valid_moves_y[r];
                    shield_x++;
                    shield_y++;
                }
            }
        }
        // When there is equal values, record it to the piece's valid moves
        if(game_prop.cover == 1){
            for(int q = 0; q < shield_x; q++){
                piece[chosen_piece].valid_moves_x[q] = valid_line_x[q];
                piece[chosen_piece].valid_moves_y[q] = valid_line_y[q];
            }
        }
        // Modify array size of valid moves
        piece[chosen_piece].cx = shield_x;
        piece[chosen_piece].cy = shield_y;
    }
}

//#-------------------------------------------------------------------------#

// Check if the king has no moves and there isn't any piece to cover the it and it is checked
// Then confirm the winner of the game
void check_mate(int chosen){
    int d = 0, j, r, p, l, ally, enemy;

    // Determine the chosen piece and its enemies and allies
    if(chosen == 28){
        ally = 1;
        enemy = -1;
        game_prop.king_check = 28;
    }else if(chosen == 4){
        ally = -1;
        enemy = 1;
        game_prop.king_check = 4;
    }
    // Get the valid moves of the chosen piece
    get_valid_moves(game_prop.T_piece);
    if(piece[game_prop.T_piece].type == 'n'){
        king_knight_att();
    }else{
        // Get the line on which the king lie and it will be checked there
        check_line();
        // Get the line on which the enemy lie and check the king's piece from it
        check_king_line(ally, enemy);
    }

    if (chosen == 4){
        // When black king is the checked, check all moves which can be done
        for(int g = 0; g < 16; g++){
            if(g == 4 || piece[g].l_o_d == 0){
                // If the piece is a black king or dead, continue the loop
                continue;
            }
            // Get the valid moves of the piece
            get_valid_moves(g);
            for(j = 0; j < line_x; j++){
                for(r = 0; r < piece[g].cx; r++){
                    // Searching for a move to cover the king's piece
                    if((piece[g].valid_moves_x[r] == line_moves_x[j]) &&
                    (piece[g].valid_moves_y[r] == line_moves_y[j])){
                        game_prop.cover = 1;
                        chosen_piece = g;
                        game_prop.cov_shield = 0;
                        // Check if there is a cover for the king or the king is checked
                        check_cover();
                        if(game_prop.cov_shield == 1){
                            // If a piece covers a check
                            // and there is another check, equal cover flag to zero
                            game_prop.cover = 0;
                        }
                    }
                }
            }
        }
    }else if(chosen == 28){
        // When white king is the checked, check all moves which can be done
        for(int g = 16; g < 32; g++){
            if(g == 28 || piece[g].l_o_d == 0){
                // If the piece is a white king or dead, continue the loop
                continue;
            }
            // Get the valid moves of the piece
            get_valid_moves(g);
            for(j = 0; j < line_x; j++){
                for(r = 0; r < piece[g].cx; r++){
                    // Searching for a move to cover the king's piece
                    if((piece[g].valid_moves_x[r] == line_moves_x[j]) &&
                    (piece[g].valid_moves_y[r] == line_moves_y[j])){
                        game_prop.cover = 1;
                        chosen_piece = g;
                        game_prop.cov_shield = 0;
                        // Check if there is a cover for the king or the king is checked
                        check_cover();
                        if(game_prop.cov_shield == 1){
                            // If a piece covers a check
                            // and there is another check, equal cover flag to zero
                            game_prop.cover = 0;
                        }
                    }
                }
            }
        }
    }
}

//#-------------------------------------------------------------------------#

// Get the valid moves for the chosen piece
void get_valid_moves(int piece_moves){
    int ally, enemy, p, l, i, j;

    // Determine the chosen piece and its enemies and allies
    if(16 <= piece_moves && piece_moves < 32 && piece_moves == chosen_piece){
        ally = 1;
        enemy = -1;
        s = 0;
        e = 16;
    }else if(16 <= piece_moves && piece_moves < 32 && piece_moves != chosen_piece){
        ally = 1;
        enemy = -1;
    }else if(0 <= piece_moves && piece_moves < 16 && piece_moves == chosen_piece){
        ally = -1;
        enemy = 1;
        s = 16;
        e = 32;
    }else if(0 <= piece_moves && piece_moves < 16 && piece_moves != chosen_piece){
        ally = -1;
        enemy = 1;
    }

    // Reset the values of the dead pieces
    if(piece[piece_moves].l_o_d == 0){
        for(j = 0; j < 63; j++){
            piece[piece_moves].valid_moves_x[j] = -10;
            piece[piece_moves].valid_moves_y[j] = -10;
            piece[piece_moves].valid_cover_x[j] = -10;
            piece[piece_moves].valid_cover_y[j] = -10;
        }
        piece[piece_moves].cx = 0;
        piece[piece_moves].cy = 0;
        piece[piece_moves].cover_cx = 0;
        piece[piece_moves].cover_cy = 0;
    }else{
        // Determine the type of chosen piece to call its function
        switch(piece[piece_moves].type){
            case 'r':
                // Clear the counter of the piece
                piece[piece_moves].cover_cx = 0;
                piece[piece_moves].cover_cy = 0;
                piece[piece_moves].cy = 0;
                piece[piece_moves].cx = 0;
                // Calling the function
                rook(piece_moves, ally, enemy, p, l);
                break;
            case 'n':
                // Clear the counter of the piece
                piece[piece_moves].cover_cx = 0;
                piece[piece_moves].cover_cy = 0;
                piece[piece_moves].cy = 0;
                piece[piece_moves].cx = 0;
                // Calling the function
                knight(piece_moves, ally, enemy, p, l);
                break;
            case 'b':
                // Clear the counter of the piece
                piece[piece_moves].cover_cx = 0;
                piece[piece_moves].cover_cy = 0;
                piece[piece_moves].cy = 0;
                piece[piece_moves].cx = 0;
                // Calling the function
                bishop(piece_moves, ally, enemy, p, l);
                break;
            case 'q':
                // Clear the counter of the piece
                piece[piece_moves].cover_cx = 0;
                piece[piece_moves].cover_cy = 0;
                piece[piece_moves].cy = 0;
                piece[piece_moves].cx = 0;
                // Calling the function
                queen(piece_moves, ally, enemy, p, l);
                break;
            case 'k':
                // Clear the counter of the piece
                piece[piece_moves].cover_cx = 0;
                piece[piece_moves].cover_cy = 0;
                piece[piece_moves].cy = 0;
                piece[piece_moves].cx = 0;
                // Calling the function
                king(piece_moves, ally, enemy, p, l);
                break;
            case 'p':
                // Clear the counter of the piece
                piece[piece_moves].cover_cx = 0;
                piece[piece_moves].cover_cy = 0;
                piece[piece_moves].cy = 0;
                piece[piece_moves].cx = 0;
                // White Pawn
                if(16 <= piece_moves && piece_moves < 32){
                    // Calling the function
                    wsol(piece_moves, ally, enemy, p, l);
                }
                // Black Pawn
                else if(0 <= piece_moves && piece_moves < 16){
                    // Calling the function
                    bsol(piece_moves, ally, enemy, p, l);
                }
            break;
        }
    }
}

//#-------------------------------------------------------------------------#

// Check if the king is under attack from the last moved piece or not
int check_king_att(int a){
    // Get the valid moves for the suspected piece
    get_valid_moves(a);
    // Determine the threaten piece and the king which it attacks
    if(a < 32 && a >= 16){
        P_king = 4;
    }else if(a < 16 && a >= 0){
        P_king = 28;
    }
    // Get the intersected moves between the threaten piece and the king which it attack
    // and if there is a intersection the king will be checked
    for(int j = 0; j < piece[a].cx; j++){
        if(piece[a].valid_moves_x[j] / 75 == piece[P_king].bcol &&
        piece[a].valid_moves_y[j] / 75 == piece[P_king].brow){
            if(a == chosen_piece){
                // Determine which king is checked
                if(P_king == 4){
                    game_prop.b_check = 1;
                }else if(P_king == 28){
                    game_prop.w_check = 1;
                }
                game_prop.check = 1;
            }
            game_prop.T_piece = a;
            return (1);
        }
    }
    return (0);
}

//#-------------------------------------------------------------------------#

void dead_pos(void){
    // Determine how many pieces are dead
    for(int h = 0; h < 32; h++){
        // Exclude knight, bishops and kings
        if(piece[h].type == 'n' || piece[h].type == 'b' || piece[h].type == 'k'){
            continue;
        }
        if(piece[h].l_o_d == 0){
            dp = 1;
        }else{
            dp = 0;
            return;
        }
    }
    // The remaining pieces are a black knight and a black king vs a white king
    if(dp == 1 &&
    ((piece[6].l_o_d == 1 && !(piece[1].l_o_d == 1)) || (piece[1].l_o_d == 1 && !(piece[6].l_o_d == 1))) && piece[2].l_o_d == 0 &&
    piece[5].l_o_d == 0 && piece[30].l_o_d == 0 && piece[25].l_o_d == 0 && piece[26].l_o_d == 0 && piece[29].l_o_d == 0){
        game_prop.d_pos = 1;
    }
    // The remaining pieces are a white knight and a white king vs a black king
    else if (dp == 1 &&
    ((piece[25].l_o_d == 1 && !(piece[30].l_o_d == 1)) || (piece[30].l_o_d == 1 && !(piece[25].l_o_d == 1))) && piece[2].l_o_d == 0 &&
    piece[5].l_o_d == 0 && piece[6].l_o_d == 0 && piece[1].l_o_d == 0 && piece[26].l_o_d == 0 && piece[29].l_o_d == 0){
        game_prop.d_pos = 1;
    }
    // The remaining pieces are a black bishop and a black king vs a white king
    else if (dp == 1 &&
    ((piece[2].l_o_d == 1 && !(piece[5].l_o_d == 1)) || (piece[5].l_o_d == 1 && !(piece[2].l_o_d == 1))) && piece[30].l_o_d == 0 &&
    piece[25].l_o_d == 0 && piece[6].l_o_d == 0 && piece[1].l_o_d == 0 && piece[26].l_o_d == 0 && piece[29].l_o_d == 0){
        game_prop.d_pos = 1;
    }
    // The remaining pieces are a white bishop and a white king vs a black king
    else if (dp == 1 &&
    ((piece[26].l_o_d == 1 && !(piece[29].l_o_d == 1)) || (piece[29].l_o_d == 1 && !(piece[26].l_o_d == 1))) && piece[30].l_o_d == 0 &&
    piece[25].l_o_d == 0 && piece[6].l_o_d == 0 && piece[1].l_o_d == 0 && piece[2].l_o_d == 0 && piece[5].l_o_d == 0){
        game_prop.d_pos = 1;
    }
    // The remaining pieces are a black bishop and a black king vs a white bishop and a white king
    // In addition to the bishops are on the same color
    else if (dp == 1 &&
    (piece[2].l_o_d == 1 && piece[29].l_o_d == 1 && piece[30].l_o_d == 0 && piece[25].l_o_d == 0 && piece[6].l_o_d == 0 &&
    piece[1].l_o_d == 0 && piece[26].l_o_d == 0 && piece[5].l_o_d == 0) ||
    (piece[26].l_o_d == 1 && piece[5].l_o_d == 1 && piece[30].l_o_d == 0 && piece[25].l_o_d == 0 && piece[6].l_o_d == 0 &&
    piece[1].l_o_d == 0 && piece[2].l_o_d == 0 && piece[29].l_o_d == 0)){
        game_prop.d_pos = 1;
    }
    // If there is a king vs king with a pawn which will be promoted, consider this case
    else if(dp == 1 && (game_prop.bpb == 1 || game_prop.bpn == 1 || game_prop.wpn == 1 || game_prop.wpn == 1) &&
    piece[4].l_o_d == 1 && piece[28].l_o_d == 1 && piece[30].l_o_d == 0 && piece[25].l_o_d == 0 && piece[6].l_o_d == 0 &&
    piece[1].l_o_d == 0 && piece[26].l_o_d == 0 && piece[5].l_o_d == 0 && piece[2].l_o_d == 0 && piece[29].l_o_d == 0){
        game_prop.d_pos = 1;
    }
    // The remaining pieces are a black king vs a white king
    else if (dp == 1 &&
    piece[4].l_o_d == 1 && piece[28].l_o_d == 1 && piece[30].l_o_d == 0 && piece[25].l_o_d == 0 && piece[6].l_o_d == 0 &&
    piece[1].l_o_d == 0 && piece[26].l_o_d == 0 && piece[5].l_o_d == 0 && piece[2].l_o_d == 0 && piece[29].l_o_d == 0){
        game_prop.d_pos = 1;
    }
}

//#-------------------------------------------------------------------------#

// If the king's piece is not checked but all valid moves for it is attacked
// Game ends in a draw if there is no piece can move and the king isn't checked
void stalemate(){
    int temp_chosen2;
    // save the chosen piece to another variable
    temp_chosen2 = chosen_piece;
    // Determine which player is playing
    if(game_prop.w_o_b == 1){
        for (int g = 0; g < 16; g++){
            if (piece[g].l_o_d == 0){
                // If the piece is dead, continue
                continue;
            }
            chosen_piece = g;
            // Get the valid moves for the chosen piece
            get_valid_moves(chosen_piece);
            // Check if the king is covered from an attack
            check_cover();
            if(piece[g].cx == 0){
                st = 1;
            }else{
                st = 0;
                chosen_piece = temp_chosen2;
                return;
            }
        }
    }else if(game_prop.w_o_b == 0){
        for (int g = 16; g < 32; g++){
            if (piece[g].l_o_d == 0){
                // If the piece is dead, continue
                continue;
            }
            chosen_piece = g;
            // Get the valid moves for the chosen piece
            get_valid_moves(chosen_piece);
            // Check if the king is covered from an attack
            check_cover();
            if(piece[g].cx == 0){
                st = 1;
            }else{
                st = 0;
                chosen_piece = temp_chosen2;
                return;
            }
        }
    }
    // Check if the game ends in a draw by stalemate
    if(st == 1 && game_prop.w_check == 0 && game_prop.b_check == 0){
        game_prop.stale = 1;
    }
    chosen_piece = temp_chosen2;
}

//#-------------------------------------------------------------------------#

// Update the board and save the white move from the player
void white_move(void){
    // Record the move to the board which has white and black pieces
    // Update the (x, y) coordinates for the piece
    // Update the row and column number for the piece on the board
    piece[chosen_piece].brow = piece[chosen_piece].y / 75;
    piece[chosen_piece].bcol = piece[chosen_piece].x / 75;
    board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 1;
    board[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
    if(!(chosen_piece == 28)){
        // Check if this move result in a check
        check_king_att(chosen_piece);
    }
    game_prop.prev++;
    game_prop.do_count = 0;
    if(game_prop.prev > 8){
        // Check if the game end in a draw by stalemate
        stalemate();
    }
    // Record the move to use it for undo and redo features
    if(game_prop.b_check == 1){
        game_prop.comp_mate[game_prop.prev] = 5;
        game_prop.comp_mate[game_prop.prev + 1] = -5;
    }else if(game_prop.stale == 1){
        game_prop.comp_mate[game_prop.prev] = 7;
    }else if(!(game_prop.comp_mate[game_prop.prev] == -4)){
        game_prop.comp_mate[game_prop.prev] = 0;
        if(!(game_prop.comp_mate[game_prop.prev] == 4)){
            game_prop.comp_mate[game_prop.prev + 1] = 0;
        }
    }
    if(piece[chosen_piece].temp_brow_prev == -1){
        game_prop.first_move[game_prop.prev] = -1;
    }else{
        game_prop.first_move[game_prop.prev] = 0;
    }
    game_prop.do_moves_x[game_prop.prev] = piece[chosen_piece].x_prev;
    game_prop.do_moves_y[game_prop.prev] = piece[chosen_piece].y_prev;
    game_prop.prev_bcol[game_prop.prev] = piece[chosen_piece].bcol_prev;
    game_prop.prev_brow[game_prop.prev] = piece[chosen_piece].brow_prev;
    game_prop.chosen_undo[game_prop.prev] = chosen_piece;
    game_prop.redo_moves_x[game_prop.prev] = piece[chosen_piece].x;
    game_prop.redo_moves_y[game_prop.prev] = piece[chosen_piece].y;
    game_prop.next_bcol[game_prop.prev] = piece[chosen_piece].bcol;
    game_prop.next_brow[game_prop.prev] = piece[chosen_piece].brow;
    game_prop.chosen_redo[game_prop.prev] = chosen_piece;
    game_prop.compare[game_prop.prev] = 0;

    // Update the board which has the type of pieces
    switch (piece[chosen_piece].type){
        // A rook is moved
        case 'r':
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = wr;
            board_types[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
            break;
        // A knight is moved
        case 'n':
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = wn;
            board_types[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
            break;
        // A bishop is moved
        case 'b':
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = wb;
            board_types[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
            break;
        // A queen is moved
        case 'q':
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = wq;
            board_types[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
            break;
        // A king is moved
        case 'k':
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = wk;
            board_types[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
            break;
        // A pawn is moved
        case 'p':
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = wp;
            board_types[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
            break;
    }
}
//-----------------------------------------------//
// Update the board and save the white move from the player
void black_move(void){
    // Record the move to the board which has white and black pieces
    // Update the (x, y) coordinates for the piece
    // Update the row and column number for the piece on the board
    piece[chosen_piece].brow = piece[chosen_piece].y / 75;
    piece[chosen_piece].bcol = piece[chosen_piece].x / 75;
    board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = -1;
    board[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
    if(!(chosen_piece == 4)){
        // Check if this move result in a check
        check_king_att(chosen_piece);
    }
    game_prop.prev++;
    game_prop.do_count = 0;
    if(game_prop.prev > 8){
        // Check if the game end in a draw by stalemate
        stalemate();
    }
    // Record the move to use it for undo and redo features
    if(game_prop.w_check == 1){
        game_prop.comp_mate[game_prop.prev] = 4;
        game_prop.comp_mate[game_prop.prev + 1] = -4;
    }else if(game_prop.stale == 1){
        game_prop.comp_mate[game_prop.prev] = 7;
    }else if(!(game_prop.comp_mate[game_prop.prev] == -5)){
        game_prop.comp_mate[game_prop.prev] = 0;
        if(!(game_prop.comp_mate[game_prop.prev] == 5)){
            game_prop.comp_mate[game_prop.prev + 1] = 0;
        }
    }
    if(piece[chosen_piece].temp_brow_prev == -1){
        game_prop.first_move[game_prop.prev] = -1;
    }else{
        game_prop.first_move[game_prop.prev] = 0;
    }
    game_prop.do_moves_x[game_prop.prev] = piece[chosen_piece].x_prev;
    game_prop.do_moves_y[game_prop.prev] = piece[chosen_piece].y_prev;
    game_prop.prev_bcol[game_prop.prev] = piece[chosen_piece].bcol_prev;
    game_prop.prev_brow[game_prop.prev] = piece[chosen_piece].brow_prev;
    game_prop.chosen_undo[game_prop.prev] = chosen_piece;
    game_prop.redo_moves_x[game_prop.prev] = piece[chosen_piece].x;
    game_prop.redo_moves_y[game_prop.prev] = piece[chosen_piece].y;
    game_prop.next_bcol[game_prop.prev] = piece[chosen_piece].bcol;
    game_prop.next_brow[game_prop.prev] = piece[chosen_piece].brow;
    game_prop.chosen_redo[game_prop.prev] = chosen_piece;
    game_prop.compare[game_prop.prev] = 0;

    // Update the board which has the type of pieces
    switch (piece[chosen_piece].type){
        // A rook is moved
        case 'r':
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = br;
            board_types[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
            break;
        // A knight is moved
        case 'n':
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = bn;
            board_types[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
            break;
        // A bishop is moved
        case 'b':
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = bb;
            board_types[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
            break;
        // A queen is moved
        case 'q':
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = bq;
            board_types[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
            break;
        // A king is moved
        case 'k':
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = bk;
            board_types[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
            break;
        case 'p':
            // A pawn is moved
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = bp;
            board_types[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
            break;
    }
}

//#-------------------------------------------------------------------------#
// White castling for the king on the right side
void white_right_castling(void){
    // Update the (x, y) coordinates for the piece
    // Update the row and column number for the piece on the board
    // Record the previous (x, y) and row and column number
    piece[31].x_prev = piece[31].x;
    piece[31].y_prev = piece[31].y;
    piece[31].brow_prev = piece[31].y / 75;
    piece[31].bcol_prev = piece[31].x / 75;
    piece[31].x = (piece[31].bcol - 2) * 75;
    piece[31].y = (piece[31].brow) * 75;
    piece[31].brow = piece[31].y / 75;
    piece[31].bcol = piece[31].x / 75;
    // Record the move to the board which has white and black pieces
    board[piece[31].brow][piece[31].bcol] = 1;
    board[piece[31].brow_prev][piece[31].bcol_prev] = 0;
    // Update the board which has the type of pieces
    board_types[piece[31].brow][piece[31].bcol] = wr;
    board_types[piece[31].brow_prev][piece[31].bcol_prev] = 0;
    game_prop.prev++;
    // Record the move to use it for undo and redo features
    game_prop.do_moves_x[game_prop.prev] = piece[31].x_prev;
    game_prop.do_moves_y[game_prop.prev] = piece[31].y_prev;
    game_prop.prev_bcol[game_prop.prev] = piece[31].bcol_prev;
    game_prop.prev_brow[game_prop.prev] = piece[31].brow_prev;
    game_prop.chosen_undo[game_prop.prev] = 31;
    game_prop.redo_moves_x[game_prop.prev] = piece[31].x;
    game_prop.redo_moves_y[game_prop.prev] = piece[31].y;
    game_prop.next_bcol[game_prop.prev] = piece[31].bcol;
    game_prop.next_brow[game_prop.prev] = piece[31].brow;
    game_prop.compare[game_prop.prev] = 3;
    game_prop.chosen_redo[game_prop.prev] = 31;
    game_prop.wrc_w_k = 0;
}
//-----------------------------------------------//
// White castling for the king on the left side
void white_left_castling(void){
    // Update the (x, y) coordinates for the piece
    // Update the row and column number for the piece on the board
    // Record the previous (x, y) and row and column number
    piece[24].x_prev = piece[24].x;
    piece[24].y_prev = piece[24].y;
    piece[24].brow_prev = piece[24].y / 75;
    piece[24].bcol_prev = piece[24].x / 75;
    piece[24].x = (piece[24].bcol + 3) * 75;
    piece[24].y = (piece[24].brow) * 75;
    piece[24].brow = piece[24].y / 75;
    piece[24].bcol = piece[24].x / 75;
    // Record the move to the board which has white and black pieces
    board[piece[24].brow][piece[24].bcol] = 1;
    board[piece[24].brow_prev][piece[24].bcol_prev] = 0;
    // Update the board which has the type of pieces
    board_types[piece[24].brow][piece[24].bcol] = wr;
    board_types[piece[24].brow_prev][piece[24].bcol_prev] = 0;
    game_prop.prev++;
    // Record the move to use it for undo and redo features
    game_prop.do_moves_x[game_prop.prev] = piece[24].x_prev;
    game_prop.do_moves_y[game_prop.prev] = piece[24].y_prev;
    game_prop.prev_bcol[game_prop.prev] = piece[24].bcol_prev;
    game_prop.prev_brow[game_prop.prev] = piece[24].brow_prev;
    game_prop.chosen_undo[game_prop.prev] = 24;
    game_prop.redo_moves_x[game_prop.prev] = piece[24].x;
    game_prop.redo_moves_y[game_prop.prev] = piece[24].y;
    game_prop.next_bcol[game_prop.prev] = piece[24].bcol;
    game_prop.next_brow[game_prop.prev] = piece[24].brow;
    game_prop.compare[game_prop.prev] = 3;
    game_prop.chosen_redo[game_prop.prev] = 24;
    game_prop.wlc_w_k = 0;
}

//#-------------------------------------------------------------------------#

// Black castling for the king on the right side
void black_right_castling(void){
    // Update the (x, y) coordinates for the piece
    // Update the row and column number for the piece on the board
    // Record the previous (x, y) and row and column number
    piece[7].x_prev = piece[7].x;
    piece[7].y_prev = piece[7].y;
    piece[7].brow_prev = piece[7].y / 75;
    piece[7].bcol_prev = piece[7].x / 75;
    piece[7].x = (piece[7].bcol - 2) * 75;
    piece[7].y = (piece[7].brow) * 75;
    piece[7].brow = piece[7].y / 75;
    piece[7].bcol = piece[7].x / 75;
    // Record the move to the board which has white and black pieces
    board[piece[7].brow][piece[7].bcol] = -1;
    board[piece[7].brow_prev][piece[7].bcol_prev] = 0;
    // Update the board which has the type of pieces
    board_types[piece[7].brow][piece[7].bcol] = br;
    board_types[piece[7].brow_prev][piece[7].bcol_prev] = 0;
    game_prop.prev++;
    // Record the move to use it for undo and redo features
    game_prop.do_moves_x[game_prop.prev] = piece[7].x_prev;
    game_prop.do_moves_y[game_prop.prev] = piece[7].y_prev;
    game_prop.prev_bcol[game_prop.prev] = piece[7].bcol_prev;
    game_prop.prev_brow[game_prop.prev] = piece[7].brow_prev;
    game_prop.chosen_undo[game_prop.prev] = 7;
    game_prop.redo_moves_x[game_prop.prev] = piece[7].x;
    game_prop.redo_moves_y[game_prop.prev] = piece[7].y;
    game_prop.next_bcol[game_prop.prev] = piece[7].bcol;
    game_prop.next_brow[game_prop.prev] = piece[7].brow;
    game_prop.compare[game_prop.prev] = 3;
    game_prop.chosen_redo[game_prop.prev] = 7;
    game_prop.brc_w_k = 0;
}
//-----------------------------------------------//
// Black castling for the king on the left side
void black_left_castling(void){
    // Update the (x, y) coordinates for the piece
    // Update the row and column number for the piece on the board
    // Record the previous (x, y) and row and column number
    piece[0].x_prev = piece[0].x;
    piece[0].y_prev = piece[0].y;
    piece[0].brow_prev = piece[0].y / 75;
    piece[0].bcol_prev = piece[0].x / 75;
    piece[0].x = (piece[0].bcol + 3) * 75;
    piece[0].y = (piece[0].brow) * 75;
    piece[0].brow = piece[0].y / 75;
    piece[0].bcol = piece[0].x / 75;
    // Record the move to the board which has white and black pieces
    board[piece[0].brow][piece[0].bcol] = -1;
    board[piece[0].brow_prev][piece[0].bcol_prev] = 0;
    // Update the board which has the type of pieces
    board_types[piece[0].brow][piece[0].bcol] = br;
    board_types[piece[0].brow_prev][piece[0].bcol_prev] = 0;
    // Record the move to use it for undo and redo features
    game_prop.prev++;
    game_prop.do_moves_x[game_prop.prev] = piece[0].x_prev;
    game_prop.do_moves_y[game_prop.prev] = piece[0].y_prev;
    game_prop.prev_bcol[game_prop.prev] = piece[0].bcol_prev;
    game_prop.prev_brow[game_prop.prev] = piece[0].brow_prev;
    game_prop.chosen_undo[game_prop.prev] = 0;
    game_prop.redo_moves_x[game_prop.prev] = piece[0].x;
    game_prop.redo_moves_y[game_prop.prev] = piece[0].y;
    game_prop.next_bcol[game_prop.prev] = piece[0].bcol;
    game_prop.next_brow[game_prop.prev] = piece[0].brow;
    game_prop.compare[game_prop.prev] = 3;
    game_prop.chosen_redo[game_prop.prev] = 0;
    game_prop.blc_w_k = 0;
}

//#-------------------------------------------------------------------------#

// Determine which piece was eliminated by the white pieces
void white_kill(int i3){
    dead_piece = i3;
    // Update the flag of (dead or live) to a dead piece
    piece[dead_piece].l_o_d = 0;
    if(game_prop.prev > 30){
        // Check if the game will ends in a draw by dead position
        dead_pos();
        if(game_prop.d_pos == 1){
            game_prop.comp_mate[game_prop.prev] = 6;
        }
    }
    // Record the move to use it for undo and redo features
    game_prop.dead_moves_x[game_prop.prev] = piece[dead_piece].x;
    game_prop.dead_moves_y[game_prop.prev] = piece[dead_piece].y;
    game_prop.prev_bcol_dead[game_prop.prev] = piece[dead_piece].bcol;
    game_prop.prev_brow_dead[game_prop.prev] = piece[dead_piece].brow;
    game_prop.dead_undo[game_prop.prev] = dead_piece;
    game_prop.next_bcol_dead[game_prop.prev] = piece[dead_piece].bcol;
    game_prop.next_brow_dead[game_prop.prev] = piece[dead_piece].brow;
    game_prop.dead_redo[game_prop.prev] = dead_piece;
    game_prop.compare[game_prop.prev] = 1;
    // Update (x, y) coordinates to send it to the tomb
    piece[dead_piece].x = 700 + bdx;
    piece[dead_piece].y = 525 - bdy;
    // Record the (x, y) coordinates to send it to the tomb when a redo is used
    game_prop.re_dead_moves_x[game_prop.prev] = piece[dead_piece].x;
    game_prop.re_dead_moves_y[game_prop.prev] = piece[dead_piece].y;
    bdy += 75;
    if(bdy == 75 * 8){
        bdx = 75;
        bdy = 0;
    }
}
//-----------------------------------------------//
// Determine which piece was eliminated by the white pieces by en passant
void white_kill_passant(int i3){
    dead_piece = i3;
    // Update the flag of (dead or live) to a dead piece
    piece[dead_piece].l_o_d = 0;
    // Update the board which has the type of pieces
    board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = wp;
    board_types[piece[dead_piece].brow][piece[dead_piece].bcol] = 0;
    // Record the move to the board which has white and black pieces
    board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 1;
    board[piece[dead_piece].brow][piece[dead_piece].bcol] = 0;
    // Record the move to use it for undo and redo features
    game_prop.dead_moves_x[game_prop.prev] = piece[dead_piece].x;
    game_prop.dead_moves_y[game_prop.prev] = piece[dead_piece].y;
    game_prop.prev_bcol_dead[game_prop.prev] = piece[dead_piece].bcol;
    game_prop.prev_brow_dead[game_prop.prev] = piece[dead_piece].brow;
    game_prop.dead_undo[game_prop.prev] = dead_piece;
    game_prop.compare[game_prop.prev] = 1;
    game_prop.next_bcol_dead[game_prop.prev] = piece[dead_piece].bcol;
    game_prop.next_brow_dead[game_prop.prev] = piece[dead_piece].brow;
    game_prop.dead_redo[game_prop.prev] = dead_piece;
    // Update (x, y) coordinates to send it to the tomb
    piece[dead_piece].x = 700 + bdx;
    piece[dead_piece].y = 525 - bdy;
    // Record the (x, y) coordinates to send it to the tomb when a redo is used
    game_prop.re_dead_moves_x[game_prop.prev] = piece[dead_piece].x;
    game_prop.re_dead_moves_y[game_prop.prev] = piece[dead_piece].y;
    bdy += 75;
    if(bdy == 75 * 8){
        bdx = 75;
        bdy = 0;
    }
}

//#-------------------------------------------------------------------------#

// Determine which piece was eliminated by the black pieces
void black_kill(int i3){
    dead_piece = i3;
    // Update the flag of (dead or live) to a dead piece
    piece[dead_piece].l_o_d = 0;
    if(game_prop.prev > 30){
        // Check if the game will ends in a draw by dead position
        dead_pos();
        if(game_prop.d_pos == 1){
            game_prop.comp_mate[game_prop.prev] = 6;
        }
    }
    // Record the move to use it for undo and redo features
    game_prop.dead_moves_x[game_prop.prev] = piece[dead_piece].x;
    game_prop.dead_moves_y[game_prop.prev] = piece[dead_piece].y;
    game_prop.prev_bcol_dead[game_prop.prev] = piece[dead_piece].bcol;
    game_prop.prev_brow_dead[game_prop.prev] = piece[dead_piece].brow;
    game_prop.dead_undo[game_prop.prev] = dead_piece;
    game_prop.next_bcol_dead[game_prop.prev] = piece[dead_piece].bcol;
    game_prop.next_brow_dead[game_prop.prev] = piece[dead_piece].brow;
    game_prop.dead_redo[game_prop.prev] = dead_piece;
    game_prop.compare[game_prop.prev] = 1;
    // Update (x, y) coordinates to send it to the tomb
    piece[dead_piece].x = 950 + wdx;
    piece[dead_piece].y = 525 - wdy;
    // Record the (x, y) coordinates to send it to the tomb when a redo is used
    game_prop.re_dead_moves_x[game_prop.prev] = piece[dead_piece].x;
    game_prop.re_dead_moves_y[game_prop.prev] = piece[dead_piece].y;
    wdy += 75;
    if(wdy == 75 * 8){
        wdx = 75;
        wdy= 0;
    }
}
//-----------------------------------------------//
// Determine which piece was eliminated by the black pieces by en passant
void black_kill_passant(int i3){
    dead_piece = i3;
    // Update the flag of (dead or live) to a dead piece
    piece[dead_piece].l_o_d = 0;
    // Update the board which has the type of pieces
    board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = bp;
    board_types[piece[dead_piece].brow][piece[dead_piece].bcol] = 0;
    // Record the move to the board which has white and black pieces
    board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = -1;
    board[piece[dead_piece].brow][piece[dead_piece].bcol] = 0;
    // Record the move to use it for undo and redo features
    game_prop.dead_moves_x[game_prop.prev] = piece[dead_piece].x;
    game_prop.dead_moves_y[game_prop.prev] = piece[dead_piece].y;
    game_prop.prev_bcol_dead[game_prop.prev] = piece[dead_piece].bcol;
    game_prop.prev_brow_dead[game_prop.prev] = piece[dead_piece].brow;
    game_prop.dead_undo[game_prop.prev] = dead_piece;
    game_prop.compare[game_prop.prev] = 1;
    game_prop.next_bcol_dead[game_prop.prev] = piece[dead_piece].bcol;
    game_prop.next_brow_dead[game_prop.prev] = piece[dead_piece].brow;
    game_prop.dead_redo[game_prop.prev] = dead_piece;
    // Update (x, y) coordinates to send it to the tomb
    piece[dead_piece].x = 950 + wdx;
    piece[dead_piece].y = 525 - wdy;
    // Record the (x, y) coordinates to send it to the tomb when a redo is used
    game_prop.re_dead_moves_x[game_prop.prev] = piece[dead_piece].x;
    game_prop.re_dead_moves_y[game_prop.prev] = piece[dead_piece].y;
    wdy += 75;
    if(wdy == 75 * 8){
        wdx = 75;
        wdy= 0;
    }
}

//#-------------------------------------------------------------------------#

// Undo function
void undo(void){
    // A counter for undo times
    game_prop.do_count++;
    // Get the last chosen piece to undo its move
    chosen_piece = game_prop.chosen_undo[game_prop.prev];
    // Get the previous coordinates for the piece
    piece[chosen_piece].x = game_prop.do_moves_x[game_prop.prev];
    piece[chosen_piece].y = game_prop.do_moves_y[game_prop.prev];
    game_prop.en_passant = game_prop.passant[game_prop.prev-1];
    // Determine which is playing now
    if(game_prop.w_o_b == 1){
        // Undo the black move
        // Update the previous move to the board which has white and black pieces
        board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
        board[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = -1;
        // Update the board which has the type of pieces
        switch (piece[chosen_piece].type){
            case 'r':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = br;
                break;
            case 'n':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = bn;
                break;
            case 'b':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = bb;
                break;
            case 'q':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = bq;
                break;
            case 'k':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = bk;
                break;
            case 'p':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = bp;
                break;
        }
        // Update the column and row number for the piece
        if(game_prop.first_move[game_prop.prev] == -1){
            piece[chosen_piece].bcol_prev = -1;
            piece[chosen_piece].brow_prev = -1;
        }
        piece[chosen_piece].bcol = game_prop.prev_bcol[game_prop.prev];
        piece[chosen_piece].brow = game_prop.prev_brow[game_prop.prev];
        // If there were castling, undo it
        if (game_prop.compare[game_prop.prev] == 3 && game_prop.w_o_b == 1 && game_prop.prev > 0){
            game_prop.bcas = 1;
            // Update the undo counter
            game_prop.prev--;
            // Get the last chosen piece to undo its move
            chosen_piece = game_prop.chosen_undo[game_prop.prev];
            // Get the previous coordinates for the piece
            piece[chosen_piece].x = game_prop.do_moves_x[game_prop.prev];
            piece[chosen_piece].y = game_prop.do_moves_y[game_prop.prev];
            // Update the previous move to the board which has white and black pieces
            board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
            board[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = -1;
            // Update the board which has the type of pieces
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
            board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = bk;
            // Rest the previous column and row number for the piece (perparing for castling)
            piece[chosen_piece].brow_prev = -1;
            piece[chosen_piece].bcol_prev = -1;
            // Update the column and row number for the piece
            piece[chosen_piece].bcol = game_prop.prev_bcol[game_prop.prev];
            piece[chosen_piece].brow = game_prop.prev_brow[game_prop.prev];
        }if(game_prop.bcas == 1){
            // Get the last chosen piece to undo its move
            chosen_piece = game_prop.chosen_undo[game_prop.prev + 1];
            // Update the column and row number for the piece
            piece[chosen_piece].brow_prev = -1;
            piece[chosen_piece].bcol_prev = -1;
            game_prop.bcas = 0;
        }
    }else if(game_prop.w_o_b == 0){
        // Undo the white move
        // Update the previous move to the board which has white and black pieces
        board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
        board[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = 1;
        // Update the board which has the type of pieces
        switch (piece[chosen_piece].type){
            case 'r':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = wr;
                break;
            case 'n':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = wn;
                break;
            case 'b':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = wb;
                break;
            case 'q':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = wq;
                break;
            case 'k':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = wk;
                break;
            case 'p':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = wp;
                break;
        }
        // Update the column and row number for the piece
        if(game_prop.first_move[game_prop.prev] == -1){
            piece[chosen_piece].bcol_prev = -1;
            piece[chosen_piece].brow_prev = -1;
        }
        piece[chosen_piece].bcol = game_prop.prev_bcol[game_prop.prev];
        piece[chosen_piece].brow = game_prop.prev_brow[game_prop.prev];
        // If there were castling, undo it
        if(game_prop.compare[game_prop.prev] == 3 && game_prop.w_o_b == 0 && game_prop.prev > 0){
            game_prop.wcas = 1;
            // Update the undo counter
            game_prop.prev--;
            // Get the last chosen piece to undo its move
            chosen_piece = game_prop.chosen_undo[game_prop.prev];
            // Get the previous coordinates for the piece
            piece[chosen_piece].x = game_prop.do_moves_x[game_prop.prev];
            piece[chosen_piece].y = game_prop.do_moves_y[game_prop.prev];
            // Update the previous move to the board which has white and black pieces
            board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
            board[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = 1;
            // Update the board which has the type of pieces
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
            board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = wk;
            // Rest the previous column and row number for the piece (preparing for castling)
            piece[chosen_piece].brow_prev = -1;
            piece[chosen_piece].bcol_prev = -1;
            // Update the column and row number for the piece
            piece[chosen_piece].bcol = game_prop.prev_bcol[game_prop.prev];
            piece[chosen_piece].brow = game_prop.prev_brow[game_prop.prev];
        }if(game_prop.wcas == 1){
            // Get the last chosen piece to undo its move
            chosen_piece = game_prop.chosen_undo[game_prop.prev + 1];
            // Update the column and row number for the piece
            piece[chosen_piece].brow_prev = -1;
            piece[chosen_piece].bcol_prev = -1;
            game_prop.wcas = 0;
        }
    }
    // Undo a promotion for a white piece
    if (game_prop.w_o_b == 0 && game_prop.compare[game_prop.prev] == 2 && game_prop.prev > 0){
        // Get the last chosen piece to undo its move
        chosen_piece = game_prop.chosen_undo[game_prop.prev];
        // Get the previous coordinates for the piece
        piece[chosen_piece].x = game_prop.do_moves_x[game_prop.prev];
        piece[chosen_piece].y = game_prop.do_moves_y[game_prop.prev];
        // Update the column and row number for the piece
        piece[chosen_piece].bcol = game_prop.prev_bcol[game_prop.prev];
        piece[chosen_piece].brow = game_prop.prev_brow[game_prop.prev];
        // Reset the promoted piece to its original type (pawn)
        piece[chosen_piece].type = 'p';
        // Update the board which has the type of pieces
        board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = wp;
        // If there was a check, draw by stalemate or dead position, on the last move,
        // undo it (for attacking and promoted piece)
        if(game_prop.comp_mate[game_prop.prev] == 5){
            game_prop.b_check = 0;
            game_prop.d_pos = 0;
            game_prop.stale = 0;
            game_prop.check = 0;
            game_prop.cover = 0;
        }
        // Update the undo counter
        game_prop.prev--;
        game_prop.do_wpromo = 1;
    }
    // Undo a promotion for a black piece
    else if (game_prop.w_o_b == 1 && game_prop.compare[game_prop.prev] == 2 && game_prop.prev > 0){
        // Get the last chosen piece to undo its move
        chosen_piece = game_prop.chosen_undo[game_prop.prev];
        // Get the previous coordinates for the piece
        piece[chosen_piece].x = game_prop.do_moves_x[game_prop.prev];
        piece[chosen_piece].y = game_prop.do_moves_y[game_prop.prev];
        // Update the column and row number for the piece
        piece[chosen_piece].bcol = game_prop.prev_bcol[game_prop.prev];
        piece[chosen_piece].brow = game_prop.prev_brow[game_prop.prev];
        // Reset the promoted piece to its original type (pawn)
        piece[chosen_piece].type = 'p';
        // Update the board which has the type of pieces
        board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = bp;
        // If there was a check, draw by stalemate or dead position, on the last move,
        // undo it (for attacking and promoted piece)
        if(game_prop.comp_mate[game_prop.prev] == 4){
            game_prop.w_check = 0;
            game_prop.d_pos = 0;
            game_prop.stale = 0;
            game_prop.check = 0;
            game_prop.cover = 0;
        }
        // Update the undo counter
        game_prop.prev--;
        game_prop.do_bpromo = 1;
    }
    // Undo the eliminated piece and return it
    if (game_prop.compare[game_prop.prev] == 1 && game_prop.prev > 0){
        // Get the last dead piece to undo its move
        dead_piece = game_prop.dead_undo[game_prop.prev];
        // Get the previous coordinates for the piece
        piece[dead_piece].x = game_prop.dead_moves_x[game_prop.prev];
        piece[dead_piece].y = game_prop.dead_moves_y[game_prop.prev];
        // Set the live or dead flag back to live (one)
        piece[dead_piece].l_o_d = 1;
        // White piece is dead
        if(game_prop.w_o_b == 1){
            // Update the previous kill to the board which has white and black pieces
            board[piece[dead_piece].brow][piece[dead_piece].bcol] = 1;
            // Update the board which has the type of pieces
            switch (piece[dead_piece].type){
                case 'r':
                    board_types[piece[dead_piece].brow][piece[dead_piece].bcol] = wr;
                    break;
                case 'n':
                    board_types[piece[dead_piece].brow][piece[dead_piece].bcol] = wn;
                    break;
                case 'b':
                    board_types[piece[dead_piece].brow][piece[dead_piece].bcol] = wb;
                    break;
                case 'q':
                    board_types[piece[dead_piece].brow][piece[dead_piece].bcol] = wq;
                    break;
                case 'p':
                    board_types[piece[dead_piece].brow][piece[dead_piece].bcol] = wp;
                    break;
            }
        }
        // White piece is dead
        else{
            // Update the previous kill to the board which has white and black pieces
            board[piece[dead_piece].brow][piece[dead_piece].bcol] = -1;
            // Update the board which has the type of pieces
            switch (piece[dead_piece].type){
                case 'r':
                    board_types[piece[dead_piece].brow][piece[dead_piece].bcol] = br;
                    break;
                case 'n':
                    board_types[piece[dead_piece].brow][piece[dead_piece].bcol] = bn;
                    break;
                case 'b':
                    board_types[piece[dead_piece].brow][piece[dead_piece].bcol] = bb;
                    break;
                case 'q':
                    board_types[piece[dead_piece].brow][piece[dead_piece].bcol] = bq;
                    break;
                case 'p':
                    board_types[piece[dead_piece].brow][piece[dead_piece].bcol] = bp;
                    break;
            }
        }
        // Update the column and row number for the piece
        piece[dead_piece].bcol = game_prop.prev_bcol_dead[game_prop.prev];
        piece[dead_piece].brow = game_prop.prev_brow_dead[game_prop.prev];
    }

    // If there was a check, draw by stalemate or dead position, on the last move,
    // undo it (for attacking piece)
    if(game_prop.comp_mate[game_prop.prev] == 4 || game_prop.comp_mate[game_prop.prev] == 5 || game_prop.comp_mate[game_prop.prev] == 6 ||
        game_prop.comp_mate[game_prop.prev] == 7 || game_prop.comp_mate[game_prop.prev] == 0){
        game_prop.w_check = 0;
        game_prop.b_check = 0;
        game_prop.d_pos = 0;
        game_prop.stale = 0;
        game_prop.check = 0;
        game_prop.cover = 0;
    }
    // If there was a check on the last move, undo it (for covering piece)
    if(game_prop.comp_mate[game_prop.prev] == -5){
        game_prop.b_check = 1;
        game_prop.check = 1;
        game_prop.cover = 1;
    }
    // If there was a check on the last move, undo it (for covering piece)
    if(game_prop.comp_mate[game_prop.prev] == -4){
        game_prop.w_check = 1;
        game_prop.check = 1;
        game_prop.cover = 1;
    }

    // Toggle the player's turn
    if (game_prop.w_o_b == 1){
        game_prop.w_o_b = 0;
    }else{
        game_prop.w_o_b = 1;
    }
    flag = 0;
    // Update the undo counter
    game_prop.prev--;
}
//-----------------------------------------------//
void redo(void){
    // Update the undo counter
    game_prop.prev++;
    // A counter for redo times
    game_prop.do_count--;
    // Get the next chosen piece to redo its move
    chosen_piece = game_prop.chosen_redo[game_prop.prev];
    // Get the next coordinates for the piece
    piece[chosen_piece].x = game_prop.redo_moves_x[game_prop.prev];
    piece[chosen_piece].y = game_prop.redo_moves_y[game_prop.prev];
    game_prop.en_passant = game_prop.passant[game_prop.prev];
    // Determine which is playing now
    if(game_prop.w_o_b == 1){
        // Redo the white move
        // Update the next move to the board which has white and black pieces
        board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
        board[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = 1;
        // Update the board which has the type of pieces
        switch (piece[chosen_piece].type){
            case 'r':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = wr;
                break;
            case 'n':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = wn;
                break;
            case 'b':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = wb;
                break;
            case 'q':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = wq;
                break;
            case 'k':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = wk;
                break;
            case 'p':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = wp;
                break;
        }
        // Update the column and row number for the piece
        piece[chosen_piece].bcol = game_prop.next_bcol[game_prop.prev];
        piece[chosen_piece].brow = game_prop.next_brow[game_prop.prev];

        // If there were castling for white, redo it
        if (game_prop.compare[game_prop.prev + 1] == 3 && game_prop.w_o_b == 1 && game_prop.prev > 0){
            game_prop.wcas = 1;
            // Update the undo counter
            game_prop.prev++;
            // Get the next chosen piece to redo its move
            chosen_piece = game_prop.chosen_redo[game_prop.prev];
            // Get the next coordinates for the piece
            piece[chosen_piece].x = game_prop.redo_moves_x[game_prop.prev];
            piece[chosen_piece].y = game_prop.redo_moves_y[game_prop.prev];
            // Update the next move to the board which has white and black pieces
            board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
            board[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = 1;
            // Update the board which has the type of pieces
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
            board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = wr;
            // Rest the next column and row number for the piece
            piece[chosen_piece].brow_prev = -1;
            piece[chosen_piece].bcol_prev = -1;
            // Update the column and row number for the piece
            piece[chosen_piece].bcol = game_prop.next_bcol[game_prop.prev];
            piece[chosen_piece].brow = game_prop.next_brow[game_prop.prev];
        }if(game_prop.wcas == 1){
            // Get the next chosen piece to redo its move
            chosen_piece = game_prop.chosen_undo[game_prop.prev - 1];
            // Update the column and row number for the piece
            piece[chosen_piece].brow_prev = -1;
            piece[chosen_piece].bcol_prev = -1;
            game_prop.wcas = 0;
        }
    }
    // Determine which is playing now
    else if(game_prop.w_o_b == 0){
        // Redo the black move
        // Update the next move to the board which has white and black pieces
        board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
        board[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = -1;
        // Update the board which has the type of pieces
        switch (piece[chosen_piece].type){
            case 'r':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = br;
                break;
            case 'n':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = bn;
                break;
            case 'b':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = bb;
                break;
            case 'q':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = bq;
                break;
            case 'k':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = bk;
                break;
            case 'p':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = bp;
                break;
        }
        // Update the column and row number for the piece
        piece[chosen_piece].bcol = game_prop.next_bcol[game_prop.prev];
        piece[chosen_piece].brow = game_prop.next_brow[game_prop.prev];

        // If there were castling for black, redo it
        if (game_prop.compare[game_prop.prev + 1] == 3 && game_prop.w_o_b == 0 && game_prop.prev > 0){
            game_prop.bcas = 1;
            // Update the undo counter
            game_prop.prev++;
            // Get the next chosen piece to redo its move
            chosen_piece = game_prop.chosen_redo[game_prop.prev];
            // Get the next coordinates for the piece
            piece[chosen_piece].x = game_prop.redo_moves_x[game_prop.prev];
            piece[chosen_piece].y = game_prop.redo_moves_y[game_prop.prev];
            // Update the next move to the board which has white and black pieces
            board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
            board[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = -1;
            // Update the board which has the type of pieces
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
            board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = br;
            // Rest the next column and row number for the piece
            piece[chosen_piece].brow_prev = -1;
            piece[chosen_piece].bcol_prev = -1;
            // Update the column and row number for the piece
            piece[chosen_piece].bcol = game_prop.next_bcol[game_prop.prev];
            piece[chosen_piece].brow = game_prop.next_brow[game_prop.prev];
        }if(game_prop.bcas == 1){
            // Get the next chosen piece to redo its move
            chosen_piece = game_prop.chosen_undo[game_prop.prev - 1];
            // Update the column and row number for the piece
            piece[chosen_piece].brow_prev = -1;
            piece[chosen_piece].bcol_prev = -1;
            game_prop.bcas = 0;
        }
    }
    // If there was white king check, redo it (for attacking piece)
    if(game_prop.comp_mate[game_prop.prev] == 4){
        game_prop.w_check = 1;
        game_prop.check = 1;
    }
    // If there was black king check, redo it (for attacking piece)
    if(game_prop.comp_mate[game_prop.prev] == 5){
        game_prop.b_check = 1;
        game_prop.check = 1;
    }
    // If there was white king check, redo it (for covering piece)
    if(game_prop.comp_mate[game_prop.prev] == -4){
        game_prop.w_check = 0;
        game_prop.check = 0;
        game_prop.cover = 0;
    }
    // If there was black king check, redo it (for covering piece)
    if(game_prop.comp_mate[game_prop.prev] == -5){
        game_prop.b_check = 0;
        game_prop.check = 0;
        game_prop.cover = 0;
    }
    // If there was a draw by dead position, redo it
    if(game_prop.comp_mate[game_prop.prev] == 6){
        game_prop.d_pos = 1;
    }
    // If there was a draw by stalemate, redo it
    if(game_prop.comp_mate[game_prop.prev] == 7){
        game_prop.stale = 1;
    }
    // If there was nothing, reset flags value
    if(game_prop.comp_mate[game_prop.prev] == 0){
        game_prop.b_check = 0;
        game_prop.w_check = 0;
        game_prop.d_pos = 0;
        game_prop.stale = 0;
        game_prop.check = 0;
    }
    // Redo the kill move and send the dead piece to the tomb
    if (game_prop.compare[game_prop.prev] == 1 && game_prop.prev > 0){
        // Get the next dead piece to redo its move
        dead_piece = game_prop.dead_redo[game_prop.prev];
        // Get the next coordinates for the piece
        piece[dead_piece].x = game_prop.re_dead_moves_x[game_prop.prev];
        piece[dead_piece].y = game_prop.re_dead_moves_y[game_prop.prev];
        // Update the column and row number for the piece
        piece[dead_piece].bcol = game_prop.next_bcol_dead[game_prop.prev];
        piece[dead_piece].brow = game_prop.next_brow_dead[game_prop.prev];
        // Set the live or dead flag back to dead (zero)
        piece[dead_piece].l_o_d = 0;
    }
    // Redo a promotion for a white piece
    if (game_prop.w_o_b == 1 && game_prop.compare[game_prop.prev + 1] == 2 && game_prop.prev > 0){
        // Get the next chosen piece to redo its move
        chosen_piece = game_prop.chosen_redo[game_prop.prev];
        // Get the next coordinates for the piece
        piece[chosen_piece].x = game_prop.redo_moves_x[game_prop.prev];
        piece[chosen_piece].y = game_prop.redo_moves_y[game_prop.prev];
        // Update the column and row number for the piece
        piece[chosen_piece].bcol = game_prop.next_bcol[game_prop.prev];
        piece[chosen_piece].brow = game_prop.next_brow[game_prop.prev];
        // Update the piece to the promoted piece
        piece[chosen_piece].type = game_prop.redo_promo_type[game_prop.prev];
        switch (piece[chosen_piece].type){
                case 'r':
                    board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = wr;
                    game_prop.wpr = 1;
                    break;
                case 'n':
                    board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = wn;
                    game_prop.wpn = 1;
                    break;
                case 'b':
                    board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = wb;
                    game_prop.wpb = 1;
                    break;
                case 'q':
                    board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = wq;
                    game_prop.wpq = 1;
                    break;
            }
        // Update the undo counter
        game_prop.prev++;
        // If there was a check, draw by stalemate or dead position, on the next move,
        // redo it (for attacking and promoted piece)
        if(game_prop.comp_mate[game_prop.prev] == 5){
            game_prop.b_check = 1;
            game_prop.check = 1;
        }
    }
    // Redo a promotion for a black piece
    else if (game_prop.w_o_b == 0 && game_prop.compare[game_prop.prev + 1] == 2 && game_prop.prev > 0){
        // Get the next chosen piece to redo its move
        chosen_piece = game_prop.chosen_redo[game_prop.prev];
        // Get the next coordinates for the piece
        piece[chosen_piece].x = game_prop.redo_moves_x[game_prop.prev];
        piece[chosen_piece].y = game_prop.redo_moves_y[game_prop.prev];
        // Update the column and row number for the piece
        piece[chosen_piece].bcol = game_prop.next_bcol[game_prop.prev];
        piece[chosen_piece].brow = game_prop.next_brow[game_prop.prev];
        // Update the piece to the promoted piece
        piece[chosen_piece].type = game_prop.redo_promo_type[game_prop.prev];
        switch (piece[chosen_piece].type){
                case 'r':
                    board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = br;
                    game_prop.bpr = 1;
                    break;
                case 'n':
                    board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = bn;
                    game_prop.bpn = 1;
                    break;
                case 'b':
                    board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = bb;
                    game_prop.bpb = 1;
                    break;
                case 'q':
                    board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = bq;
                    game_prop.bpq = 1;
                    break;
            }
        // Update the undo counter
        game_prop.prev++;
        // If there was a check, draw by stalemate or dead position, on the next move,
        // redo it (for attacking and promoted piece)
        if(game_prop.comp_mate[game_prop.prev] == 4){
            game_prop. w_check = 1;
            game_prop.check = 1;
        }
    }
    // Toggle the player's turn
    if (game_prop.w_o_b == 1){
        game_prop.w_o_b = 0;
    }else{
        game_prop.w_o_b = 1;
    }
    flag = 0;
}

//#-------------------------------------------------------------------------#

// A function to save the game
void save(void){
    int y, f, l;
    // Open file to Save information in it
    last_scene = fopen("save/last scene.bin", "wb");
    for(y = 0; y < 32; y++){
        // Save the array of pieces
        fwrite(&piece[y], sizeof(piece[y]), 1, last_scene);
    }
    for(f = 0; f < 8; f++){
        for(l = 0; l < 8; l++){
            // Save the boards
            fwrite(&board[f][l], sizeof(board[f][l]), 1, last_scene);
            fwrite(&board_types[f][l], sizeof(board_types[f][l]), 1, last_scene);
        }
    }
    // Save game properties structure
    fwrite(&game_prop, sizeof(game_prop), 1, last_scene);
    // Close the file
    fclose(last_scene);
}
//-----------------------------------------------//
// A function to load the game
void load(void){
    int y, f, l;
    // Load the saved file
    last_scene = fopen("save/last scene.bin", "rb");
    for(y = 0; y < 32; y++){
        // Load the array of pieces
        fread(&piece[y], sizeof(piece[y]), 1, last_scene);
    }
    for( f = 0; f < 8; f++){
        for(l = 0; l < 8; l++){
            // Load the boards
            fread(&board[f][l], sizeof(board[f][l]), 1, last_scene);
            fread(&board_types[f][l], sizeof(board_types[f][l]), 1, last_scene);
        }
    }
    // Load game properties structure
    fread(&game_prop, sizeof(game_prop), 1, last_scene);
    // Close the file
    fclose(last_scene);
    // A loop to determine which piece was promoted
    for(y = 0; y < 16 ; y++){
        switch(piece[y].type){
            case 'r':
                texture[(y + 1)] = texture[1];
                break;
            case 'q':
                texture[(y + 1)] = texture[4];
                break;
            case 'n':
                texture[(y + 1)] = texture[2];
                break;
            case 'b':
                texture[(y + 1)] = texture[3];
                break;
            case 'p':
                texture[(y + 1)] = texture[9];
                break;
        }
    }
    for(y = 16; y < 32 ; y++){
        switch(piece[y].type){
            case 'r':
                texture[(y + 1)] = texture[32];
                break;
            case 'q':
                texture[(y + 1)] = texture[28];
                break;
            case 'n':
                texture[(y + 1)] = texture[31];
                break;
            case 'b':
                texture[(y + 1)] = texture[30];
                break;
            case 'p':
                texture[(y + 1)] = texture[17];
                break;
        }
    }
}

//#-------------------------------------------------------------------------#
// Take user inputs and change the coordinates:

void process_input(void){
    int i3 = 0, i = 0;
    // The event will be called
    SDL_Event event;
    SDL_WaitEvent(&event);
    // Determine which event was called
    switch(event.type){
        // When pressing the close button on the window, close the game
        case SDL_QUIT:
            Ready_To_start = 0;
            break;
//-----------------------------------------------//
        // When pressing the escape key, close the game
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE){
                Ready_To_start = 0;
            }
            break;
//-----------------------------------------------//
        // When pressing the mouse button
        case SDL_MOUSEBUTTONDOWN:
            // On the save game icon, save the game
            if(event.button.button == SDL_BUTTON_LEFT && event.button.x > 600 && event.button.x < 700 &&
                event.button.y > 500 && event.button.y < 600){
                save();
                f_save = 1;
            }
//-----------------------------------------------//
            // On the load game icon, load the game
            else if(event.button.button == SDL_BUTTON_LEFT && event.button.x > 600 && event.button.x < 700 &&
                event.button.y > 0 && event.button.y < 100){
                load();
                f_load = 1;
            }
//-----------------------------------------------//
            // On the undo icon, undo the last move
            else if(event.button.button == SDL_BUTTON_LEFT && event.button.x > 850 && event.button.x < 950 &&
                event.button.y > 400 && event.button.y < 500 && game_prop.prev >= 0 && game_prop.wpromo == 0 && game_prop.bpromo == 0){
                undo();
                f_undo = 1;
            }
//-----------------------------------------------//
            // On the redo icon, redo the next move
            else if(event.button.button == SDL_BUTTON_LEFT && event.button.x > 850 && event.button.x < 950 &&
                    event.button.y > 500 && event.button.y < 600 && game_prop.do_count > 0 && game_prop.wpromo == 0 && game_prop.bpromo == 0){
                redo();
                f_redo = 1;
            }
//-----------------------------------------------//
            else{
                // Black player turn
                if(game_prop.w_o_b == 0){
                    // When a black pawn is promoted
                    // Decide which piece will promote to
                    if(game_prop.bpromo){
                        chosen_piece = temp_chosen;
                        // If a rook was chosen
                        if(event.button.x > 850 && event.button.x < 950 && event.button.y > 0
                            && event.button.y < 75 && event.button.button == SDL_BUTTON_LEFT){
                            piece[chosen_piece].type = 'r';
                            game_prop.redo_promo_type[game_prop.prev] = 'r';
                            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = br;
                            game_prop.bpr = 1;
                        }
                        // If a bishop was chosen
                        else if(event.button.x > 850 && event.button.x < 950 && event.button.y > 75
                                && event.button.y < 150 && event.button.button == SDL_BUTTON_LEFT){
                            piece[chosen_piece].type = 'b';
                            game_prop.redo_promo_type[game_prop.prev] = 'b';
                            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = bb;
                            game_prop.bpb = 1;
                            // Check if there is a move to be done for the enemy and the king isn't checked
                            stalemate();
                            dead_pos();
                        }
                        // If a knight was chosen
                        else if(event.button.x > 850 && event.button.x < 950 && event.button.y > 150
                                && event.button.y < 225 && event.button.button == SDL_BUTTON_LEFT){
                            piece[chosen_piece].type = 'n';
                            game_prop.redo_promo_type[game_prop.prev] = 'n';
                            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = bn;
                            game_prop.bpn = 1;
                            // Check if there is a move to be done for the enemy and the king isn't checked
                            stalemate();
                            dead_pos();
                        }
                        // If a queen was chosen
                        else if(event.button.x > 850 && event.button.x < 950 && event.button.y > 225
                                && event.button.y < 300 && event.button.button == SDL_BUTTON_LEFT){
                            piece[chosen_piece].type = 'q';
                            game_prop.redo_promo_type[game_prop.prev] = 'q';
                            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = bq;
                            game_prop.bpq = 1;
                        }
                        if(game_prop.bpr == 1 || game_prop.bpb == 1 || game_prop.bpn == 1 || game_prop.bpq == 1){
                            // Record the move to use it for undo and redo features
                            game_prop.prev++;
                            game_prop.do_moves_x[game_prop.prev] = piece[chosen_piece].x_prev;
                            game_prop.do_moves_y[game_prop.prev] = piece[chosen_piece].y_prev;
                            game_prop.prev_bcol[game_prop.prev] = piece[chosen_piece].bcol_prev;
                            game_prop.prev_brow[game_prop.prev] = piece[chosen_piece].brow_prev;
                            game_prop.chosen_undo[game_prop.prev] = chosen_piece;
                            game_prop.redo_moves_x[game_prop.prev] = piece[chosen_piece].x;
                            game_prop.redo_moves_y[game_prop.prev] = piece[chosen_piece].y;
                            game_prop.next_bcol[game_prop.prev] = piece[chosen_piece].bcol;
                            game_prop.next_brow[game_prop.prev] = piece[chosen_piece].brow;
                            game_prop.chosen_redo[game_prop.prev] = chosen_piece;
                            game_prop.compare[game_prop.prev] = 2;
                            game_prop.bpromo = 0;
                            if(!(chosen_piece == 4)){
                                // Check if the promoted piece is attacking the king or not
                                check_king_att(chosen_piece);
                            }
                            // If the king was checked,
                            if(game_prop.w_check == 1){
                                // record this move for undo and redo features
                                game_prop.comp_mate[game_prop.prev] = 4;
                                game_prop.comp_mate[game_prop.prev + 1] = -4;
                            }else if(!(game_prop.comp_mate[game_prop.prev] == -5)){
                                // record this move for undo and redo features
                                game_prop.comp_mate[game_prop.prev] = 0;
                            }
                            game_prop.w_o_b = 1;
                        }
                    }else{
                        // If it is a normal move
                        for(i = 0; i < 16; i++){
                            // Find which piece was chosen to move
                            if(event.button.x > piece[i].x && event.button.x < piece[i].x + 75 && event.button.y > piece[i].y
                                && event.button.y < piece[i].y + 75 && event.button.button == SDL_BUTTON_LEFT && event.button.x <= 600){
                                // A flag to determine if the button was down or not
                                if (flag == 1){
                                    flag = 0;
                                }else{
                                    flag = 1;
                                    chosen_piece = i;
                                    // If the king was checked
                                    if(game_prop.b_check){
                                        game_prop.cov_shield = 0;
                                        // Check if there is a cover for the king or the king is checked
                                        check_cover();
                                        if(game_prop.cov_shield == 0){
                                            // Check if there is a piece which masks (shield) the enemy attack
                                            check_shield();
                                        }
                                    }else{
                                        // Get the valid moves of the piece
                                        get_valid_moves(chosen_piece);
                                        // Check if there is a cover for the king or the king is checked
                                        check_cover();
                                    }
                                    // Update the (x, y) coordinates for the piece
                                    piece[chosen_piece].x_prev = piece[chosen_piece].x;
                                    piece[chosen_piece].y_prev = piece[chosen_piece].y;
                                    // Update the previous row and column number for the piece on the board
                                    piece[chosen_piece].temp_brow_prev = piece[chosen_piece].brow_prev;
                                    piece[chosen_piece].brow_prev = piece[chosen_piece].y / 75;
                                    piece[chosen_piece].bcol_prev = piece[chosen_piece].x / 75;
                                    guide = 1;
                                    render();
                                }
                            }
                        }
                    }
                }
                // White player turn
                else if(game_prop.w_o_b == 1){
                    // When a black pawn is promoted
                    // Decide which piece will promote to
                    if(game_prop.wpromo){
                        chosen_piece = temp_chosen;
                        // If a rook was chosen
                        if(event.button.x > 850 && event.button.x < 950 && event.button.y > 0
                            && event.button.y < 75 && event.button.button == SDL_BUTTON_LEFT){
                            piece[chosen_piece].type = 'r';
                            game_prop.redo_promo_type[game_prop.prev] = 'r';
                            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = wr;
                            game_prop.wpr = 1;
                        }
                        // If a bishop was chosen
                        else if(event.button.x > 850 && event.button.x < 950 && event.button.y > 75
                                && event.button.y < 150 && event.button.button == SDL_BUTTON_LEFT){
                            piece[chosen_piece].type = 'b';
                            game_prop.redo_promo_type[game_prop.prev] = 'b';
                            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = wb;
                            game_prop.wpb = 1;
                            // Check if there is a move to be done for the enemy and the king isn't checked
                            stalemate();
                            dead_pos();
                        }
                        // If a knight was chosen
                        else if(event.button.x > 850 && event.button.x < 950 && event.button.y > 150
                                && event.button.y < 225 && event.button.button == SDL_BUTTON_LEFT){
                            piece[chosen_piece].type = 'n';
                            game_prop.redo_promo_type[game_prop.prev] = 'n';
                            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = wn;
                            game_prop.wpn = 1;
                            // Check if there is a move to be done for the enemy and the king isn't checked
                            stalemate();
                            dead_pos();
                        }
                        // If a queen was chosen
                        else if(event.button.x > 850 && event.button.x < 950 && event.button.y > 225
                                && event.button.y < 300 && event.button.button == SDL_BUTTON_LEFT){
                            piece[chosen_piece].type = 'q';
                            game_prop.redo_promo_type[game_prop.prev] = 'q';
                            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = wq;
                            game_prop.wpq = 1;
                        }
                        if(game_prop.wpr == 1 || game_prop.wpb == 1 || game_prop.wpn == 1 || game_prop.wpq == 1){
                            // Record the move to use it for undo and redo features
                            game_prop.prev++;
                            game_prop.do_moves_x[game_prop.prev] = piece[chosen_piece].x_prev;
                            game_prop.do_moves_y[game_prop.prev] = piece[chosen_piece].y_prev;
                            game_prop.prev_bcol[game_prop.prev] = piece[chosen_piece].bcol_prev;
                            game_prop.prev_brow[game_prop.prev] = piece[chosen_piece].brow_prev;
                            game_prop.chosen_undo[game_prop.prev] = chosen_piece;
                            game_prop.redo_moves_x[game_prop.prev] = piece[chosen_piece].x;
                            game_prop.redo_moves_y[game_prop.prev] = piece[chosen_piece].y;
                            game_prop.next_bcol[game_prop.prev] = piece[chosen_piece].bcol;
                            game_prop.next_brow[game_prop.prev] = piece[chosen_piece].brow;
                            game_prop.chosen_redo[game_prop.prev] = chosen_piece;
                            game_prop.compare[game_prop.prev] = 2;
                            game_prop.wpromo = 0;
                            if(!(chosen_piece == 28)){
                                // Check if the promoted piece is attacking the king or not
                                check_king_att(chosen_piece);
                            }
                            // If the king was checked,
                            if(game_prop.b_check == 1){
                                // record this move for undo and redo features
                                game_prop.comp_mate[game_prop.prev] = 5;
                                game_prop.comp_mate[game_prop.prev + 1] = -5;
                            }else if(!(game_prop.comp_mate[game_prop.prev] == -4)){
                                // record this move for undo and redo features
                                game_prop.comp_mate[game_prop.prev] = 0;
                            }
                            game_prop.w_o_b = 0;
                        }
                    }else{
                        // If it is a normal move
                        for(i = 16; i < 32; i++){
                            // Find which piece was chosen to move
                            if(event.button.x > piece[i].x && event.button.x < piece[i].x + 75 && event.button.y > piece[i].y
                                && event.button.y < piece[i].y + 75 && event.button.button == SDL_BUTTON_LEFT && event.button.x<=600){
                                // A flag to determine if the button was down or not
                                if (flag == 1){
                                    flag = 0;
                                } else {
                                    flag = 1;
                                    chosen_piece = i;
                                    // If the king was checked
                                    if(game_prop.w_check){
                                        game_prop.cov_shield = 0;
                                        // Check if there is a cover for the king or the king is checked
                                        check_cover();
                                        if(game_prop.cov_shield == 0){
                                            // Check if there is a piece which masks (shield) the enemy attack
                                            check_shield();
                                        }
                                    }else{
                                        // Get the valid moves of the piece
                                        get_valid_moves(chosen_piece);
                                        // Check if there is a cover for the king or the king is checked
                                        check_cover();
                                    }
                                    // Update the (x, y) coordinates for the piece
                                    piece[chosen_piece].x_prev = piece[chosen_piece].x;
                                    piece[chosen_piece].y_prev = piece[chosen_piece].y;
                                    // Update the previous row and column number for the piece on the board
                                    piece[chosen_piece].temp_brow_prev = piece[chosen_piece].brow_prev;
                                    piece[chosen_piece].brow_prev = piece[chosen_piece].y / 75;
                                    piece[chosen_piece].bcol_prev = piece[chosen_piece].x / 75;
                                    guide = 1;
                                    render();
                                }
                            }
                        }
                    }
                }
            }
            break;
//#-------------------------------------------------------------------------#
        // Handle the mouse motion move to determine the new position for the piece
        case SDL_MOUSEMOTION:
            if(flag == 1 && (event.button.y <= 600 && event.button.x <= 600)){
                piece[chosen_piece].x = (event.button.x - 75 / 2);
                piece[chosen_piece].y = (event.button.y - 75 / 2);
            }
            break;

//#-------------------------------------------------------------------------#

        // When the mouse button is released
        case(SDL_MOUSEBUTTONUP):
            guide = 0;
            // If in the same position of the piece, Update it to the same place coordinates
            if (flag == 1 && ((event.button.x) / 75) * 75 == piece[chosen_piece].x_prev && event.button.x <= 600 &&
                ((event.button.y ) / 75) * 75 == piece[chosen_piece].y_prev && event.button.button == SDL_BUTTON_LEFT){
                flag = 0;
                // Update the (x, y) coordinates for the piece
                piece[chosen_piece].x = ((event.button.x) / 75) * 75;
                piece[chosen_piece].y = ((event.button.y) / 75) * 75;
                // Update the row and column number for the piece on the board
                piece[chosen_piece].brow = piece[chosen_piece].y / 75;
                piece[chosen_piece].bcol = piece[chosen_piece].x / 75;
                // A Flag to determine if it was the first move or not
                if(piece[chosen_piece].temp_brow_prev == -1){
                    piece[chosen_piece].brow_prev = -1;
                    piece[chosen_piece].bcol_prev = -1;
                }
                // Preparing for en passant
                if(piece[chosen_piece].type == 'p'){
                    piece[chosen_piece].brow_prev = piece[chosen_piece].temp_brow_prev;
                    piece[chosen_piece].bcol_prev = piece[chosen_piece].temp_brow_prev;
                }
            }

//#-------------------------------------------------------------------------#

            // White Turn
            else if (flag == 1 && game_prop.w_o_b == 1 && event.button.button == SDL_BUTTON_LEFT && event.button.x<=600){
                flag = 0;
                // Save the last move to a temporary variable to check if it is valid or not
                piece[chosen_piece].xtemp = ((event.button.x) / 75) * 75;
                piece[chosen_piece].ytemp = ((event.button.y) / 75) * 75;
                // Check if this move is valid or not
                if (check_valid()){
                    // If the king was checked, clear the check
                    if(game_prop.w_check == 1){
                        game_prop.w_check = 0;
                        game_prop.cover = 0;
                    }
                    // Update the (x, y) coordinates for the piece
                    piece[chosen_piece].x = ((event.button.x) / 75) * 75;
                    piece[chosen_piece].y = ((event.button.y) / 75) * 75;
                    // Record the white movement and update the boards
                    white_move();
                    // Preparing for en passant
                    if(game_prop.en_passant >= 0 && piece[chosen_piece].type == 'p' &&
                    piece[chosen_piece].brow_prev == 6 && piece[chosen_piece].brow == 4){
                        game_prop.en_passant = 2;
                    }
                    // Check if there is a promotion or not
                    if(piece[chosen_piece].type == 'p' && piece[chosen_piece].brow == 0){
                        game_prop.wpromo = 1;
                        render();
                    }else{
                        game_prop.wpromo = 0;
                    }
                    // Check if there is castling or not
                    if(game_prop.wrc_w_k && (event.button.x / 75 == 6) && piece[28].bcol == 6 && piece[28].brow == 7){
                        white_right_castling();
                    }
                    else if(game_prop.wlc_w_k && (event.button.x / 75 == 2) && piece[28].bcol == 2 && piece[28].brow == 7){
                        white_left_castling();
                    }
                    // Check if the move result in a kill or not
                    if(check_die()){
                        for(i3 = 0; i3 < 16; i3++){
                            // If there is a normal kill
                            if(event.button.x > piece[i3].x && event.button.x < piece[i3].x + 75 && event.button.y > piece[i3].y
                                && event.button.y < piece[i3].y + 75 && event.button.button == SDL_BUTTON_LEFT && i3 != chosen_piece){
                                white_kill(i3);
                                break;
                            }
                            // For en passant kill
                            else if (board_types[piece[chosen_piece].brow + 1][piece[chosen_piece].bcol] == bp
                                && board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] == wp &&
                                piece[chosen_piece].brow == 2 &&
                                event.button.x > piece[i3].x && event.button.x < piece[i3].x + 75 && (event.button.y) + 75 > piece[i3].y
                                && event.button.y + 75 < piece[i3].y + 75 && event.button.button == SDL_BUTTON_LEFT && i3 != chosen_piece){
                                white_kill_passant(i3);
                                break;
                            }
                        }
                    }
                    // If there is a promotion, toggle the turn
                    if(game_prop.wpromo){
                        game_prop.w_o_b = 1;
                    }else{
                        game_prop.w_o_b = 0;
                        // Preparing for en passant
                        if(game_prop.en_passant > 0){
                            game_prop.en_passant--;
                            game_prop.passant[game_prop.prev] = game_prop.en_passant;
                        }
                    }
                }else{
                    // If the move was not valid, keep everything in its place
                    piece[chosen_piece].x = piece[chosen_piece].x_prev;
                    piece[chosen_piece].y = piece[chosen_piece].y_prev;
                    wrong = 1;
                    render();
                    SDL_Delay(1000);
                    wrong = 0;
                    game_prop.w_o_b = 1;
                }
            }
 //#-------------------------------------------------------------------------#

            // Black Turn
            else if (flag == 1 && game_prop.w_o_b == 0 && event.button.button == SDL_BUTTON_LEFT && event.button.x<=600){
                flag = 0;
                // Save the last move to a temporary variable to check if it is valid or not
                piece[chosen_piece].xtemp = ((event.button.x) / 75) * 75;
                piece[chosen_piece].ytemp = ((event.button.y) / 75) * 75;
                // Check if this move is valid or not
                if (check_valid()){
                    // If the king was checked, clear the check
                    if(game_prop.b_check == 1){
                        game_prop.b_check = 0;
                        game_prop.cover = 0;
                    }
                    // Update the (x, y) coordinates for the piece
                    piece[chosen_piece].x = ((event.button.x) / 75) * 75;
                    piece[chosen_piece].y = ((event.button.y) / 75) * 75;
                    // Record the white movement and update the boards
                    black_move();
                    // Preparing for en passant
                    if(game_prop.en_passant >= 0 && piece[chosen_piece].type == 'p' &&
                    piece[chosen_piece].brow_prev == 1 && piece[chosen_piece].brow == 3){
                        game_prop.en_passant = 2;
                    }
                    // Check if there is a promotion or not
                    if(piece[chosen_piece].type == 'p' && piece[chosen_piece].brow == 7){
                        game_prop.bpromo = 1;
                        render();
                    }else{
                        game_prop.bpromo = 0;
                    }
                    // Check if there is castling or not
                    if(game_prop.brc_w_k && (event.button.x / 75 == 6) && piece[4].bcol == 6 && piece[4].brow == 0){
                        black_right_castling();
                    }
                    else if(game_prop.blc_w_k && (event.button.x / 75 == 2) && piece[4].bcol == 2 && piece[4].brow == 0){
                        black_left_castling();
                    }
                    // Check if the move result in a kill or not
                    if(check_die()){
                        for(i3 = 16; i3 < 32; i3++){
                            // If there is a normal kill
                            if(event.button.x > piece[i3].x && event.button.x < piece[i3].x + 75 && event.button.y > piece[i3].y
                                && event.button.y < piece[i3].y + 75 && event.button.button == SDL_BUTTON_LEFT && i3 != chosen_piece){
                                black_kill(i3);
                                break;
                            }
                            // For en passant kill
                            else if (board_types[piece[chosen_piece].brow - 1][piece[chosen_piece].bcol] == wp
                                && board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] == bp &&
                                piece[chosen_piece].brow == 5 &&
                                event.button.x > piece[i3].x && event.button.x < piece[i3].x + 75 && (event.button.y) - 75 > piece[i3].y
                                && (event.button.y) - 75 < piece[i3].y + 75 && event.button.button == SDL_BUTTON_LEFT && i3 != chosen_piece){
                                black_kill_passant(i3);
                                break;
                            }
                        }
                    }
                    // If there is a promotion, toggle the turn
                    if(game_prop.bpromo){
                        game_prop.w_o_b = 0;
                    }else{
                        game_prop.w_o_b = 1;
                        // Preparing for en passant
                        if(game_prop.en_passant > 0){
                            game_prop.en_passant--;
                            game_prop.passant[game_prop.prev] = game_prop.en_passant;
                        }
                    }
                }else{
                    // If the move was not valid, keep everything in its place
                    piece[chosen_piece].x = piece[chosen_piece].x_prev;
                    piece[chosen_piece].y = piece[chosen_piece].y_prev;
                    wrong = 1;
                    render();
                    SDL_Delay(1000);
                    wrong = 0;
                    game_prop.w_o_b = 0;
                }
            }
            // Reset the values of arrays
            clearing();

//#-------------------------------------------------------------------------#

            // Save the chosen piece in a temporary variable
            temp_chosen = chosen_piece;
            // If the king was checked, check if it is the end of the game or not
            if(game_prop.b_check == 1){
                chosen_piece = 4;
                // Get the valid moves of king's piece
                get_valid_moves(chosen_piece);
                // Check if it was a check mate or a normal check
                check_mate(chosen_piece);
                if(game_prop.b_check == 1 && game_prop.cover == 0 && piece[4].valid_moves_x[0] == -10){
                    game_prop.b_mate = 1;
                }
            }else{
                game_prop.b_mate = 0;
            }
            // If the king was checked, check if it is the end of the game or not
            if(game_prop.w_check == 1){
                chosen_piece = 28;
                // Get the valid moves of king's piece
                get_valid_moves(chosen_piece);
                // Check if it was a check mate or a normal check
                check_mate(chosen_piece);
                if(game_prop.w_check == 1 && game_prop.cover == 0 && piece[28].valid_moves_x[0] == -10){
                    game_prop.w_mate = 1;
                }
            }else{
                game_prop.w_mate = 0;
            }
            chosen_piece = temp_chosen;

//#-------------------------------------------------------------------------#

            // Declaration of a color
            SDL_Color color = {255, 255, 255};
            // Print the move in letters
            if(piece[chosen_piece].brow_prev == -1){
                moves_prev_surf = TTF_RenderText_Solid(font, board_moves[piece[chosen_piece].brow][piece[chosen_piece].bcol], color);
            }else{
                moves_prev_surf = TTF_RenderText_Solid(font, board_moves[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev], color);
            }
            moves_surf = TTF_RenderText_Solid(font, board_moves[piece[chosen_piece].brow][piece[chosen_piece].bcol], color);

//#-------------------------------------------------------------------------#

            f_save = 0;
            f_load = 0;
            f_undo = 0;
            f_redo = 0;
            // Reset the values of arrays
            clearing();
            break;
    }
}

//#-------------------------------------------------------------------------#

// Display all data which we collected on the screen:
// Render every 1000 / FPS
void render(void){
    int n, z;
    // Declaration of the SDL Variables
    SDL_Rect piece_dim[32], valid_moves[63], not_valid, undo, redo, wpromo_dim, bpromo_dim, black_turn_dim, white_turn_dim;
    SDL_Rect check_dim, b_mate_dim, w_mate_dim, save_dim, load_dim, d_pos_dim, stale_dim, moves_dim, moves_prev_dim, press_dim[4];

//----------------------------// Things to render //----------------------------//

    // Render the Board
    SDL_RenderCopy(renderer, texture[0], NULL, NULL);
//-----------------------------------------------//
    // Render the turn to be for white
    if(game_prop.w_o_b == 1 && game_prop.w_mate == 0){
        white_turn_dim.x = 600;
        white_turn_dim.y = 400;
        white_turn_dim.w = 100;
        white_turn_dim.h = 100;
        SDL_RenderCopy(renderer, white_turn_texture, NULL, &white_turn_dim);
    }
//-----------------------------------------------//
    // Render the turn to be for black
    if(game_prop.w_o_b == 0 && game_prop.b_mate == 0){
        black_turn_dim.x = 600;
        black_turn_dim.y = 100;
        black_turn_dim.w = 100;
        black_turn_dim.h = 100;
        SDL_RenderCopy(renderer, black_turn_texture, NULL, &black_turn_dim);
    }
//-----------------------------------------------//
    // Render the load icon
    load_dim.x = 600;
    load_dim.y = 0;
    load_dim.w = 100;
    load_dim.h = 100;
    SDL_RenderCopy(renderer, load_texture, NULL, &load_dim);
//-----------------------------------------------//
    // Render the save icon
    save_dim.x = 600;
    save_dim.y = 500;
    save_dim.w = 100;
    save_dim.h = 100;
    SDL_RenderCopy(renderer, save_texture, NULL, &save_dim);
//-----------------------------------------------//
    // Render the undo icon
    undo.x = 850;
    undo.y = 400;
    undo.w = 100;
    undo.h = 100;
    SDL_RenderCopy(renderer, undo_texture, NULL, &undo);
//-----------------------------------------------//
    // Render the redo icon
    redo.x = 850;
    redo.y = 500;
    redo.w = 100;
    redo.h = 100;
    SDL_RenderCopy(renderer, redo_texture, NULL, &redo);
//-----------------------------------------------//
    // Render pressing motion
    if(f_load == 1){
        press_dim[0].x = 600;
        press_dim[0].y = 0;
        press_dim[0].w = 100;
        press_dim[0].h = 100;
        SDL_RenderCopy(renderer, press_texture[0], NULL, &press_dim[0]);
    }
//-----------------------------------------------//
    // Render pressing motion
    if(f_save == 1){
        press_dim[1].x = 600;
        press_dim[1].y = 500;
        press_dim[1].w = 100;
        press_dim[1].h = 100;
        SDL_RenderCopy(renderer, press_texture[1], NULL, &press_dim[1]);
    }
//-----------------------------------------------//
    // Render pressing motion
    if(f_undo == 1){
        press_dim[2].x = 850;
        press_dim[2].y = 400;
        press_dim[2].w = 100;
        press_dim[2].h = 100;
        SDL_RenderCopy(renderer, press_texture[2], NULL, &press_dim[2]);
    }
//-----------------------------------------------//
    // Render pressing motion
    if(f_redo == 1){
        press_dim[3].x = 850;
        press_dim[3].y = 500;
        press_dim[3].w = 100;
        press_dim[3].h = 100;
        SDL_RenderCopy(renderer, press_texture[3], NULL, &press_dim[3]);
    }
//-----------------------------------------------//
    // Render the movement in letters (from the old position)
    moves_prev_texture = SDL_CreateTextureFromSurface(renderer, moves_prev_surf);
    moves_prev_dim.x = 620;
    moves_prev_dim.y = 285;
    moves_prev_dim.w = 30;
    moves_prev_dim.h = 30;
    SDL_RenderCopy(renderer, moves_prev_texture, NULL, &moves_prev_dim);
//-----------------------------------------------//
    // Render the movement in letters (to the new position)
    moves_texture = SDL_CreateTextureFromSurface(renderer, moves_surf);
    moves_dim.x = 650;
    moves_dim.y = 285;
    moves_dim.w = 30;
    moves_dim.h = 30;
    SDL_RenderCopy(renderer, moves_texture, NULL, &moves_dim);
//-----------------------------------------------//
    // If there is a normal check, render "check" word
    if(game_prop.w_check || game_prop.b_check){
        check_dim.x = 850;
        check_dim.y = 270;
        check_dim.w = 100;
        check_dim.h = 40;
        SDL_RenderCopy(renderer, check_texture, NULL, &check_dim);
    }
//-----------------------------------------------//
    // Render a menu, if there is a promotion for white
    if(game_prop.wpromo){
        wpromo_dim.x = 850;
        wpromo_dim.y = 0;
        wpromo_dim.w = 100;
        wpromo_dim.h = 300;
        SDL_RenderCopy(renderer, wpromo_texture, NULL, &wpromo_dim);
    }
//-----------------------------------------------//
    // Render a menu, if there is a promotion for black
    if(game_prop.bpromo){
        bpromo_dim.x = 850;
        bpromo_dim.y = 0;
        bpromo_dim.w = 100;
        bpromo_dim.h = 300;
        SDL_RenderCopy(renderer, bpromo_texture, NULL, &bpromo_dim);
    }
//-----------------------------------------------//
    // Render a texture for invalid moves
    if(wrong){
        not_valid.x = piece[chosen_piece].xtemp;
        not_valid.y = piece[chosen_piece].ytemp;
        not_valid.w = 75;
        not_valid.h = 75;
        SDL_RenderCopy(renderer, not_valid_texture, NULL, &not_valid);
    }
//-----------------------------------------------//
    // Render a texture for valid moves
    if(guide){
        for(z = 0; z < piece[chosen_piece].cx; z++){
            valid_moves[z].x = piece[chosen_piece].valid_moves_x[z];
            valid_moves[z].y = piece[chosen_piece].valid_moves_y[z];
            valid_moves[z].w = 75;
            valid_moves[z].h = 75;
            SDL_RenderCopy(renderer, valid_textures[z], NULL, &valid_moves[z]);
        }
    }
//-----------------------------------------------//
    // When a redo is done, reload the promoted piece (for white player)
    if(game_prop.do_bpromo){
        texture[chosen_piece + 1] = texture[12];
        game_prop.do_bpromo = 0;
    }
//-----------------------------------------------//
    // When a redo is done, reload the promoted piece (for black player)
    else if(game_prop.do_wpromo){
        texture[chosen_piece + 1] = texture[21];
        game_prop.do_wpromo = 0;
    }
//-----------------------------------------------//
    // Render the promoted pawn to a black rook
    if(game_prop.bpr){
        texture[chosen_piece + 1] = texture[1];
        game_prop.bpr = 0;
    }
//-----------------------------------------------//
    // Render the promoted pawn to a black bishop
    else if(game_prop.bpb){
        texture[chosen_piece + 1] = texture[3];
        game_prop.bpb = 0;
    }
//-----------------------------------------------//
    // Render the promoted pawn to a black knight
    else if(game_prop.bpn){
        texture[chosen_piece + 1] = texture[2];
        game_prop.bpn = 0;
    }
//-----------------------------------------------//
    // Render the promoted pawn to a black queen
    else if(game_prop.bpq){
        texture[chosen_piece + 1] = texture[4];
        game_prop.bpq = 0;
    }
//-----------------------------------------------//
    // Render the promoted pawn to a white rook
    else if(game_prop.wpr){
        texture[chosen_piece + 1] = texture[32] ;
        game_prop.wpr = 0;
    }
//-----------------------------------------------//
    // Render the promoted pawn to a white bishop
    else if(game_prop.wpb){
        texture[chosen_piece + 1] = texture[30];
        game_prop.wpb = 0;
    }
//-----------------------------------------------//
    // Render the promoted pawn to a white knight
    else if(game_prop.wpn){
        texture[chosen_piece + 1] = texture[31];
        game_prop.wpn = 0;
    }
//-----------------------------------------------//
    // Render the promoted pawn to a white queen
    else if(game_prop.wpq){
        texture[chosen_piece + 1] = texture[28];
        game_prop.wpq = 0;
    }
//-----------------------------------------------//
    // Render all piece
    for(n = 0; n < 32; n++){
        piece_dim[n].x = piece[n].x;
        piece_dim[n].y = piece[n].y;
        piece_dim[n].w = 75;
        piece_dim[n].h = 75;
        SDL_RenderCopy(renderer, texture[n + 1], NULL, &piece_dim[n]);
    }
//-----------------------------------------------//
    // If the game ends in check mate for the black, then white wins
    if(game_prop.b_mate){
        b_mate_dim.x = 0;
        b_mate_dim.y = 0;
        b_mate_dim.w = 600;
        b_mate_dim.h = 600;
        SDL_RenderCopy(renderer, b_mate_texture, NULL, &b_mate_dim);
    }
//-----------------------------------------------//
    // If the game ends in check mate for the white, then black wins
    if(game_prop.w_mate){
        w_mate_dim.x = 0;
        w_mate_dim.y = 0;
        w_mate_dim.w = 600;
        w_mate_dim.h = 600;
        SDL_RenderCopy(renderer, w_mate_texture, NULL, &w_mate_dim);
    }
//-----------------------------------------------//
    // If the game ends in a draw by dead position, render this texture
    if(game_prop.d_pos){
        d_pos_dim.x = 0;
        d_pos_dim.y = 0;
        d_pos_dim.w = 600;
        d_pos_dim.h = 600;
        SDL_RenderCopy(renderer, dead_pos_texture, NULL, &d_pos_dim);
    }
//-----------------------------------------------//
    // If the game ends in a draw by stalemate, render this texture
    if(game_prop.stale){
        stale_dim.x = 0;
        stale_dim.y = 0;
        stale_dim.w = 600;
        stale_dim.h = 600;
        SDL_RenderCopy(renderer, stale_texture, NULL, &stale_dim);
    }
//-----------------------------------------------//
    // Rendering
    SDL_RenderPresent(renderer);
}

//#-------------------------------------------------------------------------#

// Clean memory before closing the program
void destroy_window(void){
    int m;
    for(m = 0; m < 33; m++){
        // Free all images and textures
        SDL_DestroyTexture(texture[m]);
        SDL_FreeSurface(image[m]);
    }
    // Quit the images function
    IMG_Quit();
    // Quit the music player
    Mix_FreeMusic(sound[0]);
    sound[0] = NULL;
    Mix_Quit();
    // Quit the font function
    TTF_CloseFont(font);
    TTF_Quit();
    // Destroy the renderer and the window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    // Quit SDL
    SDL_Quit();
}

//#-------------------------------------------------------------------------#

int SDL_main(int argc, char *argv[]){
    // Initialize the window and renderer
    Ready_To_start = initialize_basics();

    // Close the game window
    HWND windowHandle = GetConsoleWindow();
    ShowWindow(windowHandle, SW_HIDE);
    // Initialize start state for the game
    setup();
    // Make an icon for the game
    SDL_SetWindowIcon(window, icon);

    while(Ready_To_start){
        // Get user inputs and
        process_input();
        // Wait until 300 frame per sec passed to ensure that all devices that open this game will have the same UX
        SDL_Delay(FRAME_TARGET_TIME);
        render();
    }
    // Destroy the window
    destroy_window();
    return 0;
}
//#-------------------------------------------END----------------------------------------------#
=======
// Authors:
// Youssef Ali Mohamed Bazina & Ahmed Adel Abudef

//#--------------------------------------------------------------------------------------------#
// Libraries:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_IMAGE.h>
#include <SDL_TTF.h>
#include <SDL_MIXER.h>
#include <windows.h>
//#--------------------------------------------------------------------------------------------#
#define  FPS 300
#define  FRAME_TARGET_TIME 1000/FPS

//                                -----------<Global variables>------------:

// Initialize a pointer for the window
SDL_Window *window = NULL;
// Initialize a pointers for the images
SDL_Surface *image[33], *valid_images[63], *not_valid_image, *undo_image, *redo_image, *wpromo_image, *bpromo_image, *icon, *white_turn_image , *black_turn_image;
SDL_Surface *check_image, *b_mate_image, *w_mate_image, *dead_pos_image, *stale_image, *moves_surf, *moves_prev_surf, *save_image, *load_image;
SDL_Surface *press_image[4];
// Initialize a pointers for the textures
SDL_Texture *texture[33], *valid_textures[63], *not_valid_texture, *undo_texture, *redo_texture, *wpromo_texture, *bpromo_texture, *check_texture, *white_turn_texture , *black_turn_texture;
SDL_Texture *b_mate_texture, *w_mate_texture, *dead_pos_texture, *stale_texture, *moves_texture, *moves_prev_texture, *save_texture, *load_texture;
SDL_Texture *press_texture[4];
TTF_Font *font;
// Initialize a pointers for the sounds
Mix_Music *sound[3];
// Initialize a pointer for the render
SDL_Renderer *renderer = NULL;
// Initialize save file location:
FILE *last_scene;
//-----------------------------------------------//

int Ready_To_start = 0;
int st = 0, dp = 0, enemy_line1 = 0, enemy_line2 = 0, enemy_line3 = 0, enemy_line4 = 0, enemy_line5 = 0, enemy_line6 = 0, enemy_line7 = 0, enemy_line8 = 0;
int flag = 0, sum = 0, chosen_piece = 0, dead_piece = 0, guide = 0, wrong = 0, f_save = 0, f_load = 0, f_undo = 0, f_redo = 0, temp_chosen, down;
int k1 = 0, k2 = 0, k3 = 0, k4 = 0, k5 = 0, k6 = 0, k7 = 0, k8 = 0, k9 = 0, k10 = 0, k11 = 0, k12 = 0;
int l1 = 0, l2 = 0, l3 = 0, l4 = 0, l5 = 0, l6 = 0, l7 = 0, l8 = 0;
int line_moves_x[63], line_moves_y[63], line_y = 0, line_x = 0;
int valid_line_x[63], valid_line_y[63], shield_x = 0, shield_y = 0;
int die_x[63], x, die_y[63], y, wdx = 0, wdy = 0, bdx = 0, bdy = 0, w = 0, s = 0, e = 0, P_king;

//-----------------------------------------------//
// Initialize structure to contain all dynamic properties of the game loop:
typedef struct game_properties{
    // Flag for Dead position and stalemate
    int d_pos;
    int stale;
    // Flags for promotion
    int wpromo;
    int bpromo;
    int bpr;
    int bpb;
    int bpn;
    int bpq;
    int wpr;
    int wpb;
    int wpn;
    int wpq;
    // Flag to toggle turn
    int w_o_b;
    // Counter for undo and redo times
    int do_count;
    // Flags for cover
    int cover;
    int covered;
    int cov_shield;
    // Flags for check and threaten piece
    int king_check;
    int check;
    int w_check;
    int b_check;
    // Threaten piece number
    int T_piece;
    int b_mate;
    int w_mate;
    // Flags for Castling
    int brc_w_k;
    int blc_w_k;
    int wrc_w_k;
    int wlc_w_k;
    // Counter, Arrays and Flags for Undo and Redo Features
    int prev;
    // Flags for castling
    int bcas;
    int wcas;
    // Flags for promotion
    int do_wpromo;
    int do_bpromo;
    // Save the chosen pieces along the game (Undo)
    int chosen_undo[5000];
    // Save the previous coordinates for the chosen pieces
    int do_moves_x[5000];
    int do_moves_y[5000];
    // Save the dead pieces along the game (Undo)
    int dead_undo[5000];
    // Save the previous coordinates for the dead pieces
    int dead_moves_x[5000];
    int dead_moves_y[5000];
    // Save the previous row and column numbers for the chosen pieces
    int prev_bcol[5000];
    int prev_brow[5000];
    // Save the previous row and column numbers for the dead pieces
    int prev_bcol_dead[5000];
    int prev_brow_dead[5000];
    // Save the chosen pieces along the game (Redo)
    int chosen_redo[5000];
    // Save the new coordinates for the chosen pieces
    int redo_moves_x[5000];
    int redo_moves_y[5000];
    // Save the dead pieces along the game (Redo)
    int dead_redo[5000];
    // Save the new coordinates for the dead pieces
    int re_dead_moves_x[5000];
    int re_dead_moves_y[5000];
    // Save all special moves (promotion, kill moves, castling)
    int compare[5000];
    // Save the new row and column numbers for the chosen pieces
    int next_bcol[5000];
    int next_brow[5000];
    // Save the new row and column numbers for the dead pieces
    int next_bcol_dead[5000];
    int next_brow_dead[5000];
    // Save the chosen piece's type for promotion (Redo)
    int redo_promo_type[5000];
    // Save check and stalemate and dead position along the game
    int comp_mate[63];
    // Save the first moves
    int first_move[63];
    // Flag and an array for en passant moves
    int en_passant;
    int passant[32];
}game_properties;
game_properties game_prop;
//-----------------------------------------------//
// Initialize a structure for the pieces to determine its coordinates etc..
typedef struct piece_pos{
    // X and Y coordinates
    int x;
    int y;
    int x_prev;
    int y_prev;
    int xtemp;
    int ytemp;
    // Piece position on the Board
    int brow;
    int bcol;
    int brow_prev;
    int temp_brow_prev;
    int bcol_prev;
    // Piece type
    char type;
    // Valid moves for the pieces
    int valid_moves_x[63];
    int valid_moves_y[63];
    // Cover moves for the pieces
    int valid_cover_x[63];
    int valid_cover_y[63];
    // Counter for the arrays
    int cx;
    int cy;
    int cover_cx;
    int cover_cy;
    // Determine if the piece is live or dead
    int l_o_d;
}pieces_pos;
pieces_pos piece[32];
//---------------------------------------------//
// Initialize a 2D array for the board
// Determine if the chosen piece is black or white
int board[8][8] = {
                {-1, -1, -1, -1, -1, -1, -1, -1},
                {-1, -1, -1, -1, -1, -1, -1, -1},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {1, 1, 1, 1, 1, 1, 1, 1},
                {1, 1, 1, 1, 1, 1, 1, 1}
};
int board_types[8][8] = {
                {-5, -4, -3, -2, -1, -3, -4, -5},
                {-6, -6, -6, -6, -6, -6, -6, -6},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {6, 6, 6, 6, 6, 6, 6, 6},
                {5, 4, 3, 2, 1, 3, 4, 5}
};
// Enumerating numbers to names which refers to piece's type
enum board_elements {br = -5, bn = -4, bb = -3, bq = -2, bk = -1, bp = -6, wp = 6, wk = 1, wq = 2, wb = 3, wn = 4, wr = 5};
// A Board to Determine the move of the piece in letters
char *board_moves[8][8] = {
                {"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"},
                {"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7"},
                {"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6"},
                {"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5"},
                {"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4"},
                {"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3"},
                {"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2"},
                {"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"}
};

//#-------------------------------------------------------------------------#

int initialize_basics(void){

    // Initialize SDL library
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("Error in initialization\n");
        return 0;
    }
    // Initialize SDL_ttf library
    TTF_Init();

    // Initialize the window of the game
    window = SDL_CreateWindow(
        "Chess",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1100,
        600,
        SDL_WINDOW_MINIMIZED
    );

    // Check if the window is running or not
    if(!window){
        printf("Error in window\n");
        return 0;
    }

    // Initialize renderer for the game
    renderer = SDL_CreateRenderer(window, -1, 0);

    // Check if the renderer is running or not
    if(!renderer){
        printf("Error in render\n");
        return 0;
    }
    // Return if all gone well
    return 1;
}

//#-------------------------------------------------------------------------#

// Setup the initial positions for all games elements (board and pieces)
void setup(void){
    int t, j = 0;
    // Load the font
    font = TTF_OpenFont("attach/Roboto-Bold.ttf", 100);

    // Load the images for the whole game:
    icon = IMG_Load("images/icon.png");
    image[0] = IMG_Load("images/board.png");
    image[1] = IMG_Load("images/bcas.png");
    image[2] = IMG_Load("images/bhor.png");
    image[3] = IMG_Load("images/bele.png");
    image[4] = IMG_Load("images/bque.png");
    image[5] = IMG_Load("images/bkin.png");
    image[6] = IMG_Load("images/bele.png");
    image[7] = IMG_Load("images/bhor.png");
    image[8] = IMG_Load("images/bcas.png");
    image[9] = IMG_Load("images/bsol.png");
    image[10] = IMG_Load("images/bsol.png");
    image[11] = IMG_Load("images/bsol.png");
    image[12] = IMG_Load("images/bsol.png");
    image[13] = IMG_Load("images/bsol.png");
    image[14] = IMG_Load("images/bsol.png");
    image[15] = IMG_Load("images/bsol.png");
    image[16] = IMG_Load("images/bsol.png");
    image[17] = IMG_Load("images/wsol.png");
    image[18] = IMG_Load("images/wsol.png");
    image[19] = IMG_Load("images/wsol.png");
    image[20] = IMG_Load("images/wsol.png");
    image[21] = IMG_Load("images/wsol.png");
    image[22] = IMG_Load("images/wsol.png");
    image[23] = IMG_Load("images/wsol.png");
    image[24] = IMG_Load("images/wsol.png");
    image[25] = IMG_Load("images/wcas.png");
    image[26] = IMG_Load("images/whor.png");
    image[27] = IMG_Load("images/wele.png");
    image[28] = IMG_Load("images/wque.png");
    image[29] = IMG_Load("images/wkin.png");
    image[30] = IMG_Load("images/wele.png");
    image[31] = IMG_Load("images/whor.png");
    image[32] = IMG_Load("images/wcas.png");

    press_image[0] = IMG_Load("images/load 2.png");
    press_image[1] = IMG_Load("images/save 2.png");
    press_image[2] = IMG_Load("images/undo 2.png");
    press_image[3] = IMG_Load("images/redo 2.png");

    // Create textures for images:
    for(j = 0; j < 33; j++){
        texture[j] = SDL_CreateTextureFromSurface(renderer, image[j]);
        if(!texture[j]){
            printf("Error in textures\n");
            return 0;
        }
    }
    //-----------------------------------------------//
    not_valid_image = IMG_Load("images/notvalid.png");
    for(t = 0; t < 63; t++){
        valid_images[t]=IMG_Load("images/valid.png");
        if(!valid_images[t]){
            printf("Error in valid_images");
            return 0;
        }
    }

    not_valid_texture = SDL_CreateTextureFromSurface(renderer, not_valid_image);
    for(t = 0; t < 63; t++){
        valid_textures[t] = SDL_CreateTextureFromSurface(renderer,valid_images[t]);
        if(!valid_textures[t]){
            printf("Error in valid_textures\n");
            return 0;
        }
    }

    for(j = 0; j < 4; j++){
        press_texture[j] = SDL_CreateTextureFromSurface(renderer, press_image[j]);
    }
    //-----------------------------------------------//
    wpromo_image = IMG_Load("images/wpromo.png");
    wpromo_texture = SDL_CreateTextureFromSurface(renderer, wpromo_image);

    bpromo_image = IMG_Load("images/bpromo.png");
    bpromo_texture = SDL_CreateTextureFromSurface(renderer, bpromo_image);
    //-----------------------------------------------//
    undo_image = IMG_Load("images/undo.png");
    undo_texture = SDL_CreateTextureFromSurface(renderer, undo_image);
    redo_image = IMG_Load("images/redo.png");
    redo_texture = SDL_CreateTextureFromSurface(renderer, redo_image);
    //-----------------------------------------------//
    check_image = IMG_Load("images/check.png");
    check_texture = SDL_CreateTextureFromSurface(renderer, check_image);
    //-----------------------------------------------//
    b_mate_image = IMG_Load("images/bmate.png");
    b_mate_texture = SDL_CreateTextureFromSurface(renderer, b_mate_image);

    w_mate_image = IMG_Load("images/wmate.png");
    w_mate_texture = SDL_CreateTextureFromSurface(renderer, w_mate_image);
    //-----------------------------------------------//
    save_image = IMG_Load("images/save.png");
    save_texture = SDL_CreateTextureFromSurface(renderer, save_image);

    load_image = IMG_Load("images/load.png");
    load_texture = SDL_CreateTextureFromSurface(renderer, load_image);
    //-----------------------------------------------//
    dead_pos_image = IMG_Load("images/dead.png");
    dead_pos_texture = SDL_CreateTextureFromSurface(renderer, dead_pos_image);

    stale_image = IMG_Load("images/stale.png");
    stale_texture = SDL_CreateTextureFromSurface(renderer, stale_image);
    //-----------------------------------------------//
    white_turn_image = IMG_Load("images/white turn.png");
    white_turn_texture = SDL_CreateTextureFromSurface(renderer, white_turn_image);

    black_turn_image = IMG_Load("images/black turn.png");
    black_turn_texture = SDL_CreateTextureFromSurface(renderer, black_turn_image);
    //-----------------------------------------------//
    // Set types for all pieces
    piece[0].type = 'r';
    piece[1].type = 'n';
    piece[2].type = 'b';
    piece[3].type = 'q';
    piece[4].type = 'k';
    piece[5].type = 'b';
    piece[6].type = 'n';
    piece[7].type = 'r';
    piece[8].type = 'p';
    piece[9].type = 'p';
    piece[10].type = 'p';
    piece[11].type = 'p';
    piece[12].type = 'p';
    piece[13].type = 'p';
    piece[14].type = 'p';
    piece[15].type = 'p';
    piece[16].type = 'p';
    piece[17].type = 'p';
    piece[18].type = 'p';
    piece[19].type = 'p';
    piece[20].type = 'p';
    piece[21].type = 'p';
    piece[22].type = 'p';
    piece[23].type = 'p';
    piece[24].type = 'r';
    piece[25].type = 'n';
    piece[26].type = 'b';
    piece[27].type = 'q';
    piece[28].type = 'k';
    piece[29].type = 'b';
    piece[30].type = 'n';
    piece[31].type = 'r';
    //-----------------------------------------------//
    // Enter initial values for the flags, counter and coordinates for the whole game:
    game_prop.w_o_b = 1;
    game_prop.do_count = 0;
    game_prop.cover = 0;
    game_prop.covered = 0;
    game_prop.wpromo = 0;
    game_prop.bpromo = 0;
    game_prop.do_wpromo = 0;
    game_prop.do_bpromo = 0;
    game_prop.brc_w_k = 0;
    game_prop.blc_w_k = 0;
    game_prop.wrc_w_k = 0;
    game_prop.wlc_w_k = 0;
    game_prop.bcas = 0;
    game_prop.wcas = 0;
    game_prop.prev = -1;
    game_prop.check = 0;
    game_prop.w_check = 0;
    game_prop.b_check = 0;
    game_prop.T_piece = -5;
    game_prop.b_mate = 0;
    game_prop.w_mate = 0;
    game_prop.bpr = 0;
    game_prop.bpb = 0;
    game_prop.bpn = 0;
    game_prop.bpq = 0;
    game_prop.wpr = 0;
    game_prop.wpb = 0;
    game_prop.wpn = 0;
    game_prop.wpq = 0;
    game_prop.d_pos = 0;
    game_prop.stale = 0;
    game_prop.cov_shield = 0;
    game_prop.en_passant = 0;
    //-----------------------------------------------//
    for(j = 0; j < 5000; j++){
        game_prop.compare[j] = -10;
    }
    //-----------------------------------------------//
    for (j = 0; j < 32; j++){
        piece[j].brow_prev = -1;
        piece[j].bcol_prev = -1;
        piece[j].l_o_d = 1;
        game_prop.passant[j] = 0;
    }
    //-----------------------------------------------//
    for (j = 0; j < 63; j++){
        line_moves_x[j] = -15;
        line_moves_x[y] = -15;
    }
    //-----------------------------------------------//
    // Set x and y coordinates for all pieces:
    for (j = 0; j < 8; j++){
        piece[j].y = 0;
        piece[j].x = sum ;
        piece[j].brow = piece[j].y / 75;
        piece[j].bcol = piece[j].x / 75;
        sum = sum + 75;
    }

    sum = 0;
    for (j = 8; j < 16; j++){
        piece[j].y = 75;
        piece[j].x = sum ;
        piece[j].brow = piece[j].y / 75;
        piece[j].bcol = piece[j].x / 75;
        sum = sum + 75;
    }

    sum = 0;
    for (j = 16; j < 24; j++){
        piece[j].y = 450;
        piece[j].x = sum ;
        piece[j].brow = piece[j].y / 75;
        piece[j].bcol = piece[j].x / 75;
        sum = sum + 75;
    }

    sum = 0;
    for (j = 24; j < 32; j++){
        piece[j].y = 525;
        piece[j].x = sum ;
        piece[j].brow = piece[j].y / 75;
        piece[j].bcol = piece[j].x / 75;
        sum = sum + 75;
    }
    //-----------------------------------------------//
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096);
    sound[0] = Mix_LoadMUS("attach/Erik Satie - Gnossienne No.1.mp3");
    Mix_PlayMusic(sound[0], 120);
    Mix_VolumeMusic(9);
}

//#-------------------------------------------------------------------------#

// Merge the moves which covers the king with normal valid moves for the piece
void append(int s){
    int i, j;
    for(i = piece[s].cx, j = 0; i < piece[s].cover_cx + piece[s].cx, j < piece[s].cover_cx; i++, j++){
        piece[s].valid_moves_x[i] = piece[s].valid_cover_x[j];
        piece[s].valid_moves_y[i] = piece[s].valid_cover_y[j];
    }
    piece[s].cx = piece[s].cx + piece[s].cover_cx;
    piece[s].cy = piece[s].cy + piece[s].cover_cy;
}

// Reset all counters and array to its initial values after every move
void clearing(void){
    for(int w = 0; w < 32; w++){
        for(int h = 0; h < 63; h++){
            valid_line_x[h] = -10;
            valid_line_y[h] = -10;
            line_moves_x[h] = -10;
            line_moves_y[h] = -10;
            piece[w].valid_moves_x[h] = -10;
            piece[w].valid_moves_y[h] = -10;
            piece[w].valid_cover_x[h] = -10;
            piece[w].valid_cover_y[h] = -10;
            piece[w].cx = 0;
            piece[w].cy = 0;
            piece[w].cover_cx = 0;
            piece[w].cover_cy = 0;
            die_x[w] = -10;
            die_y[w] = -10;
        }
    }
    shield_x = 0;
    shield_y = 0;
    line_x = 0;
    line_y = 0;
    x = 0;
    y = 0;
    game_prop.blc_w_k = 0;
    game_prop.brc_w_k = 0;
    game_prop.wlc_w_k = 0;
    game_prop.wrc_w_k = 0;
}

//#-------------------------------------------------------------------------#

// White Pawn Movement:
void wsol(int piece_moves, int ally, int enemy, int p, int l){
    // Upper Moves
    if (board[piece[piece_moves].brow - 1][piece[piece_moves].bcol] == 1){
        // Do nothing if there is an ally in front of it
    }if (board[piece[piece_moves].brow - 1][piece[piece_moves].bcol] == 0){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
        piece[piece_moves].cy++;
        if (piece[piece_moves].brow == 6 && board[piece[piece_moves].brow - 2][piece[piece_moves].bcol] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 2) * 75;
            piece[piece_moves].cy++;
        }
    }
    // Upper Right Moves if there is an enemy ahead to kill
    if((board[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 1] == -1 &&
            piece[piece_moves].brow -1 >= 0 && piece[piece_moves].bcol + 1 < 8)){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
        piece[piece_moves].cy++;
        die_x[x] = (piece[piece_moves].bcol + 1) * 75;
        x++;
        die_y[y] = (piece[piece_moves].brow - 1) * 75;
        y++;
    }
    // If there is Ally ahead, save it as cover position
    else if (board[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 1] == 1 &&
            piece[piece_moves].brow -1 >= 0 && piece[piece_moves].bcol + 1 < 8){
        piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol + 1) * 75;
        piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow - 1) * 75;
        piece[piece_moves].cover_cx++;
        piece[piece_moves].cover_cy++;
    }
    // Upper Left Moves if there is an enemy ahead to kill
    if((board[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 1] == -1 &&
            piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol - 1 >= 0)){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
        piece[piece_moves].cy++;
        die_x[x] = (piece[piece_moves].bcol - 1) * 75;
        x++;
        die_y[y] = (piece[piece_moves].brow - 1) * 75;
        y++;
    }
    // If there is Ally ahead, save it as cover position
    else if (board[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 1] == 1 &&
            piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol - 1 >= 0){
        piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol - 1) * 75;
        piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow - 1) * 75;
        piece[piece_moves].cover_cx++;
        piece[piece_moves].cover_cy++;
    }
    // Left Moves if there is an enemy to kill by en passant
    if(board_types[piece[piece_moves].brow][piece[piece_moves].bcol - 1] == bp &&
        game_prop.en_passant > 0 &&
        piece[piece_moves].bcol - 1 >= 0 && piece[piece_moves].brow == 3 &&
        board_types[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 1] == 0){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
        piece[piece_moves].cy++;
        die_x[x] = (piece[piece_moves].bcol - 1) * 75;
        x++;
        die_y[y] = (piece[piece_moves].brow) * 75;
        y++;
    }
    // Right Moves if there is an enemy to kill by en passant
    if(board_types[piece[piece_moves].brow][piece[piece_moves].bcol + 1] == bp &&
        game_prop.en_passant > 0 &&
        piece[piece_moves].bcol + 1 < 8 && piece[piece_moves].brow == 3 &&
        board_types[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 1] == 0){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
        piece[piece_moves].cy++;
        die_x[x] = (piece[piece_moves].bcol + 1) * 75;
        x++;
        die_y[y] = (piece[piece_moves].brow) * 75;
        y++;
    }
}
//-----------------------------------------------//
// Black Pawn Movement:
void bsol(int piece_moves, int ally, int enemy, int p, int l){
    // Lower Moves
    if (board[piece[piece_moves].brow + 1][piece[piece_moves].bcol] == -1){
        // Do nothing if there is an ally in front of it
    }if (board[piece[piece_moves].brow + 1][piece[piece_moves].bcol] == 0){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
        piece[piece_moves].cy++;
        if (piece[piece_moves].brow == 1 && board[piece[piece_moves].brow + 2][piece[piece_moves].bcol] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 2) * 75;
            piece[piece_moves].cy++;
        }
    }
    // Lower Right Moves if there is an enemy ahead to kill
    if((board[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 1] == 1 &&
            piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol + 1 < 8)){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
        piece[piece_moves].cy++;
        die_x[x] = (piece[piece_moves].bcol + 1) * 75;
        x++;
        die_y[y] = (piece[piece_moves].brow + 1) * 75;
        y++;
    }
    // If there is Ally ahead, save it as cover position
    else if (board[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 1] == -1 &&
            piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol + 1 < 8){
        piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol + 1) * 75;
        piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow + 1) * 75;
        piece[piece_moves].cover_cx++;
        piece[piece_moves].cover_cy++;
    }
    // Lower Left Moves if there is an enemy ahead to kill
    if((board[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 1] == 1 &&
            piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol - 1 >= 0)){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
        piece[piece_moves].cy++;
        die_x[x] = (piece[piece_moves].bcol-1) * 75;
        x++;
        die_y[y] = (piece[piece_moves].brow+1) * 75;
        y++;
    }
    // If there is Ally ahead, save it as cover position
    else if (board[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 1] == -1 &&
            piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol - 1 >= 0){
        piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol - 1) * 75;
        piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow + 1) * 75;
        piece[piece_moves].cover_cx++;
        piece[piece_moves].cover_cy++;
    }
    // Left Moves if there is an enemy to kill by en passant
    if(board_types[piece[piece_moves].brow][piece[piece_moves].bcol - 1] == wp &&
        game_prop.en_passant > 0 &&
        piece[piece_moves].bcol - 1 >= 0 && piece[piece_moves].brow == 4 &&
        board_types[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 1] == 0){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
        piece[piece_moves].cy++;
        die_x[x] = (piece[piece_moves].bcol - 1) * 75;
        x++;
        die_y[y] = (piece[piece_moves].brow) * 75;
        y++;
    }
    // Right Moves if there is an enemy to kill by en passant
    if(board_types[piece[piece_moves].brow][piece[piece_moves].bcol + 1] == wp &&
        game_prop.en_passant > 0 &&
        piece[piece_moves].bcol + 1 < 8 && piece[piece_moves].brow == 4 &&
        board_types[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 1] == 0){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
        piece[piece_moves].cy++;
        die_x[x] = (piece[piece_moves].bcol + 1) * 75;
        x++;
        die_y[y] = (piece[piece_moves].brow) * 75;
        y++;
    }
}
//-----------------------------------------------//
// Queen Movement:
void queen(int piece_moves, int ally, int enemy, int p, int l){
    // Positive Horizontal Movement
    for(p = (piece[piece_moves].bcol + 1); p < 8; p++){
        // If there is Ally ahead, save it as cover position
        if(board[piece[piece_moves].brow][p] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = p * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = piece[piece_moves].brow * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[piece[piece_moves].brow][p] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[piece[piece_moves].brow][p] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cy++;
            die_x[x] = p * 75;
            x++;
            die_y[y] = piece[piece_moves].brow * 75;
            y++;
            break;
        }
    }
    // Negative Horizontal Movement
    for(p = (piece[piece_moves].bcol - 1); p >= 0; p--){
        // If there is Ally ahead, save it as cover position
        if(board[piece[piece_moves].brow][p] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = p * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = piece[piece_moves].brow * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[piece[piece_moves].brow][p] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[piece[piece_moves].brow][p] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cy++;
            die_x[x] = p * 75;
            x++;
            die_y[y] = piece[piece_moves].brow * 75;
            y++;
            break;
        }
    }
    // Positive Vertical Movement
    for(p = (piece[piece_moves].brow + 1); p < 8; p++){
        // If there is Ally ahead, save it as cover position
        if(board[p][piece[piece_moves].bcol] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = piece[piece_moves].bcol * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = p * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[p][piece[piece_moves].bcol] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = p * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[p][piece[piece_moves].bcol] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = p * 75;
            piece[piece_moves].cy++;
            die_x[x] = piece[piece_moves].bcol * 75;
            x++;
            die_y[y] = p * 75;
            y++;
            break;
        }
    }
    // Negative Vertical Movement
    for(p = (piece[piece_moves].brow - 1); p >= 0; p--){
        // If there is Ally ahead, save it as cover position
        if(board[p][piece[piece_moves].bcol] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = piece[piece_moves].bcol * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = p * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[p][piece[piece_moves].bcol] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = piece[piece_moves].bcol * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = p * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[p][piece[piece_moves].bcol] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = piece[piece_moves].bcol * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = p * 75;
            piece[piece_moves].cy++;
            die_x[x] = piece[piece_moves].bcol * 75;
            x++;
            die_y[y] = p * 75;
            y++;
            break;
        }
    }
    // Upper Right Movement
    for(p = piece[piece_moves].bcol + 1, l = piece[piece_moves].brow - 1 ; p < 8 && l >= 0; p++, l--){
        // If there is Ally ahead, save it as cover position
        if(board[l][p] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = p * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = l * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[l][p] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[l][p] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
            die_x[x] = p * 75;
            x++;
            die_y[y] = l * 75;
            y++;
            break;
        }
    }
    // Upper Left Movement
    for(p = piece[piece_moves].bcol - 1, l = piece[piece_moves].brow - 1; p >= 0 && l >= 0; p--, l--){
        // If there is Ally ahead, save it as cover position
        if(board[l][p] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = p * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = l * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[l][p] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[l][p] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
            die_x[x] = p * 75;
            x++;
            die_y[y] = l * 75;
            y++;
            break;
        }
    }
    // Lower Right Movement
    for(p = piece[piece_moves].bcol + 1, l = piece[piece_moves].brow + 1; p < 8 && l < 8; p++, l++){
        // If there is Ally ahead, save it as cover position
        if(board[l][p] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = p * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = l * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[l][p] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[l][p] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
            die_x[x] = p * 75;
            x++;
            die_y[y] = l * 75;
            y++;
            break;
        }
    }
    // Lower Left Movement
    for(p = piece[piece_moves].bcol - 1,l = piece[piece_moves].brow + 1;p >= 0 && l < 8; p--, l++){
        // If there is Ally ahead, save it as cover position
        if(board[l][p] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = p * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = l * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[l][p] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[l][p] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
            die_x[x] = p * 75;
            x++;
            die_y[y] = l * 75;
            y++;
            break;
        }
    }
}
//-----------------------------------------------//
// Bishop Movement:
void bishop(int piece_moves, int ally, int enemy, int p, int l){
    // Upper Right Movement
    for(p = piece[piece_moves].bcol + 1, l = piece[piece_moves].brow - 1 ; p < 8 && l >= 0; p++, l--){
        // If there is Ally ahead, save it as cover position
        if(board[l][p] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = p * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = l * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[l][p] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[l][p] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
            die_x[x] = p * 75;
            x++;
            die_y[y] = l * 75;
            y++;
            break;
        }
    }
    // Upper Left Movement
    for(p = piece[piece_moves].bcol - 1, l = piece[piece_moves].brow - 1; p >= 0 && l >= 0; p--, l--){
        // If there is Ally ahead, save it as cover position
        if(board[l][p] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = p * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = l * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[l][p] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[l][p] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
            die_x[x] = p * 75;
            x++;
            die_y[y] = l * 75;
            y++;
            break;
        }
    }
    // Lower Right Movement
    for(p = piece[piece_moves].bcol + 1, l = piece[piece_moves].brow + 1; p < 8 && l < 8; p++, l++){
        // If there is Ally ahead, save it as cover position
        if(board[l][p] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = p * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = l * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[l][p] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[l][p] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
            die_x[x] = p * 75;
            x++;
            die_y[y] = l * 75;
            y++;
            break;
        }
    }
    // Lower Left Movement
    for(p = piece[piece_moves].bcol - 1,l = piece[piece_moves].brow + 1;p >= 0 && l < 8; p--, l++){
        // If there is Ally ahead, save it as cover position
        if(board[l][p] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = p * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = l * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[l][p] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[l][p] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = l * 75;
            piece[piece_moves].cy++;
            die_x[x] = p * 75;
            x++;
            die_y[y] = l * 75;
            y++;
            break;
        }
    }
}
//-----------------------------------------------//
// Knight Movement:
void knight(int piece_moves, int ally, int enemy, int p, int l){
    // 2 Lower 1 Left Movement
    // If there is no one, save it as a valid move
    if ((board[piece[piece_moves].brow + 2][piece[piece_moves].bcol - 1] == 0
        || board[piece[piece_moves].brow + 2][piece[piece_moves].bcol - 1] == enemy) &&
        piece[piece_moves].brow + 2 < 8 && piece[piece_moves].bcol - 1 >= 0){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 2) * 75;
        piece[piece_moves].cy++;
        // If there is an enemy ahead, save it as a kill position
        if (board[piece[piece_moves].brow + 2][piece[piece_moves].bcol - 1] == enemy){
            die_x[x] = (piece[piece_moves].bcol - 1) * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow + 2) * 75;
            y++;
        }
    }
    // If there is Ally ahead, save it as cover position
    else if (board[piece[piece_moves].brow + 2][piece[piece_moves].bcol - 1] == ally){
        piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol - 1) * 75;
        piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow + 2) * 75;
        piece[piece_moves].cover_cx++;
        piece[piece_moves].cover_cy++;
    }
    // 2 Lower 1 Right Movement
    // If there is no one, save it as a valid move
    if ((board[piece[piece_moves].brow + 2][piece[piece_moves].bcol + 1] == 0
        || board[piece[piece_moves].brow + 2][piece[piece_moves].bcol + 1] == enemy) &&
        piece[piece_moves].brow + 2 < 8 && piece[piece_moves].bcol + 1 < 8){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 2) * 75;
        piece[piece_moves].cy++;
        // If there is an enemy ahead, save it as a kill position
        if (board[piece[piece_moves].brow + 2][piece[piece_moves].bcol + 1] == enemy){
            die_x[x] = (piece[piece_moves].bcol + 1) * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow + 2) * 75;
            y++;
        }
    }
    // If there is Ally ahead, save it as cover position
    else if (board[piece[piece_moves].brow + 2][piece[piece_moves].bcol + 1] == ally){
        piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol + 1) * 75;
        piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow + 2) * 75;
        piece[piece_moves].cover_cx++;
        piece[piece_moves].cover_cy++;
    }
    // 2 Upper 1 Left Movement
    // If there is no one, save it as a valid move
    if ((board[piece[piece_moves].brow - 2][piece[piece_moves].bcol - 1] == 0
        || board[piece[piece_moves].brow - 2][piece[piece_moves].bcol - 1] == enemy) &&
        piece[piece_moves].brow - 2 >= 0 && piece[piece_moves].bcol - 1 >= 0){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 2) * 75;
        piece[piece_moves].cy++;
        // If there is an enemy ahead, save it as a kill position
        if (board[piece[piece_moves].brow - 2][piece[piece_moves].bcol - 1] == enemy){
            die_x[x] = (piece[piece_moves].bcol - 1) * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow - 2) * 75;
            y++;
        }
    }
    // If there is Ally ahead, save it as cover position
    else if (board[piece[piece_moves].brow - 2][piece[piece_moves].bcol - 1] == ally){
        piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol - 1) * 75;
        piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow - 2) * 75;
        piece[piece_moves].cover_cx++;
        piece[piece_moves].cover_cy++;
    }
    // 2 Upper 1 Right Movement
    // If there is no one, save it as a valid move
    if ((board[piece[piece_moves].brow - 2][piece[piece_moves].bcol + 1] == 0
        || board[piece[piece_moves].brow - 2][piece[piece_moves].bcol + 1] == enemy) &&
        piece[piece_moves].brow - 2 >= 0 && piece[piece_moves].bcol + 1 < 8){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 2) * 75;
        piece[piece_moves].cy++;
        // If there is an enemy ahead, save it as a kill position
        if (board[piece[piece_moves].brow - 2][piece[piece_moves].bcol + 1] == enemy){
            die_x[x] = (piece[piece_moves].bcol + 1) * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow - 2) * 75;
            y++;
        }
    }
    // If there is Ally ahead, save it as cover position
    else if (board[piece[piece_moves].brow - 2][piece[piece_moves].bcol + 1] == ally){
        piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol + 1) * 75;
        piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow - 2) * 75;
        piece[piece_moves].cover_cx++;
        piece[piece_moves].cover_cy++;
    }
    // 2 Right 1 Lower Movement
    // If there is no one, save it as a valid move
    if ((board[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 2] == 0
        || board[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 2] == enemy) &&
        piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol + 2 < 8){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 2) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
        piece[piece_moves].cy++;
        // If there is an enemy ahead, save it as a kill position
        if (board[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 2] == enemy){
            die_x[x] = (piece[piece_moves].bcol + 2) * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow + 1) * 75;
            y++;
        }
    }
    // If there is Ally ahead, save it as cover position
    else if (board[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 2] == ally){
        piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol + 2) * 75;
        piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow + 1) * 75;
        piece[piece_moves].cover_cx++;
        piece[piece_moves].cover_cy++;
    }
    // 2 Right 1 Upper Movement
    // If there is no one, save it as a valid move
    if ((board[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 2] == 0
        || board[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 2] == enemy) &&
        piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol + 2 < 8){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 2) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
        piece[piece_moves].cy++;
        // If there is an enemy ahead, save it as a kill position
        if (board[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 2] == enemy){
            die_x[x] = (piece[piece_moves].bcol + 2) * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow - 1) * 75;
            y++;
        }
    }
    // If there is Ally ahead, save it as cover position
    else if (board[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 2] == ally){
        piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol + 2) * 75;
        piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow - 1) * 75;
        piece[piece_moves].cover_cx++;
        piece[piece_moves].cover_cy++;
    }
    // 2 Left 1 Lower Movement
    // If there is no one, save it as a valid move
    if ((board[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 2] == 0
        || board[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 2] == enemy) &&
        piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol - 2 >= 0){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 2) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
        piece[piece_moves].cy++;
        // If there is an enemy ahead, save it as a kill position
        if (board[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 2] == enemy){
            die_x[x] = (piece[piece_moves].bcol - 2) * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow + 1) * 75;
            y++;
        }
    }
    // If there is Ally ahead, save it as cover position
    else if (board[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 2] == ally){
        piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol - 2) * 75;
        piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow + 1) * 75;
        piece[piece_moves].cover_cx++;
        piece[piece_moves].cover_cy++;
    }
    // 2 Left 1 Upper Movement
    // If there is no one, save it as a valid move
    if ((board[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 2] == 0
        || board[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 2] == enemy) &&
        piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol - 2 >= 0){
        piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 2) * 75;
        piece[piece_moves].cx++;
        piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
        piece[piece_moves].cy++;
        // If there is an enemy ahead, save it as a kill position
        if (board[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 2] == enemy){
            die_x[x] = (piece[piece_moves].bcol - 2) * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow - 1) * 75;
            y++;
        }
    }
    // If there is Ally ahead, save it as cover position
    else if (board[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 2] == ally){
        piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol - 2) * 75;
        piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow - 1) * 75;
        piece[piece_moves].cover_cx++;
        piece[piece_moves].cover_cy++;
    }
}
//-----------------------------------------------//
// Rook Movement:
void rook(int piece_moves, int ally, int enemy, int p, int l){
    // Right Direction Movement
    for(p = (piece[piece_moves].bcol + 1); p < 8; p++){
        // If there is Ally ahead, save it as cover position
        if(board[piece[piece_moves].brow][p] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = p * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = piece[piece_moves].brow * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[piece[piece_moves].brow][p] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[piece[piece_moves].brow][p] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cy++;
            die_x[x] = p * 75;
            x++;
            die_y[y] = piece[piece_moves].brow * 75;
            y++;
            break;
        }
    }
    // Left Direction Movement
    for(p = (piece[piece_moves].bcol - 1); p >= 0; p--){
        // If there is Ally ahead, save it as cover position
        if(board[piece[piece_moves].brow][p] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = p * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = piece[piece_moves].brow * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[piece[piece_moves].brow][p] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[piece[piece_moves].brow][p] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = p * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cy++;
            die_x[x] = p * 75;
            x++;
            die_y[y] = piece[piece_moves].brow * 75;
            y++;
            break;
        }
    }
    // Lower Movement
    for(p = (piece[piece_moves].brow + 1); p < 8; p++){
        // If there is Ally ahead, save it as cover position
        if(board[p][piece[piece_moves].bcol] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = piece[piece_moves].bcol * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = p * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[p][piece[piece_moves].bcol] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = p * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[p][piece[piece_moves].bcol] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = p * 75;
            piece[piece_moves].cy++;
            die_x[x] = piece[piece_moves].bcol * 75;
            x++;
            die_y[y] = p * 75;
            y++;
            break;
        }
    }
    // Upper Movement
    for(p = (piece[piece_moves].brow - 1); p >= 0; p--){
        // If there is Ally ahead, save it as cover position
        if(board[p][piece[piece_moves].bcol] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = piece[piece_moves].bcol * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = p * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
            break;
        }
        // If there is no one, save it as valid position
        else if(board[p][piece[piece_moves].bcol] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = piece[piece_moves].bcol * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = p * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[p][piece[piece_moves].bcol] == enemy){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = piece[piece_moves].bcol * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = p * 75;
            piece[piece_moves].cy++;
            die_x[x] = piece[piece_moves].bcol * 75;
            x++;
            die_y[y] = p * 75;
            y++;
            break;
        }
    }
}
//-----------------------------------------------//
// King Movement:
void king(int piece_moves, int ally, int enemy, int p, int l){
    int j, temp_bcol, temp_brow, temp_b1, temp_b2;
    // Stopping the loop if the piece which the player chose equal to the same the king's piece
    if(piece_moves == chosen_piece){
        // Check all enemy's valid moves which attack the king's valid moves
        for(s; s < e; s++){
            // If the king was checked, consider this action
            if(game_prop.w_check || game_prop.b_check){
                temp_bcol = piece[chosen_piece].bcol;
                temp_brow = piece[chosen_piece].brow;
                temp_b1 = board[piece[chosen_piece].brow][piece[chosen_piece].bcol];
                temp_b2 = board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol];
                board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
            }
            // Get the valid moves the enemy piece
            get_valid_moves(s);
            // If there was a check, remove the action which was considered
            if(game_prop.w_check || game_prop.b_check){
                board[temp_brow][temp_bcol] = temp_b1;
                board_types[temp_brow][temp_bcol] = temp_b2;
            }
            // Merge the cover moves with valid moves
            append(s);
            // If the piece was a black pawn and white king, take the following action
            if(piece[s].type == 'p' && s < 16){
                // Check the intersection between the black pawn valid moves and white king valid moves
                // Then remove the intersected moves as the king will be checked
                // Right Direction Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol + 1 &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol + 1 &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow)){
                    k1 = 1;
                }
                // Left Direction Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol - 1 &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol - 1 &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow)){
                    k2 = 1;
                }
                // Lower Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol  &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow + 1) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow + 1)){
                    k3 = 1;
                }
                // Upper Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol  &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow - 1) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow - 1)){
                    k4 = 1;
                }
                // Upper Right Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol + 1 &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow - 1)||(((piece[s].x) / 75) + 1 == piece[piece_moves].bcol + 1 &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow - 1)){
                    k5 = 1;
                }
                // Upper Left Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol - 1 &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow - 1)||(((piece[s].x) / 75) + 1 == piece[piece_moves].bcol - 1 &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow - 1)){
                    k6 = 1;
                }
                // Lower Right Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol + 1 &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow + 1)||(((piece[s].x) / 75) + 1 == piece[piece_moves].bcol + 1 &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow + 1)){
                    k7 = 1;
                }
                // Lower Left Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol - 1 &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow + 1) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol - 1 &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow + 1)){
                    k8 = 1;
                }
                // 2 Left Direction Movement or 1 Left Direction Movement (White King Castling on The Left Side)
                if((((piece[s].x / 75) - 1 == piece[piece_moves].bcol - 1 ||
                (piece[s].x / 75) - 1 == piece[piece_moves].bcol - 2) || (((piece[s].x / 75) + 1 == piece[piece_moves].bcol - 1 ||
                (piece[s].x / 75) + 1 == piece[piece_moves].bcol - 2))) &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow){
                    k9 = 1;
                }
                // 2 Right Direction Movement or 1 Right Direction Movement (White King Castling on The Right Side)
                if((((piece[s].x / 75) - 1 == piece[piece_moves].bcol + 1 ||
                (piece[s].x / 75) - 1 == piece[piece_moves].bcol + 2) || (((piece[s].x / 75) + 1 == piece[piece_moves].bcol + 1 ||
                (piece[s].x / 75) + 1 == piece[piece_moves].bcol + 2))) &&
                (piece[s].y / 75) + 1 == piece[piece_moves].brow){
                    k11 = 1;
                }
            }
            // If the piece was a white pawn and black king, take the following action
            else if(piece[s].type == 'p' && s >= 16){
                // Check the intersection between the white pawn valid moves and black king valid moves
                // Then remove the intersected moves as the king will be checked
                // Right Direction Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol + 1 &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol + 1 &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow)){
                    k1 = 1;
                }
                // Left Direction Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol - 1 &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol - 1 &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow)){
                    k2 = 1;
                }
                // Lower Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol  &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow + 1) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow + 1)){
                    k3 = 1;
                }
                // Upper Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol  &&
                (piece[s].y/ 75) - 1 == piece[piece_moves].brow - 1) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol &&
                (piece[s].y/ 75) - 1 == piece[piece_moves].brow - 1)){
                    k4 = 1;
                }
                // Upper Right Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol + 1 &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow - 1) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol + 1 &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow - 1)){
                    k5 = 1;
                }
                // Upper Left Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol - 1 &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow - 1) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol - 1 &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow - 1)){
                    k6 = 1;
                }
                // Lower Right Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol + 1 &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow + 1) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol + 1 &&
                (piece[s].y/ 75) - 1 == piece[piece_moves].brow + 1)){
                    k7 = 1;
                }
                // Lower Left Movement
                if((((piece[s].x) / 75) - 1 == piece[piece_moves].bcol - 1 &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow + 1) || (((piece[s].x) / 75) + 1 == piece[piece_moves].bcol - 1 &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow + 1)){
                    k8 = 1;
                }
                // 2 Left Direction Movement or 1 Left Direction Movement (Black King Castling on The Left Side)
                if((((piece[s].x / 75) - 1 == piece[piece_moves].bcol - 1 ||
                (piece[s].x / 75) - 1 == piece[piece_moves].bcol - 2) || (((piece[s].x / 75) + 1 == piece[piece_moves].bcol - 1 ||
                (piece[s].x / 75) + 1 == piece[piece_moves].bcol - 2))) &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow){
                    k10 = 1;
                }
                // 2 Right Direction Movement or 1 Right Direction Movement (Black King Castling on The Right Side)
                if((((piece[s].x / 75) - 1 == piece[piece_moves].bcol + 1 ||
                (piece[s].x / 75) - 1 == piece[piece_moves].bcol + 2) || (((piece[s].x / 75) + 1 == piece[piece_moves].bcol + 1 ||
                (piece[s].x / 75) + 1 == piece[piece_moves].bcol + 2))) &&
                (piece[s].y / 75) - 1 == piece[piece_moves].brow){
                    k12 = 1;
                }
            }else{
                // Loop for the rest of the pieces
                for(j = 0; j < piece[s].cx; j++){
                    // Right Direction Movement
                    if((piece[s].valid_moves_x[j]) / 75 == piece[piece_moves].bcol + 1 &&
                    (piece[s].valid_moves_y[j] / 75) == piece[piece_moves].brow){
                        k1 = 1;
                    }
                    // Left Direction Movement
                    if((piece[s].valid_moves_x[j]) / 75 == piece[piece_moves].bcol - 1 &&
                    (piece[s].valid_moves_y[j] / 75) == piece[piece_moves].brow){
                        k2 = 1;
                    }
                    // Lower Movement
                    if((piece[s].valid_moves_x[j]) / 75 == piece[piece_moves].bcol &&
                    (piece[s].valid_moves_y[j] / 75) == piece[piece_moves].brow + 1){
                        k3 = 1;
                    }
                    // Upper Movement
                    if((piece[s].valid_moves_x[j]) / 75 == piece[piece_moves].bcol &&
                    (piece[s].valid_moves_y[j] / 75) == piece[piece_moves].brow - 1){
                        k4 = 1;
                    }
                    // Upper Right Movement
                    if((piece[s].valid_moves_x[j]) / 75 == piece[piece_moves].bcol + 1 &&
                    (piece[s].valid_moves_y[j] / 75) == piece[piece_moves].brow - 1){
                        k5 = 1;
                    }
                    // Upper Left Movement
                    if((piece[s].valid_moves_x[j]) / 75 == piece[piece_moves].bcol - 1 &&
                    (piece[s].valid_moves_y[j] / 75) == piece[piece_moves].brow - 1){
                        k6 = 1;
                    }
                    // Lower Right Movement
                    if((piece[s].valid_moves_x[j]) / 75 == piece[piece_moves].bcol + 1 &&
                    (piece[s].valid_moves_y[j] / 75) == piece[piece_moves].brow + 1){
                        k7 = 1;
                    }
                    // Lower Left Movement
                    if((piece[s].valid_moves_x[j]) / 75 == piece[piece_moves].bcol - 1 &&
                    (piece[s].valid_moves_y[j] / 75) == piece[piece_moves].brow + 1){
                        k8 = 1;
                    }
                    // 2 Left Direction Movement or 1 Left Direction Movement (White King Castling on The Left Side)
                    if((piece[s].valid_moves_x[j] / 75 == piece[piece_moves].bcol - 1 ||
                    (piece[s].valid_moves_x[j]) / 75 == piece[piece_moves].bcol - 2) &&
                    ((piece[s].valid_moves_y[j] / 75) == piece[piece_moves].brow && s < 16)){
                        k9 = 1;
                    }
                    // 2 Left Direction Movement or 1 Left Direction Movement (Black King Castling on The Left Side)
                    if((piece[s].valid_moves_x[j] / 75 == piece[piece_moves].bcol - 1 ||
                    (piece[s].valid_moves_x[j]) / 75 == piece[piece_moves].bcol - 2) &&
                    ((piece[s].valid_moves_y[j] / 75) == piece[piece_moves].brow && s >= 16)){
                        k10 = 1;
                    }
                    // 2 Right Direction Movement or 1 Right Direction Movement (White King Castling on The Right Side)
                    if((piece[s].valid_moves_x[j] / 75 == piece[piece_moves].bcol + 1 ||
                    (piece[s].valid_moves_x[j]) / 75 == piece[piece_moves].bcol + 2) &&
                    ((piece[s].valid_moves_y[j] / 75) == piece[piece_moves].brow && s < 16)){
                        k11 = 1;
                    }
                    // 2 Right Direction Movement or 1 Right Direction Movement (Black King Castling on The Right Side)
                    if((piece[s].valid_moves_x[j] / 75 == piece[piece_moves].bcol + 1 ||
                    (piece[s].valid_moves_x[j]) / 75 == piece[piece_moves].bcol + 2) &&
                    ((piece[s].valid_moves_y[j] / 75) == piece[piece_moves].brow && s >= 16)){
                        k12 = 1;
                    }
                }
            }
            // Reset the arrays' elements and counters
            for(j = 0; j < 63; j++){
                piece[s].valid_moves_x[j] = -10;
                piece[s].valid_moves_y[j] = -10;
                piece[s].valid_cover_x[j] = -10;
                piece[s].valid_cover_y[j] = -10;
            }
            piece[s].cx = 0;
            piece[s].cy = 0;
            piece[s].cover_cx = 0;
            piece[s].cover_cy = 0;
        }
        if(board[piece[piece_moves].brow][piece[piece_moves].bcol + 1] == ally){
            // Do nothing if there is an ally
        }
        // Check if the move is valid or not for every move
        if(!k1){
            // Right Direction Movement
            // If there is no one, save it as valid position
            if(board[piece[piece_moves].brow][piece[piece_moves].bcol + 1] == 0 &&
                piece[piece_moves].brow < 8 && piece[piece_moves].bcol + 1 < 8){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
                piece[piece_moves].cy++;
            }
            // If there is an enemy ahead, save it as a kill position
            else if(board[piece[piece_moves].brow][piece[piece_moves].bcol + 1] == enemy &&
                piece[piece_moves].brow < 8 && piece[piece_moves].bcol + 1 < 8){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
                piece[piece_moves].cy++;
                die_x[x] = (piece[piece_moves].bcol + 1) * 75;
                x++;
                die_y[y] = piece[piece_moves].brow * 75;
                y++;
            }
        }else{
            // Reset the flag value
            k1 = 0;
        }
        if(board[piece[piece_moves].brow][piece[piece_moves].bcol - 1] == ally){
            // Do nothing if there is an ally
        }
        if(!k2){
            // Left Direction Movement
            // If there is no one, save it as valid position
            if(board[piece[piece_moves].brow][piece[piece_moves].bcol - 1] == 0 &&
                piece[piece_moves].brow < 8 && piece[piece_moves].bcol - 1 >= 0){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
                piece[piece_moves].cy++;
            }
            // If there is an enemy ahead, save it as a kill position
            else if(board[piece[piece_moves].brow][piece[piece_moves].bcol - 1] == enemy &&
                piece[piece_moves].brow < 8 && piece[piece_moves].bcol - 1 >= 0){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
                piece[piece_moves].cy++;
                die_x[x] = (piece[piece_moves].bcol - 1) * 75;
                x++;
                die_y[y] = piece[piece_moves].brow * 75;
                y++;
            }
        }else{
            // Reset the flag value
            k2 = 0;
        }
        if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol] == ally){
            // Do nothing if there is an ally
        }
        if(!k3){
            // Lower Movement
            // If there is no one, save it as valid position
            if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol] == 0 &&
                piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol < 8){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
                piece[piece_moves].cy++;
            }
            // If there is an enemy ahead, save it as a kill position
            else if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol] == enemy &&
                piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol < 8){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
                piece[piece_moves].cy++;
                die_x[x] = piece[piece_moves].bcol * 75;
                x++;
                die_y[y] = (piece[piece_moves].brow + 1) * 75;
                y++;
            }
        }else{
            // Reset the flag value
            k3 = 0;
        }
        if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol] == ally){
            // Do nothing if there is an ally
        }
        if(!k4){
            // Upper Movement
            // If there is no one, save it as valid position
            if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol] == 0 &&
                piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol < 8){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = piece[piece_moves].bcol * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
                piece[piece_moves].cy++;
            }
            // If there is an enemy ahead, save it as a kill position
            else if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol] == enemy &&
                piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol < 8){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = piece[piece_moves].bcol * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
                piece[piece_moves].cy++;
                die_x[x] = piece[piece_moves].bcol * 75;
                x++;
                die_y[y] = (piece[piece_moves].brow - 1) * 75;
                y++;
            }
        }else{
            // Reset the flag value
            k4 = 0;
        }
        if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 1] == ally){
            // Do nothing if there is an ally
        }
        if(!k5){
            // Upper Right Movement
            // If there is no one, save it as valid position
            if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 1] == 0 &&
                piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol + 1 < 8){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
                piece[piece_moves].cy++;
            }
            // If there is an enemy ahead, save it as a kill position
            else if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 1] == enemy &&
                piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol + 1 < 8){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
                piece[piece_moves].cy++;
                die_x[x] = (piece[piece_moves].bcol + 1) * 75;
                x++;
                die_y[y] = (piece[piece_moves].brow - 1) * 75;
                y++;
            }
        }else{
            // Reset the flag value
            k5 = 0;
        }
        if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 1] == ally){
            // Do nothing if there is an ally
        }
        if(!k6){
            // Upper Left Movement
            // If there is no one, save it as valid position
            if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 1] == 0 &&
                piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol - 1 >= 0){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
                piece[piece_moves].cy++;
            }
            // If there is an enemy ahead, save it as a kill position
            else if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 1] == enemy &&
                piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol - 1 >= 0){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
                piece[piece_moves].cy++;
                die_x[x] = (piece[piece_moves].bcol - 1) * 75;
                x++;
                die_y[y] = (piece[piece_moves].brow - 1) * 75;
                y++;
            }
        }else{
            // Reset the flag value
            k6 = 0;
        }
        if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 1] == ally){
            // Do nothing if there is an ally
        }
        if(!k7){
            // Lower Right Movement
            // If there is no one, save it as valid position
            if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 1] == 0 &&
                piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol + 1 < 8){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
                piece[piece_moves].cy++;
            }
            // If there is an enemy ahead, save it as a kill position
            else if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 1] == enemy &&
                piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol + 1 < 8){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
                piece[piece_moves].cy++;
                die_x[x] = (piece[piece_moves].bcol + 1) * 75;
                x++;
                die_y[y] = (piece[piece_moves].brow + 1) * 75;
                y++;
            }
        }else{
            // Reset the flag value
            k7 = 0;
        }
        if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 1] == ally){
            // Do nothing if there is an ally
        }
        if(!k8){
            // Lower Left Movement
            // If there is no one, save it as valid position
            if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 1] == 0 &&
                piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol - 1 >= 0){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
                piece[piece_moves].cy++;
            }
            // If there is an enemy ahead, save it as a kill position
            else if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 1] == enemy &&
                piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol - 1 >= 0){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
                piece[piece_moves].cy++;
                die_x[x] = (piece[piece_moves].bcol - 1) * 75;
                x++;
                die_y[y] = (piece[piece_moves].brow + 1) * 75;
                y++;
            }
        }else{
            // Reset the flag value
            k8 = 0;
        }
        if(!k11){
            // White King Castling on The Right Side
            if(piece[piece_moves].brow == 7 && piece[piece_moves].bcol == 4
                && piece[piece_moves].brow_prev == -1 && piece[piece_moves].bcol_prev == -1
                && board_types[piece[piece_moves].brow][piece[piece_moves].bcol + 3] == wr
                && piece[31].brow_prev == -1 && piece[31].bcol_prev == -1 && board[7][6] == 0 && board[7][5] == 0){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 2) * 75;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].cy++;
                game_prop.wrc_w_k = 1;
            }
        }else{
            // Reset the flag value
            k11 = 0;
        }
        if(!k9){
            // White King Castling on The Left Side
            if(piece[piece_moves].brow == 7 && piece[piece_moves].bcol == 4
                && piece[piece_moves].brow_prev == -1 && piece[piece_moves].bcol_prev == -1
                && board_types[piece[piece_moves].brow][piece[piece_moves].bcol - 4] == wr
                && piece[24].brow_prev == -1 && piece[24].bcol_prev == -1 && board[7][1] == 0 && board[7][2] == 0 && board[7][3] == 0){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 2) * 75;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].cy++;
                game_prop.wlc_w_k = 1;
            }
        }else{
            // Reset the flag value
            k9 = 0;
        }
        if(!k12){
            // Black King Castling on The Right Side
            if(piece[piece_moves].brow == 0 && piece[piece_moves].bcol == 4
                && piece[piece_moves].brow_prev == -1 && piece[piece_moves].bcol_prev == -1
                && board_types[piece[piece_moves].brow][piece[piece_moves].bcol + 3] == br
                && piece[7].brow_prev == -1 && piece[7].bcol_prev == -1 && board[0][6] == 0 && board[0][5] == 0){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 2) * 75;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].cy++;
                game_prop.brc_w_k = 1;
            }
        }else{
            // Reset the flag value
            k12 = 0;
        }
        if(!k10){
            // Black King Castling on The Left Side
            if(piece[piece_moves].brow == 0 && piece[piece_moves].bcol == 4
                && piece[piece_moves].brow_prev == -1 && piece[piece_moves].bcol_prev == -1
                && board_types[piece[piece_moves].brow][piece[piece_moves].bcol - 4] == br
                && piece[0].brow_prev == -1 && piece[0].bcol_prev == -1 && board[0][1] == 0 && board[0][2] == 0 && board[0][3] == 0){
                piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 2) * 75;
                piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
                piece[piece_moves].cx++;
                piece[piece_moves].cy++;
                game_prop.blc_w_k = 1;
            }
        }else{
            // Reset the flag value
            k10 = 0;
        }
    }
    // Checks the move of the king
    else{
        // Right Direction Movement
        // If there is Ally ahead, save it as cover position
        if(board[piece[piece_moves].brow][piece[piece_moves].bcol + 1] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol + 1) * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = piece[piece_moves].brow * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
        }
        // If there is no one, save it as valid position
        if(board[piece[piece_moves].brow][piece[piece_moves].bcol + 1] == 0 &&
            piece[piece_moves].brow < 8 && piece[piece_moves].bcol + 1 < 8){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[piece[piece_moves].brow][piece[piece_moves].bcol + 1] == enemy &&
            piece[piece_moves].brow < 8 && piece[piece_moves].bcol + 1 < 8){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cy++;
            die_x[x] = (piece[piece_moves].bcol + 1) * 75;
            x++;
            die_y[y] = piece[piece_moves].brow * 75;
            y++;
        }
        // Left Direction Movement
        // If there is Ally ahead, save it as cover position
        if(board[piece[piece_moves].brow][piece[piece_moves].bcol - 1] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol - 1) * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = piece[piece_moves].brow * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
        }
        // If there is no one, save it as valid position
        if(board[piece[piece_moves].brow][piece[piece_moves].bcol - 1] == 0 &&
            piece[piece_moves].brow < 8 && piece[piece_moves].bcol - 1 >= 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[piece[piece_moves].brow][piece[piece_moves].bcol - 1] == enemy &&
            piece[piece_moves].brow < 8 && piece[piece_moves].bcol - 1 >= 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cy++;
            die_x[x] = (piece[piece_moves].bcol - 1) * 75;
            x++;
            die_y[y] = piece[piece_moves].brow * 75;
            y++;
        }
        // Lower Movement
        // If there is Ally ahead, save it as cover position
        if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = piece[piece_moves].bcol * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow + 1) * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
        }
        // If there is no one, save it as valid position
        if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol] == 0 &&
            piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol < 8){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol] == enemy &&
            piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol < 8){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
            piece[piece_moves].cy++;
            die_x[x] = piece[piece_moves].bcol * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow + 1) * 75;
            y++;
        }
        // Upper Movement
        // If there is Ally ahead, save it as cover position
        if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = piece[piece_moves].bcol * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow - 1) * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
        }
        // If there is no one, save it as valid position
        if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol] == 0 &&
            piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol < 8){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = piece[piece_moves].bcol * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol] == enemy &&
            piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol < 8){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = piece[piece_moves].bcol * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
            piece[piece_moves].cy++;
            die_x[x] = piece[piece_moves].bcol * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow - 1) * 75;
            y++;
        }
        // Upper Right Movement
        // If there is Ally ahead, save it as cover position
        if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 1] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol + 1) * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow - 1) * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
        }
        // If there is no one, save it as valid position
        if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 1] == 0 &&
            piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol + 1 < 8){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol + 1] == enemy &&
            piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol + 1 < 8){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
            piece[piece_moves].cy++;
            die_x[x] = (piece[piece_moves].bcol + 1) * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow - 1) * 75;
            y++;
        }
        // Upper Left Movement
        // If there is Ally ahead, save it as cover position
        if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 1] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol - 1) * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow - 1) * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
        }
        // If there is no one, save it as valid position
        if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 1] == 0 &&
            piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol - 1 >= 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[piece[piece_moves].brow - 1][piece[piece_moves].bcol - 1] == enemy &&
            piece[piece_moves].brow - 1 >= 0 && piece[piece_moves].bcol - 1 >= 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow - 1) * 75;
            piece[piece_moves].cy++;
            die_x[x] = (piece[piece_moves].bcol - 1) * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow - 1) * 75;
            y++;
        }
        // Lower Right Movement
        // If there is Ally ahead, save it as cover position
        if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 1] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol + 1) * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow + 1) * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
        }
        // If there is no one, save it as valid position
        if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 1] == 0 &&
            piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol + 1 < 8){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol + 1] == enemy &&
            piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol + 1 < 8){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 1) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
            piece[piece_moves].cy++;
            die_x[x] = (piece[piece_moves].bcol + 1) * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow + 1) * 75;
            y++;
        }
        // Lower Left Movement
        // If there is Ally ahead, save it as cover position
        if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 1] == ally){
            piece[piece_moves].valid_cover_x[piece[piece_moves].cover_cx] = (piece[piece_moves].bcol - 1) * 75;
            piece[piece_moves].valid_cover_y[piece[piece_moves].cover_cy] = (piece[piece_moves].brow + 1) * 75;
            piece[piece_moves].cover_cx++;
            piece[piece_moves].cover_cy++;
        }
        // If there is no one, save it as valid position
        if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 1] == 0 &&
            piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol - 1 >= 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
            piece[piece_moves].cy++;
        }
        // If there is an enemy ahead, save it as a kill position
        else if(board[piece[piece_moves].brow + 1][piece[piece_moves].bcol - 1] == enemy &&
            piece[piece_moves].brow + 1 < 8 && piece[piece_moves].bcol - 1 >= 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 1) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow + 1) * 75;
            piece[piece_moves].cy++;
            die_x[x] = (piece[piece_moves].bcol - 1) * 75;
            x++;
            die_y[y] = (piece[piece_moves].brow + 1) * 75;
            y++;
        }
        // Right King Castling on The Right Side
        if(piece[piece_moves].brow == 7 && piece[piece_moves].bcol == 4
            && piece[piece_moves].brow_prev == -1 && piece[piece_moves].bcol_prev == -1
            && board_types[piece[piece_moves].brow][piece[piece_moves].bcol + 3] == wr
            && piece[31].brow_prev == -1 && piece[31].bcol_prev == -1 && board[7][6] == 0 && board[7][5] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 2) * 75;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].cy++;
            game_prop.wrc_w_k = 1;
        }
        // Right King Castling on The Left Side
        if(piece[piece_moves].brow == 7 && piece[piece_moves].bcol == 4
            && piece[piece_moves].brow_prev == -1 && piece[piece_moves].bcol_prev == -1
            && board_types[piece[piece_moves].brow][piece[piece_moves].bcol - 4] == wr
            && piece[24].brow_prev == -1 && piece[24].bcol_prev == -1 && board[7][1] == 0 && board[7][2] == 0 && board[7][3] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 2) * 75;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].cy++;
            game_prop.wlc_w_k = 1;
        }
        // Black King Castling on The Right Side
        if(piece[piece_moves].brow == 0 && piece[piece_moves].bcol == 4
            && piece[piece_moves].brow_prev == -1 && piece[piece_moves].bcol_prev == -1
            && board_types[piece[piece_moves].brow][piece[piece_moves].bcol + 3] == br
            && piece[7].brow_prev == -1 && piece[7].bcol_prev == -1 && board[0][6] == 0 && board[0][5] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol + 2) * 75;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].cy++;
            game_prop.brc_w_k = 1;
        }
        // Black King Castling on The Left Side
        if(piece[piece_moves].brow == 0 && piece[piece_moves].bcol == 4
            && piece[piece_moves].brow_prev == -1 && piece[piece_moves].bcol_prev == -1
            && board_types[piece[piece_moves].brow][piece[piece_moves].bcol - 4] == br
            && piece[0].brow_prev == -1 && piece[0].bcol_prev == -1 && board[0][1] == 0 && board[0][2] == 0 && board[0][3] == 0){
            piece[piece_moves].valid_moves_x[piece[piece_moves].cx] = (piece[piece_moves].bcol - 2) * 75;
            piece[piece_moves].valid_moves_y[piece[piece_moves].cy] = (piece[piece_moves].brow) * 75;
            piece[piece_moves].cx++;
            piece[piece_moves].cy++;
            game_prop.blc_w_k = 1;
        }
    }
}

//#-------------------------------------------------------------------------#

// Check the validity of the player's move
int check_valid(void){
    int i1, i2;
    for (i1 = 0, i2 = 0; i1 < piece[chosen_piece].cx, i2 < piece[chosen_piece].cy; i1++, i2++){
        // If the move is valid return one
        if (i2 == i1 && piece[chosen_piece].valid_moves_x[i1] == piece[chosen_piece].xtemp &&
        piece[chosen_piece].valid_moves_y[i2] == piece[chosen_piece].ytemp){
            return 1;
        }
    }
    // If the move is not valid return one
    return 0;
}

//#-------------------------------------------------------------------------#

// Check If the player's move is resulting in a dead piece
int check_die(void){
    int i, j;
    for (i = 0, j = 0; i < x, j < y; i++, j++){
        // If There is a dead piece return 1
        if (i == j && die_x[i] == piece[chosen_piece].xtemp && die_y[j] == piece[chosen_piece].ytemp){
            return 1;
        }
        // If there is en passant kill return 1 (White Pawn Kill Black Pawn)
        else if (board_types[piece[chosen_piece].brow + 1][piece[chosen_piece].bcol] == bp
                    && board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] == wp){
            return 1;
        }
        // If there is en passant kill return 1 (Black Pawn Kill White Pawn)
        else if (board_types[piece[chosen_piece].brow - 1][piece[chosen_piece].bcol] == wp
                    && board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] == bp){
            return 1;
        }
    }
    // If there is no dead pieces return 0
    return 0;
}

//#-------------------------------------------------------------------------#

// find the line of the enemy which attacks the king
void check_enemy_line(void){
    int l, p;
    // Positive Horizontal Line
    if(l1){
        for(p = (piece[game_prop.king_check].bcol + 1); p < 8; p++){
            // If the enemy on this line, equal flag to one
            if(((piece[game_prop.king_check].brow == piece[game_prop.T_piece].brow) && (p == piece[game_prop.T_piece].bcol))){
                enemy_line1 = 1;
                break;
            }
            // If The enemy not on this line, equal flag to zero
            else{
                enemy_line1 = 0;
            }
        }
    }
    // Negative Horizontal Line
    if(l2){
        for(p = (piece[game_prop.king_check].bcol - 1); p >= 0; p--){
            // If the enemy on this line, equal flag to one
            if(((piece[game_prop.king_check].brow == piece[game_prop.T_piece].brow) && (p == piece[game_prop.T_piece].bcol))){
                enemy_line2 = 1;
                break;
            }
            // If The enemy not on this line, equal flag to zero
            else{
                enemy_line2 = 0;
            }
        }
    }
    // Positive Vertical Line
    if(l3){
        for(p = (piece[game_prop.king_check].brow + 1); p < 8; p++){
            // If the enemy on this line, equal flag to one
            if(((piece[game_prop.T_piece].brow == p) && (piece[game_prop.king_check].bcol == piece[game_prop.T_piece].bcol))){
                enemy_line3 = 1;
                break;
            }
            // If The enemy not on this line, equal flag to zero
            else{
                enemy_line3 = 0;
            }
        }
    }
    // Negative Vertical Line
    if(l4){
        for(p = (piece[game_prop.king_check].brow - 1); p >= 0; p--){
            // If the enemy on this line, equal flag to one
            if(((piece[game_prop.T_piece].brow == p) && (piece[game_prop.king_check].bcol == piece[game_prop.T_piece].bcol))){
                enemy_line4 = 1;
                break;
            }
            // If The enemy not on this line, equal flag to zero
            else{
                enemy_line4 = 0;
            }
        }
    }
    // Upper Right Diagonal Line
    if(l5){
        for(p = piece[game_prop.king_check].bcol + 1, l = piece[game_prop.king_check].brow - 1 ; p < 8 && l >= 0; p++, l--){
            // If the enemy on this line, equal flag to one
            if(((piece[game_prop.T_piece].brow == l) && (p == piece[game_prop.T_piece].bcol))){
                enemy_line5 = 1;
                break;
            }
            // If The enemy not on this line, equal flag to zero
            else{
                enemy_line5 = 0;
            }
        }
    }
    // Upper Left Diagonal Line
    if(l6){
        for(p = piece[game_prop.king_check].bcol - 1, l = piece[game_prop.king_check].brow - 1; p >= 0 && l >= 0; p--, l--){
            // If the enemy on this line, equal flag to one
            if(((piece[game_prop.T_piece].brow == l) && (p == piece[game_prop.T_piece].bcol))){
                enemy_line6 = 1;
                break;
            }
            // If The enemy not on this line, equal flag to zero
            else{
                enemy_line6 = 0;
            }
        }
    }
    // Lower Right Diagonal Line
    if(l7){
        for(p = piece[game_prop.king_check].bcol + 1, l = piece[game_prop.king_check].brow + 1; p < 8 && l < 8; p++, l++){
            // If the enemy on this line, equal flag to one
            if(((piece[game_prop.T_piece].brow == l) && (p == piece[game_prop.T_piece].bcol))){
                enemy_line7 = 1;
                break;
            }
            // If The enemy not on this line, equal flag to zero
            else{
                enemy_line7 = 0;
            }
        }
    }
    // Lower Left Diagonal Line
    if(l8){
        for(p = piece[game_prop.king_check].bcol - 1,l = piece[game_prop.king_check].brow + 1;p >= 0 && l < 8; p--, l++){
            // If the enemy on this line, equal flag to one
            if(((piece[game_prop.T_piece].brow == l) && (p == piece[game_prop.T_piece].bcol))){
                enemy_line8 = 1;
                break;
            }
            // If The enemy not on this line, equal flag to zero
            else{
                enemy_line8 = 0;
            }
        }
    }
}

//#-------------------------------------------------------------------------#

// Find the intersection of the king's line and enemy and record the valid moves to stop the attack
void check_king_line(int ally, int enemy){
    // Call the function to find the line of the enemy which attacks the king
    check_enemy_line();
    int l, p;
    // Positive Horizontal Line
    if(l1 && enemy_line1){
        for(p = (piece[game_prop.king_check].bcol + 1); p < 8; p++){
            // If there is an ally on the line move, stop this loop
            if(board[piece[game_prop.king_check].brow][p] == ally){
                break;
            }
            // If there is an enemy, confirm this line and save the valid moves on this line
            // If there is no one, keep searching until there is no valid moves on this line
            else if(board[piece[game_prop.king_check].brow][p] == 0 || board[piece[game_prop.king_check].brow][p] == enemy){
                line_moves_x[line_x] = p * 75;
                line_x++;
                line_moves_y[line_y] = (piece[game_prop.king_check].brow) * 75;
                line_y++;
                if(board[piece[game_prop.king_check].brow][p] == enemy){
                    break;
                }
            }
        }
        // Reset the flags Value
        l1 = 0;
        enemy_line1 = 0;
    }
    // Negative Horizontal Line
    if(l2 && enemy_line2){
        for(p = (piece[game_prop.king_check].bcol - 1); p >= 0; p--){
            // If there is an ally on the line move, stop this loop
            if(board[piece[game_prop.king_check].brow][p] == ally){
                break;
            }
            // If there is an enemy, confirm this line and save the valid moves on this line
            // If there is no one, keep searching until there is no valid moves on this line
            else if(board[piece[game_prop.king_check].brow][p] == 0 || board[piece[game_prop.king_check].brow][p] == enemy){
                line_moves_x[line_x] = p * 75;
                line_x++;
                line_moves_y[line_y] = (piece[game_prop.king_check].brow) * 75;
                line_y++;
                if(board[piece[game_prop.king_check].brow][p] == enemy){
                    break;
                }
            }
        }
        // Reset the flags Value
        l2 = 0;
        enemy_line2 = 0;
    }
    // Positive Vertical Line
    if(l3 && enemy_line3){
        for(p = (piece[game_prop.king_check].brow + 1); p < 8; p++){
            // If there is an ally on the line move, stop this loop
            if(board[p][piece[game_prop.king_check].bcol] == ally){
                break;
            }
            // If there is an enemy, confirm this line and save the valid moves on this line
            // If there is no one, keep searching until there is no valid moves on this line
            else if(board[p][piece[game_prop.king_check].bcol] == 0 || board[p][piece[game_prop.king_check].bcol] == enemy){
                line_moves_x[line_x] = (piece[game_prop.king_check].bcol) * 75;
                line_x++;
                line_moves_y[line_y] = p * 75;
                line_y++;
                if(board[p][piece[game_prop.king_check].bcol] == enemy){
                    break;
                }
            }
        }
        // Reset the flags Value
        l3 = 0;
        enemy_line3 = 0;
    }
    // Negative Vertical Line
    if(l4 && enemy_line4){
        for(p = (piece[game_prop.king_check].brow - 1); p >= 0; p--){
            // If there is an ally on the line move, stop this loop
            if(board[p][piece[game_prop.king_check].bcol] == ally){
                break;
            }
            // If there is an enemy, confirm this line and save the valid moves on this line
            // If there is no one, keep searching until there is no valid moves on this line
            else if(board[p][piece[game_prop.king_check].bcol] == 0 || board[p][piece[game_prop.king_check].bcol] == enemy){
                line_moves_x[line_x] = piece[game_prop.king_check].bcol * 75;
                line_x++;
                line_moves_y[line_y] = p * 75;
                line_y++;
                if(board[p][piece[game_prop.king_check].bcol] == enemy){
                    break;
                }
            }
        }
        // Reset the flags Value
        l4 = 0;
        enemy_line4 = 0;
    }
    // Upper Right Diagonal Line
    if(l5 && enemy_line5){
        for(p = piece[game_prop.king_check].bcol + 1, l = piece[game_prop.king_check].brow - 1 ; p < 8 && l >= 0; p++, l--){
            // If there is an ally on the line move, stop this loop
            if(board[l][p] == ally){
                break;
            }
            // If there is an enemy, confirm this line and save the valid moves on this line
            // If there is no one, keep searching until there is no valid moves on this line
            else if(board[l][p] == 0 || board[l][p] == enemy){
                line_moves_x[line_x] = p * 75;
                line_x++;
                line_moves_y[line_y] = l * 75;
                line_y++;
                if(board[l][p] == enemy){
                    break;
                }
            }
        }
        // Reset the flags Value
        l5 = 0;
        enemy_line5 = 0;
    }
    // Upper Left Diagonal Line
    if(l6 && enemy_line6){
        for(p = piece[game_prop.king_check].bcol - 1, l = piece[game_prop.king_check].brow - 1; p >= 0 && l >= 0; p--, l--){
            // If there is an ally on the line move, stop this loop
            if(board[l][p] == ally){
                break;
            }
            // If there is an enemy, confirm this line and save the valid moves on this line
            // If there is no one, keep searching until there is no valid moves on this line
            else if(board[l][p] == 0 || board[l][p] == enemy){
                line_moves_x[line_x] = p * 75;
                line_x++;
                line_moves_y[line_y] = l * 75;
                line_y++;
                if(board[l][p] == enemy){
                    break;
                }
            }
        }
        // Reset the flags Value
        l6 = 0;
        enemy_line6 = 0;
    }
    // Lower Right Diagonal Line
    if(l7 && enemy_line7){
        for(p = piece[game_prop.king_check].bcol + 1, l = piece[game_prop.king_check].brow + 1; p < 8 && l < 8; p++, l++){
            // If there is an ally on the line move, stop this loop
            if(board[l][p] == ally){
                break;
            }
            // If there is an enemy, confirm this line and save the valid moves on this line
            // If there is no one, keep searching until there is no valid moves on this line
            else if(board[l][p] == 0 || board[l][p] == enemy){
                line_moves_x[line_x] = p * 75;
                line_x++;
                line_moves_y[line_y] = l * 75;
                line_y++;
                if(board[l][p] == enemy){
                    break;
                }
            }
        }
        // Reset the flags Value
        l7 = 0;
        enemy_line7 = 0;
    }
    // Lower Left Diagonal Line
    if(l8 && enemy_line8){
        for(p = piece[game_prop.king_check].bcol - 1,l = piece[game_prop.king_check].brow + 1;p >= 0 && l < 8; p--, l++){
            // If there is an ally on the line move, stop this loop
            if(board[l][p] == ally){
                break;
            }
            // If there is an enemy, confirm this line and save the valid moves on this line
            // If there is no one, keep searching until there is no valid moves on this line
            else if(board[l][p] == 0 || board[l][p] == enemy){
                line_moves_x[line_x] = p * 75;
                line_x++;
                line_moves_y[line_y] = l * 75;
                line_y++;
                if(board[l][p] == enemy){
                    break;
                }
            }
        }
        // Reset the flags Value
        l8 = 0;
        enemy_line8 = 0;
    }
}

// Find the position of the knight which attack the king, and record it
void king_knight_att(void){
    int j;
    for(j = 0; j < piece[game_prop.T_piece].cx; j++){
        // Lower Right Direction
        if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow) &&
        (piece[game_prop.T_piece].bcol == piece[game_prop.king_check].bcol + 1 &&
        piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow + 2)){
            line_moves_x[line_x] = (piece[game_prop.king_check].bcol + 1) * 75;
            line_x++;
            line_moves_y[line_y] = (piece[game_prop.king_check].brow + 2) * 75;
            line_y++;
        }
        // Upper Right Direction
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow) &&
        (piece[game_prop.T_piece].bcol == piece[game_prop.king_check].bcol + 1 &&
        piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow - 2)){
            line_moves_x[line_x] = (piece[game_prop.king_check].bcol + 1) * 75;
            line_x++;
            line_moves_y[line_y] = (piece[game_prop.king_check].brow - 2) * 75;
            line_y++;
        }
        // Lower Left Direction
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow) &&
        (piece[game_prop.T_piece].bcol == piece[game_prop.king_check].bcol - 1 &&
        piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow + 2)){
            line_moves_x[line_x] = (piece[game_prop.king_check].bcol - 1) * 75;
            line_x++;
            line_moves_y[line_y] = (piece[game_prop.king_check].brow + 2) * 75;
            line_y++;
        }
        // Upper Left Direction
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow) &&
        (piece[game_prop.T_piece].bcol == piece[game_prop.king_check].bcol - 1 &&
        piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow - 2)){
            line_moves_x[line_x] = (piece[game_prop.king_check].bcol - 1) * 75;
            line_x++;
            line_moves_y[line_y] = (piece[game_prop.king_check].brow - 2) * 75;
            line_y++;
        }
        // Right Lower Direction
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow) &&
        (piece[game_prop.T_piece].bcol == piece[game_prop.king_check].bcol + 2 &&
        piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow + 1)){
            line_moves_x[line_x] = (piece[game_prop.king_check].bcol + 2) * 75;
            line_x++;
            line_moves_y[line_y] = (piece[game_prop.king_check].brow + 1) * 75;
            line_y++;
        }
        // Right Upper Direction
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow) &&
        (piece[game_prop.T_piece].bcol == piece[game_prop.king_check].bcol + 2 &&
        piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow - 1)){
            line_moves_x[line_x] = (piece[game_prop.king_check].bcol + 2) * 75;
            line_x++;
            line_moves_y[line_y] = (piece[game_prop.king_check].brow - 1) * 75;
            line_y++;
        }
        // Left Lower Direction
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow) &&
        (piece[game_prop.T_piece].bcol == piece[game_prop.king_check].bcol - 2 &&
        piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow + 1)){
            line_moves_x[line_x] = (piece[game_prop.king_check].bcol - 2) * 75;
            line_x++;
            line_moves_y[line_y] = (piece[game_prop.king_check].brow + 1) * 75;
            line_y++;
        }
        // Left Upper Direction
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow) &&
        (piece[game_prop.T_piece].bcol == piece[game_prop.king_check].bcol - 2 &&
        piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow - 1)){
            line_moves_x[line_x] = (piece[game_prop.king_check].bcol - 2) * 75;
            line_x++;
            line_moves_y[line_y] = (piece[game_prop.king_check].brow - 1) * 75;
            line_y++;
        }
    }
}

//#-------------------------------------------------------------------------#

// Find the Line Which the threaten piece attacks the king from it
void check_line(void){
    int j;
    // If the king on this line, equal line flag to one
    for(j = 0; j < piece[game_prop.T_piece].cx; j++){
        // Positive Horizontal Line
        if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol + 1 &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow) ||
        ((piece[game_prop.king_check].brow == piece[game_prop.T_piece].brow) && (piece[game_prop.king_check].bcol + 1 == piece[game_prop.T_piece].bcol))){
            l1 = 1;
        }
        // Negative Horizontal Line
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol - 1 &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow) ||
        ((piece[game_prop.king_check].brow == piece[game_prop.T_piece].brow) && (piece[game_prop.king_check].bcol - 1 == piece[game_prop.T_piece].bcol))){
            l2 = 1;
        }
        // Positive Vertical Line
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow + 1) ||
        ((piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow + 1) && (piece[game_prop.king_check].bcol == piece[game_prop.T_piece].bcol))){
            l3 = 1;
        }
        // Negative Vertical Line
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow - 1) ||
        ((piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow - 1) && (piece[game_prop.king_check].bcol == piece[game_prop.T_piece].bcol))){
            l4 = 1;
        }
        // Upper Right Diagonal Line
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol + 1 &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow - 1) ||
        ((piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow - 1) && (piece[game_prop.king_check].bcol + 1 == piece[game_prop.T_piece].bcol))){
            l5 = 1;
        }
        // Upper Left Diagonal Line
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol - 1 &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow - 1) ||
        ((piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow - 1) && (piece[game_prop.king_check].bcol - 1 == piece[game_prop.T_piece].bcol))){
            l6 = 1;
        }
        // Lower Right Diagonal Line
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol + 1 &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow + 1) ||
        ((piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow + 1) && (piece[game_prop.king_check].bcol + 1 == piece[game_prop.T_piece].bcol))){
            l7 = 1;
        }
        // Lower Left Diagonal Line
        else if(((piece[game_prop.T_piece].valid_moves_x[j]) / 75 == piece[game_prop.king_check].bcol - 1 &&
        (piece[game_prop.T_piece].valid_moves_y[j] / 75) == piece[game_prop.king_check].brow + 1) ||
        ((piece[game_prop.T_piece].brow == piece[game_prop.king_check].brow + 1) && (piece[game_prop.king_check].bcol - 1 == piece[game_prop.T_piece].bcol))){
            l8 = 1;
        }
    }
}

//#-------------------------------------------------------------------------#

// Compare piece's valid moves to the valid moves on king's line to cover it
void covered_matching(void){
    for(int o = 0; o < line_x; o++){
        for(int u = 0; u < piece[chosen_piece].cx; u++){
            // If there is equal values, save it
            if((piece[chosen_piece].valid_moves_x[u] == line_moves_x[o]) &&
            (piece[chosen_piece].valid_moves_y[u] == line_moves_y[o])){
                game_prop.covered = 1;
                valid_line_x[shield_x] = piece[chosen_piece].valid_moves_x[u];
                valid_line_y[shield_y] = piece[chosen_piece].valid_moves_y[u];
                shield_x++;
                shield_y++;
            }
        }
    }
    if(game_prop.covered == 1){
        // When there is equal values, record it to the piece's valid moves
        for(int q = 0; q < shield_x; q++){
            piece[chosen_piece].valid_moves_x[q] = valid_line_x[q];
            piece[chosen_piece].valid_moves_y[q] = valid_line_y[q];
        }
        game_prop.covered = 0;
    }
    // Modify array size of valid moves
    piece[chosen_piece].cx = shield_x;
    piece[chosen_piece].cy = shield_y;
}

//#-------------------------------------------------------------------------#

// Check if there is a piece which cover the king's piece from an attack
// So it stops its movement if it is exposing the king to a danger
void check_cover(void){
    int ally, enemy, set, end, temp_bcol, temp_brow, temp_b1, temp_b2, temp_T;
    if(chosen_piece == 4 || chosen_piece == 28){
        // Do nothing if the chosen piece is a king
    }else{
        // Determine the chosen piece and its enemies and allies
        if(16 <= chosen_piece && chosen_piece < 32){
            ally = 1;
            enemy = -1;
            game_prop.king_check = 28;
            set = 0;
            end = 16;
        }else if(0 <= chosen_piece && chosen_piece < 16){
            ally = -1;
            enemy = 1;
            game_prop.king_check = 4;
            set = 16;
            end = 32;
        }
        // Save the threaten piece to a temporary variable
        if(game_prop.w_check || game_prop.b_check){
            temp_T = game_prop.T_piece;
        }

        // Make the chosen piece invisible
        // To determine the line from which the threaten is coming
        temp_bcol = piece[chosen_piece].bcol;
        temp_brow = piece[chosen_piece].brow;
        temp_b1 = board[piece[chosen_piece].brow][piece[chosen_piece].bcol];
        temp_b2 = board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol];
        board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
        board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
        for(int c = set; c < end; c++){
            if (c == 4 || c == 28 || piece[c].l_o_d == 0 || c == temp_T){
                // If it is the king or the piece which checks the king or it's a dead piece, continue
                continue;
            }
            if(check_king_att(c)){
                if(piece[c].type == 'n'){
                    king_knight_att();
                }else{
                    // Get the line on which the king lie and it will be checked there
                    check_line();
                    // Get the line on which the enemy lie and check the king's piece from it
                    check_king_line(ally, enemy);
                }
                // Compare the valid moves and the covered moves
                covered_matching();
                // A flag to determine if there is a check and a piece covers another check
                game_prop.cov_shield = 1;
            }
        }
        // Reset the values of the chosen piece on the board
        board[temp_brow][temp_bcol] = temp_b1;
        board_types[temp_brow][temp_bcol] = temp_b2;
    }
}

//#-------------------------------------------------------------------------#

// Check if there is a piece which masks (shield) the enemy attack
void check_shield(void){
    int d = 0, j, r, p, l, ally, enemy;

    if(chosen_piece == 4 || chosen_piece == 28){
        // If the chosen piece is a king, check its valid moves
        get_valid_moves(chosen_piece);
    }
    // If other piece is chosen get its valid moves to mask the enemy attack
    else{
        // Determine the chosen piece and its enemies and allies
        if(16 <= chosen_piece && chosen_piece < 32){
            ally = 1;
            enemy = -1;
            game_prop.king_check = 28;
        }else if(0 <= chosen_piece && chosen_piece < 16){
            ally = -1;
            enemy = 1;
            game_prop.king_check = 4;
        }
        // Get the valid moves of the chosen piece and the threaten piece
        get_valid_moves(chosen_piece);
        get_valid_moves(game_prop.T_piece);
        if(piece[game_prop.T_piece].type == 'n'){
            king_knight_att();
        }else{
            // Get the line on which the king lie and it will be checked there
            check_line();
            // Get the line on which the enemy lie and check the king's piece from it
            check_king_line(ally, enemy);
        }

        // Get the intersection between the valid moves of the threaten piece and the chosen one
        for(j = 0; j < line_x; j++){
            for(r = 0; r < piece[chosen_piece].cx; r++){
                // If there is an equal moves, save it
                if((piece[chosen_piece].valid_moves_x[r] == line_moves_x[j]) &&
                (piece[chosen_piece].valid_moves_y[r] == line_moves_y[j])){
                    game_prop.cover = 1;
                    valid_line_x[shield_x] = piece[chosen_piece].valid_moves_x[r];
                    valid_line_y[shield_y] = piece[chosen_piece].valid_moves_y[r];
                    shield_x++;
                    shield_y++;
                }
            }
        }
        // When there is equal values, record it to the piece's valid moves
        if(game_prop.cover == 1){
            for(int q = 0; q < shield_x; q++){
                piece[chosen_piece].valid_moves_x[q] = valid_line_x[q];
                piece[chosen_piece].valid_moves_y[q] = valid_line_y[q];
            }
        }
        // Modify array size of valid moves
        piece[chosen_piece].cx = shield_x;
        piece[chosen_piece].cy = shield_y;
    }
}

//#-------------------------------------------------------------------------#

// Check if the king has no moves and there isn't any piece to cover the it and it is checked
// Then confirm the winner of the game
void check_mate(int chosen){
    int d = 0, j, r, p, l, ally, enemy;

    // Determine the chosen piece and its enemies and allies
    if(chosen == 28){
        ally = 1;
        enemy = -1;
        game_prop.king_check = 28;
    }else if(chosen == 4){
        ally = -1;
        enemy = 1;
        game_prop.king_check = 4;
    }
    // Get the valid moves of the chosen piece
    get_valid_moves(game_prop.T_piece);
    if(piece[game_prop.T_piece].type == 'n'){
        king_knight_att();
    }else{
        // Get the line on which the king lie and it will be checked there
        check_line();
        // Get the line on which the enemy lie and check the king's piece from it
        check_king_line(ally, enemy);
    }

    if (chosen == 4){
        // When black king is the checked, check all moves which can be done
        for(int g = 0; g < 16; g++){
            if(g == 4 || piece[g].l_o_d == 0){
                // If the piece is a black king or dead, continue the loop
                continue;
            }
            // Get the valid moves of the piece
            get_valid_moves(g);
            for(j = 0; j < line_x; j++){
                for(r = 0; r < piece[g].cx; r++){
                    // Searching for a move to cover the king's piece
                    if((piece[g].valid_moves_x[r] == line_moves_x[j]) &&
                    (piece[g].valid_moves_y[r] == line_moves_y[j])){
                        game_prop.cover = 1;
                        chosen_piece = g;
                        game_prop.cov_shield = 0;
                        // Check if there is a cover for the king or the king is checked
                        check_cover();
                        if(game_prop.cov_shield == 1){
                            // If a piece covers a check
                            // and there is another check, equal cover flag to zero
                            game_prop.cover = 0;
                        }
                    }
                }
            }
        }
    }else if(chosen == 28){
        // When white king is the checked, check all moves which can be done
        for(int g = 16; g < 32; g++){
            if(g == 28 || piece[g].l_o_d == 0){
                // If the piece is a white king or dead, continue the loop
                continue;
            }
            // Get the valid moves of the piece
            get_valid_moves(g);
            for(j = 0; j < line_x; j++){
                for(r = 0; r < piece[g].cx; r++){
                    // Searching for a move to cover the king's piece
                    if((piece[g].valid_moves_x[r] == line_moves_x[j]) &&
                    (piece[g].valid_moves_y[r] == line_moves_y[j])){
                        game_prop.cover = 1;
                        chosen_piece = g;
                        game_prop.cov_shield = 0;
                        // Check if there is a cover for the king or the king is checked
                        check_cover();
                        if(game_prop.cov_shield == 1){
                            // If a piece covers a check
                            // and there is another check, equal cover flag to zero
                            game_prop.cover = 0;
                        }
                    }
                }
            }
        }
    }
}

//#-------------------------------------------------------------------------#

// Get the valid moves for the chosen piece
void get_valid_moves(int piece_moves){
    int ally, enemy, p, l, i, j;

    // Determine the chosen piece and its enemies and allies
    if(16 <= piece_moves && piece_moves < 32 && piece_moves == chosen_piece){
        ally = 1;
        enemy = -1;
        s = 0;
        e = 16;
    }else if(16 <= piece_moves && piece_moves < 32 && piece_moves != chosen_piece){
        ally = 1;
        enemy = -1;
    }else if(0 <= piece_moves && piece_moves < 16 && piece_moves == chosen_piece){
        ally = -1;
        enemy = 1;
        s = 16;
        e = 32;
    }else if(0 <= piece_moves && piece_moves < 16 && piece_moves != chosen_piece){
        ally = -1;
        enemy = 1;
    }

    // Reset the values of the dead pieces
    if(piece[piece_moves].l_o_d == 0){
        for(j = 0; j < 63; j++){
            piece[piece_moves].valid_moves_x[j] = -10;
            piece[piece_moves].valid_moves_y[j] = -10;
            piece[piece_moves].valid_cover_x[j] = -10;
            piece[piece_moves].valid_cover_y[j] = -10;
        }
        piece[piece_moves].cx = 0;
        piece[piece_moves].cy = 0;
        piece[piece_moves].cover_cx = 0;
        piece[piece_moves].cover_cy = 0;
    }else{
        // Determine the type of chosen piece to call its function
        switch(piece[piece_moves].type){
            case 'r':
                // Clear the counter of the piece
                piece[piece_moves].cover_cx = 0;
                piece[piece_moves].cover_cy = 0;
                piece[piece_moves].cy = 0;
                piece[piece_moves].cx = 0;
                // Calling the function
                rook(piece_moves, ally, enemy, p, l);
                break;
            case 'n':
                // Clear the counter of the piece
                piece[piece_moves].cover_cx = 0;
                piece[piece_moves].cover_cy = 0;
                piece[piece_moves].cy = 0;
                piece[piece_moves].cx = 0;
                // Calling the function
                knight(piece_moves, ally, enemy, p, l);
                break;
            case 'b':
                // Clear the counter of the piece
                piece[piece_moves].cover_cx = 0;
                piece[piece_moves].cover_cy = 0;
                piece[piece_moves].cy = 0;
                piece[piece_moves].cx = 0;
                // Calling the function
                bishop(piece_moves, ally, enemy, p, l);
                break;
            case 'q':
                // Clear the counter of the piece
                piece[piece_moves].cover_cx = 0;
                piece[piece_moves].cover_cy = 0;
                piece[piece_moves].cy = 0;
                piece[piece_moves].cx = 0;
                // Calling the function
                queen(piece_moves, ally, enemy, p, l);
                break;
            case 'k':
                // Clear the counter of the piece
                piece[piece_moves].cover_cx = 0;
                piece[piece_moves].cover_cy = 0;
                piece[piece_moves].cy = 0;
                piece[piece_moves].cx = 0;
                // Calling the function
                king(piece_moves, ally, enemy, p, l);
                break;
            case 'p':
                // Clear the counter of the piece
                piece[piece_moves].cover_cx = 0;
                piece[piece_moves].cover_cy = 0;
                piece[piece_moves].cy = 0;
                piece[piece_moves].cx = 0;
                // White Pawn
                if(16 <= piece_moves && piece_moves < 32){
                    // Calling the function
                    wsol(piece_moves, ally, enemy, p, l);
                }
                // Black Pawn
                else if(0 <= piece_moves && piece_moves < 16){
                    // Calling the function
                    bsol(piece_moves, ally, enemy, p, l);
                }
            break;
        }
    }
}

//#-------------------------------------------------------------------------#

// Check if the king is under attack from the last moved piece or not
int check_king_att(int a){
    // Get the valid moves for the suspected piece
    get_valid_moves(a);
    // Determine the threaten piece and the king which it attacks
    if(a < 32 && a >= 16){
        P_king = 4;
    }else if(a < 16 && a >= 0){
        P_king = 28;
    }
    // Get the intersected moves between the threaten piece and the king which it attack
    // and if there is a intersection the king will be checked
    for(int j = 0; j < piece[a].cx; j++){
        if(piece[a].valid_moves_x[j] / 75 == piece[P_king].bcol &&
        piece[a].valid_moves_y[j] / 75 == piece[P_king].brow){
            if(a == chosen_piece){
                // Determine which king is checked
                if(P_king == 4){
                    game_prop.b_check = 1;
                }else if(P_king == 28){
                    game_prop.w_check = 1;
                }
                game_prop.check = 1;
            }
            game_prop.T_piece = a;
            return (1);
        }
    }
    return (0);
}

//#-------------------------------------------------------------------------#

void dead_pos(void){
    // Determine how many pieces are dead
    for(int h = 0; h < 32; h++){
        // Exclude knight, bishops and kings
        if(piece[h].type == 'n' || piece[h].type == 'b' || piece[h].type == 'k'){
            continue;
        }
        if(piece[h].l_o_d == 0){
            dp = 1;
        }else{
            dp = 0;
            return;
        }
    }
    // The remaining pieces are a black knight and a black king vs a white king
    if(dp == 1 &&
    ((piece[6].l_o_d == 1 && !(piece[1].l_o_d == 1)) || (piece[1].l_o_d == 1 && !(piece[6].l_o_d == 1))) && piece[2].l_o_d == 0 &&
    piece[5].l_o_d == 0 && piece[30].l_o_d == 0 && piece[25].l_o_d == 0 && piece[26].l_o_d == 0 && piece[29].l_o_d == 0){
        game_prop.d_pos = 1;
    }
    // The remaining pieces are a white knight and a white king vs a black king
    else if (dp == 1 &&
    ((piece[25].l_o_d == 1 && !(piece[30].l_o_d == 1)) || (piece[30].l_o_d == 1 && !(piece[25].l_o_d == 1))) && piece[2].l_o_d == 0 &&
    piece[5].l_o_d == 0 && piece[6].l_o_d == 0 && piece[1].l_o_d == 0 && piece[26].l_o_d == 0 && piece[29].l_o_d == 0){
        game_prop.d_pos = 1;
    }
    // The remaining pieces are a black bishop and a black king vs a white king
    else if (dp == 1 &&
    ((piece[2].l_o_d == 1 && !(piece[5].l_o_d == 1)) || (piece[5].l_o_d == 1 && !(piece[2].l_o_d == 1))) && piece[30].l_o_d == 0 &&
    piece[25].l_o_d == 0 && piece[6].l_o_d == 0 && piece[1].l_o_d == 0 && piece[26].l_o_d == 0 && piece[29].l_o_d == 0){
        game_prop.d_pos = 1;
    }
    // The remaining pieces are a white bishop and a white king vs a black king
    else if (dp == 1 &&
    ((piece[26].l_o_d == 1 && !(piece[29].l_o_d == 1)) || (piece[29].l_o_d == 1 && !(piece[26].l_o_d == 1))) && piece[30].l_o_d == 0 &&
    piece[25].l_o_d == 0 && piece[6].l_o_d == 0 && piece[1].l_o_d == 0 && piece[2].l_o_d == 0 && piece[5].l_o_d == 0){
        game_prop.d_pos = 1;
    }
    // The remaining pieces are a black bishop and a black king vs a white bishop and a white king
    // In addition to the bishops are on the same color
    else if (dp == 1 &&
    (piece[2].l_o_d == 1 && piece[29].l_o_d == 1 && piece[30].l_o_d == 0 && piece[25].l_o_d == 0 && piece[6].l_o_d == 0 &&
    piece[1].l_o_d == 0 && piece[26].l_o_d == 0 && piece[5].l_o_d == 0) ||
    (piece[26].l_o_d == 1 && piece[5].l_o_d == 1 && piece[30].l_o_d == 0 && piece[25].l_o_d == 0 && piece[6].l_o_d == 0 &&
    piece[1].l_o_d == 0 && piece[2].l_o_d == 0 && piece[29].l_o_d == 0)){
        game_prop.d_pos = 1;
    }
    // If there is a king vs king with a pawn which will be promoted, consider this case
    else if(dp == 1 && (game_prop.bpb == 1 || game_prop.bpn == 1 || game_prop.wpn == 1 || game_prop.wpn == 1) &&
    piece[4].l_o_d == 1 && piece[28].l_o_d == 1 && piece[30].l_o_d == 0 && piece[25].l_o_d == 0 && piece[6].l_o_d == 0 &&
    piece[1].l_o_d == 0 && piece[26].l_o_d == 0 && piece[5].l_o_d == 0 && piece[2].l_o_d == 0 && piece[29].l_o_d == 0){
        game_prop.d_pos = 1;
    }
    // The remaining pieces are a black king vs a white king
    else if (dp == 1 &&
    piece[4].l_o_d == 1 && piece[28].l_o_d == 1 && piece[30].l_o_d == 0 && piece[25].l_o_d == 0 && piece[6].l_o_d == 0 &&
    piece[1].l_o_d == 0 && piece[26].l_o_d == 0 && piece[5].l_o_d == 0 && piece[2].l_o_d == 0 && piece[29].l_o_d == 0){
        game_prop.d_pos = 1;
    }
}

//#-------------------------------------------------------------------------#

// If the king's piece is not checked but all valid moves for it is attacked
// Game ends in a draw if there is no piece can move and the king isn't checked
void stalemate(){
    int temp_chosen2;
    // save the chosen piece to another variable
    temp_chosen2 = chosen_piece;
    // Determine which player is playing
    if(game_prop.w_o_b == 1){
        for (int g = 0; g < 16; g++){
            if (piece[g].l_o_d == 0){
                // If the piece is dead, continue
                continue;
            }
            chosen_piece = g;
            // Get the valid moves for the chosen piece
            get_valid_moves(chosen_piece);
            // Check if the king is covered from an attack
            check_cover();
            if(piece[g].cx == 0){
                st = 1;
            }else{
                st = 0;
                chosen_piece = temp_chosen2;
                return;
            }
        }
    }else if(game_prop.w_o_b == 0){
        for (int g = 16; g < 32; g++){
            if (piece[g].l_o_d == 0){
                // If the piece is dead, continue
                continue;
            }
            chosen_piece = g;
            // Get the valid moves for the chosen piece
            get_valid_moves(chosen_piece);
            // Check if the king is covered from an attack
            check_cover();
            if(piece[g].cx == 0){
                st = 1;
            }else{
                st = 0;
                chosen_piece = temp_chosen2;
                return;
            }
        }
    }
    // Check if the game ends in a draw by stalemate
    if(st == 1 && game_prop.w_check == 0 && game_prop.b_check == 0){
        game_prop.stale = 1;
    }
    chosen_piece = temp_chosen2;
}

//#-------------------------------------------------------------------------#

// Update the board and save the white move from the player
void white_move(void){
    // Record the move to the board which has white and black pieces
    // Update the (x, y) coordinates for the piece
    // Update the row and column number for the piece on the board
    piece[chosen_piece].brow = piece[chosen_piece].y / 75;
    piece[chosen_piece].bcol = piece[chosen_piece].x / 75;
    board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 1;
    board[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
    if(!(chosen_piece == 28)){
        // Check if this move result in a check
        check_king_att(chosen_piece);
    }
    game_prop.prev++;
    game_prop.do_count = 0;
    if(game_prop.prev > 8){
        // Check if the game end in a draw by stalemate
        stalemate();
    }
    // Record the move to use it for undo and redo features
    if(game_prop.b_check == 1){
        game_prop.comp_mate[game_prop.prev] = 5;
        game_prop.comp_mate[game_prop.prev + 1] = -5;
    }else if(game_prop.stale == 1){
        game_prop.comp_mate[game_prop.prev] = 7;
    }else if(!(game_prop.comp_mate[game_prop.prev] == -4)){
        game_prop.comp_mate[game_prop.prev] = 0;
        if(!(game_prop.comp_mate[game_prop.prev] == 4)){
            game_prop.comp_mate[game_prop.prev + 1] = 0;
        }
    }
    if(piece[chosen_piece].temp_brow_prev == -1){
        game_prop.first_move[game_prop.prev] = -1;
    }else{
        game_prop.first_move[game_prop.prev] = 0;
    }
    game_prop.do_moves_x[game_prop.prev] = piece[chosen_piece].x_prev;
    game_prop.do_moves_y[game_prop.prev] = piece[chosen_piece].y_prev;
    game_prop.prev_bcol[game_prop.prev] = piece[chosen_piece].bcol_prev;
    game_prop.prev_brow[game_prop.prev] = piece[chosen_piece].brow_prev;
    game_prop.chosen_undo[game_prop.prev] = chosen_piece;
    game_prop.redo_moves_x[game_prop.prev] = piece[chosen_piece].x;
    game_prop.redo_moves_y[game_prop.prev] = piece[chosen_piece].y;
    game_prop.next_bcol[game_prop.prev] = piece[chosen_piece].bcol;
    game_prop.next_brow[game_prop.prev] = piece[chosen_piece].brow;
    game_prop.chosen_redo[game_prop.prev] = chosen_piece;
    game_prop.compare[game_prop.prev] = 0;

    // Update the board which has the type of pieces
    switch (piece[chosen_piece].type){
        // A rook is moved
        case 'r':
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = wr;
            board_types[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
            break;
        // A knight is moved
        case 'n':
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = wn;
            board_types[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
            break;
        // A bishop is moved
        case 'b':
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = wb;
            board_types[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
            break;
        // A queen is moved
        case 'q':
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = wq;
            board_types[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
            break;
        // A king is moved
        case 'k':
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = wk;
            board_types[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
            break;
        // A pawn is moved
        case 'p':
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = wp;
            board_types[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
            break;
    }
}
//-----------------------------------------------//
// Update the board and save the white move from the player
void black_move(void){
    // Record the move to the board which has white and black pieces
    // Update the (x, y) coordinates for the piece
    // Update the row and column number for the piece on the board
    piece[chosen_piece].brow = piece[chosen_piece].y / 75;
    piece[chosen_piece].bcol = piece[chosen_piece].x / 75;
    board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = -1;
    board[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
    if(!(chosen_piece == 4)){
        // Check if this move result in a check
        check_king_att(chosen_piece);
    }
    game_prop.prev++;
    game_prop.do_count = 0;
    if(game_prop.prev > 8){
        // Check if the game end in a draw by stalemate
        stalemate();
    }
    // Record the move to use it for undo and redo features
    if(game_prop.w_check == 1){
        game_prop.comp_mate[game_prop.prev] = 4;
        game_prop.comp_mate[game_prop.prev + 1] = -4;
    }else if(game_prop.stale == 1){
        game_prop.comp_mate[game_prop.prev] = 7;
    }else if(!(game_prop.comp_mate[game_prop.prev] == -5)){
        game_prop.comp_mate[game_prop.prev] = 0;
        if(!(game_prop.comp_mate[game_prop.prev] == 5)){
            game_prop.comp_mate[game_prop.prev + 1] = 0;
        }
    }
    if(piece[chosen_piece].temp_brow_prev == -1){
        game_prop.first_move[game_prop.prev] = -1;
    }else{
        game_prop.first_move[game_prop.prev] = 0;
    }
    game_prop.do_moves_x[game_prop.prev] = piece[chosen_piece].x_prev;
    game_prop.do_moves_y[game_prop.prev] = piece[chosen_piece].y_prev;
    game_prop.prev_bcol[game_prop.prev] = piece[chosen_piece].bcol_prev;
    game_prop.prev_brow[game_prop.prev] = piece[chosen_piece].brow_prev;
    game_prop.chosen_undo[game_prop.prev] = chosen_piece;
    game_prop.redo_moves_x[game_prop.prev] = piece[chosen_piece].x;
    game_prop.redo_moves_y[game_prop.prev] = piece[chosen_piece].y;
    game_prop.next_bcol[game_prop.prev] = piece[chosen_piece].bcol;
    game_prop.next_brow[game_prop.prev] = piece[chosen_piece].brow;
    game_prop.chosen_redo[game_prop.prev] = chosen_piece;
    game_prop.compare[game_prop.prev] = 0;

    // Update the board which has the type of pieces
    switch (piece[chosen_piece].type){
        // A rook is moved
        case 'r':
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = br;
            board_types[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
            break;
        // A knight is moved
        case 'n':
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = bn;
            board_types[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
            break;
        // A bishop is moved
        case 'b':
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = bb;
            board_types[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
            break;
        // A queen is moved
        case 'q':
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = bq;
            board_types[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
            break;
        // A king is moved
        case 'k':
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = bk;
            board_types[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
            break;
        case 'p':
            // A pawn is moved
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = bp;
            board_types[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev] = 0;
            break;
    }
}

//#-------------------------------------------------------------------------#
// White castling for the king on the right side
void white_right_castling(void){
    // Update the (x, y) coordinates for the piece
    // Update the row and column number for the piece on the board
    // Record the previous (x, y) and row and column number
    piece[31].x_prev = piece[31].x;
    piece[31].y_prev = piece[31].y;
    piece[31].brow_prev = piece[31].y / 75;
    piece[31].bcol_prev = piece[31].x / 75;
    piece[31].x = (piece[31].bcol - 2) * 75;
    piece[31].y = (piece[31].brow) * 75;
    piece[31].brow = piece[31].y / 75;
    piece[31].bcol = piece[31].x / 75;
    // Record the move to the board which has white and black pieces
    board[piece[31].brow][piece[31].bcol] = 1;
    board[piece[31].brow_prev][piece[31].bcol_prev] = 0;
    // Update the board which has the type of pieces
    board_types[piece[31].brow][piece[31].bcol] = wr;
    board_types[piece[31].brow_prev][piece[31].bcol_prev] = 0;
    game_prop.prev++;
    // Record the move to use it for undo and redo features
    game_prop.do_moves_x[game_prop.prev] = piece[31].x_prev;
    game_prop.do_moves_y[game_prop.prev] = piece[31].y_prev;
    game_prop.prev_bcol[game_prop.prev] = piece[31].bcol_prev;
    game_prop.prev_brow[game_prop.prev] = piece[31].brow_prev;
    game_prop.chosen_undo[game_prop.prev] = 31;
    game_prop.redo_moves_x[game_prop.prev] = piece[31].x;
    game_prop.redo_moves_y[game_prop.prev] = piece[31].y;
    game_prop.next_bcol[game_prop.prev] = piece[31].bcol;
    game_prop.next_brow[game_prop.prev] = piece[31].brow;
    game_prop.compare[game_prop.prev] = 3;
    game_prop.chosen_redo[game_prop.prev] = 31;
    game_prop.wrc_w_k = 0;
}
//-----------------------------------------------//
// White castling for the king on the left side
void white_left_castling(void){
    // Update the (x, y) coordinates for the piece
    // Update the row and column number for the piece on the board
    // Record the previous (x, y) and row and column number
    piece[24].x_prev = piece[24].x;
    piece[24].y_prev = piece[24].y;
    piece[24].brow_prev = piece[24].y / 75;
    piece[24].bcol_prev = piece[24].x / 75;
    piece[24].x = (piece[24].bcol + 3) * 75;
    piece[24].y = (piece[24].brow) * 75;
    piece[24].brow = piece[24].y / 75;
    piece[24].bcol = piece[24].x / 75;
    // Record the move to the board which has white and black pieces
    board[piece[24].brow][piece[24].bcol] = 1;
    board[piece[24].brow_prev][piece[24].bcol_prev] = 0;
    // Update the board which has the type of pieces
    board_types[piece[24].brow][piece[24].bcol] = wr;
    board_types[piece[24].brow_prev][piece[24].bcol_prev] = 0;
    game_prop.prev++;
    // Record the move to use it for undo and redo features
    game_prop.do_moves_x[game_prop.prev] = piece[24].x_prev;
    game_prop.do_moves_y[game_prop.prev] = piece[24].y_prev;
    game_prop.prev_bcol[game_prop.prev] = piece[24].bcol_prev;
    game_prop.prev_brow[game_prop.prev] = piece[24].brow_prev;
    game_prop.chosen_undo[game_prop.prev] = 24;
    game_prop.redo_moves_x[game_prop.prev] = piece[24].x;
    game_prop.redo_moves_y[game_prop.prev] = piece[24].y;
    game_prop.next_bcol[game_prop.prev] = piece[24].bcol;
    game_prop.next_brow[game_prop.prev] = piece[24].brow;
    game_prop.compare[game_prop.prev] = 3;
    game_prop.chosen_redo[game_prop.prev] = 24;
    game_prop.wlc_w_k = 0;
}

//#-------------------------------------------------------------------------#

// Black castling for the king on the right side
void black_right_castling(void){
    // Update the (x, y) coordinates for the piece
    // Update the row and column number for the piece on the board
    // Record the previous (x, y) and row and column number
    piece[7].x_prev = piece[7].x;
    piece[7].y_prev = piece[7].y;
    piece[7].brow_prev = piece[7].y / 75;
    piece[7].bcol_prev = piece[7].x / 75;
    piece[7].x = (piece[7].bcol - 2) * 75;
    piece[7].y = (piece[7].brow) * 75;
    piece[7].brow = piece[7].y / 75;
    piece[7].bcol = piece[7].x / 75;
    // Record the move to the board which has white and black pieces
    board[piece[7].brow][piece[7].bcol] = -1;
    board[piece[7].brow_prev][piece[7].bcol_prev] = 0;
    // Update the board which has the type of pieces
    board_types[piece[7].brow][piece[7].bcol] = br;
    board_types[piece[7].brow_prev][piece[7].bcol_prev] = 0;
    game_prop.prev++;
    // Record the move to use it for undo and redo features
    game_prop.do_moves_x[game_prop.prev] = piece[7].x_prev;
    game_prop.do_moves_y[game_prop.prev] = piece[7].y_prev;
    game_prop.prev_bcol[game_prop.prev] = piece[7].bcol_prev;
    game_prop.prev_brow[game_prop.prev] = piece[7].brow_prev;
    game_prop.chosen_undo[game_prop.prev] = 7;
    game_prop.redo_moves_x[game_prop.prev] = piece[7].x;
    game_prop.redo_moves_y[game_prop.prev] = piece[7].y;
    game_prop.next_bcol[game_prop.prev] = piece[7].bcol;
    game_prop.next_brow[game_prop.prev] = piece[7].brow;
    game_prop.compare[game_prop.prev] = 3;
    game_prop.chosen_redo[game_prop.prev] = 7;
    game_prop.brc_w_k = 0;
}
//-----------------------------------------------//
// Black castling for the king on the left side
void black_left_castling(void){
    // Update the (x, y) coordinates for the piece
    // Update the row and column number for the piece on the board
    // Record the previous (x, y) and row and column number
    piece[0].x_prev = piece[0].x;
    piece[0].y_prev = piece[0].y;
    piece[0].brow_prev = piece[0].y / 75;
    piece[0].bcol_prev = piece[0].x / 75;
    piece[0].x = (piece[0].bcol + 3) * 75;
    piece[0].y = (piece[0].brow) * 75;
    piece[0].brow = piece[0].y / 75;
    piece[0].bcol = piece[0].x / 75;
    // Record the move to the board which has white and black pieces
    board[piece[0].brow][piece[0].bcol] = -1;
    board[piece[0].brow_prev][piece[0].bcol_prev] = 0;
    // Update the board which has the type of pieces
    board_types[piece[0].brow][piece[0].bcol] = br;
    board_types[piece[0].brow_prev][piece[0].bcol_prev] = 0;
    // Record the move to use it for undo and redo features
    game_prop.prev++;
    game_prop.do_moves_x[game_prop.prev] = piece[0].x_prev;
    game_prop.do_moves_y[game_prop.prev] = piece[0].y_prev;
    game_prop.prev_bcol[game_prop.prev] = piece[0].bcol_prev;
    game_prop.prev_brow[game_prop.prev] = piece[0].brow_prev;
    game_prop.chosen_undo[game_prop.prev] = 0;
    game_prop.redo_moves_x[game_prop.prev] = piece[0].x;
    game_prop.redo_moves_y[game_prop.prev] = piece[0].y;
    game_prop.next_bcol[game_prop.prev] = piece[0].bcol;
    game_prop.next_brow[game_prop.prev] = piece[0].brow;
    game_prop.compare[game_prop.prev] = 3;
    game_prop.chosen_redo[game_prop.prev] = 0;
    game_prop.blc_w_k = 0;
}

//#-------------------------------------------------------------------------#

// Determine which piece was eliminated by the white pieces
void white_kill(int i3){
    dead_piece = i3;
    // Update the flag of (dead or live) to a dead piece
    piece[dead_piece].l_o_d = 0;
    if(game_prop.prev > 30){
        // Check if the game will ends in a draw by dead position
        dead_pos();
        if(game_prop.d_pos == 1){
            game_prop.comp_mate[game_prop.prev] = 6;
        }
    }
    // Record the move to use it for undo and redo features
    game_prop.dead_moves_x[game_prop.prev] = piece[dead_piece].x;
    game_prop.dead_moves_y[game_prop.prev] = piece[dead_piece].y;
    game_prop.prev_bcol_dead[game_prop.prev] = piece[dead_piece].bcol;
    game_prop.prev_brow_dead[game_prop.prev] = piece[dead_piece].brow;
    game_prop.dead_undo[game_prop.prev] = dead_piece;
    game_prop.next_bcol_dead[game_prop.prev] = piece[dead_piece].bcol;
    game_prop.next_brow_dead[game_prop.prev] = piece[dead_piece].brow;
    game_prop.dead_redo[game_prop.prev] = dead_piece;
    game_prop.compare[game_prop.prev] = 1;
    // Update (x, y) coordinates to send it to the tomb
    piece[dead_piece].x = 700 + bdx;
    piece[dead_piece].y = 525 - bdy;
    // Record the (x, y) coordinates to send it to the tomb when a redo is used
    game_prop.re_dead_moves_x[game_prop.prev] = piece[dead_piece].x;
    game_prop.re_dead_moves_y[game_prop.prev] = piece[dead_piece].y;
    bdy += 75;
    if(bdy == 75 * 8){
        bdx = 75;
        bdy = 0;
    }
}
//-----------------------------------------------//
// Determine which piece was eliminated by the white pieces by en passant
void white_kill_passant(int i3){
    dead_piece = i3;
    // Update the flag of (dead or live) to a dead piece
    piece[dead_piece].l_o_d = 0;
    // Update the board which has the type of pieces
    board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = wp;
    board_types[piece[dead_piece].brow][piece[dead_piece].bcol] = 0;
    // Record the move to the board which has white and black pieces
    board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 1;
    board[piece[dead_piece].brow][piece[dead_piece].bcol] = 0;
    // Record the move to use it for undo and redo features
    game_prop.dead_moves_x[game_prop.prev] = piece[dead_piece].x;
    game_prop.dead_moves_y[game_prop.prev] = piece[dead_piece].y;
    game_prop.prev_bcol_dead[game_prop.prev] = piece[dead_piece].bcol;
    game_prop.prev_brow_dead[game_prop.prev] = piece[dead_piece].brow;
    game_prop.dead_undo[game_prop.prev] = dead_piece;
    game_prop.compare[game_prop.prev] = 1;
    game_prop.next_bcol_dead[game_prop.prev] = piece[dead_piece].bcol;
    game_prop.next_brow_dead[game_prop.prev] = piece[dead_piece].brow;
    game_prop.dead_redo[game_prop.prev] = dead_piece;
    // Update (x, y) coordinates to send it to the tomb
    piece[dead_piece].x = 700 + bdx;
    piece[dead_piece].y = 525 - bdy;
    // Record the (x, y) coordinates to send it to the tomb when a redo is used
    game_prop.re_dead_moves_x[game_prop.prev] = piece[dead_piece].x;
    game_prop.re_dead_moves_y[game_prop.prev] = piece[dead_piece].y;
    bdy += 75;
    if(bdy == 75 * 8){
        bdx = 75;
        bdy = 0;
    }
}

//#-------------------------------------------------------------------------#

// Determine which piece was eliminated by the black pieces
void black_kill(int i3){
    dead_piece = i3;
    // Update the flag of (dead or live) to a dead piece
    piece[dead_piece].l_o_d = 0;
    if(game_prop.prev > 30){
        // Check if the game will ends in a draw by dead position
        dead_pos();
        if(game_prop.d_pos == 1){
            game_prop.comp_mate[game_prop.prev] = 6;
        }
    }
    // Record the move to use it for undo and redo features
    game_prop.dead_moves_x[game_prop.prev] = piece[dead_piece].x;
    game_prop.dead_moves_y[game_prop.prev] = piece[dead_piece].y;
    game_prop.prev_bcol_dead[game_prop.prev] = piece[dead_piece].bcol;
    game_prop.prev_brow_dead[game_prop.prev] = piece[dead_piece].brow;
    game_prop.dead_undo[game_prop.prev] = dead_piece;
    game_prop.next_bcol_dead[game_prop.prev] = piece[dead_piece].bcol;
    game_prop.next_brow_dead[game_prop.prev] = piece[dead_piece].brow;
    game_prop.dead_redo[game_prop.prev] = dead_piece;
    game_prop.compare[game_prop.prev] = 1;
    // Update (x, y) coordinates to send it to the tomb
    piece[dead_piece].x = 950 + wdx;
    piece[dead_piece].y = 525 - wdy;
    // Record the (x, y) coordinates to send it to the tomb when a redo is used
    game_prop.re_dead_moves_x[game_prop.prev] = piece[dead_piece].x;
    game_prop.re_dead_moves_y[game_prop.prev] = piece[dead_piece].y;
    wdy += 75;
    if(wdy == 75 * 8){
        wdx = 75;
        wdy= 0;
    }
}
//-----------------------------------------------//
// Determine which piece was eliminated by the black pieces by en passant
void black_kill_passant(int i3){
    dead_piece = i3;
    // Update the flag of (dead or live) to a dead piece
    piece[dead_piece].l_o_d = 0;
    // Update the board which has the type of pieces
    board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = bp;
    board_types[piece[dead_piece].brow][piece[dead_piece].bcol] = 0;
    // Record the move to the board which has white and black pieces
    board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = -1;
    board[piece[dead_piece].brow][piece[dead_piece].bcol] = 0;
    // Record the move to use it for undo and redo features
    game_prop.dead_moves_x[game_prop.prev] = piece[dead_piece].x;
    game_prop.dead_moves_y[game_prop.prev] = piece[dead_piece].y;
    game_prop.prev_bcol_dead[game_prop.prev] = piece[dead_piece].bcol;
    game_prop.prev_brow_dead[game_prop.prev] = piece[dead_piece].brow;
    game_prop.dead_undo[game_prop.prev] = dead_piece;
    game_prop.compare[game_prop.prev] = 1;
    game_prop.next_bcol_dead[game_prop.prev] = piece[dead_piece].bcol;
    game_prop.next_brow_dead[game_prop.prev] = piece[dead_piece].brow;
    game_prop.dead_redo[game_prop.prev] = dead_piece;
    // Update (x, y) coordinates to send it to the tomb
    piece[dead_piece].x = 950 + wdx;
    piece[dead_piece].y = 525 - wdy;
    // Record the (x, y) coordinates to send it to the tomb when a redo is used
    game_prop.re_dead_moves_x[game_prop.prev] = piece[dead_piece].x;
    game_prop.re_dead_moves_y[game_prop.prev] = piece[dead_piece].y;
    wdy += 75;
    if(wdy == 75 * 8){
        wdx = 75;
        wdy= 0;
    }
}

//#-------------------------------------------------------------------------#

// Undo function
void undo(void){
    // A counter for undo times
    game_prop.do_count++;
    // Get the last chosen piece to undo its move
    chosen_piece = game_prop.chosen_undo[game_prop.prev];
    // Get the previous coordinates for the piece
    piece[chosen_piece].x = game_prop.do_moves_x[game_prop.prev];
    piece[chosen_piece].y = game_prop.do_moves_y[game_prop.prev];
    game_prop.en_passant = game_prop.passant[game_prop.prev-1];
    // Determine which is playing now
    if(game_prop.w_o_b == 1){
        // Undo the black move
        // Update the previous move to the board which has white and black pieces
        board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
        board[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = -1;
        // Update the board which has the type of pieces
        switch (piece[chosen_piece].type){
            case 'r':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = br;
                break;
            case 'n':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = bn;
                break;
            case 'b':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = bb;
                break;
            case 'q':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = bq;
                break;
            case 'k':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = bk;
                break;
            case 'p':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = bp;
                break;
        }
        // Update the column and row number for the piece
        if(game_prop.first_move[game_prop.prev] == -1){
            piece[chosen_piece].bcol_prev = -1;
            piece[chosen_piece].brow_prev = -1;
        }
        piece[chosen_piece].bcol = game_prop.prev_bcol[game_prop.prev];
        piece[chosen_piece].brow = game_prop.prev_brow[game_prop.prev];
        // If there were castling, undo it
        if (game_prop.compare[game_prop.prev] == 3 && game_prop.w_o_b == 1 && game_prop.prev > 0){
            game_prop.bcas = 1;
            // Update the undo counter
            game_prop.prev--;
            // Get the last chosen piece to undo its move
            chosen_piece = game_prop.chosen_undo[game_prop.prev];
            // Get the previous coordinates for the piece
            piece[chosen_piece].x = game_prop.do_moves_x[game_prop.prev];
            piece[chosen_piece].y = game_prop.do_moves_y[game_prop.prev];
            // Update the previous move to the board which has white and black pieces
            board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
            board[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = -1;
            // Update the board which has the type of pieces
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
            board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = bk;
            // Rest the previous column and row number for the piece (perparing for castling)
            piece[chosen_piece].brow_prev = -1;
            piece[chosen_piece].bcol_prev = -1;
            // Update the column and row number for the piece
            piece[chosen_piece].bcol = game_prop.prev_bcol[game_prop.prev];
            piece[chosen_piece].brow = game_prop.prev_brow[game_prop.prev];
        }if(game_prop.bcas == 1){
            // Get the last chosen piece to undo its move
            chosen_piece = game_prop.chosen_undo[game_prop.prev + 1];
            // Update the column and row number for the piece
            piece[chosen_piece].brow_prev = -1;
            piece[chosen_piece].bcol_prev = -1;
            game_prop.bcas = 0;
        }
    }else if(game_prop.w_o_b == 0){
        // Undo the white move
        // Update the previous move to the board which has white and black pieces
        board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
        board[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = 1;
        // Update the board which has the type of pieces
        switch (piece[chosen_piece].type){
            case 'r':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = wr;
                break;
            case 'n':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = wn;
                break;
            case 'b':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = wb;
                break;
            case 'q':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = wq;
                break;
            case 'k':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = wk;
                break;
            case 'p':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = wp;
                break;
        }
        // Update the column and row number for the piece
        if(game_prop.first_move[game_prop.prev] == -1){
            piece[chosen_piece].bcol_prev = -1;
            piece[chosen_piece].brow_prev = -1;
        }
        piece[chosen_piece].bcol = game_prop.prev_bcol[game_prop.prev];
        piece[chosen_piece].brow = game_prop.prev_brow[game_prop.prev];
        // If there were castling, undo it
        if(game_prop.compare[game_prop.prev] == 3 && game_prop.w_o_b == 0 && game_prop.prev > 0){
            game_prop.wcas = 1;
            // Update the undo counter
            game_prop.prev--;
            // Get the last chosen piece to undo its move
            chosen_piece = game_prop.chosen_undo[game_prop.prev];
            // Get the previous coordinates for the piece
            piece[chosen_piece].x = game_prop.do_moves_x[game_prop.prev];
            piece[chosen_piece].y = game_prop.do_moves_y[game_prop.prev];
            // Update the previous move to the board which has white and black pieces
            board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
            board[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = 1;
            // Update the board which has the type of pieces
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
            board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = wk;
            // Rest the previous column and row number for the piece (preparing for castling)
            piece[chosen_piece].brow_prev = -1;
            piece[chosen_piece].bcol_prev = -1;
            // Update the column and row number for the piece
            piece[chosen_piece].bcol = game_prop.prev_bcol[game_prop.prev];
            piece[chosen_piece].brow = game_prop.prev_brow[game_prop.prev];
        }if(game_prop.wcas == 1){
            // Get the last chosen piece to undo its move
            chosen_piece = game_prop.chosen_undo[game_prop.prev + 1];
            // Update the column and row number for the piece
            piece[chosen_piece].brow_prev = -1;
            piece[chosen_piece].bcol_prev = -1;
            game_prop.wcas = 0;
        }
    }
    // Undo a promotion for a white piece
    if (game_prop.w_o_b == 0 && game_prop.compare[game_prop.prev] == 2 && game_prop.prev > 0){
        // Get the last chosen piece to undo its move
        chosen_piece = game_prop.chosen_undo[game_prop.prev];
        // Get the previous coordinates for the piece
        piece[chosen_piece].x = game_prop.do_moves_x[game_prop.prev];
        piece[chosen_piece].y = game_prop.do_moves_y[game_prop.prev];
        // Update the column and row number for the piece
        piece[chosen_piece].bcol = game_prop.prev_bcol[game_prop.prev];
        piece[chosen_piece].brow = game_prop.prev_brow[game_prop.prev];
        // Reset the promoted piece to its original type (pawn)
        piece[chosen_piece].type = 'p';
        // Update the board which has the type of pieces
        board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = wp;
        // If there was a check, draw by stalemate or dead position, on the last move,
        // undo it (for attacking and promoted piece)
        if(game_prop.comp_mate[game_prop.prev] == 5){
            game_prop.b_check = 0;
            game_prop.d_pos = 0;
            game_prop.stale = 0;
            game_prop.check = 0;
            game_prop.cover = 0;
        }
        // Update the undo counter
        game_prop.prev--;
        game_prop.do_wpromo = 1;
    }
    // Undo a promotion for a black piece
    else if (game_prop.w_o_b == 1 && game_prop.compare[game_prop.prev] == 2 && game_prop.prev > 0){
        // Get the last chosen piece to undo its move
        chosen_piece = game_prop.chosen_undo[game_prop.prev];
        // Get the previous coordinates for the piece
        piece[chosen_piece].x = game_prop.do_moves_x[game_prop.prev];
        piece[chosen_piece].y = game_prop.do_moves_y[game_prop.prev];
        // Update the column and row number for the piece
        piece[chosen_piece].bcol = game_prop.prev_bcol[game_prop.prev];
        piece[chosen_piece].brow = game_prop.prev_brow[game_prop.prev];
        // Reset the promoted piece to its original type (pawn)
        piece[chosen_piece].type = 'p';
        // Update the board which has the type of pieces
        board_types[game_prop.prev_brow[game_prop.prev]][game_prop.prev_bcol[game_prop.prev]] = bp;
        // If there was a check, draw by stalemate or dead position, on the last move,
        // undo it (for attacking and promoted piece)
        if(game_prop.comp_mate[game_prop.prev] == 4){
            game_prop.w_check = 0;
            game_prop.d_pos = 0;
            game_prop.stale = 0;
            game_prop.check = 0;
            game_prop.cover = 0;
        }
        // Update the undo counter
        game_prop.prev--;
        game_prop.do_bpromo = 1;
    }
    // Undo the eliminated piece and return it
    if (game_prop.compare[game_prop.prev] == 1 && game_prop.prev > 0){
        // Get the last dead piece to undo its move
        dead_piece = game_prop.dead_undo[game_prop.prev];
        // Get the previous coordinates for the piece
        piece[dead_piece].x = game_prop.dead_moves_x[game_prop.prev];
        piece[dead_piece].y = game_prop.dead_moves_y[game_prop.prev];
        // Set the live or dead flag back to live (one)
        piece[dead_piece].l_o_d = 1;
        // White piece is dead
        if(game_prop.w_o_b == 1){
            // Update the previous kill to the board which has white and black pieces
            board[piece[dead_piece].brow][piece[dead_piece].bcol] = 1;
            // Update the board which has the type of pieces
            switch (piece[dead_piece].type){
                case 'r':
                    board_types[piece[dead_piece].brow][piece[dead_piece].bcol] = wr;
                    break;
                case 'n':
                    board_types[piece[dead_piece].brow][piece[dead_piece].bcol] = wn;
                    break;
                case 'b':
                    board_types[piece[dead_piece].brow][piece[dead_piece].bcol] = wb;
                    break;
                case 'q':
                    board_types[piece[dead_piece].brow][piece[dead_piece].bcol] = wq;
                    break;
                case 'p':
                    board_types[piece[dead_piece].brow][piece[dead_piece].bcol] = wp;
                    break;
            }
        }
        // White piece is dead
        else{
            // Update the previous kill to the board which has white and black pieces
            board[piece[dead_piece].brow][piece[dead_piece].bcol] = -1;
            // Update the board which has the type of pieces
            switch (piece[dead_piece].type){
                case 'r':
                    board_types[piece[dead_piece].brow][piece[dead_piece].bcol] = br;
                    break;
                case 'n':
                    board_types[piece[dead_piece].brow][piece[dead_piece].bcol] = bn;
                    break;
                case 'b':
                    board_types[piece[dead_piece].brow][piece[dead_piece].bcol] = bb;
                    break;
                case 'q':
                    board_types[piece[dead_piece].brow][piece[dead_piece].bcol] = bq;
                    break;
                case 'p':
                    board_types[piece[dead_piece].brow][piece[dead_piece].bcol] = bp;
                    break;
            }
        }
        // Update the column and row number for the piece
        piece[dead_piece].bcol = game_prop.prev_bcol_dead[game_prop.prev];
        piece[dead_piece].brow = game_prop.prev_brow_dead[game_prop.prev];
    }

    // If there was a check, draw by stalemate or dead position, on the last move,
    // undo it (for attacking piece)
    if(game_prop.comp_mate[game_prop.prev] == 4 || game_prop.comp_mate[game_prop.prev] == 5 || game_prop.comp_mate[game_prop.prev] == 6 ||
        game_prop.comp_mate[game_prop.prev] == 7 || game_prop.comp_mate[game_prop.prev] == 0){
        game_prop.w_check = 0;
        game_prop.b_check = 0;
        game_prop.d_pos = 0;
        game_prop.stale = 0;
        game_prop.check = 0;
        game_prop.cover = 0;
    }
    // If there was a check on the last move, undo it (for covering piece)
    if(game_prop.comp_mate[game_prop.prev] == -5){
        game_prop.b_check = 1;
        game_prop.check = 1;
        game_prop.cover = 1;
    }
    // If there was a check on the last move, undo it (for covering piece)
    if(game_prop.comp_mate[game_prop.prev] == -4){
        game_prop.w_check = 1;
        game_prop.check = 1;
        game_prop.cover = 1;
    }

    // Toggle the player's turn
    if (game_prop.w_o_b == 1){
        game_prop.w_o_b = 0;
    }else{
        game_prop.w_o_b = 1;
    }
    flag = 0;
    // Update the undo counter
    game_prop.prev--;
}
//-----------------------------------------------//
void redo(void){
    // Update the undo counter
    game_prop.prev++;
    // A counter for redo times
    game_prop.do_count--;
    // Get the next chosen piece to redo its move
    chosen_piece = game_prop.chosen_redo[game_prop.prev];
    // Get the next coordinates for the piece
    piece[chosen_piece].x = game_prop.redo_moves_x[game_prop.prev];
    piece[chosen_piece].y = game_prop.redo_moves_y[game_prop.prev];
    game_prop.en_passant = game_prop.passant[game_prop.prev];
    // Determine which is playing now
    if(game_prop.w_o_b == 1){
        // Redo the white move
        // Update the next move to the board which has white and black pieces
        board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
        board[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = 1;
        // Update the board which has the type of pieces
        switch (piece[chosen_piece].type){
            case 'r':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = wr;
                break;
            case 'n':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = wn;
                break;
            case 'b':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = wb;
                break;
            case 'q':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = wq;
                break;
            case 'k':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = wk;
                break;
            case 'p':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = wp;
                break;
        }
        // Update the column and row number for the piece
        piece[chosen_piece].bcol = game_prop.next_bcol[game_prop.prev];
        piece[chosen_piece].brow = game_prop.next_brow[game_prop.prev];

        // If there were castling for white, redo it
        if (game_prop.compare[game_prop.prev + 1] == 3 && game_prop.w_o_b == 1 && game_prop.prev > 0){
            game_prop.wcas = 1;
            // Update the undo counter
            game_prop.prev++;
            // Get the next chosen piece to redo its move
            chosen_piece = game_prop.chosen_redo[game_prop.prev];
            // Get the next coordinates for the piece
            piece[chosen_piece].x = game_prop.redo_moves_x[game_prop.prev];
            piece[chosen_piece].y = game_prop.redo_moves_y[game_prop.prev];
            // Update the next move to the board which has white and black pieces
            board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
            board[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = 1;
            // Update the board which has the type of pieces
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
            board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = wr;
            // Rest the next column and row number for the piece
            piece[chosen_piece].brow_prev = -1;
            piece[chosen_piece].bcol_prev = -1;
            // Update the column and row number for the piece
            piece[chosen_piece].bcol = game_prop.next_bcol[game_prop.prev];
            piece[chosen_piece].brow = game_prop.next_brow[game_prop.prev];
        }if(game_prop.wcas == 1){
            // Get the next chosen piece to redo its move
            chosen_piece = game_prop.chosen_undo[game_prop.prev - 1];
            // Update the column and row number for the piece
            piece[chosen_piece].brow_prev = -1;
            piece[chosen_piece].bcol_prev = -1;
            game_prop.wcas = 0;
        }
    }
    // Determine which is playing now
    else if(game_prop.w_o_b == 0){
        // Redo the black move
        // Update the next move to the board which has white and black pieces
        board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
        board[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = -1;
        // Update the board which has the type of pieces
        switch (piece[chosen_piece].type){
            case 'r':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = br;
                break;
            case 'n':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = bn;
                break;
            case 'b':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = bb;
                break;
            case 'q':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = bq;
                break;
            case 'k':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = bk;
                break;
            case 'p':
                board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
                board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = bp;
                break;
        }
        // Update the column and row number for the piece
        piece[chosen_piece].bcol = game_prop.next_bcol[game_prop.prev];
        piece[chosen_piece].brow = game_prop.next_brow[game_prop.prev];

        // If there were castling for black, redo it
        if (game_prop.compare[game_prop.prev + 1] == 3 && game_prop.w_o_b == 0 && game_prop.prev > 0){
            game_prop.bcas = 1;
            // Update the undo counter
            game_prop.prev++;
            // Get the next chosen piece to redo its move
            chosen_piece = game_prop.chosen_redo[game_prop.prev];
            // Get the next coordinates for the piece
            piece[chosen_piece].x = game_prop.redo_moves_x[game_prop.prev];
            piece[chosen_piece].y = game_prop.redo_moves_y[game_prop.prev];
            // Update the next move to the board which has white and black pieces
            board[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
            board[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = -1;
            // Update the board which has the type of pieces
            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = 0;
            board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = br;
            // Rest the next column and row number for the piece
            piece[chosen_piece].brow_prev = -1;
            piece[chosen_piece].bcol_prev = -1;
            // Update the column and row number for the piece
            piece[chosen_piece].bcol = game_prop.next_bcol[game_prop.prev];
            piece[chosen_piece].brow = game_prop.next_brow[game_prop.prev];
        }if(game_prop.bcas == 1){
            // Get the next chosen piece to redo its move
            chosen_piece = game_prop.chosen_undo[game_prop.prev - 1];
            // Update the column and row number for the piece
            piece[chosen_piece].brow_prev = -1;
            piece[chosen_piece].bcol_prev = -1;
            game_prop.bcas = 0;
        }
    }
    // If there was white king check, redo it (for attacking piece)
    if(game_prop.comp_mate[game_prop.prev] == 4){
        game_prop.w_check = 1;
        game_prop.check = 1;
    }
    // If there was black king check, redo it (for attacking piece)
    if(game_prop.comp_mate[game_prop.prev] == 5){
        game_prop.b_check = 1;
        game_prop.check = 1;
    }
    // If there was white king check, redo it (for covering piece)
    if(game_prop.comp_mate[game_prop.prev] == -4){
        game_prop.w_check = 0;
        game_prop.check = 0;
        game_prop.cover = 0;
    }
    // If there was black king check, redo it (for covering piece)
    if(game_prop.comp_mate[game_prop.prev] == -5){
        game_prop.b_check = 0;
        game_prop.check = 0;
        game_prop.cover = 0;
    }
    // If there was a draw by dead position, redo it
    if(game_prop.comp_mate[game_prop.prev] == 6){
        game_prop.d_pos = 1;
    }
    // If there was a draw by stalemate, redo it
    if(game_prop.comp_mate[game_prop.prev] == 7){
        game_prop.stale = 1;
    }
    // If there was nothing, reset flags value
    if(game_prop.comp_mate[game_prop.prev] == 0){
        game_prop.b_check = 0;
        game_prop.w_check = 0;
        game_prop.d_pos = 0;
        game_prop.stale = 0;
        game_prop.check = 0;
    }
    // Redo the kill move and send the dead piece to the tomb
    if (game_prop.compare[game_prop.prev] == 1 && game_prop.prev > 0){
        // Get the next dead piece to redo its move
        dead_piece = game_prop.dead_redo[game_prop.prev];
        // Get the next coordinates for the piece
        piece[dead_piece].x = game_prop.re_dead_moves_x[game_prop.prev];
        piece[dead_piece].y = game_prop.re_dead_moves_y[game_prop.prev];
        // Update the column and row number for the piece
        piece[dead_piece].bcol = game_prop.next_bcol_dead[game_prop.prev];
        piece[dead_piece].brow = game_prop.next_brow_dead[game_prop.prev];
        // Set the live or dead flag back to dead (zero)
        piece[dead_piece].l_o_d = 0;
    }
    // Redo a promotion for a white piece
    if (game_prop.w_o_b == 1 && game_prop.compare[game_prop.prev + 1] == 2 && game_prop.prev > 0){
        // Get the next chosen piece to redo its move
        chosen_piece = game_prop.chosen_redo[game_prop.prev];
        // Get the next coordinates for the piece
        piece[chosen_piece].x = game_prop.redo_moves_x[game_prop.prev];
        piece[chosen_piece].y = game_prop.redo_moves_y[game_prop.prev];
        // Update the column and row number for the piece
        piece[chosen_piece].bcol = game_prop.next_bcol[game_prop.prev];
        piece[chosen_piece].brow = game_prop.next_brow[game_prop.prev];
        // Update the piece to the promoted piece
        piece[chosen_piece].type = game_prop.redo_promo_type[game_prop.prev];
        switch (piece[chosen_piece].type){
                case 'r':
                    board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = wr;
                    game_prop.wpr = 1;
                    break;
                case 'n':
                    board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = wn;
                    game_prop.wpn = 1;
                    break;
                case 'b':
                    board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = wb;
                    game_prop.wpb = 1;
                    break;
                case 'q':
                    board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = wq;
                    game_prop.wpq = 1;
                    break;
            }
        // Update the undo counter
        game_prop.prev++;
        // If there was a check, draw by stalemate or dead position, on the next move,
        // redo it (for attacking and promoted piece)
        if(game_prop.comp_mate[game_prop.prev] == 5){
            game_prop.b_check = 1;
            game_prop.check = 1;
        }
    }
    // Redo a promotion for a black piece
    else if (game_prop.w_o_b == 0 && game_prop.compare[game_prop.prev + 1] == 2 && game_prop.prev > 0){
        // Get the next chosen piece to redo its move
        chosen_piece = game_prop.chosen_redo[game_prop.prev];
        // Get the next coordinates for the piece
        piece[chosen_piece].x = game_prop.redo_moves_x[game_prop.prev];
        piece[chosen_piece].y = game_prop.redo_moves_y[game_prop.prev];
        // Update the column and row number for the piece
        piece[chosen_piece].bcol = game_prop.next_bcol[game_prop.prev];
        piece[chosen_piece].brow = game_prop.next_brow[game_prop.prev];
        // Update the piece to the promoted piece
        piece[chosen_piece].type = game_prop.redo_promo_type[game_prop.prev];
        switch (piece[chosen_piece].type){
                case 'r':
                    board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = br;
                    game_prop.bpr = 1;
                    break;
                case 'n':
                    board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = bn;
                    game_prop.bpn = 1;
                    break;
                case 'b':
                    board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = bb;
                    game_prop.bpb = 1;
                    break;
                case 'q':
                    board_types[game_prop.next_brow[game_prop.prev]][game_prop.next_bcol[game_prop.prev]] = bq;
                    game_prop.bpq = 1;
                    break;
            }
        // Update the undo counter
        game_prop.prev++;
        // If there was a check, draw by stalemate or dead position, on the next move,
        // redo it (for attacking and promoted piece)
        if(game_prop.comp_mate[game_prop.prev] == 4){
            game_prop. w_check = 1;
            game_prop.check = 1;
        }
    }
    // Toggle the player's turn
    if (game_prop.w_o_b == 1){
        game_prop.w_o_b = 0;
    }else{
        game_prop.w_o_b = 1;
    }
    flag = 0;
}

//#-------------------------------------------------------------------------#

// A function to save the game
void save(void){
    int y, f, l;
    // Open file to Save information in it
    last_scene = fopen("save/last scene.bin", "wb");
    for(y = 0; y < 32; y++){
        // Save the array of pieces
        fwrite(&piece[y], sizeof(piece[y]), 1, last_scene);
    }
    for(f = 0; f < 8; f++){
        for(l = 0; l < 8; l++){
            // Save the boards
            fwrite(&board[f][l], sizeof(board[f][l]), 1, last_scene);
            fwrite(&board_types[f][l], sizeof(board_types[f][l]), 1, last_scene);
        }
    }
    // Save game properties structure
    fwrite(&game_prop, sizeof(game_prop), 1, last_scene);
    // Close the file
    fclose(last_scene);
}
//-----------------------------------------------//
// A function to load the game
void load(void){
    int y, f, l;
    // Load the saved file
    last_scene = fopen("save/last scene.bin", "rb");
    for(y = 0; y < 32; y++){
        // Load the array of pieces
        fread(&piece[y], sizeof(piece[y]), 1, last_scene);
    }
    for( f = 0; f < 8; f++){
        for(l = 0; l < 8; l++){
            // Load the boards
            fread(&board[f][l], sizeof(board[f][l]), 1, last_scene);
            fread(&board_types[f][l], sizeof(board_types[f][l]), 1, last_scene);
        }
    }
    // Load game properties structure
    fread(&game_prop, sizeof(game_prop), 1, last_scene);
    // Close the file
    fclose(last_scene);
    // A loop to determine which piece was promoted
    for(y = 0; y < 16 ; y++){
        switch(piece[y].type){
            case 'r':
                texture[(y + 1)] = texture[1];
                break;
            case 'q':
                texture[(y + 1)] = texture[4];
                break;
            case 'n':
                texture[(y + 1)] = texture[2];
                break;
            case 'b':
                texture[(y + 1)] = texture[3];
                break;
            case 'p':
                texture[(y + 1)] = texture[9];
                break;
        }
    }
    for(y = 16; y < 32 ; y++){
        switch(piece[y].type){
            case 'r':
                texture[(y + 1)] = texture[32];
                break;
            case 'q':
                texture[(y + 1)] = texture[28];
                break;
            case 'n':
                texture[(y + 1)] = texture[31];
                break;
            case 'b':
                texture[(y + 1)] = texture[30];
                break;
            case 'p':
                texture[(y + 1)] = texture[17];
                break;
        }
    }
}

//#-------------------------------------------------------------------------#
// Take user inputs and change the coordinates:

void process_input(void){
    int i3 = 0, i = 0;
    // The event will be called
    SDL_Event event;
    SDL_WaitEvent(&event);
    // Determine which event was called
    switch(event.type){
        // When pressing the close button on the window, close the game
        case SDL_QUIT:
            Ready_To_start = 0;
            break;
//-----------------------------------------------//
        // When pressing the escape key, close the game
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE){
                Ready_To_start = 0;
            }
            break;
//-----------------------------------------------//
        // When pressing the mouse button
        case SDL_MOUSEBUTTONDOWN:
            // On the save game icon, save the game
            if(event.button.button == SDL_BUTTON_LEFT && event.button.x > 600 && event.button.x < 700 &&
                event.button.y > 500 && event.button.y < 600){
                save();
                f_save = 1;
            }
//-----------------------------------------------//
            // On the load game icon, load the game
            else if(event.button.button == SDL_BUTTON_LEFT && event.button.x > 600 && event.button.x < 700 &&
                event.button.y > 0 && event.button.y < 100){
                load();
                f_load = 1;
            }
//-----------------------------------------------//
            // On the undo icon, undo the last move
            else if(event.button.button == SDL_BUTTON_LEFT && event.button.x > 850 && event.button.x < 950 &&
                event.button.y > 400 && event.button.y < 500 && game_prop.prev >= 0 && game_prop.wpromo == 0 && game_prop.bpromo == 0){
                undo();
                f_undo = 1;
            }
//-----------------------------------------------//
            // On the redo icon, redo the next move
            else if(event.button.button == SDL_BUTTON_LEFT && event.button.x > 850 && event.button.x < 950 &&
                    event.button.y > 500 && event.button.y < 600 && game_prop.do_count > 0 && game_prop.wpromo == 0 && game_prop.bpromo == 0){
                redo();
                f_redo = 1;
            }
//-----------------------------------------------//
            else{
                // Black player turn
                if(game_prop.w_o_b == 0){
                    // When a black pawn is promoted
                    // Decide which piece will promote to
                    if(game_prop.bpromo){
                        chosen_piece = temp_chosen;
                        // If a rook was chosen
                        if(event.button.x > 850 && event.button.x < 950 && event.button.y > 0
                            && event.button.y < 75 && event.button.button == SDL_BUTTON_LEFT){
                            piece[chosen_piece].type = 'r';
                            game_prop.redo_promo_type[game_prop.prev] = 'r';
                            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = br;
                            game_prop.bpr = 1;
                        }
                        // If a bishop was chosen
                        else if(event.button.x > 850 && event.button.x < 950 && event.button.y > 75
                                && event.button.y < 150 && event.button.button == SDL_BUTTON_LEFT){
                            piece[chosen_piece].type = 'b';
                            game_prop.redo_promo_type[game_prop.prev] = 'b';
                            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = bb;
                            game_prop.bpb = 1;
                            // Check if there is a move to be done for the enemy and the king isn't checked
                            stalemate();
                            dead_pos();
                        }
                        // If a knight was chosen
                        else if(event.button.x > 850 && event.button.x < 950 && event.button.y > 150
                                && event.button.y < 225 && event.button.button == SDL_BUTTON_LEFT){
                            piece[chosen_piece].type = 'n';
                            game_prop.redo_promo_type[game_prop.prev] = 'n';
                            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = bn;
                            game_prop.bpn = 1;
                            // Check if there is a move to be done for the enemy and the king isn't checked
                            stalemate();
                            dead_pos();
                        }
                        // If a queen was chosen
                        else if(event.button.x > 850 && event.button.x < 950 && event.button.y > 225
                                && event.button.y < 300 && event.button.button == SDL_BUTTON_LEFT){
                            piece[chosen_piece].type = 'q';
                            game_prop.redo_promo_type[game_prop.prev] = 'q';
                            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = bq;
                            game_prop.bpq = 1;
                        }
                        if(game_prop.bpr == 1 || game_prop.bpb == 1 || game_prop.bpn == 1 || game_prop.bpq == 1){
                            // Record the move to use it for undo and redo features
                            game_prop.prev++;
                            game_prop.do_moves_x[game_prop.prev] = piece[chosen_piece].x_prev;
                            game_prop.do_moves_y[game_prop.prev] = piece[chosen_piece].y_prev;
                            game_prop.prev_bcol[game_prop.prev] = piece[chosen_piece].bcol_prev;
                            game_prop.prev_brow[game_prop.prev] = piece[chosen_piece].brow_prev;
                            game_prop.chosen_undo[game_prop.prev] = chosen_piece;
                            game_prop.redo_moves_x[game_prop.prev] = piece[chosen_piece].x;
                            game_prop.redo_moves_y[game_prop.prev] = piece[chosen_piece].y;
                            game_prop.next_bcol[game_prop.prev] = piece[chosen_piece].bcol;
                            game_prop.next_brow[game_prop.prev] = piece[chosen_piece].brow;
                            game_prop.chosen_redo[game_prop.prev] = chosen_piece;
                            game_prop.compare[game_prop.prev] = 2;
                            game_prop.bpromo = 0;
                            if(!(chosen_piece == 4)){
                                // Check if the promoted piece is attacking the king or not
                                check_king_att(chosen_piece);
                            }
                            // If the king was checked,
                            if(game_prop.w_check == 1){
                                // record this move for undo and redo features
                                game_prop.comp_mate[game_prop.prev] = 4;
                                game_prop.comp_mate[game_prop.prev + 1] = -4;
                            }else if(!(game_prop.comp_mate[game_prop.prev] == -5)){
                                // record this move for undo and redo features
                                game_prop.comp_mate[game_prop.prev] = 0;
                            }
                            game_prop.w_o_b = 1;
                        }
                    }else{
                        // If it is a normal move
                        for(i = 0; i < 16; i++){
                            // Find which piece was chosen to move
                            if(event.button.x > piece[i].x && event.button.x < piece[i].x + 75 && event.button.y > piece[i].y
                                && event.button.y < piece[i].y + 75 && event.button.button == SDL_BUTTON_LEFT && event.button.x <= 600){
                                // A flag to determine if the button was down or not
                                if (flag == 1){
                                    flag = 0;
                                }else{
                                    flag = 1;
                                    chosen_piece = i;
                                    // If the king was checked
                                    if(game_prop.b_check){
                                        game_prop.cov_shield = 0;
                                        // Check if there is a cover for the king or the king is checked
                                        check_cover();
                                        if(game_prop.cov_shield == 0){
                                            // Check if there is a piece which masks (shield) the enemy attack
                                            check_shield();
                                        }
                                    }else{
                                        // Get the valid moves of the piece
                                        get_valid_moves(chosen_piece);
                                        // Check if there is a cover for the king or the king is checked
                                        check_cover();
                                    }
                                    // Update the (x, y) coordinates for the piece
                                    piece[chosen_piece].x_prev = piece[chosen_piece].x;
                                    piece[chosen_piece].y_prev = piece[chosen_piece].y;
                                    // Update the previous row and column number for the piece on the board
                                    piece[chosen_piece].temp_brow_prev = piece[chosen_piece].brow_prev;
                                    piece[chosen_piece].brow_prev = piece[chosen_piece].y / 75;
                                    piece[chosen_piece].bcol_prev = piece[chosen_piece].x / 75;
                                    guide = 1;
                                    render();
                                }
                            }
                        }
                    }
                }
                // White player turn
                else if(game_prop.w_o_b == 1){
                    // When a black pawn is promoted
                    // Decide which piece will promote to
                    if(game_prop.wpromo){
                        chosen_piece = temp_chosen;
                        // If a rook was chosen
                        if(event.button.x > 850 && event.button.x < 950 && event.button.y > 0
                            && event.button.y < 75 && event.button.button == SDL_BUTTON_LEFT){
                            piece[chosen_piece].type = 'r';
                            game_prop.redo_promo_type[game_prop.prev] = 'r';
                            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = wr;
                            game_prop.wpr = 1;
                        }
                        // If a bishop was chosen
                        else if(event.button.x > 850 && event.button.x < 950 && event.button.y > 75
                                && event.button.y < 150 && event.button.button == SDL_BUTTON_LEFT){
                            piece[chosen_piece].type = 'b';
                            game_prop.redo_promo_type[game_prop.prev] = 'b';
                            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = wb;
                            game_prop.wpb = 1;
                            // Check if there is a move to be done for the enemy and the king isn't checked
                            stalemate();
                            dead_pos();
                        }
                        // If a knight was chosen
                        else if(event.button.x > 850 && event.button.x < 950 && event.button.y > 150
                                && event.button.y < 225 && event.button.button == SDL_BUTTON_LEFT){
                            piece[chosen_piece].type = 'n';
                            game_prop.redo_promo_type[game_prop.prev] = 'n';
                            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = wn;
                            game_prop.wpn = 1;
                            // Check if there is a move to be done for the enemy and the king isn't checked
                            stalemate();
                            dead_pos();
                        }
                        // If a queen was chosen
                        else if(event.button.x > 850 && event.button.x < 950 && event.button.y > 225
                                && event.button.y < 300 && event.button.button == SDL_BUTTON_LEFT){
                            piece[chosen_piece].type = 'q';
                            game_prop.redo_promo_type[game_prop.prev] = 'q';
                            board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] = wq;
                            game_prop.wpq = 1;
                        }
                        if(game_prop.wpr == 1 || game_prop.wpb == 1 || game_prop.wpn == 1 || game_prop.wpq == 1){
                            // Record the move to use it for undo and redo features
                            game_prop.prev++;
                            game_prop.do_moves_x[game_prop.prev] = piece[chosen_piece].x_prev;
                            game_prop.do_moves_y[game_prop.prev] = piece[chosen_piece].y_prev;
                            game_prop.prev_bcol[game_prop.prev] = piece[chosen_piece].bcol_prev;
                            game_prop.prev_brow[game_prop.prev] = piece[chosen_piece].brow_prev;
                            game_prop.chosen_undo[game_prop.prev] = chosen_piece;
                            game_prop.redo_moves_x[game_prop.prev] = piece[chosen_piece].x;
                            game_prop.redo_moves_y[game_prop.prev] = piece[chosen_piece].y;
                            game_prop.next_bcol[game_prop.prev] = piece[chosen_piece].bcol;
                            game_prop.next_brow[game_prop.prev] = piece[chosen_piece].brow;
                            game_prop.chosen_redo[game_prop.prev] = chosen_piece;
                            game_prop.compare[game_prop.prev] = 2;
                            game_prop.wpromo = 0;
                            if(!(chosen_piece == 28)){
                                // Check if the promoted piece is attacking the king or not
                                check_king_att(chosen_piece);
                            }
                            // If the king was checked,
                            if(game_prop.b_check == 1){
                                // record this move for undo and redo features
                                game_prop.comp_mate[game_prop.prev] = 5;
                                game_prop.comp_mate[game_prop.prev + 1] = -5;
                            }else if(!(game_prop.comp_mate[game_prop.prev] == -4)){
                                // record this move for undo and redo features
                                game_prop.comp_mate[game_prop.prev] = 0;
                            }
                            game_prop.w_o_b = 0;
                        }
                    }else{
                        // If it is a normal move
                        for(i = 16; i < 32; i++){
                            // Find which piece was chosen to move
                            if(event.button.x > piece[i].x && event.button.x < piece[i].x + 75 && event.button.y > piece[i].y
                                && event.button.y < piece[i].y + 75 && event.button.button == SDL_BUTTON_LEFT && event.button.x<=600){
                                // A flag to determine if the button was down or not
                                if (flag == 1){
                                    flag = 0;
                                } else {
                                    flag = 1;
                                    chosen_piece = i;
                                    // If the king was checked
                                    if(game_prop.w_check){
                                        game_prop.cov_shield = 0;
                                        // Check if there is a cover for the king or the king is checked
                                        check_cover();
                                        if(game_prop.cov_shield == 0){
                                            // Check if there is a piece which masks (shield) the enemy attack
                                            check_shield();
                                        }
                                    }else{
                                        // Get the valid moves of the piece
                                        get_valid_moves(chosen_piece);
                                        // Check if there is a cover for the king or the king is checked
                                        check_cover();
                                    }
                                    // Update the (x, y) coordinates for the piece
                                    piece[chosen_piece].x_prev = piece[chosen_piece].x;
                                    piece[chosen_piece].y_prev = piece[chosen_piece].y;
                                    // Update the previous row and column number for the piece on the board
                                    piece[chosen_piece].temp_brow_prev = piece[chosen_piece].brow_prev;
                                    piece[chosen_piece].brow_prev = piece[chosen_piece].y / 75;
                                    piece[chosen_piece].bcol_prev = piece[chosen_piece].x / 75;
                                    guide = 1;
                                    render();
                                }
                            }
                        }
                    }
                }
            }
            break;
//#-------------------------------------------------------------------------#
        // Handle the mouse motion move to determine the new position for the piece
        case SDL_MOUSEMOTION:
            if(flag == 1 && (event.button.y <= 600 && event.button.x <= 600)){
                piece[chosen_piece].x = (event.button.x - 75 / 2);
                piece[chosen_piece].y = (event.button.y - 75 / 2);
            }
            break;

//#-------------------------------------------------------------------------#

        // When the mouse button is released
        case(SDL_MOUSEBUTTONUP):
            guide = 0;
            // If in the same position of the piece, Update it to the same place coordinates
            if (flag == 1 && ((event.button.x) / 75) * 75 == piece[chosen_piece].x_prev && event.button.x <= 600 &&
                ((event.button.y ) / 75) * 75 == piece[chosen_piece].y_prev && event.button.button == SDL_BUTTON_LEFT){
                flag = 0;
                // Update the (x, y) coordinates for the piece
                piece[chosen_piece].x = ((event.button.x) / 75) * 75;
                piece[chosen_piece].y = ((event.button.y) / 75) * 75;
                // Update the row and column number for the piece on the board
                piece[chosen_piece].brow = piece[chosen_piece].y / 75;
                piece[chosen_piece].bcol = piece[chosen_piece].x / 75;
                // A Flag to determine if it was the first move or not
                if(piece[chosen_piece].temp_brow_prev == -1){
                    piece[chosen_piece].brow_prev = -1;
                    piece[chosen_piece].bcol_prev = -1;
                }
                // Preparing for en passant
                if(piece[chosen_piece].type == 'p'){
                    piece[chosen_piece].brow_prev = piece[chosen_piece].temp_brow_prev;
                    piece[chosen_piece].bcol_prev = piece[chosen_piece].temp_brow_prev;
                }
            }

//#-------------------------------------------------------------------------#

            // White Turn
            else if (flag == 1 && game_prop.w_o_b == 1 && event.button.button == SDL_BUTTON_LEFT && event.button.x<=600){
                flag = 0;
                // Save the last move to a temporary variable to check if it is valid or not
                piece[chosen_piece].xtemp = ((event.button.x) / 75) * 75;
                piece[chosen_piece].ytemp = ((event.button.y) / 75) * 75;
                // Check if this move is valid or not
                if (check_valid()){
                    // If the king was checked, clear the check
                    if(game_prop.w_check == 1){
                        game_prop.w_check = 0;
                        game_prop.cover = 0;
                    }
                    // Update the (x, y) coordinates for the piece
                    piece[chosen_piece].x = ((event.button.x) / 75) * 75;
                    piece[chosen_piece].y = ((event.button.y) / 75) * 75;
                    // Record the white movement and update the boards
                    white_move();
                    // Preparing for en passant
                    if(game_prop.en_passant >= 0 && piece[chosen_piece].type == 'p' &&
                    piece[chosen_piece].brow_prev == 6 && piece[chosen_piece].brow == 4){
                        game_prop.en_passant = 2;
                    }
                    // Check if there is a promotion or not
                    if(piece[chosen_piece].type == 'p' && piece[chosen_piece].brow == 0){
                        game_prop.wpromo = 1;
                        render();
                    }else{
                        game_prop.wpromo = 0;
                    }
                    // Check if there is castling or not
                    if(game_prop.wrc_w_k && (event.button.x / 75 == 6) && piece[28].bcol == 6 && piece[28].brow == 7){
                        white_right_castling();
                    }
                    else if(game_prop.wlc_w_k && (event.button.x / 75 == 2) && piece[28].bcol == 2 && piece[28].brow == 7){
                        white_left_castling();
                    }
                    // Check if the move result in a kill or not
                    if(check_die()){
                        for(i3 = 0; i3 < 16; i3++){
                            // If there is a normal kill
                            if(event.button.x > piece[i3].x && event.button.x < piece[i3].x + 75 && event.button.y > piece[i3].y
                                && event.button.y < piece[i3].y + 75 && event.button.button == SDL_BUTTON_LEFT && i3 != chosen_piece){
                                white_kill(i3);
                                break;
                            }
                            // For en passant kill
                            else if (board_types[piece[chosen_piece].brow + 1][piece[chosen_piece].bcol] == bp
                                && board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] == wp &&
                                piece[chosen_piece].brow == 2 &&
                                event.button.x > piece[i3].x && event.button.x < piece[i3].x + 75 && (event.button.y) + 75 > piece[i3].y
                                && event.button.y + 75 < piece[i3].y + 75 && event.button.button == SDL_BUTTON_LEFT && i3 != chosen_piece){
                                white_kill_passant(i3);
                                break;
                            }
                        }
                    }
                    // If there is a promotion, toggle the turn
                    if(game_prop.wpromo){
                        game_prop.w_o_b = 1;
                    }else{
                        game_prop.w_o_b = 0;
                        // Preparing for en passant
                        if(game_prop.en_passant > 0){
                            game_prop.en_passant--;
                            game_prop.passant[game_prop.prev] = game_prop.en_passant;
                        }
                    }
                }else{
                    // If the move was not valid, keep everything in its place
                    piece[chosen_piece].x = piece[chosen_piece].x_prev;
                    piece[chosen_piece].y = piece[chosen_piece].y_prev;
                    wrong = 1;
                    render();
                    SDL_Delay(1000);
                    wrong = 0;
                    game_prop.w_o_b = 1;
                }
            }
 //#-------------------------------------------------------------------------#

            // Black Turn
            else if (flag == 1 && game_prop.w_o_b == 0 && event.button.button == SDL_BUTTON_LEFT && event.button.x<=600){
                flag = 0;
                // Save the last move to a temporary variable to check if it is valid or not
                piece[chosen_piece].xtemp = ((event.button.x) / 75) * 75;
                piece[chosen_piece].ytemp = ((event.button.y) / 75) * 75;
                // Check if this move is valid or not
                if (check_valid()){
                    // If the king was checked, clear the check
                    if(game_prop.b_check == 1){
                        game_prop.b_check = 0;
                        game_prop.cover = 0;
                    }
                    // Update the (x, y) coordinates for the piece
                    piece[chosen_piece].x = ((event.button.x) / 75) * 75;
                    piece[chosen_piece].y = ((event.button.y) / 75) * 75;
                    // Record the white movement and update the boards
                    black_move();
                    // Preparing for en passant
                    if(game_prop.en_passant >= 0 && piece[chosen_piece].type == 'p' &&
                    piece[chosen_piece].brow_prev == 1 && piece[chosen_piece].brow == 3){
                        game_prop.en_passant = 2;
                    }
                    // Check if there is a promotion or not
                    if(piece[chosen_piece].type == 'p' && piece[chosen_piece].brow == 7){
                        game_prop.bpromo = 1;
                        render();
                    }else{
                        game_prop.bpromo = 0;
                    }
                    // Check if there is castling or not
                    if(game_prop.brc_w_k && (event.button.x / 75 == 6) && piece[4].bcol == 6 && piece[4].brow == 0){
                        black_right_castling();
                    }
                    else if(game_prop.blc_w_k && (event.button.x / 75 == 2) && piece[4].bcol == 2 && piece[4].brow == 0){
                        black_left_castling();
                    }
                    // Check if the move result in a kill or not
                    if(check_die()){
                        for(i3 = 16; i3 < 32; i3++){
                            // If there is a normal kill
                            if(event.button.x > piece[i3].x && event.button.x < piece[i3].x + 75 && event.button.y > piece[i3].y
                                && event.button.y < piece[i3].y + 75 && event.button.button == SDL_BUTTON_LEFT && i3 != chosen_piece){
                                black_kill(i3);
                                break;
                            }
                            // For en passant kill
                            else if (board_types[piece[chosen_piece].brow - 1][piece[chosen_piece].bcol] == wp
                                && board_types[piece[chosen_piece].brow][piece[chosen_piece].bcol] == bp &&
                                piece[chosen_piece].brow == 5 &&
                                event.button.x > piece[i3].x && event.button.x < piece[i3].x + 75 && (event.button.y) - 75 > piece[i3].y
                                && (event.button.y) - 75 < piece[i3].y + 75 && event.button.button == SDL_BUTTON_LEFT && i3 != chosen_piece){
                                black_kill_passant(i3);
                                break;
                            }
                        }
                    }
                    // If there is a promotion, toggle the turn
                    if(game_prop.bpromo){
                        game_prop.w_o_b = 0;
                    }else{
                        game_prop.w_o_b = 1;
                        // Preparing for en passant
                        if(game_prop.en_passant > 0){
                            game_prop.en_passant--;
                            game_prop.passant[game_prop.prev] = game_prop.en_passant;
                        }
                    }
                }else{
                    // If the move was not valid, keep everything in its place
                    piece[chosen_piece].x = piece[chosen_piece].x_prev;
                    piece[chosen_piece].y = piece[chosen_piece].y_prev;
                    wrong = 1;
                    render();
                    SDL_Delay(1000);
                    wrong = 0;
                    game_prop.w_o_b = 0;
                }
            }
            // Reset the values of arrays
            clearing();

//#-------------------------------------------------------------------------#

            // Save the chosen piece in a temporary variable
            temp_chosen = chosen_piece;
            // If the king was checked, check if it is the end of the game or not
            if(game_prop.b_check == 1){
                chosen_piece = 4;
                // Get the valid moves of king's piece
                get_valid_moves(chosen_piece);
                // Check if it was a check mate or a normal check
                check_mate(chosen_piece);
                if(game_prop.b_check == 1 && game_prop.cover == 0 && piece[4].valid_moves_x[0] == -10){
                    game_prop.b_mate = 1;
                }
            }else{
                game_prop.b_mate = 0;
            }
            // If the king was checked, check if it is the end of the game or not
            if(game_prop.w_check == 1){
                chosen_piece = 28;
                // Get the valid moves of king's piece
                get_valid_moves(chosen_piece);
                // Check if it was a check mate or a normal check
                check_mate(chosen_piece);
                if(game_prop.w_check == 1 && game_prop.cover == 0 && piece[28].valid_moves_x[0] == -10){
                    game_prop.w_mate = 1;
                }
            }else{
                game_prop.w_mate = 0;
            }
            chosen_piece = temp_chosen;

//#-------------------------------------------------------------------------#

            // Declaration of a color
            SDL_Color color = {255, 255, 255};
            // Print the move in letters
            if(piece[chosen_piece].brow_prev == -1){
                moves_prev_surf = TTF_RenderText_Solid(font, board_moves[piece[chosen_piece].brow][piece[chosen_piece].bcol], color);
            }else{
                moves_prev_surf = TTF_RenderText_Solid(font, board_moves[piece[chosen_piece].brow_prev][piece[chosen_piece].bcol_prev], color);
            }
            moves_surf = TTF_RenderText_Solid(font, board_moves[piece[chosen_piece].brow][piece[chosen_piece].bcol], color);

//#-------------------------------------------------------------------------#

            f_save = 0;
            f_load = 0;
            f_undo = 0;
            f_redo = 0;
            // Reset the values of arrays
            clearing();
            break;
    }
}

//#-------------------------------------------------------------------------#

// Display all data which we collected on the screen:
// Render every 1000 / FPS
void render(void){
    int n, z;
    // Declaration of the SDL Variables
    SDL_Rect piece_dim[32], valid_moves[63], not_valid, undo, redo, wpromo_dim, bpromo_dim, black_turn_dim, white_turn_dim;
    SDL_Rect check_dim, b_mate_dim, w_mate_dim, save_dim, load_dim, d_pos_dim, stale_dim, moves_dim, moves_prev_dim, press_dim[4];

//----------------------------// Things to render //----------------------------//

    // Render the Board
    SDL_RenderCopy(renderer, texture[0], NULL, NULL);
//-----------------------------------------------//
    // Render the turn to be for white
    if(game_prop.w_o_b == 1 && game_prop.w_mate == 0){
        white_turn_dim.x = 600;
        white_turn_dim.y = 400;
        white_turn_dim.w = 100;
        white_turn_dim.h = 100;
        SDL_RenderCopy(renderer, white_turn_texture, NULL, &white_turn_dim);
    }
//-----------------------------------------------//
    // Render the turn to be for black
    if(game_prop.w_o_b == 0 && game_prop.b_mate == 0){
        black_turn_dim.x = 600;
        black_turn_dim.y = 100;
        black_turn_dim.w = 100;
        black_turn_dim.h = 100;
        SDL_RenderCopy(renderer, black_turn_texture, NULL, &black_turn_dim);
    }
//-----------------------------------------------//
    // Render the load icon
    load_dim.x = 600;
    load_dim.y = 0;
    load_dim.w = 100;
    load_dim.h = 100;
    SDL_RenderCopy(renderer, load_texture, NULL, &load_dim);
//-----------------------------------------------//
    // Render the save icon
    save_dim.x = 600;
    save_dim.y = 500;
    save_dim.w = 100;
    save_dim.h = 100;
    SDL_RenderCopy(renderer, save_texture, NULL, &save_dim);
//-----------------------------------------------//
    // Render the undo icon
    undo.x = 850;
    undo.y = 400;
    undo.w = 100;
    undo.h = 100;
    SDL_RenderCopy(renderer, undo_texture, NULL, &undo);
//-----------------------------------------------//
    // Render the redo icon
    redo.x = 850;
    redo.y = 500;
    redo.w = 100;
    redo.h = 100;
    SDL_RenderCopy(renderer, redo_texture, NULL, &redo);
//-----------------------------------------------//
    // Render pressing motion
    if(f_load == 1){
        press_dim[0].x = 600;
        press_dim[0].y = 0;
        press_dim[0].w = 100;
        press_dim[0].h = 100;
        SDL_RenderCopy(renderer, press_texture[0], NULL, &press_dim[0]);
    }
//-----------------------------------------------//
    // Render pressing motion
    if(f_save == 1){
        press_dim[1].x = 600;
        press_dim[1].y = 500;
        press_dim[1].w = 100;
        press_dim[1].h = 100;
        SDL_RenderCopy(renderer, press_texture[1], NULL, &press_dim[1]);
    }
//-----------------------------------------------//
    // Render pressing motion
    if(f_undo == 1){
        press_dim[2].x = 850;
        press_dim[2].y = 400;
        press_dim[2].w = 100;
        press_dim[2].h = 100;
        SDL_RenderCopy(renderer, press_texture[2], NULL, &press_dim[2]);
    }
//-----------------------------------------------//
    // Render pressing motion
    if(f_redo == 1){
        press_dim[3].x = 850;
        press_dim[3].y = 500;
        press_dim[3].w = 100;
        press_dim[3].h = 100;
        SDL_RenderCopy(renderer, press_texture[3], NULL, &press_dim[3]);
    }
//-----------------------------------------------//
    // Render the movement in letters (from the old position)
    moves_prev_texture = SDL_CreateTextureFromSurface(renderer, moves_prev_surf);
    moves_prev_dim.x = 620;
    moves_prev_dim.y = 285;
    moves_prev_dim.w = 30;
    moves_prev_dim.h = 30;
    SDL_RenderCopy(renderer, moves_prev_texture, NULL, &moves_prev_dim);
//-----------------------------------------------//
    // Render the movement in letters (to the new position)
    moves_texture = SDL_CreateTextureFromSurface(renderer, moves_surf);
    moves_dim.x = 650;
    moves_dim.y = 285;
    moves_dim.w = 30;
    moves_dim.h = 30;
    SDL_RenderCopy(renderer, moves_texture, NULL, &moves_dim);
//-----------------------------------------------//
    // If there is a normal check, render "check" word
    if(game_prop.w_check || game_prop.b_check){
        check_dim.x = 850;
        check_dim.y = 270;
        check_dim.w = 100;
        check_dim.h = 40;
        SDL_RenderCopy(renderer, check_texture, NULL, &check_dim);
    }
//-----------------------------------------------//
    // Render a menu, if there is a promotion for white
    if(game_prop.wpromo){
        wpromo_dim.x = 850;
        wpromo_dim.y = 0;
        wpromo_dim.w = 100;
        wpromo_dim.h = 300;
        SDL_RenderCopy(renderer, wpromo_texture, NULL, &wpromo_dim);
    }
//-----------------------------------------------//
    // Render a menu, if there is a promotion for black
    if(game_prop.bpromo){
        bpromo_dim.x = 850;
        bpromo_dim.y = 0;
        bpromo_dim.w = 100;
        bpromo_dim.h = 300;
        SDL_RenderCopy(renderer, bpromo_texture, NULL, &bpromo_dim);
    }
//-----------------------------------------------//
    // Render a texture for invalid moves
    if(wrong){
        not_valid.x = piece[chosen_piece].xtemp;
        not_valid.y = piece[chosen_piece].ytemp;
        not_valid.w = 75;
        not_valid.h = 75;
        SDL_RenderCopy(renderer, not_valid_texture, NULL, &not_valid);
    }
//-----------------------------------------------//
    // Render a texture for valid moves
    if(guide){
        for(z = 0; z < piece[chosen_piece].cx; z++){
            valid_moves[z].x = piece[chosen_piece].valid_moves_x[z];
            valid_moves[z].y = piece[chosen_piece].valid_moves_y[z];
            valid_moves[z].w = 75;
            valid_moves[z].h = 75;
            SDL_RenderCopy(renderer, valid_textures[z], NULL, &valid_moves[z]);
        }
    }
//-----------------------------------------------//
    // When a redo is done, reload the promoted piece (for white player)
    if(game_prop.do_bpromo){
        texture[chosen_piece + 1] = texture[12];
        game_prop.do_bpromo = 0;
    }
//-----------------------------------------------//
    // When a redo is done, reload the promoted piece (for black player)
    else if(game_prop.do_wpromo){
        texture[chosen_piece + 1] = texture[21];
        game_prop.do_wpromo = 0;
    }
//-----------------------------------------------//
    // Render the promoted pawn to a black rook
    if(game_prop.bpr){
        texture[chosen_piece + 1] = texture[1];
        game_prop.bpr = 0;
    }
//-----------------------------------------------//
    // Render the promoted pawn to a black bishop
    else if(game_prop.bpb){
        texture[chosen_piece + 1] = texture[3];
        game_prop.bpb = 0;
    }
//-----------------------------------------------//
    // Render the promoted pawn to a black knight
    else if(game_prop.bpn){
        texture[chosen_piece + 1] = texture[2];
        game_prop.bpn = 0;
    }
//-----------------------------------------------//
    // Render the promoted pawn to a black queen
    else if(game_prop.bpq){
        texture[chosen_piece + 1] = texture[4];
        game_prop.bpq = 0;
    }
//-----------------------------------------------//
    // Render the promoted pawn to a white rook
    else if(game_prop.wpr){
        texture[chosen_piece + 1] = texture[32] ;
        game_prop.wpr = 0;
    }
//-----------------------------------------------//
    // Render the promoted pawn to a white bishop
    else if(game_prop.wpb){
        texture[chosen_piece + 1] = texture[30];
        game_prop.wpb = 0;
    }
//-----------------------------------------------//
    // Render the promoted pawn to a white knight
    else if(game_prop.wpn){
        texture[chosen_piece + 1] = texture[31];
        game_prop.wpn = 0;
    }
//-----------------------------------------------//
    // Render the promoted pawn to a white queen
    else if(game_prop.wpq){
        texture[chosen_piece + 1] = texture[28];
        game_prop.wpq = 0;
    }
//-----------------------------------------------//
    // Render all piece
    for(n = 0; n < 32; n++){
        piece_dim[n].x = piece[n].x;
        piece_dim[n].y = piece[n].y;
        piece_dim[n].w = 75;
        piece_dim[n].h = 75;
        SDL_RenderCopy(renderer, texture[n + 1], NULL, &piece_dim[n]);
    }
//-----------------------------------------------//
    // If the game ends in check mate for the black, then white wins
    if(game_prop.b_mate){
        b_mate_dim.x = 0;
        b_mate_dim.y = 0;
        b_mate_dim.w = 600;
        b_mate_dim.h = 600;
        SDL_RenderCopy(renderer, b_mate_texture, NULL, &b_mate_dim);
    }
//-----------------------------------------------//
    // If the game ends in check mate for the white, then black wins
    if(game_prop.w_mate){
        w_mate_dim.x = 0;
        w_mate_dim.y = 0;
        w_mate_dim.w = 600;
        w_mate_dim.h = 600;
        SDL_RenderCopy(renderer, w_mate_texture, NULL, &w_mate_dim);
    }
//-----------------------------------------------//
    // If the game ends in a draw by dead position, render this texture
    if(game_prop.d_pos){
        d_pos_dim.x = 0;
        d_pos_dim.y = 0;
        d_pos_dim.w = 600;
        d_pos_dim.h = 600;
        SDL_RenderCopy(renderer, dead_pos_texture, NULL, &d_pos_dim);
    }
//-----------------------------------------------//
    // If the game ends in a draw by stalemate, render this texture
    if(game_prop.stale){
        stale_dim.x = 0;
        stale_dim.y = 0;
        stale_dim.w = 600;
        stale_dim.h = 600;
        SDL_RenderCopy(renderer, stale_texture, NULL, &stale_dim);
    }
//-----------------------------------------------//
    // Rendering
    SDL_RenderPresent(renderer);
}

//#-------------------------------------------------------------------------#

// Clean memory before closing the program
void destroy_window(void){
    int m;
    for(m = 0; m < 33; m++){
        // Free all images and textures
        SDL_DestroyTexture(texture[m]);
        SDL_FreeSurface(image[m]);
    }
    // Quit the images function
    IMG_Quit();
    // Quit the music player
    Mix_FreeMusic(sound[0]);
    sound[0] = NULL;
    Mix_Quit();
    // Quit the font function
    TTF_CloseFont(font);
    TTF_Quit();
    // Destroy the renderer and the window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    // Quit SDL
    SDL_Quit();
}

//#-------------------------------------------------------------------------#

int SDL_main(int argc, char *argv[]){
    // Initialize the window and renderer
    Ready_To_start = initialize_basics();

    // Close the game window
    HWND windowHandle = GetConsoleWindow();
    ShowWindow(windowHandle, SW_HIDE);
    // Initialize start state for the game
    setup();
    // Make an icon for the game
    SDL_SetWindowIcon(window, icon);

    while(Ready_To_start){
        // Get user inputs and
        process_input();
        // Wait until 300 frame per sec passed to ensure that all devices that open this game will have the same UX
        SDL_Delay(FRAME_TARGET_TIME);
        render();
    }
    // Destroy the window
    destroy_window();
    return 0;
}
//#-------------------------------------------END----------------------------------------------#
>>>>>>> 78ef9a3d624ede2677c090b3df9606c897b96ae3
