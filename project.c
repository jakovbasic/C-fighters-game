#include "project.h"
#include <stdlib.h>
#include <stdio.h>
#include "string.h"

int main(void){

Game game;
game.numFighters = 0;
game.fighters = malloc(sizeof(Fighter)+1);
game.run = 1;
while(game.run){
    char* input = malloc(sizeof(char)*1000);
    fgets(input, 1000, stdin);

    checkInput(&game, input);
    free(input);
    
    }
    return 0;
}

// tarkistaa syötteen:

void checkInput(Game* game, char* input){

    switch (input[0]){
    case 'A':
        checkA(game, input);
        break;
    case 'H':
        checkH(game, input);
        break;
    case 'L':
        gameStatus(game);
        break;
    case 'W':
        saveGame(game, input);
        break;
    case 'O':
        loadGame(game, input);
        break;
    case 'Q':
        exitGame(game);
        break;
    default:
        printf("Invalid command %s\n",input);
        break;
    }
}

// tarkistaa A komennon parametrit:

void checkA(Game* game, char* input){

    char id;
    char* name = malloc(strlen(input));
    int hp;
    char* wpn = malloc(strlen(input));
    int dmg;

    hp = 0;
    
    sscanf(input,"%c %s %d %s %d", &id, name, &hp, wpn, &dmg);

    int Aval = addFighter(game, name, hp, wpn, dmg);

    if(Aval) {
        printf("SUCCESS\n");
        free(name);
        free(wpn);
    }
    else{
        printf("\n");
        free(name);
        free(wpn);
    }
}

// tarkistaa H komennon parametrit:

void checkH(Game* game, char* input){

    int splits = 0;
    char* pch;
    pch = strtok(input," ");
    while (pch != NULL){
        splits++;
        pch = strtok (NULL, " \n");
    }



    char id;
    char* par1 = malloc(strlen(input));
    char* par2 = malloc(strlen(input));

    sscanf(input,"%c %s %s", &id, par1, par2);

    char* attacker = calloc((int)strlen(par1), sizeof(char));
    char* target = calloc((int)strlen(par2), sizeof(char));
    strncpy(attacker,par1,(int)strlen(par1));
    strncpy(target,par2,(int)strlen(par2));
    free(par1);
    free(par2);

    int Hval = attack(game, attacker, target);

    if(Hval) {
        printf("SUCCESS\n");
        free(attacker);
        free(target);
    }
    else{
        printf("\n");
        free(attacker);
        free(target);
    }
}

// komentoa A vastaava funktio:
// lisää taistelijan.
// palauttaa arvon int 1, jos taistelija lisätään onnistuneesti, ja int 0, jos lisäys epäonnistuu.

int addFighter(Game* game, char* name, int hp, char* weapon, int damage){

    if(name == NULL || hp <= 0 || weapon == NULL || damage <= 0) return 0;

    int check = 0;
    if(game->numFighters != 0){
        for(int i = 0; i< game->numFighters; i++){
            if(strcmp(game->fighters[i].name,name) == 0) check++;
        }
    } 
    if(check > 0) return 0;
    
    if(game->numFighters > 0){
        game->fighters = realloc(game->fighters, (sizeof(Fighter)+1)*(game->numFighters+1)); 
        
    }

    game->fighters[game->numFighters].name = malloc(strlen(name)+1);
    strcpy(game->fighters[game->numFighters].name, name);
    game->fighters[game->numFighters].hp = hp;
    game->fighters[game->numFighters].weapon = malloc(strlen(weapon)+1);
    strcpy(game->fighters[game->numFighters].weapon, weapon);
    game->fighters[game->numFighters].damage = damage;
    game->fighters[game->numFighters].xp = 0;

    game->numFighters++;

    return 1;
}

// komentoa H vastaava funktio:
// attacker parametria vastaava taistelija vahingoittaa target parametria vastaavaa taistelijaa.
// palauttaa 1 mikäli hyökkäys tapahtuu onnistuneesti, ja 0 jos hyökkäys epäonnistuu.

int attack(Game* game, char* attacker, char* target){

    if(attacker == NULL || target == NULL || strcmp(attacker,target) == 0 ) return 0;
    int checkAtt = 0;
    int checkTrg = 0;
    int iAtt = 0;
    int iTrg = 0;
    
    for(int i = 0; i < game->numFighters; i++){

        if(strcmp(game->fighters[i].name, attacker) == 0) {
            checkAtt++;
            iAtt = i;
            }
        if(strcmp(game->fighters[i].name, target) == 0) {
            checkTrg++;
            iTrg = i;
            }
    } if(checkAtt != 1 && checkTrg != 1) return 0;
    if(game->fighters[iAtt].hp <= 0 || game->fighters[iTrg].hp <= 0) return 0;

    int totalDmg = game->fighters[iAtt].damage;
    if(game->fighters[iAtt].damage > game->fighters[iTrg].hp) totalDmg = game->fighters[iTrg].hp;


    game->fighters[iTrg].hp = game->fighters[iTrg].hp - totalDmg;
    
    game->fighters[iAtt].xp = game->fighters[iAtt].xp + totalDmg;
    
    printf("%s attacked %s with %s by %d damage.\n", game->fighters[iAtt].name, game->fighters[iTrg].name, game->fighters[iAtt].weapon, game->fighters[iAtt].damage);
    printf("%s has %d hit points remaining.\n", game->fighters[iTrg].name, game->fighters[iTrg].hp);
    printf("%s gained %d experience points.\n", game->fighters[iAtt].name, totalDmg);

    return 1;
}

// apufunktio funktiolle gameStatus:

int compare(const void * a, const void * b){

   const Fighter* p1 = a;
   const Fighter* p2 = b;
    return (p2->xp - p1->xp);
}

// komentoa L vastaava funktio:

void gameStatus(Game* game){

    qsort(game->fighters, game->numFighters, sizeof(Fighter), compare);

    for(int i=0; i < game->numFighters; i++){
        printf("%s %d %d %s %d\n", game->fighters[i].name, game->fighters[i].hp, game->fighters[i].xp, game->fighters[i].weapon, game->fighters[i].damage);
    }
    printf("SUCCESS\n");
}

// komentoa W vastaava funktio:

void saveGame(Game* game, char* input){
    char id;
    char* fileName = malloc(strlen(input));

    sscanf(input,"%c %s", &id, fileName);
    if(!fileName) {
        free(fileName);
        printf("\n");
    } else{

        FILE* fileptr = fopen(fileName, "w");

        for(int i = 0; i < game->numFighters; i++){
            fprintf(fileptr, "%s %d %d %s %d\n", game->fighters[i].name, game->fighters[i].hp, game->fighters[i].xp, game->fighters[i].weapon, game->fighters[i].damage);
        }
        fclose(fileptr);
        free(fileName);
        printf("SUCCESS\n");
    }
}

// komentoa O vastaava funktio:

void loadGame(Game* game, char* input){
    char id;
    char* fileName = malloc(strlen(input));

    sscanf(input,"%c %s", &id, fileName);
    if(!fileName) {
        free(fileName);
        printf("\n");
    } else{

        FILE* fileptr = fopen(fileName, "r");

        if(fileptr != NULL){
        int count = 0;
        int val = fgetc(fileptr);
        while (val != EOF){
            if(val == '\n') count++;
            val = fgetc(fileptr);
        }
        rewind(fileptr);
        
        for(int i = 0; i < count; i++){
            fscanf(fileptr, "%s %d %d %s %d\n", game->fighters[i].name, &game->fighters[i].hp, &game->fighters[i].xp, game->fighters[i].weapon, &game->fighters[i].damage);
        }
        fclose(fileptr);
        free(fileName);
        printf("SUCCESS\n");
        }
        else{
            free(fileName);
            printf("\n");
        }
    }
}

// komentoa Q vastaava funktio:

void exitGame(Game* game){
    for(int i = 0; i < game->numFighters; i++){
        free(game->fighters[i].name);
        free(game->fighters[i].weapon);
    }
    free(game->fighters);
    printf("SUCCESS\n");
    game->run = 0;
}

