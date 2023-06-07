
#ifndef PRINT_LIBRARY_H
#define PRINT_LIBRARY_H
#include <string>
#include <stack>
#include "optimal_path.h"

void print_matrix_from_source(int**, std::string, std::string, int, int);
void print_matrix_to_sink(int**, std::string, std::string, int, int, int);
void print_mtx(int**, std::string, std::string, int, int);
void print_tb(char**, std::string, std::string, int, int);
void print_tb_right(char**, std::string, std::string, int, int);
void print_stack(stack<pair<node, node> >);

#endif // !PRINT_LIBRARY_H

