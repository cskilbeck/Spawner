#include <windows.h>

// silly buggers because __stdcall forces name mangling
#define EXPORT comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)

struct IDirect3D9;
typedef struct IDirect3D9 *LPDIRECT3D9, *PDIRECT3D9;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    return TRUE;
}

LPDIRECT3D9 APIENTRY Direct3DCreate9(UINT SDKVersion)
{
    // dll export without mangled name
    #pragma EXPORT

    MessageBoxA(NULL, "Direct3DCreate9", "MsgBox", MB_ICONEXCLAMATION);
    return (LPDIRECT3D9)(0xDEADBEEF);
}
