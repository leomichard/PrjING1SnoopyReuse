#include <stdio.h>
#include "fonction.h"
#include <windows.h>
#include "conio.h"
#include "windows.h"
#include <time.h>


#define LIGNES 12
#define COLONNES 22
#define BLOCFIXE 0x14
#define OISEAU 0x0E
#define VIDE 0xFE
#define DEPLACABLE 0xD1
#define SNOOPY 0xF5
#define COEUR 0x03
#define BONUS 0xE1
#define PLUSLOIN 0xE7
#define TELEPORTE 0xB8


typedef struct plateau plateau;

int scoreN1 = 0;
int scoreN2 = 0;
int scoreJeu = 0;
int paused = 0;

int compteurOiseau = 0;
int vie = 3;
int mdpN1 = 111;
int mdpN2 = 222;

int tempsRestant = 0;
int tempsMaximal = 120;
int tempsEcouleN1 = 0;

int tempsEcouleN2 = 0;

int yfleche = 16;
int xfleche = 43;
int yflechemin = 16;
int yflechemax = 26;

int xflecheNiveau = 43;
int yflecheNiveau = 16;
int yflecheNiveauMax = 18;
int yflecheNiveauMin = 16;

void color(int couleurDuTexte, int couleurDeFond) {
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, couleurDeFond * 16 + couleurDuTexte);
}

void gotoligcol(int lig, int col) {
    COORD coord;
    coord.X = col;
    coord.Y = lig;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Fonction pour initialiser le plateau avec les bords
void initialiserPlateau(int plateau[LIGNES][COLONNES]) {
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            if (i == 0 && j != 0 && j != COLONNES - 1) {
                plateau[i][j] = 0xC1; // Bord supérieur avec des barres
            } else if (i == LIGNES - 1 && j != 0 && j != COLONNES - 1) {
                plateau[i][j] = 0xC2; // Bord inférieur avec des barres
            } else if (j == 0 && i != 0 && i != LIGNES - 1) {
                plateau[i][j] = 0xB4; // Bord gauche avec des barres
            } else if (j == COLONNES - 1 && i != 0 && i != LIGNES - 1) {
                plateau[i][j] = 0xC3; // Bord droit avec des barres
            } else {
                plateau[i][j] = 0xFE; // Initialiser le reste du plateau à espace vide
            }
        }
    }
}

// Fonction pour afficher le plateau
void afficherPlateau(int plateau[LIGNES][COLONNES], Ball b, Snoopy s) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            if (b.x == j && b.y == i) {
                color(12, 0);
                printf("%c ", BALLE);
                color(15, 0);
            } else if (s.x == j && s.y == i) {
                color(2, 0);
                printf("%c ", SNOOPY);
                color(15, 0);
            } else {
                color(5, 0);
                printf("%c ", plateau[i][j]);
                color(15, 0);
            }
        }
        printf("\n");
    }
}

void afficherOiseau(int plateau[LIGNES][COLONNES]) {
    plateau[1][1] = OISEAU;
    plateau[10][1] = OISEAU;
    plateau[1][20] = OISEAU;
    plateau[10][20] = OISEAU;
}

void blocNonDeplacableN1(int plateau[LIGNES][COLONNES]) {
    plateau[1][7] = BLOCFIXE;
    plateau[1][13] = BLOCFIXE;
    plateau[2][12] = BLOCFIXE;
    plateau[2][20] = BLOCFIXE;
    plateau[3][1] = BLOCFIXE;
    plateau[4][2] = BLOCFIXE;
    plateau[4][9] = BLOCFIXE;
    plateau[5][19] = BLOCFIXE;
    plateau[6][3] = BLOCFIXE;
    plateau[8][17] = BLOCFIXE;
    plateau[9][3] = BLOCFIXE;
    plateau[9][9] = BLOCFIXE;
}

void blocNonDeplacableN2(int plateau[LIGNES][COLONNES]) {
    plateau[1][4] = BLOCFIXE;
    plateau[1][12] = BLOCFIXE;
    plateau[1][17] = BLOCFIXE;
    plateau[2][1] = BLOCFIXE;
    plateau[2][2] = BLOCFIXE;
    plateau[2][10] = BLOCFIXE;
    plateau[3][8] = BLOCFIXE;
    plateau[4][13] = BLOCFIXE;
    plateau[4][20] = BLOCFIXE;
    plateau[6][16] = BLOCFIXE;
    plateau[7][4] = BLOCFIXE;
    plateau[7][7] = BLOCFIXE;
    plateau[8][2] = BLOCFIXE;
    plateau[8][9] = BLOCFIXE;
    plateau[10][10] = BLOCFIXE;
    plateau[10][14] = BLOCFIXE;
    plateau[10][17] = BLOCFIXE;
}

void blocDeplacableN1(int plateau[LIGNES][COLONNES]) {
    plateau[2][16] = DEPLACABLE;
    plateau[4][15] = DEPLACABLE;
    plateau[7][7] = DEPLACABLE;
    plateau[7][13] = DEPLACABLE;
}

void collecteBlocDeplacableN1(int plateau[LIGNES][COLONNES], Snoopy *snoopy) {
    if (snoopy->y == 7 && snoopy->x == 13) {
        if (plateau[7][13] != VIDE) {
            plateau[7][13] = VIDE;
        }
    } else if (snoopy->y == 2 && snoopy->x == 16) {
        if (plateau[2][16] != VIDE) {
            plateau[2][16] = VIDE;
        }
    } else if (snoopy->y == 4 && snoopy->x == 15) {
        if (plateau[4][15] != VIDE) {
            plateau[4][15] = VIDE;
        }
    } else if (snoopy->y == 7 && snoopy->x == 7) {
        if (plateau[7][7] != VIDE) {
            plateau[7][7] = VIDE;
        }
    }
}

void blocDeplacableN2(int plateau[LIGNES][COLONNES]) {
    plateau[3][12] = DEPLACABLE;
    plateau[4][6] = DEPLACABLE;
    plateau[6][12] = DEPLACABLE;
    plateau[9][8] = DEPLACABLE;
    plateau[9][12] = DEPLACABLE;
}

void collecteBlocDeplacableN2(int plateau[LIGNES][COLONNES], Snoopy *snoopy) {
    if (snoopy->y == 3 && snoopy->x == 12) {
        if (plateau[3][12] != VIDE) {
            plateau[3][12] = VIDE;
        }
    } else if (snoopy->y == 4 && snoopy->x == 6) {
        if (plateau[4][6] != VIDE) {
            plateau[4][6] = VIDE;
        }
    } else if (snoopy->y == 6 && snoopy->x == 12) {
        if (plateau[6][12] != VIDE) {
            plateau[6][12] = VIDE;
        }
    } else if (snoopy->y == 9 && snoopy->x == 8) {
        if (plateau[9][8] != VIDE) {
            plateau[9][8] = VIDE;
        }
    } else if (snoopy->y == 9 && snoopy->x == 12) {
        if (plateau[9][12] != VIDE) {
            plateau[9][12] = VIDE;
        }
    }
}

void blocBonusN1(int plateau[LIGNES][COLONNES]) {
    plateau[1][4] = BONUS;
    plateau[3][7] = BONUS;
    plateau[4][1] = BONUS;
    plateau[8][15] = BONUS;
}

void collecteBlocBonusN1(int plateau[LIGNES][COLONNES], Snoopy *snoopy) {
    if (snoopy->y == 1 && snoopy->x == 4) {
        if (plateau[1][4] != VIDE) {
            plateau[1][4] = VIDE;
            vie++;
        }
    } else if (snoopy->y == 3 && snoopy->x == 7) {
        if (plateau[3][7] != VIDE) {
            plateau[3][7] = VIDE;
            vie--;
        }
    } else if (snoopy->y == 4 && snoopy->x == 1) {
        if (plateau[4][1] != VIDE) {
            plateau[4][1] = VIDE;
            vie--;
        }
    } else if (snoopy->y == 8 && snoopy->x == 15) {
        if (plateau[8][15] != VIDE) {
            plateau[8][15] = VIDE;
            vie++;
        }
    }
}

void blocBonusN2(int plateau[LIGNES][COLONNES]) {
    plateau[3][5] = BONUS;
    plateau[5][7] = BONUS;
    plateau[5][13] = BONUS;
    plateau[6][2] = BONUS;
    plateau[7][16] = BONUS;
}

void collecteBlocBonusN2(int plateau[LIGNES][COLONNES], Snoopy *snoopy) {
    if (snoopy->y == 3 && snoopy->x == 5) {
        if (plateau[3][5] != VIDE) {
            plateau[3][5] = VIDE;
            vie--;
        }
    } else if (snoopy->y == 5 && snoopy->x == 7) {
        if (plateau[5][7] != VIDE) {
            plateau[5][7] = VIDE;
            vie++;
        }
    } else if (snoopy->y == 5 && snoopy->x == 13) {
        if (plateau[5][13] != VIDE) {
            plateau[5][13] = VIDE;
            vie--;
        }
    } else if (snoopy->y == 6 && snoopy->x == 2) {
        if (plateau[6][2] != VIDE) {
            plateau[6][2] = VIDE;
            vie++;
        }
    } else if (snoopy->y == 7 && snoopy->x == 16) {
        if (plateau[7][16] != VIDE) {
            plateau[7][16] = VIDE;
            vie--;
        }
    }
}

void blocPlusLoinN1(int plateau[LIGNES][COLONNES]) {
    plateau[5][7] = PLUSLOIN;
    plateau[8][11] = PLUSLOIN;
}

void blocPlusLoinN2(int plateau[LIGNES][COLONNES]) {
    plateau[3][3] = PLUSLOIN;
    plateau[7][14] = PLUSLOIN;
}

void blocTeleporterN2(int plateau[LIGNES][COLONNES]) {
    plateau[10][4] = TELEPORTE;
}

void collecteBlocTPN2(int plateau[LIGNES][COLONNES], Snoopy *snoopy) {
    if (snoopy->y == 10 && snoopy->x == 4) {
        if (plateau[10][4] != VIDE) {
            plateau[10][4] = VIDE;
        }
    }
}

void rebondir(Ball *ball, int plateau[LIGNES][COLONNES], Snoopy *snoopy, PositionInitiale positionInitialeBalle,
              PositionInitiale positionInitialeSnoopy) {
    int suivantX = ball->x + ball->direction_x;
    int suivantY = ball->y + ball->direction_y;
    if (suivantX == snoopy->x && suivantY == snoopy->y) {
        vie--;
        ball->x = positionInitialeBalle.x;
        ball->y = positionInitialeBalle.y;
        snoopy->x = positionInitialeSnoopy.x;
        snoopy->y = positionInitialeSnoopy.y;

        Vie();
        return;
    }
    if (plateau[suivantY][suivantX] == 0xC1) {
        // Bord supérieur
        ball->direction_y = -ball->direction_y;
    } else if (plateau[suivantY][suivantX] == 0xC2) {
        // Bord inférieur
        ball->direction_y = -ball->direction_y;
    } else if (plateau[suivantY][suivantX] == 0xC3) {
        // Bord droit
        ball->direction_x = -ball->direction_x;
    } else if (plateau[suivantY][suivantX] == 0xB4) {
        // Bord gauche
        ball->direction_x = -ball->direction_x;
    }
    if (suivantX == 0 && suivantY == 0) {
        // Coin supérieur gauche
        ball->direction_x = -ball->direction_x;
        ball->direction_y = -ball->direction_y;
    } else if (suivantX == COLONNES - 1 && suivantY == 0) {
        // Coin supérieur droit
        ball->direction_x = -ball->direction_x;
        ball->direction_y = -ball->direction_y;
    } else if (suivantX == 0 && suivantY == LIGNES - 1) {
        // Coin inférieur gauche
        ball->direction_x = -ball->direction_x;
        ball->direction_y = -ball->direction_y;
    } else if (suivantX == COLONNES - 1 && suivantY == LIGNES - 1) {
        // Coin inférieur droit
        ball->direction_x = -ball->direction_x;
        ball->direction_y = -ball->direction_y;
    }
}

void affichageSnoopy() {
    color(3, 0);
    printf(
        "                                                                                                            \n"
        "      =@@@@@      #@@@@@%            =@@@@@@#              -@@@@@@%             #@@@@@:   %%%%%+    .%%%%%  \n"
        "    @@@@@@@@   .@@@@@@@@@@@       @@@@@@@@@@@@@@        %@@@@@@@@@@@@@       *@@@@@@@@@@@  @@@@@@  %@@@@@   \n"
        "   @@@@@@@@@  .@@@@@@@@@@@@@    +@@@@@@@@@@@@@@@@#    :@@@@@@@@@@@@@@@@@    @@@@@@@@@@@@@@  @@@@@@@@@@@@    \n"
        "   @@@@@@*    @@@@@@  @@@@@@.  *@@@@@@@    %@@@@@@%   @@@@@@@    *@@@@@@@   @@@@@@  @@@@@@:  #@@@@@@@@@     \n"
        "   @@@@@@-    @@@@@@  -@@@@@-  @@@@@@        @@@@@@  @@@@@@.       @@@@@@.  @@@@@#:@@@@@@@    :@@@@@@#      \n"
        "   %@@@@@@-   @@@@@@  -@@@@@-  @@@@@@        @@@@@@- @@@@@@        =@@@@@=  @@@@@#=@@@@@@*     @@@@@@       \n"
        "    @@@@@@@   @@@@@@  -@@@@@-  @@@@@@        @@@@@@  @@@@@@.       @@@@@@.  @@@@@#=@@@@@       @@@@@@       \n"
        "    *@@@@@@   @@@@@@  -@@@@@-  *@@@@@@@    #@@@@@@%  .@@@@@@@    +@@@@@@@   @@@@@#             @@@@@@       \n"
        " =@@@@@@@@@   @@@@@@  -@@@@@-   +@@@@@@@@@@@@@@@@%    :@@@@@@@@@@@@@@@@@    @@@@@#             @@@@@@       \n"
        " =@@@@@@@@    @@@@@@  -@@@@@-     @@@@@@@@@@@@@@        %@@@@@@@@@@@@@      @@@@@#             @@@@@@       \n"
        " :@@@@@#      %@@@@@  :@@@@@:        @@@@@@@@.             %@@@@@@@:        @@@@@+             @@@@@@       \n"
        "\n");
    color(15, 0);
}

void affichageMenu() {
    printf("\n        ***** BIENVENUE DANS LE MENU DU JEU ******\n");
    printf("\n               1: Regles du jeu\n"
        "\n               2: Nouvelle Partie\n"
        "\n               3: Charger une Partie\n"
        "\n               4: Mot de Passe\n"
        "\n               5: Scores\n"
        "\n               6: Quitter\n"
        "\n       APPUYEZ SUR Z POUR MONTER ET S POUR DESCENDRE");
}

void deplacementFleche() {
    while (1) // boucle et événement de fin d’éxécution
    {
        if (kbhit()) {
            char key = getch();
            switch (key) {
                case 's':
                    if (yfleche + 2 <= yflechemax) {
                        yfleche += 2;
                    }
                    break;
                case 'z':
                    if (yfleche - 2 >= yflechemin) { yfleche -= 2; }
                    break;
                case '\r':
                    if (yfleche == 16) {
                        system("cls");
                        RegleDuJeu();
                        printf("\nAPPUYER SUR UNE TOUCHE POUR REVENIR AU MENU");
                        getch();
                    }
                    if (yfleche == 18) {
                        system("cls");
                        niveau1();
                        if (vie == 0) {
                            break;
                        }
                        compteurOiseau = 0;
                        niveau2();
                    }
                    if (yfleche == 20) {
                        system("cls");
                        printf("\nCHARGEMENT DE LA PARTIE\n");
                        getch();
                    }
                    if (yfleche == 22) {
                        system("cls");
                        affichageMenuNiveau();
                        gotoligcol(yflecheNiveau, xflecheNiveau);
                        printf("<--");
                        deplacementFlecheNiveau();
                    }
                    if (yfleche == 24) {
                        system("cls");
                        ScoreDuJeu();
                        getch();
                    }
                    if (yfleche == 26) {
                        exit(0);
                    }
                    break;
                default:
                    break;
            }
            {
                system("cls");
                affichageSnoopy();
                affichageMenu();
                gotoligcol(yfleche, xfleche);
                printf(" <--");
            }
        }
    }
}

void collecteOiseau(int plateau[LIGNES][COLONNES], Snoopy *snoopy) {
    if (snoopy->y == 1 && snoopy->x == 1) {
        if (plateau[1][1] != VIDE) {
            plateau[1][1] = VIDE;
            compteurOiseau++;
        }
    } else if (snoopy->y == 1 && snoopy->x == 20) {
        if (plateau[1][20] != VIDE) {
            plateau[1][20] = VIDE;
            compteurOiseau++;
        }
    } else if (snoopy->y == 10 && snoopy->x == 1) {
        if (plateau[10][1] != VIDE) {
            plateau[10][1] = VIDE;
            compteurOiseau++;
        }
    } else if (snoopy->y == 10 && snoopy->x == 20) {
        if (plateau[10][20] != VIDE) {
            plateau[10][20] = VIDE;
            compteurOiseau++;;
        }
    }
    CONSOLE_SCREEN_BUFFER_INFO csbi; // Structure pour stocker des informations sur l'écran de la console
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    // On obtient des informations sur l'écran de la console
    int currentLig = csbi.dwCursorPosition.Y; // position du curseur
    int currentCol = csbi.dwCursorPosition.X; // position du curseur
    gotoligcol(18, 60);
    printf("| Vous avez %d %c ", compteurOiseau, 0x0E);
    gotoligcol(currentLig, currentCol);
}

void RegleDuJeu() {
    printf("Bienvenue sur le jeu Snoopy, voici les regles du jeu : \n"
        "\nLe but du jeu est simple, Snoopy est represente sur un plateau de jeu de 20 par 10, par un symbole S.\n"
        "Il ne peut se deplacer que verticalement et horizontalement Son but est de recuperer 4 oiseaux aux 4 coins du niveau en un temps imparti.\n"
        "Le probleme est que ces 4 oiseaux ne sont pas si faciles a recuperer. Une balle rebondit constamment dans le niveau afin de freiner Snoopy dans sa quete.\n"
        "Mais ce n'est pas tout, d'autres pieges sont presents comme des teleporteurs que la balle peut emprunter ou des cases\n"
        "piegees, voir meme des blocs a pousser ou a casser... \n "
        "\nSi tout est clair pour vous, alors n'attendez plus et commencez a jouer !!\n");
}

void deplacementSnoopy(Snoopy *snoopy, int plateau[LIGNES][COLONNES]) {
    int snoopyXMax = 21;
    int snoopyXMin = 1;
    int snoopyYMax = 11;
    int snoopyYMin = 1;
    if (kbhit()) {
        char direction = getch();
        switch (direction) {
            case 'z': {
                const int caseDuHaut = plateau[snoopy->y - 1][snoopy->x];
                if (snoopy->y == snoopyYMin) {
                } else if (caseDuHaut == BLOCFIXE) {
                } else if (caseDuHaut == BONUS) {
                    snoopy->y--;
                } else if (caseDuHaut == TELEPORTE) {
                    snoopy->y = 3;
                    snoopy->x = 15;
                } else if (caseDuHaut == VIDE) {
                    snoopy->y--;
                } else if (caseDuHaut == PLUSLOIN) {
                    snoopy->y--;
                    snoopy->y--;
                    snoopy->y--;
                } else if (caseDuHaut == DEPLACABLE) {
                    snoopy->y--;
                } else if (caseDuHaut == OISEAU) {
                    snoopy->y--;
                }
                break;
            }
            case 's': {
                const int caseDuBas = plateau[snoopy->y + 1][snoopy->x];
                if (snoopy->y == snoopyYMax) {
                } else if (caseDuBas == BLOCFIXE) {
                } else if (caseDuBas == BONUS) {
                    snoopy->y++;
                } else if (caseDuBas == TELEPORTE) {
                    snoopy->y = 3;
                    snoopy->x = 15;
                } else if (caseDuBas == VIDE) {
                    snoopy->y++;
                } else if (caseDuBas == PLUSLOIN) {
                    snoopy->y++;
                    snoopy->y++;
                    snoopy->y++;
                } else if (caseDuBas == DEPLACABLE) {
                    snoopy->y++;
                } else if (caseDuBas == OISEAU) {
                    snoopy->y++;
                }
                break;
            }
            case 'q': {
                const int caseDeGauche = plateau[snoopy->y][snoopy->x - 1];
                if (snoopy->x == snoopyXMin) {
                } else if (caseDeGauche == BLOCFIXE) {
                } else if (caseDeGauche == BONUS) {
                    snoopy->x--;
                } else if (caseDeGauche == TELEPORTE) {
                    snoopy->y = 3;
                    snoopy->x = 15;
                } else if (caseDeGauche == VIDE) {
                    snoopy->x--;
                } else if (caseDeGauche == PLUSLOIN) {
                    snoopy->x--;
                    snoopy->x--;
                    snoopy->x--;
                } else if (caseDeGauche == DEPLACABLE) {
                    snoopy->x--;
                } else if (caseDeGauche == OISEAU) {
                    snoopy->x--;
                }
                break;
            }
            case 'd': {
                const int caseDeDroite = plateau[snoopy->y][snoopy->x + 1];
                if (snoopy->x == snoopyXMax) {
                } else if (caseDeDroite == BLOCFIXE) {
                } else if (caseDeDroite == BONUS) {
                    snoopy->x++;
                } else if (caseDeDroite == TELEPORTE) {
                    snoopy->y = 3;
                    snoopy->x = 15;
                } else if (caseDeDroite == VIDE) {
                    snoopy->x++;
                } else if (caseDeDroite == PLUSLOIN) {
                    snoopy->x++;
                    snoopy->x++;
                    snoopy->x++;
                } else if (caseDeDroite == DEPLACABLE) {
                    snoopy->x++;
                } else if (caseDeDroite == OISEAU) {
                    snoopy->x++;
                }
                break;
            }
            default:
                break;
        }
    }
}

void sleep_ms(int milliseconds) {
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

void Vie() {
    CONSOLE_SCREEN_BUFFER_INFO csbi; // Structure pour stocker des informations sur l'écran de la console
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    // On obtient des informations sur l'écran de la console
    int currentLig = csbi.dwCursorPosition.Y; // position du curseur
    int currentCol = csbi.dwCursorPosition.X; // position du curseur

    // Déplacer le curseur à une position spécifique pour afficher le texte des vies
    gotoligcol(17, 60);
    if (vie != 0) {
        printf("| Encore %d %c", vie, COEUR);
    }
    gotoligcol(currentLig, currentCol);
}

void TempsRestantN1() {
    static time_t debut = 0; // Garder le temps de début constant entre les itérations
    time_t fin = time(NULL);

    if (debut == 0) {
        debut = fin; // Initialise le temps de début lors de la première itération
    }

    tempsEcouleN1 = (int) (fin - debut);
    tempsRestant = (tempsMaximal - tempsEcouleN1);

    if (tempsEcouleN1 >= tempsMaximal) {
        vie--; // Décrémentez la vie
        Vie();
        // Réinitialisez le compteur
        debut = fin;
    }
}

void afficherTempsRestantN1() {
    TempsRestantN1();
    gotoligcol(19, 60); // À ajuster en fonction de votre mise en page
    printf("| Il vous reste %d secondes", tempsRestant);
}

void TempsRestantN2() {
    static time_t debut = 0; // Garder le temps de début constant entre les itérations
    time_t fin = time(NULL);

    if (debut == 0) {
        debut = fin; // Initialise le temps de début lors de la première itération
    }

    tempsEcouleN2 = (int) (fin - debut);
    tempsRestant = (tempsMaximal - tempsEcouleN2);

    if (tempsEcouleN2 >= tempsMaximal) {
        vie--; // Décrémentez la vie
        Vie();
        // Réinitialisez le compteur
        debut = fin;
    }
}

void afficherTempsRestantN2() {
    TempsRestantN2();
    gotoligcol(19, 60); // À ajuster en fonction de votre mise en page
    printf("| Il vous reste %d secondes", tempsRestant);
}


void niveau1() {
    tempsRestant = 0;
    tempsMaximal = 120;
    tempsEcouleN1 = 0;
    compteurOiseau = 0;
    vie = 3;
    PositionInitiale positionInitialeBalle = {5, 5};
    PositionInitiale positionInitialeSnoopy = {10, 5};

    Ball myBall = {positionInitialeBalle.x, positionInitialeBalle.y, 1, 1};
    Snoopy mySnoopy = {positionInitialeSnoopy.x, positionInitialeSnoopy.y, 0};
    int plateau[LIGNES][COLONNES];

    initialiserPlateau(plateau);
    afficherOiseau(plateau);
    blocNonDeplacableN1(plateau);
    blocBonusN1(plateau);
    blocDeplacableN1(plateau);
    blocPlusLoinN1(plateau);
    while (compteurOiseau < 4 && vie != 0) {
        affichageSnoopy();
        afficherPlateau(plateau, myBall, mySnoopy);
        rebondir(&myBall, plateau, &mySnoopy, positionInitialeBalle, positionInitialeSnoopy);
        deplacementSnoopy(&mySnoopy, plateau);
        collecteBlocBonusN1(plateau, &mySnoopy);
        collecteOiseau(plateau, &mySnoopy);
        TempsRestantN1();
        afficherTempsRestantN1();
        collecteBlocDeplacableN1(plateau, &mySnoopy);
        Vie();
        myBall.x += myBall.direction_x;
        myBall.y += myBall.direction_y;
        sleep_ms(10);
        system("cls");
    }
    if (compteurOiseau == 4) {
        system("cls"); // efface la console
        win();
        ScoreN1();
        printf("\nFelicitations, vous avez termine le niveau 1 !\n");
        getch(); // ferme le programme*/
    }
    if (vie == 0) {
        system("cls"); // efface la console
        gameover();
        ScoreN1();
        printf("\n\n\nVous avez perdu, retentez votre chance en recommencant\n");
        getch(); // ferme le programme*/
    }
    if (vie == 0) {
        printf("\nAppuyer sur une touche pour retourner au menu !\n");
        getch();
    } else if (compteurOiseau == 4) {
        printf("\nAppuyer sur une touche pour passer au niveau 2!\n");
        getch();
    }
}

int verifMotDePasse(int mdpN1) {
    int mdp;
    system("cls");
    printf("\nVeuillez entrer votre mot de passe :\n");

    scanf("%d", &mdp);
    while (mdp != mdpN1) {
        printf("Code errone, veuillez reessayer :\n");
        scanf("%d", &mdp);
    }
    printf("\nMot de passe correct !\n");
    Sleep(1000);
}

int verifMotDePasseN2(int mdpN2) {
    int mdp;
    system("cls");
    printf("\nVeuillez entrer votre mot de passe :\n");
    scanf("%d", &mdp);
    while (mdp != mdpN2) {
        printf("Code errone, veuillez reessayer :\n");
        scanf("%d", &mdp);
    }
    printf("\nMot de passe correct !\n");
    Sleep(1000);
}

void affichageMenuNiveau() {
    system("cls");
    affichageSnoopy();
    printf("\n        ***** VOICI LES DIFFERENTS NIVEAUX DU JEU ******\n");
    printf("\n                     1: Niveau1\n"
        "\n                     2: Niveau2\n"
        "\n       VEUILLEZ SELECTIONNER LE NIVEAU QUE VOUS VOULEZ LANCER");
}

void deplacementFlecheNiveau() {
    while (1) // boucle et événement de fin d’éxécution
    {
        if (kbhit()) {
            char key = getch();
            switch (key) {
                case 's':
                    if (yflecheNiveau + 2 <= yflecheNiveauMax) {
                        yflecheNiveau += 2;
                    }
                    break;
                case 'z':
                    if (yflecheNiveau - 2 >= yflecheNiveauMin) { yflecheNiveau -= 2; }
                    break;
                case 'p':
                    break;
                case '\r':
                    if (yflecheNiveau == 16) {
                        system("cls");
                        verifMotDePasse(mdpN1);
                        compteurOiseau = 0;
                        niveau1();
                        printf("\nAPPUYER SUR UNE TOUCHE POUR REVENIR AU MENU");
                        getch();
                    }
                    if (yflecheNiveau == 18) {
                        system("cls");
                        verifMotDePasseN2(mdpN2);
                        compteurOiseau = 0;
                        niveau2();
                        getch();
                    }
                    break;
                default:
                    break;
            }
            {
                system("cls");
                affichageSnoopy();
                affichageMenuNiveau();
                gotoligcol(yflecheNiveau, xflecheNiveau);
                printf(" <--");
            }
        }
    }
}

void niveau2() {
    tempsRestant = 0;
    tempsMaximal = 120;
    tempsEcouleN2 = 0;
    compteurOiseau = 0;
    vie = 3;
    PositionInitiale positionInitialeBalle = {6, 1};
    PositionInitiale positionInitialeSnoopy = {10, 5};

    Ball myBall = {positionInitialeBalle.x, positionInitialeBalle.y, 1, 1};
    Snoopy mySnoopy = {positionInitialeSnoopy.x, positionInitialeSnoopy.y};
    int plateau[LIGNES][COLONNES];

    initialiserPlateau(plateau);
    afficherOiseau(plateau);
    blocNonDeplacableN2(plateau);
    blocBonusN2(plateau);
    blocDeplacableN2(plateau);
    blocPlusLoinN2(plateau);
    blocTeleporterN2(plateau);
    while (compteurOiseau < 4 && vie != 0) {
        affichageSnoopy();
        afficherPlateau(plateau, myBall, mySnoopy);
        Vie();
        rebondir(&myBall, plateau, &mySnoopy, positionInitialeBalle, positionInitialeSnoopy);
        deplacementSnoopy(&mySnoopy, plateau);
        collecteBlocBonusN2(plateau, &mySnoopy);
        collecteOiseau(plateau, &mySnoopy);
        TempsRestantN2();
        afficherTempsRestantN2();
        collecteBlocDeplacableN2(plateau, &mySnoopy);
        collecteBlocTPN2(plateau, &mySnoopy);
        myBall.x += myBall.direction_x;
        myBall.y += myBall.direction_y;
        sleep_ms(10);
        system("cls");
    }
    if (compteurOiseau == 4) {
        system("cls"); // efface la console
        win();
        gotoligcol(30, 40);
        ScoreN2();
        printf("\nFelicitations, vous avez termine le niveau 2 !\n");
        getch(); // ferme le programme*/
    }
    if (vie == 0) {
        system("cls"); // efface la console
        gameover();
        ScoreN2();
        printf("\n\nVous avez perdu, retentez votre chance en recommencant\n");
        getch(); // ferme le programme*/
    }
    printf("\nAppuyer sur une touche pour retourner au menu !\n");
    getch();
}

void ScoreN1() {
    scoreN1 = 0;
    scoreN1 = tempsEcouleN1 * 100;
    printf("\nVotre score du niveau 1 est de : %d\n", scoreN1);
}

void ScoreN2() {
    scoreN2 = 0;
    scoreN2 = tempsEcouleN2 * 100;
    printf("\nVotre score du niveau 2 est de : %d\n", scoreN2);
}

void ScoreDuJeu() {
    scoreJeu = 0;
    ScoreN1();
    ScoreN2();
    scoreJeu = scoreN1 + scoreN2;
    printf("\n\nVotre score total est de : %d", scoreJeu);
}

void gameover() {
    color(12, 0);
    printf(
        "                                                                                                                                                          \n"

        "                XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX                \n"
        "            XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX            \n"
        "          XXXXXXXXXX                                                                                                                  XXXXXXXXXX          \n"
        "        XXXXXXX                                                                                                                             XXXXXX        \n"
        "       XXXXXX      XXXXXX                                                                                                                     XXXXX       \n"
        "      XXXXX     XXXXXXXXXXXXX                                                                                                                  XXXXX      \n"
        "     XXXXX     XXXXXX  XXXXXXX                                                                                                                  XXXXX     \n"
        "     XXXXX    XXXXXXX   XXXXXX    XXXXXXXXXXX XXXXXXXXXXXX XXXXXXX     XXXXXXX         XXXXXXXXX  XXXXXX    XXXXX   XXXXXXX    XXXXXXXXXXX       XXXX     \n"
        "     XXXX     XXXXXXX           XXXXX   XXXXX XXXXXX XXXXXXXXXXXXXX  XXXX  XXXXX     XXXXX   XXXXX XXXXX   XXXXX  XXXX   XXXX  XXXXXX XXXXX      XXXX     \n"
        "     XXXX     XXXXXXXXXXXXXXXX XXXXXX   XXXXX XXXXX   XXXXX   XXXXX XXXXXXXXXXXXX    XXXXX   XXXXXX XXXXX XXXXXX XXXXXXXXXXXXX XXXXX             XXXX     \n"
        "     XXXXX     XXXXXX   XXXXXX XXXXXX   XXXXX XXXXX   XXXXX   XXXXX XXXXX            XXXXX   XXXXXX  XXXXXXXXXX  XXXXX         XXXXX             XXXX     \n"
        "     XXXXX     XXXXXX   XXXXX  XXXXXX   XXXXX XXXXX   XXXXX   XXXXX XXXXX   XXXXX    XXXXX   XXXXX   XXXXXXXXX   XXXXX   XXXXX XXXXX            XXXXX     \n"
        "      XXXXX     XXXXXXXXXXXX    XXXXXXXXXXXXX XXXXX   XXXXX   XXXXX  XXXXXXXXXXX      XXXXXXXXXXX     XXXXXXX     XXXXXXXXXXX  XXXXX           XXXXX      \n"
        "       XXXXXX       XXXXX          XXXXXXXXXX  XXXX   XXXX     XXXX     XXXXX             XXX          XXXXX          XXXX     XXXXX          XXXXX       \n"
        "        XXXXXXX                                                                                                                             XXXXXX        \n"
        "          XXXXXXXXXX                                                                                                                   XXXXXXXXX          \n"
        "            XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX            \n"
        "                XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX               \n"

        "                                                                                                                                                          ");
    color(15, 0);
}

void win() {
    color(10, 0);
    printf(
        "                                                                     @@@@@@@                                                                   =@@@@@            \n"
        "                                                                   @@@@@@@@@.                                                               @@@@@@@@@*           \n"
        "                                                                 :@@@@@@@@@@                                                                @@@@@*               \n"
        "          @@@@@#            @@@*     @@@@@@@@@@@@      %@@@-     @@@@@@@-                                                                                        \n"
        "        @@@@@@@@@@@@      @@@@@@@   @@@@@@@@@@@@@@   @@@@@@@@   @@@@@@@                @@@@@          =@@@@*          @@@@@     %@@@@-   -@@@@@@@@@@@@@          \n"
        "        @@@@@@@@@@@@@     %@@@@@@   @@@@@@@@@@@@@@    @@@@@@@   @@@@@@@                @@@@@%       @@@@@@@@@@#      :@@@@@     @@@@@#   @@@@@@@@@@@@@@+         \n"
        "        @@@@@@--@@@@@@     @@@@@#   @@@@@@=           @@@@@@@@  @@@@@@@                @@@@@%     +@@@@@@@@@@@@@     -@@@@@     @@@@@#  +@@@@@@@@@@@@@@          \n"
        "        @@@@@@  *@@@@+     @@@@@.   @@@@@%            @@@@@@@@@@@@@@@@=                @@@@@@     @@@@@@*:@@@@@@@    +@@@@@     @@@@@#  *@@@@@@                  \n"
        "        @@@@@@@@@@@@#      @@@@@    @@@@@@.           @@@@@@@@@@@@@@@@#                @@@@@@    @@@@@@     @@@@@-   #@@@@@     @@@@@#  *@@@@@-                  \n"
        "        @@@@@@@@@@@@@      @@@@@    @@@@@@@@@@@       @@@@@@@@@@@@@@@@@                @@@@@@   @@@@@@      +@@@@@   %@@@@@     @@@@@*  *@@@@@@                  \n"
        "        @@@@@@@@@@@@@@@    @@@@@    @@@@@@@@@@@       @@@@@@.@@@@@@@@@@                @@@@@@   @@@@@@       @@@@@   %@@@@@     @@@@@+  *@@@@@@@@@@@             \n"
        "        @@@@@-    @@@@@@   %@@@@    @@@@@@@           @@@@@   -@@@@@@@@                @@@@@@   @@@@@@       @@@@@   #@@@@@     @@@@@=  *@@@@@@@@@@@             \n"
        "        @@@@@      @@@@@   %@@@@.   @@@@@@            @@@@@    -@@@@@@@                @@@@@=   @@@@@@       @@@@@   .@@@@@     @@@@@.  +@@@@@@@++@%             \n"
        "        @@@@@      @@@@@   @@@@@+   @@@@@             @@@@@     @@@@@@@      @@@@@@   %@@@@@    @@@@@@      -@@@@@    @@@@@     @@@@@   =@@@@@@                  \n"
        "        @@@@@      @@@@@   @@@@@%  =@@@@@=            @@@@@     @@@@@@@      @@@@@@-  @@@@@@     @@@@@@     @@@@@     @@@@@@   #@@@@@   :@@@@@                   \n"
        "        @@@@@*     @@@@@  .@@@@@@  @@@@@@@            @@@@@@    @@@@@@@      :@@@@@@@@@@@@@       @@@@@@%%@@@@@@       @@@@@@@@@@@@@     @@@@@                   \n"
        "        @@@@@@    @@@@@*  @@@@@@@  =@@@@@@@@          @@@@@@    @@@@@@@       .@@@@@@@@@@@         @@@@@@@@@@@@         @@@@@@@@@@@      @@@@@@                  \n"
        "              .@@@@@@@@              @@@@@@@@@@@@+                               @@@@@@@             @@@@@@@@             @@@@@@@        @@@@@@@                 \n"
        "   @@@@@@@@@@@@@@@@@@                  +@@@@@@@@@@@@@@@@@                                                                                :@@@@@@@@@@             \n"
        "   @@@@@@@@@@@@@@@@                        #@@@@@@@@@@@@@                                                                                  @@@@@@@@@@@@          \n"
        "   @@@@@@@@@@-                                    %@@@@@@                                                                                     .@@@@@@@@          \n"
        "                                                                                                                                                    *@@          \n"
        "                                                                                                                                                                 \n"
        "                                                                                                                                                                 \n"
        "                                                                                                                                                                 \n"
        "                                                                                                                                                                 ");
    color(15, 0);
}
