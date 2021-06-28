#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "game.h"


game_t* game_create_game(){
    game_t* gameInfo = malloc(sizeof(game_t));
    //creating a placeholder for the max score list, this comes in handy when shuffling the scores.
    char* initScores[MAX_AMOUNT_OF_SCORES] = {"0", "0", "0"};

    if(gameInfo != NULL){
        gameInfo->streak = 0;
        gameInfo->highScore = 0;
        // for(int i = 0; i < MAX_AMOUNT_OF_SCORES; i++){
        //     gameInfo->maxScores[i] = initScores[i];
        // }
    }
    
    return gameInfo;
}


static unsigned int game_random_number(){
    unsigned int min = 0;
    unsigned int max = MAX_AMOUNT_OF_CHOICES;
    unsigned int distance = (max - min);
    unsigned int randomNumber;
    do {
        randomNumber = rand();
    } while(randomNumber >= (RAND_MAX - RAND_MAX % distance));

    randomNumber %= distance;
    return randomNumber;
}

enum choiceType game_get_choice_PC(){
    unsigned int randomNumber = game_random_number();
    enum choiceType enemyChoice = randomNumber; 
    return enemyChoice;
}

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

void game_streak_up(game_t* gameInfo){
    gameInfo->streak += 1;
}

void game_reset_streak(game_t* gameInfo){
    gameInfo->streak = 0;
}

static char * toString(int number) {
    int length = snprintf(NULL, 0, "%d", number + 1);
    char *str = malloc(length + 1);
    snprintf(str, length + 1, "%d", number);
    return str;
}

void game_check_score(game_t* gameInfo){
    if(gameInfo->streak > gameInfo->highScore){
        gameInfo->highScore = gameInfo->streak;
    }
    // char* temp = toString(gameInfo->streak);
    // char* tempList = calloc(MAX_AMOUNT_OF_CHOICES, sizeof(char*));
    // memcpy(gameInfo->maxScores, tempList, MAX_AMOUNT_OF_SCORES * sizeof(char*));
    
    //for(int i = 0; i < MAX_AMOUNT_OF_SCORES; i++){
        // if(gameInfo->maxScores[i] < temp){
        //     char* tempSaved = gameInfo->maxScores[i];
        //     if((i + 1) >= MAX_AMOUNT_OF_SCORES){
        //         gameInfo->maxScores[i] = temp;
        //     }
        //     else{

        //     }

        //     int index = 0;
        //     while(index < MAX_AMOUNT_OF_SCORES){

        //         index++;
        //     }
        //     return;
        // } 
    //}
    //free(temp);
}


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