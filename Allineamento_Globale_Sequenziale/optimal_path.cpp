
#include "optimal_path.h"
#include <iostream>
#include "from_source.h"
#include "to_sink.h"
#include <algorithm>

using namespace std;

void construction_optimal_path(vector<int>& vect_ts, vector<int>& vect_fs, list<int>& scores_optimal_path, vector<int>& vect_indexes, int middle_node, int count_max) {

    auto it = find_if(vect_ts.begin(), vect_ts.end(), [middle_node](int i) {return i == middle_node; }); // utilizzo una lambda function per trovare gli indici degli elementi 
    // del vettore vect_ts che hanno lo score max, in modo tale che poi potrò risalire ai valori di score di questi nodi ottenuti dalla from source e che 
    // quindi si trovano in vect_fs.

    while (it != vect_ts.end()) {

        //se entro nel while allora vuol dire che ho trovato il primo elemento (partendo dalla fine di vect_ts) del vettore vect_ts che ha il valore uguale a middle_node e quindi questo    
        //elemento devo già aggiungerlo al cammino ottimale. se non entro nel while vuol dire che non ci sono elementi che hanno un valore pari a middle node (ma arrivato all'inizio del while la prima volta già so che ci sarà           
        //sicuramente almento un elemento che ha valore uguale a middle_node).
        vect_indexes.emplace_back(distance(vect_ts.begin(), it)); //mi salvo gli indici dei nodi della colonna centrale corrente che contengono lo score max (può essere anche solo un nodo). 
        scores_optimal_path.emplace_back(vect_fs[distance(vect_ts.begin(), it)]); //siccome ci sono più nodi con lo score massimo allora li inserisco uno per uno in scores_optimal_path partendo dal primo e aggiungendo mano mano tutti gli altri in coda.

        it = find_if(next(it), vect_ts.end(), [middle_node](int i) {return i == middle_node; }); //vado all'elemento precedente per vedere se è presente un altro nodo con lo stesso score di middle max, e così via..
    }
}

void calculate_final_optimal_path(char** M_tb, list<char>& actions_optimal_path, int A_n, int central_column_index, string& A, string& B) {

    // Adesso, partendo dall'ultimo nodo trovato dopo aver applicato la from_source le to_sink che sarà
    // praticamente quello che si troverà in posizione (a_n, central_column_index) vado a ritroso mano mano spostandomi in base ai simboli trovati (-, |, \)
    // e aggiungendo i simboli trovati in cima alla lista actions_optimal_path:
    bool source_reached = false;
    int index_row = A_n;
    int index_column = central_column_index;
    int index_character = 0;
    char action = ' ';
    list<char> temp;

    do {

        // stampo alcune info:
        /*cout << "mi trovo in calculate_final_optimal_path:" << endl;
        cout << "index_row: " << index_row << endl;
        cout << "index_column: " << index_column << endl;
        cout << "A: " << A << endl;
        cout << "B: " << B << endl;*/

        //prendo l'azione da fare:
        action = M_tb[index_row][index_column];

        temp.emplace_front(action);


        // dopodichè mi sposto in m_tb nella cella corretta in base al carattere presente in action:
        switch (action) {

        case '\\':
            ++index_character;
            --index_row; //prima decremento la var e poi la uso.
            --index_column;
            break;

        case '-':
            ++index_character;
            --index_column;
            break;

        case '|':
            ++index_character;
            --index_row;
            break;
        }

        if ((index_row == 0) && (index_column == 0)) {
            source_reached = true;
        }

    } while (!source_reached); //continuo fino a quando non arrivo al nodo source.


    //stampo temp per essere certo che tutte le azioni inserite dalla procedura "calculate_final_optimal_path" siano giuste:
    /*cout << " " << endl;
    cout << "mi trovo alla fine di calculate_final_optimal_path e stampo temp" << endl;
    for (auto it = temp.begin(); it != temp.end(); ++it) {
        cout << *it << endl;
    }
    cout << " " << endl;*/
    //cout << " " << endl;


    // Qui faccio la CONCATENAZIONE tra actions_optimal_path e temp:
    //aggiungo tutto quello presente in temp in coda alla lista chiamata "actions_optimal_path":
    actions_optimal_path.splice(actions_optimal_path.end(), temp); //dovrebbe costare un tempo o(1) in quanto già so qual è l'elemento finale della prima lista 
    // (che sarà la actions_optimal_path) a cui aggancio il primo elemento della seconda lista (che in questo caso è temp).

    //stamp actions_optimal_path:
    /*cout << " " << endl;
    cout << "mi trovo alla fine di calculate_final_optimal_path e stampo actions_optimal_path" << endl;
    for (auto it = actions_optimal_path.begin(); it != actions_optimal_path.end(); ++it) {
        cout << *it << endl;
    }
    cout << " " << endl;
    cout << " " << endl;*/
}

// La funzione calculate_middle_column si preoccuperà di calcolare gli scores di tutti i nodi 
// della colonna centrale (facendo la somma tra from_source e to_sink) che verrà salvata in vect_fs.
// - vect_fs e vect_fs verranno passati per riferimento 
// (in modo da non dover eseguire la copia degli elementi).
node calculate_index_row_middle_node(string& A, string& B, int number_rows_from_source, int number_columns_from_source, int number_rows_to_sink, int number_columns_to_sink, int source_index_row, int sink_score, int central_column_index, int a, int b, int gap, int gap_ext, bool to_sink_odd) {

    int middle_node_value = 0;
    int count_max = 0;
    node middle_node = { 0,0,0 };
    vector<int> vect_fs;
    vector<int> vect_ts;


    //passo 1: chiamo la funzione from_source:
    vect_fs = from_source(A, B, number_rows_from_source, number_columns_from_source, a, b, gap, gap_ext, source_index_row);
    /*cout << "" << endl;
    cout << "stampa della colonna centrale ottenuta dalla funzione from source:" << endl;
    for (int i = 0; i < vect_fs.size(); i++) {
        cout << "vect_fs[i]" << endl;
        cout << vect_fs[i] << endl;
    }
    cout << "" << endl;*/


    //passo 2: chiamo la funzione to_sink:
    // Se to_sink_odd == true allora decremento di 1 la varaibile central_column_index in modo da essere certo che la to_sink arrivi
    // a calcolare fino alla colonna centrale di interesse e si blocchi lì senza andare oltre.
    // Se invece to_sink_odd == false allora central_column_index - int(to_sink_odd) = 0 e quindi passerò alla to_sink solamente il valore 
    // di central_column_index
    vect_ts = to_sink(A, B, number_rows_to_sink, number_columns_to_sink, central_column_index - int(to_sink_odd), a, b, gap, gap_ext, sink_score);

    /*cout << "stampa della colonna centrale ottenuta dalla funzione to sink:" << endl;
    for (int i = 0; i < vect_ts.size(); i++) {
        cout << "vect_ts[i]" << endl;
        cout << vect_ts[i] << endl;
    }*/


    //passo 3: sommo i valori delle due colonne: (nel CODICE PARALLELO ANCHE QUESTA SOMMA POTRA' essere parallelizzata).
    //il risultato verrà memorizzato nel vettore vect_ts (quarto parametro della funzione).
    transform(vect_fs.begin(), vect_fs.end(), vect_ts.begin(), vect_ts.begin(), std::plus<int>()); //vedi se riesci a trovare qualcosa di più efficiente..
    /*cout << "" << endl;
    cout << "stampa del vettore risultato dopo aver sommato quello proveniente da from source con quello proveniente da to sink:" << endl;
    for (int i = 0; i < vect_ts.size(); i++) {
        cout << "vect_ts[i]" << endl;
        cout << vect_ts[i] << endl;
    }
    cout << "" << endl;*/

    //passo 4: trovo il VALORE del nodo migliore del vettore vect_fs perchè proprio quello sarà il middle_node:
    middle_node_value = *max_element(vect_ts.begin(), vect_ts.end()); //la funzione max_element restituisce un puntatore all'elemento più alto nel vettore dato in input, 
    //ma se metto *max_element allora la funzione mi restituirà direttamente il valore e non il puntatore. 
    //Qualora ci fossero più elementi max allora la funzione restituisce il primo elemento.
    /*cout << "" << endl;
    cout << "nodo max:" << endl;
    cout << middle_node_value << endl;*/

    //passo 5: conto quanti elementi di vect_fs hanno il punteggio max di score (sicuramente c'è ne 1 ma potrebbe essercene anche più di uno) e li aggiungo alla lista chiamata "optimal_path" 
    //che conterrà il cammino ottimale che restituirò alla fine.
    //count_max = count(vect_ts.begin(), vect_ts.end(), middle_node_value); // in realtà non serve..
    //cout << "nel vettore vect_ts ci sono " << count_max << " elementi con lo score massimo." << endl;

    //passo 6: trovo l'indice di riga del middle node (qualora ce ne fossero più di uno mi salvo in solamente quello più in basso) e lo salvo dentro middle_node_index.
    // Trova l'ultimo elemento con il valore massimo
    auto it = vect_ts.end(); // inizializza l'iterator all'ultimo elemento del vettore.
    while (*--it != middle_node_value); //continuo a spostare all'indietro l'iterator fino a quando trovo il nodo che ha esattamente un valore pari a middle_node_value.

    // Stampa l'indice dell'ultimo elemento con il valore massimo
    //cout << "L'ultimo elemento con valore massimo (" << middle_node_value << ") ha indice " << distance(vect_ts.begin(), it) << endl;
    //middle_node_index_row = distance(vect_ts.begin(), it); //mi memorizzo l'indice di riga dell'ultimo (qualora ce ne fosse più di uno) middle_node della colonna centrale corrente.
    middle_node.index_row = distance(vect_ts.begin(), it);
    middle_node.index_column = central_column_index;
    middle_node.score = vect_fs[middle_node.index_row]; //in realtà lo score che devo memorizzarmi è quello che ho calcolato solamente con la from_source 
    // e non anche con la to_sink.

    return middle_node;
}


bool adiacent_nodes(vector<int>& vect_ts, int middle_node_value, int number_node_max) {

    int index_last_node_with_middle_node_value = 0;
    auto it = vect_ts.end(); // inizializza l'iterator all'ultimo elemento del vettore.

    while (*--it != middle_node_value); // Continuo a spostare all'indietro l'iterator fino a quando trovo il nodo che ha esattamente un valore pari a middle_node_value.
    index_last_node_with_middle_node_value = distance(vect_ts.begin(), it); //mi salvo l'indice dell'ultimo nodo della colonna corrente che ha il valore di score pari a 
    // middle_node_value.
    // Adesso partendo dal nodo in posizione index_last_node_with_middle_node_value vado all'indietro di number_node_max nodi per verificare che tutti questi abbiamo il valore
    // di score pari a middle_node_value. Se così fosse allora posso essere certo che tutti questi number_node_max adiacenti hanno il valore di score più alto e quindi questa
    // funzione restituisce true, altrimenti false.
    for (int i = index_last_node_with_middle_node_value; i > number_node_max - index_last_node_with_middle_node_value; i--) {
        if (vect_ts[i] < middle_node_value) {
            return false; //vuol dire che ho trovato un nodo adiacente che non ha il valore massimo di score e quindi posso già restituire false.
        }
    }

    return true;
}

vector<int> copy_central_column(int** M, vector<int> column_res, int A_n, int k) {
    for (int i = 0; i <= A_n; i++) {
        column_res[i] = M[i][k];
    }

    return column_res;
}