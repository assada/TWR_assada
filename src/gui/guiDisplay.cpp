#include "guiDisplay.h"
#include <math.h>

Adafruit_SSD1306 mainDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void GuiDisplay::setupDisplay()
{
    mainDisplay.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS, false);
    mainDisplay.dim(true);
}

void GuiDisplay::displaySplash(String text)
{
    mainDisplay.clearDisplay();
    mainDisplay.setTextSize(1);
    mainDisplay.setTextColor(WHITE);

    mainDisplay.setTextSize(1);
    mainDisplay.setFont(&FreeSans9pt7b);
    mainDisplay.setCursor(0, 15);
    mainDisplay.print(text);
    mainDisplay.setCursor(0, 0);
    mainDisplay.display();
    mainDisplay.setCursor(0, 0);
}

void GuiDisplay::putText(String text, int x, int y, int size)
{
    mainDisplay.setTextSize(size);
    mainDisplay.setTextColor(WHITE);
    mainDisplay.setFont();
    mainDisplay.setCursor(x, y);
    mainDisplay.print(text);
    mainDisplay.display();
}

void GuiDisplay::clearDisplay()
{
    mainDisplay.clearDisplay();
    mainDisplay.display();
}

void GuiDisplay::Start()
{

    this->setupDisplay();
    this->displaySplash("TEST");
    mainDisplay.dim(false);

    taskFinished = false;
    xTaskCreatePinnedToCore(
        this->mainLoop,
        "mainDisplay",      
        5000,
        this,
        taskPriority,
        &Task1,
        taskCore
    ); 

}

void GuiDisplay::dot(int16_t x, int16_t y)
{
    // mainDisplay.drawChar(x, y, '.',  WHITE, BLACK, 1);
    mainDisplay.drawPixel(x, y, WHITE);
    mainDisplay.display();
}

void GuiDisplay::PauseLoop()
{
    vTaskSuspend(Task1);
}

void GuiDisplay::ResumeLoop()
{
    vTaskResume(Task1);
}

void GuiDisplay::mainLoop(void *pvParameters) {
    GuiDisplay *l_pThis = (GuiDisplay *) pvParameters; 
    long timeGui = 0;
    for (;;) {
        unsigned long now = millis();
        timeGui = now - timeGui;
        vTaskDelay(10 / portTICK_PERIOD_MS);
        
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            int y = (sin(x * 2 * M_PI / SCREEN_WIDTH) + 1) * SCREEN_HEIGHT / 2;
            l_pThis->dot(x, y);
            l_pThis->dot(SCREEN_WIDTH-x, y);
            l_pThis->dot(x, SCREEN_HEIGHT-y);
            // if (x == SCREEN_WIDTH - 1) {
            //     l_pThis->clearDisplay();
            //     j++;
            // }
        }
    }
}