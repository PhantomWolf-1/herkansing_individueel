set(COMPONENT_SRCS "main.c")
set(COMPONENT_ADD_INCLUDEDIRS .)
set(COMPONENT_REQUIRES  nvs_flash 
                        esp32-smbus 
                        esp32-i2c-lcd1602 
                        esp32-i2c-rotaryEncoder 
                        )
register_component()