#ifndef __XBDM_H__
#define __XBDM_H__

#define XBDMAPI

#include <iostream>
#include <sstream>
#include <vector>
#include <functional>
#include <algorithm>
#include <cctype>
#include <memory>
#include <fstream>
#include "XbdmDefinitions.h"

using std::string;

// because fuck you microsoft
#define _WIN32_WINNT 0x0501

#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
    #include <Ws2tcpip.h>
#else
    // worry about this later, maybe use winnames.h from velocity
    #define ZeroMemory(buff, len) memset(buff, 0, len)
#endif

namespace XBDM
{
    class DevConsole
    {
    public:
        DevConsole(std::string consoleIp);

        // connection functions
        bool OpenConnection();
        bool CloseConnection();
        void ResetConnection();

        bool SendBinary(const BYTE *buffer, DWORD length);
        bool RecieveBinary(BYTE *buffer, DWORD length, bool text = true);
        bool SendCommand(string command, string &response, DWORD responseLength = 0x400, DWORD statusLength = -1);
        bool SendCommand(string command, string &response, ResponseStatus &status, DWORD responseLength = 0x400, DWORD statusLength = -1);
        bool RecieveResponse(string &response, ResponseStatus &status, DWORD responseLength = 0x400, DWORD statusLength = -1);

        // getters
        bool                        IsHddEnabled(bool &ok, bool forceResend = false);
        DWORD                       GetDebugMemorySize(bool &ok, bool forceResend = false);
        std::string                 GetType(bool &ok, bool forceResend = false);
        std::string                 GetFeatures(bool &ok, bool forceResend = false);
        std::string                 GetDebugName(bool &ok, bool forceResend = false);
        std::string                 GetPlatform(bool &ok, bool forceResend = false);
        std::string                 GetMotherboard(bool &ok, bool forceResend = false);
        std::string                 GetBaseKernelVersion(bool &ok, bool forceResend = false);
        std::string                 GetKernelVersion(bool &ok, bool forceResend = false);
        std::string                 GetRecoveryVersion(bool &ok, bool forceResend = false);
        std::string                 GetActiveTitle(bool &ok, bool forceResend = false);
        DebugMemStatus              GetDebugMemoryStatus(bool &ok, bool forceResend = false);
        DumpMode                    GetDumpMode(bool &ok, bool forceResend = false);
        DumpSettings                GetDumpSettings(bool &ok, bool forceResend = false);
        SystemInformation           GetSystemInformation(bool &ok, bool forceResend = false);
        std::vector<Drive>          GetDrives(bool &ok, bool forceResend = false);
        std::vector<FileEntry>      GetDirectoryContents(std::string directory, bool &ok);
        std::vector<Module>         GetLoadedModules(bool &ok, bool forceResend = false);
        std::vector<Thread>         GetThreads(bool &ok, bool forceResend = false);
        std::vector<MemoryRegion>   GetMemoryRegions(bool &ok, bool forceResend = false);
        std::unique_ptr<BYTE[]>     GetScreenshot(bool &ok);
        std::unique_ptr<BYTE[]>     GetMemory(DWORD address, DWORD length, bool &ok);
        void                        GetFile(std::string remotePath, std::string localPath, bool &ok);

        void                        DumpMemory(DWORD address, DWORD length, std::string dumpPath);

        // rebooting functions
        void                        RebootToXShell();
        void                        RebootToCurrentTitle();
        void                        ColdReboot();
        void                        LaunchXEX(std::string xexPath);

        // automated controller input functions
        void                        StartAutomatingInput(DWORD userIndex, bool &ok, DWORD queueLen = 0);
        void                        StopAutomatingInput(DWORD userIndex, bool &ok);
        void                        AddGamepadToQueue(DWORD userIndex, GamepadState gamepad, bool &ok);
        void                        SendGamepads(DWORD userIndex, std::vector<GamepadState> &gamepads, bool &ok);
        void                        ClearGamepadQueue(DWORD userIndex, bool &ok);

        // file operations
        void                        RenameFile(std::string oldName, std::string newName, bool &ok);
        void                        MoveFile(std::string oldName, std::string newName, bool &ok);


    private:
        SOCKET          xsocket;
        bool            connected;
        std::string     ip;

        // cached properties
        DWORD                       debugMemSize;
        std::string                 type;
        std::string                 features;
        std::string                 debugName;
        std::string                 activeTitle;
        DebugMemStatus              debugMemStatus;
        DumpMode                    dumpMode;
        DumpSettings                dumpSettings;
        SystemInformation           systemInformation;
        std::vector<Drive>          drives;
        std::vector<Module>         loadedModules;
        std::vector<Thread>         threads;
        std::vector<MemoryRegion>   memoryRegions;

        // i would use std::regex in these 3 functions, but it's not fully implemented in mingw yet
        DWORD           GetIntegerProperty(std::string &response, std::string propertyName, bool &ok, bool hex = false, bool update = false);
        std::string     GetStringProperty(std::string &response, std::string propertyName, bool &ok, bool update = false);
        std::string     GetEnumProperty(std::string &response, std::string propertyName, bool &ok);

        std::string     MemoryRegionFlagsToString(DWORD flags);
    };
};

#endif
