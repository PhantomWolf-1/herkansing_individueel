#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "menu.h"
#include "game.h"

#include "esp_log.h"


#define MENU_TAG "MENU"

//initialize the lcd screen and returns its information
// Parameters
// - none
i2c_lcd1602_info_t* lcd_init(void){
    i2c_port_t i2c_num = I2C_MASTER_NUM;
    uint8_t address = CONFIG_LCD1602_I2C_ADDRESS;

    //setting up the smbus
    smbus_info_t* smbus_info = smbus_malloc();
    smbus_init(smbus_info, i2c_num, address);
    smbus_set_timeout(smbus_info, 1000/portTICK_RATE_MS);

    i2c_lcd1602_info_t * lcd_info = i2c_lcd1602_malloc();
    i2c_lcd1602_init(lcd_info, smbus_info, true, LCD_NUM_ROWS, LCD_NUM_COLUMNS, LCD_NUM_VIS_COLUMNS);

    // turn off backlight
    //ESP_LOGI(MENU_TAG, "backlight off");
    i2c_lcd1602_set_backlight(lcd_info, false);

    // turn on backlight
    //ESP_LOGI(MENU_TAG, "backlight on");
    i2c_lcd1602_set_backlight(lcd_info, true);

    // turn on cursor 
    //ESP_LOGI(MENU_TAG, "cursor on");
    i2c_lcd1602_set_cursor(lcd_info, false);

    return lcd_info;
}

//creates and returns a menu_t* object, all values getting initialized inside the struct
// Parameters:
// - none
menu_t* menu_create_menu(void){
    //allocate some memory
    menu_t* tempMenuPointer = malloc(sizeof(menu_t));

    //create all the main menu items
    menu_item_t mainItems[MAX_MAIN_MENU_ITEMS] = {
        {MAIN_MENU_ID_0, MAIN, GAME, "PLAY", NULL},
        {MAIN_MENU_ID_1, MAIN, SCORE, "H-SCORE", NULL}
    }; 

    //create all the game menu items
    menu_item_t gameItems[MAX_GAME_MENU_ITEMS] = {
        {GAME_MENU_ID_0, GAME, GAME, "ROCK", NULL},
        {GAME_MENU_ID_1, GAME, GAME, "PAPER", NULL},
        {GAME_MENU_ID_2, GAME, GAME, "SCISSORS", NULL},
        {GAME_MENU_ID_3, GAME, MAIN, "BACK", NULL}
    };

    //create all the score menu items
    menu_item_t scoreItems[MAX_SCORE_MENU_ITEMS] = {
        {SCORE_MENU_ID_0, SCORE, MAIN, "BACK", NULL}
    };

    i2c_lcd1602_info_t* lcdInfo = lcd_init();

    //checks if the memry allocation went correct
    if(tempMenuPointer != NULL){
        // Initialize menu with values
        tempMenuPointer->lcdInfo = lcdInfo;
        tempMenuPointer->mainMenuItems = calloc(MAX_MAIN_MENU_ITEMS, sizeof(menu_item_t));
        tempMenuPointer->gameMenuItems = calloc(MAX_GAME_MENU_ITEMS, sizeof(menu_item_t));
        tempMenuPointer->scoreMenuItems = calloc(MAX_SCORE_MENU_ITEMS, sizeof(menu_item_t));

        memcpy(tempMenuPointer->mainMenuItems, mainItems, MAX_MAIN_MENU_ITEMS * sizeof(menu_item_t));
        memcpy(tempMenuPointer->gameMenuItems, gameItems, MAX_GAME_MENU_ITEMS * sizeof(menu_item_t));
        memcpy(tempMenuPointer->scoreMenuItems, scoreItems, MAX_SCORE_MENU_ITEMS * sizeof(menu_item_t));
        tempMenuPointer->currentMenu = MAIN;
        tempMenuPointer->currentMenuIndex = MENU_START_INDEX;

        //ESP_LOGD(MENU_TAG, "malloc menu_t %p", tempMenuPointer);
    }
    else {
        ESP_LOGD(MENU_TAG, "MALLOC NOT CORRECT!!!");
    }

    return tempMenuPointer;
}

//free all the memory used un the men_t* object given as parameter
// Parameters:
// - menu_t* menu: object that stores menu information
void menu_free_all(menu_t* menu){
    free(menu->lcdInfo);
    menu->lcdInfo = NULL;
    free(menu->mainMenuItems);
    menu->mainMenuItems = NULL;
    free(menu->gameMenuItems);
    menu->gameMenuItems = NULL;
    free(menu->scoreMenuItems);
    menu->scoreMenuItems = NULL;
    free(menu);
    menu = NULL;
    
}

//writes the title based on the menuType given as parameter, other parameter is the lcdInfo. This is the information of the lcd screen
// Parameters:
// - i2c_lcd1602_info_t* lcdInfo: object that stores lcd information
// - enum menuType type: value of the enum menuType
void menu_write_menu_title(i2c_lcd1602_info_t* lcdInfo, enum menuType type)
{
    //move the cursor to a specific location on the screen
    i2c_lcd1602_move_cursor(lcdInfo, TITLE_COLUMN, TITLE_ROW);
    //checks the type of the menuType, based on that writes the title on the screen
    switch (type)
    {
        case MAIN:
            i2c_lcd1602_write_string(lcdInfo, "*MAIN*");
            break;

        case GAME:
            i2c_lcd1602_write_string(lcdInfo, "*GAME*");
            break;

        case SCORE:
            i2c_lcd1602_write_string(lcdInfo, "*H-SCORE*");
            break;
    
        default:
        // something of an error message or something
            break;
    }
}

//a toString method, this parses a number to a char*
// Parameters:
// - int number: number value
static char * toString(int number) {
    int length = snprintf(NULL, 0, "%d", number + 1);
    char *str = malloc(length + 1);
    snprintf(str, length + 1, "%d", number);
    return str;
}

//writes the menu items on the screen. lcd information and a menu_t* object given as parameters 
// Parameters:
// - i2c_lcd1602_info_t* lcdInfo: object that stores lcd information
// - game_t* gameInfo: object that stores game information
void menu_write_menu_text(i2c_lcd1602_info_t* lcdInfo, menu_t* menu)
{
    i2c_lcd1602_move_cursor(lcdInfo, MENU_TEXT_COLUMN, MENU_TEXT_ROW);
    switch (menu->currentMenu){
        case MAIN:
            for(int i = 0; i < MAX_MAIN_MENU_ITEMS; i++){
                menu_item_t item = menu->mainMenuItems[i];
                i2c_lcd1602_move_cursor(lcdInfo, MENU_TEXT_COLUMN, MENU_TEXT_ROW + i);
                i2c_lcd1602_write_string(lcdInfo, item.menuText);
            }
            break;

        case GAME:
            for(int i = 0; i < MAX_GAME_MENU_ITEMS; i++){
                        menu_item_t item = menu->gameMenuItems[i];
                        i2c_lcd1602_move_cursor(lcdInfo, MENU_TEXT_COLUMN, MENU_TEXT_ROW + i);
                        i2c_lcd1602_write_string(lcdInfo, item.menuText);
                    }
            break;

        case SCORE:
            for(int i = 0; i < MAX_SCORE_MENU_ITEMS; i++){
                    menu_item_t item = menu->scoreMenuItems[i];
                    i2c_lcd1602_move_cursor(lcdInfo, MENU_TEXT_COLUMN, MENU_TEXT_ROW + i);
                    i2c_lcd1602_write_string(lcdInfo, item.menuText);
                }
            break;
        
        default:
            break;
    }
}

//displays the cursor, menu_t* object as parameter
// Parameters:
// - menu_t* menu: object that stores menu information
void menu_display_cursor(menu_t* menu){
    i2c_lcd1602_move_cursor(menu->lcdInfo, CURSOR_POS_COLUMN, menu->currentMenuIndex);
    i2c_lcd1602_write_string(menu->lcdInfo, "<");
}

//writes text on a specific position on the screen.
// Parameters:
// - i2c_lcd1602_info_t* lcdInfo: object that stores lcd information
// - char* text: text for writing on screen
// - int column: the column position on screen
// - int row: the row position on screen
void menu_write_text_on_position(i2c_lcd1602_info_t* lcdInfo, char* text, int column, int row){
    i2c_lcd1602_move_cursor(lcdInfo, column, row);
    i2c_lcd1602_write_string(lcdInfo, text);
}

//display and manage the menu on the screen
// Parameters:
// - menu_t* menu: object that stores menu information
// - game_t* gameInfo: object that stores game information
void menu_display_menu(menu_t* menu, game_t* gameInfo){

    i2c_lcd1602_clear(menu->lcdInfo);
    menu_write_menu_title(menu->lcdInfo, menu->currentMenu);
    menu_write_menu_text(menu->lcdInfo, menu);
    menu_display_cursor(menu);

    //looking at what menu the user is and activate a start method for that specific menu
    switch (menu->currentMenu)
    {
        case MAIN:
            
            break;

        case GAME:
            
            break;

        case SCORE:
            menu_write_text_on_position(menu->lcdInfo, toString(gameInfo->highScore), HIGHSCORE_COLUMN, HIGHSCORE_ROW);
            break;
        
        default:
            break;
    }

}

//display the outcome on the screen
// Parameters:
// - menu_t* menu: object that stores menu information
// - enum choiceType myChoice: value of the enum choiceType
// - game_t* gameInfo: object that stores game information
void menu_display_outcome(menu_t* menu, enum choiceType myChoice, game_t* gameInfo){
    //gets the choice of the foe
    enum choiceType enemyChoice = game_get_choice_PC();
    //getting the result of the round
    enum outcome result = game_check_outcome(myChoice, enemyChoice);

    //ESP_LOGI(MENU_TAG, "My choice: %d", myChoice);
    //ESP_LOGI(MENU_TAG, "enemy choice: %d", enemyChoice);
    i2c_lcd1602_clear(menu->lcdInfo);

    //display your choice
    menu_write_text_on_position(menu->lcdInfo, PLAYER_RESULT_TEXT, PLAYER_RESULT_TEXT_POS_COLUMN, PLAYER_RESULT_TEXT_POS_ROW);
    menu_write_text_on_position(menu->lcdInfo, game_get_text_of_enum_choice(myChoice), PLAYER_CHOICE_POS_COLUMN, PLAYER_CHOICE_POS_ROW);
    //dispplay foe choice
    menu_write_text_on_position(menu->lcdInfo, FOE_RESULT_TEXT, FOE_RESULT_TEXT_POS_COLUMN, FOE_RESULT_TEXT_POS_ROW);
    menu_write_text_on_position(menu->lcdInfo, game_get_text_of_enum_choice(enemyChoice), FOE_CHOICE_POS_COLUMN, FOE_CHOICE_POS_ROW);
    
    //display the outcome of the round
    menu_write_text_on_position(menu->lcdInfo, RESULT_GAME_TEXT, RESULT_GAME_TEXT_POS_COLUMN, RESULT_GAME_TEXT_POS_ROW);
    menu_write_text_on_position(menu->lcdInfo, game_get_text_of_enum_outcome(result), RESULT_GAME_POS_COLUMN, RESULT_GAME_POS_ROW);
    
    //based on the result, modify the streak value
    switch (result)
    {
        case WON:
            ESP_LOGI(MENU_TAG, "WON");
            game_streak_up(gameInfo);
            break;

        case LOST:
            ESP_LOGI(MENU_TAG, "LOST");
            game_check_score(gameInfo);
            game_reset_streak(gameInfo);
            break;

        case TIE:
            ESP_LOGI(MENU_TAG, "TIE");
            break;
        
        default:
            //result nog correct, soemthing went wrong!
            ESP_LOGI(MENU_TAG, "SOMETHING WENT WRONG!!!");
            break;
    }
    //display the streak value
    menu_write_text_on_position(menu->lcdInfo, STREAK_TEXT, STREAK_TEXT_POS_COLUMN, STREAK_TEXT_POS_ROW);
    menu_write_text_on_position(menu->lcdInfo, toString(gameInfo->streak), STREAK_SCORE_POS_COLUMN, STREAK_SCORE_POS_ROW);
    
    //giving the user time to look at the results, thus waiting an amount of time
    for(int i = AMOUNT_OF_SEC_SWITCH; i >= 0; i--){
        ESP_LOGI(MENU_TAG, "WAITING SECONDS FOR SWICTH: %d", i);
        vTaskDelay(AMOUNT_OF_MS/portTICK_RATE_MS);
    }
    //display the menu again
    menu_display_menu(menu, gameInfo);
}

//handles the key event for navigating through the menu
// Parameters:
// - menu_t* menu: object that stores menu information
// - int key: value of the event that happend, for example: when clicking in the rotary encoder 
// - game_t* gameInfo: object that stores game information
void menu_handle_key_event(menu_t* menu, int key, game_t* gameInfo){

    menu_item_t* items;
    int maxIndex;
    //based on the current menu, some value will ben initialized
    switch (menu->currentMenu)
    {
        case MAIN:
            items = menu->mainMenuItems;
            maxIndex = MAX_MAIN_MENU_ITEMS;
            break;

        case GAME:
            items = menu->gameMenuItems;
            maxIndex = MAX_GAME_MENU_ITEMS;
            break;

        case SCORE:
            items = menu->scoreMenuItems;
            maxIndex = MAX_SCORE_MENU_ITEMS;
            break;

        default:
        items = NULL;
            break;
    }

    //checks if the item list has some values
    if(items != NULL){
        //if the key event is clicked
        if(key == KEY_CLICKED){
            //checks if the current menu is in the GAME screen and the item is nog the "BACK" item in the list, then it is a option on ROCK,PAPER,SCISSORS
            if(menu->currentMenu == GAME && items[menu->currentMenuIndex].goingTo != MAIN){
                menu_display_outcome(menu, menu->currentMenuIndex, gameInfo);
            }
            menu->currentMenu = items[menu->currentMenuIndex].goingTo;
            menu->currentMenuIndex = MENU_START_INDEX;
            menu_display_menu(menu, gameInfo);
        }
        //if the key event is left (thus turning left with the button)
        else if(key == KEY_LEFT){
            //removing the cursor of its location
            menu_write_text_on_position(menu->lcdInfo, " ", CURSOR_POS_COLUMN, menu->currentMenuIndex);

            //move the cursor to a new postion
            menu->currentMenuIndex -= CURSOR_POS_MOVE;
            //checks if the index is in bounds of the list
            if(menu->currentMenuIndex < 0){ 
                menu->currentMenuIndex = maxIndex - 1;
            }
            //redrawing everything
            menu_display_cursor(menu);
        }
        //if the key event is left (thus turning left with the button)
        else if(key == KEY_RIGHT){
            //removing the cursor of its location
            menu_write_text_on_position(menu->lcdInfo, " ", CURSOR_POS_COLUMN, menu->currentMenuIndex);

            //move the cursor to a new postion
            menu->currentMenuIndex += CURSOR_POS_MOVE;
            //checks if the index is in bounds of the list
            if(menu->currentMenuIndex >= maxIndex){
                menu->currentMenuIndex = 0;
            }
            //redrawing everything
            menu_display_cursor(menu);
        }
    }
}
