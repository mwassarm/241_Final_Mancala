#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <climits>

using namespace std;

// Define a struct to represent a house on the board
struct House {
    int stones; // The number of stones in the house
    bool isStore; // Whether the house is a store
    House(int s = 4, bool st = false) : stones(s), isStore(st) {} // Constructor to initialize the number of stones and whether it is a store
};

// Define a class to represent the game board
class Board {
private:
    vector<House> player1; // Houses for player 1
    vector<House> player2; // Houses for player 2
public:
    Board() {
        // Initialize the board with 4 stones in each house and set the stores
        for (int i = 0; i < 6; i++) {
            player1.push_back(House());
            player2.push_back(House());
        }
        player1.push_back(House(0, true)); // The last house for player 1 is the store
        player2.push_back(House(0, true)); // The last house for player 2 is the store
    }
    void displayBoard() {
        // Display the board
        cout << "\t" << "\t" << "\t" << "5" << "\t  " << "4" << "\t    " << "3" << "\t      " << "2" << "\t  " << "\t" << "1" << "\t  " << "0" << endl;
        cout << "\t" << "\t" << "   -------------------------------------------------------------" << endl;
        cout << "\t" << "\t" << "   |    ";
        for (int i = 5; i >= 0; i--) {
            cout << player2[i].stones << "    |    ";
        }
        cout <<  endl;

        cout << "Computers Score: " << player2.back().stones;
        cout << " |" << "-----------------------------------------------------------" << "| ";
        cout << "Your Score: " << player1.back().stones << endl;

        cout << "\t" << "\t" << "   |    ";
        for (int i = 0; i <= 5; i++) {
            cout << player1[i].stones << "    |    ";
        }
        cout << endl;
        cout << "\t" << "\t" << "   -------------------------------------------------------------" << endl;
        cout << "\t" << "\t" << "\t" << "0" << "\t  " << "1" << "\t    " << "2" << "\t      " << "3" << "\t  " << "\t" << "4" << "\t  " << "5" << endl;
    }
    bool isPlayer1Turn() {
        return true; // Player 1 always goes first
    }
    bool isGameOver() {
        // The game is over if either player's houses are all empty
        bool player1Empty = true;
        bool player2Empty = true;
        for (int i = 0; i < 6; i++) {
            if (player1[i].stones != 0) {
                player1Empty = false;
            }
            if (player2[i].stones != 0) {
                player2Empty = false;
            }
        }
        return player1Empty || player2Empty;
    }
    bool moveStones(int player, int houseIndex) {
        vector<House> &houses = (player == 1) ? player1 : player2; // Determine which player's houses to use
        House &selectedHouse = houses[houseIndex];
        if (selectedHouse.stones == 0) {
            cout << "Invalid move: Selected house is empty" << endl;
            return true;
        }
      int numStones = selectedHouse.stones;
        selectedHouse.stones = 0; // Remove all stones from the selected house
      
        int currentIndex = houseIndex + 1;
        while (numStones > 0) {
            // Loop through consecutive houses and add one stone to each
            if (currentIndex == 6 && player == 1) {
                // Skip opponent's store
                currentIndex = 0;
                continue;
            } else if (currentIndex == 13 && player == 2) {
                // Skip opponent's store
                currentIndex = 7;
                continue;
            }
            House &currentHouse = houses[currentIndex];
            currentHouse.stones++;
            numStones--;
            currentIndex++;
        }
        // Check if last stone was placed in the player's store
        if ((currentIndex == 6 && player == 1) || (currentIndex == 13 && player == 2)) {
            cout << "You get to take another turn!" << endl;
            return true;
        }
        // Check if last stone was placed in an empty house on the player's side
        if (numStones == 0 && currentIndex >= 0 && currentIndex <= 5 && houses[currentIndex].stones == 1)         {
            int oppositeIndex = 12 - currentIndex;
            int capturedStones = houses[oppositeIndex].stones;
            houses[oppositeIndex].stones = 0;
            houses[currentIndex].stones = 0;
            houses[player - 1].stones += capturedStones; // Add captured stones to player's store
        }
    }

int evaluate() {
    // Evaluation function that returns the score for the current game state
    int score = player2.back().stones - player1.back().stones;
    for (int i = 0; i < 6; i++) {
        score += player2[i].stones - player1[i].stones;
    }
    return score;
}

int minimax(int depth, int alpha, int beta, bool maximizingPlayer) {
    if (depth == 0) 
    {
        // Reached the maximum depth, evaluate the game state and return the score
        return evaluate();
    }

    // Determine which player's turn it is
    vector<House>& currentPlayerHouses = maximizingPlayer ? player2 : player1;
    vector<House>& otherPlayerHouses = maximizingPlayer ? player1 : player2;

    int bestValue = maximizingPlayer ? INT_MIN : INT_MAX;

    // Simulate all possible moves for the current player
    for (int i = 0; i < 6; i++) {
        if (currentPlayerHouses[i].stones == 0) {
            // Empty house, can't move from here
            continue;
        }

        // Make a copy of the board and apply the move
        Board nextBoard = *this;
        bool moveAgain = nextBoard.moveStones(maximizingPlayer ? 2 : 1, i);
        if (moveAgain) {
            // Player gets another turn, recursively call minimax with the same player
            int value = minimax(depth, alpha, beta, maximizingPlayer);
            if (maximizingPlayer) {
                bestValue = max(bestValue, value);
                alpha = max(alpha, bestValue);
            } else {
                bestValue = min(bestValue, value);
                beta = min(beta, bestValue);
            }
        } else {
            // Other player's turn, recursively call minimax with the other player
            int value = minimax(depth - 1, alpha, beta, !maximizingPlayer);
            if (maximizingPlayer) {
                bestValue = max(bestValue, value);
                alpha = max(alpha, bestValue);
            } else {
                bestValue = min(bestValue, value);
                beta = min(beta, bestValue);
            }
        }

        // Alpha-beta pruning
        if (beta <= alpha) {
            break;
        }
    }

    return bestValue;
}

void playComputerTurn(int difficulty) {
    // Use the minimax algorithm with alpha-beta pruning to determine the best move for the computer player
    int bestMove = -1;
    int bestValue = INT_MIN;
    for (int i = 0; i < 6; i++) 
    {
        if (player2[i].stones == 0) 
        {
            // Empty house, can't move from here
            continue;
        }

        // Make a copy of the board and apply the move
        Board nextBoard = *this;
    int value = getNextValue(nextBoard, i, difficulty, INT_MIN, INT_MAX, false);
    
    if (value > bestValue) 
    {
        bestMove = i;
        bestValue = value;
    }
  }

moveStones(2, bestMove);
cout << "The computer chooses house " << bestMove << endl;
displayBoard();

// Check for another turn
if (player2.back().stones == 1) {
    // The last stone was dropped in the computer's store, so the computer gets another turn
    cout << "The computer gets another turn!" << endl;
    playComputerTurn(difficulty);
}
}

int getNextValue(Board board, int move, int depth, int alpha, int beta, bool isMax) {
// Make the move on the board
int player = isMax ? 2 : 1;
board.moveStones(player, move);

// Check if the game is over or if the maximum recursion depth has been reached
if (board.isGameOver() || depth == 0) {
    return board.evaluateBoard();
}
// Recursively evaluate the next set of moves and determine the best value
int value;
if (isMax) {
    value = INT_MIN;
    for (int i = 0; i < 6; i++) {
        if (board.player2[i].stones == 0) {
            // Empty house, can't move from here
            continue;
        }
        value = max(value, getNextValue(board, i, depth-1, alpha, beta, false));
        alpha = max(alpha, value);
        if (beta <= alpha) {
            break;
        }
    }
} 
else {
    value = INT_MAX;
    for (int i = 0; i < 6; i++) {
        if (board.player1[i].stones == 0) {
            // Empty house, can't move from here
            continue;
        }
        value = min(value, getNextValue(board, i, depth-1, alpha, beta, true));
        beta = min(beta, value);
        if (beta <= alpha) {
            break;
        }
    }
}
return value;
}

int evaluateBoard() {
// Evaluate the board for the computer player (player 2)
return player2.back().stones - player1.back().stones;
}
void playHumanTurn() {
    // Prompt the human player for their move
    int houseIndex;
    do {
        cout << "Your turn. Enter the index of the house you want to move stones from (0-5): ";
        cin >> houseIndex;
    } while (houseIndex < 0 || houseIndex > 5 || player1[houseIndex].stones == 0);

    // Move the stones
    moveStones(1, houseIndex);
}
};
int main() {
    Board b;

    // Prompt the player to select a game difficulty
    int difficulty;
    cout << "Select game difficulty (1-5): ";
    cin >> difficulty;
    b.displayBoard();

    // Validate the difficulty level
    if (difficulty < 1 || difficulty > 5) {
        cout << "Invalid difficulty level. Exiting the game." << endl;
        return 0;
    }

    // Play the game until a player wins
    while (!b.isGameOver()) {
        // Human player's turn
        b.playHumanTurn();

        // Check if the human player won
        if (b.isGameOver()) {
            cout << "You win!" << endl;
            break;
        }
        
        // Computer player's turn
        b.playComputerTurn(difficulty);

        // Check if the computer player won
        if (b.isGameOver()) {
            cout << "The Computer wins!" << endl;
            break;
        }
    }

    return 0;
}

  
