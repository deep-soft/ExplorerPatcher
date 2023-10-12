#include "symbols.h"

const char* twinui_pcshell_SN[TWINUI_PCSHELL_SB_CNT] = {
    TWINUI_PCSHELL_SB_0,
    TWINUI_PCSHELL_SB_1,
    TWINUI_PCSHELL_SB_2,
    TWINUI_PCSHELL_SB_3,
    TWINUI_PCSHELL_SB_4,
    TWINUI_PCSHELL_SB_5,
    TWINUI_PCSHELL_SB_6,
    TWINUI_PCSHELL_SB_7,
    TWINUI_PCSHELL_SB_8
};
const char* startdocked_SN[STARTDOCKED_SB_CNT] = {
    STARTDOCKED_SB_0,
    STARTDOCKED_SB_1,
    STARTDOCKED_SB_2,
    STARTDOCKED_SB_3,
    STARTDOCKED_SB_4
};
const char* startui_SN[STARTUI_SB_CNT] = {
    STARTUI_SB_0
};

const wchar_t DownloadSymbolsXML[] =
L"<toast scenario=\"reminder\" "
L"activationType=\"protocol\" launch=\"https://github.com/valinet/ExplorerPatcher\" duration=\"short\">\r\n"
L"	<visual>\r\n"
L"		<binding template=\"ToastGeneric\">\r\n"
L"			<text><![CDATA[Unable to find symbols for OS build %s]]></text>\r\n"
L"			<text><![CDATA[Downloading and applying symbol information, please wait...]]></text>\r\n"
L"			<text placement=\"attribution\"><![CDATA[ExplorerPatcher]]></text>\r\n"
L"		</binding>\r\n"
L"	</visual>\r\n"
L"	<audio src=\"ms-winsoundevent:Notification.Default\" loop=\"false\" silent=\"false\"/>\r\n"
L"</toast>\r\n";

const wchar_t DownloadOKXML[] =
L"<toast scenario=\"reminder\" "
L"activationType=\"protocol\" launch=\"https://github.com/valinet/ExplorerPatcher\" duration=\"long\">\r\n"
L"	<visual>\r\n"
L"		<binding template=\"ToastGeneric\">\r\n"
L"			<text><![CDATA[Successfully downloaded symbols for OS build %s]]></text>\r\n"
L"			<text><![CDATA[Please restart File Explorer to apply the changes and enable additional functionality.]]></text>\r\n"
L"			<text placement=\"attribution\"><![CDATA[ExplorerPatcher]]></text>\r\n"
L"		</binding>\r\n"
L"	</visual>\r\n"
L"	<audio src=\"ms-winsoundevent:Notification.Default\" loop=\"false\" silent=\"false\"/>\r\n"
L"</toast>\r\n";

const wchar_t InstallOK[] =
L"<toast scenario=\"reminder\" "
L"activationType=\"protocol\" launch=\"https://github.com/valinet/ExplorerPatcher\" duration=\"long\">\r\n"
L"	<visual>\r\n"
L"		<binding template=\"ToastGeneric\">\r\n"
L"			<text><![CDATA[Installation succeeded!]]></text>\r\n"
L"			<text><![CDATA[This notification will not show again until the next OS build update.]]></text>\r\n"
L"			<text placement=\"attribution\"><![CDATA[ExplorerPatcher]]></text>\r\n"
L"		</binding>\r\n"
L"	</visual>\r\n"
L"	<audio src=\"ms-winsoundevent:Notification.Default\" loop=\"false\" silent=\"false\"/>\r\n"
L"</toast>\r\n";

extern INT VnDownloadSymbols(HMODULE hModule, char* dllName, char* szLibPath, UINT sizeLibPath);
extern INT VnGetSymbols(const char* pdb_file, DWORD* addresses, char** symbols, DWORD numOfSymbols);

static BOOL ProcessTwinuiPcshellSymbols(const char* pszSettingsPath, DWORD* pOffsets)
{
    HKEY hKey = NULL;
    DWORD dwDisposition;
    RegCreateKeyExW(
        HKEY_CURRENT_USER,
        TEXT(REGPATH) L"\\" TEXT(TWINUI_PCSHELL_SB_NAME),
        0,
        NULL,
        REG_OPTION_NON_VOLATILE,
        KEY_WRITE,
        NULL,
        &hKey,
        &dwDisposition
    );
    if (!hKey || hKey == INVALID_HANDLE_VALUE)
    {
        printf("[Symbols] Unable to create registry key.\n");
        return FALSE;
    }

    CHAR szHash[100];
    WCHAR wszPath[MAX_PATH];

    ZeroMemory(szHash, sizeof(szHash));
    ZeroMemory(wszPath, sizeof(wszPath));

    char twinui_pcshell_sb_dll[MAX_PATH];
    ZeroMemory(twinui_pcshell_sb_dll, sizeof(twinui_pcshell_sb_dll));
    GetSystemDirectoryA(twinui_pcshell_sb_dll, MAX_PATH);
    strcat_s(twinui_pcshell_sb_dll, MAX_PATH, "\\");
    strcat_s(twinui_pcshell_sb_dll, MAX_PATH, TWINUI_PCSHELL_SB_NAME);
    strcat_s(twinui_pcshell_sb_dll, MAX_PATH, ".dll");

    GetSystemDirectoryW(wszPath, MAX_PATH);
    wcscat_s(wszPath, MAX_PATH, L"\\" _T(TWINUI_PCSHELL_SB_NAME) L".dll");
    ComputeFileHash(wszPath, szHash, ARRAYSIZE(szHash));

    printf("[Symbols] Downloading symbols for \"%s\" (\"%s\")...\n", twinui_pcshell_sb_dll, szHash);
    if (VnDownloadSymbols(
        NULL,
        twinui_pcshell_sb_dll,
        pszSettingsPath,
        MAX_PATH
    ))
    {
        printf("[Symbols] Symbols for \"%s\" are not available - unable to download.\n", twinui_pcshell_sb_dll);
        printf("[Symbols] Please refer to \"https://github.com/valinet/ExplorerPatcher/wiki/Symbols\" for more information.\n");
        if (hKey) RegCloseKey(hKey);
        return FALSE;
    }

    printf("[Symbols] Reading symbols...\n");
    if (!IsWindows11())
    {
        DWORD flOldProtect = 0;
        if (VirtualProtect(twinui_pcshell_SN, sizeof(twinui_pcshell_SN), PAGE_EXECUTE_READWRITE, &flOldProtect))
        {
            twinui_pcshell_SN[1] = twinui_pcshell_SN[0];
            VirtualProtect(twinui_pcshell_SN, sizeof(twinui_pcshell_SN), flOldProtect, &flOldProtect);
        }
    }
    if (IsWindows11Version22H2OrHigher())
    {
        DWORD flOldProtect = 0;
        if (VirtualProtect(twinui_pcshell_SN, sizeof(twinui_pcshell_SN), PAGE_EXECUTE_READWRITE, &flOldProtect))
        {
            twinui_pcshell_SN[7] = "CMultitaskingViewManager::_CreateXamlMTVHost";
            twinui_pcshell_SN[TWINUI_PCSHELL_SB_CNT - 1] = "CMultitaskingViewManager::_CreateDCompMTVHost";
            VirtualProtect(twinui_pcshell_SN, sizeof(twinui_pcshell_SN), flOldProtect, &flOldProtect);
        }
    }
    if (VnGetSymbols(
        pszSettingsPath,
        pOffsets,
        twinui_pcshell_SN,
        IsWindows11() ? TWINUI_PCSHELL_SB_CNT : 4
    ))
    {
        if (!IsWindows11())
        {
            printf("[Symbols] Failure in reading symbols for \"%s\".\n", twinui_pcshell_sb_dll);
            if (hKey) RegCloseKey(hKey);
            return FALSE;
        }

        //printf("[Symbols] Hooking Win+C is not available in this build.\n");
        if (VnGetSymbols(
            pszSettingsPath,
            pOffsets,
            twinui_pcshell_SN,
            TWINUI_PCSHELL_SB_CNT - 1
        ))
        {
            printf("[Symbols] Windows 10 window switcher style may not be available in this build.\n");
            if (VnGetSymbols(
                pszSettingsPath,
                pOffsets,
                twinui_pcshell_SN,
                TWINUI_PCSHELL_SB_CNT - 2
            ))
            {
                printf("[Symbols] Failure in reading symbols for \"%s\".\n", twinui_pcshell_sb_dll);
                if (hKey) RegCloseKey(hKey);
                return FALSE;
            }
        }
    }

    if (!IsWindows11())
    {
        pOffsets[1] = 0;
    }
    RegSetValueExW(hKey, TEXT(TWINUI_PCSHELL_SB_0), 0, REG_DWORD, &pOffsets[0], sizeof(DWORD));
    RegSetValueExW(hKey, TEXT(TWINUI_PCSHELL_SB_1), 0, REG_DWORD, &pOffsets[1], sizeof(DWORD));
    RegSetValueExW(hKey, TEXT(TWINUI_PCSHELL_SB_2), 0, REG_DWORD, &pOffsets[2], sizeof(DWORD));
    RegSetValueExW(hKey, TEXT(TWINUI_PCSHELL_SB_3), 0, REG_DWORD, &pOffsets[3], sizeof(DWORD));
    RegSetValueExW(hKey, TEXT(TWINUI_PCSHELL_SB_4), 0, REG_DWORD, &pOffsets[4], sizeof(DWORD));
    RegSetValueExW(hKey, TEXT(TWINUI_PCSHELL_SB_5), 0, REG_DWORD, &pOffsets[5], sizeof(DWORD));
    RegSetValueExW(hKey, TEXT(TWINUI_PCSHELL_SB_6), 0, REG_DWORD, &pOffsets[6], sizeof(DWORD));
    RegSetValueExW(hKey, TEXT(TWINUI_PCSHELL_SB_7), 0, REG_DWORD, &pOffsets[7], sizeof(DWORD));
    RegSetValueExW(hKey, TEXT(TWINUI_PCSHELL_SB_8), 0, REG_DWORD, &pOffsets[8], sizeof(DWORD));

    RegSetValueExA(hKey, "Hash", 0, REG_SZ, szHash, strlen(szHash) + 1);

    if (hKey) RegCloseKey(hKey);
    return TRUE;
}

static BOOL ProcessStartDockedSymbols(const char* pszSettingsPath, DWORD* pOffsets)
{
    HKEY hKey = NULL;
    DWORD dwDisposition;
    RegCreateKeyExW(
        HKEY_CURRENT_USER,
        TEXT(REGPATH_STARTMENU) L"\\" TEXT(STARTDOCKED_SB_NAME),
        0,
        NULL,
        REG_OPTION_NON_VOLATILE,
        KEY_WRITE,
        NULL,
        &hKey,
        &dwDisposition
    );
    if (!hKey || hKey == INVALID_HANDLE_VALUE)
    {
        printf("[Symbols] Unable to create registry key.\n");
        return FALSE;
    }

    CHAR szHash[100];
    WCHAR wszPath[MAX_PATH];

    ZeroMemory(szHash, sizeof(szHash));
    ZeroMemory(wszPath, sizeof(wszPath));

    char startdocked_sb_dll[MAX_PATH];
    ZeroMemory(startdocked_sb_dll, sizeof(startdocked_sb_dll));
    GetWindowsDirectoryA(startdocked_sb_dll, MAX_PATH);
    strcat_s(startdocked_sb_dll, MAX_PATH, "\\SystemApps\\Microsoft.Windows.StartMenuExperienceHost_cw5n1h2txyewy\\");
    strcat_s(startdocked_sb_dll, MAX_PATH, STARTDOCKED_SB_NAME);
    strcat_s(startdocked_sb_dll, MAX_PATH, ".dll");

    GetWindowsDirectoryW(wszPath, MAX_PATH);
    wcscat_s(wszPath, MAX_PATH, L"\\SystemApps\\Microsoft.Windows.StartMenuExperienceHost_cw5n1h2txyewy\\" _T(STARTDOCKED_SB_NAME) L".dll");
    ComputeFileHash(wszPath, szHash, ARRAYSIZE(szHash));

    printf("[Symbols] Downloading symbols for \"%s\" (\"%s\")...\n", startdocked_sb_dll, szHash);
    if (VnDownloadSymbols(
        NULL,
        startdocked_sb_dll,
        pszSettingsPath,
        MAX_PATH
    ))
    {
        printf("[Symbols] Symbols for \"%s\" are not available - unable to download.\n", startdocked_sb_dll);
        printf("[Symbols] Please refer to \"https://github.com/valinet/ExplorerPatcher/wiki/Symbols\" for more information.\n");
        if (hKey) RegCloseKey(hKey);
        return FALSE;
    }

    printf("[Symbols] Reading symbols...\n");
    if (VnGetSymbols(
        pszSettingsPath,
        pOffsets,
        startdocked_SN,
        STARTDOCKED_SB_CNT
    ))
    {
        printf("[Symbols] Failure in reading symbols for \"%s\".\n", startdocked_sb_dll);
        if (hKey) RegCloseKey(hKey);
        return FALSE;
    }

    RegSetValueExW(hKey, TEXT(STARTDOCKED_SB_0), 0, REG_DWORD, &pOffsets[0], sizeof(DWORD));
    RegSetValueExW(hKey, TEXT(STARTDOCKED_SB_1), 0, REG_DWORD, &pOffsets[1], sizeof(DWORD));
    RegSetValueExW(hKey, TEXT(STARTDOCKED_SB_2), 0, REG_DWORD, &pOffsets[2], sizeof(DWORD));
    RegSetValueExW(hKey, TEXT(STARTDOCKED_SB_3), 0, REG_DWORD, &pOffsets[3], sizeof(DWORD));
    RegSetValueExW(hKey, TEXT(STARTDOCKED_SB_4), 0, REG_DWORD, &pOffsets[4], sizeof(DWORD));

    RegSetValueExA(hKey, "Hash", 0, REG_SZ, szHash, strlen(szHash) + 1);

    if (hKey) RegCloseKey(hKey);
    return TRUE;
}

static BOOL ProcessStartUISymbols(const char* pszSettingsPath, DWORD* pOffsets)
{
    HKEY hKey = NULL;
    DWORD dwDisposition;
    RegCreateKeyExW(
        HKEY_CURRENT_USER,
        TEXT(REGPATH_STARTMENU) L"\\" TEXT(STARTUI_SB_NAME),
        0,
        NULL,
        REG_OPTION_NON_VOLATILE,
        KEY_WRITE,
        NULL,
        &hKey,
        &dwDisposition
    );
    if (!hKey || hKey == INVALID_HANDLE_VALUE)
    {
        printf("[Symbols] Unable to create registry key.\n");
        return FALSE;
    }

    CHAR szHash[100];
    WCHAR wszPath[MAX_PATH];

    ZeroMemory(szHash, sizeof(szHash));
    ZeroMemory(wszPath, sizeof(wszPath));

    char startui_sb_dll[MAX_PATH];
    ZeroMemory(startui_sb_dll, sizeof(startui_sb_dll));
    GetWindowsDirectoryA(startui_sb_dll, MAX_PATH);
    strcat_s(startui_sb_dll, MAX_PATH, "\\SystemApps\\Microsoft.Windows.StartMenuExperienceHost_cw5n1h2txyewy\\");
    strcat_s(startui_sb_dll, MAX_PATH, STARTUI_SB_NAME);
    strcat_s(startui_sb_dll, MAX_PATH, ".dll");

    GetWindowsDirectoryW(wszPath, MAX_PATH);
    wcscat_s(wszPath, MAX_PATH, L"\\SystemApps\\Microsoft.Windows.StartMenuExperienceHost_cw5n1h2txyewy\\" _T(STARTUI_SB_NAME) L".dll");
    ComputeFileHash(wszPath, szHash, ARRAYSIZE(szHash));

    printf("[Symbols] Downloading symbols for \"%s\" (\"%s\")...\n", startui_sb_dll, szHash);
    if (VnDownloadSymbols(
        NULL,
        startui_sb_dll,
        pszSettingsPath,
        MAX_PATH
    ))
    {
        printf("[Symbols] Symbols for \"%s\" are not available - unable to download.\n", startui_sb_dll);
        printf("[Symbols] Please refer to \"https://github.com/valinet/ExplorerPatcher/wiki/Symbols\" for more information.\n");
        if (hKey) RegCloseKey(hKey);
        return FALSE;
    }

    printf("[Symbols] Reading symbols...\n");
    if (VnGetSymbols(
        pszSettingsPath,
        pOffsets,
        startui_SN,
        STARTUI_SB_CNT
    ))
    {
        printf("[Symbols] Failure in reading symbols for \"%s\".\n", startui_sb_dll);
        if (hKey) RegCloseKey(hKey);
        return FALSE;
    }

    RegSetValueExW(hKey, TEXT(STARTUI_SB_0), 0, REG_DWORD, &pOffsets[0], sizeof(DWORD));

    RegSetValueExA(hKey, "Hash", 0, REG_SZ, szHash, strlen(szHash) + 1);

    if (hKey) RegCloseKey(hKey);
    return TRUE;
}

DWORD DownloadSymbols(DownloadSymbolsParams* params)
{
    Sleep(6000);

    printf("[Symbols] Started \"Download symbols\" thread.\n");

    RTL_OSVERSIONINFOW rovi;
    DWORD32 ubr = VnGetOSVersionAndUBR(&rovi);
    TCHAR szReportedVersion[MAX_PATH + 1];
    ZeroMemory(
        szReportedVersion,
        sizeof(szReportedVersion)
    );
    wsprintf(
        szReportedVersion,
        L"%d.%d.%d.%d",
        rovi.dwMajorVersion,
        rovi.dwMinorVersion,
        rovi.dwBuildNumber,
        ubr
    );

    TCHAR buffer[1000];
    ZeroMemory(
        buffer,
        1000
    );
    wsprintf(
        buffer,
        DownloadSymbolsXML,
        szReportedVersion
    );
    if (params->bVerbose)
    {
        HRESULT hr = S_OK;
        __x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument* inputXml = NULL;
        hr = String2IXMLDocument(
            buffer,
            wcslen(buffer),
            &inputXml,
#ifdef DEBUG
            stdout
#else
            NULL
#endif
        );
        hr = ShowToastMessage(
            inputXml,
            APPID,
            sizeof(APPID) / sizeof(TCHAR) - 1,
#ifdef DEBUG
            stdout
#else
            NULL
#endif
        );
    }
    wprintf(
        L"[Symbols] "
        L"Attempting to download symbols for OS version %s.\n",
        szReportedVersion
    );

    char szSettingsPath[MAX_PATH];
    ZeroMemory(
        szSettingsPath,
        sizeof(szSettingsPath)
    );
    SHGetFolderPathA(
        NULL,
        SPECIAL_FOLDER_LEGACY,
        NULL,
        SHGFP_TYPE_CURRENT,
        szSettingsPath
    );
    strcat_s(
        szSettingsPath,
        MAX_PATH,
        APP_RELATIVE_PATH
    );
    CreateDirectoryA(szSettingsPath, NULL);
    strcat_s(
        szSettingsPath,
        MAX_PATH,
        "\\"
    );
    printf("[Symbols] Downloading to \"%s\".\n", szSettingsPath);

    symbols_addr symbols_PTRS;
    ZeroMemory(&symbols_PTRS, sizeof(symbols_addr));
    BOOL bDownloaded = FALSE;
    if (params->loadResult.bNeedToDownloadTwinuiPcshellSymbols)
    {
        bDownloaded |= ProcessTwinuiPcshellSymbols(szSettingsPath, symbols_PTRS.twinui_pcshell_PTRS);
    }
    if (params->loadResult.bNeedToDownloadStartDockedSymbols && IsWindows11())
    {
        bDownloaded |= ProcessStartDockedSymbols(szSettingsPath, symbols_PTRS.startdocked_PTRS);
    }
    if (params->loadResult.bNeedToDownloadStartUISymbols && rovi.dwBuildNumber >= 18362)
    {
        bDownloaded |= ProcessStartUISymbols(szSettingsPath, symbols_PTRS.startui_PTRS);
    }

    printf("[Symbols] Finished gathering symbol data.\n");

    if (bDownloaded)
    {
        if (params->bVerbose)
        {
            __x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument* inputXml = NULL;
            HRESULT hr = String2IXMLDocument(
                InstallOK,
                wcslen(InstallOK),
                &inputXml,
#ifdef DEBUG
                stdout
#else
                NULL
#endif
            );
            hr = ShowToastMessage(
                inputXml,
                APPID,
                sizeof(APPID) / sizeof(TCHAR) - 1,
#ifdef DEBUG
                stdout
#else
                NULL
#endif
            );
            Sleep(4000);
            exit(0);
        }
        else
        {
            wsprintf(
                buffer,
                DownloadOKXML,
                szReportedVersion
            );
            __x_ABI_CWindows_CData_CXml_CDom_CIXmlDocument* inputXml2 = NULL;
            HRESULT hr = String2IXMLDocument(
                buffer,
                wcslen(buffer),
                &inputXml2,
#ifdef DEBUG
                stdout
#else
                NULL
#endif
            );
            hr = ShowToastMessage(
                inputXml2,
                APPID,
                sizeof(APPID) / sizeof(TCHAR) - 1,
#ifdef DEBUG
                stdout
#else
                NULL
#endif
            );
        }
    }

    printf("[Symbols] Finished \"Download symbols\" thread.\n");
    return 0;
}

LoadSymbolsResult LoadSymbols(symbols_addr* symbols_PTRS)
{
    LoadSymbolsResult result;
    ZeroMemory(&result, sizeof(LoadSymbolsResult));

    HKEY hKey = NULL;
    DWORD dwDisposition;
    DWORD dwSize;

    RTL_OSVERSIONINFOW rovi;
    DWORD32 ubr = VnGetOSVersionAndUBR(&rovi);

    CHAR szHash[33];
    CHAR szStoredHash[33];
    ZeroMemory(szHash, sizeof(szHash));
    ZeroMemory(szStoredHash, sizeof(szStoredHash));
    TCHAR wszPath[MAX_PATH];
    BOOL bOffsetsValid;

    // Load twinui.pcshell.dll offsets
    bOffsetsValid = FALSE;
    RegCreateKeyExW(
        HKEY_CURRENT_USER,
        TEXT(REGPATH) L"\\" TEXT(TWINUI_PCSHELL_SB_NAME),
        0,
        NULL,
        REG_OPTION_NON_VOLATILE,
        KEY_READ,
        NULL,
        &hKey,
        &dwDisposition
    );
    if (!hKey || hKey == INVALID_HANDLE_VALUE)
    {
        result.bSuccess = FALSE;
        return result;
    }

    GetSystemDirectoryW(wszPath, MAX_PATH);
    wcscat_s(wszPath, MAX_PATH, L"\\" TEXT(TWINUI_PCSHELL_SB_NAME) L".dll");
    if (ComputeFileHash(wszPath, szHash, ARRAYSIZE(szHash)) == ERROR_SUCCESS)
    {
        szStoredHash[0] = 0;
        dwSize = sizeof(szStoredHash);
        RegQueryValueExA(hKey, "Hash", 0, NULL, szStoredHash, &dwSize);

        if (!_stricmp(szHash, szStoredHash))
        {
            dwSize = sizeof(DWORD);
            RegQueryValueExW(hKey, TEXT(TWINUI_PCSHELL_SB_0), 0, NULL, &symbols_PTRS->twinui_pcshell_PTRS[0], &dwSize);
            RegQueryValueExW(hKey, TEXT(TWINUI_PCSHELL_SB_1), 0, NULL, &symbols_PTRS->twinui_pcshell_PTRS[1], &dwSize);
            RegQueryValueExW(hKey, TEXT(TWINUI_PCSHELL_SB_2), 0, NULL, &symbols_PTRS->twinui_pcshell_PTRS[2], &dwSize);
            RegQueryValueExW(hKey, TEXT(TWINUI_PCSHELL_SB_3), 0, NULL, &symbols_PTRS->twinui_pcshell_PTRS[3], &dwSize);
            RegQueryValueExW(hKey, TEXT(TWINUI_PCSHELL_SB_4), 0, NULL, &symbols_PTRS->twinui_pcshell_PTRS[4], &dwSize);
            RegQueryValueExW(hKey, TEXT(TWINUI_PCSHELL_SB_5), 0, NULL, &symbols_PTRS->twinui_pcshell_PTRS[5], &dwSize);
            RegQueryValueExW(hKey, TEXT(TWINUI_PCSHELL_SB_6), 0, NULL, &symbols_PTRS->twinui_pcshell_PTRS[6], &dwSize);
            RegQueryValueExW(hKey, TEXT(TWINUI_PCSHELL_SB_7), 0, NULL, &symbols_PTRS->twinui_pcshell_PTRS[7], &dwSize);
            if (IsWindows11Version22H2OrHigher())
            {
                RegQueryValueExW(hKey, TEXT(TWINUI_PCSHELL_SB_LAST), 0, NULL, &symbols_PTRS->twinui_pcshell_PTRS[TWINUI_PCSHELL_SB_CNT - 1], &dwSize);
            }
            bOffsetsValid = TRUE;
        }
        else
        {
            printf("[Symbols] Symbols for \"%s\" are not available.\n", TWINUI_PCSHELL_SB_NAME);
            result.bNeedToDownloadTwinuiPcshellSymbols = TRUE;
        }
    }

    RegCloseKey(hKey);
    if (!bOffsetsValid)
    {
        RegDeleteTreeW(
            HKEY_CURRENT_USER,
            TEXT(REGPATH) L"\\" TEXT(TWINUI_PCSHELL_SB_NAME)
        );
    }

    if (IsWindows11())
    {
        // Load StartDocked.dll offsets
        bOffsetsValid = FALSE;
        RegCreateKeyExW(
            HKEY_CURRENT_USER,
            TEXT(REGPATH_STARTMENU) L"\\" TEXT(STARTDOCKED_SB_NAME),
            0,
            NULL,
            REG_OPTION_NON_VOLATILE,
            KEY_READ,
            NULL,
            &hKey,
            &dwDisposition
        );

        GetWindowsDirectoryW(wszPath, MAX_PATH);
        wcscat_s(wszPath, MAX_PATH, L"\\SystemApps\\Microsoft.Windows.StartMenuExperienceHost_cw5n1h2txyewy\\" TEXT(STARTDOCKED_SB_NAME) L".dll");
        if (ComputeFileHash(wszPath, szHash, ARRAYSIZE(szHash)) == ERROR_SUCCESS)
        {
            szStoredHash[0] = 0;
            dwSize = sizeof(szStoredHash);
            RegQueryValueExA(hKey, "Hash", 0, NULL, szStoredHash, &dwSize);

            if (!_stricmp(szHash, szStoredHash))
            {
                dwSize = sizeof(DWORD);
                RegQueryValueExW(hKey, TEXT(STARTDOCKED_SB_0), 0, NULL, &symbols_PTRS->startdocked_PTRS[0], &dwSize);
                RegQueryValueExW(hKey, TEXT(STARTDOCKED_SB_1), 0, NULL, &symbols_PTRS->startdocked_PTRS[1], &dwSize);
                RegQueryValueExW(hKey, TEXT(STARTDOCKED_SB_2), 0, NULL, &symbols_PTRS->startdocked_PTRS[2], &dwSize);
                RegQueryValueExW(hKey, TEXT(STARTDOCKED_SB_3), 0, NULL, &symbols_PTRS->startdocked_PTRS[3], &dwSize);
                RegQueryValueExW(hKey, TEXT(STARTDOCKED_SB_4), 0, NULL, &symbols_PTRS->startdocked_PTRS[4], &dwSize);
                bOffsetsValid = TRUE;
            }
            else
            {
                printf("[Symbols] Symbols for \"%s\" are not available.\n", STARTDOCKED_SB_NAME);
                result.bNeedToDownloadStartDockedSymbols = TRUE;
            }
        }
        if (hKey) RegCloseKey(hKey);
        if (!bOffsetsValid)
        {
            RegDeleteTreeW(
                HKEY_CURRENT_USER,
                TEXT(REGPATH_STARTMENU) L"\\" TEXT(STARTDOCKED_SB_NAME)
            );
        }
    }

    if (rovi.dwBuildNumber >= 18362)
    {
        // Load StartUI.dll offsets
        bOffsetsValid = FALSE;
        RegCreateKeyExW(
            HKEY_CURRENT_USER,
            TEXT(REGPATH_STARTMENU) L"\\" TEXT(STARTUI_SB_NAME),
            0,
            NULL,
            REG_OPTION_NON_VOLATILE,
            KEY_READ,
            NULL,
            &hKey,
            &dwDisposition
        );

        GetWindowsDirectoryW(wszPath, MAX_PATH);
        wcscat_s(wszPath, MAX_PATH, L"\\SystemApps\\Microsoft.Windows.StartMenuExperienceHost_cw5n1h2txyewy\\" TEXT(STARTUI_SB_NAME) L".dll");
        if (ComputeFileHash(wszPath, szHash, ARRAYSIZE(szHash)) == ERROR_SUCCESS)
        {
            szStoredHash[0] = 0;
            dwSize = sizeof(szStoredHash);
            RegQueryValueExA(hKey, "Hash", 0, NULL, szStoredHash, &dwSize);

            if (!_stricmp(szHash, szStoredHash))
            {
                dwSize = sizeof(DWORD);
                RegQueryValueExW(hKey, TEXT(STARTUI_SB_0), 0, NULL, &symbols_PTRS->startui_PTRS[0], &dwSize);
                bOffsetsValid = TRUE;
            }
            else
            {
                printf("[Symbols] Symbols for \"%s\" are not available.\n", STARTUI_SB_NAME);
                result.bNeedToDownloadStartUISymbols = TRUE;
            }
        }
        if (hKey) RegCloseKey(hKey);
        if (!bOffsetsValid)
        {
            RegDeleteTreeW(
               HKEY_CURRENT_USER,
               TEXT(REGPATH_STARTMENU) L"\\" TEXT(STARTUI_SB_NAME)
           );
        }
    }

    // Delete "OSBuild" value from previous versions of EP
    RegCreateKeyExW(
        HKEY_CURRENT_USER,
        TEXT(REGPATH),
        0,
        NULL,
        REG_OPTION_NON_VOLATILE,
        KEY_WRITE,
        NULL,
        &hKey,
        &dwDisposition
    );
    if (hKey && hKey != INVALID_HANDLE_VALUE)
    {
        RegDeleteValueW(hKey, TEXT("OSBuild"));
        RegCloseKey(hKey);
    }

    result.bSuccess = TRUE;
    return result;
}
