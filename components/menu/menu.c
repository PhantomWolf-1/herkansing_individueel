#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "menu.h"
#include "game.h"

#include "esp_log.h"

#define MENU_TAG "MENU"



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

menu_t* menu_create_menu(void){
    menu_t* tempMenuPointer = malloc(sizeof(menu_t));

    menu_item_t mainItems[MAX_MAIN_MENU_ITEMS] = {
        {MAIN_MENU_ID_0, MAIN, GAME, "PLAY", NULL},
        {MAIN_MENU_ID_1, MAIN, SCORE, "H-SCORE", NULL}
    }; 

    menu_item_t gameItems[MAX_GAME_MENU_ITEMS] = {
        {GAME_MENU_ID_0, GAME, GAME, "ROCK", NULL},
        {GAME_MENU_ID_1, GAME, GAME, "PAPER", NULL},
        {GAME_MENU_ID_2, GAME, GAME, "SCISSORS", NULL},
        {GAME_MENU_ID_3, GAME, MAIN, "BACK", NULL}
    };

    menu_item_t scoreItems[MAX_SCORE_MENU_ITEMS] = {
        {SCORE_MENU_ID_0, SCORE, MAIN, "BACK", NULL}
    };

    i2c_lcd1602_info_t* lcdInfo = lcd_init();

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
        //ESP_LOGD(MENU_TAG, "MALLOC NOT CORRECT!!!");
    }

    return tempMenuPointer;
}

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

void menu_write_menu_title(i2c_lcd1602_info_t* lcdInfo, enum menuType type)
{
    i2c_lcd1602_move_cursor(lcdInfo, TITLE_COLUMN, TITLE_ROW);
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

static char * toString(int number) {
    int length = snprintf(NULL, 0, "%d", number + 1);
    char *str = malloc(length + 1);
    snprintf(str, length + 1, "%d", number);
    return str;
}

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

void menu_display_cursor(menu_t* menu){
    i2c_lcd1602_move_cursor(menu->lcdInfo, CURSOR_POS_COLUMN, menu->currentMenuIndex);
    i2c_lcd1602_write_string(menu->lcdInfo, "<");
}

void menu_write_text_on_position(i2c_lcd1602_info_t* lcdInfo, char* text, int column, int row){
    i2c_lcd1602_move_cursor(lcdInfo, column, row);
    i2c_lcd1602_write_string(lcdInfo, text);
}

void menu_welcome_message(menu_t* menu){

}

void menu_display_menu(menu_t* menu, game_t* gameInfo){

    i2c_lcd1602_clear(menu->lcdInfo);
    menu_write_menu_title(menu->lcdInfo, menu->currentMenu);
    menu_write_menu_text(menu->lcdInfo, menu);
    menu_display_cursor(menu);

    //lookinf at what menu the user is and activate a start method for that specific menu
    switch (menu->currentMenu)
    {
        case MAIN:
            
            break;

        case GAME:
            
            break;

        case SCORE:
        menu_write_text_on_position(menu->lcdInfo, toString(gameInfo->highScore), SCORE_LIST_COLUMN, SCORE_LIST_ROW);
            // for(int i = 0; i < MAX_AMOUNT_OF_SCORES; i++){
            //     menu_write_text_on_position(menu->lcdInfo, gameInfo->maxScores[i], SCORE_LIST_COLUMN, SCORE_LIST_ROW + i);
            // }
            break;
        
        default:
            break;
    }

}

void menu_display_outcome(menu_t* menu, enum choiceType myChoice, game_t* gameInfo){
    enum choiceType enemyChoice = game_get_choice_PC();
    enum outcome result = game_check_outcome(myChoice, enemyChoice);
    //ESP_LOGI(MENU_TAG, "My choice: %d", myChoice);
    //ESP_LOGI(MENU_TAG, "enemy choice: %d", enemyChoice);
    i2c_lcd1602_clear(menu->lcdInfo);

    menu_write_text_on_position(menu->lcdInfo, PLAYER_RESULT_TEXT, PLAYER_RESULT_TEXT_POS_COLUMN, PLAYER_RESULT_TEXT_POS_ROW);
    menu_write_text_on_position(menu->lcdInfo, game_get_text_of_enum_choice(myChoice), PLAYER_CHOICE_POS_COLUMN, PLAYER_CHOICE_POS_ROW);
    menu_write_text_on_position(menu->lcdInfo, FOE_RESULT_TEXT, FOE_RESULT_TEXT_POS_COLUMN, FOE_RESULT_TEXT_POS_ROW);
    menu_write_text_on_position(menu->lcdInfo, game_get_text_of_enum_choice(enemyChoice), FOE_CHOICE_POS_COLUMN, FOE_CHOICE_POS_ROW);
    
    menu_write_text_on_position(menu->lcdInfo, RESULT_GAME_TEXT, RESULT_GAME_TEXT_POS_COLUMN, RESULT_GAME_TEXT_POS_ROW);
    menu_write_text_on_position(menu->lcdInfo, game_get_text_of_enum_outcome(result), RESULT_GAME_POS_COLUMN, RESULT_GAME_POS_ROW);

    menu_write_text_on_position(menu->lcdInfo, RESULT_GAME_TEXT, RESULT_GAME_TEXT_POS_COLUMN, RESULT_GAME_TEXT_POS_ROW);
    menu_write_text_on_position(menu->lcdInfo, game_get_text_of_enum_outcome(result), RESULT_GAME_POS_COLUMN, RESULT_GAME_POS_ROW);
    
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
    menu_write_text_on_position(menu->lcdInfo, STREAK_TEXT, STREAK_TEXT_POS_COLUMN, STREAK_TEXT_POS_ROW);
    menu_write_text_on_position(menu->lcdInfo, toString(gameInfo->streak), STREAK_SCORE_POS_COLUMN, STREAK_SCORE_POS_ROW);
    
    for(int i = AMOUNT_OF_SEC_SWITCH; i >= 0; i--){
        ESP_LOGI(MENU_TAG, "WAITING SECONDS FOR SWICTH: %d", i);
        vTaskDelay(AMOUNT_OF_MS/portTICK_RATE_MS);
    }
    menu_display_menu(menu, gameInfo);
}


void menu_handle_key_event(menu_t* menu, int key, game_t* gameInfo){

    menu_item_t* items;
    int maxIndex;
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

    if(items != NULL){
        if(key == KEY_CLICKED){
        if(menu->currentMenu == GAME && items[menu->currentMenuIndex].goingTo != MAIN){
            menu_display_outcome(menu, menu->currentMenuIndex, gameInfo);
        }
        menu->currentMenu = items[menu->currentMenuIndex].goingTo;
        menu->currentMenuIndex = MENU_START_INDEX;
        menu_display_menu(menu, gameInfo);
        }
        else if(key == KEY_LEFT){
            menu_write_text_on_position(menu->lcdInfo, " ", CURSOR_POS_COLUMN, menu->currentMenuIndex);
            menu->currentMenuIndex -= CURSOR_POS_MOVE;
            if(menu->currentMenuIndex < 0){ 
                menu->currentMenuIndex = maxIndex - 1;
            }
            menu_display_cursor(menu);
        }
        else if(key == KEY_RIGHT){
            menu_write_text_on_position(menu->lcdInfo, " ", CURSOR_POS_COLUMN, menu->currentMenuIndex);
            menu->currentMenuIndex += CURSOR_POS_MOVE;
            if(menu->currentMenuIndex >= maxIndex){
                menu->currentMenuIndex = 0;
            }
            menu_display_cursor(menu);
        }
    }
}

void menu_display_top_scores(menu_t* menu){

}


void menu_create_custom_characters(void){
            
}