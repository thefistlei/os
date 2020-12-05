// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h" 
#include "stdlib.h"
#include "windows.h"

#include <fstream>  

#pragma data_seg("MySec")
HANDLE hMapFile = NULL;

HWND hWnd;
char pszShareText[256];
char* psz;
int nDate = 0;
#pragma data_seg()
#pragma comment(linker,"/section:MySec,rws")  

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

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

_declspec(dllexport) void init()
{ 
    // Create the file mapping object.
    hMapFile = CreateFileMapping(//创建一个想共享的文件数据句柄;
        INVALID_HANDLE_VALUE,   // 表示在页面文件中创建一个可共享的文件映射;
        NULL,                   // 表示使用默认安全对象;
        PAGE_READWRITE,         //  以可读、可写方式打开映射;
        0,                      // 文件映射的最大长度的高32位;
        MAP_SIZE,               // 文件映射的最大长度的低32位;
        FULL_MAP_NAME           // 指定文件映射对象的名字;
    );
    if (hMapFile == NULL)
    { 
        return;
    }
     
}
  

//将wchar_t* 转成char*的实现函数如下：

char* w2c(char* pcstr, const wchar_t* pwstr, size_t len)
{
    int nlength = wcslen(pwstr);

    //获取转换后的长度

    int nbytes = WideCharToMultiByte(0, // specify the code page used to perform the conversion
        0,         // no special flags to handle unmapped characters
        pwstr,     // wide character string to convert
        nlength,   // the number of wide characters in that string
        NULL,      // no output buffer given, we just want to know how long it needs to be
        0,
        NULL,      // no replacement character given
        NULL);    // we don't want to know if a character didn't make it through the translation
        // make sure the buffer is big enough for this, making it larger if necessary

    if (nbytes > len)
        nbytes = len;

    // 通过以上得到的结果，转换unicode 字符为ascii 字符

    WideCharToMultiByte(0, // specify the code page used to perform the conversion
        0,         // no special flags to handle unmapped characters
        pwstr,   // wide character string to convert
        nlength,   // the number of wide characters in that string
        pcstr, // put the output ascii characters at the end of the buffer
        nbytes,                           // there is at least this much space there
        NULL,      // no replacement character given
        NULL);
    return pcstr;
}

//将char* 转成wchar_t*的实现函数如下：
//这是把asii字符转换为unicode字符，和上面相同的原理
void c2w(wchar_t* pwstr, size_t len, const char* str) 
{ 
    if (str) 
    { 
        size_t nu = strlen(str); 
        size_t n = (size_t)MultiByteToWideChar(CP_ACP, 0, (const char*)str, (int)nu, NULL, 0);
        if (n >= len)
            n = len - 1;

        MultiByteToWideChar(CP_ACP, 0, (const char*)str, (int)nu, pwstr, (int)n);
        pwstr[n] = 0; 
    } 
}

_declspec(dllexport) void setIntData(int nDate1)
{
    nDate = nDate1;
}

_declspec(dllexport) int getIntData()
{
    return nDate;
}

_declspec(dllexport) void setData(char* pszText)
{ 
    //PVOID pView = NULL;
    //// 将一个文件映射对象映射到当前应用程序的地址空间;
    //pView = MapViewOfFile(//使用MapViewOfFile来获取共享的内存地址;
    //    hMapFile,               // 文件映像对象句柄;
    //    FILE_MAP_ALL_ACCESS,    //  映射对象的文方件数据的访问式;
    //    0,                      // 文件映射起始偏移的高32位;
    //    VIEW_OFFSET,            // 表示文件映射起始偏移的低32位;
    //    VIEW_SIZE               // 映射文件的字节数;
    //);
    //if (pView == NULL)
    //{
    //    return;
    //} 
     
 

    psz = (char*)malloc(strlen(pszText));
    memcpy_s(psz, strlen(pszText), pszText, strlen(pszText));//*********写数据; 

    std::ofstream outfile;
    outfile.open("test.txt"); 
    outfile.write(psz, strlen(psz));
    outfile.close();

    //wchar_t pwstr[100]; 
    //c2w(pwstr, strlen(pszShareText), pszShareText);
    //DWORD cbMessage = (wcslen(pwstr) + 1) * sizeof(*pwstr);
    //memcpy_s(pView, VIEW_SIZE, pwstr, cbMessage);//*********写数据;

    //MessageBox(hWnd, LPCWSTR(pszText), LPCWSTR(pszText), MB_OK);
}

_declspec(dllexport) char* getData()
{
    /*
    HANDLE hMapFile = NULL;
    PVOID pView = NULL;

    hMapFile = OpenFileMapping(//打开一个现成的文件映射对象的函数;
        FILE_MAP_READ,          // Read access
        FALSE,                  // 如这个函数返回的句柄能由当前进程启动的新进程继承，则这个参数为TRUE;
        FULL_MAP_NAME           // 指定要打开的文件映射对象名称;
    );
    if (hMapFile == NULL)
    {
       return NULL;
    }

    //// 将一个文件映射对象映射到当前应用程序的地址空间;
    pView = MapViewOfFile(
        hMapFile,               // 文件映像对象句柄;
        FILE_MAP_READ,          //  映射对象的文件数据的访问式;
        0,                      // 文件映射起始偏移的高32位;
        VIEW_OFFSET,            // 表示文件映射起始偏移的低32位;
        VIEW_SIZE               // 映射文件的字节数;
    );
    if (pView == NULL)
    {
        return NULL;
    } 

    PWSTR pwstr = PWSTR(pView);
    char pcstr[256];
    memset(pcstr, 0, 2 * wcslen(pwstr) + 1); 
    w2c(pcstr, pwstr, 2 * wcslen(pwstr) + 1); 

    */
    //char* psz = (char*)pView;
    //int nL = strlen(psz);
    //char* pcstr = (char*)malloc(sizeof(char) * nL);
    return psz;
} 

_declspec(dllexport) void setWnd(HWND hWnd1)
{
    hWnd = hWnd1;
}
 