#include "DIYables_TFT_Touch_Shield.h"

#ifdef HOST_BUILD
#include <cmath>
#include <cstring>
#include <utility>

DIYables_TFT_ILI9488_Shield::DIYables_TFT_ILI9488_Shield() 
    : window_(nullptr), renderer_(nullptr), cursor_x_(0), cursor_y_(0),
      text_color_(0xFFFF), text_size_(1), rotation_(0),
      screen_width_(480), screen_height_(320) {
}

DIYables_TFT_ILI9488_Shield::~DIYables_TFT_ILI9488_Shield() {
    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
    }
    if (window_) {
        SDL_DestroyWindow(window_);
    }
}

void DIYables_TFT_ILI9488_Shield::begin() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return;
    }
    
    window_ = SDL_CreateWindow(
        "TFT Display Emulator",
        screen_width_, screen_height_,
        0
    );
    
    if (!window_) {
        return;
    }
    
    renderer_ = SDL_CreateRenderer(window_, nullptr);
    if (!renderer_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
        return;
    }
    
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
    fillScreen(0x0000); // Black
    SDL_RenderPresent(renderer_);
}

void DIYables_TFT_ILI9488_Shield::setRotation(uint8_t rotation) {
    rotation_ = rotation;
    // In a full implementation, this would swap width/height for rotations 1 and 3
    if (rotation == 1 || rotation == 3) {
        screen_width_ = 480;
        screen_height_ = 320;
    } else {
        screen_width_ = 320;
        screen_height_ = 480;
    }
}

void DIYables_TFT_ILI9488_Shield::color565ToRGB(uint16_t color, uint8_t& r, uint8_t& g, uint8_t& b) {
    // RGB565 format: RRRRRGGGGGGBBBBB
    r = (color >> 11) * 255 / 31;
    g = ((color >> 5) & 0x3F) * 255 / 63;
    b = (color & 0x1F) * 255 / 31;
}

void DIYables_TFT_ILI9488_Shield::setSDLColor(uint16_t color) {
    uint8_t r, g, b;
    color565ToRGB(color, r, g, b);
    SDL_SetRenderDrawColor(renderer_, r, g, b, 255);
}

void DIYables_TFT_ILI9488_Shield::fillScreen(uint16_t color) {
    if (!renderer_) return;
    setSDLColor(color);
    SDL_RenderClear(renderer_);
    SDL_RenderPresent(renderer_);
}

void DIYables_TFT_ILI9488_Shield::setTextColor(uint16_t color) {
    text_color_ = color;
}

void DIYables_TFT_ILI9488_Shield::setTextSize(uint8_t size) {
    text_size_ = size;
}

void DIYables_TFT_ILI9488_Shield::setCursor(int16_t x, int16_t y) {
    cursor_x_ = x;
    cursor_y_ = y;
}

void DIYables_TFT_ILI9488_Shield::print(const char* text) {
    if (!renderer_ || !text) return;
    
    // Simple character rendering using rectangles (5x7 pixels per character)
    setSDLColor(text_color_);
    
    int16_t x = cursor_x_;
    int16_t y = cursor_y_;
    int char_width = 6 * text_size_;
    int char_height = 8 * text_size_;
    
    // Very basic text rendering - just draw rectangles for now
    // In a full implementation, you'd render actual glyphs
    while (*text) {
        if (*text == '\n') {
            x = cursor_x_;
            y += char_height;
        } else {
            SDL_FRect rect = {(float)x, (float)y, (float)(char_width - text_size_), (float)char_height};
            SDL_RenderFillRect(renderer_, &rect);
            x += char_width;
        }
        text++;
    }
    
    SDL_RenderPresent(renderer_);
}

void DIYables_TFT_ILI9488_Shield::drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, 
                                              int16_t w, int16_t h, uint16_t color, uint16_t bg_color) {
    if (!renderer_ || !bitmap) return;
    
    uint8_t r_fg, g_fg, b_fg;
    uint8_t r_bg, g_bg, b_bg;
    color565ToRGB(color, r_fg, g_fg, b_fg);
    color565ToRGB(bg_color, r_bg, g_bg, b_bg);
    
    for (int16_t j = 0; j < h; j++) {
        for (int16_t i = 0; i < w; i++) {
            int byte_idx = (j * ((w + 7) / 8)) + (i / 8);
            int bit_idx = 7 - (i % 8);
            
            if (bitmap[byte_idx] & (1 << bit_idx)) {
                SDL_SetRenderDrawColor(renderer_, r_fg, g_fg, b_fg, 255);
            } else {
                SDL_SetRenderDrawColor(renderer_, r_bg, g_bg, b_bg, 255);
            }
            
            SDL_FRect rect = {(float)(x + i), (float)(y + j), 1.0f, 1.0f};
            SDL_RenderFillRect(renderer_, &rect);
        }
    }
    
    SDL_RenderPresent(renderer_);
}

void DIYables_TFT_ILI9488_Shield::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (!renderer_) return;
    
    setSDLColor(color);
    SDL_FRect rect = {(float)x, (float)y, (float)w, (float)h};
    SDL_RenderFillRect(renderer_, &rect);
    SDL_RenderPresent(renderer_);
}

void DIYables_TFT_ILI9488_Shield::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, 
                                                 int16_t radius, uint16_t color) {
    if (!renderer_) return;
    
    // Simplified rounded rectangle - just draw a regular rectangle for now
    // Full implementation would draw arcs at corners
    (void)radius; // Unused parameter
    fillRect(x, y, w, h, color);
}

void DIYables_TFT_ILI9488_Shield::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    if (!renderer_) return;
    
    setSDLColor(color);
    
    // Midpoint circle algorithm
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 1 - r;
    
    while (x <= y) {
        SDL_RenderPoint(renderer_, x0 + x, y0 + y);
        SDL_RenderPoint(renderer_, x0 - x, y0 + y);
        SDL_RenderPoint(renderer_, x0 + x, y0 - y);
        SDL_RenderPoint(renderer_, x0 - x, y0 - y);
        SDL_RenderPoint(renderer_, x0 + y, y0 + x);
        SDL_RenderPoint(renderer_, x0 - y, y0 + x);
        SDL_RenderPoint(renderer_, x0 + y, y0 - x);
        SDL_RenderPoint(renderer_, x0 - y, y0 - x);
        
        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
    
    SDL_RenderPresent(renderer_);
}

void DIYables_TFT_ILI9488_Shield::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    if (!renderer_) return;
    
    setSDLColor(color);
    SDL_RenderLine(renderer_, x0, y0, x1, y1);
    SDL_RenderPresent(renderer_);
}

void DIYables_TFT_ILI9488_Shield::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, 
                                                 int16_t x2, int16_t y2, uint16_t color) {
    if (!renderer_) return;
    
    setSDLColor(color);
    
    // Simple triangle fill using scanline algorithm
    // Sort vertices by y-coordinate
    if (y0 > y1) { std::swap(y0, y1); std::swap(x0, x1); }
    if (y0 > y2) { std::swap(y0, y2); std::swap(x0, x2); }
    if (y1 > y2) { std::swap(y1, y2); std::swap(x1, x2); }
    
    if (y1 == y2) {
        // Flat bottom triangle
        for (int16_t y = y0; y <= y1; y++) {
            float t = (float)(y - y0) / (y1 - y0);
            int16_t xa = x0 + t * (x1 - x0);
            int16_t xb = x0 + t * (x2 - x0);
            if (xa > xb) std::swap(xa, xb);
            SDL_RenderLine(renderer_, xa, y, xb, y);
        }
    } else if (y0 == y1) {
        // Flat top triangle
        for (int16_t y = y0; y <= y2; y++) {
            float t = (float)(y - y0) / (y2 - y0);
            int16_t xa = x0 + t * (x2 - x0);
            int16_t xb = x1 + t * (x2 - x1);
            if (xa > xb) std::swap(xa, xb);
            SDL_RenderLine(renderer_, xa, y, xb, y);
        }
    } else {
        // General case - split into two triangles
        int16_t x3 = x0 + (float)(y1 - y0) / (y2 - y0) * (x2 - x0);
        fillTriangle(x0, y0, x1, y1, x3, y1, color);
        fillTriangle(x1, y1, x3, y1, x2, y2, color);
    }
    
    SDL_RenderPresent(renderer_);
}

#else
// Non-host build stubs
DIYables_TFT_ILI9488_Shield::DIYables_TFT_ILI9488_Shield() {}
DIYables_TFT_ILI9488_Shield::~DIYables_TFT_ILI9488_Shield() {}
void DIYables_TFT_ILI9488_Shield::begin() {}
void DIYables_TFT_ILI9488_Shield::setRotation(uint8_t rotation) { (void)rotation; }
void DIYables_TFT_ILI9488_Shield::fillScreen(uint16_t color) { (void)color; }
void DIYables_TFT_ILI9488_Shield::setTextColor(uint16_t color) { (void)color; }
void DIYables_TFT_ILI9488_Shield::setTextSize(uint8_t size) { (void)size; }
void DIYables_TFT_ILI9488_Shield::setCursor(int16_t x, int16_t y) { (void)x; (void)y; }
void DIYables_TFT_ILI9488_Shield::print(const char* text) { (void)text; }
void DIYables_TFT_ILI9488_Shield::drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg_color) {
    (void)x; (void)y; (void)bitmap; (void)w; (void)h; (void)color; (void)bg_color;
}
void DIYables_TFT_ILI9488_Shield::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    (void)x; (void)y; (void)w; (void)h; (void)color;
}
void DIYables_TFT_ILI9488_Shield::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color) {
    (void)x; (void)y; (void)w; (void)h; (void)radius; (void)color;
}
void DIYables_TFT_ILI9488_Shield::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    (void)x0; (void)y0; (void)r; (void)color;
}
void DIYables_TFT_ILI9488_Shield::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    (void)x0; (void)y0; (void)x1; (void)y1; (void)color;
}
void DIYables_TFT_ILI9488_Shield::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
    (void)x0; (void)y0; (void)x1; (void)y1; (void)x2; (void)y2; (void)color;
}
#endif
