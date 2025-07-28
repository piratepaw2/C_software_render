#define UNICODE
#define _UNICODE
#include "utils.c"
#include <windows.h>
#include <stdint.h>

#define WIDTH 800
#define HEIGHT 600


int mouse1Down = 0;
POINT mousePos;

int bufferWidth = WIDTH;
int bufferHeight = HEIGHT;
#include "render.c"
#include "paint.c"



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_SIZE: {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            ResizeBuffer(width, height);
        } break;
        
        case WM_LBUTTONDOWN: {
            mouse1Down = 1;
        } break;
        
        case WM_LBUTTONUP: {
            mouse1Down = 0;
        } break;
        
        
        break;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            Render();
            StretchDIBits(hdc, 0, 0, bufferWidth, bufferHeight,
                          0, 0, bufferWidth, bufferHeight,
                          pixelBuffer, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
            EndPaint(hwnd, &ps);
        } break;
        
        case WM_DESTROY:
        PostQuitMessage(0);
        break;
        
        default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    
    return 0;
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"ScratchWindowClass";
    
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    
    RegisterClass(&wc);
    
    HWND hwnd = CreateWindowEx(
                               0,
                               CLASS_NAME,
                               L"Software Renderer",
                               WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                               CW_USEDEFAULT, CW_USEDEFAULT, bufferWidth, bufferHeight,
                               0, 0, hInstance, 0);
    
    // Resize
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    int clientWidth = clientRect.right - clientRect.left;
    int clientHeight = clientRect.bottom - clientRect.top;
    ResizeBuffer(clientWidth, clientHeight);
    
    
    if (!hwnd) return 0;
    
    HDC hdc = GetDC(hwnd);
    
    int running = 1;
    MSG msg = { 0 };
    while (running) {
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = 0;
                break;
            }
            
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        GetCursorPos(&mousePos);
        ScreenToClient(hwnd, &mousePos);
        
        
        Render();
        StretchDIBits(hdc, 0, 0, bufferWidth, bufferHeight, 0,
                      0, bufferWidth, bufferHeight,
                      pixelBuffer,
                      &bitmapInfo,
                      DIB_RGB_COLORS,
                      SRCCOPY);
        
        Sleep(16);
    }
    
    ReleaseDC(hwnd, hdc);
    
    return 0;
}
