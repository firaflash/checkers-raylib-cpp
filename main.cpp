// @file checkers.cpp
// @brief contains the full code for the checkers game
// @author Firaol Feyisa 
// @date 2024-08-01
// @version 1.0
// @bug No known bugs. Rport bugs to flexflash09@gmail.com or @firaflash via all platform
// @rules you can know the rules of the movments by taping the button " help " in the game or go to the "help " function

#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include "raylib.h"

using namespace std;

const int BOARD_WIDTH = 800;
const int BOARD_HEIGHT = 800;
const int CELL_SIZE = 100;
const int CELL_CENTER_POS = CELL_SIZE / 2;
const int QORKI_SIZE = 30;

enum cellType{
    //You might want one more cell type.
    emptyCell,
    player1Qorki,
    player2Qorki,
    player1KingQorki,
    player2KingQorki
};

struct Cell{
    int row;
    int col;
    int cellType;
    int cellSize = CELL_SIZE;
};

struct Qorki{
    Color qorkiColor;
    int qorkiSize = QORKI_SIZE;
    int qorkiOrientation;
};

struct Board{
    int boardWidth;
    int boardHeight;
    Color boardColor;
};

struct Game{
    Board board;
    Cell cellInfo[8][8];
    std::string playerOneName;
    std::string playerTwoName;
    int p1;
    int p2;
    bool turn;
    int winner;
};

struct Button
{
    Rectangle rect;
    Color color;
};


/// @brief initializes the game
/// @param game the game to initialize
void initGame(Game& game);

/// @brief initializes the board
/// @param board the board to initialize
void initBoard(Board& board);

/// @brief draws the board based on board properties
/// @param board the board to draw
void drawBoard(Board board);

/// @brief gets the cell type based on the row and col on initialization
/// @param row,col the row and col to get the cell type
int initCellType(int row, int col);

/// @brief draw the cells based on their type
/// @param cell,board the cells to draw and the board in which the cells will be drawn
void drawCellsOnBoard(Cell cell[8][8]);

/// @brief getCellColor based on the cell type
/// @param cell the cell to get the color
Color getCellColor(Cell cell);

/// @brief draw qorki based on the cell type
/// @param cell the cell to draw the qorki on
void drawQorki(Cell cell[8][8]);

/// @brief returns qokri color based on cell type.
/// @param cell the cell to get the cell type from and determine the qorki color.
Color getQorkiColor(Cell cell);

/// @brief updates the game cellInfo array based on user click
/// @param cells the cell infos of the current game.
void updateGame(Cell cells[8][8], Game& game, Sound& move);

/// @brief returns the cell the user clicked on
/// @param x,y the x and y coordinates of the click
Cell getCell(int x, int y);

/// @brief moves the qorki from the selected cell to the target cell
/// @param selectedCell,targetCell,cells the selected cell, target cell and the game cell info array
void moveQorki(Cell selectedCell, Cell targetCell, Cell cells[8][8], Sound& move);

/// @brief handles the qorki actions
/// @param selectedCell,targetCell,cells the selected cell, target cell and the game cell info array
void handleQorkiMove(Cell selectedCell, Cell targetCell, Cell cells[8][8], Game& game, Sound& move);
void handleKingQorkiMove(Cell selectedCell,Cell targetCell,Cell cells[8][8], Game& game, Sound& move);

/// @brief checks if a move is a legal checkers move 
/// @param selectedCell,targetCell,cells the selected cell, target cell and the game cell info array
bool isMoveValid(Cell selectedCell, Cell targetCell, Cell cells[8][8], Game& game, Sound& move);
bool isKingMoveValid(Cell selectedCell, Cell targetCell, Cell cells[8][8], Game& game, Sound& move);

/// @brief removes the cell from the game
/// @param cell,cells the cell to remove and the game cell info array
void removeQorki(Cell cell, Cell cells[8][8]);

/// @brief Determines the winner of the game based on the current move.
/// @param selectedCell, targetCell, cells, game the selected cell, target cell, game boardand the game cell info array
void winner(Cell selectedCell, Cell targetCell, Cell cells[8][8], Game& game);

/// @brief Checks if the mouse is hovering over the button.
/// @param button The button to check.
/// @return Returns true if the mouse is over the button, false otherwise.
bool is_mouse_over_button(Button button);

/// @brief Resets the game state and restarts the game.
/// @param game, move The game object to reset.
void resetGame(Game& game, Sound& move, Sound& click);

/// @brief Handles text input for saving or loading game states.
/// @param file, type The filename to save to or load from and Specifies whether the operation is a save or load action.
void text_input(std::string& file, std::string type, Sound& click);

/// @brief Loads a previously saved game from a file.
/// @param game The game object to load the state into.
void loadgame(Game& game, Sound& click);

/// @brief Saves the current game state to a file.
/// @param game The game object whose state is to be saved.
void savegame(Game& game, Sound& click);

/// @brief Displays the help page with instructions or information.
/// @param game the game board info
void help_page(Game &game, Sound& click);

/// @brief Gets name for the players.
/// @param game, the game board info.
void player_name(Game& game, Sound& click);

/// @brief Draws the text and box drawing on the game board.
/// @param game the game board info
void drawings(Game& game);

/// @brief checks if there is on more valid moves 
/// @param selectedCell,targetCell,cells, game the selected cell, target cell, game board info and the game cell info array
bool stalmate(Cell selectedCell, Cell targetCell, Cell cells[8][8], Game& game);

int main(){
    newgame:
    Game game;
    Sound move, click;
    restart:
    initGame(game);
    initBoard(game.board);
    open:
    InitWindow((game.board.boardWidth)+300, game.board.boardHeight, "DAMA");
    InitAudioDevice();
    move = LoadSound("Game sound\\gamesound.wav");
    click = LoadSound("Game sound\\click.mp3");
    SetSoundVolume(move, 1.0f);
    SetSoundVolume(click, 1.0f);
    while (!WindowShouldClose()){ 
        BeginDrawing();
            ClearBackground(RAYWHITE);
            drawBoard(game.board); 
            drawCellsOnBoard(game.cellInfo);
            drawQorki(game.cellInfo);
            updateGame(game.cellInfo, game, move);
            drawings(game);
             
            Color turn;
            if(game.turn) {
                turn = (Color){250,91,82,255};  // SaddleBrown
            } else {
                turn = (Color){217,216,196,255};  // DarkBrown
            }

            DrawRectangle(BOARD_WIDTH + 120, 250, 30, 30, turn);
            Rectangle Hshadow = {BOARD_WIDTH + 14, 724, 280, 50};
            Button help;
            help.rect = {BOARD_WIDTH + 10, 720, 280, 50};
            help.color = GRAY;
            float roundness = 0.6f;  // Adjust roundness (0.0 for sharp corners, 1.0 for fully rounded)
            int segments = 10;       // Adjust smoothness (higher value means smoother corners)
            DrawRectangleRounded(Hshadow, roundness,segments, BLACK);
            DrawRectangleRounded(help.rect, roundness,segments, help.color);
            DrawText("HELP", BOARD_WIDTH + 40, 735, 28, BLACK);
            if((is_mouse_over_button(help)) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))){
                PlaySound(click);
                help_page(game, click);
                UnloadSound(move);
                CloseAudioDevice(); 
                goto open;
            }


            //DrawRectangle(BOARD_WIDTH + 120, 250, 30, 30, turn);
            Button restart;
            Rectangle Rshadow = {BOARD_WIDTH + 14, 659, 280, 50};
            restart.rect = {BOARD_WIDTH + 10, 655, 280, 50};
            restart.color = GRAY; 
            DrawRectangleRounded(Rshadow, roundness,segments, BLACK);
            DrawRectangleRounded(restart.rect, roundness,segments, restart.color);
            DrawText("RESTART GAME", BOARD_WIDTH + 40, 670, 28, BLACK);
            if((is_mouse_over_button(restart)) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))){
                PlaySound(click);
                WaitTime(0.5);
                UnloadSound(move);
                CloseAudioDevice(); 
                CloseWindow();
                goto restart;
            }

            //DrawRectangle(BOARD_WIDTH + 120, 250, 30, 30, turn);
            Button load;
            Rectangle Lshadow = {BOARD_WIDTH + 14, 594, 280, 50};
            load.rect = {BOARD_WIDTH + 10, 590, 280, 50};
            load.color = GRAY;
            roundness = 0.6f;
            segments = 10;
            DrawRectangleRounded(Lshadow, roundness, segments, BLACK);
            DrawRectangleRounded(load.rect, roundness, segments, load.color);
            DrawText("LOAD A GAME", BOARD_WIDTH + 40, 605, 28, BLACK);
            if((is_mouse_over_button(load)) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))){
                PlaySound(click);
                loadgame(game, click);
                cout << "Game loaded! Player one name: " << game.playerOneName << endl;
                cout << "Player one score: " << game.p1 << endl;
                UnloadSound(move);
                CloseAudioDevice(); 
                goto open;
            }

            //DrawRectangle(BOARD_WIDTH + 120, 250, 30, 30, turn);
            Button save;
            save.rect = {BOARD_WIDTH + 10, 525, 280, 50};
            save.color = GRAY;
            Rectangle Sshadow = {BOARD_WIDTH + 14, 529, 280, 50};
            DrawRectangleRounded(Sshadow, roundness, segments, BLACK);
            DrawRectangleRounded(save.rect, roundness, segments, save.color);
            DrawText("SAVE GAME", BOARD_WIDTH + 40, 540, 28, BLACK);
            if((is_mouse_over_button(save)) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))){
                PlaySound(click);
                savegame(game, click);
                cout << "Game saved!\n";
                goto quit;
            }

            //DrawRectangle(BOARD_WIDTH + 120, 250, 30, 30, turn);
            Button name;
            name.rect = {BOARD_WIDTH + 10, 460, 280, 50};
            name.color = GRAY;
            Rectangle Nshadow = {BOARD_WIDTH + 14, 464, 280, 50};
            DrawRectangleRounded(Nshadow, roundness, segments, BLACK);
            DrawRectangleRounded(name.rect, roundness ,segments , name.color);
            DrawText("ADD NAMES", BOARD_WIDTH + 40, 475, 28, BLACK);
            if((is_mouse_over_button(name)) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))){
                PlaySound(click);
                player_name(game, click);
                UnloadSound(move);
                CloseAudioDevice(); 
                goto open;
            }


            if (game.winner== 1) {
                CloseWindow();
                InitWindow(game.board.boardWidth/2, game.board.boardHeight/4, "Game Over");
                Button button1, button2;
                button1.rect = {50, 100, 115, 50};
                button2.rect = {250, 100, 100, 50};
                button1.color = SKYBLUE;
                button2.color = SKYBLUE;
                
                while (!WindowShouldClose()) {
                    BeginDrawing();
                    ClearBackground(RAYWHITE);
                    DrawText("PLAYER ONE WON", game.board.boardWidth/8 + 10, 30, 20, SKYBLUE);
                    DrawRectangle(165, 105, 4, 45, BLACK);
                    DrawRectangle(55, 150, 114, 4, BLACK);
                    DrawRectangle(350, 105, 4, 45, BLACK);
                    DrawRectangle(255, 150, 99, 4, BLACK);
                    // Draw the buttons
                    DrawRectangleRec(button1.rect, button1.color);
                    DrawText("New Game", 60, 115, 20, BLACK);

                    DrawRectangleRec(button2.rect, button2.color);
                    DrawText("Quit", 280, 115, 20, BLACK);
                    if((is_mouse_over_button(button1)) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))){
                        PlaySound(click);
                        resetGame(game, move, click);
                        UnloadSound(move);
                        CloseAudioDevice();
                        CloseWindow();
                        goto newgame;
                    }else if((is_mouse_over_button(button2)) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))){
                        PlaySound(click);
                        CloseWindow();
                        goto quit;
                    }

                    EndDrawing();
                }
            } else if (game.winner == 2) {
                CloseWindow();
                InitWindow(game.board.boardWidth/2, game.board.boardHeight/4, "Game Over");
                Button button1, button2;
                button1.rect = {50, 100, 115, 50};
                button2.rect = {250, 100, 100, 50};
                button1.color = PINK;
                button2.color = PINK;
                while (!WindowShouldClose()) {
                    BeginDrawing();
                    ClearBackground(RAYWHITE);
                    DrawText("PLAYER TWO WON", game.board.boardWidth/8 + 10, 30, 20, PINK);
                    DrawRectangle(165, 105, 4, 45, BLACK);
                    DrawRectangle(55, 150, 114, 4, BLACK);
                    DrawRectangle(350, 105, 4, 45, BLACK);
                    DrawRectangle(255, 150, 99, 4, BLACK);
                    // Draw the buttons
                    DrawRectangleRec(button1.rect, button1.color);
                    DrawText("New Game", 60, 115, 20, BLACK);

                    DrawRectangleRec(button2.rect, button2.color);
                    DrawText("Quit", 280, 115, 20, BLACK);
                    if((is_mouse_over_button(button1)) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))){
                        PlaySound(click);
                        resetGame(game, move, click);
                        UnloadSound(move);
                        CloseAudioDevice(); 
                        CloseWindow();
                        goto newgame;
                    }else if((is_mouse_over_button(button2)) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))){
                        PlaySound(click);
                        CloseWindow();
                        goto quit;
                    }

                    EndDrawing();
                }
            }
        EndDrawing();

    }
    quit:
    UnloadSound(click);
    UnloadSound(move);
    CloseAudioDevice();       

    return 0;
}

void initGame(Game& game){
    game.playerOneName = "Player 1";
    game.playerTwoName = "Player 2";
    game.p1 = 0;
    game.p2 = 0;
    game.turn = true;

    for(int row = 0; row < 8; row++){
        for(int col =0; col <8; col++){
            game.cellInfo[row][col].row = row;
            game.cellInfo[row][col].col = col;
            game.cellInfo[row][col].cellType = initCellType(row, col);
        }
    }
}

void initBoard(Board& board){
    board.boardHeight = BOARD_HEIGHT;
    board.boardWidth = BOARD_WIDTH;
    board.boardColor = RAYWHITE;
}

void drawBoard(Board board){
    DrawRectangle(0, 0, board.boardWidth, board.boardHeight, board.boardColor);
    DrawRectangle(board.boardWidth, 0, 300, board.boardHeight, WHITE);
}

int initCellType(int row, int col){
    bool isEvenRow = row % 2 == 0;
    bool isEvenCol = col % 2 == 0;
    if((isEvenRow && !isEvenCol) || (!isEvenRow && isEvenCol)){
        if(row < 3){
            return player2Qorki;
        }
        if(row > 4){
            return player1Qorki;
        }
    }
    return emptyCell;
}

void drawCellsOnBoard(Cell cell[8][8]){
    for(int row = 0; row < 8; row++)
        for(int col = 0; col < 8; col++){
            Cell currentCell = cell[row][col];
            Color cellColor = getCellColor(cell[row][col]);
            DrawRectangle(col * currentCell.cellSize, row * currentCell.cellSize, currentCell.cellSize, currentCell.cellSize, cellColor);
        }
}

Color getCellColor(Cell cell){
    bool isEvenRow = cell.row % 2 == 0;
    bool isEvenCol = cell.col % 2 == 0;
    Color darkWood = (Color){86,107,48,255};    // Dark brown for dark squares
    Color lightWood = (Color){254,255,218,255}; // Light brown for light squares

    if((isEvenRow && !isEvenCol) || (!isEvenRow && isEvenCol)){
        return darkWood;
    }
    return lightWood;
}

void drawQorki(Cell cells[8][8]){
    Qorki qorki;
    float cellCenterPos = cells[0][0].cellSize / 2;
    for(int row = 0; row < 8; row++){
        int rowPos = row * cells[0][0].cellSize;
        for(int col = 0; col < 8; col++){
            int colPos = col * cells[0][0].cellSize;
            Cell currentCell = cells[row][col];
            if(currentCell.cellType != emptyCell){
                Color qorkiColor = getQorkiColor(currentCell);
                DrawCircle(cellCenterPos + colPos, cellCenterPos + rowPos, qorki.qorkiSize,qorkiColor);
            }
        }
    }
}

Color getQorkiColor(Cell cell) {
    // Define complementary wood-like colors for the Qorki pieces
    Color player1QorkiColor = (Color){251,251,238,255};  // SaddleBrown for Player 1's Qorki
    Color player1KingColor = (Color){106,81,75,255};   // Gold for Player 1's King Qorki (brighter)
    Color player2QorkiColor = (Color){84,85,84,255}; // Tan for Player 2's Qorki
    Color player2KingColor = (Color){23,21,18,255}; // HotPink for Player 2's King Qorki (brighter)


    switch(cell.cellType) {
        case player1Qorki:
            return player1QorkiColor;
        case player1KingQorki:
            return player1KingColor;
        case player2Qorki:
            return player2QorkiColor;
        case player2KingQorki:
            return player2KingColor;
        // Other cell types, if any
        // default:
    }
}


void updateGame(Cell cells[8][8], Game& game, Sound& move){
    Cell static selectedCell;
    Cell static targetCell;

    int selectedXPos;
    int selectedYPos;

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        selectedXPos = GetMouseX();
        selectedYPos = GetMouseY();
        selectedCell = getCell(selectedXPos, selectedYPos);
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        selectedXPos = GetMouseX();
        selectedYPos = GetMouseY();
        targetCell = getCell(selectedXPos, selectedYPos);
        winner(selectedCell,targetCell,cells, game);
        if(cells[selectedCell.row][selectedCell.col].cellType == player1Qorki || cells[selectedCell.row][selectedCell.col].cellType == player2Qorki)
        handleQorkiMove(selectedCell, targetCell, cells, game, move);
        if(cells[selectedCell.row][selectedCell.col].cellType == player1KingQorki || cells[selectedCell.row][selectedCell.col].cellType == player2KingQorki)
        handleKingQorkiMove(selectedCell, targetCell, cells, game, move);
    }

}

Cell getCell(int x, int y){
    Cell c;
    c.row = y/c.cellSize;
    c.col = x/c.cellSize;
    return c;
}

void handleQorkiMove(Cell selectedCell, Cell targetCell, Cell cells[8][8], Game& game, Sound& move) {
    if(game.turn){
        if(cells[selectedCell.row][selectedCell.col].cellType == player1Qorki){
            if(isMoveValid(selectedCell,targetCell,cells, game, move)){
                moveQorki(selectedCell,targetCell,cells,move);
                game.turn = false;
            }
        }
    }else{
        if(cells[selectedCell.row][selectedCell.col].cellType == player2Qorki){
            if(isMoveValid(selectedCell,targetCell,cells, game, move)){
                moveQorki(selectedCell,targetCell,cells,move);
                game.turn = true;
            }
        }
    }
}
void handleKingQorkiMove(Cell selectedCell,Cell targetCell,Cell cells[8][8], Game& game, Sound& move){
    if(game.turn){
        if(cells[selectedCell.row][selectedCell.col].cellType == player1KingQorki){
            if(isKingMoveValid(selectedCell,targetCell,cells, game, move)){
                moveQorki(selectedCell,targetCell,cells,move);
                game.turn = false;
            }
        }
    }else{
        if(cells[selectedCell.row][selectedCell.col].cellType == player2KingQorki){
            if(isKingMoveValid(selectedCell,targetCell,cells, game,move)){
                moveQorki(selectedCell,targetCell,cells,move);
                game.turn = true;
            }
        }
    }
}

void moveQorki(Cell selectedCell, Cell targetCell, Cell cells[8][8], Sound& move){
    cells[targetCell.row][targetCell.col].cellType = cells[selectedCell.row][selectedCell.col].cellType;
    removeQorki(selectedCell, cells);
    if (targetCell.row == 0 && cells[targetCell.row][targetCell.col].cellType == player1Qorki) {
        cells[targetCell.row][targetCell.col].cellType = player1KingQorki;
    }else if (targetCell.row == 7 && cells[targetCell.row][targetCell.col].cellType == player2Qorki) {
        cells[targetCell.row][targetCell.col].cellType = player2KingQorki;
    }
    PlaySound(move);
}

void removeQorki(Cell cell, Cell cells[8][8]){
    cells[cell.row][cell.col].cellType = emptyCell;
}

bool isMoveValid(Cell selectedCell, Cell targetCell, Cell cells[8][8], Game& game, Sound& move){
    Cell turntype;
    //This is so that only not the selected type and the one that is being capured shouldnt be the same but also there king couter parts
    if(game.turn){
        turntype.cellType = player1KingQorki;
    }else{
        turntype.cellType = player2KingQorki;
    }
    // Check if the selected cell has a Qorki
    if (cells[selectedCell.row][selectedCell.col].cellType == emptyCell)
        return false;
    // Check if the target cell already has a Qorki
    if (cells[targetCell.row][targetCell.col].cellType != emptyCell){
        return false;
    }
    // Check if the selected cell and the target cell are the same
    if (selectedCell.row == targetCell.row && selectedCell.col == targetCell.col)
        return false;
    int rowdiff = targetCell.row - selectedCell.row;
    int coldiff = targetCell.col - selectedCell.col;

    // For player1Qorki, the row difference should be negative (moving upwards)
    if (cells[selectedCell.row][selectedCell.col].cellType == player2Qorki && rowdiff <= 0) {
        return false;
    }
    // For player2Qorki, the row difference should be positive (moving downwards)
    if (cells[selectedCell.row][selectedCell.col].cellType == player1Qorki && rowdiff >= 0) {
        return false;
    }
    if (abs(selectedCell.row - targetCell.row) == 1 && abs(selectedCell.col - targetCell.col) == 1) {
        return true;
    } else if(abs(selectedCell.row - targetCell.row) > 2){
        if(abs(selectedCell.col - targetCell.col) == 0){
            Cell nextcell;
            Cell guess;
            guess.row = selectedCell.row + 1;
            guess.col = selectedCell.col + 1;
            guess.cellType = cells[guess.row][guess.col].cellType;
            int midrow;
            int midcol;
            int count = 1;

            // Determine the direction of movement
            int rowDirection = (targetCell.row > selectedCell.row) ? 1 : -1;
            int colDirection = (guess.cellType != emptyCell) ? 1 : -1;

            do {
                int nextrow = selectedCell.row + 2 * rowDirection;
                int nextcol = selectedCell.col + 2 * colDirection;

                // Set the next potential cell based on direction and check for pieces
                nextcell.row = nextrow;
                nextcell.col = nextcol;
                
                // Checks if the next cell is out of the board
                if (nextcell.col < 0) {
                    return false;
                }else if (nextcell.col > 7) {
                    return false;
                }
                if (nextcell.row < 0) {
                    return false;
                }else if (nextcell.row > 7) {
                    return false;
                }

                // Calculate the intermediate cell position (the cell being captured)
                midrow = (selectedCell.row + nextcell.row) / 2;
                midcol = (selectedCell.col + nextcell.col) / 2;

                Cell removeCell;
                removeCell.row = midrow;
                removeCell.col = midcol;

                // Check if the intermediate cell has an opponent's Qorki
                if ((cells[midrow][midcol].cellType != cells[selectedCell.row][selectedCell.col].cellType) && (cells[midrow][midcol].cellType !=turntype.cellType) && (cells[midrow][midcol].cellType != emptyCell)) {
                    
                    // Capture the Qorki
                    removeQorki(removeCell, cells);

                    // Update scores based on which player performed the capture
                    if((cells[selectedCell.row][selectedCell.col].cellType == player1Qorki) || 
                    cells[selectedCell.row][selectedCell.col].cellType == player1KingQorki) {
                        game.p1++;
                    } else if((cells[selectedCell.row][selectedCell.col].cellType == player2Qorki) || 
                            cells[selectedCell.row][selectedCell.col].cellType == player2KingQorki) {
                        game.p2++;
                    }
                    winner(selectedCell,targetCell,cells, game);


                    // Move the KingQorki to the next cell
                    moveQorki(selectedCell, nextcell, cells, move);
                    
                } else {
                    // No valid capture found, move is invalid
                    return false;
                }

                // Update selectedCell to the new position for potential second capture
                if(count == 1) {
                    selectedCell = nextcell;
                    nextcell = targetCell;
                }
                colDirection = (nextcell.col > selectedCell.col) ? 1 : -1;

                count++;
            } while(count == 2);
            game.turn = !game.turn;
        }
        else{
            if(abs(rowdiff) != abs(coldiff)){
                return false;
            }
            Cell nextcell;
            int midrow;
            int midcol;
            int count = 1;
            int repet;
            if (abs(rowdiff) == 4){
                repet = 2;
            }else if(abs(rowdiff) == 6){
                repet= 3;
            }

            // Determine the direction of movement
            int rowDirection = (targetCell.row > selectedCell.row) ? 1 : -1;
            int colDirection = (targetCell.col > selectedCell.col) ? 1 : -1;

            do {
                int nextrow = selectedCell.row + 2 * rowDirection;
                int nextcol = selectedCell.col + 2 * colDirection;

                // Set the next potential cell based on direction and check for pieces
                nextcell.row = nextrow;
                nextcell.col = nextcol;

                // Calculate the intermediate cell position (the cell being captured)
                midrow = (selectedCell.row + nextcell.row) / 2;
                midcol = (selectedCell.col + nextcell.col) / 2;

                Cell removeCell;
                removeCell.row = midrow;
                removeCell.col = midcol;

                // Check if the intermediate cell has an opponent's Qorki
                if ((cells[midrow][midcol].cellType != cells[selectedCell.row][selectedCell.col].cellType) && (cells[midrow][midcol].cellType !=turntype.cellType) && (cells[midrow][midcol].cellType != emptyCell)) {
                    
                    // Capture the Qorki
                    removeQorki(removeCell, cells);

                    // Update scores based on which player performed the capture
                    if((cells[selectedCell.row][selectedCell.col].cellType == player1Qorki) || 
                    cells[selectedCell.row][selectedCell.col].cellType == player1KingQorki) {
                        game.p1++;
                    } else if((cells[selectedCell.row][selectedCell.col].cellType == player2Qorki) || 
                            cells[selectedCell.row][selectedCell.col].cellType == player2KingQorki) {
                        game.p2++;
                    }
                    winner(selectedCell,targetCell,cells, game);

                    // Move the KingQorki to the next cell
                    moveQorki(selectedCell, nextcell, cells, move);
                    
                } else {
                    // No valid capture found, move is invalid
                    return false;
                }

                // Update selectedCell to the new position for potential second capture
                if(count == 1) {
                    selectedCell = nextcell;
                    nextcell = targetCell;
                }

                count++;
            } while(count == repet);
            game.turn = !game.turn;
        }
        return false; 
    }else if (abs(selectedCell.row - targetCell.row) == 2 && abs(selectedCell.col - targetCell.col) == 2) {
        // Check the intermediate cell to see if there is an opponent's Qorki
        int midRow = (selectedCell.row + targetCell.row) / 2;
        int midCol = (selectedCell.col + targetCell.col) / 2;
        Cell removeCell;
        removeCell.row = midRow;
        removeCell.col = midCol;

        // Check if the intermediate cell has an opponent's Qorki
        if ((cells[midRow][midCol].cellType != cells[selectedCell.row][selectedCell.col].cellType) && (cells[midRow][midCol].cellType !=turntype.cellType) &&(cells[midRow][midCol].cellType != emptyCell)) {
                removeQorki(removeCell, cells);
                if((cells[selectedCell.row][selectedCell.col].cellType == player1Qorki) || cells[selectedCell.row][selectedCell.col].cellType == player1KingQorki){
                    game.p1++;
                }
                else if((cells[selectedCell.row][selectedCell.col].cellType == player2Qorki) || cells[selectedCell.row][selectedCell.col].cellType == player2KingQorki){
                    game.p2++;
                }
                winner(selectedCell,targetCell,cells, game);
                return true;   
        }
        else{
            return false;
        }
    }else{
        return false;
    }
    // implement all other conditions
    return true;
}

bool isKingMoveValid(Cell selectedCell, Cell targetCell, Cell cells[8][8], Game& game, Sound& move){
    Cell turntype;
    //This is so that only not the selected type and the one that is being capured shouldnt be the same but also there normal couter parts
    if(game.turn){
        turntype.cellType = player1Qorki;
    }else{
        turntype.cellType = player2Qorki;
    }
    // Check if the selected cell has a Qorki
    if (cells[selectedCell.row][selectedCell.col].cellType == emptyCell)
        return false;
    // Check if the target cell already has a Qorki
    if (cells[targetCell.row][targetCell.col].cellType != emptyCell){
        return false;
    } 
    // Check if the selected cell and the target cell are the same
    if (selectedCell.row == targetCell.row && selectedCell.col == targetCell.col)
        return false;
    if (abs(selectedCell.row - targetCell.row) == 1 && abs(selectedCell.col - targetCell.col) == 1) {
        return true;
    } else if (abs(selectedCell.row - targetCell.row) == 2 && abs(selectedCell.col - targetCell.col) == 2) {
        // Check the intermediate cell to see if there is an opponent's Qorki
        int midRow = (selectedCell.row + targetCell.row) / 2;
        int midCol = (selectedCell.col + targetCell.col) / 2;
        Cell removeCell;
        removeCell.row = midRow;
        removeCell.col = midCol;

        // Check if the intermediate cell has an opponent's Qorki
        if ((cells[midRow][midCol].cellType != cells[selectedCell.row][selectedCell.col].cellType) && (cells[midRow][midCol].cellType !=turntype.cellType) &&(cells[midRow][midCol].cellType != emptyCell)) {
                removeQorki(removeCell, cells);
                if((cells[selectedCell.row][selectedCell.col].cellType == player1Qorki) || cells[selectedCell.row][selectedCell.col].cellType == player1KingQorki){
                    game.p1++;
                }
                else if((cells[selectedCell.row][selectedCell.col].cellType == player2Qorki) || cells[selectedCell.row][selectedCell.col].cellType == player2KingQorki){
                    game.p2++;
                }
                winner(selectedCell,targetCell,cells, game);
                return true;   
        }else if ((cells[midRow][midCol].cellType == emptyCell)){
            return true;
        }else{
            return false;
        }
    }else if (abs(selectedCell.row - targetCell.row) > 2 ) {
        int row, col; // The direction of the row and column
        int rowdiff = targetCell.row - selectedCell.row;
        int coldiff = targetCell.col - selectedCell.col;

        if(abs(selectedCell.col - targetCell.col) == 0){
            Cell nextcell;
            Cell guess;
            int midrow;
            int midcol;
            int count = 1;

            // Determine the direction of movement
            int rowDirection = (targetCell.row > selectedCell.row) ? 1 : -1;
            if (rowDirection == 1){
                guess.row = selectedCell.row - 1;
                guess.col = selectedCell.col + 1;
                guess.cellType = cells[guess.row][guess.col].cellType;
            }
            else{
                guess.row = selectedCell.row + 1;
                guess.col = selectedCell.col + 1;
                guess.cellType = cells[guess.row][guess.col].cellType;
            }
            int colDirection = (guess.cellType != emptyCell) ? 1 : -1;

            do {
                int nextrow = selectedCell.row + 2 * rowDirection;
                int nextcol = selectedCell.col + 2 * colDirection;

                // Set the next potential cell based on direction and check for pieces
                nextcell.row = nextrow;
                nextcell.col = nextcol;

                // Checks if the next cell is out of the board
                if (nextcell.col < 0) {
                    return false;
                }else if (nextcell.col > 7) {
                    return false;
                }
                if (nextcell.row < 0) {
                    return false;
                }else if (nextcell.row > 7) {
                    return false;
                }

                if(cells[nextrow][nextcol].cellType != emptyCell){
                    return false;
                }

                // Calculate the intermediate cell position (the cell being captured)
                midrow = (selectedCell.row + nextcell.row) / 2;
                midcol = (selectedCell.col + nextcell.col) / 2;

                Cell removeCell;
                removeCell.row = midrow;
                removeCell.col = midcol;

                // Check if the intermediate cell has an opponent's Qorki
                if ((cells[midrow][midcol].cellType != cells[selectedCell.row][selectedCell.col].cellType) && (cells[midrow][midcol].cellType !=turntype.cellType) && (cells[midrow][midcol].cellType != emptyCell)) {
                    
                    // Capture the Qorki
                    removeQorki(removeCell, cells);

                    // Update scores based on which player performs the capture
                    if((cells[selectedCell.row][selectedCell.col].cellType == player1Qorki) || 
                    cells[selectedCell.row][selectedCell.col].cellType == player1KingQorki) {
                        game.p1++;
                    } else if((cells[selectedCell.row][selectedCell.col].cellType == player2Qorki) || 
                            cells[selectedCell.row][selectedCell.col].cellType == player2KingQorki) {
                        game.p2++;
                    }
                    winner(selectedCell,targetCell,cells, game);

                    // Move the KingQorki to the next cell
                    moveQorki(selectedCell, nextcell, cells, move);
                    
                } else {
                    // No valid capture found, move is invalid
                    return false;
                }

                // Update selectedCell to the new position for potential second capture
                if(count == 1) {
                    selectedCell = nextcell;
                    nextcell = targetCell;
                }
                colDirection = (nextcell.col > selectedCell.col) ? 1 : -1;
                count++;
            } while(count == 2);
            game.turn = !game.turn;
        }else if(abs(rowdiff) != abs(coldiff)){
                return false;
        }else if(abs(selectedCell.col - targetCell.col) > 2){
            Cell nextcell;
            Cell removeCell;
            int rowDirection = (targetCell.row > selectedCell.row) ? 1 : -1;
            int colDirection = (targetCell.col > selectedCell.col) ? 1 : -1;

            row = selectedCell.row + rowDirection;
            col = selectedCell.col + colDirection;

            while (row != targetCell.row && col != targetCell.col) {
                if (cells[row][col].cellType != emptyCell) {
                    removeCell.row = row;
                    removeCell.col = col;
                    if ((cells[row][col].cellType != cells[selectedCell.row][selectedCell.col].cellType) && (cells[row][col].cellType !=turntype.cellType)) {
                        if(cells[row+rowDirection][col+colDirection].cellType != emptyCell){
                            return false;
                        }
                        removeQorki(removeCell, cells);
                        // Update scores based on which player performed the capture
                        if(cells[selectedCell.row][selectedCell.col].cellType == player1KingQorki) {
                            game.p1 = game.p1 + 1;
                        } else if(cells[selectedCell.row][selectedCell.col].cellType == player2KingQorki) {
                            game.p2 = game.p2 + 1;
                        }
                        winner(selectedCell,targetCell,cells, game);
                    }
                    else if ((cells[row][col].cellType == cells[selectedCell.row][selectedCell.col].cellType) || (cells[row][col].cellType ==turntype.cellType)){
                        return false;
                    }
                }
                row += rowDirection;
                col += colDirection;
            }
            return true;
        }
        return false;
    }
    return false;
}
void winner(Cell selectedCell, Cell targetCell, Cell cells[8][8], Game& game){
    bool playeronemove = false;
    bool playertwomove = false;
    if (game.p1 == 11 && game.p2 < 11) {
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                if (cells[row][col].cellType == player2Qorki) {
                    selectedCell.row = row;
                    selectedCell.col = col;
                    selectedCell.cellType = player2Qorki;
                }
            }
        }
        targetCell.row = selectedCell.row + 1;
        targetCell.col = selectedCell.col + 1;
        targetCell.cellType = cells[targetCell.row][targetCell.col].cellType;
        if(stalmate(selectedCell,targetCell,cells, game)){
            playeronemove = true;
        }
        targetCell.row = selectedCell.row + 2;
        targetCell.col = selectedCell.col + 2;
        targetCell.cellType = cells[targetCell.row][targetCell.col].cellType;
        if(stalmate(selectedCell,targetCell,cells, game)){
            playeronemove = true;
        }
        targetCell.row = selectedCell.row + 1;
        targetCell.col = selectedCell.col - 1;
        targetCell.cellType = cells[targetCell.row][targetCell.col].cellType;
        if(stalmate(selectedCell,targetCell,cells, game)){
            playeronemove = true;
        }
        targetCell.row = selectedCell.row + 2;
        targetCell.col = selectedCell.col - 2;
        targetCell.cellType = cells[targetCell.row][targetCell.col].cellType;
        if(stalmate(selectedCell,targetCell,cells, game)){
            playeronemove = true;
        }
        if (!playeronemove) {
            game.winner = 1;  // Player 1 wins
        }
    } else if (game.p2 == 11 && game.p1 < 11) {
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                if (game.cellInfo[row][col].cellType == player1Qorki) {
                    selectedCell.row = row;
                    selectedCell.col = col;
                    selectedCell.cellType = player1Qorki;
                }
            }
        }
        targetCell.row = selectedCell.row - 1;
        targetCell.col = selectedCell.col + 1;
        targetCell.cellType = cells[targetCell.row][targetCell.col].cellType;
        if(stalmate(selectedCell,targetCell,cells, game)){
            playertwomove = true;
        }
        targetCell.row = selectedCell.row - 2;
        targetCell.col = selectedCell.col + 2;
        targetCell.cellType = cells[targetCell.row][targetCell.col].cellType;
        if(stalmate(selectedCell,targetCell,cells, game)){
            playertwomove = true;
        }
        targetCell.row = selectedCell.row - 1;
        targetCell.col = selectedCell.col - 1;
        targetCell.cellType = cells[targetCell.row][targetCell.col].cellType;
        if(stalmate(selectedCell,targetCell,cells, game)){
            playertwomove = true;
        }
        targetCell.row = selectedCell.row - 2;
        targetCell.col = selectedCell.col - 2;
        targetCell.cellType = cells[targetCell.row][targetCell.col].cellType;
        if(stalmate(selectedCell,targetCell,cells, game)){
            playertwomove = true;

        } 
        if (!playertwomove) {
            game.winner = 2;  // Player 2 wins
        }
    }
    if (game.p1 == 12) {
        game.winner = 1;  // Player One won
    } else if (game.p2 == 12) {
        game.winner = 2;  // Player Two won
    }
}
bool is_mouse_over_button(Button button){
    return CheckCollisionPointRec(GetMousePosition(), button.rect);
}
void resetGame(Game& game, Sound& move, Sound& click) {
    game.p1 = 0;
    game.p2 = 0;
    game.turn = true;
    game.winner = 0;

    // Reset any other game-related variables as needed

    // Reinitialize the game
    initGame(game);
    initBoard(game.board);

    // Reload the sound
    move = LoadSound("Game sound\\gamesound.wav");
    click = LoadSound("Game sound\\click.wav");
    SetSoundVolume(move, 1.0f);
    SetSoundVolume(click, 1.0f);
}
void text_input(std::string& file, std::string type, Sound& click){
    const int screenWidth = 800;
    const int screenHeight = 450;
    int MAX_INPUT_CHARS = 20;

    InitWindow(screenWidth, screenHeight, "Input File Name");

    char name[MAX_INPUT_CHARS + 1] = "\0";      // NOTE: One extra space required for null terminator char '\0'
    int letterCount = 0;

    Rectangle textBox = { screenWidth/2.0f - 260, 180, 520, 50 };
    bool mouseOnText = false;

    int framesCounter = 0;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (CheckCollisionPointRec(GetMousePosition(), textBox)){
            mouseOnText = true;
        }else{
            mouseOnText = false;
        }    

        if (mouseOnText)
        {
            // Set the window's cursor to the I-Beam
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            // Get char pressed (unicode character) on the queue
            int key = GetCharPressed();

            // Check if more characters have been pressed on the same frame
            while (key > 0)
            {
                // NOTE: Only allow keys in range [32..125]
                if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS))
                {
                    name[letterCount] = (char)key;
                    name[letterCount+1] = '\0'; // Add null terminator at the end of the string.
                    letterCount++;
                }

                key = GetCharPressed();  // Check next character in the queue
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                letterCount--;
                if (letterCount < 0) letterCount = 0;
                name[letterCount] = '\0';
            }
        }
        else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (mouseOnText) {
            framesCounter++;
        }else {
            framesCounter = 0;
        }
        bool isDatFile = (letterCount >= 4 &&
                  name[letterCount - 4] == '.' &&
                  name[letterCount - 3] == 'd' &&
                  name[letterCount - 2] == 'a' &&
                  name[letterCount - 1] == 't');
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Enter File Name", 310, 100, 20, GRAY);
            DrawText("Needs to be .dat format", 270, 140, 20, GRAY);

            DrawRectangleRec(textBox, LIGHTGRAY);
            if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
            else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

            DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);

            DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount, MAX_INPUT_CHARS), 290, 250, 20, DARKGRAY);

            if (mouseOnText)
            {
                if (letterCount < MAX_INPUT_CHARS)
                {
                    // Draw blinking underscore char
                    if (((framesCounter/20)%2) == 0) DrawText("_", (int)textBox.x + 8 + MeasureText(name, 40), (int)textBox.y + 12, 40, MAROON);
                }
                else DrawText("Press BACKSPACE to delete chars...", 230, 300, 20, GRAY);
            }
            file = std::string(name);
            Button button1, button2;
            button1.rect = {320, 320, 115, 50};
            button2.rect = {320, 320, 120, 50};
            button1.color = SKYBLUE;
            button2.color = PINK;
            if(type == "save"){
                DrawRectangle(435, 325, 4, 45, BLACK);
                DrawRectangle(325, 370, 114, 4, BLACK);
                DrawRectangleRec(button1.rect, button1.color);
                DrawText("Save", 355, 335, 20, BLACK);
                if (isDatFile && (is_mouse_over_button(button1)) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) {
                    PlaySound(click);
                    break;
                }else if (isDatFile && (IsKeyPressed(KEY_ENTER))) {
                    break; 
                }
            }else if(type == "load"){
                DrawRectangle(440, 325, 4, 45, BLACK);
                DrawRectangle(325, 370, 119, 4, BLACK);
                DrawRectangleRec(button2.rect, button2.color);
                DrawText("Load", 355, 335, 20, BLACK);
                if (isDatFile && (is_mouse_over_button(button2)) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) {
                    PlaySound(click);
                    break;
                }else if (isDatFile && (IsKeyPressed(KEY_ENTER))) {
                    break; 
                }
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();    // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
}
// Save game state to a file
void savegame(Game &game, Sound& click) {
    std::string type ="save";
    std::string file;
    text_input(file, type, click);
    ofstream save(file, ios::binary);

    // Check if the file opened successfully
    if (!save.is_open()) {
        cout << "Error: Unable to open file for saving." << endl;
        return;  // Avoid exit(1); just return and handle the error in calling function
    }

    // Attempt to write to the file
    save.write(reinterpret_cast<char *>(&game), sizeof(game));
    if (!save) {  // Check if the write operation failed
        cerr << "Error: Failed to save game data." << endl;
    } else {
        cout << "Game saved successfully!" << endl;
    }

    // Close the file
    save.close();
}

// Load game state from a file
void loadgame(Game& game, Sound& click) {
    std::string type ="load";
    std::string file;
    text_input(file, type, click);
    ifstream load(file, ios::binary);

    // Check if the file opened successfully
    if (!load.is_open()) {
        cerr << "Error: Unable to open file for loading." << endl;
        return;  // Avoid exit(1); just return and handle the error in calling function
    }

    // Attempt to read from the file
    load.read(reinterpret_cast<char *>(&game), sizeof(game));
    if (!load) {  // Check if the read operation failed
        cerr << "Error: Failed to load game data." << endl;
    } else {
        cout << "Game loaded successfully!" << endl;
    }

    // Close the file
    load.close();
}
void help_page(Game &game, Sound& click) {
    // Create a new window for the help page
    InitWindow((game.board.boardWidth / 2) + 90, game.board.boardHeight / (3 / 2) - 60, "Help - Checkers Game");
     
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        // Draw game instructions
        DrawText( "How To Play:",20 , 30, 35, BLACK);
        DrawText("--> Two players take turns moving their", 13, 100, 20, BLACK);
        DrawText("pieces diagonally across an 8x8 board.", 13, 120, 20, BLACK);
        DrawText("--> Each player has 12 pieces to start.", 13, 140, 20, BLACK);
        DrawText("--> Regular Pieces can only move forward ", 13, 160, 20, BLACK);
        DrawText("diagonally.", 13, 180, 20, BLACK);
        DrawText("--> king pieces can move both forward and", 13, 200, 20, BLACK);
        DrawText("backward.", 13, 220, 20, BLACK);
        DrawText("--> Kings are promoted when regular pieces", 13, 240, 20, BLACK);
        DrawText(" reach the opponent's back row.", 13 , 260 , 20 , BLACK);
        DrawText("--> To capture opponent's piece, jump over it", 13, 280, 20, BLACK);
        DrawText("diagonally into an empty space. and ", 13, 300, 20, BLACK);
        DrawText("--> Its mandatory when its possible.", 13, 320, 20, BLACK); 
        DrawText("-->The game ends when a player captures all ", 13, 340, 20, BLACK);
        DrawText("of the other pieces or blocks them from", 13, 360, 20, BLACK);
        DrawText(" making any moves.", 13, 380, 20, BLACK);
      
        

        // Draw a button to return to the game
        Button CONTINUE;
        CONTINUE.rect = { 150, 480, 150, 50 };
        CONTINUE.color = DARKGRAY;
        int segment = 10;
        float roundness = 0.6f;
        Rectangle Gshadow = {154, 484, 150, 50};
        DrawRectangleRounded(Gshadow, roundness, segment, BLACK);
        DrawRectangleRounded(CONTINUE.rect, roundness , segment, CONTINUE.color);
        DrawText("GOT IT!", 175, 495, 20, WHITE);

        // Check for mouse input to return to the game
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && is_mouse_over_button(CONTINUE)) {
            PlaySound(click);
            CloseWindow();  // Close the help window
            return;         // Exit the help page function
            
        }

        EndDrawing();
    }
       
       
     // Close the help window when done
}
void player_name(Game& game, Sound& click){
    const int screenWidth = 800;
    const int screenHeight = 450;
    int MAX_INPUT_CHARS = 14;

    InitWindow(screenWidth, screenHeight, "Enter player name");

    char p1[MAX_INPUT_CHARS + 1] = "\0"; 
    char p2[MAX_INPUT_CHARS + 1] = "\0";     // NOTE: One extra space required for null terminator char '\0'
    int p1letterCount = 0;
    int p2letterCount = 0;

    Rectangle textBox = { screenWidth/2.0f - 260, 120, 520, 50 };
    Rectangle textBox2 = { screenWidth/2.0f - 260, 220, 520, 50 };
    int mouseOnText = 0;

    int p1framesCounter = 0;
    int p2framesCounter = 0;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (CheckCollisionPointRec(GetMousePosition(), textBox)){
            // Set the window's cursor to the I-Beam
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            // Get char pressed (unicode character) on the queue
            int key = GetCharPressed();

            
            // Check if more characters have been pressed on the same frame
                while (key > 0)
                {
                    // NOTE: Only allow keys in range [32..125]
                    if ((key >= 32) && (key <= 125) && (p1letterCount < MAX_INPUT_CHARS))
                    {
                        p1[p1letterCount] = (char)key;
                        p1[p1letterCount+1] = '\0'; // Add null terminator at the end of the string.
                        p1letterCount++;
                    }

                    key = GetCharPressed();  // Check next character in the queue
                }

                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    p1letterCount--;
                    if (p1letterCount < 0) p1letterCount = 0;
                    p1[p1letterCount] = '\0';
                }
                mouseOnText = 1;
        }else if(CheckCollisionPointRec(GetMousePosition(), textBox2)){
            // Set the window's cursor to the I-Beam
                SetMouseCursor(MOUSE_CURSOR_IBEAM);

                // Get char pressed (unicode character) on the queue
                int key = GetCharPressed();
                while (key > 0)
                {
                    // NOTE: Only allow keys in range [32..125]
                    if ((key >= 32) && (key <= 125) && (p2letterCount < MAX_INPUT_CHARS))
                    {
                        p2[p2letterCount] = (char)key;
                        p2[p2letterCount+1] = '\0'; // Add null terminator at the end of the string.
                        p2letterCount++;
                    }

                    key = GetCharPressed();  // Check next character in the queue
                }

                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    p2letterCount--;
                    if (p2letterCount < 0) p2letterCount = 0;
                    p2[p2letterCount] = '\0';
                }
                mouseOnText = 2;
        }else {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);   
            mouseOnText = 0;
        }


        if (mouseOnText == 1) {
            p1framesCounter++;
        }else if(mouseOnText == 2) {
            p2framesCounter++;
        }else{
            p1framesCounter = 0;
            p2framesCounter = 0;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("PLAYER 1 NAME", 310, 95, 20, GRAY);
            DrawText("PLAYER 2 NAME", 310, 195, 20, GRAY);

            DrawRectangleRec(textBox, LIGHTGRAY);
            DrawRectangleRec(textBox2, LIGHTGRAY);
            if (mouseOnText == 1) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
            else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

            if (mouseOnText == 2) DrawRectangleLines((int)textBox2.x, (int)textBox2.y, (int)textBox2.width, (int)textBox2.height, RED);
            else DrawRectangleLines((int)textBox2.x, (int)textBox2.y, (int)textBox2.width, (int)textBox2.height, DARKGRAY);

            if (mouseOnText == 1){
                DrawText(p1, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);
            }else if(mouseOnText == 2){
                DrawText(p2, (int)textBox2.x + 5, (int)textBox2.y + 8, 40, MAROON);
            }

            if (mouseOnText == 1){
                DrawText(TextFormat("INPUT CHARS: %i/%i", p1letterCount, MAX_INPUT_CHARS), 290, 275, 20, DARKGRAY);
            }else if(mouseOnText == 2){
                DrawText(TextFormat("INPUT CHARS: %i/%i", p2letterCount, MAX_INPUT_CHARS), 290, 275, 20, DARKGRAY);
            }

            if (mouseOnText == 1 && p1letterCount < MAX_INPUT_CHARS) {
                if (((p1framesCounter/20) % 2) == 0) {
                    DrawText("_", (int)textBox.x + 8 + MeasureText(p1, 40), (int)textBox.y + 12, 40, MAROON);
                }
            }else if (mouseOnText == 2 && p2letterCount < MAX_INPUT_CHARS) {
                if (((p2framesCounter/20) % 2) == 0) {
                    DrawText("_", (int)textBox2.x + 8 + MeasureText(p2, 40), (int)textBox2.y + 12, 40, MAROON);
                }
            }
            if (mouseOnText && p1letterCount == MAX_INPUT_CHARS && p2letterCount == MAX_INPUT_CHARS) {
                DrawText("Press BACKSPACE to delete chars...", 230, 315, 20, GRAY);
            }
            Button close, save;
            save.rect = {200, 350, 115, 50};
            close.rect = {480, 350, 115, 50};
            save.color = SKYBLUE;
            close.color = PINK;
            DrawRectangle(315, 355, 4, 45, BLACK);
            DrawRectangle(205, 400, 114, 4, BLACK);
            DrawRectangle(595, 355, 4, 45, BLACK);
            DrawRectangle(485, 400, 114, 4, BLACK);
            DrawRectangleRec(save.rect, save.color);
            DrawText("Save", 232, 365, 20, BLACK);
            DrawRectangleRec(close.rect, close.color);
            DrawText("Close", 510, 365, 20, BLACK);
            DrawText("Press close if you do not want names", 205, 415, 20, GRAY);
            if ((is_mouse_over_button(save)) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) {
                PlaySound(click);
                game.playerOneName = std::string(p1);
                game.playerTwoName = std::string(p2);
                break;
            }else if ((is_mouse_over_button(close)) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) {
                PlaySound(click);
                game.playerOneName = "Player 1";
                game.playerTwoName = "Player 2";
                break; 
            }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); 
}
void drawings(Game& game){
    DrawText(TextFormat("SCORES:"), BOARD_WIDTH + 20, 20, 50, BLACK);
    DrawText(TextFormat("%s",game.playerOneName.c_str()), BOARD_WIDTH + 20, 70, 35, SKYBLUE);
    DrawText(TextFormat("%i",game.p1), BOARD_WIDTH + 20, 110, 35, PINK);
    DrawText(TextFormat("%s",game.playerTwoName.c_str()), BOARD_WIDTH + 20, 150, 35, PINK);
    DrawText(TextFormat("%i",game.p2), BOARD_WIDTH + 20, 190, 35, SKYBLUE);
    DrawText(TextFormat("Turn:"), BOARD_WIDTH + 20, 250, 30, BLACK);
    DrawText(TextFormat("DAMA"), BOARD_WIDTH + 60, 340, 60, BLACK);
}
bool stalmate(Cell selectedCell, Cell targetCell, Cell cells[8][8], Game& game){
    // Check if the selected cell has a Qorki
    if (cells[selectedCell.row][selectedCell.col].cellType == emptyCell)
        return false;
    // Check if the target cell already has a Qorki
    if (cells[targetCell.row][targetCell.col].cellType != emptyCell){
        return false;
    }
    // Check if the selected cell and the target cell are the same
    if (selectedCell.row == targetCell.row && selectedCell.col == targetCell.col)
        return false;
    int rowdiff = targetCell.row - selectedCell.row;
    int coldiff = targetCell.col - selectedCell.col;
    // To check if it is diagonal
    if(abs(rowdiff) != abs(coldiff)){
        return false;
    }

    // For player1Qorki, the row difference should be negative (moving upwards)
    if (cells[selectedCell.row][selectedCell.col].cellType == player2Qorki && rowdiff <= 0) {
        return false;
    }
    // For player2Qorki, the row difference should be positive (moving downwards)
    if (cells[selectedCell.row][selectedCell.col].cellType == player1Qorki && rowdiff >= 0) {
        return false;
    }
    if (abs(selectedCell.row - targetCell.row) == 1 && abs(selectedCell.col - targetCell.col) == 1) {
        return true;
    }else if (abs(selectedCell.row - targetCell.row) == 2 && abs(selectedCell.col - targetCell.col) == 2) {
        // Check the intermediate cell to see if there is an opponent's Qorki
        int midRow = (selectedCell.row + targetCell.row) / 2;
        int midCol = (selectedCell.col + targetCell.col) / 2;
        Cell removeCell;
        removeCell.row = midRow;
        removeCell.col = midCol;

        // Check if the intermediate cell has an opponent's Qorki
        if ((cells[midRow][midCol].cellType != cells[selectedCell.row][selectedCell.col].cellType)&&(cells[midRow][midCol].cellType != emptyCell)) {
                return true;   
        }
        else{
            return false;
        }
    }
return false;
}
// besebebu 1500 line enargew 