// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Utils.h"
#include "TinySDK.h"
#include <iostream>


void __stdcall GetOffsets() {
    tinySDK::BaseAddress = (DWORD_PTR)GetModuleHandle(nullptr);
    GetModuleInformation(GetCurrentProcess(), (HMODULE)tinySDK::BaseAddress, &tinySDK::info, sizeof(tinySDK::info));

    std::cout << "ModuleBase: " << std::hex << tinySDK::BaseAddress << std::endl;
    
    //928db88
    uintptr_t uworldAdressPtr = (tinySDK::BaseAddress + 0x928db88);
    tinySDK::m_UWorld = (tinySDK::UWorld**)uworldAdressPtr;
}

//main hack thread created on the DllMain
void HackThread(HMODULE hModule) {
    //getting the offsets
    //We create a console instance, we don't need this, but it's useful for displaying things
    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    GetOffsets();

    while (!GetAsyncKeyState(VK_END)) {

        auto persistentLevel = (*tinySDK::m_UWorld)->PersistentLevel;

        if (persistentLevel == nullptr)
            continue;

        auto gameInstance = persistentLevel->OwningWorld->OwningGameInstance;

        auto localPlayer = gameInstance->LocalPlayers.Data[0];

        auto localPlayerController = localPlayer->PlayerController;

        if (localPlayerController == nullptr)
            continue;

        auto localChar = localPlayerController->Character;

        if (localChar == nullptr)
            continue;

        auto localMovementComp = localChar->CharacterMovement;

        if (localMovementComp == nullptr)
            continue;

    }
    std::cout << "Exit" << std::endl;
    Sleep(100);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
}

//Used to attach the dll to the process we inject to
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        HANDLE handle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HackThread, hModule, NULL, NULL);
        break;
    }
    return 1;
}

