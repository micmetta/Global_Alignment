
#ifndef ALIGNMENT_H
#define ALIGNMENT_H
#include <string>

using namespace std;

void diagonal_alignment(char, char, string&, string&);
void insertion_aligment(char, string&, string&);
void deletion_alignment(char, string&, string&);
int gap_affinity(int, int, int&);
int max_score(int, int, int, char*, int&);
char best_action(int, int, int);


#endif
