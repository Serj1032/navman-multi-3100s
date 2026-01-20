#ifndef DIYables_TFT_Touch_Shield_H
#define DIYables_TFT_Touch_Shield_H

#include <Arduino.h>

#ifdef HOST_BUILD
#include <SDL3/SDL.h>
#endif

// Mock TFT display using SDL3 for host testing
class DIYables_TFT_ILI9488_Shield {
public:
    DIYables_TFT_ILI9488_Shield();
    ~DIYables_TFT_ILI9488_Shield();
    
    void begin();
    void setRotation(uint8_t rotation);
    void fillScreen(uint16_t color);
    
    // Text methods
    void setTextColor(uint16_t color);
    void setTextSize(uint8_t size);
    void setCursor(int16_t x, int16_t y);
    void print(const char* text);
    
    // Drawing methods
    void drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg_color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color);
    void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
    
private:
#ifdef HOST_BUILD
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    int16_t cursor_x_;
    int16_t cursor_y_;
    uint16_t text_color_;
    uint8_t text_size_;
    uint8_t rotation_;
    int screen_width_;
    int screen_height_;
    
    // Helper to convert RGB565 to RGB888
    void color565ToRGB(uint16_t color, uint8_t& r, uint8_t& g, uint8_t& b);
    void setSDLColor(uint16_t color);
#endif
};

#endif // DIYables_TFT_Touch_Shield_H
