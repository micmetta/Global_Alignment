
#include "alignment.h"
#include <iostream>
#include <string>

using namespace std;

void diagonal_alignment(char a, char b, string& A_al, string& B_al) {
    A_al = a + A_al;
    B_al = b + B_al;
}

void insertion_aligment(char b, string& A_al, string& B_al) {
    A_al = '-' + A_al;
    B_al = b + B_al;
}

void deletion_alignment(char a, string& A_al, string& B_al) {
    A_al = a + A_al;
    B_al = '-' + B_al;
}

// affine gap function
// la funzione sottostante si preoccupa di calcolare e restituire questo:
// int gap_aff = sigma + epsilon * (k-1) = gap + gap_ext * (lenght) [solamente che in questa formula non c'è lenght -1]
// praticamente la funzione di sotto mi calcola il costo del gap corrente. questa formula mi serve per considerare il fatto che aprire un gap avrà un costo maggiore rispetto a mantenere il gap aperto e 
// questa considerazione conviene farla perchè nella realtà è più facile che ci siano una sequenza di gaps consecutivi piuttosto che gaps sporadici non consecutivi in quanto è molto più probabile
// in natura, è molto più probabile che nelle evoluzioni di specie si perdano o si inseriscano dei blocchettini consecutivi di nucleotidi rispetto alle singole basi perse o aggiunte che però 
// si trovano in zone sparse del genoma.  

// int gap = 2; initial gap penalty. gap penalty is lower than mismatch: two sequences from same species assumed. (sarebbe sigma)
// float gap_ext = 1; bigger gap penalties for affine gap penalty (sarebbe la epsilon)
// int length = 0; questa è la lunghezza del gap ovvero porta il conto di quanti gap consecutivi sono stati aperti, sarebbe il k che ci ha detto beccuti.
int gap_affinity(int gap, int gap_ext, int& length) {
    int gap_aff = gap + (gap_ext * length);

    return gap_aff;
}

// get maximal score and trace it
//nella chiamata a questa funzione gli passo questi parametri -> max_score(scu, scd, scl, &ptr, length);
int  max_score(int up, int diag, int left, char* ptr, int& length) {
    int  max = 0;

    if (diag >= up && diag >= left) {
        max = diag; //allora vuol dire che lo score max per il nodo corrente è quello proveniente dal nodo diagonale.
        *ptr = '\\'; //mi memorizzo questo puntatore in modo da ricordarmi che per il nodo corrente il nodo migliore è stato quello diagonale (diagonale e quindi \\).
        length = 0; // reset gap-length, in questo caso resetto il la lunghezza del gap perchè in questo caso per arrivare a questo nodo la scelta migliore è usare l'arco diagonale e quindi devo solo verificare se si è verificato
        //un match o un mismatch ma non ho bisogno di fare nessuna inserzione o delezione e quindi non mi serve aumentare la lunghezza del gap.
    }
    else if (up > left) {
        max = up; //allora vuol dire che lo score max per il nodo corrente è quello proveniente dal nodo subito sopra.
        *ptr = '|'; //mi memorizzo questo puntatore in modo da ricordarmi che per il nodo corrente il nodo migliore è stato quello subito sopra di esso (verticale e quindi |).

        //ricorda di rimettere length++ se vuoi avere degli allineamenti più reali..
        //length++; // add 1 gap-length, in questo caso incremento la lunghezza del gap perchè vuol dire che per arrivare a questo nodo la scelta migliore da fare è fare una delezione.
    }
    else {
        max = left; //allora vuol dire che lo score max per il nodo corrente è quello proveniente dal nodo subito a sinistra.
        *ptr = '-'; //mi memorizzo questo puntatore in modo da ricordarmi che per il nodo corrente il nodo migliore è stato quello subito a sinistra (orizzontale e quindi -).

        //ricorda di rimettere length++ se vuoi avere degli allineamenti più reali..
        //length++; // add 1 gap-length, in questo caso incremento la lunghezza del gap perchè vuol dire che per arrivare a questo nodo la scelta migliore da fare è fare una inserzione.
    }

    return  max; //restituisco lo score max che poi salverò come valore nel nodo corrente.
}

char best_action(int up, int diag, int left) {

    char best_action_for_current_node;

    //qualora ci fossero piu' archi migliori possibili io ne memorizzero' solamente uno dando questa precedenza:
    //- prima la diagonale (se c'è).
    //- poi la inserzione (se c'è).
    //- e poi infine la delezione (se c'è).

    if (diag >= up && diag >= left) {
        best_action_for_current_node = '\\';
    }
    else if (left > up) {
        best_action_for_current_node = '-';
    }
    else {
        best_action_for_current_node = '|';
    }

    return best_action_for_current_node; //restituisco lo score max che poi salverò come valore nel nodo corrente.
}