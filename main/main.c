#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"

#include "qwiic_twist.h"
#include "menu.h"


#define MAINTAG "MAIN"

qwiic_twist_t* qwiic_twist_rotary;
menu_t* menu;

void rotary_task(void *);
void clicked(void);
void pressed(void);
void onMove(int16_t);

//setup the i2c
void i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_DISABLE;  // GY-2561 provides 10kΩ pullups
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_DISABLE;  // GY-2561 provides 10kΩ pullups
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf);
    i2c_driver_install(i2c_master_port, conf.mode,
                       I2C_MASTER_RX_BUF_LEN,
                       I2C_MASTER_TX_BUF_LEN, 0);

    printf("CONFIG I2C MASTER COMPLETE!!!!\n");
}

void set_rotary_encoder(void){
    qwiic_twist_rotary = (qwiic_twist_t*)malloc(sizeof(*qwiic_twist_rotary));
    qwiic_twist_rotary->port = I2C_MASTER_NUM;
    qwiic_twist_rotary->onButtonClicked = &clicked;
    qwiic_twist_rotary->onButtonPressed = &pressed;
    qwiic_twist_rotary->onMoved = &onMove;
    qwiic_twist_init(qwiic_twist_rotary);

}


void hello_task(void *pvParameter)
{
    printf("Hello world!\n");
    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
    vTaskDelete(NULL);
}

void menu_task(void* pvParameter){
    // start the menu task in the menu.c
    // dont forget the memory management!!!
    menu = menu_create_menu();
    menu_display_menu(menu);

    qwiic_twist_start_task(qwiic_twist_rotary);

    while(1)
    {
        vTaskDelay(500 / portTICK_RATE_MS);
    }
    menu_free_all(menu);
    vTaskDelete(NULL);
}

void app_main()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    //nvs_flash_init();
    i2c_master_init();
    set_rotary_encoder();

    //xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);
    xTaskCreate(&menu_task, "menu_task", 2048, NULL, 5, NULL);
}



/*
 * This method handles the key event "OK" (onButtonClicked), this is necessary for navigating through the menu.
 */
void clicked(void){
    ESP_LOGI(MAINTAG, "clicked rotary encoder");
    menu_handle_key_event(menu, KEY_CLICKED);
}

/*
 *  This method is not used. Its a placeholder method (onButtonPressed).
 */
void pressed(void){
    ESP_LOGI(MAINTAG, "pressed rotary encoder");
}

/*
 *  This method handles the key event turning left and right (onMoved). This is necessary for navigating through the menu, cause this is the scrolling event.
 */
void onMove(int16_t move_value){
    ESP_LOGI(MAINTAG, "moving...");
    if(move_value > 0){
        menu_handle_key_event(menu, KEY_RIGHT);
    }
    else if(move_value < 0){
        menu_handle_key_event(menu, KEY_LEFT);
    }
}
