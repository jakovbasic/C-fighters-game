#ifndef _PROJECT__H_
#define _PROJECT__H_


//rakenteita:

typedef struct{
    char* name;
    int hp;
    int xp;
    char* weapon;
    int damage;
} Fighter;

typedef struct game_st Game;

struct game_st{
    Fighter* fighters;
    int numFighters;
    int run;
};


//funktioita:

//syötteen tarkistus:

void checkInput(Game* game, char* input);

void checkA(Game* game, char* input);
void checkH(Game* game, char* input);


//komentoja vastaavat funktiot:

int addFighter(Game* game, char* name, int hp, char* weapon, int damage);
int attack(Game* game, char* attacker, char* target);
void gameStatus(Game* game);
void saveGame(Game* game, char* input);
void loadGame(Game* game, char* input);
void exitGame(Game* game);

// apufunktioita:

int compare(const void * a, const void * b);


#endif //! _PROJECT__H_
