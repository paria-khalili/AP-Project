// Replace this with your actual name and student number before submission:
// Paria_Khalili_123456789_Project_Phase1.cpp

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>

using namespace std;

const int BOARD_SIZE = 8; // 8x8 board for Othello

// Converts column letter (A–H / a–h) to 0–7 index
int colCharToIndex(char c) {
    if (c >= 'A' && c <= 'H') return c - 'A';
    if (c >= 'a' && c <= 'h') return c - 'a';
    return -1;
}

// ------------------ CLASS: Board ------------------
class Board {
private:
    vector<vector<char>> grid;

public:
    Board() {
        grid = vector<vector<char>>(BOARD_SIZE, vector<char>(BOARD_SIZE, '.'));
    }

    void initialize() {
        for (auto& row : grid) fill(row.begin(), row.end(), '.');
        grid[3][3] = 'W';
        grid[3][4] = 'B';
        grid[4][3] = 'B';
        grid[4][4] = 'W';
    }

    void print() const {
        cout << "  A B C D E F G H\n";
        for (int i = 0; i < BOARD_SIZE; ++i) {
            cout << i + 1 << " ";
            for (int j = 0; j < BOARD_SIZE; ++j)
                cout << grid[i][j] << " ";
            cout << "\n";
        }
    }

    bool placePiece(int row, int col, char player) {
        if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
            return false;
        if (grid[row][col] != '.')
            return false;
        grid[row][col] = player;
        return true;
    }

    void setCell(int row, int col, char val) {
        grid[row][col] = val;
    }

    char getCell(int row, int col) const {
        return grid[row][col];
    }

    const vector<vector<char>>& getGrid() const {
        return grid;
    }
};

// ------------------ CLASS: Player ------------------
class Player {
private:
    char color;

public:
    Player(char c) : color(c) {}

    char getColor() const {
        return color;
    }
};

// ------------------ CLASS: Game ------------------
class Game {
private:
    Board board;
    Player black{'B'};
    Player white{'W'};
    char currentTurn = 'B';

public:
    void start() {
        string input;
        cout << "Welcome to Othello Phase 1!\n";
        while (true) {
            cout << "Enter command: ";
            getline(cin, input);
            if (input.empty()) continue;

            string cmd, arg1, arg2;
            parseInput(input, cmd, arg1, arg2);

            if (cmd == "new") {
                board.initialize();
                currentTurn = 'B';
                board.print();
                cout << "Player Turn: " << currentTurn << "\n";
            }
            else if (cmd == "place") {
                handlePlace(arg1);
            }
            else if (cmd == "save") {
                if (arg1.empty()) {
                    cout << "Please provide filename. Usage: save filename.oth\n";
                    continue;
                }
                saveToFile(arg1);
            }
            else if (cmd == "load") {
                if (arg1.empty()) {
                    cout << "Please provide filename. Usage: load filename.oth\n";
                    continue;
                }
                if (!loadFromFile(arg1)) {
                    cout << "Error loading file.\n";
                } else {
                    board.print();
                    cout << "Player Turn: " << currentTurn << "\n";
                }
            }
            else if (cmd == "exit") {
                cout << "Exiting the game.\n";
                break;
            }
            else {
                cout << "Unknown command.\n";
            }
        }
    }

private:
    void switchTurn() {
        currentTurn = (currentTurn == 'B') ? 'W' : 'B';
    }

    void parseInput(const string& input, string& cmd, string& arg1, string& arg2) {
        istringstream iss(input);
        iss >> cmd >> arg1 >> arg2;
    }

    void handlePlace(const string& arg1) {
        if (arg1.empty() || arg1.length() != 2) {
            cout << "Invalid place format. Use format like D4.\n";
            return;
        }

        int col = colCharToIndex(arg1[0]);
        int row = arg1[1] - '1';

        if (col == -1 || row < 0 || row >= BOARD_SIZE) {
            cout << "Invalid coordinates. Use A1 to H8.\n";
            return;
        }

        if (board.placePiece(row, col, currentTurn)) {
            board.print();
            switchTurn();
            cout << "Player Turn: " << currentTurn << "\n";
        } else {
            cout << "Invalid move. Cell is not empty or out of bounds.\n";
        }
    }

    void saveToFile(const string& filename) {
        ofstream out(filename);
        if (!out) {
            cout << "Failed to open file for writing.\n";
            return;
        }

        for (const auto& row : board.getGrid()) {
            for (char cell : row)
                out << cell;
            out << '\n';
        }
        out << currentTurn << '\n';
        out.close();
        cout << "Game saved to " << filename << "\n";
    }

    bool loadFromFile(const string& filename) {
        ifstream in(filename);
        if (!in) return false;

        vector<vector<char>> newGrid(BOARD_SIZE, vector<char>(BOARD_SIZE));
        string line;

        for (int i = 0; i < BOARD_SIZE; ++i) {
            if (!getline(in, line) || line.size() < BOARD_SIZE) return false;
            for (int j = 0; j < BOARD_SIZE; ++j)
                newGrid[i][j] = line[j];
        }

        if (!getline(in, line) || line.empty()) return false;
        char turn = line[0];
        if (turn != 'B' && turn != 'W') return false;

        for (int i = 0; i < BOARD_SIZE; ++i)
            for (int j = 0; j < BOARD_SIZE; ++j)
                board.setCell(i, j, newGrid[i][j]);

        currentTurn = turn;
        return true;
    }
};

// ------------------ MAIN ENTRY POINT ------------------
int main() {
    Game game;
    game.start();
    return 0;
}
