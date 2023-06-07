#include "read_string.h"
#include <iostream>
#include <string>

using namespace std;

char get_last_char(string str) {
    int i = str.length();
    while (str[i] == '\0') {
        i--;
    }

    return str[i];
}

char get_previous_last_char(string str) {
    int i = str.length();
    while (str[i] == '\0') {
        i--;
    }

    return str[i-1];
}

int get_length_string(string str) {
    int i = str.length();
    if (str[i - 1] == '\0') {
        i--;
    }

    return i;
}

string read_backwards_string(string str, int stop) {
    string new_string = "";

    if (stop >= 3) {
        /*cout << "str: " << str << endl;
        cout << "stop: " << stop << endl;*/

        //for (int i = str.length() - 1; i >= stop - 1; i--) { //leggo la str al contrario fino allo stop
        //    new_string = str[i] + new_string; //man mano aggiungo i caratteri letti alla stringa che poi restituirò alla fine.
        //} C'ERA PRIMA..
        //cout << "get_length_string(str) IN read_backwards_string: " << get_length_string(str) << endl;
        for (int i = get_length_string(str); i >= get_length_string(str) - stop; i--) { //leggo la str al contrario fino allo stop
            new_string = str[i] + new_string; //man mano aggiungo i caratteri letti alla stringa che poi restituirò alla fine.
        }

    }

    //prima l'else if non c'era.. l'ho aggiunto ora... (controlla se anche tutto il resto continua a funzionare.
    else if (stop == 1) {
        /*cout << "str: " << str << endl;
        cout << "stop: " << stop << endl;*/
        // c'era prima..
        //new_string = str[str.length() - 1]; //probabilmente anche qui devi usare get_length_string(str) e non str.length().
        new_string = str[get_length_string(str) - 1];
    }

    else {
        /*cout << "str: " << str << endl;
        cout << "stop: " << stop << endl;*/

        //for (int i = str.length() - 1; i > stop; i--) { //leggo la str al contrario fino allo stop
        //    new_string = str[i] + new_string; //man mano aggiungo i caratteri letti alla stringa che poi restituirò alla fine.
        //}
        //int i = str.length() - 1; c'era prima del caso di Pietro..
        int i = get_length_string(str) - 1;
        do {
            new_string = str[i] + new_string;
            --i;
            --stop;
        } while (stop > 0);

    }

    return new_string;
}

string get_last_two_chars(string str) {
    string new_string = "";

    new_string = new_string + get_previous_last_char(str);
    new_string = new_string + get_last_char(str);

    return new_string;
}


string read_backwards_string_with_p_start(string str, int start, int stop) {
    string new_string = "";

    /*cout << "str: " << str << endl;
    cout << "stop: " << stop << endl;*/

    for (int i = start - 1; i > stop; i--) { //leggo la str al contrario fino allo stop

        new_string = str[i] + new_string; //man mano aggiungo i caratteri letti alla stringa che poi restituirò alla fine.
    }

    return new_string;
}

string read_backwards_string_with_p_start_2(string str, int start, int stop) { //a differenza di read_backwards_string_with_p_start la condizione di arresto del for cambia perchè diventa  i >= stop - 1;

    string new_string = "";

    /*cout << "str: " << str << endl;
    cout << "start: " << start << endl;
    cout << "stop: " << stop << endl;*/

    for (int i = start - 1; i >= stop - 1; i--) { //leggo la str al contrario fino allo stop
        new_string = str[i] + new_string; //man mano aggiungo i caratteri letti alla stringa che poi restituirò alla fine.
    }

    return new_string;
}

string read_forwards_string_with_p_start_2(string str, int stop) {

    string new_string = "";

    /*cout << "str: " << str << endl;
    cout << "stop: " << stop << endl;*/

    //for (int i = 0; i < stop - 1; i++) { c'era prima..
    for (int i = 0; i < stop; i++) { //leggo la str dall'inizio fino allo stop
        new_string = new_string + str[i]; //man mano aggiungo i caratteri letti alla stringa che poi restituirò alla fine.
    }

    return new_string;
}

string same_string_without_first_char(string str) {
    string new_string = "";
    for (int i = 1; i < get_length_string(str); i++) {
        new_string = new_string + str[i];
    }

    return new_string;
}