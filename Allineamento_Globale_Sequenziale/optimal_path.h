#ifndef OPTIMAL_PATH_H
#define OPTIMAL_PATH_H
#include <string>
#include <vector>
#include <list>

using namespace std;

struct node {
    int index_row;
    int index_column;
    int score;
};

void construction_optimal_path(vector<int>&, vector<int>&, list<int>&, vector<int>&, int, int);
void calculate_final_optimal_path(char**, list<char>&, int, int, string&, string&);
node calculate_index_row_middle_node(string&, string&, int, int, int, int, int, int, int, int, int, int, int, bool);
bool adiacent_nodes(vector<int>&, int, int);
vector<int> copy_central_column(int**, vector<int>, int, int);

#endif
