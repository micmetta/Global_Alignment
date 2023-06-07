// allineamento_globale_sequenziale.cpp : questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
// in questa versione le modifiche apportate sono le seguenti:
// - ottimizzazione in termini di occupazione di memoria (passo da o(n*m) a o(n)). (da fare)

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <time.h>
#include <cstdlib>
#include <vector>
#include <numeric>
#include <list>
#include "print_library.h"
#include "read_string.h"
#include "alignment.h"
#include "from_source.h"
#include "optimal_path.h"
#include <stack>
#include <chrono>


using namespace std;

// Qui dichiaro globalmente tutti i cronometri che utilizzerò per calcolarmi tutto il flusso sequenziale (ovvero ts):
float cr_sequenziale_main0 = 0;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// c'era prima..
//int compute_distance(node source_current, node sink_current) {
//    return abs(source_current.index_column - sink_current.index_column); //la diff con sotto è che non c'è il +1
//}

// considera tutta la dimensione della sottomatrice (in termine di numero di nodi) che si sta considerando in quel momento.
// Esempio:
// SE source = (5, 3) e sink (7, 5)
// ALLORA (|5 - 7| + 1) * (|3 - 5| + 1) = 3 * 3 = 9
// return 9 (num. nodi totali della sottomatrice corrente).
unsigned long long compute_distance(node source_current, node sink_current) {

    /*cout << "" << endl;
    cout << "" << endl;
    cout << "source_current.index_row: " << source_current.index_row << endl;
    cout << "sink_current.index_row: " << sink_current.index_row << endl;
    cout << "source_current.index_column: " << source_current.index_column << endl;
    cout << "sink_current.index_column: " << sink_current.index_column << endl;
    cout << "d: " << (unsigned long long)(abs(source_current.index_row - sink_current.index_row) + 1) * (unsigned long long)(abs(source_current.index_column - sink_current.index_column) + 1) << endl;*/

    return ((unsigned long long)(abs(source_current.index_row - sink_current.index_row) + 1) * (unsigned long long)(abs(source_current.index_column - sink_current.index_column) + 1));
}



// Questa funzione si preoccupa di applicare l'approccio Divide et Impera per risolvere tutte le chiamate delle sottomatrici sia di sinistra che di destra 
// in maniera iterativa utilizzando una pila.
int Global_Alignment_and_Partitioning(string& A, string& B, int A_n, int B_n, int a, int b, int gap, int gap_ext, list<char>& actions_optimal_path) {

    int source_column_current = 0;
    int central_column_current = 0;
    unsigned long long d = 0; // distanza tra source_corrente e sink_corrente. E' la distanza 
    //tra source_corrente e sink_corrente IN TERMINI DI NUMERO DI COLONNE CHE CI SONO TRA LORO.
    const int k = 100; // E' il parametro che Aldinucci utilizzava per definire il livello di grana.
    // Se k == 1 allora vuol dire che vogliamo applicare il Divide et Impera fino a quando è possibile (livello di grana massima).
    // k non deve mai essere uguale o maggiore alla lunghezza della stringa B altrimenti avrai un errore.
    int number_rows_part_one = 0; // conterrà ogni volta il numero di righe che dovrà avere la sottomatrice sinistra.
    int number_columns_part_one = 0; // conterrà ogni volta il numero di colonne che dovrà avere la sottomatrice sinistra.
    int number_rows_part_two = 0; // conterrà ogni volta il numero di righe che dovrà avere la sottomatrice destra.
    int number_columns_part_two = 0; // conterrà ogni volta il numero di colonne che dovrà avere la sottomatrice destra.
    int max_iteration = 19; //usalo per testare la from_source total (mi permette di contare quante iterazioni l'algoritmo impiega prima di trovare il cammino
    // ottimale completo.
    // (tutta la prima sottomatrice per essere risolta ha bisogno di max_iteration = 10. (questo vale per l'esempio ACTAGCTAGC - TAGCTAGCTA) ).
    int iteration_current = 0; //parto da 0 perchè tutte le iterazioni vengono eseguite nel ciclo while.

    bool to_sink_odd = true; //variabile che mi permetterà di capire quale delle due funzioni presenti in "to_sink.h" devo applicare.
    string A_modified = "";
    string B_modified = "";
    stack<pair<node, node> > s; //istanzio lo stack che conterrà coppie source-sink che dovranno essere risolte.
    node initial_source = { 0,0,0 }; //nodo source iniziale.
    node initial_sink = { A_n, B_n, 0 }; //nodo sink iniziale.
    node middle_node = { 0,0,0 }; //sarà il nodo centrale che ogni volta verrà aggiornato. 


    //////// START ALGORITMO: ////////
    
    // La prima cosa da fare è inserire la coppia { initial_source, initial_sink } nello stack s:
    // inizializzo lo stack:
    s.push({ initial_source, initial_sink });

    // stampa di s
    /*cout << "s prima dell'ingresso nel ciclo while: " << endl;
    print_stack(s);
    cout << "" << endl;
    cout << "" << endl;
    cout << "" << endl;*/


    //inizio il ciclo while:
    while (!s.empty()) {
    //while (iteration_current < max_iteration) {

        iteration_current++;
        //cout << "Iterazione numero: " << iteration_current << endl;
        //cout << "" << endl;

        //Passo 0) Estraggo la coppia source_corrente - sink_corrente dalla cima di s (che quindi verrà eliminata da s).
        pair<node, node> topPair = s.top();
        s.pop(); //elimino la coppia appena poppata.

        //stampo la coppia di nodi appena estratta:
        /*cout << "source_corrente: " << "(" << topPair.first.index_row << "," << topPair.first.index_column << ")" << endl;
        cout << "score_source_corrente: " << topPair.first.score << endl;
        cout << "" << endl;
        cout << "sink_corrente: " << "(" << topPair.second.index_row << "," << topPair.second.index_column << ")" << endl;
        cout << "score_sink_corrente: " << topPair.second.score << endl;*/

        //Passo 1) if (la distanza tra la coppia source_corrente - sink_corrente è UGUALE A k):
        d = compute_distance(topPair.first, topPair.second);
        //cout << "d in Global_Alignment_and_Partitioning: " << d << endl;
        if (d > k) {

            //cout << "Sono entrato in if (d > k) (quindi scompongo ancora il problema) " << endl;

            // Passo 1.1) calcolo la nuova colonna centrale:
            // Questo controllo qui sotto viene fatto sempre non solo per calcolare l'indice della nuova colonna centrale 
            // ma anche per capire quale delle due funzioni to_sink bisognerà chiamare all'interno della funzione "calculate_index_row_middle_node".
            if (((topPair.second.index_column - topPair.first.index_column + 1) % 2) == 0) {
                central_column_current = (topPair.second.index_column - topPair.first.index_column + 1) / 2;
                to_sink_odd = true; // in questo modo dentro la funzione "calculate_index_row_middle_node" (che chiamo più giù in questo codice),
                // quando chiamerò la funzione "to_sink" in corrispondenza del
                // parametro central_column_current non gli passerò il valore di central_column_current ma gli passerò central_column_current - int(to_sink_odd) e quindi:
                // central_column_current - int(to_sink_odd) = central_column_current - 1.
                // In questo modo, anche in questo caso la to_sink riuscirà a bloccarsi esattamente in corrispondenza della colonna centrale corretta.
            }
            else {
                central_column_current = (topPair.second.index_column - topPair.first.index_column) / 2;
                to_sink_odd = false; //stessa cosa detta sopra ma questa volta il valore del parametro central_column_current sarà proprio quello di central_column_current
                // perchè chiaramente in questo caso: 
                // central_column_current - int(to_sink_odd) = central_column_current - 0 = central_column_current.
            }

            //stampo la to_sink_odd e la central_column_current appena calcolate
            /*cout << "" << endl;
            cout << "" << endl;
            cout << "to_sink_odd: " << to_sink_odd << endl;
            cout << "central_column_current: " << central_column_current << endl;
            cout << "" << endl;
            cout << "" << endl;*/


            //Passo 1.2) Suddividi la matrice in 2 parti:

            // Parte 1) Che va dalla colonna in cui si trova source_corrente a quella centrale.
            // dalla seconda iterazione in poi posso usare la formula di sotto per sapere quante righe dovrò istanziare per la parte sinistra
            number_rows_part_one = topPair.second.index_row - topPair.first.index_row; //ricorda che sempre da 0 a contare le righe nel creare la matrice 
            // e quando poi istanzierò dinamicamente la finestra il +1 verrà aggiunto direttamente nel codice della funzione che farà l'allocazione dinamica.
            number_columns_part_one = central_column_current; //anche le colonne le conto partendo sempre da 0 e quando poi istanzierò dinamicamente la finestra il +1 
            // verrà aggiunto direttamente nel codice della funzione che farà l'allocazione dinamica.

            // Parte 2) Che invece va dalla colonna centrale alla colonna in cui si trova sink_corrente.
            number_rows_part_two = topPair.second.index_row - topPair.first.index_row;
            number_columns_part_two = topPair.second.index_column; //altrimenti la toSink non funziona bene.. (se metto -1 la to_sink non arriva fino alla colonna giusta))

            /*cout << "number_rows_part_one: " << number_rows_part_one << endl;
            cout << "number_columns_part_one: " << number_columns_part_one << endl;
            cout << "" << endl;
            cout << "number_rows_part_two: " << number_rows_part_two << endl;
            cout << "number_columns_part_two: " << number_columns_part_two << endl;
            cout << "" << endl;*/

            //Passo 1.3) Trova_middle_node nella colonna centrale applicando la
            //From_Source sulla Parte 1) e la To_Sink sulla Parte 2).

            // IMPORTANTE: Prima di chiamare la calculate_index_row_middle_node devo fare in modo che le due stringhe A e B che passo ad essa 
            // siano quelle corrette altrimenti i valori che mi calcoleranno la from_source e la to_sink non saranno quelli giusti.
            // Per questo motivo utilizzo A_modified e B_modified.



            //QUESTA E' LA A_modified che c'era nel sequenziale di prima
            // ma l'ho camibiata perchè durante gli esperimenti del parallelo
            // mi sono accorto che era sbagliata..
            //A_modified = A.substr(topPair.first.index_row, topPair.second.index_row - topPair.first.index_row);
            
            // Questa è la A_modified che c'è anche nel parallelo:
            // nonostante questo ci sono delle piccole differenze con il risultato
            // che mi da il parallelo..
            if (topPair.first.index_row != topPair.second.index_row) {
                A_modified = A.substr(topPair.first.index_row, topPair.second.index_row - topPair.first.index_row);
            }
            else {
                //se topPair.first.index_row == topPair.second.index_row allora vuol dire che non posso usare la formula di sopra altrimenti
                // per A non prenderò nemmeno un carattere !!
                //A_modified = struct_pool_t->A.substr(topPair.first.index_row, topPair.second.index_row - topPair.first.index_row + 1);
                //A_modified = struct_pool_t->A[topPair.first.index_row - 1]; c'era prima
                A_modified = A[topPair.first.index_row];
            }

            
            B_modified = B.substr(topPair.first.index_column, topPair.second.index_column - topPair.first.index_column);

            /*cout << "A_modified prima di chiamare la calculate_index_row_middle_node: " << A_modified << endl;
            cout << "B_modified prima di chiamare la calculate_index_row_middle_node: " << B_modified << endl;
            cout << "" << endl;*/

            // Prende in input il parametro to_sink_odd per quello detto al Passo 1.1).
            middle_node = calculate_index_row_middle_node(A_modified, B_modified, number_rows_part_one, number_columns_part_one, number_rows_part_two, number_columns_part_two, topPair.first.score, topPair.second.score, central_column_current, a, b, gap, gap_ext, to_sink_odd);

            // stampa middle_node_index_row e middle_node.index_column (ricorda che i due valori qui sotto mi diranno quali sono le coordinate del 
            // middle_node che cercavo considerando di essere partito da 0 sia lungo le righe che lungo le colonne):
            /*cout << "middle_node.index_row: " << middle_node.index_row << endl;
            cout << "middle_node.index_column: " << middle_node.index_column << endl;
            cout << "middle_node.score: " << middle_node.score << endl;
            cout << "" << endl;*/

            // Passo 1.4) A questo punto sono certo che in middle_node ho le coordinate del nodo centrale presente nella colonna centrale corrente e quindi posso continuare 
            // con l'esecuzione delle seguenti istruzioni:
            // 
            // Aggiungo (con l’operazione push) in s queste due nuove coppie:
            // 1) middle_node, topPair.second;
            // 2) topPair.first, middle_node;

            // Prima di fare le aggiunte appena detto però, devo fare per forza questo aggiornamento di sotto 
            // altrimenti da un certo punto in poi tutte le coppie che andrò ad impilare avranno
            // le coordinate del middle_node sbagliate perchè esse non faranno più riferimento alla matrice complessiva.
            middle_node.index_row = topPair.first.index_row + middle_node.index_row;
            middle_node.index_column = topPair.first.index_column + middle_node.index_column;

            s.push({ middle_node, topPair.second });
            s.push({ topPair.first, middle_node });
            //print_stack(s); //stampo lo stack
            //cout << "" << endl;
            //cout << "" << endl;
        }
        else {
            
            //cout << "Sono entrato in else (quindi eseguo la from source total) " << endl;

            // Passo 1.1)
            //cout << "Sono entrato nell'else (quindi d <= k)" << endl;
            
            // Passo 1.1) Quando entro qui dentro vuol dire che sono arrivato sostanzialmente al caso base in cui non applico più la suddivisione mediante l'approccio Divid et Impera, questo 
            // perchè ad esempio SE k == 1 allora vuol dire che il nodo source_corrente SI TROVA NELLA COLONNA IMMEDIATAMENTE PRECEDENTE a quella in cui si trova il nodo sink_corrente 
            // e quindi in questo caso questo vuol dire che tra i due nodi non ci sono sicuramente ulteriori colonne. 
            // E' chiaro però che se k > 1 allora vuol dire che avrei potuto effettuare ancora una suddivisione con l'approccio Divid et Impera
            // ma per qualche motivo sono interessato a capire quali sono i tempi d'esecuzione con un valore di k maggiore di 1.

            // Questo vuol dire che posso eseguire direttamente la funzione from_source_total che si preoccuperà di individuare tutti gli archi migliori che mi permettono di andare 
            // dal nodo source_corrente al nodo sink_corrente.

            // IMPORTANTE: Prima di chiamare la from_source_total devo fare in modo che le due stringhe A e B che passo ad essa siano quelle corrette altrimenti i valori
            // che mi calcolerà non saranno quelli giusti.

            // Prima di chiamare la from_source_total devo controllare che SE (topPair.second.index_row - topPair.first.index_row > 0 e A_modified == "")
            // allora, poichè se continuassi normalmente, otterrei un errore nella from_source_total in quanto giustamente essa cercherà di utilizzare 
            // dei nucleotidi di A anche se quest'ultima è una stringa vuota, allora devo modificare A_modified come mostrato qui sotto per far sì che questo
            // errore non si verifichi. 

            // In realtà l'if non l'ho messo perchè basta eseguire queste istruzioni prima della chiamata della from_source_total per far sì che il programma funzioni
            // in qualsiasi caso.
            A_modified = A.substr(topPair.first.index_row, topPair.second.index_row - topPair.first.index_row);
            B_modified = B.substr(topPair.first.index_column, topPair.second.index_column - topPair.first.index_column);

            from_source_total(A_modified, B_modified, topPair.second.index_row - topPair.first.index_row, topPair.second.index_column - topPair.first.index_column, a, b, gap, gap_ext, actions_optimal_path, topPair.first.score);
        }
    }

    /*cout << "Numero di iterazioni totali effettuate: " << iteration_current << endl;
    cout << "" << endl;
    cout << "" << endl;*/

    return 0;
}


int main()
{   

    auto start0 = chrono::high_resolution_clock::now(); //start cr_sequenziale


    string A, B; // Dichiaro le due stringhe che corrisponderanno alle due sequenze da allineare.
    string A_al, B_al = ""; // Dichiaro le due stringhe finali che otterrò come risultato dall'allineamento.
    string line; // reading in data

    //bool print_mat; // variabile che uso per sapere se si vuole stampare la matrice o solamente l'allineamento delle due sequenze.
    //bool print_align; // sarà true se l'utente vuole stampare l'allineamento.
    //int align_nuc = 150; // numero di nucleotidi per riga presenti nel file di input.
    
    int a = 1; // valore della scoring matrix per i matches
    int b = -2; // valore della scoring matrix per i mismatches tra A-G e C-T (A-G = -2, C-T = -2).
    //int c = -2; // valore della scoring matrix per tutti gli altri mismatches.

    int gap = 1; // valore di penalità quando viene aperto un nuovo gap. (Devi pensare che sarà il valore che metti però con il - davanti) - 2

    int A_n;
    int B_n;
    int index_A;
    int index_B;
    float gap_ext = 1; // valore di penalità quando viene tenuto aperto un gap. (Devi pensare che sarà il valore che metti però con il - davanti) (in alcuni casi funziona meglio se lo setti a 0.5) - 1
    list<char> actions_optimal_path; //sarà la lista che conterrà gli archi di tutto il cammino miglire individuato dall'algoritmo.
    //RICORDA: la list però NON è thread safe.

    // User interface
    /*cout << endl << "Print matrices? [0/1]" << endl;
    cin >> print_mat;
    cout << "Print alignments? [0/1]" << endl;
    cin >> print_align;
    cout << "Nucleotides per row? (150 recommended)" << endl;
    cin >> align_nuc;*/

    /*print_mat = 1;
    print_align = 1;
    align_nuc = 150;*/

    cout << "==============================" << endl;

    // Leggo il file che contiene le due stringhe da allineare.
    ifstream myfile("sequences.txt");
    if (myfile.is_open()) {
        getline(myfile, line);
        A = line;
        A.erase(A.end() - 1); // rimuovo gli spazi vuoti presenti alla fine di ogni sequenza.
        getline(myfile, line);
        B = line;
        myfile.close();
        cout << "Stringa di riferimento:" << endl;
        cout << A << endl;
        cout << "Seconda stringa:" << endl;
        cout << B << endl;
    }
    else {
        cout << "Unable to open file";
        return 1;
    }

    // Mi memorizzo la lunghezza delle due sequenze da allineare.
    A_n = get_length_string(A);
    B_n = get_length_string(B);

    cout << "" << endl;
    cout << "Lunghezza stringa di riferimento:" << endl;
    cout << A_n << endl;
    cout << "Lunghezza seconda stringa" << endl;
    cout << B_n << endl; //la seconda stringa non deve avere lo spazio quando è terminata altrimenti la dimensione di questa stringa sarà quella reale +1.
    cout << "" << endl;


    // METTI IL FOR PER TROVARE IL k migliore e poi calcola il ts finale che ti servirà per calcolare la legge di Amdhal dopo
    // i test sulla stringa da 1M !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    //auto start = chrono::high_resolution_clock::now();
    //E' la procedura che si preoccuperà di eseguire l'algoritmo di allineamento globale dall'inizio alla fine utilizzando l'approccio Divide et Impera.
    Global_Alignment_and_Partitioning(A, B, A_n, B_n, a, b, gap, gap_ext, actions_optimal_path);
    //auto stop = chrono::high_resolution_clock::now();
    //auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);


    //cout << "" << endl;
    //cout << "" << endl;
    //cout << "Stampo tutti gli elementi della lista actions_optimal_path AL CONTRARIO:" << endl;
    //for (auto it = actions_optimal_path.rbegin(); it != actions_optimal_path.rend(); it++) { //con rbegin() e rend() la lettura rimane costante (time complexity O(1)).
    //    cout << *it << endl;
    //}
    //cout << "" << endl;
    //cout << "" << endl;

    ////CALCOLO L'ALLINEMANETO FINALE
    index_A = A_n - 1; //in modo tale che con l'indice di A possa posizionarmi sull'ultimo carattere della stringa A.
    index_B = B_n - 1; //in modo tale che con l'indice di B possa posizionarmi sull'ultimo carattere della stringa B.


    //cout << "index_A: " << index_A << endl;
    //cout << "index_B: " << index_B << endl;

    // La lettura del cammino ottimale viene fatta al contrario, partendo quindi dalla fine di actions_optimal_path per poi arrivare 
    // all'inizio di actions_optimal_path.
    for (auto it = actions_optimal_path.rbegin(); it != actions_optimal_path.rend(); it++) { //con rbegin() e rend() la lettura rimane costante (time complexity O(1)).
        switch (*it) {

        case '\\':
            diagonal_alignment(A[index_A], B[index_B], A_al, B_al);
            --index_A;
            --index_B;
            break;

        case '-':
            insertion_aligment(B[index_B], A_al, B_al);
            --index_B; //in questo caso incremento solo l'indice della stringa B.
            break;

        case '|':
            deletion_alignment(A[index_A], A_al, B_al);
            --index_A; //in questo caso incremento solo l'indice della stringa A.
            break;
        }

    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ////Stampo l'allineamento finale tra le due stringhe:
    /*cout << "" << endl;
    cout << "" << endl;
    cout << "A_al e B_al:" << endl;
    cout << A_al << endl;
    cout << B_al << endl;
    cout << "" << endl;
    cout << "" << endl;*/


    // Salvataggio del risultato finale dell'allineamento su file:
    ofstream file("risultato_allineamento.txt");
    if (file.is_open()) {
        file << A_al << endl;
        file << B_al << endl;
        file.close();

        //cout << "Il risultato dell'allineamento e' stato salvato correttamente." << endl;
    }
    else {
        //cout << "ERRORE: Problema nell'apertura del file di salvataggio dell'allineamento." << endl;
    }
    //////////////////////////////////////////////////////////////

    auto stop0 = chrono::high_resolution_clock::now();
    auto duration0 = chrono::duration_cast<chrono::seconds>(stop0 - start0);


    // Stampa dei parametri utilizzati.
    cout << " " << endl;
    cout << " " << endl;
    cout << "Parametri utilizzati:" << endl;
    cout << "score di un match = " << a << endl;
    cout << "score di un mismatch = " << b << endl;
    cout << "costo per aprire un gap = -" << gap << endl;
    cout << "costo per estendere un gap = -" << gap_ext << endl;
    
    cout << "" << endl;
    cout << "Lunghezza stringa di riferimento:" << endl;
    cout << A_n << endl;
    cout << "Lunghezza seconda stringa" << endl;
    cout << B_n << endl; //la seconda stringa non deve avere lo spazio quando è terminata altrimenti la dimensione di questa stringa sarà quella reale +1.
    cout << "" << endl;

    cr_sequenziale_main0 = duration0.count(); // end cr_sequenziale_main0
    cout << "************************************************" << endl;
    cout << "ts finale (s): " << cr_sequenziale_main0 << endl;
    cout << "************************************************" << endl;
    cout << "" << endl;

    return 0;
}