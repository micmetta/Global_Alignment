
#include "from_source.h"
#include "alignment.h"
#include "print_library.h"
#include "optimal_path.h"
#include <iostream>
#include <string>
#include <vector>
#include <list>

using namespace std;


vector<int> from_source(string A, string B, int A_n, int central_column_index, int a, int b, int gap, int gap_ext, int middle_node_value_after_first_subdivision) {

    //passo 1: Alloco la memoria per la matrice necessaria per calcolare la from_source con costo o(2*n).
    // la matrice M avrà a_n + 1 righe e central_column_index + 1 colonne.
    int** M = new int* [A_n + 1]; //dichiaro m come una matrice dove ogni riga avrà dimensione pari a 2.
    const int num_column = 2;
    vector<int> column_res(A_n + 1);

    //int** print_m = new int* [A_n + 1]; //da togliere perchè mi serve solo per la stampa..

    int x = 0, y = 0;
    int scu, scd, scl; // scores for respectively cell above, diagonal and left (questi sono rispettivamente i valori di score della cella superiore, cella in diagonale e della cella a sinistra rispetto ovviamente alla cella corrente che stiamo considerando.
    char ptr, nuc;
    int k = 1; //mi servirà dopo come indice per sapere in quale colonna della matrice devo scrivere gli score della nuova colonna e quindi di conseguenza mi dirà anche qual è la colonna da cui devo partire per calcolare gli scores della nuova colonna.
    int length = 0; // initial gap length (questa è la lunghezza del gap ovvero porta il conto di quanti gap consecutivi sono stati aperti, sarebbe il k che ci ha detto beccuti)

    // creo la matrice di scoring:
    const int  s[4][4] = {
                            { a,  b,  b, b },
                            { b,  a,  b, b },
                            { b,  b,  a, b },
                            { b,  b,  b, a }
    };


    for (int i = 0; i <= A_n; i++) {
        M[i] = new int[num_column]; //qui inserisco le colonne di M e dico che esse saranno sempre e soltanto 2. 
    }

    //da togliere..
    //for (int i = 0; i <= A_n; i++) {
    //    print_m[i] = new int[central_column_index + 1]; //qui inserisco le colonne di print_m e dico che esse saranno sempre e soltanto 2. 
    //}


    // devi fare in modo che il source venga aggiornato correttamente quando faccio la prima chiamata alla sottomatrice di destra (e in realta' questo vale anche per tutte le altre sotto-chiamate che faccio per 
    // completare il lato destro. Per il lato sinistro invece, il source deve essere settato sempre uguale a 0 perche' esso corrispondera' sempre al source iniziale per qualsiasi sotto-chiamata del lato 
    // sinistro.

    // Lo stesso ragionamento scritto sopra devi farlo per il nodo sink per la parte sinistra perche' ogni volta dopo aver trovato il middle node nella matrice di sinistra il nodo sink assume proprio il valore
    // del nodo middle trovato al termine della chiamata precedente. In questo modo per la matrice di sinistra quando dovrai trovare il nuovo middle node, la from source partira' sempre dallo stesso source
    // che sara' sempre 0, mentre il nodo sink avra' il valore che aveva il middle node alla fine della chiamata precedente, e così via..


    //passo 2: riempio la prima riga della matrice M.
    M[0][0] = middle_node_value_after_first_subdivision; // inserisco prima il valore del source che dipenderà dal valore di middle_node_value_after_first_subdivision 
    // (se sto risolvendo la sottomatrice di sinistra ottenuta dopo la primissima scomposizione il valore di middle_node_value_after_first_subdivision sara' sempre 0 ma qualora stessi 
    // risolvendo la sottomatrice di destra ottenuta sempre dopo la primissima scomposizione, allora il valore di middle_node_value_after_first_subdivision non e' detto che sia 0.

    //con il for di sotto semplicemente inizializzo tutti i nodi della prima riga della matrice m (tranne il source che ha valore 0) con i rispettivi valori decrescenti:
    // 0, -2, -3, -4, -5, -6 (come fatto nell'esercizio con beccuti solamente che qui alle inserizioni stiamo dando score -2 e non -1)..
    //inserisco manualmente l'affine gap in tutti i nodi della prima riga della matrice (tranne per il nodo source)

    //con il for di sotto semplicemente inizializzo il nodo subito a destra del source in tenendo conto del gap e del gap_ext:
    M[0][num_column - 1] = M[0][0] - gap;

    /*cout << "M[0][num_column - 1]:" << endl;
    cout << M[0][num_column - 1] << endl;
    cout << " " << endl;*/

    //print_m[0][0] = M[0][0]; //da togliere
    //print_m[0][num_column - 1] = M[0][num_column - 1]; //da togliere


    //passo 3: riempio tutta la prima colonna della matrice M.
    //con il for di sotto semplicemente inizializzo tutti i nodi della prima colonna della matrice m(tranne il source che ha valore 0) con i rispettivi valori decrescenti:
    // 0, 
    //-2, 
    //-3, 
    //-4, 
    //-5, 
    //-6  (come fatto nell'esercizio con beccuti)
    for (int i = 1; i <= A_n; i++) {
        // inserisco manualmente l'affine gap in tutti i nodi della prima colonna della matrice (tranne per il nodo source)

        // le due righe di sotto le devi usare PERCHE' TENGONO CONTO DEL FATTO CHE NELLA REALTA' E' PIU' PROBABILE CHE CI SIANO GAPS VICINI CHE DISCONTINUI:
        //m[i][0] = m[i - 1][0] - (gap + (gap_ext * (i - 1))); //m[1][0] = -(2 + (1 * (1-1))) -> m[1][0] = -2;  m[2][0] = -(2 + (1 * (2-1))) -> m[2][0] = -3;  m[3][0] = -(2 + (1 * (3-1))) -> m[3][0] = -4; ecc...
        //print_m[i][0] = m[i][0]; //da togliere

        M[i][0] = M[i - 1][0] - gap; //ho tolto + (gap_ext * (i - 1)) per mettermi nello stesso caso dell'esercizio..
        //print_m[i][0] = M[i][0]; //da togliere
        
        //cout << "M[i][0]:" << endl;
        //cout << M[i][0] << endl;
    }
    //////////////////////////////////////////////////////////


    //passo 4: utilizzo la finestra di dimensione 2*n e la faccio scorrere in avanti ogni volta spostandomi di una colonna in modo da poter calcolare tutti gli score della colonna corrente
    // utilizzando ogni volta solo ed esclusivamente i valori della colonna precedente.
    // inizierò quindi a scorrere tutti i caratteri sia della stringa A (presente lungo le righe) che della stringa B (presente lungo le colonne) pero' solamente fino 
    // alla colonna centrale (perchè sono dentro la funzione frmo_source) in modo tale da capire quali score inserire nella matrice M.

    // Con il doppio ciclo for qui sotto faccio questo:
    // calcolo gli score di ogni nodo partendo da quello in corrispondenza della prima riga e prima colonna ovvero quello presente in diagonale subito sotto al source.


    //best_action_for_central_column.emplace_back('-'); //la migliore azione per il nodo 0 della colonna centrale corrente sono sicuro 
    //che sarà sempre '-' e quindi lo aggiungo prima di entrare nel doppio for di sotto perchè lì dentro si parte dal nodo 1 e non dal nodo 0.

    for (int j = 1; j <= central_column_index; j++) { //j indice che mi serve solamente per scorrere i caratteri della stringa b fino alla colonna centrale.
        for (int i = 1; i <= A_n; i++) { //i = indice di riga della matrice (scorre tutte le righe della matrice fino all'ultima riga) e mi serve anche per scorrere i caratteri della stringa a.

            nuc = B[j - 1]; //prendo il nucleotide presente nella stringa B in posizione B[j-1] (quando j=1 sarebbe il carattere t della stringa b, verificalo con la stampa).
            switch (nuc) //vedo a quale nucleotide corrisponde e in base al carattere setto x in base ad uno specifico valore.
            {
            case 'C':  x = 0;  break;
            case 'T':  x = 1;  break;
            case 'A':  x = 2;  break;
            case 'G':  x = 3;
            }

            nuc = A[i - 1]; //prendo il nucleotide presente nella stringa A in posizione A[i-1] (quando i=1 sarebbe il carattere a della stringa a, verificalo con la stampa).
            switch (nuc) //vedo a quale nucleotide corrisponde e in base al carattere setto y in base ad uno specifico valore.
            {
            case 'C':  y = 0;  break;
            case 'T':  y = 1;  break;
            case 'A':  y = 2;  break;
            case 'G':  y = 3;
            }


            if (k == 1) {
                if (i == 1) {
                    // lo faccio solo per inizializzare il primissimo nodo della colonna corrente che devo riempire.
                    // le due istruzioni di sotto c'erano prima E LE DEVI USARE PER IL DISCORSO DEI GAPS DISCONTINUI..
                    // m[0][k] = -(gap + (gap_ext * (j - 1)));
                    // print_m[0][j] = m[0][k]; //da togliere 

                    // le due istruzioni di sotto le ho aggiunte ora:
                    M[0][k] = M[0][k - 1] - gap;
                    //print_m[0][j] = M[0][k]; //da togliere 
                }

                // vuol dire che k==1 e quindi devo calcolare i valori di score della colonna 1 della finestra usando i valori della colonna 0 della finestra.
                // le tre istruzioni di sotto c'erano prima E LE DEVI USARE PER IL DISCORSO DEI GAPS DISCONTINUI..
                //scu = m[i-1][k] - gap_affinity(gap, gap_ext, length); // calcolo lo score proveniente dalla cella superiore rispetto a quella che sto considerando correntemente; scu quando i=1 e j=1 sarà -> scu = -4
                //scd = m[i-1][k-1] + s[x][y]; // calcolo lo score proveniente dalla cella in diagonale a sinistra  rispetto a quella che sto considerando correntemente; scd quando i=1 e j=1 sarà -> scd = -7
                //scl = m[i][k-1] - gap_affinity(gap, gap_ext, length); // calcolo lo score proveniente dalla cella di sinistra rispetto a quella che sto considerando correntemente; scl quando i=1 e j=1 sarà -> scl = -4

                // le tre istruzioni di sotto le ho aggiunte ora:
                scu = M[i - 1][k] - gap; // calcolo lo score proveniente dalla cella superiore rispetto a quella che sto considerando correntemente; scu quando i=1 e j=1 sarà -> scu = -4
                scd = M[i - 1][k - 1] + s[x][y]; // calcolo lo score proveniente dalla cella in diagonale a sinistra  rispetto a quella che sto considerando correntemente; scd quando i=1 e j=1 sarà -> scd = -7
                scl = M[i][k - 1] - gap; // calcolo lo score proveniente dalla cella di sinistra rispetto a quella che sto considerando correntemente; scl quando i=1 e j=1 sarà -> scl = -4

                /*cout << " " << endl;
                cout << " " << endl;
                cout << "x: " << x << endl;
                cout << "y: " << y << endl;
                cout << "scu: " << scu << endl;
                cout << "scd: " << scd << endl;
                cout << "scl: " << scl << endl;
                cout << " " << endl;
                cout << " " << endl;*/

                M[i][k] = max_score(scu, scd, scl, &ptr, length); // calcolo lo score max per il nodo corrente e lo memorizzo al suo interno.
            }
            else {
                //vuol dire che k==0 e quindi devo calcolare i valori di score della colonna 0 della finestra usando pero' i valori di score della colonna 1 della finestra. 
                if (i == 1) {
                    // lo faccio solo per inizializzare il primissimo nodo della colonna corrente che devo riempire.
                    // le due istruzioni di sotto c'erano prima E LE DEVI USARE PER IL DISCORSO DEI GAPS DISCONTINUI..
                    //m[0][k] = -(gap + (gap_ext * (j - 1))); 
                    //print_m[0][j] = m[0][k]; //da togliere

                    // le due istruzioni di sotto le ho aggiunte ora:
                    M[0][k] = M[0][k + 1] - gap;
                    //print_m[0][j] = M[0][k]; //da togliere 
                }

                // le tre istruzioni di sotto c'erano prima E LE DEVI USARE PER IL DISCORSO DEI GAPS DISCONTINUI..
                //scu = m[i-1][k] - gap_affinity(gap, gap_ext, length); // calcolo lo score proveniente dalla cella superiore rispetto a quella che sto considerando correntemente; scu quando i=1 e j=1 sarà -> scu = -4
                //scd = m[i-1][k+1] + s[x][y]; // calcolo lo score proveniente dalla cella in diagonale a sinistra  rispetto a quella che sto considerando correntemente; scd quando i=1 e j=1 sarà -> scd = -7
                //scl = m[i][k+1] - gap_affinity(gap, gap_ext, length); // calcolo lo score proveniente dalla cella di sinistra rispetto a quella che sto considerando correntemente; scl quando i=1 e j=1 sarà -> scl = -4

                // le tre istruzioni di sotto le ho aggiunte ora:
                scu = M[i - 1][k] - gap; // calcolo lo score proveniente dalla cella superiore rispetto a quella che sto considerando correntemente; scu quando i=1 e j=1 sarà -> scu = -4
                scd = M[i - 1][k + 1] + s[x][y]; // calcolo lo score proveniente dalla cella in diagonale a sinistra  rispetto a quella che sto considerando correntemente; scd quando i=1 e j=1 sarà -> scd = -7
                scl = M[i][k + 1] - gap; // calcolo lo score proveniente dalla cella di sinistra rispetto a quella che sto considerando correntemente; scl quando i=1 e j=1 sarà -> scl = -4

                M[i][k] = max_score(scu, scd, scl, &ptr, length); // calcolo lo score max per il nodo corrente e lo memorizzo al suo interno.         
            }


            //Eseguo alcuni controlli:
            //if (j == central_column_index) {
            //    //allora vuol dire che devo memorizzarmi tutti gli archi migliori per ogni nodo presente nella colonna centrale. Se un nodo della colonna centrale ha più archi migliori allora
            //    //mi memorizzo solamente uno:
            //    best_action_for_central_column.emplace_back(best_action(scu, scd, scl));
            //}
            //if (j == central_column_index - 1) {
            //    //mi memorizzo tutti gli archi migliori di tutti i nodi che precededono la 
            //    // central_column_index
            //    if (i == 1) {
            //        best_action_for_previous_central_column.emplace_back('-');
            //    }
            //    best_action_for_previous_central_column.emplace_back(best_action(scu, scd, scl));
            //}
            //////////////////////////

            //da togliere
            //print_m[i][j] = M[i][k]; //mi salvo nella matrice che stamperò alla fine della funzione from_source il valore della finestra che ho appena calcolato (ovvero l'elemento m[i][k]).
        }


        // dopo aver inserito tutti gli elementi scorrendo tutte le righe per la colonna corrente posso aggiornare k.
        // adesso sono certo che il ciclo interno ovvero quello che scorre lungo le righe della matrice M è finito.
        // adesso pero' sappiamo che la matrice m ha dimensione 2*a_n e quindi prima di andare al ciclo successivo devo assicurarmi che la colonna appena calcolata diventi la colonna 0 della matrice M, in modo 
        // tale che all'iterata successiva la colonna 1 della matrice m possa essere utilizzata per inserire i valori di score della nuova colonna da considerare.
        // in questo modo praticamente implemento realmente la finestra di dimensione 2*n che riutilizzo di continuo senza dover ogni volta deallocale e riallocare la memoria.
        if (k == 0) {
            k = 1;
        }
        else {
            k = 0;
        }
    }
    // fine ciclo for più esterno.


    //devo cambiare il valore di k perchè nell'ultima iterazione sicuramente il valore dell'ultima colonna è stato cambiato inutilmente.
    if (k == 0) {
        k = 1;
    }
    else {
        k = 0;
    }

    column_res = copy_central_column(M, column_res, A_n, k); //copio la colonna centrale nel vettore risultato perchè la matrice m verrà deallocata


    /// procedura di stampa:
    //print_matrix_from_source(print_m, A, B, A_n, central_column_index); //il parametro central_column_index l'ho aggiunto dopo per non avere problemi 
    //quando chiamavo la print_matrix_from_source
    ///

    // libero la memoria occupata (per m) dinamicamente:
    for (int i = 0; i <= A_n; i++)  delete M[i];
    delete[] M;

    //da togliere...
    /*for (int i = 0; i <= A_n; i++)  delete print_m[i];
    delete[] print_m;*/


    //cout << "la memoria e' stata liberata con successo!" << endl;
    


    return column_res;
}

void from_source_total(string A, string B, int A_n, int central_column_index, int a, int b, int gap, int gap_ext, list<char>& actions_optimal_path, int value_source) {

    /*cout << "Sono nella from_source_total." << endl;
    cout << "A: " << A << endl;
    cout << "B: " << B << endl;
    cout << "A_n: " << A_n << endl;
    cout << "central_column_index: " << central_column_index << endl;
    cout << "" << endl;*/

    //la matrice m avrà A_n + 1 righe e central_column_index + 1 colonne.
    int** M = new int* [A_n + 1]; //dichiaro m come una matrice dove ogni riga avrà dimensione pari a 2.
    char** M_tb = new char* [A_n + 1]; //dichiaro m_tb come una matrice di supporto (che conterrà tutti i migliori archi per ogni nodo) dove ogni riga avrà dimensione pari a a_n + 1.

    const int num_column = 2;
    vector<int> column_res(A_n + 1);
    
    //int** print_m = new int* [A_n + 1];//da togliere
    
    int x = 0, y = 0;
    int scu, scd, scl; // scores for respectively cell above, diagonal and left (questi sono rispettivamente i valori di score della cella superiore, cella in diagonale e della cella a sinistra rispetto ovviamente alla cella corrente che stiamo considerando.
    char ptr, nuc;
    int k = 1; //mi servirà dopo come indice per sapere in quale colonna della matrice devo scrivere gli score della nuova colonna e quindi di conseguenza mi dirà anche qual è la colonna da cui devo partire per calcolare gli scores della nuova colonna.
    int length = 0; // initial gap length (questa è la lunghezza del gap ovvero porta il conto di quanti gap consecutivi sono stati aperti, sarebbe il k che ci ha detto beccuti)

    // creo la matrice di scoring:
    const int  s[4][4] = {
                            { a,  b,  b, b },
                            { b,  a,  b, b },
                            { b,  b,  a, b },
                            { b,  b,  b, a }
    };


    //passo 1: Alloco la memoria per la matrice necessaria per calcolare la from_source con costo o(2*n).
    for (int i = 0; i <= A_n; i++) {
        M[i] = new int[num_column]; //qui inserisco le colonne di m e dico che esse saranno sempre e soltanto 2. 
    }

    for (int i = 0; i <= A_n; i++) {
        M_tb[i] = new char[central_column_index + 1]; //qui inserisco le colonne di m_tb e dico che per ogni riga ci sarà un numero di colonne pari a central_column_index + 1.
    }

    //da togliere..
    //for (int i = 0; i <= A_n; i++) {
    //    print_m[i] = new int[central_column_index + 1]; //qui inserisco le colonne di m e dico che esse saranno sempre e soltanto 2. 
    //}
    //////////////////////////////////////////////////////////////////////////////////////////////////////


    //passo 2: Riempio la prima riga della matrice m.
    M[0][0] = value_source; //sarà sempre 0 quando chiamo la funzione "from_source_total" per completare quello che resta della sottomatrice sinistra, mentre 
    //potrà essere un altro valore quando chiamo la funzione "from_source_total" per completare quello che resta della sottomatrice destra.
    M_tb[0][0] = 'n'; //inizializzo il nodo source anche nella matrice di supporto.
    /*cout << "central_column_index IN From_source_total (la finestra scorrera' da 0 fino a central_column_index compresa): " << central_column_index << endl;
    cout << "M[0][0]: " << M[0][0] << endl;
    cout << " " << endl;*/

    // Con il for di sotto semplicemente inizializzo il nodo subito a destra del source tenendo conto del gap e del gap_ext:
    M[0][num_column - 1] = M[0][0] - gap; //messo ora..
    /*cout << "M[0][num_column - 1]:" << endl;
    cout << M[0][num_column - 1] << endl;
    cout << " " << endl;*/

    // con il for di sotto semplicemente inizializzo tutti i nodi della prima riga della matrice m_tb (tranne il source che ha valore 0) con i rispettivi valori decrescenti:
    // 0, -2, -3, -4, -5, -6 (come fatto nell'esercizio con beccuti).
    // Lo faccio solo per la M_tb e non anche per la matrice M perchè quest'ultima ha solamente due colonne (abbattimento del costo di memoria).
    for (int j = 1; j <= central_column_index; j++) {
        //inserisco manualmente l'affine gap in tutti i nodi della prima riga della matrice (tranne per il nodo source)
        M_tb[0][j] = '-'; //siccome sto andando lungo la prima riga e quindi mi sto spostandi in orizzontale allora queste sono inserzioni e quindi 
        // nella m_tb mi memorizzo in ogni posizione della prima riga il carattere '-' per simboleggiare l'arco ->
    }
    /////////////////////////////////////////////////


    //print_m[0][0] = M[0][0]; //da togliere
    //print_m[0][num_column - 1] = M[0][num_column - 1]; //da togliere


    //passo 3: Riempio tutta la prima colonna della matrice M.
    //con il for di sotto semplicemente inizializzo tutti i nodi della prima colonna della matrice m(tranne il source che ha valore 0) con i rispettivi valori decrescenti:
    // 0, 
    //-2, 
    //-3, 
    //-4, 
    //-5, 
    //-6  (come fatto nell'esercizio con beccuti solamente che qui alle inserizioni stiamo dando score -2 e non -1)..
    for (int i = 1; i <= A_n; i++) {
        //inserisco manualmente l'affine gap in tutti i nodi della prima colonna della matrice (tranne per il nodo source)
        // le due righe di sotto erano quelle che c'erano prima:

        //La riga di sotto LA DEVI METTERE PER CONSIDERARE IL FATTO che nella realtà è più probabile che siano gaps consecutivi che gaps discontinui:
        //m[i][0] = m[i - 1][0] - (gap + (gap_ext * (i - 1))); //m[1][0] = -(2 + (1 * (1-1))) -> m[1][0] = -2;  m[2][0] = -(2 + (1 * (2-1))) -> m[2][0] = -3;  m[3][0] = -(2 + (1 * (3-1))) -> m[3][0] = -4; ecc...

        M[i][0] = M[i - 1][0] - gap; //ho tolto + (gap_ext * (i - 1)) per mettermi nello stesso caso dell'esercizio..
        //print_m[i][0] = M[i][0]; //da togliere

        M_tb[i][0] = '|'; //siccome sto andando lungo la prima colonna e quindi mi sto spostando in verticale allora queste sono delezioni e quindi nella m_tb 
        // mi memorizzo in ogni posizione della prima riga il carattere '|' per simboleggiare l'arco che va verso il basso.

        /*cout << "M[i][0]:" << endl;
        cout << M[i][0] << endl;
        cout << "M_tb[i][0]:" << endl;
        cout << M_tb[i][0] << endl;*/
    }
    //////////////////////////////////////////////////////////


    //passo 4: Utilizzo la finestra di dimensione 2*n e la faccio scorrere in avanti ogni volta spostandomi di una colonna in modo da poter calcolare tutti gli score della colonna corrente
    // utilizzando ogni volta solo ed esclusivamente i valori della colonna precedente.
    // inizierò quindi a scorrere tutti i caratteri sia della stringa A (presente lungo le righe) che della stringa B (presente lungo le colonne) pero' solamente fino 
    // alla colonna centrale (perchè sono dentro la funzione from_source) in modo tale da capire quali score inserire nella matrice M.

    // Con il doppio ciclo for qui sotto faccio questo:
    // calcolo gli score di ogni nodo partendo da quello in corrispondenza della prima riga e prima colonna ovvero quello presente in diagonale subito sotto al source.
    for (int j = 1; j <= central_column_index; j++) { //j indice che mi serve solamente per scorrere i caratteri della stringa B fino alla colonna centrale.
        for (int i = 1; i <= A_n; i++) { //i = indice di riga della matrice (scorre tutte le righe della matrice fino all'ultima riga) e mi serve anche per scorrere i caratteri della stringa A.

            nuc = B[j - 1]; //prendo il nucleotide presente nella stringa B in posizione B[j-1].
            //cout << "nucleotide corrente di B: " << nuc << endl;
            
            switch (nuc) //vedo a quale nucleotide corrisponde e in base al carattere setto x in base ad uno specifico valore.
            {
            case 'C':  x = 0;  break;
            case 'T':  x = 1;  break;
            case 'A':  x = 2;  break;
            case 'G':  x = 3;
            }

            nuc = A[i - 1]; //prendo il nucleotide presente nella stringa A in posizione A[i-1] (quando i=1 sarebbe il carattere a della stringa a, verificalo con la stampa).
            //cout << "nucleotide corrente di A: " << nuc << endl;
            
            switch (nuc) //vedo a quale nucleotide corrisponde e in base al carattere setto y in base ad uno specifico valore.
            {
            case 'C':  y = 0;  break;
            case 'T':  y = 1;  break;
            case 'A':  y = 2;  break;
            case 'G':  y = 3;
            }


            if (k == 1) {
                if (i == 1) {
                    // lo faccio solo per inizializzare il primissimo nodo della colonna corrente che devo riempire.
                    // le due istruzioni di sotto c'erano prima e MI SERVONO QUANDO VOGLIO CONSIDERARE IL FATTO che nella realtà è più probabile che 
                    // siano gaps consecutivi che gaps discontinui:
                    //m[0][k] = -(gap + (gap_ext * (j - 1)));
                    //print_m[0][j] = m[0][k]; //da togliere 

                    // le due istruzioni di sotto le ho aggiunte ora:
                    M[0][k] = M[0][k - 1] - gap;
                    //print_m[0][j] = M[0][k]; //da togliere 

                }
                // Vuol dire che k==1 e quindi devo calcolare i valori di score della colonna 1 della finestra usando i valori della colonna 0 della finestra.
                // le tre istruzioni di sotto c'erano prima e MI SERVONO QUANDO VOGLIO CONSIDERARE IL FATTO che nella realtà è più probabile che 
                // siano gaps consecutivi che gaps discontinui:

                //scu = m[i-1][k] - gap_affinity(gap, gap_ext, length); // calcolo lo score proveniente dalla cella superiore rispetto a quella che sto considerando correntemente; scu quando i=1 e j=1 sarà -> scu = -4
                //scd = m[i-1][k-1] + s[x][y]; // calcolo lo score proveniente dalla cella in diagonale a sinistra  rispetto a quella che sto considerando correntemente; scd quando i=1 e j=1 sarà -> scd = -7
                //scl = m[i][k-1] - gap_affinity(gap, gap_ext, length); // calcolo lo score proveniente dalla cella di sinistra rispetto a quella che sto considerando correntemente; scl quando i=1 e j=1 sarà -> scl = -4

                // le tre istruzioni di sotto le ho aggiunte ora:
                scu = M[i - 1][k] - gap; // calcolo lo score proveniente dalla cella superiore rispetto a quella che sto considerando correntemente; scu quando i=1 e j=1 sarà -> scu = -4
                scd = M[i - 1][k - 1] + s[x][y]; // calcolo lo score proveniente dalla cella in diagonale a sinistra  rispetto a quella che sto considerando correntemente; scd quando i=1 e j=1 sarà -> scd = -7
                scl = M[i][k - 1] - gap; // calcolo lo score proveniente dalla cella di sinistra rispetto a quella che sto considerando correntemente; scl quando i=1 e j=1 sarà -> scl = -4

                // stampo alcune info:
                /*cout << " " << endl;
                cout << " " << endl;
                cout << "x: " << x << endl;
                cout << "y: " << y << endl;
                cout << "scu: " << scu << endl;
                cout << "scd: " << scd << endl;
                cout << "scl: " << scl << endl;
                cout << " " << endl;
                cout << " " << endl;*/

                M[i][k] = max_score(scu, scd, scl, &ptr, length); // calcolo lo score max per il nodo corrente (quando i=1 e j=1 allora m[1][1] = -4) e lo memorizzo al suo interno.
                M_tb[i][j] = ptr; //mi memorizzo il puntatore che mi dice per il nodo corrente qual è stato il nodo migliore che mi ha portato ad esso. (per m_tb[1][1] quindi per il nodo che si trova 
               //subito sotto a destra in diagonale rispetto al nodo source, il valore del puntatore sarà: m_tb[1][1] = \\;
            }
            else {
                // Vuol dire che k==0 e quindi devo calcolare i valori di score della colonna 0 della finestra usando pero' i valori di score della colonna 1 della finestra. 
                if (i == 1) {

                    // lo faccio solo per inizializzare il primissimo nodo della colonna corrente che devo riempire.
                    // le due istruzioni di sotto c'erano prima e MI SERVONO QUANDO VOGLIO CONSIDERARE IL FATTO che nella realtà è più probabile che 
                    // siano gaps consecutivi che gaps discontinui:
                    //m[0][k] = -(gap + (gap_ext * (j - 1))); 
                    //print_m[0][j] = m[0][k]; //da togliere

                    // le due istruzioni di sotto le ho aggiunte ora:
                    M[0][k] = M[0][k + 1] - gap;
                    //print_m[0][j] = M[0][k]; //da togliere 
                }

                // le tre istruzioni di sotto c'erano prima e MI SERVONO QUANDO VOGLIO CONSIDERARE IL FATTO che nella realtà è più probabile che 
                // siano gaps consecutivi che gaps discontinui:
                //scu = m[i-1][k] - gap_affinity(gap, gap_ext, length); // calcolo lo score proveniente dalla cella superiore rispetto a quella che sto considerando correntemente; scu quando i=1 e j=1 sarà -> scu = -4
                //scd = m[i-1][k+1] + s[x][y]; // calcolo lo score proveniente dalla cella in diagonale a sinistra  rispetto a quella che sto considerando correntemente; scd quando i=1 e j=1 sarà -> scd = -7
                //scl = m[i][k+1] - gap_affinity(gap, gap_ext, length); // calcolo lo score proveniente dalla cella di sinistra rispetto a quella che sto considerando correntemente; scl quando i=1 e j=1 sarà -> scl = -4

                // le tre istruzioni di sotto le ho aggiunte ora:
                scu = M[i - 1][k] - gap; // calcolo lo score proveniente dalla cella superiore rispetto a quella che sto considerando correntemente; scu quando i=1 e j=1 sarà -> scu = -4
                scd = M[i - 1][k + 1] + s[x][y]; // calcolo lo score proveniente dalla cella in diagonale a sinistra  rispetto a quella che sto considerando correntemente; scd quando i=1 e j=1 sarà -> scd = -7
                scl = M[i][k + 1] - gap; // calcolo lo score proveniente dalla cella di sinistra rispetto a quella che sto considerando correntemente; scl quando i=1 e j=1 sarà -> scl = -4


                M[i][k] = max_score(scu, scd, scl, &ptr, length); // calcolo lo score max per il nodo corrente (quando i=1 e j=1 allora m[1][1] = -4) e lo memorizzo al suo interno.

                // adesso in ptr, la funzione max_score, ha memorizzato la miglior azione da fare (guardando tutti i nodi che hanno un arco entrante in quello corrente) 
                // per spostarmi nel nodo corrente (ovvero nel nodo m[i][k]). 

                // adesso quindi non mi resta che memorizzare quest'azione nella lista di char chiamato "best_actions": 
                // (e' gia' un'ottimizzazione memorizzarmi solo l'arco migliore per il nodo che mi interessa per questo ancora non la faccio)..

                M_tb[i][j] = ptr; //mi memorizzo il puntatore che mi dice per il nodo corrente qual è stato il nodo migliore che mi ha portato ad esso. (per m_tb[1][1] quindi per il nodo che si trova 
                //subito sotto a destra in diagonale rispetto al nodo source, il valore del puntatore sarà: m_tb[1][1] = \\;
            }

            //da togliere
            //print_m[i][j] = M[i][k]; //mi salvo nella matrice che stamperò alla fine della funzione from_source il valore della finestra che ho appena calcolato (ovvero l'elemento m[i][k]).

        }

        // Dopo aver inserito tutti gli elementi scorrendo tutte le righe per la colonna corrente posso aggiornare k questo perchè adesso sono certo 
        // che il ciclo interno ovvero quello che scorre lungo le righe della matrice M è finito.
        // Adesso pero' sappiamo che la matrice M ha dimensione 2*a_n e quindi prima di andare al ciclo successivo devo assicurarmi che la colonna 
        // appena calcolata diventi la colonna 0 della matrice M, in modo tale che all'iterata successiva la colonna 1 della matrice M 
        // possa essere utilizzata per inserire i valori di score della nuova colonna da considerare.
        // In questo modo praticamente implemento realmente la finestra di dimensione 2*n che riutilizzo di continuo senza dover ogni volta deallocale e riallocare la memoria.
        if (k == 0) {
            k = 1;
        }
        else {
            k = 0;
        }
    }
    // fine anche del ciclo for più esterno


    // Devo cambiare il valore di k perchè nell'ultima iterazione sicuramente il valore dell'ultima colonna è stato cambiato inutilmente.
    if (k == 0) {
        k = 1;
    }
    else {
        k = 0;
    }
    column_res = copy_central_column(M, column_res, A_n, k); //copio la colonna centrale nel vettore risultato perchè la matrice m verrà deallocata


    /// procedura di stampa 
    //print_matrix_from_source(print_m, A, B, A_n, central_column_index); //il parametro central_column_index l'ho aggiunto dopo per non avere problemi 
    //quando chiamavo la print_matrix_from_source
    ///

    /// procedura di stampa
    //print_tb(M_tb, A, B, A_n, central_column_index);
    ///


    //adesso posso finalmente aggiornare il cammino ottimale aggiungendo le azioni da fare che mancano per poter arrivare al nodo source partendo da dove ho deciso di interrompermi 
    //con la from_source e la to_sink:
    //calculate_final_optimal_path(M_tb, actions_optimal_path, A_n, central_column_index, submatrix_right, A, B); c'era prima ma ora non c'è più bisogno di 
    // conoscere submatrix_right perchè tutti gli archi li aggiungo sempre in testa indipendentemente che mi trovi nella sottomatrice sinistra o destra.
    calculate_final_optimal_path(M_tb, actions_optimal_path, A_n, central_column_index, A, B);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*cout << "A_n: " << A_n << endl;
    cout << "central_column_index: " << central_column_index << endl;
    cout << "Number row of M_tb: " << A_n + 1 << endl;
    cout << "Number column of M_tb: " << central_column_index + 1 << endl;*/

    // libero la memoria occupata (per m) dinamicamente:
    for (int i = 0; i <= A_n; i++)  delete M[i];
    delete[] M;
    
    for (int i = 0; i <= A_n; ++i) delete M_tb[i];
    delete[] M_tb;

    //da togliere..
    /*for (int i = 0; i <= A_n; i++)  delete print_m[i];
    delete[] print_m;*/

    //cout << "la memoria di m e m_tb e' stata liberata con successo!" << endl;
}