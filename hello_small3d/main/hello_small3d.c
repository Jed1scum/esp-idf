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

// This tells the library the resolution of our ASCII screen.
#define S3L_RESOLUTION_X 320
#define S3L_RESOLUTION_Y 240

// This tells the library the name of a function we use to write pixels.
#define S3L_PIXEL_FUNCTION drawPixel

#include "../components/small3d/small3dlib.h"



#define U S3L_FRACTIONS_PER_UNIT // this is the library unit, like e.g. 1 meter

S3L_Unit triangleVertices[]   = {  // x,   y,   z 
                                      U,   0,   0,     // vertex 1
                                      0,   U,   0,     // vertex 2
                                     -U,   U/2, 0 };   // vertex 3

S3L_Index triangleTriangles[] = { 0, 1, 2 }; // our single triangle

#undef U

#define SCREEN_SIZE (S3L_RESOLUTION_X * S3L_RESOLUTION_Y)



//UI
UG_GUI gui;
const int screenWidth = 320;
const int screenHeight = 240;
uint16_t fb[320 * 240];
static void pset(UG_S16 x, UG_S16 y, UG_COLOR color)
{
    fb[y * 320 + x] = color;
}


void App();
void MonitorInput();
void ui_update_display();
void DisplayMessage(const char*);
void DrawUGUI();
void DrawSmall3d();

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

    DrawUGUI();
    DrawSmall3d();
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

void DrawUGUI(){
    UG_FillFrame(0, 40, screenWidth, 20, C_BLUE);
    UG_DrawTriangle(200,120,1,1,20,C_PURPLE);
    UG_FillTriangle(100,180,1,1,20,C_GREEN);

    UG_DrawPixel(100, 200, C_HOT_PINK);
    UG_DrawCircle(50,200,20,C_RED);
    UG_FillCircle(50,200,10,C_ROSY_BROWN);
    UG_DrawLine(0, 300,200, 300,C_YELLOW);
    ui_update_display();
}

void ui_update_display()
{
    write_frame_rectangleLE(0, 0, screenWidth, screenHeight, fb);
}

//injection method for small3d
void drawPixel(S3L_PixelInfo *p){
    short x = p->x;
    short y = p->y;

    UG_DrawPixel(x, y, C_BLACK);
}

void DrawSmall3d(){
    
    /*for (int i = 0; i < SCREEN_SIZE; ++i) // init the screen
    screen[i] = '.';
    */
    S3L_Model3D triangleModel; // 3D model representing our triangle
    S3L_initModel3D(triangleVertices,9,triangleTriangles,1,&triangleModel);

    S3L_Scene scene;           // scene of 3D models (we only have 1)
    S3L_initScene(&triangleModel,1,&scene);

    // shift the camera a little bit so that we can see the triangle
    scene.camera.transform.translation.z = -2 * S3L_FRACTIONS_PER_UNIT;
    scene.camera.transform.translation.y = S3L_FRACTIONS_PER_UNIT / 2;

    S3L_newFrame();            // has to be called before each frame
    S3L_drawScene(scene);      /* this starts the scene rendering, the library
                                    will now start calling our drawPixel function to
                                    render the camera view */
    int index = 0;

    /*
    for (int y = 0; y < S3L_RESOLUTION_Y; ++y) // now display the screen
    {
        for (int x = 0; x < S3L_RESOLUTION_X; ++x)
        {
        putchar(screen[index]);
        index++;
        }

        putchar('\n');
    }*/

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