#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>

#define D3D_SDK_VERSION 32

struct IDirect3D9;
typedef struct IDirect3D9 *LPDIRECT3D9, *PDIRECT3D9;

typedef LPDIRECT3D9(APIENTRY *Direct3dCreate9_fn)(UINT);

int main(int argc, char **argv)
{
    printf("Child %s\n", argv[1]);

    // load d3d9.dll from... somewhere...

    HMODULE d3d9_dll = LoadLibraryA("d3d9.dll");
    if(d3d9_dll == nullptr) {
        printf("Can't load d3d9 %08x\n", GetLastError());
    }

    // get fn pointer for Direct3DCreate9

    Direct3dCreate9_fn d3d9_create = reinterpret_cast<Direct3dCreate9_fn>(GetProcAddress(d3d9_dll, "Direct3DCreate9"));

    if(d3d9_create == nullptr) {
        printf("Can't find function %08x\n", GetLastError());
    }

    // call it - if it loaded our DLL, we get a MessageBox here, otherwise not

    LPDIRECT3D9 p = d3d9_create(D3D_SDK_VERSION);
    printf("d3d9_create returned %p\n", p);

    // spawn another child if cmd-line says so

    char c = argv[1][0];
    if(c > '1' && c <= '9') {

        char cmd_line[MAX_PATH];
        strcpy(cmd_line, "child.exe 0");
        cmd_line[strlen(cmd_line) - 1] = c - 1;

        STARTUPINFOA si = { 0 };
        PROCESS_INFORMATION pi = { 0 };

        si.cb = sizeof(si);

        if(!CreateProcessA(NULL, cmd_line, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
            printf("Error: Can't spawn child %08x\n", GetLastError());
        } else {
            printf("Child spawned descendent\n");
            WaitForSingleObject(pi.hProcess, INFINITE);
        }
    }
    printf("Child %s quits\n", argv[1]);
    return 0;
}
