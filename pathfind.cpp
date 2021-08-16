//
// Created by Tunde Aluko on 16.07.20.
//


#include "pathfind.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using std::cout;
using std::ifstream;
using std::istringstream;
using std::sort;
using std::string;
using std::vector;
using std::abs;


// directional deltas
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

vector<State> ParseLine(string line) {
    istringstream sline(line);
    int n;
    char c;
    vector<State> row;
    while (sline >> n >> c && c == ',') {
        if (n == 0) {
            row.emplace_back(State::sEmpty);
        } else {
            row.emplace_back(State::sObstacle);
        }
    }
    return row;
}

vector<vector<State>> ReadBoardFile(string path) {
    ifstream testEnvironment (path);
    vector<vector<State>> board{};
    if (testEnvironment) {
        string line;
        while (getline(testEnvironment, line)) {
            vector<State> row = ParseLine(line);
            board.emplace_back(row);
        }
    }
    return board;
}

/**
 * Compare the F values of two cells.
 */
bool Compare(const vector<int> a, const vector<int> b) {
    int f1 = a[2] + a[3]; // f1 = g1 + h1
    int f2 = b[2] + b[3]; // f2 = g2 + h2
    return f1 > f2;
}

/**
 * Sort the two-dimensional vector of ints in descending order.
 */
void CellSort(vector<vector<int>> *v) {
    sort(v->begin(), v->end(), Compare);
}

// Calculate the manhattan distance
int Heuristic(int x1, int y1, int x2, int y2) {
    int mDist = abs(x2 - x1) + abs(y2 - y1);
    return mDist;
}

/**
 * Check that a cell is valid: on the environment, not an obstacle, and clear.
 */
bool CheckValidCell(int x, int y, vector<vector<State>> &environment) {
    bool on_environment_x = (x >= 0 && x < environment.size());
    bool on_environment_y = (y >= 0 && y < environment[0].size());
    if (on_environment_x && on_environment_y)
        return environment[x][y] == State::sEmpty;
    return false;
}

/**
 * Add a node to the open list and mark it as open.
 */
void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &openlist, vector<vector<State>> &environment) {
    // Add node to open vector, and mark environment cell as closed.
    openlist.emplace_back(vector<int>{x, y, g, h});
    environment[x][y] = State::sClosed;
}


/**
 * Expand current nodes's neighbors and add them to the open list.
 */
void ExpandNeighbors(const vector<int> &current, int goal[2], vector<vector<int>> &openlist, vector<vector<State>> &environment) {
    // Get current node's data.
    int x = current[0];
    int y = current[1];
    int g = current[2];

    // Loop through current node's potential neighbors.
    for (int i = 0; i < 4; i++) {
        int x2 = x + delta[i][0];
        int y2 = y + delta[i][1];

        // Check that the potential neighbor's x2 and y2 values are on the environment and not closed.
        if (CheckValidCell(x2, y2, environment)) {
            // Increment g value and add neighbor to open list.
            int g2 = g + 1;
            int h2 = Heuristic(x2, y2, goal[0], goal[1]);
            AddToOpen(x2, y2, g2, h2, openlist, environment);
        }
    }
}

/**
 * Implementation of A* search algorithm
 */
vector<vector<State>> Search(vector<vector<State>> environment, int init[2], int goal[2]) {
    // Create the vector of open nodes.
    vector<vector<int>> open {};

    // Initialize the starting node.
    int x = init[0];
    int y = init[1];
    int g = 0; //g-value
    int h = Heuristic(x, y, goal[0],goal[1]);
    AddToOpen(x, y, g, h, open, environment);

    while (open.size() > 0) {
        // Get the next node
        CellSort(&open);
        auto current = open.back();
        open.pop_back();
        x = current[0];
        y = current[1];
        environment[x][y] = State::sPath;

        // Check if we're done.
        if (x == goal[0] && y == goal[1]) {
            environment[init[0]][init[1]] = State::sInit;
            environment[goal[0]][goal[1]] = State::sObject;
            return environment;
        }

        // If we're not done, expand search to current node's neighbors.
        ExpandNeighbors(current, goal, open, environment);
    }

    // We've run out of new nodes to explore and haven't found a path.
    cout << "No path found!" << "\n";
    return std::vector<vector<State>>{};
}

string CellString(State cell) {
    switch(cell) {
        case State::sObstacle: return "⛰️   ";
        case State::sPath: return "🚗  ";
        case State::sInit: return "📍  ";
        case State::sObject: return "🙋  ";
        default: return "0   ";
    }
}


void PrintDriveMap(const vector<vector<State>> board) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            cout << CellString(board[i][j]);
        }
        cout << "\n";
    }
}

int main()
{
    auto driveMap = ReadBoardFile("environment.board");
    int init[2]{0, 0};
    int goal[2]{4, 5};
    auto drivePath = (Search(driveMap, init, goal));
    PrintDriveMap(drivePath);
}
