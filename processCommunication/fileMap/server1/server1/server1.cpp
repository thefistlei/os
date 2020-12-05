// server1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#pragma region Includes  
#include <stdio.h>  
#include <windows.h>  
#pragma endregion  
#define MAP_PREFIX          L"Local\\"  
#define MAP_NAME            L"SampleMap"  
#define FULL_MAP_NAME       MAP_PREFIX MAP_NAME  

// Max size of the file mapping object.  
#define MAP_SIZE            65536  

// File offset where the view is to begin.  
#define VIEW_OFFSET         0  

// The number of bytes of a file mapping to map to the view. All bytes of the   
// view must be within the maximum size of the file mapping object (MAP_SIZE).   
// If VIEW_SIZE is 0, the mapping extends from the offset (VIEW_OFFSET) to    
// the end of the file mapping.  
#define VIEW_SIZE           1024  

// Unicode string message to be written to the mapped view. Its size in byte   
// must be less than the view size (VIEW_SIZE).  
#define MESSAGE             L"hello, i am server."  


int main(int argc, wchar_t* argv[])
{
    // Prepare a message to be written to the view.  
    PWSTR pszMessage = (PWSTR)MESSAGE;
    DWORD cbMessage = (wcslen(pszMessage) + 1) * sizeof(*pszMessage);

    HANDLE hMapFile = NULL;
    PVOID pView = NULL;

    // Create the file mapping object.  
    hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,   // Use paging file - shared memory  
        NULL,                   // Default security attributes  
        PAGE_READWRITE,         // Allow read and write access  
        0,                      // High-order DWORD of file mapping max size  
        MAP_SIZE,               // Low-order DWORD of file mapping max size  
        FULL_MAP_NAME           // Name of the file mapping object  
    );
    if (hMapFile == NULL)
    {
        wprintf(L"CreateFileMapping failed w/err 0x%08lx\n", GetLastError());
        goto Cleanup;
    }
    wprintf(L"The file mapping (%s) is created\n", FULL_MAP_NAME);

    // Map a view of the file mapping into the address space of the current   
    // process.  
    pView = MapViewOfFile(
        hMapFile,               // Handle of the map object  
        FILE_MAP_ALL_ACCESS,    // Read and write access  
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


    // Write the message to the view.  
    memcpy_s(pView, VIEW_SIZE, pszMessage, cbMessage);

    wprintf(L"This message is written to the view:\n\"%s\"\n",
        pszMessage);

    // Wait to clean up resources and stop the process.  
    wprintf(L"Press ENTER to clean up resources and quit");
    getchar(); 

Cleanup:
    {
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



    return 0;
}