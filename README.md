# chess game
 It is an informal chess game with GUI design using SDL libraries.
 
  Contents:
 
• Game Description and Features.

• Overview of the Design.

• Design Assumption.

• Used Data Structures.

• The Main Functions and Algorithms.

• Flowcharts and Pseudocode for the Main Algorithm.

• User Manual.

• Sample Runs.

• References.

Game Description and Features:

An offline chess game for two players with a GUI design using the SDL library.
It includes all the well-known basics of a normal chess game in addition to:

• Control Device:

User can interface with a window using the mouse.

• Valid and not Valid Movements:

when the player selects a piece, all valid
moves on the board will appear as a
green square to help the user to find his
move if he was a beginner. If any player
enters any invalid move a red square
will appear on the board.

• Special Movements:

1. Castling:

special move to protect your king and activate your rook. It’s the only
time in chess you can move two pieces in one move. The king moves
two squares to the right or left and the rook moves directly to the other
side of the king.

2. En passant:

To perform this capture, you must take your opponent’s pawn as if it
had moved just one square. You move your pawn diagonally to an
adjacent square, from rank 5 to rank 6.

3. Promotion:

It happens when a pawn reaches the last square in its column then it
can be transformed into queen, rook, knight, or bishop so a short menu
appears to choose the wanted transformation from it.

• Capturing Move:

When capturing move is done the dead piece is sent to the box of dead
pieces.

• Tomb:

Where all dead pieces were sent to.

• Movement Record:

It is a written record for the movement refers to the last place and
the current place of the chosen piece.

• Background Music:

To help players to focus on the game.

• Turn Circle:

It is a circle show who can play at this moment a white
player or a black player.

• End Game:

There are three ways to end the game:

1. Draw by Stalemate.

2. Draw by Dead Position.

3. Win or Lose by Checkmate.

• Undo and Redo:

players can use undo until they reach the first move in the game
including all promotions, checks, castling, en passant, and
capturing movements. Redo must be done after undo is used.
Players can resume the game in any chosen move between undo
and redo.

• Save and Load:

the moves can be saved at any time in the game with all of its
variables (undo and redo data) when the user left click on the
save icon and load it again when the user left click on load
icon.

Overview of Design:

• Using procedural programming due to using functions which are
constructed in C language.

• Using the SDL libraries to make the Graphical User Interface, then
started to initialize the game window. Initializing the initial position of
the pieces and the board, then rendering it on the game window.

• Using global structures to hold the properties of each piece, so we can
call or edit them at any time, also using global variables, so we can deal
with the code as one object.

• The game loop starting with the process input to specify which click
event happened and then rendering any changes for the game by the rate
of 300 fps. And when the user presses the escape button or the close
window button the game shutdown.

• Control the clicks and key events in the game using “SDL_WaitEvent”
function to wait for the button pressing event to stand out, so that there is
no CPU high usage.

• Depends on three events: mouse button down, mouse button up, mouse
motion, handling them by the coordinates of the mouse and binding them
to the position of pieces and buttons on the screen to identify which one is
selected.

• Control the movement of the pieces by a function for every piece type in
the game, in addition to special movements like castling, promotion and
en passant, and handling the check case by the main function and other
helping ones, also stalemate and dead position each one has its function
to check it.

• UI/UX: A responsive design that is made in a uniquely using illustrator
and photoshop and helps the user to find what he desires in a chess game
as undo and redo features, the valid moves for every piece, save and load
feature, and a place for dead pieces, In addition to a piece of relaxing
music to help the users to play in a special atmosphere.

Design Assumptions:

• Undo and redo can be called at any time with no restrictions, even if the
player used the save or load he can use them. Just can’t use it when the
promotion menu arises. Also, with the array of undo and redo we refer to
a kill by 1, and promotion by 2, and castling by 3.

• The white player always starts the game, And each move will be toggled.

• Refer to the board as a matrix of 8 rows and 8 columns.

• Refer to the pieces in the board as numbers like the black horse is -4 and
the white horse is 4. Also, the white pieces are positive numbers the black
ones are negative numbers. For improving this idea, by enumerating the
number to abbreviations that can be easily known as (black queen = bq =
-2) and so on.

• Castling Movement: castling may be done only if the king has never
moved, the rook involved has never moved, the squares between the king
and the rook involved are unoccupied, the king is not in check, and the
king does not cross over or end on a square attacked by an enemy piece.

• En Passant Movement: The capturing pawn must have advanced exactly
three ranks to perform this move. The captured pawn must have moved
two squares in one move, landing right next to the capturing pawn. The
en passant capture must be performed on the turn immediately after the
pawn being captured moves. If the player does not capture en passant on
that turn, they no longer can do it later.

• Promotion: there may be more than one queen, bishop, knight, or rook in
the game due to promotion.

• The square size of the board is 75 pixels so when math and coordinates
are involved, we divide the coordinates of the mouse moving on the game
window by 75. When we need to know the row and the column at which
the piece lies, we divide by 75, taking the integers advantage of removing
decimal number, then multiply it by 75.

• A draw by stalemate happens when the player who needs to move has no
legal moves and his king is not in check.

• The piece combinations that lead to a draw by insufficient material:
o King vs. king

o King and bishop vs. king

o King and knight vs. king

o King and bishop vs. king and bishop of the same colour as the
opponent’s bishop

Used Data Structure:

1. An array of surfaces SDL_SURFACE *images[33] to put images on it.

2. An array of textures SDL_TEXTURES *textures[33] to enable images to
be rendered.

3. Firstly the array of structure “piece[32]” which save some properties for
each piece as:

• x, y coordinates for the current and previous positions.

• Row and column number in the board for the current and previous
positions.

• Type of each piece.

• An array for valid moves which is split into two arrays one for x
coordinates and the other for y coordinates.

• An array for valid cover moves which is split into two arrays one
for x coordinates and the other for y coordinates. (cover moves are
the moves which cover the king from an attack).

• An indicate to the piece to know if it was dead or not.

4. Two arrays die_x[63] and die_y[63] to put the x and y coordinates in it
for the possible kills of the valid movements.

5. Undo and redo array which records the whole game as:

• The chosen piece for each play in an array, its previous movement
which recorded in two arrays one for x coordinates and the other
for y coordinates.

• The previous row and column numbers for the chosen piece in the
board which is recorded in two arrays one for row number and the
other for column number.

• The dead piece for each play in an array, its movement which
recorded in two arrays one for x coordinates and the other for y
coordinates.

• The previous last row and column numbers for the dead piece in
the board which is recorded in two arrays one for row number and
the other for column number.

• Vice versa for redo arrays.

• An array for specifying the result of this move if it was a kill,
promotion, or castling. “compare [63]”

• An array for specifying the result of this move if it was a check,
checkmate, draw by stalemate, or draw by dead position.
“comp_mate [63]”

• First_move [63]: An array for specifying if this move was the first
one or not which helps in castling situations.

6. Two arrays die_x [63] and die_y[63] to put the x and y coordinates in it
for the possible kills of the valid movements.

7. Two arrays line_moves_x [63] and line_moves_y [63] to store x and y
coordinates of the line between the king and the threatening piece.

8. Two arrays valid_line_x [63] and valid_line_y[63] to store x and y
coordinates of the matching squares between the line between the king
and the threatening piece and the valid moves of the ally piece.

9. Two dimensions array board [8][8] to ease tracing the pieces movement
on the board and distinguishing between the ally pieces and the enemy
pieces for the current state.

10. Two dimensions array board_types [8][8] to ease tracing the pieces
movement on the board and distinguishing between the ally pieces and
the enemy pieces and the type of each piece for the current state.

11. Two dimensions array board_moves [8][8] to store the location as
(char... num) for each piece.

12. Piece_dim [63] Array to specify the dimensions of the pieces and board
on the window by x, y coordinates and its width and height.

Main Functions and Algorithms:

1. The initializing function Setup():

Is used to prepare the game for the start state; load images and music, set
the initial x, y coordinates for the pieces and its type.
2. The controller function process input():

Is used to take inputs from the user and send the changes to the render
function and if we go in detail with it, we will go to say that it contains a
lot of sub-functions as it directs the game to undo moves, redo moves, save
and load the game, of course, if the user chooses it, in addition to a function
to check the validity of the moves.

3. Moves collector function get_valid_moves():
It collects all valid moves for the chosen piece at the current state to check
later if the input move is valid or not according to its data.

4. Updater functions; White_move() & black_move():

It updates the coordinates, rows, columns, the board array elements and
types if the chosen move is valid.

5. Warning function Check_king_att():

It compares between the position of the opposite king and the futurism
valid moves for the last chosen piece. If they equal, then sent the order to
the render function to show that the king is checked.

6. Ending functions:

• check_mate():

It activates when the king was checked and it for loop on all allies’ pieces
to check if one of them can cover the king by comparing between the
futurism valid moves for the piece and the coordinates of the squares on
the line between the king and the threatening piece.

• Stalemate():

It checks if the king has no valid moves, but it’s not checked, and all allies’
pieces have no valid moves on their turn by using get_valid_moves ().

Flowcharts for the Main Algorithm:

The whole flowchart link: https://miro.com/app/board/o9J_lX_-
7Uk=/?moveToWidget=3074457353619579443&cot=14

• game_loop() flowchart link: https://miro.com/app/board/o9J_lX_-
7Uk=/?moveToWidget=3074457353634148906&cot=14

• process_input() flowchart link: https://miro.com/app/board/o9J_lX_-
7Uk=/?moveToWidget=3074457353634149103&cot=14

• black_move() flowchart link: https://miro.com/app/board/o9J_lX_-
7Uk=/?moveToWidget=3074457353634001303&cot=14

• white_move() flowchart link: https://miro.com/app/board/o9J_lX_-
7Uk=/?moveToWidget=3074457353634001475&cot=14

• get_valid_moves() flowchart link: https://miro.com/app/board/o9J_lX_-
7Uk=/?moveToWidget=3074457353634001267&cot=14
• check_king_att() flowchart link: https://miro.com/app/board/o9J_lX_-
7Uk=/?moveToWidget=3074457353634001236&cot=14

• check_mate() flowchart link: https://miro.com/app/board/o9J_lX_-
7Uk=/?moveToWidget=3074457353634001148&cot=14

• stalemate() flowchart link: https://miro.com/app/board/o9J_lX_-
7Uk=/?moveToWidget=3074457353666283425&cot=14

User Manual:

• By holding the left click of the mouse on the desired piece, the user
can move the piece (according to the turn) at any place as long as it is
a valid move and in the board, then release the button to place the
piece where he wanted.

• The valid moves are marked with a green colour on the valid squares.

• If the move is invalid, the square where the user wanted to put the
piece, will be marked with a red colour.

• The user can press on the save button in the bottom of the window to
save the game, and press on the load button in the top of the window
to load the game at any time after saving it.

• By pressing on the undo or redo icon in the bottom right of the
window, the user can undo or redo one move or more. And start to
play at any state he wants.

• When there is a check for the king, the user can see in the right middle
of the window a word which indicates that there is a check.

• When there is a checkmate, stalemate, or draw by dead position the
game will end with a texture which determines who is the winner, and
if there is a draw, it will tell.

Sample Runs:

• Undo & Redo Animation, follow the link for a gif: Undo & Redo.

• En Passant Situation, follow the link for a gif: En Passant.

• Promotion Animation, follow the link for a gif: Promotion.

• Castling Animation, follow the link for a gif: Castling.

• Check Situation, follow the link for a gif: Check.

• Checkmate Situation, follow the link for a gif: Checkmate.

• Stalemate Situation, follow the link for a gif: Stalemate.

• Dead Position Situation (King and Bishop VS King), follow the link for a
gif: Dead Position 1.

• Dead Position Situation (King and Bishop VS King and Bishop on the
same square colour) , follow the link for a gif: Dead Position 2.

References:
• SDL 2 documentation link: https://wiki.libsdl.org/

• SDL 2 library download link: https://libsdl.org/download-2.0.php

• SDL Image documentation link:

https://www.libsdl.org/projects/SDL_image/docs/index.html

• SDL Image librar download link:

https://www.libsdl.org/projects/SDL_image/

• SDL Mixer documentation link:

https://www.libsdl.org/projects/SDL_mixer/docs/index.html

• SDL Mixer library download link:

https://libsdl.org/projects/SDL_mixer/

• SDL TTF documentation link:

https://www.libsdl.org/projects/SDL_ttf/docs/index.html

• SDL TTF library download link: https://www.libsdl.org/projects/SDL_ttf/ 
