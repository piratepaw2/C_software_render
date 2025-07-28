

uint32_t *pixelBuffer;

BITMAPINFO bitmapInfo;


void ResizeBuffer(int width, int height) {
    if (pixelBuffer) {
        VirtualFree(pixelBuffer, 0, MEM_RELEASE);
    }
    
    bufferWidth = width;
    bufferHeight = height;
    
    pixelBuffer = (uint32_t*)VirtualAlloc(0, width * height * sizeof(uint32_t), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    
    bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
    bitmapInfo.bmiHeader.biWidth = width;
    bitmapInfo.bmiHeader.biHeight = -height; // top-down DIB
    bitmapInfo.bmiHeader.biPlanes = 1;
    bitmapInfo.bmiHeader.biBitCount = 32;
    bitmapInfo.bmiHeader.biCompression = BI_RGB;
}

/*
 
//Format for drawing
//each pixel is done RRGGBB
    for (int y = 0; y < bufferHeight; ++y) {
        for (int x = 0; x < bufferWidth; ++x) {
            uint8_t r = 0xF0;
            uint8_t g = 0x20;
            uint8_t b = 0xA0;
            
            pixelBuffer[y * bufferWidth + x] = (r) | (g << 8) | (b << 16);
        }
    } 


*/

typedef struct{
    int top;
    int bottom;
    int right;
    int left;
    int color;
}Rect;

void Render() {
    
    Rect background = {bufferHeight, 0, bufferWidth, 0, 0x000000};
    Rect drawbox = {.9 * bufferHeight, .1 * bufferHeight,.4 * bufferWidth, .1 * bufferWidth, 0xFFFFFF};
    
    
    
    
    
    //black background
    for (int y = background.bottom; y < background.top; ++y) {
        for (int x = background.left; x < background.right; ++x) {
            pixelBuffer[y * bufferWidth + x] = background.color;
        }
    } 
    
    //white drawing box that will be on the left side of the screen
    for (int y = drawbox.bottom; y < drawbox.top; ++y) {
        for (int x = drawbox.left; x < drawbox.right; ++x) {
            pixelBuffer[y * bufferWidth + x] = drawbox.color;
        }
    }
    
    
}

