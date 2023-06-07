
#include "to_sink.h"
#include "read_string.h"
#include "alignment.h"
#include "print_library.h"
#include "optimal_path.h"
#include <iostream>


vector<int> to_sink(string& A, string& B, int A_n, int B_n, int central_column_index, int a, int b, int gap, int gap_ext, int sink_score) {

    /*cout << "Sono nella to_sink_difference_even" << endl;
    cout << "A: " << A << endl;
    cout << "B: " << B << endl;
    cout << "A_n: " << A_n << endl;
    cout << "B_n: " << B_n << endl;*/

    //B_n = get_length_string(B);

    //cout << "B_n dopo: " << B_n << endl;
    /*cout << "central_column_index: " << central_column_index << endl;
    cout << " " << endl;*/

    //passo 1: alloco la memoria per la matrice necessaria per calcolare la to_sink con costo o(2*n).
    int** M = new int* [A_n + 1]; //dichiaro m come una matrice dove ogni riga avrà un numero di colonne pari a 2.
    const int num_column = 2; //numero di colonne della finestra scorrevole.

    vector<int> column_res(A_n + 1); //dichiaro il vettore che conterrà la colonna finale calcolata da to_sink che poi dovà essere sommata a quella ottenuta dal from_source.

    //int** print_m = new int* [A_n + 1]; //e' la matrice che utilizzo per stampare alla fine tutta la sottomatrice su cui ha lavorato la to_sink.

    //const int num_column_print_m = 1 + (get_length_string(B) - central_column_index);

    int x = 0, y = 0;
    int sc_under, sc_lower_right_diagonal, sc_right; // scores for respectively cell above, diagonal and left (questi sono rispettivamente i valori di score della cella superiore, cella in diagonale e della cella a sinistra rispetto ovviamente alla cella corrente che stiamo considerando.
    char ptr, nuc;
    int k = 0; //mi servirà dopo come indice per sapere in quale colonna della matrice devo scrivere gli score della nuova colonna e quindi di conseguenza mi dirà anche qual è la colonna da cui devo partire per calcolare gli scores della nuova colonna.
    int length = 0; // initial gap length (questa è la lunghezza del gap ovvero porta il conto di quanti gap consecutivi sono stati aperti, sarebbe il k che ci ha detto beccuti)

    // creo la matrice di scoring:
    const int  s[4][4] = {
                            { a,  b,  b, b },
                            { b,  a,  b, b },
                            { b,  b,  a, b },
                            { b,  b,  b, a }
    };



    for (int i = 0; i <= A_n; i++) {
        M[i] = new int[num_column]; //qui inserisco le colonne di m e dico che esse saranno sempre e soltanto 2. 
    }

    //for (int i = 0; i <= A_n; i++) {
    //    print_m[i] = new int[get_length_string(B)]; //qui inserisco il numero di colonne della amtrice print_m (qui ho utilizzato più memoria di quello che serve perchè altrimenti le cose da fare sotto diventavano troppo complesse e comunque
    //    //questa matrice print_m non la userò quando eseguirò veramente l'algoritmo.
    //}


    //passo 2: riempio la prima riga della matrice m.
    M[A_n][1] = sink_score; //inserisco prima il valore del sink che non è detto che sarà sempre 0.
    //print_m[A_n][B_n - 1] = M[A_n][1]; //da togliere //l'ultima colonna è proprio la 4

    // con l'istruzione di sotto semplicemente inizializzo il nodo subito a sinistra del sink tenendo conto del gap e del gap_ext: 
    // (perchè ricorda che la finestra ha solo 2 colonne)
    //M[A_n][0] = -gap; //dovresti mettere M[A_n][1] - gap
    M[A_n][0] = M[A_n][1] - gap;
    //print_m[A_n][B_n - 2] = M[A_n][0];

    // stampo alcune info:
    /*cout << " " << endl;
    cout << " " << endl;
    cout << "M[A_n][0]:" << endl;
    cout << M[A_n][0] << endl;
    cout << " " << endl;
    cout << "M[A_n][1]:" << endl;
    cout << M[A_n][1] << endl;
    cout << " " << endl;*/
    //////////////////////////////////////////////// fine passo 2


    //passo 3: riempio tutta la prima colonna della matrice m.
    // con il for di sotto semplicemente inizializzo tutti i nodi della seconda colonna della matrice M (tranne il sink che ha valore 0) 
    // con i rispettivi valori decrescenti, e guardali dal basso verso l'alto perchè è così che si inseriscono:
    //-5, 
    //-4, 
    //-3, 
    //-2, 
    //-1, 
    // 0  (come fatto nell'esercizio con beccuti solamente che qui alle inserizioni stiamo dando score -2 e non -1)..
    for (int i = A_n - 1; i >= 0; i--) {

        //inserisco manualmente l'affine gap in tutti i nodi della seconda colonna della finestra iniziale (tranne per il nodo sink):
        // c'era prima ma l'ho tolto perchè nella from source non sto considerando ancora il gap_ext:
        //M[i][1] = -(gap + (gap_ext * ((A_n - 1) - i))); //m[4][1] = -(2 + (1 * (4 - 4))) -> m[4][1] = -2;  m[3][1] = -(2 + (1 * (4 - 3))) -> m[3][1] = -3;  m[2][1] = -(2 + (1 * (4 - 2))) -> m[2][1] = -4; ecc...
        //print_m[i][B_n - 1] = M[i][1]; //da togliere

        M[i][1] = M[i + 1][1] - gap; //m[4][1] = -(2 + (1 * (4 - 4))) -> m[4][1] = -2;  m[3][1] = -(2 + (1 * (4 - 3))) -> m[3][1] = -3;  m[2][1] = -(2 + (1 * (4 - 2))) -> m[2][1] = -4; ecc...
        //print_m[i][B_n - 1] = M[i][1]; //da togliere

        /*cout << "M[i][1], i==" << i << endl;
        cout << M[i][1] << endl;*/
    }
    ////////////////////////////////////////////////////////// fine passo 3


    //passo 4: utilizzo la finestra di dimensione 2*n e la faccio scorrere all'indietro ogni volta spostandomi di una colonna, in modo da poter calcolare tutti gli score della colonna corrente
    //utilizzando ogni volta solo ed esclusivamente i valori della colonna soccessiva (perchè devi immaginare che la finistra parta da destra e si soposti ogni volta verso sinistra di una colonna per volta).
    //inizierò quindi a scorrere dalla fine tutti i caratteri sia della stringa a (presente lungo le righe) che della stringa b (presente lungo le colonne) pero' solamente fino 
    // alla colonna centrale (perchè sono dentro la funzione to_sink) in modo tale da capire quali score inserire nella matrice m.

    //con il doppio ciclo for qui sotto faccio questo:
    //calcolo gli score di ogni nodo partendo da quello in corrispondenza della penultima riga e penultima colonna ovvero quello presente in diagonale subito sopra al sink.
    char nuc_A = ' '; // da togliere...
    char nuc_B = ' '; // da togliere...

    // - Il for di sotto funziona solamente nella prima iterata forse perchè B_n == 10 e quindi la colonna centrale si troverà in posizione 5 (sempre partendo da 0) 
    //   sia se parto da sinistra e sia se parto da destra.
    // - Questo è però un caso fortuito in quanto ad esempio se B_n == 9 allora la colonna centrale si sarebbe trovata sempre in posizione 5 (partendo da 0) 
    //   se partissi da sinistra, mentre se invece partissi da destra essa si troverebbe in posizione 4 (partendo da 0) E QUINDI E' CHIARO CHE LA To_sink non riuscirebbe 
    //   a calcolare quello che deve e a BLOCCARSI ESATTAMENTE in corrispondenza della vera colonna centrale ma si fermerebbe dopo la colonna centrale corretta e chiaramente
    //   questo porterebbe ad ottenere dei risultati sbagliati.
    // 
    //   for (int j = B_n; j >= get_length_string(B) - central_column_index; j--)
    // 
    // - Per risolvere questo problema bisogna ragione per 2 casi che ho spiegato in Global_Alignment_and_Partitioning 
    //   poco prima di calcolare il valore di central_column_index ovvero durante il Passo 1.1):

    /*cout << "get_length_string(B): " << get_length_string(B) << endl;
    cout << "get_length_string(B) - central_column_index: " << get_length_string(B) - central_column_index << endl;*/

    for (int j = get_length_string(B); j >= get_length_string(B) - central_column_index; j--) { //j indice che mi serve solamente per scorrere i caratteri della stringa B partendo dall'ultimo fino ad arrivare alla colonna centrale.
        for (int i = A_n; i >= 1; i--) { //i = indice di riga della matrice (scorre tutte le righe della matrice fino alla prima riga) e mi serve anche per scorrere i caratteri della stringa A (partendo sempre dall'ultimo)).

            nuc = B[j - 1]; //prendo il nucleotide presente nella stringa B in posizione B[j-1] (quando j = b_n sarebbe il carattere g della stringa b, verificalo con la stampa).
            nuc_B = nuc;


            // controlla I VALORI DELLA TO SINK PARTENDO DAL BASSO DALLA SECONDA ITERATA IN POI (QUANDO IL SINK == 3) PERCHE' NON LI CALCOLA BENISSIMO SE VEDI QUELLO IN DIAGONALE
            // SUBITO PRIMA DEL SINK PROBABILMENTE perchè i caratteri di A e B che legge in quel momento non sono giusti.


            switch (nuc) //vedo a quale nucleotide corrisponde e in base al carattere setto x in base ad uno specifico valore.
            {
            case 'C':  x = 0;  break;
            case 'T':  x = 1;  break;
            case 'A':  x = 2;  break;
            case 'G':  x = 3;
            }

            nuc = A[i - 1]; //prendo il nucleotide presente nella stringa a in posizione A[i-1] (quando i=1 sarebbe il carattere a della stringa a, verificalo con la stampa).
            nuc_A = nuc;

            switch (nuc) //vedo a quale nucleotide corrisponde e in base al carattere setto y in base ad uno specifico valore.
            {
            case 'C':  y = 0;  break;
            case 'T':  y = 1;  break;
            case 'A':  y = 2;  break;
            case 'G':  y = 3;
            }

            //aggiorno la matrice m in base ai valori che voglio.
            if (k == 1) {
                if (i == A_n) {
                    //l'istruzione di sotto mi serve per poter inizializzare ogni volta l'ultimo nodo in basso della finestra corrente in corrispondenza della colonna 1 (perchè k=1) che devo riempire.
                    //le due istruzioni di sotto erano quelle che c'erano prima e mi servono PER TENER CONTO SEMPRE DELLA DISCONTINUITA' DEL GAPS..
                    //m[i][k] = -(gap + (gap_ext * ((b_n) - j))); // m[5][1] = -(2 + (1 * (5 - 5))) = -2;  // m[5][1] = -(2 + (1 * (5 - 4))) = -3; ecc...
                    //print_m[i][j - 2] = m[i][k]; //da togliere 

                    //le due istruzioni di sotto sono quelle che ho aggiunto ora:
                    M[i][k] = M[i][k - 1] - gap; // m[5][1] = -(2 + (1 * (5 - 5))) = -2;  // m[5][1] = -(2 + (1 * (5 - 4))) = -3; ecc...
                    //print_m[i][j - 2] = M[i][k]; //da togliere c'era prima..
                    
                    //questo qui sotto dovrai toglierlo perchè mi serve solo per la stampa:
                    /*cout << "i: " << i << endl;
                    cout << "j-2: " << j - 2 << endl;
                    cout << "M[i][k]: " << M[i][k] << endl;*/

                    //if (j > 1) { //l'ho messo perchè se j==1 allora j-2 = -1 e questo errore esce fuori SOLAMENTE QUANDO DEALLOCO LA MEMORIA di print_m !!!!
                    //    print_m[i][j - 2] = M[i][k]; //da togliere 
                    //}
                    //else {
                    //    print_m[i][j] = M[i][k];//da togliere 
                    //}

                }
                //vuol dire che k==1 e quindi devo calcolare i valori di score della colonna 1 della finestra usando pero' i valori di score della colonna 0 della finestra.

                //le 3 istruzioni di sotto erano quelle che c'erano prima e mi servono PER TENER CONTO SEMPRE DELLA DISCONTINUITA' DEL GAPS..
                //sc_under = m[i][k] - gap_affinity(gap, gap_ext, length); // calcolo lo score proveniente dalla cella superiore rispetto a quella che sto considerando correntemente; scu quando i=1 e j=1 sarà -> scu = -4
                //sc_lower_right_diagonal = m[i][k - 1] + s[x][y]; // calcolo lo score proveniente dalla cella in diagonale a sinistra  rispetto a quella che sto considerando correntemente; scd quando i=1 e j=1 sarà -> scd = -7
                //sc_right = m[i - 1][k - 1] - gap_affinity(gap, gap_ext, length); // calcolo lo score proveniente dalla cella di sinistra rispetto a quella che sto considerando correntemente; scl quando i=1 e j=1 sarà -> scl = -4

                //le 3 istruzioni di sotto sono quelle che ho aggiunto ora:
                sc_under = M[i][k] - gap; // calcolo lo score proveniente dalla cella superiore rispetto a quella che sto considerando correntemente; scu quando i=1 e j=1 sarà -> scu = -4
                sc_lower_right_diagonal = M[i][k - 1] + s[x][y]; // calcolo lo score proveniente dalla cella in diagonale a sinistra  rispetto a quella che sto considerando correntemente; scd quando i=1 e j=1 sarà -> scd = -7
                sc_right = M[i - 1][k - 1] - gap; // calcolo lo score proveniente dalla cella di sinistra rispetto a quella che sto considerando correntemente; scl quando i=1 e j=1 sarà -> scl = -4

                M[i - 1][k] = max_score(sc_under, sc_lower_right_diagonal, sc_right, &ptr, length); // calcolo lo score max per il nodo corrente (quando i=1 e j=1 allora m[1][1] = -4) e lo memorizzo al suo interno.
            }
            else {
                //vuol dire che k==0 e quindi devo calcolare i valori di score della colonna 0 della finestra usando pero' i valori di score della colonna 1 della finestra. 
                if (i == A_n) {
                    //lo faccio solo per inizializzare il primissimo nodo della colonna corrente che devo riempire.
                    //le due istruzioni di sotto erano quelle che c'erano prima e mi servono PER TENER CONTO SEMPRE DELLA DISCONTINUITA' DEL GAPS..
                    //m[i][k] = -(gap + (gap_ext * ((b_n) - j)));
                    //print_m[i][j - 2] = m[i][k]; //da togliere

                    //le due istruzioni di sotto sono quelle che ho aggiunto ora:
                    M[i][k] = M[i][k + 1] - gap;
                    //print_m[i][j - 2] = M[i][k]; //da togliere c'era prima..
                    //questo qui sotto dovrai toglierlo perchè mi serve solo per la stampa:
                    /*cout << "i: " << i << endl;
                    cout << "j-2: " << j - 2 << endl;
                    cout << "M[i][k]: " << M[i][k] << endl;*/

                    //if (j > 1) { //l'ho messo perchè se j==1 allora j-2 = -1 e questo errore esce fuori SOLAMENTE QUANDO DEALLOCO LA MEMORIA di print_m !!!!
                    //    print_m[i][j - 2] = M[i][k]; //da togliere 
                    //}
                    //else {
                    //    print_m[i][j] = M[i][k];//da togliere 
                    //}
                }
                //le 3 istruzioni di sotto erano quelle che c'erano prima e mi servono PER TENER CONTO SEMPRE DELLA DISCONTINUITA' DEL GAPS..:
                //sc_under = m[i][k] - gap_affinity(gap, gap_ext, length); // calcolo lo score proveniente dalla cella inferiore rispetto a quella che sto considerando correntemente;
                //sc_lower_right_diagonal = m[i][k + 1] + s[x][y]; // calcolo lo score proveniente dalla cella presente in diagonale bassa a destra rispetto a quella che sto considerando correntemente; scd quando i=1 e j=1 sarà -> scd = -7
                //sc_right = m[i - 1][k + 1] - gap_affinity(gap, gap_ext, length); // calcolo lo score proveniente dalla cella di destra rispetto a quella che sto considerando correntemente; scl quando i=1 e j=1 sarà -> scl = -4

                //le 3 istruzioni di sotto sono quelle che ho aggiunto ora:
                sc_under = M[i][k] - gap; // calcolo lo score proveniente dalla cella inferiore rispetto a quella che sto considerando correntemente;
                sc_lower_right_diagonal = M[i][k + 1] + s[x][y]; // calcolo lo score proveniente dalla cella presente in diagonale bassa a destra rispetto a quella che sto considerando correntemente; scd quando i=1 e j=1 sarà -> scd = -7
                sc_right = M[i - 1][k + 1] - gap; // calcolo lo score proveniente dalla cella di destra rispetto a quella che sto considerando correntemente; scl quando i=1 e j=1 sarà -> scl = -4


                /*cout << " " << endl;
                cout << " " << endl;
                cout << "nuc_A: " << nuc_A << endl;
                cout << "nuc_B: " << nuc_B << endl;
                cout << "sc_under: " << sc_under << endl;
                cout << "sc_lower_right_diagonal: " << sc_lower_right_diagonal << endl;
                cout << "sc_right: " << sc_right << endl;
                cout << " " << endl;
                cout << " " << endl;*/


                M[i - 1][k] = max_score(sc_under, sc_lower_right_diagonal, sc_right, &ptr, length); // calcolo lo score max per il nodo corrente (quando i=1 e j=1 allora m[1][1] = -4) e lo memorizzo al suo interno.
            }
            //da togliere.. Questa riga qui sotto c'era prima ma dava errore poi quando facevvo la deallocalizione per print_m:
            //print_m[i - 1][j - 2] = M[i - 1][k]; //mi salvo nella matrice che stamperò alla fine della funzione from_source il valore della finestra che ho appena calcolato (ovvero l'elemento m[i][k]).
            //Questo codice di sotto dovrai toglierlo perchè mi serve solo per la stampa:
            /*cout << "i: " << i << endl;
            cout << "j-2: " << j - 2 << endl;
            cout << "M[i][k]: " << M[i][k] << endl;*/

            //if (j > 1) { //l'ho messo perchè se j==1 allora j-2 = -1 e questo errore esce fuori SOLAMENTE QUANDO DEALLOCO LA MEMORIA di print_m !!!!
            //    print_m[i - 1][j - 2] = M[i - 1][k]; //da togliere 
            //}
            //else {
            //    print_m[i - 1][j] = M[i - 1][k];//da togliere 
            //}

        }

        // dopo aver inserito tutti gli elementi scorrendo tutte le righe per la colonna corrente posso aggiornare k.
        // adesso sono certo che il ciclo interno ovvero quello che scorre lungo le righe della matrice m è finito.
        // adesso pero' sappiamo che la matrice m ha dimensione 2*a_n e quindi prima di andare al ciclo successivo devo assicurarmi che la colonna appena calcolata diventi la colonna 0 della matrice m, in modo 
        // tale che all'iterata successiva la colonna 1 della matrice m possa essere utilizzata per inserire i valori di score della nuova colonna da considerare.
        // in questo modo praticamente implemento realmente la finestra di dimensione 2*n che riutilizzo di continuo senza dover ogni volta deallocale e riallocare la memoria.
        if (k == 0) {
            k = 1;
        }
        else {
            k = 0;
        }

    }
    // fine del ciclo for più esterno



    // In questo caso non serve cambiare il valore di k perchè la finestra scorre nella direzione opposta a come scorrena nella from source.
    // Stampo alcune info:
    /*cout << " " << endl;
    cout << " " << endl;
    cout << "valore k: " << k << endl;
    for (int i = 0; i <= A_n; i++) {
        cout << "M[i][k]: " << M[i][k] << endl;
        cout << "  " << endl;

    }*/

    column_res = copy_central_column(M, column_res, A_n, k); //copio la colonna centrale nel vettore risultato perchè la matrice m verrà deallocata
    //print_matrix_to_sink(print_m, A, B, A_n, get_length_string(B), num_column_print_m);


    // libero la memoria occupata (per M) dinamicamente:
    for (int i = 0; i <= A_n; i++)  delete M[i];
    delete[] M;
    /*cout << "la memoria per la matrice M e' stata liberata con successo dalla to_sink_difference_even!" << endl;
    cout << " " << endl;*/

    //da togliere..
    /*for (int i = 0; i <= A_n; i++)  delete print_m[i];
    delete[] print_m;*/
    /*cout << "la memoria per la matrice print_m e' stata liberata con successo dalla to_sink_difference_even!" << endl;
    cout << " " << endl;*/

    return column_res;
}