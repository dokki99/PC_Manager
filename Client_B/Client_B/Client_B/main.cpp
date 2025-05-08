#include <windows.h>
#include "resource.h"

#define MAX_SEAT 30

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK SeatDlgProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI Recv_Thread(LPVOID);

void Split_C_T(TCHAR*, TCHAR*);
void Update_Seat(TCHAR*);
void Change_Screen();
void Send_Text(const char*, const char*);

HINSTANCE g_hInst;
LPCTSTR MainClass = TEXT("PC Client");

HWND hWndMain;		// ���� ȭ�� �ڵ�

// ȭ��ó�� ���� ����////////////////////////////////////////

HBITMAP hBit;

/////////////////////////////////////////////////////////////

// ���ϰ��� ����/////////////////////////////////////////////

SOCKET clientsock = 0;

sockaddr_in addr_client;
int addrlen_clt = sizeof(sockaddr);
int nReturn;
WSADATA wsadata;

DWORD ThreadID;
HANDLE hThread;

/////////////////////////////////////////////////////////////

// �������� ����/////////////////////////////////////////////

BOOL CONN_ST = FALSE;						// ���� ���� ����
const char g_szlpAddress[17] = "192.168.0.217";
const unsigned short g_uPort = 7878;		// ��Ʈ �ѹ�
const int buflen = 4096;					// ���� ũ��
TCHAR buf[buflen];

/////////////////////////////////////////////////////////////

// ��Ʈ�� ���� ����

HWND SEAT_BUTTON[MAX_SEAT];
HWND JOIN_BTN, LOGIN_BTN, LOGOUT_BTN, FINDID_BTN, FINDPW_BTN, SELECTION_BTN;
HWND CHARGE_BTN0, CHARGE_BTN1, CHARGE_BTN2, CHARGE_BTN3, CHARGE_BTN4, CHARGE_BTN5, CHARGE_BTN6, CHARGE_BTN7, CHARGE_BTN8, CHARGE_BTN9;
HWND ID_EDIT, PW_EDIT;
HWND STATIC_TEXT_ID, STATIC_TEXT_PW, STATIC_TEXT_H_USER, STATIC_TEXT_REST_TIME;

// � ���� ����

TCHAR hUser_ID[30];		// ���� ���� ����

int View_State;			// ȭ�����

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpszCmdParam, int nCmdShow) {
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = CreateSolidBrush(RGB(218, 220, 214));
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_ASTERISK);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = MainWndProc;
	WndClass.lpszClassName = MainClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(MainClass, MainClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;

}

/*--------------------------------------------------------
 MainWndProc: ���� ������ ���ν���
--------------------------------------------------------*/
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	TCHAR TEXT[256],B_num[3],ID[30], PW[30];
	static HDC hdc;
	static HBRUSH hBrush = NULL;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		hBrush = CreateSolidBrush(RGB(218, 220, 214));
		//// ���� ���� ���� ���̵� �ʱ�ȭ
		//lstrcpy(hUser_ID, "");

		/*
		// ���� �ʱ�ȭ (���� ���̺귯�� ����, ���� �ý��� ���� ����)//////////////////////////////
		nReturn = WSAStartup(WORD(2.0), &wsadata);

		// ���� ���� (IPv4: AF_INET | IPv6: AF_INET6 , ���� ��� Ÿ��, �������� ����)
		clientsock = socket(AF_INET, SOCK_STREAM, 0);

		addr_client.sin_family = AF_INET;
		addr_client.sin_addr.s_addr = inet_addr(g_szlpAddress);
		addr_client.sin_port = htons(g_uPort);

		// ���� ����
		nReturn = connect(clientsock, (sockaddr*)&addr_client, addrlen_clt);

		// ���� ������ ����
		hThread = CreateThread(NULL, 0, Recv_Thread, &clientsock, 0, &ThreadID);
		CloseHandle(hThread);

		if (nReturn) {
			MessageBox(hWndMain, "�������", "����", MB_OK);
		}
		*/

		//////////////////////////////////////////////////////////////////////////////////////////

		// ID / PW �Է�â �����
		STATIC_TEXT_ID = CreateWindow("static", "ID ", WS_CHILD | WS_VISIBLE | SS_CENTER, 15, 12, 40, 30, hWndMain, (HMENU)1, g_hInst, NULL);
		STATIC_TEXT_PW = CreateWindow("static", "PW ", WS_CHILD | WS_VISIBLE | SS_CENTER, 15, 52, 40, 30, hWndMain, (HMENU)2, g_hInst, NULL);

		ID_EDIT = CreateWindow("edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 55, 10, 200, 25, hWndMain, (HMENU)3, g_hInst, NULL);
		PW_EDIT = CreateWindow("edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 55, 50, 200, 25, hWndMain, (HMENU)4, g_hInst, NULL);

		// ȸ������/�α���/�¼����� ��ư ����
		LOGIN_BTN = CreateWindow("button", "�α���", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 15, 80, 240, 30, hWndMain, (HMENU)UTIL_BTN, g_hInst, NULL);
		FINDID_BTN = CreateWindow("button", "IDã��", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 15, 115, 80, 30, hWndMain, (HMENU)(UTIL_BTN + 1), g_hInst, NULL);
		FINDPW_BTN = CreateWindow("button", "PWã��", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 95, 115, 80, 30, hWndMain, (HMENU)(UTIL_BTN + 2), g_hInst, NULL);
		JOIN_BTN = CreateWindow("button", "ȸ�� ����", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 175, 115, 80, 30, hWndMain, (HMENU)(UTIL_BTN + 3), g_hInst, NULL);
		
		// Priceȭ��(PricePage{ 0,0,900,700 }) �ڵ����
		
		// ���� ��ư �����
		CHARGE_BTN0 = CreateWindow("button", "1�ð�\n1,000��", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE, 100, 70, 130, 80, hWndMain, (HMENU)(CHARGE_BTN), g_hInst, NULL);
		CHARGE_BTN1 = CreateWindow("button", "2�ð�\n2,000��", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE, 280, 70, 130, 80, hWndMain, (HMENU)(CHARGE_BTN + 1), g_hInst, NULL);
		CHARGE_BTN2 = CreateWindow("button", "3�ð�\n3,000��", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE, 100, 170, 130, 80, hWndMain, (HMENU)(CHARGE_BTN + 2), g_hInst, NULL);
		CHARGE_BTN3 = CreateWindow("button", "5�ð�\n5,000��", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE, 280, 170, 130, 80, hWndMain, (HMENU)(CHARGE_BTN + 3), g_hInst, NULL);
		CHARGE_BTN4 = CreateWindow("button", "10�ð�\n10,000��", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE, 100, 270, 130, 80, hWndMain, (HMENU)(CHARGE_BTN + 4), g_hInst, NULL);
		CHARGE_BTN5 = CreateWindow("button", "17�ð�\n15,000��", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE, 280, 270, 130, 80, hWndMain, (HMENU)(CHARGE_BTN + 5), g_hInst, NULL);
		CHARGE_BTN6 = CreateWindow("button", "25�ð�\n20,000��", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE, 100, 370, 130, 80, hWndMain, (HMENU)(CHARGE_BTN + 6), g_hInst, NULL);
		CHARGE_BTN7 = CreateWindow("button", "35�ð�\n30,000��", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE, 280, 370, 130, 80, hWndMain, (HMENU)(CHARGE_BTN + 7), g_hInst, NULL);
		CHARGE_BTN8 = CreateWindow("button", "50�ð�\n40,000��", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE, 100, 470, 130, 80, hWndMain, (HMENU)(CHARGE_BTN + 8), g_hInst, NULL);
		CHARGE_BTN9 = CreateWindow("button", "60�ð�\n50,000��", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE, 280, 470, 130, 80, hWndMain, (HMENU)(CHARGE_BTN + 9), g_hInst, NULL);

		SELECTION_BTN = CreateWindow("button", "�¼� ����", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 700, 550, 100, 50, hWndMain, (HMENU)(UTIL_BTN + 4), g_hInst, NULL);
		LOGOUT_BTN = CreateWindow("button", "�α� �ƿ�", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 600, 550, 100, 50, hWndMain, (HMENU)(UTIL_BTN + 5), g_hInst, NULL);
		STATIC_TEXT_H_USER = CreateWindow("static", "", WS_CHILD | WS_VISIBLE | SS_LEFT, 700, 50, 200, 30, hWndMain, (HMENU)5, g_hInst, NULL);
		STATIC_TEXT_REST_TIME = CreateWindow("static", "", WS_CHILD | WS_VISIBLE | SS_LEFT, 700, 100, 300, 30, hWndMain, (HMENU)6, g_hInst, NULL);

		// �ʱ�ȭ���� LoginPage�� Screen ����
		View_State = 0;
		Change_Screen();
		InvalidateRect(hWndMain, NULL, TRUE);

		return 0;
	case WM_CTLCOLORSTATIC:
		SetBkMode((HDC)wParam, TRANSPARENT);

		return (INT_PTR)hBrush;
	case WM_PAINT:
		hdc = BeginPaint(hWndMain, &ps);

		if (View_State == 1) {
			Rectangle(hdc, 60, 50, 450, 570);
		}

		EndPaint(hWndMain, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case UTIL_BTN:
			// �α���
			GetWindowText(ID_EDIT, ID, 30);
			GetWindowText(PW_EDIT, PW, 30);

			if (lstrcmp(ID, "") != 0 && lstrcmp(PW, "") != 0) {
				
				//lstrcpy(TEXT, hUser_ID);
				//lstrcat(TEXT, "-");
				//lstrcat(TEXT, hUser_PW);
				//Send_Text("C00", TEXT);

				lstrcpy(hUser_ID, ID);

				lstrcpy(TEXT, "������� ");
				lstrcat(TEXT, hUser_ID);
				lstrcat(TEXT, "��");
				SetWindowText(STATIC_TEXT_H_USER, TEXT);

				lstrcpy(TEXT, "�ܿ� �ð�: ");
				lstrcat(TEXT, "00:00");

				SetWindowText(STATIC_TEXT_REST_TIME, TEXT);

				SetWindowText(ID_EDIT, "");
				SetWindowText(PW_EDIT, "");
				lstrcpy(ID, "");
				lstrcpy(PW, "");
				
				// ����ȭ���� PricePage�� Screen ����
				View_State = 1;
				Change_Screen();
			}
			break;
		case UTIL_BTN + 1:
			//IDã��

			break;
		case UTIL_BTN + 2:
			//PWã��

			break;
		case UTIL_BTN + 3:
			// ȸ�� ����
			GetWindowText(ID_EDIT, ID, 30);
			GetWindowText(PW_EDIT, PW, 30);

			if (lstrcmp(ID, "") != 0 && lstrcmp(PW, "") != 0) {
				SetWindowText(ID_EDIT, "");
				SetWindowText(PW_EDIT, "");
				// ����ȭ���� PricePage�� Screen ����
				View_State = 1;
				Change_Screen();
			}
			break;
		case UTIL_BTN + 4:
			// �ڸ� ����
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DLG_SEAT), hWnd, (DLGPROC)SeatDlgProc);
			break;
		default:
			if ((399 < LOWORD(wParam)) && (LOWORD(wParam) < 500)) {
				wsprintf(B_num, "%s-%d", hUser_ID, LOWORD(wParam) - 399);
				Send_Text("C06", B_num);
			}
			break;
		}
		return 0;
	case WM_DESTROY:
		// brush ����
		if (hBrush) DeleteObject(hBrush);
		// �α׾ƿ� �޼��� ������
		//nReturn = send(clientsock, "CFF", sizeof("CFF"), 0);
		// ���� ���� (��Ĺ ��ü)
		closesocket(clientsock);
		// ���� ����
		WSACleanup();
		//KillTimer(hWndMain, 1);

		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

BOOL CALLBACK SeatDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	TCHAR num[5];
	TCHAR s_num[3], TEXT[256];

	switch (iMessage) {
	case WM_INITDIALOG:

		Send_Text("C04", "");

		// �ӽ� �¼�����
		for (int k = 1; k <= 30; k++) {
			SEAT_BUTTON[k - 1] = GetDlgItem(hDlg, IDC_BTN_SEAT1 + k - 1);
		}

		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			EndDialog(hDlg, IDOK);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		default:
			if ((199 < LOWORD(wParam)) && (LOWORD(wParam) < 230)) {
				wsprintf(s_num, "%d", LOWORD(wParam) - 199);
				Send_Text("C01", s_num);
			}
			break;
		}
		return TRUE;
	}
	return FALSE;
}


/*
Format: CODE - TEXT ����{
	CODE[0] = (S / C) ���� / Ŭ���̾�Ʈ
	CODE[1~2] = (16����) 00 ~ FF �� 256���� ���°� ǥ�� ����
}

CODE:
	---------------(Server)---------------
	S00 = �α��� ����/���� �޽���
	S01 = ID ã�Ƽ� �޽���
	S02 = PW �ʱ�ȭ �����ֱ�
	S03 = ȸ������ ����/���� �޽���
	S04 = �¼� ����Ʈ �޽���
	S05 = �¼� ���� ����/���� �޽���
	S06 = �¼� ���ÿ��� ��� ����
	---------------(Client)---------------
	C00 = �α��� ��û �޽���
	C01 = ID ã�� ��û �޽���
	C02 = PW ã�� ��û �޽���
	C03 = ȸ������ ��û �޽���
	C04 = �¼� ����Ʈ ��û �޽���
	C05 = �¼� ���� �޽���
	C06 = �¼� ���ÿ��� ��� ���� �޼���
*/

/*--------------------------------------------------------
 Recv_Thread(LPVOID): ���� ������
--------------------------------------------------------*/
DWORD WINAPI Recv_Thread(LPVOID Param) {
	SOCKET* P = (SOCKET*)Param;
	TCHAR CODE[4], TEXT[256];

	for (;;) {
		// ���� ����ϱ� (���� ��ü, Ŭ���̾�Ʈ �ּ� ����, �ּ� ���� ���� ũ��)

		nReturn = recv(*P, buf, 1024, 0);

		if (nReturn != 0 && nReturn != SOCKET_ERROR) {
			Split_C_T(CODE, TEXT);

			if (lstrcmp(CODE, "S00") == 0) {
				// �α��� ����/���� ����
				if (lstrcmp(TEXT, "SUCCESS") == 0) {
					// ����ȭ���� PricePage�� Screen ����
					View_State = 1;
					Change_Screen();
				}
				else {
					MessageBox(hWndMain, "fail", "�˸�", MB_OK);
				}
			}
			else if (lstrcmp(CODE, "S01") == 0) {
				// �ڵ�����ȣ ������ ���̵� �ޱ�
			}
			else if (lstrcmp(CODE, "S02") == 0) {
				// �ڵ�����ȣ, ���̵� ������ PW�ʱ�ȭ
			}
			else if (lstrcmp(CODE, "S03") == 0) {
				// ȸ������ ����/���� ����
				if (lstrcmp(TEXT, "SUCCESS") == 0) {
					MessageBox(hWndMain, "success", "!", MB_OK);
				}
				else {
					MessageBox(hWndMain, "fail", "�˸�", MB_OK);
				}
			}
			else if (lstrcmp(CODE, "S04") == 0) {
				// �¼����� ������Ʈ
				Update_Seat(TEXT);
			}
			else if (lstrcmp(CODE, "S05") == 0) {
				// �¼����� ����/���� ����
				if (lstrcmp(TEXT, "SUCCESS") == 0) {
					MessageBox(hWndMain, "success", "!", MB_OK);
				}
				else {
					MessageBox(hWndMain, "fail", "�˸�", MB_OK);
				}
			}
			else if (lstrcmp(CODE, "S06") == 0) {
				// ������ �������
			}
		}
		else {
			MessageBox(hWndMain, "������ ������ ���������ϴ�!", "server", MB_OK);
			return 0;
		}
	}
	return 0;
}

/*--------------------------------------------------------
 Split_C_T(TCHAR* CODE, TCHAR* TEXT) : ���� ���� �ؽ�Ʈ��
 CODE�� TEXT�� �и��ϴ� �Լ�
-------------------------------------------------------- */
void Split_C_T(TCHAR* CODE, TCHAR* TEXT) {
	int i;

	lstrcpy(CODE, "");
	lstrcpy(TEXT, "");

	for (i = 0; i < lstrlen(buf); i++) {
		if (i < 3) {
			CODE[i] = buf[i];
		}
		else if (i == 3) {
			CODE[i] = '\0';
		}
		else if (i > 3) {
			TEXT[i - 4] = buf[i];
		}
	}

	TEXT[i - 4] = '\0';
}

/*--------------------------------------------------------
 Update_Seat(TCHAR* TEXT) : �¼� ��Ȳ �ֽ�ȭ
-------------------------------------------------------- */
void Update_Seat(TCHAR* TEXT) {
	int i;
	for (i = 0; i < MAX_SEAT + 1; i++) {
		if (TEXT[i] == '0') {
			if (!IsWindowEnabled(SEAT_BUTTON[i])) EnableWindow(SEAT_BUTTON[i], TRUE);
		}
		else if (TEXT[i] == '1') {
			if (IsWindowEnabled(SEAT_BUTTON[i])) EnableWindow(SEAT_BUTTON[i], FALSE);
		}
	}

}

/*--------------------------------------------------------
 Change_Screen() : ȭ�� ����
 ------------(View_State)------------
 0 : LoginPage
 1 : PrincePage
-------------------------------------------------------- */
void Change_Screen() {
	RECT LoginPage{ 0,0,280,195 };
	RECT PricePage{ 0,0,900,700 };
	
	if (View_State == 0) {
		SetWindowPos(hWndMain, NULL, LoginPage.left, LoginPage.top, LoginPage.right, LoginPage.bottom, SWP_NOMOVE | SWP_NOZORDER);
		SetWindowText(hWndMain, "�α���");
		ShowWindow(CHARGE_BTN0, SW_HIDE);
		ShowWindow(CHARGE_BTN1, SW_HIDE);
		ShowWindow(CHARGE_BTN2, SW_HIDE);
		ShowWindow(CHARGE_BTN3, SW_HIDE);
		ShowWindow(CHARGE_BTN4, SW_HIDE);
		ShowWindow(CHARGE_BTN5, SW_HIDE);
		ShowWindow(CHARGE_BTN6, SW_HIDE);
		ShowWindow(CHARGE_BTN7, SW_HIDE);
		ShowWindow(CHARGE_BTN8, SW_HIDE);
		ShowWindow(CHARGE_BTN9, SW_HIDE);
		ShowWindow(LOGIN_BTN, SW_SHOW);
		ShowWindow(FINDID_BTN, SW_SHOW);
		ShowWindow(FINDPW_BTN, SW_SHOW);
		ShowWindow(JOIN_BTN, SW_SHOW);
		ShowWindow(SELECTION_BTN, SW_HIDE);
		ShowWindow(STATIC_TEXT_ID, SW_SHOW);
		ShowWindow(STATIC_TEXT_PW, SW_SHOW);
		ShowWindow(ID_EDIT, SW_SHOW);
		ShowWindow(PW_EDIT, SW_SHOW);
		ShowWindow(STATIC_TEXT_H_USER, SW_HIDE);
		ShowWindow(STATIC_TEXT_REST_TIME, SW_HIDE);
	}
	else if (View_State == 1) {
		SetWindowPos(hWndMain, NULL, PricePage.left, PricePage.top, PricePage.right, PricePage.bottom, SWP_NOMOVE | SWP_NOZORDER);
		SetWindowText(hWndMain, "����ȭ��");
		ShowWindow(CHARGE_BTN0, SW_SHOW);
		ShowWindow(CHARGE_BTN1, SW_SHOW);
		ShowWindow(CHARGE_BTN2, SW_SHOW);
		ShowWindow(CHARGE_BTN3, SW_SHOW);
		ShowWindow(CHARGE_BTN4, SW_SHOW);
		ShowWindow(CHARGE_BTN5, SW_SHOW);
		ShowWindow(CHARGE_BTN6, SW_SHOW);
		ShowWindow(CHARGE_BTN7, SW_SHOW);
		ShowWindow(CHARGE_BTN8, SW_SHOW);
		ShowWindow(CHARGE_BTN9, SW_SHOW);
		ShowWindow(LOGIN_BTN, SW_HIDE);
		ShowWindow(FINDID_BTN, SW_HIDE);
		ShowWindow(FINDPW_BTN, SW_HIDE);
		ShowWindow(JOIN_BTN, SW_HIDE);
		ShowWindow(SELECTION_BTN, SW_SHOW);
		ShowWindow(STATIC_TEXT_ID, SW_HIDE);
		ShowWindow(STATIC_TEXT_PW, SW_HIDE);
		ShowWindow(ID_EDIT, SW_HIDE);
		ShowWindow(PW_EDIT, SW_HIDE);
		ShowWindow(STATIC_TEXT_H_USER, SW_SHOW);
		ShowWindow(STATIC_TEXT_REST_TIME, SW_SHOW);
	}
}

/*--------------------------------------------------------
 Send_Text(const char*, const char*, SOCKET*): ������
 �ؽ�Ʈ �۽��ϴ� �Լ�
--------------------------------------------------------*/
void Send_Text(const char* code, const char* Text) {
	TCHAR S_TEXT[300];

	if (lstrcmp(code, "C00") == 0) {
		wsprintf(S_TEXT, "%s-%s", code, Text);
		nReturn = send(clientsock, S_TEXT, sizeof(S_TEXT), 0);
	}
	else if (lstrcmp(code, "C01") == 0) {
		wsprintf(S_TEXT, "%s-%s", code, Text);
		nReturn = send(clientsock, S_TEXT, sizeof(S_TEXT), 0);
	}
	else if (lstrcmp(code, "C02") == 0) {
		wsprintf(S_TEXT, "%s-%s", code, Text);
		nReturn = send(clientsock, S_TEXT, sizeof(S_TEXT), 0);
	}
	else if (lstrcmp(code, "C03") == 0) {
		wsprintf(S_TEXT, "%s-%s", code, Text);
		nReturn = send(clientsock, S_TEXT, sizeof(S_TEXT), 0);
	}
	else if (lstrcmp(code, "C04") == 0) {
		wsprintf(S_TEXT, "%s-%s", code, Text);
		nReturn = send(clientsock, S_TEXT, sizeof(S_TEXT), 0);
	}
	else if (lstrcmp(code, "C05") == 0) {
		wsprintf(S_TEXT, "%s-%s", code, Text);
		nReturn = send(clientsock, S_TEXT, sizeof(S_TEXT), 0);
	}
	else if (lstrcmp(code, "C06") == 0) {
		wsprintf(S_TEXT, "%s-%s", code, Text);
		nReturn = send(clientsock, S_TEXT, sizeof(S_TEXT), 0);
	}
}