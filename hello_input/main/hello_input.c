/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "nvs.h"
#include "nvs_flash.h"

//includes
#include "sdcard.h"
#include "display.h"
//#include "gamepad.h"

#include "../components/ugui/ugui.h"
#include "../components/drivers/input/gamepad.h"

//UI
UG_GUI gui;
uint16_t fb[320 * 240];
static void pset(UG_S16 x, UG_S16 y, UG_COLOR color)
{
    fb[y * 320 + x] = color;
}


void App();
void MonitorInput();
void ui_update_display();
void DisplayMessage(const char*);

void app_main()
{
    printf("Hello Input!\n");

    

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    nvs_flash_init();

    gamepad_init();
    display_init();
    
    display_clear(0xffff);

    UG_Init(&gui, pset, 320, 240);


    DisplayMessage("Hello input: \n");

    App();


    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}

void App(){
    while(1)
        {
            MonitorInput();
        
        }

    }

void MonitorInput(){
   const char* result = NULL;
    printf("%s: HEAP=%#010x\n", __func__, esp_get_free_heap_size());
    input_gamepad_state previousState;
    gamepad_read(&previousState);

    while (true)
    {
		input_gamepad_state state;
		gamepad_read(&state);

      
	        if(!previousState.values[GAMEPAD_INPUT_DOWN] && state.values[GAMEPAD_INPUT_DOWN])
	        {
                printf("Press Down");
                DisplayMessage("Down");
                break;
	        }
	        else if(!previousState.values[GAMEPAD_INPUT_UP] && state.values[GAMEPAD_INPUT_UP])
	        {
                 printf("Press Up");
                 DisplayMessage("Up");
                 break;
                 
	        }
	        else if(!previousState.values[GAMEPAD_INPUT_RIGHT] && state.values[GAMEPAD_INPUT_RIGHT])
	        {
                 printf("Press Right");
                 DisplayMessage("Right");
                 break;
                 
	        }
	        else if(!previousState.values[GAMEPAD_INPUT_LEFT] && state.values[GAMEPAD_INPUT_LEFT])
	        {
                 printf("Press Left");
                 DisplayMessage("Left");
                 break;

	        }
	        else if(!previousState.values[GAMEPAD_INPUT_A] && state.values[GAMEPAD_INPUT_A])
	        {
                 printf("Press A");
                 DisplayMessage("A");
                break;
	        }
            else if(!previousState.values[GAMEPAD_INPUT_B] && state.values[GAMEPAD_INPUT_B])
	        {
                 printf("Press B");;
                 DisplayMessage("B");
                break;
	        }
            else if (!previousState.values[GAMEPAD_INPUT_MENU] && state.values[GAMEPAD_INPUT_MENU])
            {
                 printf("Press Menu");
                 DisplayMessage("Menu");
                 break;
            }
            else if (!previousState.values[GAMEPAD_INPUT_START] && state.values[GAMEPAD_INPUT_START])
            {
                 printf("Press Start");
                 DisplayMessage("Start");
                 break;
            }else if (!previousState.values[GAMEPAD_INPUT_SELECT] && state.values[GAMEPAD_INPUT_SELECT])
            {
                 printf("Press Select");
                 DisplayMessage("Select");
                 break;
            }
            else if (!previousState.values[GAMEPAD_INPUT_R] && state.values[GAMEPAD_INPUT_R])
            {
                 printf("Press RB");
                 DisplayMessage("RB");
                 break;
            }
            else if (!previousState.values[GAMEPAD_INPUT_L] && state.values[GAMEPAD_INPUT_L])
            {
                 printf("Press LB");
                 DisplayMessage("LB");
                 break;
            }
		
        previousState = state;
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void DisplayMessage(const char* message)
{
    UG_FillFrame(0, 0, 319, 239, C_WHITE);

    // Header
    UG_FillFrame(0, 0, 319, 20, C_PURPLE);
    UG_FontSelect(&FONT_8X8);
    const short titleLeft = (320 / 2) - (strlen(message) * 9 / 2);
    UG_SetForecolor(C_WHITE);
    UG_SetBackcolor(C_PURPLE);
    UG_PutString(titleLeft, 4, message);


    ui_update_display();
}

void ui_update_display()
{
    write_frame_rectangleLE(0, 0, 320, 240, fb);
}

static void ui_draw_image(short x, short y, short width, short height, uint16_t* data)
{
    for (short i = 0 ; i < height; ++i)
    {
        for (short j = 0; j < width; ++j)
        {
            uint16_t pixel = data[i * width + j];
            UG_DrawPixel(x + j, y + i, pixel);
        }
    }
}