// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Utils.h"
#include "TinySDK.h"
#include <iostream>

//Minhook dependencies
#include "MinHook.h"
#pragma comment(lib, "libMinHook.x64.lib")

//ProcessEvent Prototype
typedef void(__stdcall* fnProcessEvent)(void* Object, tinySDK::UFunction* Function, void* Params);
fnProcessEvent process_event;
fnProcessEvent process_event_original;

void ProcessEventHook(void* Object, tinySDK::UFunction* Function, void* Params)
{
    //Do anything you want
    //std::cout << Function->GetFullName() << std::endl;

    return process_event(Object, Function, Params);
}

void __stdcall GetOffsets() {
    tinySDK::BaseAddress = (DWORD_PTR)GetModuleHandle(nullptr);
    GetModuleInformation(GetCurrentProcess(), (HMODULE)tinySDK::BaseAddress, &tinySDK::info, sizeof(tinySDK::info));

    std::cout << "ModuleBase: " << std::hex << tinySDK::BaseAddress << std::endl;
    
    //Basic Offsets
    tinySDK::UObject::GObjects = reinterpret_cast<tinySDK::TUObjectArray*>(tinySDK::BaseAddress + 0x90D9100);
    tinySDK::FName::GNames = reinterpret_cast<tinySDK::FNamePool*>(tinySDK::BaseAddress + 0x909CD00);
    tinySDK::m_UWorld = reinterpret_cast<tinySDK::UWorld**>(tinySDK::BaseAddress + 0x928db88);

    // ProcessEvent
    auto localPlayer = (*tinySDK::m_UWorld)->PersistentLevel->OwningWorld->OwningGameInstance->LocalPlayers.Data[0];
    void** vmtLocalPlayer = *reinterpret_cast<void***>(localPlayer);
    process_event = reinterpret_cast<fnProcessEvent>(vmtLocalPlayer[68]);
    process_event_original = process_event;
}

//main hack thread created on the DllMain
void HackThread(HMODULE hModule) {
    //getting the offsets
    //We create a console instance, we don't need this, but it's useful for displaying things
    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    GetOffsets();

    // Initialize MinHook.
    if (MH_Initialize() != MH_OK)
    {
        std::cout << "Failed to initialize Minhook!" << std::endl;
    }

    //Hooking ProcessEvent
    if (MH_CreateHook(process_event, ProcessEventHook, (LPVOID*)&process_event) != MH_OK)
    {
        std::cout << "C Hook failed: 1" << std::endl;
        //return;
    }

    // Enable the hook for Process Event
    if (MH_EnableHook(process_event_original) != MH_OK)
    {
        std::cout << "Hook failed: 1" << std::endl;
    }

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

        if(GetAsyncKeyState(VK_MBUTTON) & 1){
            //Just an example, you have to add your own functions to the SDK
            localChar->Jump();
        }

    }

    // Disable the hook
    if (MH_DisableHook(process_event_original) != MH_OK)
    {
        std::cout << "ERROR: Disabling hook" << std::endl;
    }


    Sleep(100);
    if (MH_Uninitialize() != MH_OK);
    Sleep(100);
    std::cout << "Exiting..." << std::endl;
    FreeConsole();
    Sleep(100);
    FreeLibraryAndExitThread(hModule, 0);
    Sleep(100);

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

