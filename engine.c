///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//                                Codage des pièces                                 //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//  emSq, P+, P-, K, N, B, R, Q                      0  0  0    0  0  0  0       //
//     0,  1,  2, 3, 4, 5, 6, 7                     64 32 16    8  4  2  1       //
//                                                                               //
//  w = 8  b = 16  virgin = 32                                                   //
//                                                                               //
//  Pb : P+ | w = 9     0001 | 1000 = 1001           1 & 1 = 1   bitwise AND     //
//  wK :  K | w = 11    0011 | 1000 = 1011           1 & 0 = 0                   //
//  wN :  N | w = 12    0100 | 1000 = 1100           0 & 1 = 0                   //
//  wB :  B | w = 13    0101 | 1000 = 1101           0 & 0 = 0                   //
//  wR :  R | w = 14    0110 | 1000 = 1110                                       //
//  wQ :  Q | w = 15    0111 | 1000 = 1111           1 | 1 = 1   bitwise OR      //
//                                                   1 | 0 = 1                   //
//  Pn : P- | b = 18    00010 | 10000 = 10010        0 | 1 = 1                   //
//  bK :  K | b = 19    00011 | 10000 = 10011        0 | 0 = 0                   //
//  bN :  N | b = 20    00100 | 10000 = 10100                                    //
//  bB :  B | b = 21    00101 | 10000 = 10101                                    //
//  bR :  R | b = 22    00110 | 10000 = 10110                                    //
//  bQ :  Q | b = 23    00111 | 10000 = 10111                                    //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>

int echiquier[128] = {                // 0x88 echiquier + scores de position

    22, 20, 21, 23, 19, 21, 20, 22,    0,  0,  5,  5,  0,  0,  5,  0, 
    18, 18, 18, 18, 18, 18, 18, 18,    5,  5,  0,  0,  0,  0,  5,  5,
     0,  0,  0,  0,  0,  0,  0,  0,    5, 10, 15, 20, 20, 15, 10,  5,
     0,  0,  0,  0,  0,  0,  0,  0,    5, 10, 20, 30, 30, 20, 10,  5,    
     0,  0,  0,  0,  0,  0,  0,  0,    5, 10, 20, 30, 30, 20, 10,  5,
     0,  0,  0,  0,  0,  0,  0,  0,    5, 10, 15, 20, 20, 15, 10,  5,
     9,  9,  9,  9,  9,  9,  9,  9,    5,  5,  0,  0,  0,  0,  5,  5,
    14, 12, 13, 15, 11, 13, 12, 14,    0,  0,  5,  5,  0,  0,  5,  0

};

char *notation[] = {           // convertir l'ID de case en notation de plateau

    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",     "i8","j8","k8","l8","m8","n8","o8", "p8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",     "i7","j7","k7","l7","m7","n7","o7", "p7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",     "i6","j6","k6","l6","m6","n6","o6", "p6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",     "i5","j5","k5","l5","m5","n5","o5", "p5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",     "i4","j4","k4","l4","m4","n4","o4", "p4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",     "i3","j3","k3","l3","m3","n3","o3", "p3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",     "i2","j2","k2","l2","m2","n2","o2", "p2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",     "i1","j1","k1","l1","m1","n1","o1", "p1",

};

/* 
    Carte  des pièces sur l'échiquier:


    
            case vide,
            sauter 2nd,
            pion noir,
            roi noir,
            cavalier noir,
            fou noir,
            tour noire,
            reine noire,
            sauter 9e,
            pion blanc,
            sauter 11e,
            roi blanc,
            cavalier blanc,
            fou blanc,
            tour blanche,
            reine blanche

*/

//char pieces[] = ".-pknbrq-P-KNBRQ";     // afficher les caractère ASCII pour representer les pièces sur l'échiquier

char *pieces[] = {                      // afficher les caractère unicode pour representer les pièces sur l'échiquier

	".", "-", "\u265F", "\u265A", "\u265E", "\u265D", "\u265C", "\u265B", 
	"-", "\u2659", "-", "\u2654", "\u2658", "\u2657", "\u2656", "\u2655",  

};

enum { WHITE = 8, BLACK = 16};    // côté à déplacer

static int decalage_coup[] = {

   15,  16,  17,   0,                           // pion blanc
  -15, -16, -17,   0,                           // pion noire
    1,  16,  -1, -16,   0,                      // les tours
    1,  16,  -1, -16,  15, -15, 17, -17,  0,    // les deux reines,rois et fous
   14, -14,  18, -18,  31, -31, 33, -33,  0,    // chevalier
    3,  -1,  12,  21,  16,   7, 12              /* indices de départ pour chaque type de pièce dans l'ordre : pions noirs,
                                                   pions blancs, rois, chevaliers, fous, tours, reines...*/ 

};


int poids_pieces[] = { 0, 0, -100, 0, -300, -350, -500, -900, 0, 100, 0, 0, 300, 350, 500, 900 };
int meilleur_src, meilleur_destination;    // pour stocker le meilleur coup trouvé dans la recherche


void affichageEchiquier()
{
    for(int sq = 0; sq < 128; sq++)
    {
        if(!(sq % 16)) printf(" %d  ", 8 - (sq / 16));    // Afficher les notations a gauches de l'échiquier
        //printf(" %c", ((sq & 8) && (sq += 7)) ? '\n' : pieces[echiquier[sq] & 15]);    // pièces ASCII
        printf(" %s", ((sq & 8) && (sq += 7)) ? "\n" : pieces[echiquier[sq] & 15]);    // pièces unicode
    }
    
    printf("\n     a b c d e f g h\n\nVotre coup: \n"); // afficher les notation en dessous de l'échiquier
}

int recherchePosition(int couleur, int profondeur, int alpha, int beta)    // renvoie le score du meilleur coup et stocke le meilleur coup
{
    if(profondeur == 0)    // si le nœud de la feuille est atteint, évaluer la position
    {
        // Evaluer la  position        
        int mat_score = 0, pos_score = 0, pce, eval = 0;
    
        for(int sq = 0; sq < 128; sq++)    // looper sur les carrés de l'échiquier
        {
            if((sq & 0x88) == 0)    // evaluer seulment les carrés sur l'échiquier
            {
                if(pce = echiquier[sq])    // stockage du valeur du code du pièce
                {
                    mat_score += poids_pieces[pce & 15]; // valeur matèriel
                    (pce & 8) ? (pos_score += echiquier[sq + 8]) : (pos_score -= echiquier[sq + 8]); // valeur positionnel
                }
            }
        }
    
        eval = mat_score + pos_score;

        return (couleur == 8) ? eval : -eval;   // retourner la valeur de l'évaluation de cette position
    }

    int old_alpha = alpha;    // nécessaire pour vérifier s'il faut stocker le meilleur mouvement ou non
    int temp_src;             // le meilleur carré source temporaire
    int temp_destination;             // le meilleur carré destination temporaire
    int score = -10000;       // moins l'infinie

    // générer
    int piece, type, directions, case_destination, case_capture, piece_capture, vecteur_etape;
    
    for(int case_src = 0; case_src < 128; case_src++)    // looper sur les carré de l'échiquier
    {
        if(!(case_src & 0x88))    // check if square where piece to generate moves for stands is on echiquier
        {
            piece = echiquier[case_src];    // stocker le code de la piece actuelle (vide si le case est vide)
                                        
            if(piece & couleur)    // si la piece appartient au couleur qui doit jouer
            {
                type = piece & 7;   // extraire le type de la piece (e.g. Pb, Pn, K, N, R, B, Q)
                directions = decalage_coup[type + 30];    // initialiser le pointeur de la liste des décalages de mouvement de la pièce actuelle
                
                while(vecteur_etape = decalage_coup[++directions])    // boucle sur les décalages de déplacement
                {
                    case_destination = case_src;    // initialiser le case de destination
                    
                    do                          // boucle sur les carrés de destination dans un rayon
                    {           
                        case_destination += vecteur_etape;    // obtenir la prochaine case de destination
                        case_capture = case_destination;    // initialiser le carré où se produit une capture de pièce
                        
                        if(case_destination & 0x88) break;    //sortir de la boucle si la case de destination est inexistante
    
                        piece_capture = echiquier[case_capture];    // initialiser la pièce capturée
    
                        if(piece_capture & couleur) break;    // sortir de la boucle si sa propre piece est capturee
                        if(type < 3 && !(vecteur_etape & 7) != !piece_capture) break;    // les pions ne capturent que diagonalement
                        if((piece_capture & 7) == 3) return 10000;    

                        // faire bouger une piece
                        echiquier[case_capture] = 0;    // clear captured square
                        echiquier[case_src] = 0;         // clear source square (from square where piece was)
                        echiquier[case_destination] = piece;     // put piece to destination square (to square)

                        // promouvoir un pion
                        if(type < 3)    // si le pion...
                        {
                            if(case_destination + vecteur_etape + 1 & 0x80)    // aille a la 1ere ou la 8eme rangee...
                                echiquier[case_destination]|=7;    // le transformer en dame
                        }
                        
                        score = -recherchePosition(24 - couleur, profondeur - 1, -beta, -alpha);    // appel de recherche negamax récursif
                                              
                        // reprendre un coup
                        echiquier[case_destination] = 0;    // effacer la case de destination 
                        echiquier[case_src] = piece;     // remettre la pièce dans sa case d'origine 
                        echiquier[case_capture] = piece_capture;    // restaurer la pièce capturée sur la case initiale 

                        // detection d'Echec et Mat
                        meilleur_src = case_src;
                        meilleur_destination = case_destination;

                        // algorithme de recherche : alpha-beta 
                        if(score > alpha)
                        {
                            if(score >= beta)
                                return beta;    // beta
                            
                            alpha = score;     // alpha agit comme max dans minimax, meilleur score de coup jusqu'à présent
        
                            // enregistrer le meilleur coup dans une branche donnée
                            temp_src = case_src;
                            temp_destination = case_destination;
                        }              
                        
                        piece_capture += type < 5;    // fausse capture 
                        
                        // fausse capture pour les pions si la double poussée de pion est sur les cartes
                        if(type < 3 & 6*couleur + (case_destination & 0x70) == 0x80)piece_capture--;
                    }
    
                    while(!piece_capture);    // terminer la boucle si quelque chose a été capturé
                }
            }
        }
    }

    // stocker le meilleur coup
    if(alpha != old_alpha)
    {
        meilleur_src = temp_src;
        meilleur_destination = temp_destination;
    }

    return alpha;   // return le meilleur coup
}

int main()
{
    char coup_utilisateur[5];    // stocker le coup fait par l'utilisateur (format: e2e4)
    
    int profondeur = 3;    // profondeur de la recherche du meilleur coup 
    int couleur = WHITE;    // les blancs jouent en premier

    affichageEchiquier();

    while(1)  // jouer contre l'ordi
    {
        memset(&coup_utilisateur[0], 0, sizeof(coup_utilisateur));    // effacer la saisie de l'utilisateur
        
        if(!fgets(coup_utilisateur, 5, stdin)) continue;    // stocker le coup de l'utilisateur
        if(coup_utilisateur[0] == '\n') continue;    // repeter la boucle si aucun coup est entre par l'utilisateur

        int source_utilisateur;
        int destination_utilisateur;    // variables pour stoquer la case source et la case destination de l'utilisateur

        for(int sq = 0; sq < 128; sq++)    // boucle sur toutes les cases de l'echiquier
        {
            if(!(sq & 0x88))    // si la case existe
                if(!strncmp(coup_utilisateur, notation[sq], 2))    // si la case source existe
                    source_utilisateur = sq;                          // convertir la chaine de caracteres en decimal

                if(!strncmp(coup_utilisateur + 2, notation[sq], 2))    // si la case destination existe
                    destination_utilisateur = sq;                              // convertir la chaine de caracteres en decimal
            }
        
        
        // faire le coup de l'utilisateur
        echiquier[destination_utilisateur] = echiquier[source_utilisateur];
        echiquier[source_utilisateur] = 0;

        // promouvoir un pion
        if(((echiquier[destination_utilisateur] == 9) && (destination_utilisateur >= 0 && destination_utilisateur <= 7)) ||
           ((echiquier[destination_utilisateur] == 18) && (destination_utilisateur >= 112 && destination_utilisateur <= 119)))
            echiquier[destination_utilisateur] |= 7;    // transformer le pion en dame de la couleur correspondante

        affichageEchiquier();

        couleur = 24 - couleur;   // changer de couleur
              
        int score = recherchePosition(couleur, profondeur, -10000, 10000);    // rechercher la meilleur reponse de l'AI

        // faire le coup de l'AI
        echiquier[meilleur_destination] = echiquier[meilleur_src];
        echiquier[meilleur_src] = 0;

        // promouvoir un pion
        if(((echiquier[meilleur_destination] == 9) && (meilleur_destination >= 0 && meilleur_destination <= 7)) ||
           ((echiquier[meilleur_destination] == 18) && (meilleur_destination >= 112 && meilleur_destination <= 119)))
            echiquier[meilleur_destination] |= 7;    // transformer le pion en dame de la couleur correspondante

        couleur = 24 - couleur;    // changer de couleur

        affichageEchiquier();
        printf("\nscore: '%d'\n", score);

        // detection d'Echec et Mat 
        if(score == 10000 || score == -10000) {printf("Echec et Mat!\n"); break;}

        //getchar();
    }

    return 0;
}
