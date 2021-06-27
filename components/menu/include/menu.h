#ifndef INDIV_MENU
#define INDIV_MENU

#include "i2c-lcd1602.h"

//init
#undef USE_STDIN

#define CONFIG_I2C_MASTER_SCL 23
#define CONFIG_I2C_MASTER_SDA 18
#define CONFIG_LCD1602_I2C_ADDRESS 0x27

//some i2c standard defines
#define I2C_MASTER_NUM           I2C_NUM_0
#define I2C_MASTER_TX_BUF_LEN    0                     // disabled
#define I2C_MASTER_RX_BUF_LEN    0                     // disabled
#define I2C_MASTER_FREQ_HZ       100000
#define I2C_MASTER_SDA_IO        CONFIG_I2C_MASTER_SDA
#define I2C_MASTER_SCL_IO        CONFIG_I2C_MASTER_SCL
#define LCD_NUM_ROWS			 4
#define LCD_NUM_COLUMNS			 40
#define LCD_NUM_VIS_COLUMNS		 20

//Key inputs
#define MAX_KEY_INPUTS 3
#define KEY_CLICKED 0
#define KEY_LEFT 1
#define KEY_RIGHT 2

//Menu ID's (for navigation)
//main menu ID's
#define MAX_MAIN_MENU_ITEMS 2
#define MAIN_MENU_ID_0 0 //-> game item, to play the game.
#define MAIN_MENU_ID_1 1 //-> score item, to go to the score screen.
//game menu ID's
#define MAX_GAME_MENU_ITEMS 4
#define GAME_MENU_ID_0 2 //-> rock item, game choose item
#define GAME_MENU_ID_1 3 //-> paper item, game choose item
#define GAME_MENU_ID_2 4 //-> scissors item, game choose item
#define GAME_MENU_ID_3 5 //-> back item, going back to main menu screen
//score menu ID's
#define MAX_SCORE_MENU_ITEMS 1
#define SCORE_MENU_ID_0 6 //-> back item, going back to main menu screen


typedef struct{
    unsigned int id;
    unsigned int goingTo;
    char* menuText[LCD_NUM_ROWS];
    void (*KeyEvent[MAX_KEY_INPUTS])(void);
    void (*MenuEntryEvent)(void);
} menu_item_t;

typedef struct{
    i2c_lcd1602_info_t* lcdInfo;
    unsigned int currentMenuIndex;
    menu_item_t* mainMenuItems[MAX_MAIN_MENU_ITEMS];
    menu_item_t* gameMenuItems[MAX_GAME_MENU_ITEMS];
    menu_item_t* scoreMenuItems[MAX_SCORE_MENU_ITEMS];
}menu_t;

i2c_lcd1602_info_t* lcd_init(void);
menu_t* menu_create_menu(void);
void menu_free_all(menu_t* menu);
void menu_welcome_message(menu_t* menu);
void menu_display_menu(menu_t* menu);
void menu_handle_key_event(menu_t* menu);
void menu_display_top_scores(menu_t* menu);

void menu_create_custom_characters(void);
#endif