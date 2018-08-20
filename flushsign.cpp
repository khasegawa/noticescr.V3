#include "stdafx.h"
#include "noticescr.h"

#define ID_TIMER  1

#define INTERVAL    1000 // (ms)
#define ALERTTIME   60   // (s)

static HWND g_hWndSign;  // 「サイン」のウィンドウハンドル

static void CALLBACK TimerFunc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);

BOOL startTimer(HWND hwnd, HWND hWndSign)
{
	g_hWndSign = hWndSign;

	if (SetTimer(hwnd, (UINT)ID_TIMER, (UINT)INTERVAL, (TIMERPROC)TimerFunc) == 0) {
		return FALSE;
	}

	return TRUE;
}

// 残り時間監視用のタイマー関数
static void CALLBACK TimerFunc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	BOOL ret;

	BOOL active;
	ret = SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, &active, 0);
	if (! ret || ! active) {
		// スクリーンセーバが設定されていない → 何もしない
		return;
	}

	BOOL running;
	ret = SystemParametersInfo(SPI_GETSCREENSAVERRUNNING, 0, &running, 0);
	if (! ret || running) {
		// スクリーンセーバ動作中 → 何もしない
		return;
	}

	int waittime;
	ret = SystemParametersInfo(SPI_GETSCREENSAVETIMEOUT, 0, &waittime, 0);
	if (! ret) {
		// スクリーンセーバ待機時間が取得できない
		return;
	}

	LASTINPUTINFO lii;
	lii.cbSize = sizeof (LASTINPUTINFO);
	ret = GetLastInputInfo(&lii);
	if (! ret) {
		// ユーザ最終操作時刻が取得できない
		return;
	}

	DWORD idol = (GetTickCount() - lii.dwTime) / 1000; // ms -> s
	int rest = waittime - idol;

	debugPrintInt(rest);

	if (rest >= ALERTTIME) {
		ShowWindow(g_hWndSign, SW_HIDE);
	} else {
		int ratio = (ALERTTIME - rest) * 255 / ALERTTIME;
		SetLayeredWindowAttributes(g_hWndSign, 0, ratio, LWA_ALPHA);
		ShowWindow(g_hWndSign, SW_SHOWNA);
	}
}
