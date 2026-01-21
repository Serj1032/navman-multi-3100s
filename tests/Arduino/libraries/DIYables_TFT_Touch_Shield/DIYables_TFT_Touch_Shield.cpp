#include "DIYables_TFT_Touch_Shield.h"

#ifdef HOST_BUILD
#include <cmath>
#include <cstring>
#include <utility>

// Simple 5x7 bitmap font for ASCII characters 32-127
// Each character is 5 bytes (5 columns x 7 rows, packed into bytes)
static const uint8_t font5x7[][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // 32 (space)
    {0x00, 0x00, 0x5F, 0x00, 0x00}, // 33 !
    {0x00, 0x07, 0x00, 0x07, 0x00}, // 34 "
    {0x14, 0x7F, 0x14, 0x7F, 0x14}, // 35 #
    {0x24, 0x2A, 0x7F, 0x2A, 0x12}, // 36 $
    {0x23, 0x13, 0x08, 0x64, 0x62}, // 37 %
    {0x36, 0x49, 0x55, 0x22, 0x50}, // 38 &
    {0x00, 0x05, 0x03, 0x00, 0x00}, // 39 '
    {0x00, 0x1C, 0x22, 0x41, 0x00}, // 40 (
    {0x00, 0x41, 0x22, 0x1C, 0x00}, // 41 )
    {0x14, 0x08, 0x3E, 0x08, 0x14}, // 42 *
    {0x08, 0x08, 0x3E, 0x08, 0x08}, // 43 +
    {0x00, 0x50, 0x30, 0x00, 0x00}, // 44 ,
    {0x08, 0x08, 0x08, 0x08, 0x08}, // 45 -
    {0x00, 0x60, 0x60, 0x00, 0x00}, // 46 .
    {0x20, 0x10, 0x08, 0x04, 0x02}, // 47 /
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 48 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 49 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 50 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 51 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 52 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 53 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 54 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 55 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 56 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 57 9
    {0x00, 0x36, 0x36, 0x00, 0x00}, // 58 :
    {0x00, 0x56, 0x36, 0x00, 0x00}, // 59 ;
    {0x08, 0x14, 0x22, 0x41, 0x00}, // 60 <
    {0x14, 0x14, 0x14, 0x14, 0x14}, // 61 =
    {0x00, 0x41, 0x22, 0x14, 0x08}, // 62 >
    {0x02, 0x01, 0x51, 0x09, 0x06}, // 63 ?
    {0x32, 0x49, 0x79, 0x41, 0x3E}, // 64 @
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // 65 A
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // 66 B
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // 67 C
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // 68 D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // 69 E
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // 70 F
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // 71 G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // 72 H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // 73 I
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // 74 J
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // 75 K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // 76 L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // 77 M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // 78 N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // 79 O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // 80 P
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // 81 Q
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // 82 R
    {0x46, 0x49, 0x49, 0x49, 0x31}, // 83 S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // 84 T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // 85 U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // 86 V
    {0x3F, 0x40, 0x38, 0x40, 0x3F}, // 87 W
    {0x63, 0x14, 0x08, 0x14, 0x63}, // 88 X
    {0x07, 0x08, 0x70, 0x08, 0x07}, // 89 Y
    {0x61, 0x51, 0x49, 0x45, 0x43}, // 90 Z
    {0x00, 0x7F, 0x41, 0x41, 0x00}, // 91 [
    {0x02, 0x04, 0x08, 0x10, 0x20}, // 92 backslash
    {0x00, 0x41, 0x41, 0x7F, 0x00}, // 93 ]
    {0x04, 0x02, 0x01, 0x02, 0x04}, // 94 ^
    {0x40, 0x40, 0x40, 0x40, 0x40}, // 95 _
    {0x00, 0x01, 0x02, 0x04, 0x00}, // 96 `
    {0x20, 0x54, 0x54, 0x54, 0x78}, // 97 a
    {0x7F, 0x48, 0x44, 0x44, 0x38}, // 98 b
    {0x38, 0x44, 0x44, 0x44, 0x20}, // 99 c
    {0x38, 0x44, 0x44, 0x48, 0x7F}, // 100 d
    {0x38, 0x54, 0x54, 0x54, 0x18}, // 101 e
    {0x08, 0x7E, 0x09, 0x01, 0x02}, // 102 f
    {0x0C, 0x52, 0x52, 0x52, 0x3E}, // 103 g
    {0x7F, 0x08, 0x04, 0x04, 0x78}, // 104 h
    {0x00, 0x44, 0x7D, 0x40, 0x00}, // 105 i
    {0x20, 0x40, 0x44, 0x3D, 0x00}, // 106 j
    {0x7F, 0x10, 0x28, 0x44, 0x00}, // 107 k
    {0x00, 0x41, 0x7F, 0x40, 0x00}, // 108 l
    {0x7C, 0x04, 0x18, 0x04, 0x78}, // 109 m
    {0x7C, 0x08, 0x04, 0x04, 0x78}, // 110 n
    {0x38, 0x44, 0x44, 0x44, 0x38}, // 111 o
    {0x7C, 0x14, 0x14, 0x14, 0x08}, // 112 p
    {0x08, 0x14, 0x14, 0x18, 0x7C}, // 113 q
    {0x7C, 0x08, 0x04, 0x04, 0x08}, // 114 r
    {0x48, 0x54, 0x54, 0x54, 0x20}, // 115 s
    {0x04, 0x3F, 0x44, 0x40, 0x20}, // 116 t
    {0x3C, 0x40, 0x40, 0x20, 0x7C}, // 117 u
    {0x1C, 0x20, 0x40, 0x20, 0x1C}, // 118 v
    {0x3C, 0x40, 0x30, 0x40, 0x3C}, // 119 w
    {0x44, 0x28, 0x10, 0x28, 0x44}, // 120 x
    {0x0C, 0x50, 0x50, 0x50, 0x3C}, // 121 y
    {0x44, 0x64, 0x54, 0x4C, 0x44}, // 122 z
    {0x00, 0x08, 0x36, 0x41, 0x00}, // 123 {
    {0x00, 0x00, 0x7F, 0x00, 0x00}, // 124 |
    {0x00, 0x41, 0x36, 0x08, 0x00}, // 125 }
    {0x10, 0x08, 0x08, 0x10, 0x08}, // 126 ~
};

DIYables_TFT_ILI9488_Shield::DIYables_TFT_ILI9488_Shield() 
    : window_(nullptr), renderer_(nullptr), framebuffer_(nullptr), font_atlas_(nullptr),
      cursor_x_(0), cursor_y_(0), text_color_(0xFFFF), text_size_(1), rotation_(0),
      screen_width_(480), screen_height_(320) {
}

DIYables_TFT_ILI9488_Shield::~DIYables_TFT_ILI9488_Shield() {
    if (font_atlas_) {
        SDL_DestroyTexture(font_atlas_);
    }
    if (framebuffer_) {
        SDL_DestroyTexture(framebuffer_);
    }
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
    
    // Create framebuffer texture to persist display content
    framebuffer_ = SDL_CreateTexture(renderer_, 
                                     SDL_PIXELFORMAT_RGBA8888,
                                     SDL_TEXTUREACCESS_TARGET,
                                     screen_width_, screen_height_);
    if (!framebuffer_) {
        SDL_DestroyRenderer(renderer_);
        SDL_DestroyWindow(window_);
        renderer_ = nullptr;
        window_ = nullptr;
        return;
    }
    
    // Set framebuffer as render target and clear it
    SDL_SetRenderTarget(renderer_, framebuffer_);
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
    setSDLColor(0x0000); // Black
    SDL_RenderClear(renderer_);
    SDL_SetRenderTarget(renderer_, nullptr); // Back to window
    
    // Create font atlas
    createFontAtlas();
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
    if (!renderer_ || !framebuffer_) return;
    SDL_SetRenderTarget(renderer_, framebuffer_);
    
    // Temporarily disable blending to fully replace all pixels
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_NONE);
    setSDLColor(color);
    
    // Clear the entire framebuffer
    SDL_RenderClear(renderer_);
    
    // Fill entire screen to ensure complete coverage
    SDL_FRect fullscreen = {0.0f, 0.0f, (float)screen_width_, (float)screen_height_};
    SDL_RenderFillRect(renderer_, &fullscreen);
    
    // Restore blending for subsequent drawing operations
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer_, nullptr);
}

void DIYables_TFT_ILI9488_Shield::createFontAtlas() {
    // Create atlas texture with 2x resolution: 16 chars wide x 6 chars tall = 95 printable ASCII chars
    const int scale = 2; // Resolution multiplier
    const int atlas_width = 16 * 6 * scale;  // 16 columns * 6 pixels per char * scale
    const int atlas_height = 6 * 8 * scale;  // 6 rows * 8 pixels per char * scale
    
    font_atlas_ = SDL_CreateTexture(renderer_,
                                    SDL_PIXELFORMAT_RGBA8888,
                                    SDL_TEXTUREACCESS_TARGET,
                                    atlas_width, atlas_height);
    if (!font_atlas_) return;
    
    // Set atlas as render target
    SDL_SetRenderTarget(renderer_, font_atlas_);
    SDL_SetTextureBlendMode(font_atlas_, SDL_BLENDMODE_BLEND);
    
    // Clear atlas with transparent background
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 0);
    SDL_RenderClear(renderer_);
    
    // Render all characters in white at higher resolution
    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
    
    for (int c = 0; c < 95; c++) {  // 95 printable ASCII chars (32-126)
        int atlas_x = (c % 16) * 6 * scale;
        int atlas_y = (c / 16) * 8 * scale;
        
        const uint8_t* glyph = font5x7[c];
        
        // Render character bitmap at higher resolution
        for (int col = 0; col < 5; col++) {
            uint8_t column_data = glyph[col];
            for (int row = 0; row < 7; row++) {
                if (column_data & (1 << row)) {
                    SDL_FRect pixel = {
                        (float)(atlas_x + col * scale),
                        (float)(atlas_y + row * scale),
                        (float)scale,
                        (float)scale
                    };
                    SDL_RenderFillRect(renderer_, &pixel);
                }
            }
        }
    }
    
    // Restore render target
    SDL_SetRenderTarget(renderer_, nullptr);
}

extern bool main_loop_running;

void DIYables_TFT_ILI9488_Shield::processEvents() {
    if (!window_ || !framebuffer_) return;
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            main_loop_running = false;
            printf("SDL Quit event received. Exiting main loop.\n");
        }
    }
    
    // Copy framebuffer to screen
    SDL_SetRenderTarget(renderer_, nullptr);
    SDL_RenderClear(renderer_);
    SDL_RenderTexture(renderer_, framebuffer_, nullptr, nullptr);
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
    if (!renderer_ || !framebuffer_ || !font_atlas_ || !text) return;
    
    SDL_SetRenderTarget(renderer_, framebuffer_);
    
    // Get current color for tinting
    uint8_t r, g, b;
    color565ToRGB(text_color_, r, g, b);
    SDL_SetTextureColorMod(font_atlas_, r, g, b);
    
    // Disable blending for the font atlas to ensure solid pixel replacement
    // This allows "erasing" text by drawing it in the background color
    SDL_SetTextureBlendMode(font_atlas_, SDL_BLENDMODE_NONE);
    
    int16_t x = cursor_x_;
    int16_t y = cursor_y_;
    const int char_width = 6;
    const int char_height = 8;
    const int scale = 2; // Match the atlas scale
    
    while (*text) {
        if (*text == '\n') {
            x = cursor_x_;
            y += char_height * text_size_;
            text++;
            continue;
        }
        
        // Get character index
        uint8_t c = *text;
        if (c < 32 || c > 126) c = 32;
        int char_index = c - 32;
        
        // Calculate atlas source rect (at higher resolution)
        int atlas_x = (char_index % 16) * char_width * scale;
        int atlas_y = (char_index / 16) * char_height * scale;
        
        SDL_FRect src = {(float)atlas_x, (float)atlas_y, (float)(5 * scale), (float)(7 * scale)};
        SDL_FRect dst = {
            (float)x,
            (float)y,
            (float)(5 * text_size_),
            (float)(7 * text_size_)
        };
        
        SDL_RenderTexture(renderer_, font_atlas_, &src, &dst);
        
        x += char_width * text_size_;
        text++;
    }
    
    // Restore blending mode for font atlas
    SDL_SetTextureBlendMode(font_atlas_, SDL_BLENDMODE_BLEND);
    
    SDL_SetRenderTarget(renderer_, nullptr);
}

void DIYables_TFT_ILI9488_Shield::drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, 
                                              int16_t w, int16_t h, uint16_t color, uint16_t bg_color) {
    if (!renderer_ || !framebuffer_ || !bitmap) return;
    
    SDL_SetRenderTarget(renderer_, framebuffer_);
    
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
    
    SDL_SetRenderTarget(renderer_, nullptr);
}

void DIYables_TFT_ILI9488_Shield::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (!renderer_ || !framebuffer_) return;
    
    SDL_SetRenderTarget(renderer_, framebuffer_);
    setSDLColor(color);
    SDL_FRect rect = {(float)x, (float)y, (float)w, (float)h};
    SDL_RenderFillRect(renderer_, &rect);
    SDL_SetRenderTarget(renderer_, nullptr);
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
    if (!renderer_ || !framebuffer_) return;
    
    SDL_SetRenderTarget(renderer_, framebuffer_);
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
    
    SDL_SetRenderTarget(renderer_, nullptr);
}

void DIYables_TFT_ILI9488_Shield::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    if (!renderer_ || !framebuffer_) return;
    
    SDL_SetRenderTarget(renderer_, framebuffer_);
    setSDLColor(color);
    SDL_RenderLine(renderer_, x0, y0, x1, y1);
    SDL_SetRenderTarget(renderer_, nullptr);
}

void DIYables_TFT_ILI9488_Shield::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, 
                                                 int16_t x2, int16_t y2, uint16_t color) {
    if (!renderer_ || !framebuffer_) return;
    
    SDL_SetRenderTarget(renderer_, framebuffer_);
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
    
    SDL_SetRenderTarget(renderer_, nullptr);
}

#else
// Non-host build stubs
DIYables_TFT_ILI9488_Shield::DIYables_TFT_ILI9488_Shield() {}
DIYables_TFT_ILI9488_Shield::~DIYables_TFT_ILI9488_Shield() {}
void DIYables_TFT_ILI9488_Shield::begin() {}
void DIYables_TFT_ILI9488_Shield::processEvents() {}
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
