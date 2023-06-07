
#ifndef READ_STRING_H
#define READ_STRING_H
#include <string>

using namespace std;

char get_last_char(string);
char get_previous_last_char(string);
int get_length_string(string);
string read_backwards_string(string, int);
string get_last_two_chars(string);
string read_backwards_string_with_p_start(string, int, int);
string read_backwards_string_with_p_start_2(string, int, int);
string read_forwards_string_with_p_start_2(string, int);
string same_string_without_first_char(string);

#endif
