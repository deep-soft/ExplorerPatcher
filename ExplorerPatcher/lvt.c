#include "lvt.h"

Windows_UI_Xaml_IDependencyObject* LVT_FindChildByClassName(Windows_UI_Xaml_IDependencyObject* pRootDependencyObject, Windows_UI_Xaml_IVisualTreeHelperStatics* pVisualTreeHelperStatics, LPCWSTR pwszRefName, INT* prevIndex)
{
    if (!pRootDependencyObject)
    {
        return NULL;
    }

    //WCHAR wszDebug[MAX_PATH];
    HRESULT hr = S_OK;
    INT32 Count = -1;
    hr = pVisualTreeHelperStatics->lpVtbl->GetChildrenCount(pVisualTreeHelperStatics, pRootDependencyObject, &Count);
    if (SUCCEEDED(hr))
    {
        for (INT32 Index = (prevIndex ? *prevIndex : 0); Index < Count; ++Index)
        {
            Windows_UI_Xaml_IDependencyObject* pChild = NULL;
            hr = pVisualTreeHelperStatics->lpVtbl->GetChild(pVisualTreeHelperStatics, pRootDependencyObject, Index, &pChild);
            if (SUCCEEDED(hr))
            {
                HSTRING hsChild = NULL;
                pChild->lpVtbl->GetRuntimeClassName(pChild, &hsChild);
                if (SUCCEEDED(hr))
                {
                    PCWSTR pwszName = WindowsGetStringRawBuffer(hsChild, 0);
                    //swprintf_s(wszDebug, MAX_PATH, L"Name: %s\n", pwszName);
                    //OutputDebugStringW(wszDebug);
                    if (!_wcsicmp(pwszName, pwszRefName))
                    {
                        WindowsDeleteString(hsChild);
                        if (prevIndex) *prevIndex = Index + 1;
                        return pChild;
                    }
                    WindowsDeleteString(hsChild);
                }
                pChild->lpVtbl->Release(pChild);
            }
        }
    }

    if (prevIndex) *prevIndex = Count;
    return NULL;
}

Windows_UI_Xaml_IDependencyObject* LVT_FindChildByName(Windows_UI_Xaml_IDependencyObject* pRootDependencyObject, Windows_UI_Xaml_IVisualTreeHelperStatics* pVisualTreeHelperStatics, LPCWSTR pwszRefName)
{
    if (!pRootDependencyObject)
    {
        return NULL;
    }

    //WCHAR wszDebug[MAX_PATH];
    HRESULT hr = S_OK;
    INT32 Count = -1;
    hr = pVisualTreeHelperStatics->lpVtbl->GetChildrenCount(pVisualTreeHelperStatics, pRootDependencyObject, &Count);
    if (SUCCEEDED(hr))
    {
        for (INT32 Index = 0; Index < Count; ++Index)
        {
            Windows_UI_Xaml_IDependencyObject* pChild = NULL;
            hr = pVisualTreeHelperStatics->lpVtbl->GetChild(pVisualTreeHelperStatics, pRootDependencyObject, Index, &pChild);
            if (SUCCEEDED(hr))
            {
                Windows_UI_Xaml_IFrameworkElement* pFrameworkElement = NULL;
                hr = pChild->lpVtbl->QueryInterface(pChild, &IID_Windows_UI_Xaml_IFrameworkElement, &pFrameworkElement);
                if (SUCCEEDED(hr))
                {
                    HSTRING hsChild = NULL;
                    pFrameworkElement->lpVtbl->get_Name(pFrameworkElement, &hsChild);
                    if (SUCCEEDED(hr))
                    {
                        PCWSTR pwszName = WindowsGetStringRawBuffer(hsChild, 0);
                        //swprintf_s(wszDebug, MAX_PATH, L"Name: %s\n", pwszName);
                        //OutputDebugStringW(wszDebug);
                        if (!_wcsicmp(pwszName, pwszRefName))
                        {
                            WindowsDeleteString(hsChild);
                            pFrameworkElement->lpVtbl->Release(pFrameworkElement);
                            return pChild;
                        }
                        WindowsDeleteString(hsChild);
                    }
                    pFrameworkElement->lpVtbl->Release(pFrameworkElement);
                }
                pChild->lpVtbl->Release(pChild);
            }
        }
    }

    return NULL;
}

// Referenece: https://www.reddit.com/r/Windows10/comments/nvcrie/windows_11_start_menu_how_to_temporary_make_your/
void LVT_StartUI_EnableRoundedCorners(HWND hWnd, BOOL bApply)
{
    WCHAR wszDebug[MAX_PATH];
    HRESULT hr = S_OK;

    Windows_UI_Xaml_IDependencyObject* pRootDependencyObject = NULL;
    Windows_UI_Xaml_Controls_ICanvasStatics* pCanvasStatics = NULL;

    if (SUCCEEDED(hr))
    {
        HSTRING_HEADER hshControlsCanvasStatics;
        HSTRING hsControlsCanvasStatics = NULL;
        hr = WindowsCreateStringReference(L"Windows.UI.Xaml.Controls.Canvas", 31, &hshControlsCanvasStatics, &hsControlsCanvasStatics);
        if (SUCCEEDED(hr) && hsControlsCanvasStatics)
        {
            hr = RoGetActivationFactory(hsControlsCanvasStatics, &IID_Windows_UI_Xaml_Controls_ICanvasStatics, &pCanvasStatics);
            WindowsDeleteString(hsControlsCanvasStatics);
        }
    }

    if (SUCCEEDED(hr))
    {
        HSTRING_HEADER hshWindowStatics;
        HSTRING hsWindowStatics = NULL;
        hr = WindowsCreateStringReference(L"Windows.UI.Xaml.Window", 22, &hshWindowStatics, &hsWindowStatics);
        if (SUCCEEDED(hr) && hsWindowStatics)
        {
            Windows_UI_Xaml_IWindowStatics* pWindowStatics = NULL;
            hr = RoGetActivationFactory(hsWindowStatics, &IID_Windows_UI_Xaml_IWindowStatics, &pWindowStatics);
            if (SUCCEEDED(hr))
            {
                Windows_UI_Xaml_IWindow* pWindow = NULL;
                hr = pWindowStatics->lpVtbl->get_Current(pWindowStatics, &pWindow);
                if (SUCCEEDED(hr))
                {
                    IInspectable* pUIElement = NULL;
                    hr = pWindow->lpVtbl->get_Content(pWindow, &pUIElement);
                    if (SUCCEEDED(hr))
                    {
                        hr = pUIElement->lpVtbl->QueryInterface(pUIElement, &IID_Windows_UI_Xaml_IDependencyObject, &pRootDependencyObject);

                        pUIElement->lpVtbl->Release(pUIElement);
                    }
                    pWindow->lpVtbl->Release(pWindow);
                }
                pWindowStatics->lpVtbl->Release(pWindowStatics);
            }
            WindowsDeleteString(hsWindowStatics);
        }
    }

    if (pRootDependencyObject)
    {
        HSTRING_HEADER hshVisualTreeHelperStatics;
        HSTRING hsVisualTreeHelperStatics = NULL;
        hr = WindowsCreateStringReference(L"Windows.UI.Xaml.Media.VisualTreeHelper", 38, &hshVisualTreeHelperStatics, &hsVisualTreeHelperStatics);
        if (SUCCEEDED(hr) && hsVisualTreeHelperStatics)
        {
            Windows_UI_Xaml_IVisualTreeHelperStatics* pVisualTreeHelperStatics = NULL;
            hr = RoGetActivationFactory(hsVisualTreeHelperStatics, &IID_Windows_UI_Xaml_IVisualTreeHelperStatics, &pVisualTreeHelperStatics);
            if (SUCCEEDED(hr))
            {
                Windows_UI_Xaml_IDependencyObject* pStartSizingFrame = LVT_FindChildByClassName(pRootDependencyObject, pVisualTreeHelperStatics, L"StartUI.StartSizingFrame", NULL);
                if (pStartSizingFrame)
                {
                    Windows_UI_Xaml_IDependencyObject* pStartSizingFramePanel = LVT_FindChildByClassName(pStartSizingFrame, pVisualTreeHelperStatics, L"StartUI.StartSizingFramePanel", NULL);
                    if (pStartSizingFramePanel)
                    {
                        // Drop shadow
                        Windows_UI_Xaml_IDependencyObject* pDropShadow = LVT_FindChildByClassName(pStartSizingFramePanel, pVisualTreeHelperStatics, L"Windows.UI.Xaml.Controls.Image", NULL);
                        if (pDropShadow)
                        {
                            Windows_UI_Xaml_IUIElement* pIUIElement = NULL;
                            pDropShadow->lpVtbl->QueryInterface(pDropShadow, &IID_Windows_UI_Xaml_IUIElement, &pIUIElement);
                            if (pIUIElement)
                            {
                                if (bApply)
                                {
                                    pIUIElement->lpVtbl->put_Visibility(pIUIElement, Windows_UI_Xaml_Visibility_Collapsed);
                                }
                                else
                                {
                                    pIUIElement->lpVtbl->put_Visibility(pIUIElement, Windows_UI_Xaml_Visibility_Visible);
                                }
                                pIUIElement->lpVtbl->Release(pIUIElement);
                            }
                            pDropShadow->lpVtbl->Release(pDropShadow);
                        }
                        Windows_UI_Xaml_IDependencyObject* pContentPresenter = LVT_FindChildByClassName(pStartSizingFramePanel, pVisualTreeHelperStatics, L"Windows.UI.Xaml.Controls.ContentPresenter", NULL);
                        if (pContentPresenter)
                        {
                            Windows_UI_Xaml_IDependencyObject* pFrame = LVT_FindChildByClassName(pContentPresenter, pVisualTreeHelperStatics, L"Windows.UI.Xaml.Controls.Frame", NULL);
                            if (pFrame)
                            {
                                // Main menu padding
                                Windows_UI_Xaml_Controls_IControl* pIBorder = NULL;
                                pFrame->lpVtbl->QueryInterface(pFrame, &IID_Windows_UI_Xaml_Controls_IControl, &pIBorder);
                                if (pIBorder)
                                {
                                    Windows_UI_Xaml_Thickness th;
                                    th.Left = (bApply ? 10.0 : 0.0);
                                    th.Bottom = th.Left;
                                    th.Right = th.Left;
                                    th.Top = th.Left;
                                    pIBorder->lpVtbl->put_Padding(pIBorder, th);
                                    pIBorder->lpVtbl->Release(pIBorder);
                                }
                                Windows_UI_Xaml_IDependencyObject* pContentPresenter2 = LVT_FindChildByClassName(pFrame, pVisualTreeHelperStatics, L"Windows.UI.Xaml.Controls.ContentPresenter", NULL);
                                if (pContentPresenter2)
                                {
                                    Windows_UI_Xaml_IDependencyObject* pSplitViewFrame = LVT_FindChildByClassName(pContentPresenter2, pVisualTreeHelperStatics, L"StartUI.SplitViewFrame", NULL);
                                    if (pSplitViewFrame)
                                    {
                                        Windows_UI_Xaml_IDependencyObject* pRootGrid = LVT_FindChildByName(pSplitViewFrame, pVisualTreeHelperStatics, L"RootGrid");
                                        if (pRootGrid)
                                        {
                                            // Main menu corners
                                            Windows_UI_Xaml_IDependencyObject* pAcrylicBorder = LVT_FindChildByName(pRootGrid, pVisualTreeHelperStatics, L"AcrylicBorder");
                                            if (pAcrylicBorder)
                                            {
                                                Windows_UI_Xaml_Controls_IBorder* pIBorder = NULL;
                                                pAcrylicBorder->lpVtbl->QueryInterface(pAcrylicBorder, &IID_Windows_UI_Xaml_Controls_IBorder, &pIBorder);
                                                if (pIBorder)
                                                {
                                                    Windows_UI_Xaml_CornerRadius cr;
                                                    cr.BottomLeft = (bApply ? 10.0 : 0.0);
                                                    cr.BottomRight = cr.BottomLeft;
                                                    cr.TopLeft = cr.BottomLeft;
                                                    cr.TopRight = cr.BottomLeft;
                                                    pIBorder->lpVtbl->put_CornerRadius(pIBorder, cr);
                                                    pIBorder->lpVtbl->Release(pIBorder);
                                                }
                                                pAcrylicBorder->lpVtbl->Release(pAcrylicBorder);
                                            }
                                            // Live tiles corners
                                            Windows_UI_Xaml_IDependencyObject* pRootContent = LVT_FindChildByName(pRootGrid, pVisualTreeHelperStatics, L"RootContent");
                                            if (pRootContent)
                                            {
                                                Windows_UI_Xaml_IDependencyObject* pGrid = LVT_FindChildByClassName(pRootContent, pVisualTreeHelperStatics, L"Windows.UI.Xaml.Controls.Grid", NULL);
                                                if (pGrid)
                                                {
                                                    Windows_UI_Xaml_IDependencyObject* pContentRoot = LVT_FindChildByName(pGrid, pVisualTreeHelperStatics, L"ContentRoot");
                                                    if (pContentRoot)
                                                    {
                                                        Windows_UI_Xaml_IDependencyObject* pBorder = LVT_FindChildByClassName(pContentRoot, pVisualTreeHelperStatics, L"Windows.UI.Xaml.Controls.Border", NULL);
                                                        if (pBorder)
                                                        {
                                                            Windows_UI_Xaml_IDependencyObject* pContentPaneGrid = LVT_FindChildByName(pBorder, pVisualTreeHelperStatics, L"ContentPaneGrid");
                                                            if (pContentPaneGrid)
                                                            {
                                                                Windows_UI_Xaml_IDependencyObject* pGridPane = LVT_FindChildByName(pContentPaneGrid, pVisualTreeHelperStatics, L"GridPane");
                                                                if (pGridPane)
                                                                {
                                                                    Windows_UI_Xaml_IDependencyObject* ppage = LVT_FindChildByName(pGridPane, pVisualTreeHelperStatics, L"page");
                                                                    if (ppage)
                                                                    {
                                                                        Windows_UI_Xaml_IDependencyObject* pgridRoot = LVT_FindChildByName(ppage, pVisualTreeHelperStatics, L"gridRoot");
                                                                        if (pgridRoot)
                                                                        {
                                                                            Windows_UI_Xaml_IDependencyObject* pgroupItems = LVT_FindChildByName(pgridRoot, pVisualTreeHelperStatics, L"groupItems");
                                                                            if (pgroupItems)
                                                                            {
                                                                                Windows_UI_Xaml_IDependencyObject* pBorder2 = LVT_FindChildByClassName(pgroupItems, pVisualTreeHelperStatics, L"Windows.UI.Xaml.Controls.Border", NULL);
                                                                                if (pBorder2)
                                                                                {
                                                                                    Windows_UI_Xaml_IDependencyObject* pScrollViewer = LVT_FindChildByName(pBorder2, pVisualTreeHelperStatics, L"ScrollViewer");
                                                                                    if (pScrollViewer)
                                                                                    {
                                                                                        Windows_UI_Xaml_IDependencyObject* pBorder3 = LVT_FindChildByClassName(pScrollViewer, pVisualTreeHelperStatics, L"Windows.UI.Xaml.Controls.Border", NULL);
                                                                                        if (pBorder3)
                                                                                        {
                                                                                            Windows_UI_Xaml_IDependencyObject* pGrid2 = LVT_FindChildByClassName(pBorder3, pVisualTreeHelperStatics, L"Windows.UI.Xaml.Controls.Grid", NULL);
                                                                                            if (pGrid2)
                                                                                            {
                                                                                                Windows_UI_Xaml_IDependencyObject* pScrollContentPresenter = LVT_FindChildByName(pGrid2, pVisualTreeHelperStatics, L"ScrollContentPresenter");
                                                                                                if (pScrollContentPresenter)
                                                                                                {
                                                                                                    Windows_UI_Xaml_IDependencyObject* pItemsPresenter = LVT_FindChildByClassName(pScrollContentPresenter, pVisualTreeHelperStatics, L"Windows.UI.Xaml.Controls.ItemsPresenter", NULL);
                                                                                                    if (pItemsPresenter)
                                                                                                    {
                                                                                                        Windows_UI_Xaml_IDependencyObject* pTileGrid = LVT_FindChildByClassName(pItemsPresenter, pVisualTreeHelperStatics, L"Windows.UI.Xaml.Controls.TileGrid", NULL);
                                                                                                        if (pTileGrid)
                                                                                                        {
                                                                                                            INT iIndex = 0;
                                                                                                            BOOL bSkipFirst = TRUE;
                                                                                                            while (TRUE)
                                                                                                            {
                                                                                                                Windows_UI_Xaml_IDependencyObject* pCurrentGroup = LVT_FindChildByClassName(pTileGrid, pVisualTreeHelperStatics, L"StartUI.TileListViewItem", &iIndex);
                                                                                                                if (!pCurrentGroup)
                                                                                                                {
                                                                                                                    break;
                                                                                                                }
                                                                                                                if (bSkipFirst)
                                                                                                                {
                                                                                                                    bSkipFirst = FALSE;
                                                                                                                    pCurrentGroup->lpVtbl->Release(pCurrentGroup);
                                                                                                                    continue;
                                                                                                                }
                                                                                                                Windows_UI_Xaml_IDependencyObject* pcontentPresenter = LVT_FindChildByName(pCurrentGroup, pVisualTreeHelperStatics, L"contentPresenter");
                                                                                                                if (pcontentPresenter)
                                                                                                                {
                                                                                                                    Windows_UI_Xaml_IDependencyObject* pTileGroupViewControl = LVT_FindChildByClassName(pcontentPresenter, pVisualTreeHelperStatics, L"StartUI.TileGroupViewControl", NULL);
                                                                                                                    if (pTileGroupViewControl)
                                                                                                                    {
                                                                                                                        Windows_UI_Xaml_IDependencyObject* pGrid3 = LVT_FindChildByClassName(pTileGroupViewControl, pVisualTreeHelperStatics, L"Windows.UI.Xaml.Controls.Grid", NULL);
                                                                                                                        if (pGrid3)
                                                                                                                        {
                                                                                                                            Windows_UI_Xaml_IDependencyObject* pNestedPanel = LVT_FindChildByName(pGrid3, pVisualTreeHelperStatics, L"NestedPanel");
                                                                                                                            if (pNestedPanel)
                                                                                                                            {
                                                                                                                                INT jIndex = 0;
                                                                                                                                while (TRUE)
                                                                                                                                {
                                                                                                                                    Windows_UI_Xaml_IDependencyObject* pCurrentTile = LVT_FindChildByClassName(pNestedPanel, pVisualTreeHelperStatics, L"StartUI.TileListViewItem", &jIndex);
                                                                                                                                    if (!pCurrentTile)
                                                                                                                                    {
                                                                                                                                        break;
                                                                                                                                    }
                                                                                                                                    Windows_UI_Xaml_IDependencyObject* pcontentPresenter2 = LVT_FindChildByName(pCurrentTile, pVisualTreeHelperStatics, L"contentPresenter");
                                                                                                                                    if (pcontentPresenter2)
                                                                                                                                    {
                                                                                                                                        Windows_UI_Xaml_IDependencyObject* pTileViewControl = LVT_FindChildByClassName(pcontentPresenter2, pVisualTreeHelperStatics, L"StartUI.TileViewControl", NULL);
                                                                                                                                        if (pTileViewControl)
                                                                                                                                        {
                                                                                                                                            Windows_UI_Xaml_Controls_IGrid2* pIGrid2 = NULL;
                                                                                                                                            pTileViewControl->lpVtbl->QueryInterface(pTileViewControl, &IID_Windows_UI_Xaml_Controls_IGrid2, &pIGrid2);
                                                                                                                                            if (pIGrid2)
                                                                                                                                            {
                                                                                                                                                Windows_UI_Xaml_CornerRadius cr;
                                                                                                                                                cr.BottomLeft = (bApply ? 5.0 : 0.0);
                                                                                                                                                cr.BottomRight = cr.BottomLeft;
                                                                                                                                                cr.TopLeft = cr.BottomLeft;
                                                                                                                                                cr.TopRight = cr.BottomLeft;
                                                                                                                                                pIGrid2->lpVtbl->put_CornerRadius(pIGrid2, cr);
                                                                                                                                                pIGrid2->lpVtbl->Release(pIGrid2);
                                                                                                                                            }
                                                                                                                                            pTileViewControl->lpVtbl->Release(pTileViewControl);
                                                                                                                                        }
                                                                                                                                        pcontentPresenter2->lpVtbl->Release(pcontentPresenter2);
                                                                                                                                    }
                                                                                                                                    pCurrentTile->lpVtbl->Release(pCurrentTile);
                                                                                                                                }
                                                                                                                                pNestedPanel->lpVtbl->Release(pNestedPanel);
                                                                                                                            }
                                                                                                                            pGrid3->lpVtbl->Release(pGrid3);
                                                                                                                        }
                                                                                                                        pTileGroupViewControl->lpVtbl->Release(pTileGroupViewControl);
                                                                                                                    }
                                                                                                                    pcontentPresenter->lpVtbl->Release(pcontentPresenter);
                                                                                                                }
                                                                                                                pCurrentGroup->lpVtbl->Release(pCurrentGroup);
                                                                                                            }
                                                                                                            pTileGrid->lpVtbl->Release(pTileGrid);
                                                                                                        }
                                                                                                        pItemsPresenter->lpVtbl->Release(pItemsPresenter);
                                                                                                    }
                                                                                                    pScrollContentPresenter->lpVtbl->Release(pScrollContentPresenter);
                                                                                                }
                                                                                                pGrid2->lpVtbl->Release(pGrid2);
                                                                                            }
                                                                                            pBorder3->lpVtbl->Release(pBorder3);
                                                                                        }
                                                                                        pScrollViewer->lpVtbl->Release(pScrollViewer);
                                                                                    }
                                                                                    pBorder2->lpVtbl->Release(pBorder2);
                                                                                }
                                                                                pgroupItems->lpVtbl->Release(pgroupItems);
                                                                            }
                                                                            pgridRoot->lpVtbl->Release(pgridRoot);
                                                                        }
                                                                        ppage->lpVtbl->Release(ppage);
                                                                    }
                                                                    pGridPane->lpVtbl->Release(pGridPane);
                                                                }
                                                                pContentPaneGrid->lpVtbl->Release(pContentPaneGrid);
                                                            }
                                                            pBorder->lpVtbl->Release(pBorder);
                                                        }
                                                        pContentRoot->lpVtbl->Release(pContentRoot);
                                                    }
                                                    pGrid->lpVtbl->Release(pGrid);
                                                }
                                                pRootContent->lpVtbl->Release(pRootContent);
                                            }
                                            pRootGrid->lpVtbl->Release(pRootGrid);
                                        }
                                        pSplitViewFrame->lpVtbl->Release(pSplitViewFrame);
                                    }
                                    pContentPresenter2->lpVtbl->Release(pContentPresenter2);
                                }
                                pFrame->lpVtbl->Release(pFrame);
                            }
                            pContentPresenter->lpVtbl->Release(pContentPresenter);
                        }
                        pStartSizingFramePanel->lpVtbl->Release(pStartSizingFramePanel);
                    }
                    pStartSizingFrame->lpVtbl->Release(pStartSizingFrame);
                }
                pVisualTreeHelperStatics->lpVtbl->Release(pVisualTreeHelperStatics);
            }
            WindowsDeleteString(hsVisualTreeHelperStatics);
        }
        pRootDependencyObject->lpVtbl->Release(pRootDependencyObject);
    }

    if (pCanvasStatics)
    {
        pCanvasStatics->lpVtbl->Release(pCanvasStatics);
    }
}

void LVT_StartDocked_DisableRecommendedSection(HWND hWnd, BOOL bApply)
{
    WCHAR wszDebug[MAX_PATH];
    HRESULT hr = S_OK;

    Windows_UI_Xaml_IDependencyObject* pRootDependencyObject = NULL;

    if (SUCCEEDED(hr))
    {
        HSTRING_HEADER hshWindowStatics;
        HSTRING hsWindowStatics = NULL;
        hr = WindowsCreateStringReference(L"Windows.UI.Xaml.Window", 22, &hshWindowStatics, &hsWindowStatics);
        if (SUCCEEDED(hr) && hsWindowStatics)
        {
            Windows_UI_Xaml_IWindowStatics* pWindowStatics = NULL;
            hr = RoGetActivationFactory(hsWindowStatics, &IID_Windows_UI_Xaml_IWindowStatics, &pWindowStatics);
            if (SUCCEEDED(hr))
            {
                Windows_UI_Xaml_IWindow* pWindow = NULL;
                hr = pWindowStatics->lpVtbl->get_Current(pWindowStatics, &pWindow);
                if (SUCCEEDED(hr))
                {
                    IInspectable* pUIElement = NULL;
                    hr = pWindow->lpVtbl->get_Content(pWindow, &pUIElement);
                    if (SUCCEEDED(hr))
                    {
                        hr = pUIElement->lpVtbl->QueryInterface(pUIElement, &IID_Windows_UI_Xaml_IDependencyObject, &pRootDependencyObject);

                        pUIElement->lpVtbl->Release(pUIElement);
                    }
                    pWindow->lpVtbl->Release(pWindow);
                }
                pWindowStatics->lpVtbl->Release(pWindowStatics);
            }
            WindowsDeleteString(hsWindowStatics);
        }
    }

    if (pRootDependencyObject)
    {
        HSTRING_HEADER hshVisualTreeHelperStatics;
        HSTRING hsVisualTreeHelperStatics = NULL;
        hr = WindowsCreateStringReference(L"Windows.UI.Xaml.Media.VisualTreeHelper", 38, &hshVisualTreeHelperStatics, &hsVisualTreeHelperStatics);
        if (SUCCEEDED(hr) && hsVisualTreeHelperStatics)
        {
            Windows_UI_Xaml_IVisualTreeHelperStatics* pVisualTreeHelperStatics = NULL;
            hr = RoGetActivationFactory(hsVisualTreeHelperStatics, &IID_Windows_UI_Xaml_IVisualTreeHelperStatics, &pVisualTreeHelperStatics);
            if (SUCCEEDED(hr))
            {
                Windows_UI_Xaml_IDependencyObject* pStartSizingFrame = LVT_FindChildByClassName(pRootDependencyObject, pVisualTreeHelperStatics, L"StartDocked.StartSizingFrame", NULL);
                if (pStartSizingFrame)
                {
                    Windows_UI_Xaml_IDependencyObject* pStartSizingFramePanel = LVT_FindChildByClassName(pStartSizingFrame, pVisualTreeHelperStatics, L"StartDocked.StartSizingFramePanel", NULL);
                    if (pStartSizingFramePanel)
                    {
                        Windows_UI_Xaml_IDependencyObject* pContentPresenter = LVT_FindChildByClassName(pStartSizingFramePanel, pVisualTreeHelperStatics, L"Windows.UI.Xaml.Controls.ContentPresenter", NULL);
                        if (pContentPresenter)
                        {
                            Windows_UI_Xaml_IDependencyObject* pFrame = LVT_FindChildByClassName(pContentPresenter, pVisualTreeHelperStatics, L"Windows.UI.Xaml.Controls.Frame", NULL);
                            if (pFrame)
                            {
                                Windows_UI_Xaml_IDependencyObject* pContentPresenter2 = LVT_FindChildByClassName(pFrame, pVisualTreeHelperStatics, L"Windows.UI.Xaml.Controls.ContentPresenter", NULL);
                                if (pContentPresenter2)
                                {
                                    Windows_UI_Xaml_IDependencyObject* pLauncherFrame = LVT_FindChildByClassName(pContentPresenter2, pVisualTreeHelperStatics, L"StartDocked.LauncherFrame", NULL);
                                    if (pLauncherFrame)
                                    {
                                        Windows_UI_Xaml_IDependencyObject* pRootGrid = LVT_FindChildByName(pLauncherFrame, pVisualTreeHelperStatics, L"RootGrid");
                                        if (pRootGrid)
                                        {
                                            Windows_UI_Xaml_IDependencyObject* pRootContent = LVT_FindChildByName(pRootGrid, pVisualTreeHelperStatics, L"RootContent");
                                            if (pRootContent)
                                            {
                                                Windows_UI_Xaml_IDependencyObject* pMainContent = LVT_FindChildByName(pRootContent, pVisualTreeHelperStatics, L"MainContent");
                                                if (pMainContent)
                                                {
                                                    Windows_UI_Xaml_IDependencyObject* pUndockedRoot = LVT_FindChildByName(pMainContent, pVisualTreeHelperStatics, L"UndockedRoot");
                                                    if (pUndockedRoot)
                                                    {
                                                        Windows_UI_Xaml_IDependencyObject* pStartInnerFrame = LVT_FindChildByClassName(pUndockedRoot, pVisualTreeHelperStatics, L"StartMenu.StartInnerFrame", NULL);
                                                        if (pStartInnerFrame)
                                                        {
                                                            Windows_UI_Xaml_IDependencyObject* pFrameRoot = LVT_FindChildByName(pStartInnerFrame, pVisualTreeHelperStatics, L"FrameRoot");
                                                            if (pFrameRoot)
                                                            {
                                                                Windows_UI_Xaml_IDependencyObject* pTopLevelRoot = LVT_FindChildByName(pFrameRoot, pVisualTreeHelperStatics, L"TopLevelRoot");
                                                                if (pTopLevelRoot)
                                                                {
                                                                    Windows_UI_Xaml_IDependencyObject* pStartMenuPinnedList = LVT_FindChildByName(pTopLevelRoot, pVisualTreeHelperStatics, L"StartMenuPinnedList");
                                                                    if (pStartMenuPinnedList)
                                                                    {
                                                                        Windows_UI_Xaml_IFrameworkElement* pFrameworkElement = NULL;
                                                                        pStartMenuPinnedList->lpVtbl->QueryInterface(pStartMenuPinnedList, &IID_Windows_UI_Xaml_IFrameworkElement, &pFrameworkElement);
                                                                        if (pFrameworkElement)
                                                                        {
                                                                            if (bApply)
                                                                            {
                                                                                pFrameworkElement->lpVtbl->put_Height(pFrameworkElement, 510.0);
                                                                            }
                                                                            else
                                                                            {
                                                                                pFrameworkElement->lpVtbl->put_Height(pFrameworkElement, 252.0);
                                                                            }
                                                                            pFrameworkElement->lpVtbl->Release(pFrameworkElement);
                                                                        }
                                                                        Windows_UI_Xaml_IDependencyObject* pRoot = LVT_FindChildByName(pStartMenuPinnedList, pVisualTreeHelperStatics, L"Root");
                                                                        if (pRoot)
                                                                        {
                                                                            Windows_UI_Xaml_IDependencyObject* pPinnedListPipsPager = LVT_FindChildByName(pRoot, pVisualTreeHelperStatics, L"PinnedListPipsPager");
                                                                            if (pPinnedListPipsPager)
                                                                            {
                                                                                Windows_UI_Xaml_IUIElement* pIUIElement = NULL;
                                                                                pPinnedListPipsPager->lpVtbl->QueryInterface(pPinnedListPipsPager, &IID_Windows_UI_Xaml_IUIElement, &pIUIElement);
                                                                                if (pIUIElement)
                                                                                {
                                                                                    if (bApply)
                                                                                    {
                                                                                        pIUIElement->lpVtbl->put_Visibility(pIUIElement, Windows_UI_Xaml_Visibility_Collapsed);
                                                                                    }
                                                                                    else
                                                                                    {
                                                                                        pIUIElement->lpVtbl->put_Visibility(pIUIElement, Windows_UI_Xaml_Visibility_Visible);
                                                                                    }
                                                                                    pIUIElement->lpVtbl->Release(pIUIElement);
                                                                                }
                                                                                pPinnedListPipsPager->lpVtbl->Release(pPinnedListPipsPager);
                                                                            }
                                                                            pRoot->lpVtbl->Release(pRoot);
                                                                        }
                                                                        pStartMenuPinnedList->lpVtbl->Release(pStartMenuPinnedList);
                                                                    }
                                                                    pTopLevelRoot->lpVtbl->Release(pTopLevelRoot);
                                                                }
                                                                pFrameRoot->lpVtbl->Release(pFrameRoot);
                                                            }
                                                            pStartInnerFrame->lpVtbl->Release(pStartInnerFrame);
                                                        }
                                                        pUndockedRoot->lpVtbl->Release(pUndockedRoot);
                                                    }
                                                    pMainContent->lpVtbl->Release(pMainContent);
                                                }
                                                pRootContent->lpVtbl->Release(pRootContent);
                                            }
                                            pRootGrid->lpVtbl->Release(pRootGrid);
                                        }
                                        pLauncherFrame->lpVtbl->Release(pLauncherFrame);
                                    }
                                    pContentPresenter2->lpVtbl->Release(pContentPresenter2);
                                }
                                pFrame->lpVtbl->Release(pFrame);
                            }
                            pContentPresenter->lpVtbl->Release(pContentPresenter);
                        }
                        pStartSizingFramePanel->lpVtbl->Release(pStartSizingFramePanel);
                    }
                    pStartSizingFrame->lpVtbl->Release(pStartSizingFrame);
                }
                pVisualTreeHelperStatics->lpVtbl->Release(pVisualTreeHelperStatics);
            }
            WindowsDeleteString(hsVisualTreeHelperStatics);
        }
        pRootDependencyObject->lpVtbl->Release(pRootDependencyObject);
    }
}