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
#include "../components/gb/display_gb.h"


//movement
int shapeXPos = 50;
int shapeYPos = 200;
short xPos;

//message
char* message;
//UI
UG_GUI gui;
const int screenWidth = 320;
const int screenHeight = 240;
//uint16_t fb[320 * 240];

uint16_t* buffer1;
uint16_t* buffer2;
uint16_t* currentFB;
uint16_t* nextFB;

void SwapBuffer();

void SwapBuffer(){
    //switch the pointers
    uint16_t* temp = currentFB;
    currentFB = nextFB;
    nextFB = temp;
}

static void pset(UG_S16 x, UG_S16 y, UG_COLOR color)
{
    //fb[y * 320 + x] = color;
    nextFB[y * screenWidth + x] = color;
}


void App();
void MonitorInput();
void ui_update_display();
void DisplayMessage(const char*);
void DrawUGUI();
void RenderLoop();
void UpdatePlayer();


void app_main()
{
    printf("\n %s: HEAP=%#010x\n", __func__, esp_get_free_heap_size());
    printf("Hello Shapes!\n");
    //Using dynamic ram rather than static ram as we don't have enought static ram
    int screenSize = screenWidth * screenHeight;
    printf("Creating dynamic ram for swap buffers: %d", screenSize);
    buffer1 = calloc(1, screenSize);
    assert(buffer1 != NULL);
    buffer2 = calloc(1, screenSize);
    assert(buffer1 != NULL);

    //Assign buffers to be ready for swap buffering
    currentFB = buffer1;
    nextFB = buffer2;

    nvs_flash_init();
    
    
    /*
    //setup double buffer
    displayBuffer[0] = heap_caps_malloc(screenWidth*screenHeight * 2, MALLOC_CAP_8BIT | MALLOC_CAP_DMA);
    displayBuffer[1] = heap_caps_malloc(screenWidth*screenHeight * 2, MALLOC_CAP_8BIT | MALLOC_CAP_DMA);
    frameBuffer = displayBuffer[0];
    for (int i = 0; i < 2; ++i)
    {
        memset(displayBuffer[i], 0, screenWidth * screenHeight * 2);
    }
    printf("app_main: displayBuffer[0]=%p, [1]=%p\n", displayBuffer[0], displayBuffer[1]);
    */

    gamepad_init();
    display_init();
    
    display_clear(0xffff);

    UG_Init(&gui, pset, 320, 240);


    DisplayMessage("Hello input: \n");

    App();

    fflush(stdout);
    esp_restart();
}



void App(){
   
    //MonitorInput();
    RenderLoop();
    /*
    while(1)
        {
            
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }*/

    }

void MonitorInput(){
   //const char* result = NULL;
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
                message ="Down";
                shapeYPos --;
                break;
	        }
	        else if(!previousState.values[GAMEPAD_INPUT_UP] && state.values[GAMEPAD_INPUT_UP])
	        {
                 printf("Press Up");
                 message ="Up";
                 shapeYPos ++;
                 break;
                 
	        }
	        else if(!previousState.values[GAMEPAD_INPUT_RIGHT] && state.values[GAMEPAD_INPUT_RIGHT])
	        {
                 printf("Press Right");
                 message ="Right";
                 shapeXPos ++;
                 break;
                 
	        }
	        else if(!previousState.values[GAMEPAD_INPUT_LEFT] && state.values[GAMEPAD_INPUT_LEFT])
	        {
                 printf("Press Left");
                 message ="Left";
                 shapeXPos --;
                 break;

	        }
	        else if(!previousState.values[GAMEPAD_INPUT_A] && state.values[GAMEPAD_INPUT_A])
	        {
                 printf("Press A");
                 message ="A";
                break;
	        }
            else if(!previousState.values[GAMEPAD_INPUT_B] && state.values[GAMEPAD_INPUT_B])
	        {
                 printf("Press B");;
                 message ="B";
                break;
	        }
            else if (!previousState.values[GAMEPAD_INPUT_MENU] && state.values[GAMEPAD_INPUT_MENU])
            {
                 printf("Press Menu");
                 message ="Menu";
                 break;
            }
            else if (!previousState.values[GAMEPAD_INPUT_START] && state.values[GAMEPAD_INPUT_START])
            {
                 printf("Press Start");
                 message = "Start";
                 break;
            }else if (!previousState.values[GAMEPAD_INPUT_SELECT] && state.values[GAMEPAD_INPUT_SELECT])
            {
                 printf("Press Select");
                 message ="Select";
                 break;
            }
            else if (!previousState.values[GAMEPAD_INPUT_R] && state.values[GAMEPAD_INPUT_R])
            {
                 printf("Press RB");
                 message = "RB";
                 break;
            }
            else if (!previousState.values[GAMEPAD_INPUT_L] && state.values[GAMEPAD_INPUT_L])
            {
                 printf("Press LB");
                 message ="LB";
                 break;
            }
		
        previousState = state;
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}


void RenderLoop(){
    //DrawUGUI();
    printf("%s: HEAP=%#010x\n", __func__, esp_get_free_heap_size());
    

    while(true){
        
        display_clear(0xffff);
        //UG_FillTriangle(xPos,180,1,1,20,C_GREEN);
        renderGfx(0,0, screenWidth, screenHeight, nextFB,0,0, screenWidth);
        //SwapBuffer();
        //UG_FillFrame(0, 0, 319, 239, C_WHITE);
        //DisplayMessage(message);
        //DrawUGUI();
        //UpdatePlayer();
        //ui_update_display();  
        vTaskDelay(.03 / portTICK_PERIOD_MS);//.03 / portTICK_PERIOD_MS);  
    }
    
}

void DisplayMessage(const char* message)
{
    // Header
    UG_FillFrame(0, 0, 319, 20, C_PURPLE);
    UG_FontSelect(&FONT_8X8);
    const short titleLeft = (320 / 2) - (strlen(message) * 9 / 2);
    UG_SetForecolor(C_WHITE);
    UG_SetBackcolor(C_PURPLE);
    UG_PutString(titleLeft, 4, message);
}

void DrawUGUI(){

    
    //UG_FillFrame(0, 40, screenWidth, 20, C_BLUE);
    //UG_DrawTriangle(200,120,1,1,20,C_PURPLE);
    UG_FillTriangle(xPos,180,1,1,20,C_GREEN);

    //UG_DrawPixel(100, 200, C_HOT_PINK);
    
    //UG_DrawLine(0, 300,200, 300,C_YELLOW);
    //ui_update_display();
}

void UpdatePlayer(){
    UG_DrawCircle(shapeXPos,shapeYPos,20,C_RED);
    UG_FillCircle(shapeXPos,shapeYPos,10,C_ROSY_BROWN);
    //ui_update_display();
    //UG_Update();
}

void ui_update_display()
{
    //do double buffer. https://github.com/OtherCrashOverride/go-play/issues/5
    /*
    // swap buffers
    currentBuffer = currentBuffer ? 0 : 1;
    frameBuffer = displayBuffer[currentBuffer];

    

    for (int i=0; i<screenWidth*screenHeight; i++) {
                int r,g,b;
                r=((frameBuffer[i]>>11)&0x1f)<<3;
                g=((frameBuffer[i]>>5)&0x3f)<<2;
                b=((frameBuffer[i]>>0)&0x1f)<<3;
                uint16_t a = 200;
                r=r*(256-a);
                g=g*(256-a);
                b=b*(256-a);
                frameBuffer[i]=((r>>(3+8))<<11)+((g>>(2+8))<<5)+((b>>(3+8))<<0);
            }
    */
    //fb = frameBuffer;
   //renderGfx(0,0, screenWidth, screenHeight, fb,0,0, screenWidth);

    
}




