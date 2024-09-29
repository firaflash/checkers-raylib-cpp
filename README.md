
# Raylib Checkers Game

This project is a graphical checkers game developed in C++ using Raylib. Players can enjoy a visually engaging game of checkers with sound effects, smooth animations, and various game features, such as saving/loading game states, piece movement, and winner detection. The game is built with object-oriented principles and offers both standard and king piece functionality.

## Features

- **Graphical Interface**: An interactive 8x8 checkers board with colorful cells and player pieces (qorkis) drawn using Raylib.
- **Qorki Movement**: Supports both regular and king pieces, with diagonal movements and legal move validation.
- **Multiplayer Mode**: Two players can enter their names and take turns moving their pieces across the board.
- **King Piece Movement**: Special king rules allow movement in all diagonal directions and multiple captures.
- **Sound Integration**: Sound effects for moves, captures, and game events.
- **Save & Load**: Players can save their game and load it later to continue.
- **Game Reset**: Reset the board to start a new game.
- **Winner Detection**: Automatically identifies the winner based on remaining pieces and valid moves.
- **Stalemate Detection**: Handles situations where no valid moves remain.
- **Help Page**: Displays instructions and game rules.

## Functionality

- `initGame()`: Initializes the game settings and players.
- `drawBoard()`: Draws the checkers board and its cells.
- `drawQorki()`: Renders player pieces (qorkis) based on their types (regular or king).
- `moveQorki()`: Manages the movement of pieces and captures.
- `handleQorkiMove()`: Handles move actions for both regular and king pieces.
- `savegame()` & `loadgame()`: Saves and loads the game state to/from files.
- `resetGame()`: Resets the game board for a new match.
- `isMoveValid()` & `isKingMoveValid()`: Validates moves for regular and king pieces.
- `removeQorki()`: Removes a piece after a capture.
- `winner()`: Determines the winner based on the game’s state.
- `stalmate()`: Checks for stalemates where no legal moves are possible.

## How to Run

1. Install the necessary dependencies, including Raylib and a C++ compiler (g++, Visual Studio, etc.).
2. Compile the project using the following command:
    ```bash
    g++ main.cpp -o game -lraylib
    ```
3. Run the compiled executable:
    ```bash
    ./game
    ```

## How to Play

- Start the game by entering the player names.
- Take turns moving pieces diagonally across the board. Regular pieces can only move forward, while king pieces can move in all diagonal directions.
- Capture opponent pieces by jumping over them.
- Save the game at any time or reset it for a fresh start.
- The game declares a winner when one player captures all opponent pieces or if no valid moves remain.

## Future Enhancements

- Add single-player AI mode.
- Improve the graphical interface with more advanced animations.
- Implement online multiplayer functionality.

## Contribution

If you'd like to contribute, follow these steps:

1. Fork this repository.
2. Create a new branch (`git checkout -b feature/YourFeature`).
3. Commit your changes (`git commit -m 'Add new feature'`).
4. Push to the branch (`git push origin feature/YourFeature`).
5. Open a Pull Request.

## Contact

For inquiries or feedback, feel free to reach out to me:

- Telegram: @firaflash
- Email: flexflash09@gmail.com

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

