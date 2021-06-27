#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "menu.h"
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
    i2c_lcd1602_set_cursor(lcd_info, true);

    return lcd_info;
}

menu_t* menu_create_menu(void){
    menu_t* tempMenuPointer = malloc(sizeof(menu_t));

    menu_item_t mainItems[MAX_MAIN_MENU_ITEMS] = {
        {MAIN_MENU_ID_0, MAIN, GAME, "PLAY", {NULL, NULL, NULL}, NULL},
        {MAIN_MENU_ID_1, MAIN, SCORE, "SCORES", {NULL, NULL, NULL}, NULL}
    }; 

    menu_item_t gameItems[MAX_GAME_MENU_ITEMS] = {
        {GAME_MENU_ID_0, GAME, GAME, "ROCK", {NULL, NULL, NULL}, NULL},
        {GAME_MENU_ID_1, GAME, GAME, "PAPER", {NULL, NULL, NULL}, NULL},
        {GAME_MENU_ID_2, GAME, GAME, "SCISSORS", {NULL, NULL, NULL}, NULL},
        {GAME_MENU_ID_3, GAME, MAIN, "BACK", {NULL, NULL, NULL}, NULL}
    };

    menu_item_t scoreItems[MAX_SCORE_MENU_ITEMS] = {
        {SCORE_MENU_ID_0, SCORE, MAIN, "BACK", {NULL, NULL, NULL}, NULL}
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
        tempMenuPointer->currentMenuIndex = GAME_START_INDEX;

        //ESP_LOGD(MENU_TAG, "malloc menu_t %p", tempMenuPointer);
    }
    else {
        //ESP_LOGD(MENU_TAG, "MALLOC NOT CORRECT!!!");
    }

    return tempMenuPointer;
}

void menu_free_all(menu_t* menu){

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
        i2c_lcd1602_write_string(lcdInfo, "*SCORES*");
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
    int text_ROW = MENU_TEXT_ROW;
    i2c_lcd1602_move_cursor(lcdInfo, MENU_TEXT_COLUMN, MENU_TEXT_ROW);
    switch (menu->currentMenu){
    case MAIN:
        for(int i = text_ROW; i < 2; i++){
            menu_item_t item = menu->mainMenuItems[i];
            i2c_lcd1602_write_string(lcdInfo, item.menuText);
            if(i == 0){
                i2c_lcd1602_move_cursor(lcdInfo, MENU_TEXT_COLUMN, 0);
            }else{
                i2c_lcd1602_move_cursor(lcdInfo, MENU_TEXT_COLUMN, 1);
            }
        }
        break;

    case GAME:
        for(int i = 0; i < MAX_GAME_MENU_ITEMS; i++){
                menu_item_t item = menu->gameMenuItems[i];
                i2c_lcd1602_write_string(lcdInfo, item.menuText);
                i2c_lcd1602_move_cursor(lcdInfo, MENU_TEXT_COLUMN, MENU_TEXT_ROW + i);
            }
        break;

    case SCORE:
        for(int i = 0; i < MAX_SCORE_MENU_ITEMS; i++){
                menu_item_t item = menu->scoreMenuItems[i];
                i2c_lcd1602_write_string(lcdInfo, item.menuText);
                i2c_lcd1602_move_cursor(lcdInfo, MENU_TEXT_COLUMN, MENU_TEXT_ROW + i);
            }
        break;
    
    default:
        break;
    }
}


void menu_welcome_message(menu_t* menu){

}

void menu_display_menu(menu_t* menu){

    i2c_lcd1602_clear(menu->lcdInfo);
    menu_write_menu_title(menu->lcdInfo, menu->currentMenu);
    menu_write_menu_text(menu->lcdInfo, menu);
}



void menu_handle_key_event(menu_t* menu, int key){

}

void menu_display_top_scores(menu_t* menu){

}


void menu_create_custom_characters(void){
            
}