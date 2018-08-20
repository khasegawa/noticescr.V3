// noticescr.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <crtdbg.h>
#include "noticescr.h"

#define MAX_LOADSTRING 100

// �O���[�o���ϐ�: _tWinMain�Œ�`�AWndProc�Ŏg�p
static HINSTANCE hInst;                // ���݂̃C���^�[�t�F�C�X
static TCHAR szTitle[MAX_LOADSTRING];  // �^�C�g�� �o�[�̃e�L�X�g
// �O���[�o���ϐ�: _tWinMain�Œ�`�AAbout�Ŏg�p
static WCHAR *gProgPath;               // ���̃v���O�����t�@�C���̃t���p�X

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂�:
static ATOM MyRegisterClass(HINSTANCE hInstance, TCHAR szWindowClass[]);
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	TCHAR szWindowClass[MAX_LOADSTRING];  // ���C�� �E�B���h�E �N���X��
	HWND hWnd;

    _CrtSetReportMode(_CRT_ASSERT, 0);

	gProgPath = __wargv[0];

	MSG msg;
	HACCEL hAccelTable;

	// �O���[�o������������������Ă��܂��B
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_NOTICESCR, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance, szWindowClass);

	// �������C���X�^���X�͈����
	hWnd = FindWindow(szWindowClass, NULL);
	if (hWnd) {
		MessageBox(NULL, L"���̃v���O�����͊��ɓ����Ă��܂��B\n�^�X�N�g���C���m�F���Ă��������B", szTitle, MB_OK);
		SetForegroundWindow(hWnd);
		return 0;
	}

	hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B

	// �A�v���P�[�V�����̏����������s���܂�
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

	// �V���[�g�J�b�g�L�[�ǂݍ���
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NOTICESCR));

	// ���C�� ���b�Z�[�W ���[�v:
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//  �֐�: MyRegisterClass()
//
//  �ړI: �E�B���h�E �N���X��o�^���܂��B
//
//  �R�����g:
//    ���̊֐�����юg�����́A'RegisterClassEx' �֐����ǉ����ꂽ
//    Windows 95 ���O�� Win32 �V�X�e���ƌ݊�������ꍇ�ɂ̂ݕK�v�ł��B
//    �A�v���P�[�V�������A�֘A�t����ꂽ
//    �������`���̏������A�C�R�����擾�ł���悤�ɂ���ɂ́A
//    ���̊֐����Ăяo���Ă��������B
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

// ���ۃT�C���p�̃E�B���h�E���쐬����
static HWND createSign(HWND hwnd, HINSTANCE hInstance)
{
	// �E�B���h�E�N���X�̑�����ݒ肷��
	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.style = 0;                         // �X�^�C��
	wc.lpfnWndProc = DefWindowProc;
	wc.cbClsExtra = 0;                    // �g�����P
	wc.cbWndExtra = 0;                    // �g�����Q
	wc.hInstance = hInstance;
	wc.hIcon = 0;
	wc.hIconSm = wc.hIcon;                // �q�A�C�R��
	wc.hCursor = LoadCursor(NULL, IDC_NO);
	wc.hbrBackground = CreateSolidBrush(RGB(0x00, 0x00, 0x00));
	wc.lpszMenuName = NULL;               // ���j���[��
	wc.lpszClassName = L"Sign";           // �E�B���h�E�N���X��
	
	// �E�B���h�E�N���X��o�^����
	if(RegisterClassEx(&wc) == 0) {
		return 0;
	}

	// ���݂̉𑜓x�𓾂�
	static DEVMODE devMode;
	int scrwidth, scrheight;
	if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode)) {
		scrwidth = devMode.dmPelsWidth;
		scrheight = devMode.dmPelsHeight;
	} else {
		// ����𑜓x���擾�ł��Ȃ��ꍇ�̓K���Ȓl
		scrwidth = 640;
		scrheight = 480;
	}

	// �E�B���h�E���쐬����
	HWND hWndSign = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST,
		wc.lpszClassName, L"", WS_POPUP,
		0, 0, scrwidth, scrheight,
		hwnd, NULL, hInstance, NULL);

	return hWndSign;
}

// �^�X�N�g���C��̃A�C�R���̉E�N���b�N���j���[���쐬����
void createMenu(HWND hwnd, HINSTANCE hInstance, TCHAR *title, HMENU *phMenu, NOTIFYICONDATA *pnid)
{
	*phMenu = CreatePopupMenu();
	MENUITEMINFO menuiteminfo[2];
	menuiteminfo[0].cbSize = sizeof(*menuiteminfo);
	menuiteminfo[0].fMask = MIIM_STRING | MIIM_ID;
	menuiteminfo[0].wID = IDD_ABOUTBOX;
	menuiteminfo[0].dwTypeData = L"���̃\�t�g�E�F�A�ɂ���";
	menuiteminfo[0].cch = wcslen(menuiteminfo[0].dwTypeData);
	InsertMenuItem(*phMenu, 0, true, &menuiteminfo[0]);
	menuiteminfo[1].cbSize = sizeof(*menuiteminfo);
	menuiteminfo[1].fMask = MIIM_STRING | MIIM_ID;
	menuiteminfo[1].wID = IDM_EXIT;
	menuiteminfo[1].dwTypeData = L"�I��";
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


//  �֐�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �ړI:  ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND	- �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT	- ���C�� �E�B���h�E�̕`��
//  WM_DESTROY	- ���~���b�Z�[�W��\�����Ė߂�
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
				MessageBox(hWnd, L"�E�B���h�E�쐬�̃G���[�ł��B�I�����܂��B", L"�G���[", MB_OK);
				DestroyWindow(hWnd);
			}

			if (! startTimer(hWnd, hWndSign)) {
				MessageBox(hWnd, L"�^�C�}�[�N���̃G���[�ł��B�I�����܂��B", L"�G���[", MB_OK);
				DestroyWindow(hWnd);
			}
		}
		break;

	case WM_COMMAND:
		{
			int wmId    = LOWORD(wParam);
			int wmEvent = HIWORD(wParam);
			// �I�����ꂽ���j���[�̉��:
			switch (wmId) {
			case IDD_ABOUTBOX:
				EnableMenuItem(hMenu, 0, MF_BYPOSITION | MF_GRAYED);  // About�_�C�A���O���j���[���d�˂ČĂׂȂ��悤�ɂ���
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				EnableMenuItem(hMenu, 0, MF_BYPOSITION | MF_ENABLED); // About�_�C�A���O���j���[�����ɖ߂�
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
			// TODO: �`��R�[�h�������ɒǉ����Ă�������...
			EndPaint(hWnd, &ps);
		}
		break;

	case WM_TRAYICONMESSAGE:
		switch(lParam) {
		case WM_RBUTTONDOWN: // �^�X�N�g���C�A�C�R����ŉE�N���b�N
			POINT pt;
			GetCursorPos(&pt);
			SetForegroundWindow(hWnd);  // ���ꂪ�Ȃ��ƁA�|�b�v�A�b�v���j���[�������Ȃ��Ȃ�
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

// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���ł��B
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
