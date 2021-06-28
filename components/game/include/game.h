#ifndef INDIV_GAME
#define INDIV_GAME

#define MAX_AMOUNT_OF_SCORES 3
#define MAX_AMOUNT_OF_CHOICES 3

enum choiceType{ROCK, PAPER, SCISSORS, ERROR_CHOICE};
enum outcome{WON, LOST, TIE, ERROR_OUTCOME};

typedef struct{
    unsigned int streak;
    unsigned int highScore;
    //char* maxScores[MAX_AMOUNT_OF_SCORES];
} game_t;


game_t* game_create_game();

enum choiceType game_get_choice_PC();
enum outcome game_check_outcome(enum choiceType myChoice, enum choiceType enemyChoice);
void game_streak_up(game_t* gameInfo);
void game_reset_streak(game_t* gameInfo);
void game_check_score(game_t* gameInfo);

char* game_get_text_of_enum_choice(enum choiceType type);
char* game_get_text_of_enum_outcome(enum outcome result);

#endif