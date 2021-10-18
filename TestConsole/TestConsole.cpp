// TestConsole.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#include <iostream>
#include <cstdio>
#include <windows.h>
#include <detours.h>

#pragma comment(lib, "detours.lib")

int main(int argc, char* argv[])
{
    std::cout << "Hello World!\n";
    printf("Test!!!!!!!");

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    si.cb = sizeof(STARTUPINFO);
    WCHAR DirPath[MAX_PATH + 1];
    //wcscpy_s(DirPath, MAX_PATH, L"E:\\Codes\\SoftwareSecurity\\InjectDll\\Release");//dll所在的文件夹(绝对路径)
    //char DLLPath[MAX_PATH + 1] = "E:\\Codes\\SoftwareSecurity\\InjectDll\\Release\\InjectDll.dll";//dll的地址(绝对路径)
    //wcscpy_s(DirPath, MAX_PATH, L"../InjectDll/Release");//dll所在的文件夹(相对路径)(可调试运行的路径选择)
    wcscpy_s(DirPath, MAX_PATH, L"./");//dll所在的文件夹(相对路径)(可直接双击运行Qt.exe时的路径选择)
    char DLLPath[MAX_PATH + 1] = "./InjectDll.dll";//dll的地址(相对路径)
    WCHAR EXE[MAX_PATH + 1] = { 0 };
    //wcscpy_s(EXE, MAX_PATH, L"E:\\Codes\\SoftwareSecurity\\InjectDll\\Debug\\TestApp.exe");
    swprintf(EXE, MAX_PATH + 1, L"%hs", argv[1]);
    if (DetourCreateProcessWithDllEx(EXE, NULL, NULL, NULL, TRUE,
        CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED, NULL, DirPath, &si, &pi,
        DLLPath, NULL))
    {
        ResumeThread(pi.hThread);
        WaitForSingleObject(pi.hProcess, INFINITE);
    }
    else
    {
        char error[100];
        sprintf_s(error, "%d", GetLastError());
    }
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
