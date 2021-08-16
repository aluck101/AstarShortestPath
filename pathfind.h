//
// Created by Tunde Aluko on 16.07.20.
//


#ifndef PATHFIND_H
#define PATHFIND_H

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

enum class State {sEmpty, sObstacle, sClosed, sPath, sInit, sObject};
vector<State> ParseLine(string line);
vector<vector<State>> ReadBoardFile(string path);
bool Compare(const vector<int> a, const vector<int> b);
void CellSort(vector<vector<int>> *v);
int Heuristic(int x1, int y1, int x2, int y2);
bool CheckValidCell(int x, int y, vector<vector<State>> &environment);
void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &openlist, vector<vector<State>> &environment);
void ExpandNeighbors(const vector<int> &current, int goal[2], vector<vector<int>> &openlist, vector<vector<State>> &environment);
vector<vector<State>> Search(vector<vector<State>> environment, int init[2], int goal[2]);
string CellString(State cell);
void PrintDriveMap(const vector<vector<State>> board);
#endif