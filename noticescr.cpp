// noticescr.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <crtdbg.h>
#include "noticescr.h"

#define MAX_LOADSTRING 100

// グローバル変数: _tWinMainで定義、WndProcで使用
static HINSTANCE hInst;                // 現在のインターフェイス
static TCHAR szTitle[MAX_LOADSTRING];  // タイトル バーのテキスト
// グローバル変数: _tWinMainで定義、Aboutで使用
static WCHAR *gProgPath;               // このプログラムファイルのフルパス

// このコード モジュールに含まれる関数の宣言を転送します:
static ATOM MyRegisterClass(HINSTANCE hInstance, TCHAR szWindowClass[]);
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	TCHAR szWindowClass[MAX_LOADSTRING];  // メイン ウィンドウ クラス名
	HWND hWnd;

    _CrtSetReportMode(_CRT_ASSERT, 0);

	gProgPath = __wargv[0];

	MSG msg;
	HACCEL hAccelTable;

	// グローバル文字列を初期化しています。
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_NOTICESCR, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance, szWindowClass);

	// 動かすインスタンスは一つだけ
	hWnd = FindWindow(szWindowClass, NULL);
	if (hWnd) {
		MessageBox(NULL, L"このプログラムは既に動いています。\nタスクトレイを確認してください。", szTitle, MB_OK);
		SetForegroundWindow(hWnd);
		return 0;
	}

	hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

	// アプリケーションの初期化を実行します
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
					CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
					NULL, NULL, hInstance, NULL);
	if (!hWnd) {
		return 0;
	}

#if 0
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
#endif

	// ショートカットキー読み込み
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NOTICESCR));

	// メイン メッセージ ループ:
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
//  コメント:
//    この関数および使い方は、'RegisterClassEx' 関数が追加された
//    Windows 95 より前の Win32 システムと互換させる場合にのみ必要です。
//    アプリケーションが、関連付けられた
//    正しい形式の小さいアイコンを取得できるようにするには、
//    この関数を呼び出してください。
//
ATOM MyRegisterClass(HINSTANCE hInstance, TCHAR szWindowClass[])
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LARGE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_NOTICESCR);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

// 黒丸サイン用のウィンドウを作成する
static HWND createSign(HWND hwnd, HINSTANCE hInstance)
{
	// ウィンドウクラスの属性を設定する
	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.style = 0;                         // スタイル
	wc.lpfnWndProc = DefWindowProc;
	wc.cbClsExtra = 0;                    // 拡張情報１
	wc.cbWndExtra = 0;                    // 拡張情報２
	wc.hInstance = hInstance;
	wc.hIcon = 0;
	wc.hIconSm = wc.hIcon;                // 子アイコン
	wc.hCursor = LoadCursor(NULL, IDC_NO);
	wc.hbrBackground = CreateSolidBrush(RGB(0x00, 0x00, 0x00));
	wc.lpszMenuName = NULL;               // メニュー名
	wc.lpszClassName = L"Sign";           // ウィンドウクラス名
	
	// ウィンドウクラスを登録する
	if(RegisterClassEx(&wc) == 0) {
		return 0;
	}

	// 現在の解像度を得る
	static DEVMODE devMode;
	int scrwidth, scrheight;
	if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode)) {
		scrwidth = devMode.dmPelsWidth;
		scrheight = devMode.dmPelsHeight;
	} else {
		// 万一解像度が取得できない場合の適当な値
		scrwidth = 640;
		scrheight = 480;
	}

	// ウィンドウを作成する
	HWND hWndSign = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST,
		wc.lpszClassName, L"", WS_POPUP,
		0, 0, scrwidth, scrheight,
		hwnd, NULL, hInstance, NULL);

	return hWndSign;
}

// タスクトレイ上のアイコンの右クリックメニューを作成する
void createMenu(HWND hwnd, HINSTANCE hInstance, TCHAR *title, HMENU *phMenu, NOTIFYICONDATA *pnid)
{
	*phMenu = CreatePopupMenu();
	MENUITEMINFO menuiteminfo[2];
	menuiteminfo[0].cbSize = sizeof(*menuiteminfo);
	menuiteminfo[0].fMask = MIIM_STRING | MIIM_ID;
	menuiteminfo[0].wID = IDD_ABOUTBOX;
	menuiteminfo[0].dwTypeData = L"このソフトウェアについて";
	menuiteminfo[0].cch = wcslen(menuiteminfo[0].dwTypeData);
	InsertMenuItem(*phMenu, 0, true, &menuiteminfo[0]);
	menuiteminfo[1].cbSize = sizeof(*menuiteminfo);
	menuiteminfo[1].fMask = MIIM_STRING | MIIM_ID;
	menuiteminfo[1].wID = IDM_EXIT;
	menuiteminfo[1].dwTypeData = L"終了";
	menuiteminfo[1].cch = wcslen(menuiteminfo[1].dwTypeData);
	InsertMenuItem(*phMenu, 1, true, &menuiteminfo[1]);

	pnid->cbSize = sizeof(NOTIFYICONDATA);
	pnid->hWnd = hwnd;
	pnid->uID = 0;
	pnid->uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	pnid->uCallbackMessage = WM_TRAYICONMESSAGE;
	pnid->hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wcsncpy_s(pnid->szTip, title, _TRUNCATE);
	Shell_NotifyIcon(NIM_ADD, pnid);
}


//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:  メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND	- アプリケーション メニューの処理
//  WM_PAINT	- メイン ウィンドウの描画
//  WM_DESTROY	- 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HMENU hMenu;
	static NOTIFYICONDATA nid;

	switch (message) {
	case WM_CREATE:
		{
			createMenu(hWnd, hInst, szTitle, &hMenu, &nid);

			HWND hWndSign = createSign(hWnd, hInst);
			if (! hWndSign) {
				MessageBox(hWnd, L"ウィンドウ作成のエラーです。終了します。", L"エラー", MB_OK);
				DestroyWindow(hWnd);
			}

			if (! startTimer(hWnd, hWndSign)) {
				MessageBox(hWnd, L"タイマー起動のエラーです。終了します。", L"エラー", MB_OK);
				DestroyWindow(hWnd);
			}
		}
		break;

	case WM_COMMAND:
		{
			int wmId    = LOWORD(wParam);
			int wmEvent = HIWORD(wParam);
			// 選択されたメニューの解析:
			switch (wmId) {
			case IDD_ABOUTBOX:
				EnableMenuItem(hMenu, 0, MF_BYPOSITION | MF_GRAYED);  // Aboutダイアログメニューを重ねて呼べないようにする
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				EnableMenuItem(hMenu, 0, MF_BYPOSITION | MF_ENABLED); // Aboutダイアログメニューを元に戻す
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: 描画コードをここに追加してください...
			EndPaint(hWnd, &ps);
		}
		break;

	case WM_TRAYICONMESSAGE:
		switch(lParam) {
		case WM_RBUTTONDOWN: // タスクトレイアイコン上で右クリック
			POINT pt;
			GetCursorPos(&pt);
			SetForegroundWindow(hWnd);  // これがないと、ポップアップメニューが消えなくなる
			TrackPopupMenuEx(hMenu, TPM_LEFTALIGN, pt.x, pt.y, hWnd, 0);
			break;
		}
		break;

	case WM_DESTROY:
		DestroyMenu(hMenu);
		Shell_NotifyIcon(NIM_DELETE, &nid);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// バージョン情報ボックスのメッセージ ハンドラです。
INT_PTR CALLBACK About(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
	case WM_INITDIALOG:
		{
			int bufSize = GetFileVersionInfoSize(gProgPath, 0);
			if (bufSize > 0) {
				void *buf = (void *)malloc(sizeof (BYTE) * bufSize);
				if (buf != NULL) {
					GetFileVersionInfo(gProgPath, 0, bufSize, buf);

					void *str;
					unsigned int strSize;

					VerQueryValue(buf, L"\\StringFileInfo\\0411fde9\\ProductName", (LPVOID *)&str, &strSize);
					SetWindowText(hwndDlg, (WCHAR *)str);
					SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT2), (WCHAR *)str);

					VerQueryValue(buf, L"\\StringFileInfo\\0411fde9\\FileVersion", (LPVOID *)&str, &strSize);
					SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT1), (WCHAR *)str);

					VerQueryValue(buf, L"\\StringFileInfo\\0411fde9\\LegalCopyright", (LPVOID *)&str, &strSize);
					SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT3), (WCHAR *)str);

					free(buf);
				}
			}
		}
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hwndDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
