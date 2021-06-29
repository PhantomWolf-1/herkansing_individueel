#ifndef INDIV_MENU
#define INDIV_MENU

#include "i2c-lcd1602.h"
#include "game.h"

//init
#undef USE_STDIN


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
#define CONFIG_I2C_MASTER_SCL 23
#define CONFIG_I2C_MASTER_SDA 18
#define CONFIG_LCD1602_I2C_ADDRESS 0x27

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

//menu start index by startup
#define MENU_START_INDEX 0

//indexes of where the menu title places
#define TITLE_ROW 0
#define TITLE_COLUMN 11

//indexes of where the menu text places
#define MENU_TEXT_ROW 0
#define MENU_TEXT_COLUMN 0

//the position of the highscore
#define HIGHSCORE_ROW 1
#define HIGHSCORE_COLUMN 10

//define the postition of the cursor, only the column is necessary
//besides a offset for the ROW position
#define CURSOR_POS_COLUMN 9
//define a value which the cursor moves per tick with the rotary encoder
#define CURSOR_POS_MOVE 1

//some values for the positions and some text for the result game screen.
#define PLAYER_RESULT_TEXT "My choice:"
#define PLAYER_RESULT_TEXT_POS_COLUMN 0
#define PLAYER_RESULT_TEXT_POS_ROW 0
#define PLAYER_CHOICE_POS_COLUMN 0
#define PLAYER_CHOICE_POS_ROW 1

#define FOE_RESULT_TEXT "Foe choice:"
#define FOE_RESULT_TEXT_POS_COLUMN 0
#define FOE_RESULT_TEXT_POS_ROW 2
#define FOE_CHOICE_POS_COLUMN 0
#define FOE_CHOICE_POS_ROW 3

#define RESULT_GAME_TEXT "You:"
#define RESULT_GAME_TEXT_POS_ROW 0
#define RESULT_GAME_TEXT_POS_COLUMN 12
#define RESULT_GAME_POS_ROW 1
#define RESULT_GAME_POS_COLUMN 12

#define STREAK_TEXT "Streak:"
#define STREAK_TEXT_POS_ROW 2
#define STREAK_TEXT_POS_COLUMN 12
#define STREAK_SCORE_POS_ROW 3
#define STREAK_SCORE_POS_COLUMN 12

//values for waiting at the result screen
#define AMOUNT_OF_SEC_SWITCH 8
#define AMOUNT_OF_MS 1000

//menuType is the for keeping track at which menu part the appliction is
enum menuType{MAIN, GAME, SCORE};

//this struct is to store information about a item in the menu
typedef struct{
    unsigned int id;
    enum menuType isFrom;
    enum menuType goingTo;
    char* menuText;
    void (*KeyEvent)(void);
} menu_item_t;


//this struct is to store all the menu items and on which position and screen the application is.
typedef struct{
    i2c_lcd1602_info_t* lcdInfo;
    enum menuType currentMenu; 
    int currentMenuIndex;
    menu_item_t* mainMenuItems;
    menu_item_t* gameMenuItems;
    menu_item_t* scoreMenuItems;
} menu_t;

//initialize the lcd screen
i2c_lcd1602_info_t* lcd_init(void);

//creates a menu_t* object and initialize it
menu_t* menu_create_menu(void);

//free al the memory that is currently used in the application, about the menu_t* object
void menu_free_all(menu_t* menu);

//writes the menu title for a given menuType
void menu_write_menu_title(i2c_lcd1602_info_t* lcdInfo, enum menuType type);

//writes the menu item at for the current menuType, thus where the application it on that time
void menu_write_menu_text(i2c_lcd1602_info_t* lcdInfo, menu_t* menu);

//displays the cursor
void menu_display_cursor(menu_t* menu);

//writes given text to a given position on the screen 
void menu_write_text_on_position(i2c_lcd1602_info_t* lcdInfo, char* text, int column, int row);

//manages everything that needs to be displayed, like the cursor. this will be called in the display menu
void menu_display_menu(menu_t* menu, game_t* gameInfo);

//displays the outcome of the round between the player and the foe. Like your choice, his/her choice and the result
void menu_display_outcome(menu_t* menu, enum choiceType myChoice, game_t* gameInfo);

//handles the input given with the key
void menu_handle_key_event(menu_t* menu, int key, game_t* gameInfo);

#endif