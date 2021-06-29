#ifndef INDIV_GAME
#define INDIV_GAME


//some defines to set the maximum number of some lists or options
#define MAX_AMOUNT_OF_SCORES 3
#define MAX_AMOUNT_OF_CHOICES 3

//this value is for adding it to the streak of the player. it will add each time the player wins a round
#define STREAK_UP_VALUE 1

//enum choiceType is to separate the different options in the game
enum choiceType{ROCK, PAPER, SCISSORS, ERROR_CHOICE};

//enum outcome is to separate the different outcomes of each round on rock,paper,scissors against a foe
enum outcome{WON, LOST, TIE, ERROR_OUTCOME};

//struct game_t is to store values about the game in it, like the streak the player is on or the highscore
typedef struct{
    unsigned int streak;
    unsigned int highScore;
    //char* maxScores[MAX_AMOUNT_OF_SCORES];
} game_t;

//creates a game_t object
game_t* game_create_game();

//generate a random choice for the foe
enum choiceType game_get_choice_PC();

//checks the outcome of the round against a foe
enum outcome game_check_outcome(enum choiceType myChoice, enum choiceType enemyChoice);

//ups the streak counter of the player
void game_streak_up(game_t* gameInfo);

//resets the streak counter
void game_reset_streak(game_t* gameInfo);

//checks between the new streak and the highscore, to check which one is higher
void game_check_score(game_t* gameInfo);

//get a text form of a enum value from the enum choiceType
char* game_get_text_of_enum_choice(enum choiceType type);

//get a text form of a enum value from the enum outcome
char* game_get_text_of_enum_outcome(enum outcome result);

#endif