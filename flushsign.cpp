#include "stdafx.h"
#include "noticescr.h"

#define ID_TIMER  1

#define INTERVAL    1000 // (ms)
#define ALERTTIME   60   // (s)

static HWND g_hWndSign;  // �u�T�C���v�̃E�B���h�E�n���h��

static void CALLBACK TimerFunc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);

BOOL startTimer(HWND hwnd, HWND hWndSign)
{
	g_hWndSign = hWndSign;

	if (SetTimer(hwnd, (UINT)ID_TIMER, (UINT)INTERVAL, (TIMERPROC)TimerFunc) == 0) {
		return FALSE;
	}

	return TRUE;
}

// �c�莞�ԊĎ��p�̃^�C�}�[�֐�
static void CALLBACK TimerFunc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	BOOL ret;

	BOOL active;
	ret = SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, &active, 0);
	if (! ret || ! active) {
		// �X�N���[���Z�[�o���ݒ肳��Ă��Ȃ� �� �������Ȃ�
		return;
	}

	BOOL running;
	ret = SystemParametersInfo(SPI_GETSCREENSAVERRUNNING, 0, &running, 0);
	if (! ret || running) {
		// �X�N���[���Z�[�o���쒆 �� �������Ȃ�
		return;
	}

	int waittime;
	ret = SystemParametersInfo(SPI_GETSCREENSAVETIMEOUT, 0, &waittime, 0);
	if (! ret) {
		// �X�N���[���Z�[�o�ҋ@���Ԃ��擾�ł��Ȃ�
		return;
	}

	LASTINPUTINFO lii;
	lii.cbSize = sizeof (LASTINPUTINFO);
	ret = GetLastInputInfo(&lii);
	if (! ret) {
		// ���[�U�ŏI���쎞�����擾�ł��Ȃ�
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
