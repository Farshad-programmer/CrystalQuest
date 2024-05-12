#include <iostream>
#include <string>

static bool running = true;

// ###############################################################
//                          Platform globals
// ###############################################################

// ###############################################################
//                          Platform Function
// ###############################################################

bool PlatformCreateWidnow(int width, int height, char* title);
void PlatformUpdateWindow();

// ###############################################################
//                          Windows Platform
// ###############################################################
#ifdef _win32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#endif
#include <windows.h>
#include <wtypes.h>
// ###############################################################
//                          Windows globals
// ###############################################################
static HWND window ;
// ###############################################################
//                          Platform Implementation 
// ###############################################################

LRESULT CALLBACK WindowsWindowCallback(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
{
  LRESULT result = 0;
  switch (msg)
  {
  case WM_CLOSE:
    running = false;
    break;
  
  default:
    // let window handle the defualt window for now
    result = DefWindowProcA(window, msg, wparam, lparam);
    break;
  }

  return result;
}


bool PlatformCreateWidnow(int width, int height, char* title)
{
  HINSTANCE instance = GetModuleHandleA(0);

  WNDCLASSA wc = {};
  wc.hInstance = instance;
  wc.hIcon = LoadIcon(instance, IDI_APPLICATION); // default
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);       // defualt arrow cursur
  wc.lpszClassName = title;                       // this is not the title , just uniuq identifeir(id)
  //wc.lpfnWndProc = DefWindowProcA;  //default              // callback for input into the window
  wc.lpfnWndProc = WindowsWindowCallback;


  if(!RegisterClassA(&wc))
  {
    return false;
  }

  // WC_CAPTION | WC_SYSMENU | WC_THICKFRAME | WC_MINIMIZEBOX | WC_MAXIMIZEBOX
  int dwStyle = WS_OVERLAPPEDWINDOW;

  window = CreateWindowExA(0, title, // this refeences lpszClassName from wc
                                  title,// thisis the ctual title
                                  dwStyle,
                                  100,
                                  100,
                                  width,
                                  height,
                                  NULL, // parent 
                                  NULL, // menu
                                  instance,
                                  NULL); // lpparam

  if(window == NULL)
  {
    return false;
  }    
  ShowWindow(window,SW_SHOW);
  return true;                            
}
void PlatformUpdateWindow()
{
  MSG msg;
  while (PeekMessageA(&msg, window, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessageA(&msg); // cals the callback specifed when creating the window 
  }
  
}

int main()
{
  PlatformCreateWidnow(1200, 720, "Crystal Quest");

  while (running)
  {
    // update
    PlatformUpdateWindow();
  }
  return 0;
}
