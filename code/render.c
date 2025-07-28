

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

Rect Convert_Rect(float top, float bottom, float right, float left, int color){
    Rect r;
    r.top = (int)(top * bufferHeight);
    r.bottom = (int)(bottom * bufferHeight);
    r.right = (int)(right * bufferWidth);
    r.left = (int)(left * bufferWidth);
    r.color = color;
    return r;
}

void DrawRect(Rect r){
    for (int y = r.bottom; y < r.top; ++y) {
        for (int x = r.left; x < r.right; ++x) {
            if (x >= 0 && x < bufferWidth && y >= 0 && y < bufferHeight) 
                pixelBuffer[y * bufferWidth + x] = r.color;
        }
    } 
    
}


typedef struct{
    int x;
    int y;
    int xrad;
    int yrad;
    int color;
}Circ;

Circ Convert_Circ(float x, float y, float xrad, float yrad, int color){
    Circ c;
    c.x = (int)(x * bufferWidth);
    c.y = (int)(y * bufferHeight);
    c.xrad = (int)(xrad * bufferWidth);
    c.yrad = (int)(yrad * bufferHeight);
    c.color = color;
    return c;
}

void DrawCirc(Circ c){
    for(int y = c.y - c.yrad; y < c.y + c.yrad; ++y){
        for(int x = c.x - c.xrad; x < c.x + c.xrad; ++x){
            float dx = (float)(x - c.x)/c.xrad;
            float dy = (float)(y - c.y)/c.yrad;
            if((dx * dx + dy * dy) <= 1.0f){
                if(x >= 0 && x < bufferWidth && y >=0 && y < bufferHeight){
                    pixelBuffer[y * bufferWidth + x] = c.color;
                }
            }
        }
    }
}



void Render() {
    //black background
    Rect background = Convert_Rect(1, 0, 1, 0, 0x000000);
    //white drawing box that will be on the left side of the screen
    Rect drawbox = Convert_Rect(.9, .1 ,.7, .025, 0xFFFFFF);
    //Interactive Box
    Rect userbox = Convert_Rect(.9, .1, .98,.75,0xD3D3D3);
    
    DrawRect(background);
    DrawRect(drawbox);
    DrawRect(userbox);
    
    
    if(mouse1Down && mousePos.y < drawbox.top && mousePos.y > drawbox.bottom && mousePos.x < drawbox.right && mousePos.x > drawbox.left){
        Circ draw = {mousePos.x, mousePos.y, 5, 5, 0xFF1DCE};
        DrawCirc(draw);
    }
}




