// StrProg.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "StrProg.h"
#include "..\..\StrLib\StrLib\StrLib.h"

#define MAX_LOADSTRING 100

typedef struct
{
	HDC hdc;
	int xText;
	int yText;
	int xStart;
	int yStart;
	int xIncr;
	int yIncr;
	int xMax;
	int yMax;
}
CBPARAM;

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

TCHAR szAppName[] = TEXT("StrProg");
TCHAR szString[MAX_LENGTH + 1];

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_STRPROG, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_STRPROG));

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_STRPROG));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_STRPROG);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		SendDlgItemMessage(hDlg, IDC_STRING, EM_LIMITTEXT, MAX_LENGTH, 0);
		return TRUE;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
			GetDlgItemText(hDlg, IDC_STRING, szString, MAX_LENGTH);
			EndDialog(hDlg, TRUE);
			return TRUE;

		case IDCANCEL:
			EndDialog(hDlg, FALSE);
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CALLBACK GetStrCallBack(PTSTR pString, CBPARAM * pcbp)
{
	TextOut(pcbp->hdc, pcbp->xText, pcbp->yText,
		pString, lstrlen(pString));

	if ((pcbp->yText += pcbp->yIncr) > pcbp->yMax)
	{
		pcbp->yText = pcbp->yStart;
		if ((pcbp->xText += pcbp->xIncr) > pcbp->xMax)
			return FALSE;
	}
	return TRUE;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM
	lParam)
{
	static HINSTANCE hInst;
	static int cxChar, cyChar, cxClient, cyClient;
	static UINT iDataChangeMsg;
	CBPARAM cbparam;
	HDC hdc;
	PAINTSTRUCT ps;
	TEXTMETRIC tm;

	switch (message)
	{
	case WM_CREATE:
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxChar = (int)tm.tmAveCharWidth;
		cyChar = (int)(tm.tmHeight + tm.tmExternalLeading);
		ReleaseDC(hwnd, hdc);
		// Register message for notifying instances of data changes
		iDataChangeMsg = RegisterWindowMessage(TEXT("StrProgDataChange"));
		return 0;
	case WM_COMMAND:
		switch (wParam)
		{
		case IDM_ENTER:
			if (DialogBox(hInst, TEXT("EnterDlg"), hwnd, &DlgProc))
			{
				if (AddString(szString))
					PostMessage(HWND_BROADCAST, iDataChangeMsg, 0, 0);
				else
					MessageBeep(0);
			}
			break;
		case IDM_DELETE:
			if (DialogBox(hInst, TEXT("DeleteDlg"), hwnd, &DlgProc))
			{
				if (DeleteString(szString))
					PostMessage(HWND_BROADCAST, iDataChangeMsg, 0, 0);
				else
					MessageBeep(0);
			}
			break;
		}
		return 0;

	case WM_SIZE:
		cxClient = (int)LOWORD(lParam);
		cyClient = (int)HIWORD(lParam);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		cbparam.hdc = hdc;
		cbparam.xText = cbparam.xStart = cxChar;
		cbparam.yText = cbparam.yStart = cyChar;
		cbparam.xIncr = cxChar * MAX_LENGTH;
		cbparam.yIncr = cyChar;
		cbparam.xMax = cbparam.xIncr * (1 + cxClient / cbparam.xIncr);
		cbparam.yMax = cyChar * (cyClient / cyChar - 1);

		GetStrings((GETSTRCB)GetStrCallBack, (PVOID)&cbparam);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		if (message == iDataChangeMsg)
			InvalidateRect(hwnd, NULL, TRUE);
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}


