#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "game.h"

//this method has no parameters, it returns a game_t* object 
game_t* game_create_game(){
    //allocate memory for the game_t struct object
    game_t* gameInfo = malloc(sizeof(game_t));

    //checks if the memory allocation went correct
    if(gameInfo != NULL){
        //set some default values as placeholders in the game_t struct
        gameInfo->streak = 0;
        gameInfo->highScore = 0;
    }
    
    return gameInfo;
}

//this method is private for this file, it generates a random number between the 0 and the maximum of choices that can be made. This returns a random number
static unsigned int game_random_number(){
    //initialize and calculate some value to generate a random number
    unsigned int min = 0;
    unsigned int max = MAX_AMOUNT_OF_CHOICES;
    unsigned int distance = (max - min);
    unsigned int randomNumber;

    //this loop will go on until ir reaches a value that is bigger/even compare to a calculation for a minimum
    do {
        randomNumber = rand();
    } while(randomNumber >= (RAND_MAX - RAND_MAX % distance));

    randomNumber %= distance;
    return randomNumber;
}

//this method returns a random choice between ROCK,PAPER,SCISSORS. This is meant for the foe, to give a random choice against your choice
enum choiceType game_get_choice_PC(){
    unsigned int randomNumber = game_random_number();

    //the random number is equal to the values of the enum, they are in range
    enum choiceType enemyChoice = randomNumber; 
    return enemyChoice;
}

//checks the outcome of the round between two type of choices. This return a value of the enum outcome
enum outcome game_check_outcome(enum choiceType myChoice, enum choiceType enemyChoice){
    enum outcome result = ERROR_OUTCOME;

    if(myChoice == ROCK){
        if(enemyChoice == SCISSORS){
            result = WON;
        }
        else if(enemyChoice == PAPER){
            result = LOST;
        } 
        else{
            result = TIE;
        }
    }
    else if(myChoice == PAPER){
        if(enemyChoice == ROCK){
            result = WON;
        }
        else if(enemyChoice == SCISSORS){
            result = LOST;
        } 
        else{
            result = TIE;
        }
    }
    else if(myChoice == SCISSORS){
        if(enemyChoice == PAPER){
            result = WON;
        }
        else if(enemyChoice == ROCK){
            result = LOST;
        } 
        else{
            result = TIE;
        }
    }

    return result;
   
}

//this method adds a streak value to the streak the player has. Parameter is a game_t* object 
void game_streak_up(game_t* gameInfo){
    gameInfo->streak += STREAK_UP_VALUE;
}

//this method resets the streak of the player. Parameter is a game_t* object
void game_reset_streak(game_t* gameInfo){
    gameInfo->streak = 0;
}

//a toString method, this parses a number to a char*
static char * toString(int number) {
    int length = snprintf(NULL, 0, "%d", number + 1);
    char *str = malloc(length + 1);
    snprintf(str, length + 1, "%d", number);
    return str;
}

//this method checks if the current streak, which ended, is higher than the current highscore. If so, the current streak is the new highscore. Parameter is a game_t* object
void game_check_score(game_t* gameInfo){
    if(gameInfo->streak > gameInfo->highScore){
        gameInfo->highScore = gameInfo->streak;
    }
}

//returns a char*. This returns text based on the type it is from the enum choiceType
char* game_get_text_of_enum_choice(enum choiceType type){
    switch (type)
    {
        case ROCK: return "ROCK";
        case PAPER: return "PAPER";
        case SCISSORS: return "SCISSORS";
        case ERROR_CHOICE: return "ERROR_CHOICE";
        default:
            break;
    }
    return " ";
}

//returns a char*. This returns text based on the type it is from the enum outcome
char* game_get_text_of_enum_outcome(enum outcome result){
    switch (result)
    {
        case WON: return "WON";
        case LOST: return "LOST";
        case TIE: return "TIE";
        case ERROR_OUTCOME: return "ERROR_OUTCOME";
        default:
            break;
    }
    return " ";
}