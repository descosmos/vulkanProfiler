#include <windows.h>
#include <cstdlib>
#include <iostream>

const wchar_t* VULKAN_HOOK_DLL_PATH = L"D:\\projects\\c++\\Vulkan-Hook\\x64\\Debug\\VulkanHook.dll";
const char* REMOTE_INJECT_DLL_PATH = "D:\\projects\\c++\\perfmon3\\x64\\Release\\RemoteInject.dll";
const wchar_t* REMOTE_INJECT_D3D11_PATH = L"D:\\projects\\c++\\perfmon3\\x64\\Release\\d3d11hook.dll";

HMODULE RemoteInjectHandle = NULL;
typedef int (*RemoteInjectFuncType)(DWORD dwPid, LPCWSTR szDll);
RemoteInjectFuncType remoteInject = NULL;

int load_remoteInject_dll(const char* dllPath) {
    RemoteInjectHandle = LoadLibraryA(dllPath);
    if (RemoteInjectHandle == NULL) {
        printf("Load RemoteInject.dll FAILED.\n");
        return -1;
    }

    FARPROC funcAddress = GetProcAddress(RemoteInjectHandle, "remoteInject");
    if (funcAddress == NULL) {
        printf("GetProcAddress remoteInject FAILED.\n");
        return -1;
    }

    remoteInject = reinterpret_cast<RemoteInjectFuncType>(funcAddress);

    return 0;
}

int inject_dll_to_target_process(int pid) {
    int res = remoteInject(pid, VULKAN_HOOK_DLL_PATH);
    printf("remoteInject res: %d\n", res);

    if (res != 1) {
        printf("remoteInject FAILED.\n");
        return -1;
    }

    return 0;
}

int main() {
    // init
    int pid = 0;
    std::cout << "Enter target pid: " << std::endl;
    std::cin >> pid;
    std::cout << "Target process: " << pid << std::endl;

    if (load_remoteInject_dll(REMOTE_INJECT_DLL_PATH) == -1) {
        exit(-1);
    }

    if (inject_dll_to_target_process(pid) == -1) {
        exit(-1);
    }

    return 0;
}

