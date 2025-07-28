

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

void Render() {
    /* each pixel is done RRGGBB
    for (int y = 0; y < bufferHeight; ++y) {
        for (int x = 0; x < bufferWidth; ++x) {
            uint8_t r = 0xF0;
            uint8_t g = 0x20;
            uint8_t b = 0xA0;
            
            pixelBuffer[y * bufferWidth + x] = (r) | (g << 8) | (b << 16);
        }
    } 


*/
    
    
    
}

