
#include "print_library.h"
#include <iostream>
#include <string>

using namespace std;


// le dimensioni dei vettori e delle matrici non sono generalizzate
void print_matrix_from_source(int** print_m, string A, string B, int A_n, int central_column_index) {

    int B_n = B.length(); //

    cout << " " << endl;
    cout << " " << endl;
    cout << "stampo la parte di matrice completa ottenuta da from_source:" << endl;

    //stampo la seconda stringa (che sarebbe la b):
    cout << "        ";
    for (int j = 0; j < B_n; j++) {
        cout << B[j] << "   ";
    }
    cout << "\n  ";

    //stampo la stringa di riferimento (che sarebbe la a):
    for (int i = 0; i <= A_n; i++) {
        if (i > 0) {
            cout << A[i - 1] << " ";
        }
        //stampo tutti i valori di score dei nodi della matrice print_m:
        for (int j = 0; j <= central_column_index; j++) {
            cout.width(3); //mi serve per definire un certo spazio tra un valore e quello successivo.
            cout << print_m[i][j] << " "; //stampo il valore di score del nodio di print_m in posizione i,j.
        }
        cout << endl;
    }
    cout << endl;
}

// le dimensioni dei vettori e delle matrici non sono generalizzate
//void print_matrix_to_sink(int** print_m, string A, string B, int A_n, int B_n, int central_column_index) {
//
//    cout << " " << endl;
//    cout << " " << endl;
//    cout << "stampo la parte di matrice completa ottenuta da to_sink:" << endl;
//
//    //stampo la seconda stringa (che sarebbe la b):
//    cout << "        ";
//    for (int j = 0; j < B_n; j++) {
//        cout << B[j] << "   ";
//    }
//    cout << "\n  ";
//
//    //stampo la stringa di riferimento (che sarebbe la a):
//    for (int i = 0; i <= A_n; i++) {
//        if (i > 0) {
//            cout << A[i - 1] << " ";
//        }
//        //stampo tutti i valori di score dei nodi della matrice print_m:
//        for (int j = 0; j < B_n - central_column_index; j++) {
//            cout.width(3); //mi serve per definire un certo spazio tra un valore e quello successivo.
//            cout << print_m[i][j] << " "; //stampo il valore di score del nodio di print_m in posizione i,j.
//        }
//        cout << endl;
//    }
//    cout << endl;
//}

// le dimensioni dei vettori e delle matrici non sono generalizzate
void print_matrix_to_sink(int** print_m, string A, string B, int A_n, int B_n, const int num_column_print_m) {

    cout << " " << endl;
    cout << " " << endl;
    cout << "stampo la parte di matrice completa ottenuta da to_sink:" << endl;

    //stampo la seconda stringa (che sarebbe la b):
    cout << "        ";
    for (int j = 0; j < B_n; j++) {
        cout << B[j] << "   ";
    }
    cout << "\n  ";

    //stampo la stringa di riferimento (che sarebbe la a):
    for (int i = 0; i <= A_n; i++) {
        if (i > 0) {
            cout << A[i - 1] << " ";
        }
        //stampo tutti i valori di score dei nodi della matrice print_m:
        for (int j = num_column_print_m - 1; j < B_n; j++) {
            cout.width(3); //mi serve per definire un certo spazio tra un valore e quello successivo.
            cout << print_m[i][j] << " "; //stampo il valore di score del nodio di print_m in posizione i,j.
        }
        cout << endl;
    }
    cout << endl;
}


void print_mtx(int** m, string A, string B, int A_n, int B_n) {

    //stampo la seconda stringa (che sarebbe la a):
    cout << "        ";
    for (int j = 0; j < B_n; j++) {
        cout << B[j] << "   ";
    }
    cout << "\n  ";

    //stampo la stringa di riferimento (che sarebbe la a):
    for (int i = 0; i <= A_n; i++) {
        if (i > 0) {
            cout << A[i - 1] << " ";
        }
        //stampo tutti i valori di score dei nodi della matrice m:
        for (int j = 0; j <= B_n; j++) {
            cout.width(3); //mi serve per definire un certo spazio tra un valore e quello successivo.
            cout << m[i][j] << " "; //stampo il valore di score del nodio di m in posizione i,j.
        }
        cout << endl;
    }
    cout << endl;
}

// stampo le due stringhe di input e tutta la matrice di traceback chiamata m_tb.
void print_tb(char** m_tb, string A, string B, int A_n, int B_n) {
    cout << "        ";

    //stampo la seconda stringa (che sarebbe la a):
    for (int j = 0; j < B_n; j++) {
        cout << B[j] << "   "; //stampo il carattere b[j] della seconda stringa.
    }
    cout << "\n  ";

    //stampo la stringa di riferimento (che sarebbe la b):
    for (int i = 0; i <= A_n; i++) {
        if (i > 0) {
            cout << A[i - 1] << " "; //stampo il carattere a[i-1] della stringa di riferimento.
        }
        //stampo tutti i valori di score dei nodi della matrice m_tb:
        for (int j = 0; j <= B_n; j++) {
            cout.width(3);
            cout << m_tb[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void print_tb_right(char** m_tb, string A, string B, int A_n, int B_n) {
    cout << "        ";

    //stampo la seconda stringa (che sarebbe la a):
    for (int j = 0; j < B_n; j++) {
        cout << B[j] << "   "; //stampo il carattere b[j] della seconda stringa.
    }
    cout << "\n  ";

    //stampo la stringa di riferimento (che sarebbe la b):
    for (int i = 0; i <= A_n; i++) {
        if (i > 0) {
            cout << A[i] << " "; //stampo il carattere a[i-1] della stringa di riferimento.
        }
        //stampo tutti i valori di score dei nodi della matrice m_tb:
        for (int j = 0; j <= B_n; j++) {
            cout.width(3);
            cout << m_tb[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void print_stack(stack<pair<node, node> > s) {

    // Stampa il contenuto dello stack
    cout << "Stampa stack s corrente: " << endl;
    cout << "" << endl;

    while (!s.empty()) {
        // Otteniamo la coppia di Point in cima allo stack
        std::pair<node, node> topPair = s.top();

        // Stampa i valori di x e y per i due Point nella coppia
        cout << "Coppia source-sink corrente: " << endl;
        cout << "source_corrente: (" << topPair.first.index_row << ", " << topPair.first.index_column << ")" << endl;
        cout << "sink_corrente: (" << topPair.second.index_row << ", " << topPair.second.index_column << ")" << endl;
        cout << "" << endl;

        // Rimuoviamo la coppia di Point dallo stack
        s.pop();
    }
}