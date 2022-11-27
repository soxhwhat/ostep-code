#include <Windows.h>
#include <tchar.h>
#include <cstdio>
#include <strsafe.h>
#include <iostream>


void ErrorExit(PTSTR lpszFunction);

void CreateSubProcessAndRedirectIO(TCHAR* cmdVal, HANDLE *hChildStd_IN_Rd, HANDLE *hChildStd_IN_Wr);

int _tmain(int argc, TCHAR** argv)
{
    // argv check
    if (argc <= 1)
    {
        std::cerr << "Please enter sub process.\n";
        ExitProcess(1);
    }



    // Create subprocess and redirect its stdin
    HANDLE hChildStd_IN_Rd, hChildStd_IN_Wr;
    CreateSubProcessAndRedirectIO(argv[1], &hChildStd_IN_Rd, &hChildStd_IN_Wr);

    // IO interaction
    DWORD dwWritten;
    auto write = [&](char cmd){

        bool bSuccess = WriteFile(hChildStd_IN_Wr, &cmd, 1, &dwWritten, NULL);
        if (!bSuccess)
        {
            ErrorExit(TEXT((PTSTR)"Write File error!"));
        }
    };

    char ch;
    while (std::cin.get(ch))
    {
        write(ch);
    }

    ExitProcess(0);
}

void CreateSubProcessAndRedirectIO(TCHAR* cmdVal, PHANDLE hChildStd_IN_Rd, PHANDLE hChildStd_IN_Wr){


    SECURITY_ATTRIBUTES saAttr;

    // Set the bInheritHandle flag so pipe handles are inherited.
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    // Create a pipe for the child process's STDIN.
    if (!CreatePipe(hChildStd_IN_Rd, hChildStd_IN_Wr, &saAttr, 0))
    {
        ErrorExit(TEXT((PTSTR)"Create pipe error!"));
    }

    // Ensure the write handle to the pipe for STDIN is not inherited.
    if (!SetHandleInformation(*hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))
        ErrorExit(TEXT((PTSTR)"Stdin SetHandleInformation"));

    // Create a child process that uses the previously created pipes for STDIN and STDOUT.
    TCHAR* szCmdline = cmdVal;
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;
    BOOL bSuccess = FALSE;

    // Set up members of the PROCESS_INFORMATION structure.
    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

    // Set up members of the STARTUPINFO structure.
    // This structure specifies the STDIN and STDOUT handles for redirection.
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    siStartInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    siStartInfo.hStdInput = *hChildStd_IN_Rd;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    // Create the child process.
    bSuccess = CreateProcess(NULL,
                             szCmdline,                            // command line
                             NULL,                                 // process security attributes
                             NULL,                                 // primary thread security attributes
                             TRUE,                                 // handles are inherited
                             CREATE_NEW_CONSOLE,                   // creation flags
                             NULL,                                 // use parent's environment
                             NULL,                                 // use parent's current directory
                             &siStartInfo,                         // STARTUPINFO pointer
                             &piProcInfo);                         // receives PROCESS_INFORMATION

    if (!bSuccess)
    {
        ErrorExit(TEXT((PTSTR)"Create Process error!"));
    }

    // Close handles to the child process and its primary thread.
    // Some applications might keep these handles to monitor the status
    // of the child process, for example.
    //
    // Only close handles, the process is also running.
    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);

    // Close handles to the stdin pipes no longer needed by the child process.
    // If they are not explicitly closed, there is no way to recognize that the child process has ended.
    CloseHandle(*hChildStd_IN_Rd);
}


void ErrorExit(PTSTR lpszFunction)
// Format a readable error message, display a message box,
// and exit from the application.
{
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dw,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf,
            0, NULL);

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
            (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
            LocalSize(lpDisplayBuf) / sizeof(TCHAR),
            TEXT("%s failed with error %d: %s"),
            lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(1);
}