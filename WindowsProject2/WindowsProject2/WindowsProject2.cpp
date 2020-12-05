// WindowsProject2.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "WindowsProject2.h"

#define MAX_LOADSTRING 100

#include <fstream>  

HWND g_hWnd;
void testSetFunc();
void testGetFunc();

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT2));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//

HANDLE hMapFile = NULL;
PVOID pView = NULL;
HWND hWnd;
char pszShareText[256];

// Max size of the file mapping object.
#define MAP_SIZE            65536

// File offset where the view is to begin.
#define VIEW_OFFSET         0

#define MAP_PREFIX          L"Local\\"
#define MAP_NAME            L"SampleMap"
#define FULL_MAP_NAME       MAP_PREFIX MAP_NAME

// The number of bytes of a file mapping to map to the view. All bytes of the 
// view must be within the maximum size of the file mapping object (MAP_SIZE). 
// If VIEW_SIZE is 0, the mapping extends from the offset (VIEW_OFFSET) to  
// the end of the file mapping.
#define VIEW_SIZE           1024


void testSetFunc()
{
    HINSTANCE hInst;
    hInst = LoadLibrary("Dll1.dll");

    typedef void (*setIntData)(int nDate1);
    setIntData setIntDataFunc = (setIntData)GetProcAddress(hInst, MAKEINTRESOURCE(5));
    if (!setIntDataFunc)
    {
        return;
    }
    setIntDataFunc(166);
}

void testGetFunc()
{
    HINSTANCE hInst;
    hInst = LoadLibrary("Dll1.dll");

    typedef int (*getIntData)();
    getIntData getIntDataFunc = (getIntData)GetProcAddress(hInst, MAKEINTRESOURCE(2));
    if (!getIntDataFunc)
    {
        return;
    }
    int n = getIntDataFunc();
    char ch[256] = "\0";
    sprintf_s(ch, "%d", n);
    MessageBox(g_hWnd, ch, "f", MB_OK);
}

void testSetFunc_2()
{
    HINSTANCE hInst;
    hInst = LoadLibrary("Dll1.dll");

    typedef void (*setData)(char*);
    setData setIntDataFunc = (setData)GetProcAddress(hInst, MAKEINTRESOURCE(4));
    if (!setIntDataFunc)
    {
        return;
    }
    char ch[256] = "hello\0";
    setIntDataFunc(ch);
}

void testGetFunc_2()
{
    HINSTANCE hInst;
    hInst = LoadLibrary("Dll1.dll");

    typedef char* (*getIntData)();
    getIntData getIntDataFunc = (getIntData)GetProcAddress(hInst, MAKEINTRESOURCE(1));
    if (!getIntDataFunc)
    {
        return;
    }
    char* psz = getIntDataFunc();
    char ch[256] = "\0";
    sprintf_s(ch, "%s", psz);
    MessageBox(g_hWnd, ch, "f", MB_OK);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   g_hWnd = hWnd;

   //HANDLE hMapFile = NULL;
   //PVOID pView = NULL;

   //hMapFile = OpenFileMapping(//打开一个现成的文件映射对象的函数;
   //    FILE_MAP_READ,          // Read access
   //    FALSE,                  // 如这个函数返回的句柄能由当前进程启动的新进程继承，则这个参数为TRUE;
   //    FULL_MAP_NAME           // 指定要打开的文件映射对象名称;
   //);
   //if (hMapFile == NULL)
   //{
   //    return NULL;
   //}

   ////// 将一个文件映射对象映射到当前应用程序的地址空间;
   //pView = MapViewOfFile(
   //    hMapFile,               // 文件映像对象句柄;
   //    FILE_MAP_READ,          //  映射对象的文件数据的访问式;
   //    0,                      // 文件映射起始偏移的高32位;
   //    VIEW_OFFSET,            // 表示文件映射起始偏移的低32位;
   //    VIEW_SIZE               // 映射文件的字节数;
   //);
   //if (pView == NULL)
   //{
   //    return NULL;
   //}

   //PWSTR pwstr = PWSTR(pView);
   //char pcstr[256];
   //memset(pcstr, 0, 2 * wcslen(pwstr) + 1);
   //w2c(pcstr, pwstr, 2 * wcslen(pwstr) + 1);



  // testGetFunc();
   testGetFunc_2();
 

   //std::ofstream outfile;
   //outfile.open("test1.txt");
   //outfile.write(psz, strlen(psz));
   //outfile.close();

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
            {
                testSetFunc();
            }
               // DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
