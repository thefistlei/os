// client1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#pragma region Includes  
#include <stdio.h>  
#include <windows.h>  
#pragma endregion  
#define MAP_PREFIX          L"Local\\"  
#define MAP_NAME            L"SampleMap"  
#define FULL_MAP_NAME       MAP_PREFIX MAP_NAME  

// File offset where the view is to begin.  
#define VIEW_OFFSET         0  

// The number of bytes of a file mapping to map to the view. All bytes of the   
// view must be within the maximum size of the file mapping object. If   
// VIEW_SIZE is 0, the mapping extends from the offset (VIEW_OFFSET) to the   
// end of the file mapping.  
#define VIEW_SIZE           1024  


int wmain(int argc, wchar_t* argv[])
{
    HANDLE hMapFile = NULL;
    PVOID pView = NULL;

    // Try to open the named file mapping identified by the map name.  
    hMapFile = OpenFileMapping(
        FILE_MAP_READ,          // Read access  
        FALSE,                  // Do not inherit the name  
        FULL_MAP_NAME           // File mapping name   
    );
    if (hMapFile == NULL)
    {
        wprintf(L"OpenFileMapping failed w/err 0x%08lx\n", GetLastError());
        goto Cleanup;
    }
    wprintf(L"The file mapping (%s) is opened\n", FULL_MAP_NAME);

    // Map a view of the file mapping into the address space of the current   
    // process.  
    pView = MapViewOfFile(
        hMapFile,               // Handle of the map object  
        FILE_MAP_READ,          // Read access  
        0,                      // High-order DWORD of the file offset   
        VIEW_OFFSET,            // Low-order DWORD of the file offset  
        VIEW_SIZE               // The number of bytes to map to view  
    );
    if (pView == NULL)
    {
        wprintf(L"MapViewOfFile failed w/err 0x%08lx\n", GetLastError());
        goto Cleanup;
    }
    wprintf(L"The file view is mapped\n");

    // Read and display the content in view.  
    wprintf(L"Read from the file mapping:\n\"%s\"\n", (PWSTR)pView);

    // Wait to clean up resources and stop the process.  
    wprintf(L"Press ENTER to clean up resources and quit");
    getchar();

Cleanup:

    if (hMapFile)
    {
        if (pView)
        {
            // Unmap the file view.  
            UnmapViewOfFile(pView);
            pView = NULL;
        }
        // Close the file mapping object.  
        CloseHandle(hMapFile);
        hMapFile = NULL;
    }

    return 0;
}