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
#include "gamepad.h"

#include "../components/ugui/ugui.h"

UG_GUI gui;
uint16_t fb[320 * 240];

void ScreenMain();
void helloWorldBase();
void initialize_nvs();
void  ui_update_display();
void DisplayMessage(const char*);

static void pset(UG_S16 x, UG_S16 y, UG_COLOR color)
{
    fb[y * 320 + x] = color;
}

void app_main()
{

    printf("Hello esp\n");
    initialize_nvs();
    ScreenMain();
    //helloWorldBase();
    
}

void ScreenMain(){
    
    printf("ScreenMain()");

    gamepad_init();


    display_init();
    //display_show_splash();
    display_clear(0xffff);

    UG_Init(&gui, pset, 320, 240);

    //menu_main();
    DisplayMessage("Hello ESP Play");

    while(1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    //indicate_error();
}



void DisplayMessage(const char* message)
{
    printf("DisplayMessage()");

    UG_FillFrame(0, 0, 319, 239, C_WHITE);

    // Header
    UG_FillFrame(0, 0, 319, 15, C_MIDNIGHT_BLUE);
    UG_FontSelect(&FONT_8X8);
    const short titleLeft = (320 / 2) - (strlen(message) * 9 / 2);
    UG_SetForecolor(C_WHITE);
    UG_SetBackcolor(C_MIDNIGHT_BLUE);
    UG_PutString(titleLeft, 4, message);

    /*
    UG_FontSelect(&FONT_8X12);
    short left = (320 / 2) - (strlen(message) * 9 / 2);
    short top = (240 / 2) + 8 + (12 / 2) + 16;
    UG_SetForecolor(C_BLACK);
    UG_SetBackcolor(C_WHITE);
    UG_FillFrame(0, top, 319, top + 12, C_WHITE);
    UG_PutString(left, top, message);
    */

    ui_update_display();
}



void helloWorldBase(){
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

    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}

void initialize_nvs()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK( nvs_flash_erase() );
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
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
