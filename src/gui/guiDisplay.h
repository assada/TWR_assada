#pragma once

#include "Adafruit_SSD1306.h"
#include <Adafruit_GFX.h>
#include <Fonts/FreeSansBold9pt7b.h>

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32



class GuiDisplay
{
    public:
        void setupDisplay();
        void displaySplash(String text);
        static void mainLoop(void *pvParameters);
        void putText(String text, int x, int y, int size);
        void clearDisplay();
        void Start();
        volatile bool taskFinished = true;
    private:
        TaskHandle_t Task1;
        const int taskCore = 0;
        const int taskPriority = 1;
};