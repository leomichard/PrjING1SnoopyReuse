#ifndef SNOOPY_FONCTION_H
#define SNOOPY_FONCTION_H

#define LIGNES 12
#define COLONNES 22
#define BALLE 0x9D

typedef struct {
    int x;
    int y;
    int direction_x;
    int direction_y;
} Ball;

typedef struct {
    int x;
    int y;
    unsigned int score;
} Snoopy;

typedef struct {
    int x;
    int y;
} PositionInitiale;

// Fonctions
void initialiserPlateau(int plateau[LIGNES][COLONNES]);
void afficherPlateau(int plateau[LIGNES][COLONNES], Ball b, Snoopy s);
void afficherOiseau(int plateau[LIGNES][COLONNES]);
void rebondir(Ball * ball, int plateau[LIGNES][COLONNES],Snoopy *snoopy, PositionInitiale positionInitialeBalle, PositionInitiale positionInitialeSnoopy);
void blocNonDeplacableN1(int plateau[LIGNES][COLONNES]);
void color(int couleurDuTexte,int couleurDeFond);
void gotoligcol(int lig, int col);
void affichageMenu();
void RegleDuJeu();
void deplacementFleche();
void affichageSnoopy();
void deplacementSnoopy(Snoopy *snoopy, int plateau[LIGNES][COLONNES]);
void blocDeplacableN1(int plateau[LIGNES][COLONNES]);
void collecteOiseau(int plateau[LIGNES][COLONNES], Snoopy *snoopy);
void niveau1();
void Vie();
int verifMotDePasse(int mdp1);
void affichageMenuNiveau();
void deplacementFlecheNiveau();
void blocBonusN1(int plateau[LIGNES][COLONNES]);
void collecteBlocDeplacableN1(int plateau [LIGNES][COLONNES], Snoopy *snoopy);
void collecteBlocBonusN1(int plateau[LIGNES][COLONNES], Snoopy *snoopy);
void blocTeleporterN2(int plateau[LIGNES][COLONNES]);
void blocPlusLoinN1(int plateau[LIGNES][COLONNES]);
void afficherTempsRestantN1();
void afficherTempsRestantN2();
void blocNonDeplacableN2(int plateau[LIGNES][COLONNES]);
void blocBonusN2(int plateau[LIGNES][COLONNES]);
void collecteBlocBonusN2(int plateau[LIGNES][COLONNES], Snoopy *snoopy);
void niveau2();
void blocDeplacableN2(int plateau[LIGNES][COLONNES]);
void collecteBlocDeplacableN2(int plateau [LIGNES][COLONNES], Snoopy *snoopy);
void blocPlusLoinN2(int plateau[LIGNES][COLONNES]);
int verifMotDePasseN2(int mdpN2);
void collecteBlocTPN2(int plateau[LIGNES][COLONNES], Snoopy *snoopy);
void ScoreN1();
void ScoreN2();
void ScoreDuJeu();
void TempsRestantN1();
void TempsRestantN2();
void gameover();
void win();

#endif //SNOOPY_FONCTION_H
