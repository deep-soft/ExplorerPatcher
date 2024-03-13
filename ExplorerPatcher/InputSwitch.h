﻿#pragma once

#include <Windows.h>

DEFINE_GUID(CLSID_InputSwitchControl, 0xb9bc2a50, 0x43c3, 0x41aa, 0xa0, 0x86, 0x5d, 0xb1, 0x4e, 0x18, 0x4b, 0xae);
DEFINE_GUID(IID_IInputSwitchControl, 0xb9bc2a50, 0x43c3, 0x41aa, 0xa0, 0x82, 0x5d, 0xb1, 0x4e, 0x18, 0x4b, 0xae);

enum __MIDL___MIDL_itf_inputswitchserver_0000_0000_0001
{
    ISCT_IDL_DESKTOP,
    ISCT_IDL_TOUCHKEYBOARD,
    ISCT_IDL_LOGONUI,
    ISCT_IDL_UAC,
    ISCT_IDL_SETTINGSPANE,
    ISCT_IDL_OOBE,
    ISCT_IDL_USEROOBE
};

struct __MIDL___MIDL_itf_inputswitchserver_0000_0000_0002
{
    int dummy; // We don't need its contents
};

struct __MIDL___MIDL_itf_inputswitchserver_0000_0000_0003
{
    WCHAR* pszTooltip;
    HICON hIcon;
    BOOL fDisabled;
    BOOL fHidden;
    WCHAR* pszIconGlyph;
};

enum __MIDL___MIDL_itf_inputswitchserver_0000_0000_0004
{
    INPUT_SWITCH_IDL_IME_CLICK_TYPE_LEFT,
    INPUT_SWITCH_IDL_IME_CLICK_TYPE_RIGHT,
    INPUT_SWITCH_IDL_IME_CLICK_TYPE_LEFT_DISABLED
};

enum __MIDL___MIDL_itf_inputswitchserver_0000_0000_0005
{
    INPUT_SWITCH_IDL_MODALITY_STANDARDKEYBOARD = 0x1,
    INPUT_SWITCH_IDL_MODALITY_SPLITKEYBOARD = 0x2,
    INPUT_SWITCH_IDL_MODALITY_CLASSICKEYBOARD = 0x4,
    INPUT_SWITCH_IDL_MODALITY_HANDWRITING = 0x8,
    INPUT_SWITCH_IDL_MODALITY_HIDE = 0x10,
    INPUT_SWITCH_IDL_MODALITY_ONEHANDEDKEYBOARD = 0x20,
};

enum __MIDL___MIDL_itf_inputswitchserver_0000_0000_0006
{
    INPUT_SWITCH_IDL_ALIGN_DEFAULT,
    INPUT_SWITCH_IDL_ALIGN_RIGHT_EDGE,
    INPUT_SWITCH_IDL_ALIGN_LEFT_EDGE,
};

enum __MIDL___MIDL_itf_inputswitchserver_0000_0000_0008
{
    INPUT_SWITCH_IDL_CFOM_NO_OVERRIDE,
    INPUT_SWITCH_IDL_CFOM_DESKTOP,
    INPUT_SWITCH_IDL_CFOM_IMMERSIVE,
};

interface IInputSwitchCallback;

#ifdef __cplusplus
MIDL_INTERFACE("b9bc2a50-43c3-41aa-a082-5db14e184bae")
IInputSwitchControl : IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE Init(__MIDL___MIDL_itf_inputswitchserver_0000_0000_0001) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetCallback(IInputSwitchCallback*) = 0;
    virtual HRESULT STDMETHODCALLTYPE ShowInputSwitch(const RECT*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetProfileCount(UINT*, BOOL*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetCurrentProfile(__MIDL___MIDL_itf_inputswitchserver_0000_0000_0002*) = 0;
    virtual HRESULT STDMETHODCALLTYPE RegisterHotkeys() = 0;
    virtual HRESULT STDMETHODCALLTYPE ClickImeModeItem(__MIDL___MIDL_itf_inputswitchserver_0000_0000_0004, POINT, const RECT*) = 0;
    virtual HRESULT STDMETHODCALLTYPE ForceHide() = 0;
    virtual HRESULT STDMETHODCALLTYPE ShowTouchKeyboardInputSwitch(const RECT*, __MIDL___MIDL_itf_inputswitchserver_0000_0000_0006, int, DWORD, __MIDL___MIDL_itf_inputswitchserver_0000_0000_0005) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetContextFlags(DWORD*) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetContextOverrideMode(__MIDL___MIDL_itf_inputswitchserver_0000_0000_0008) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetCurrentImeModeItem(__MIDL___MIDL_itf_inputswitchserver_0000_0000_0003*) = 0;
    virtual HRESULT STDMETHODCALLTYPE ActivateInputProfile(const WCHAR*) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetUserSid(const WCHAR*) = 0;
};
#else
typedef interface IInputSwitchControl IInputSwitchControl;
#endif

#ifdef __cplusplus
MIDL_INTERFACE("b9bc2a50-43c3-41aa-a082-5db14e184bae")
IInputSwitchControlSV2 : IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE Init(__MIDL___MIDL_itf_inputswitchserver_0000_0000_0001) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetCallback(IInputSwitchCallback*) = 0;
    virtual HRESULT STDMETHODCALLTYPE ShowInputSwitch(const RECT*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetProfileCount(UINT*, BOOL*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetCurrentProfile(__MIDL___MIDL_itf_inputswitchserver_0000_0000_0002*) = 0;
    virtual HRESULT STDMETHODCALLTYPE RegisterHotkeys() = 0;
    virtual HRESULT STDMETHODCALLTYPE ClickImeModeItem(__MIDL___MIDL_itf_inputswitchserver_0000_0000_0004, POINT, const RECT*) = 0;
    virtual HRESULT STDMETHODCALLTYPE ClickImeModeItemWithAnchor(__MIDL___MIDL_itf_inputswitchserver_0000_0000_0004, IUnknown*) = 0;
    virtual HRESULT STDMETHODCALLTYPE ForceHide() = 0;
    virtual HRESULT STDMETHODCALLTYPE ShowTouchKeyboardInputSwitch(const RECT*, __MIDL___MIDL_itf_inputswitchserver_0000_0000_0006, int, DWORD, __MIDL___MIDL_itf_inputswitchserver_0000_0000_0005) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetContextFlags(DWORD*) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetContextOverrideMode(__MIDL___MIDL_itf_inputswitchserver_0000_0000_0008) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetCurrentImeModeItem(__MIDL___MIDL_itf_inputswitchserver_0000_0000_0003*) = 0;
    virtual HRESULT STDMETHODCALLTYPE ActivateInputProfile(const WCHAR*) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetUserSid(const WCHAR*) = 0;
};
#else
typedef interface IInputSwitchControlSV2 IInputSwitchControlSV2;
#endif

#ifdef __cplusplus
extern "C" {
#endif

HRESULT CInputSwitchControlProxy_CreateInstance(IInputSwitchControl* original, REFIID riid, void** ppvObject);
HRESULT CInputSwitchControlProxySV2_CreateInstance(IInputSwitchControlSV2* original, REFIID riid, void** ppvObject);

#ifdef __cplusplus
}
#endif
