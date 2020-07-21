/*

Method:

Spawner.exe calls SetDLLDirectory("location of our d3d9.dll")
Spawner.exe calls CreateProcess("child.exe")

Child.exe calls LoadLibrary("d3d9.dll")
Child.exe calls GetProcAddress("Direct3DCreate9")
Child.exe calls Direct3DCreate9(...)
Child.exe calls CreateProcess("child.exe") a few times (to test that SetDLLDirectory sticks)

d3d9.dll has one function in it called Direct3DCreate9 which pops a MessageBox

Test 1 - don't build the DLL and run it
Expect to see: system d3d9.dll is loaded (result of call to Direct3DCreate9 is a valid LPDIRECT3D9)

Test 2 - build the DLL and run it
Expect to see: our d3d9.dll is loaded and MessageBoxes pop up

Note that our d3d9.dll is in an arbitrary folder, not the same folder as the exe or the working folder

*/

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>

#if defined(_DEBUG)
#define DLL_PATH "..\\DLL\\Debug"
#else
#define DLL_PATH "..\\DLL\\Release"
#endif

int main(int, char **)
{
    // spoof DLL path - will it stick for child and grandchild processes?

    if(!SetDllDirectoryA(DLL_PATH)) {
        printf("Error: SetDllDirectoryA failed: %08x\n", GetLastError());
        return 1;
    }

    // child spawns N descendents

    char cmd_line[MAX_PATH];
    strcpy(cmd_line, "child.exe 3");

    // spawn 1st child

    STARTUPINFOA si = { 0 };
    PROCESS_INFORMATION pi = { 0 };

    si.cb = sizeof(si);

    if(!CreateProcessA(NULL, cmd_line, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        printf("Error: Can't spawn child %08x\n", GetLastError());
    } else {
        printf("Spawned child\n");
        WaitForSingleObject(pi.hProcess, INFINITE);
    }
    printf("Spawner quits\n");
    return 0;
}
