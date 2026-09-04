#include <stdio.h>
#include "fonction.h"
#include <unistd.h>
#include "windows.h"
#include<time.h>

int main() {
    int yfleche = 16;
    int xfleche = 44;
    affichageSnoopy();
    affichageMenu();
    gotoligcol(yfleche, xfleche);
    printf("<--");
    deplacementFleche();
    return 0;

}
