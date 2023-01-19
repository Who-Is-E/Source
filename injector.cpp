#include <string>
#include <thread>
#include <iostream>
#include <windows.h>
#include <libloaderapi.h>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <filesystem>

// NOTE: I'm new to c++ game hacking so it might be ass and inefficent feel free to give suggestions
// This is only here for testing purposes 


void error(const char* errorTitle, const char* errorMessage) {
    MessageBox(0, errorMessage, errorTitle, 0);
    exit(-1);
   
}

void getProcessId(const char* windowTitle, DWORD& process_id) {
    GetWindowThreadProcessId(FindWindow(NULL, windowTitle), &process_id);
    
}

bool doesFileExist(std::string fileName) {
    struct stat buffer;
    return (stat(fileName.c_str(), &buffer) == 0);
}
/*
void openKrnl() {
    // Under the works 
}
*/
int main() {
    DWORD procID = NULL;
    char dllPath[MAX_PATH];

    const char* krnldll = "krnl.dll";
    const char* windowTitle = "Roblox";
    const char* alternativeDll = "WhoIsE.dll";

    if (doesFileExist(krnldll) || doesFileExist(alternativeDll)) {
        printf("[INFO] - Searching for dll (1) \n");
        Sleep(200);
        printf("[INFO] - Attempting to modify dll file (2) \n");
        std::rename(krnldll, alternativeDll);
        Sleep(1000);
    }

    else {
        system("cls"); system("Color 04"); printf("[ERROR] - Krnl.dll is missing. Your anti virus most likely deleted it! (OR) Error re-naming dll file, check permissions! \n");
        error("Error 101", "Krnl.dll is missing. Your anti virus most likely deleted it!");
        
    }
    if (!GetFullPathName(krnldll, MAX_PATH, dllPath, nullptr) || !GetFullPathName(alternativeDll, MAX_PATH, dllPath, nullptr)) {
        system("cls"); system("Color 04"); printf("[ERROR] - Unable to get directory of dll - try reinstalling! \n");
        error("Error 102", "Unable to get directory of dll! Re-install.");
        
    }

    else {
        printf("[INFO] - Injecting (1/4) \n");
        Sleep(200);
        getProcessId(windowTitle, procID);
    }
    if (procID == NULL) {
        
        system("cls"); system("Color 04"); printf("[ERROR] - Roblox is not open! \n");
        error("Error 103", "Unable to find roblox open!");
    }
    HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, NULL, procID);
    printf("[INFO] - Injecting (2/4) \n");

    if (!h_process) {
        
        system("cls"); system("Color 04"); printf("[ERROR] - Roblox may not be open \n");
        error("Error 104", "Process handle failure! Ensure you're running it properly!");
    }

    void* allocatedMemory = VirtualAllocEx(h_process, nullptr, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    printf("[INFO] - Injecting (3/4) \n");
    if (!allocatedMemory) {
        system("cls"); system("Color 04"); printf("[ERROR] - Memory allocation failure - Try running as administrator? \n");
        error("Error 105", "Memory allocation failed!");
    }

    if (!WriteProcessMemory(h_process, allocatedMemory, dllPath, MAX_PATH, nullptr)) {
        
        system("cls"); system("Color 04"); printf("[ERROR] - Function failure (Write Process Memory) \n");
        error("Error 106", "WriteProcessMemory Function failure");
    }

    HANDLE h_thread = CreateRemoteThread(h_process, nullptr, NULL, LPTHREAD_START_ROUTINE(LoadLibraryA), allocatedMemory, NULL, nullptr);
    printf("[INFO] - Injecting (4/4) \n");
    if (!h_thread) {
       
        system("cls"); system("Color 04"); printf("[ERROR] - Create Remote Thread Failure \n");
        error("Error 107", "CRT Function failure");
    }
    CloseHandle(h_process);
    VirtualFreeEx(h_process, allocatedMemory, NULL, MEM_RELEASE);
    printf("[INFO] - Cleaning Memory \n");

    system("cls");
   
    MessageBox(0, "Injected Sucessfully - Simply execute your desired script - You Do **NOT** have to click 'inject' on the krnl ui, simply execute it.", "READ!", 0);
    Sleep(500);
  //  openKrnl();
}
