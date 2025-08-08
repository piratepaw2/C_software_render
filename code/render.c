

uint32_t *pixelBuffer;
uint32_t *canvasBuffer;

BITMAPINFO bitmapInfo;

#define BLANK 0xFFFFFF


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
    
    canvasBuffer = malloc(sizeof(uint32_t) * bufferWidth * bufferHeight);
    for (int i = 0; i < bufferWidth * bufferHeight; i++) {
        canvasBuffer[i] = BLANK;
    }
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

void DrawCircToBuffer(Circ c, uint32_t *targetBuffer) {
    for(int y = c.y - c.yrad; y < c.y + c.yrad; ++y){
        for(int x = c.x - c.xrad; x < c.x + c.xrad; ++x){
            float dx = (float)(x - c.x) / c.xrad;
            float dy = (float)(y - c.y) / c.yrad;
            if((dx * dx + dy * dy) <= 1.0f){
                if(x >= 0 && x < bufferWidth && y >= 0 && y < bufferHeight){
                    targetBuffer[y * bufferWidth + x] = c.color;
                }
            }
        }
    }
}



void Render() {
    
    //Background
    Rect background = Convert_Rect(1.0, 0.0, 1.0, 0.0, 0x000000);
    DrawRect(background);
    //white drawing box that will be on the left side of the screen
    Rect drawbox = Convert_Rect(.9, .1 ,.7, .025, 0xFFFFFF);
    //Interactive Box
    Rect userbox = Convert_Rect(.9, .1, .98,.75,0xD3D3D3);
    //Circles for Different Colors
    Circ color_circles[12] = {
        Convert_Circ(0,0,.02,.02,0x000000),// Black
        Convert_Circ(0,0,.02,.02,0xFFFFFF),// White
        Convert_Circ(0,0,.02,.02,0x808080),// Grey
        Convert_Circ(0,0,.02,.02,0xFF0000),// Red
        Convert_Circ(0,0,.02,.02,0x0000FF),// Blue
        Convert_Circ(0,0,.02,.02,0xFFFF00),// Yellow
        Convert_Circ(0,0,.02,.02,0x006400),// Dark Green
        Convert_Circ(0,0,.02,.02,0x90EE90),// Light Green
        Convert_Circ(0,0,.02,.02,0xFFA500),// Orange
        Convert_Circ(0,0,.02,.02,0x964B00),// Brown
        Convert_Circ(0,0,.02,.02,0xFFC0CB),// Pink
        Convert_Circ(0,0,.02,.02,0xA020f0),// Purple
    };
    
    int static init = 1;//draw background once
    static int drawcolor = 0x000000; //black by default
    
    if(init){
        //Copy Previous Drawing
        for (int y = drawbox.bottom; y < drawbox.top; y++) {
            for (int x = drawbox.left; x < drawbox.right; x++) {
                canvasBuffer[y * bufferWidth + x] = BLANK;
            }
        }
        init = 0;
    }
    
    //Drawing in the Drawbox
    if(mouse1Down && mousePos.y < drawbox.top && mousePos.y > drawbox.bottom && mousePos.x < drawbox.right && mousePos.x > drawbox.left){
        Circ draw = {mousePos.x, mousePos.y, 5, 5, drawcolor};
        DrawCircToBuffer(draw, canvasBuffer);
    }
    
    DrawRect(drawbox);
    DrawRect(userbox);
    
    //User Drawing Buffer
    for (int y = 0; y < bufferHeight; ++y) {
        for (int x = 0; x < bufferWidth; ++x) {
            uint32_t color = canvasBuffer[y * bufferWidth + x];
            if (color != BLANK) {
                pixelBuffer[y * bufferWidth + x] = color;
            }
        }
    }
    
    
    //Draw the Circles && Change Mouse Draw Color
    for(int y = 0; y<6; y++){
        for(int x = 0; x < 2; x++){
            int index = x+y*2;
            if(x == 1)
                color_circles[index].x = .667f * (userbox.right-userbox.left) + userbox.left;
            else color_circles[index].x = .333f * (userbox.right-userbox.left) + userbox.left;
            color_circles[index].y = (float)(y+1)/12.0f * (userbox.top-userbox.bottom) + userbox.bottom;
            
            DrawCirc(color_circles[index]);
            
            if(mouse1Down){
                int dx = mousePos.x - color_circles[index].x;
                int dy = mousePos.y - color_circles[index].y;
                if(dx * dx + dy * dy <= color_circles[index].xrad * color_circles[index].xrad)
                    drawcolor = color_circles[index].color;
            }
        }
    }
    
    
    
}




