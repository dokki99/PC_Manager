#include <windows.h>
#include "resource.h"

#define MAX_SEAT 30

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK SeatDlgProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI Recv_Thread(LPVOID);

void Split_C_T(TCHAR*, TCHAR*);
void Update_Seat(TCHAR *);
void Change_Screen(int);
void Send_Text(const char*, const char*);

HINSTANCE g_hInst;
LPCTSTR MainClass = TEXT("좌석 선택");

HWND hWndMain;		// 메인 화면 핸들

// 화면처리 관련 변수////////////////////////////////////////

HBITMAP hBit;

/////////////////////////////////////////////////////////////

// 소켓관련 변수/////////////////////////////////////////////

SOCKET clientsock = 0;

sockaddr_in addr_client;
int addrlen_clt = sizeof(sockaddr);
int nReturn;
WSADATA wsadata;

DWORD ThreadID;
HANDLE hThread;

/////////////////////////////////////////////////////////////

// 서버관련 변수/////////////////////////////////////////////

BOOL CONN_ST = FALSE;						// 서버 오픈 상태
const char g_szlpAddress[17] = "192.168.0.217";
const unsigned short g_uPort = 7878;		// 포트 넘버
const int buflen = 4096;					// 버퍼 크기
TCHAR buf[buflen];

/////////////////////////////////////////////////////////////

// 컨트롤 관련 변수

HWND SEAT_BUTTON[MAX_SEAT];
HWND JOIN_BTN, LOGIN_BTN, SELECTION_BTN;
HWND ID_EDIT, PW_EDIT;
HWND STATIC_TEXT_ID, STATIC_TEXT_PW;

// 운영 관련 변수

TCHAR hUser_ID[30];
TCHAR hUser_PW[30];


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
 MainWndProc: 메인 윈도우 프로시저
--------------------------------------------------------*/
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hDC;
	PAINTSTRUCT ps;
	static HBRUSH hBrush = NULL;


	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		hBrush = CreateSolidBrush(RGB(218, 220, 214));

		//// 현재 접속 유저 아이디 초기화
		//lstrcpy(hUser_ID, "");

		//// 소켓 초기화 (윈속 라이브러리 버전, 윈속 시스템 관련 정보)//////////////////////////////
		//nReturn = WSAStartup(WORD(2.0), &wsadata);

		//// 소켓 생성 (IPv4: AF_INET | IPv6: AF_INET6 , 소켓 통신 타입, 프로토콜 결정)
		//clientsock = socket(AF_INET, SOCK_STREAM, 0);

		//addr_client.sin_family = AF_INET;
		//addr_client.sin_addr.s_addr = inet_addr(g_szlpAddress);
		//addr_client.sin_port = htons(g_uPort);

		//// 서버 연결
		//nReturn = connect(clientsock, (sockaddr*)&addr_client, addrlen_clt);

		//// 수신 스레드 생성
		//hThread = CreateThread(NULL, 0, Recv_Thread, &clientsock, 0, &ThreadID);
		//CloseHandle(hThread);

		//if (nReturn) {
		//	MessageBox(hWndMain, "연결실패", "오류", MB_OK);
		//}

		//////////////////////////////////////////////////////////////////////////////////////////


		// ID / PW 입력창 만들기

		STATIC_TEXT_ID = CreateWindow("static", "ID ", WS_CHILD | WS_VISIBLE | SS_CENTER , 200, 200, 40, 30, hWndMain, (HMENU)1, g_hInst, NULL);
		STATIC_TEXT_PW = CreateWindow("static", "PW ", WS_CHILD | WS_VISIBLE | SS_CENTER, 200, 250, 40, 30, hWndMain, (HMENU)2, g_hInst, NULL);

		ID_EDIT = CreateWindow("edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 240, 200, 200, 25, hWndMain, (HMENU)3, g_hInst, NULL);
		PW_EDIT = CreateWindow("edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 240, 250, 200, 25, hWndMain, (HMENU)4, g_hInst, NULL);

		// 회원가입/로그인/좌석선택 버튼 생성

		JOIN_BTN = CreateWindow("button", "회원 가입", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 100, 50, hWndMain, (HMENU)UTIL_BTN_ID, g_hInst, NULL);
		LOGIN_BTN = CreateWindow("button", "로그인", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 100, 0, 100, 50, hWndMain, (HMENU)(UTIL_BTN_ID + 1), g_hInst, NULL);
		SELECTION_BTN = CreateWindow("button", "좌석 선택", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 200, 0, 100, 50, hWndMain, (HMENU)(UTIL_BTN_ID + 2), g_hInst, NULL);

		ShowWindow(SELECTION_BTN, SW_HIDE);
		InvalidateRect(hWndMain, NULL, TRUE);

		return 0;
	case WM_CTLCOLORSTATIC:
		SetBkMode((HDC)wParam, TRANSPARENT);

		return (INT_PTR)hBrush;
	case WM_COMMAND:
		switch (LOWORD(wParam)){
		case UTIL_BTN_ID:
			// 회원 가입
			GetWindowText(ID_EDIT, hUser_ID, 30);
			GetWindowText(PW_EDIT, hUser_PW, 30);

			if (lstrcmp(hUser_ID, "") != 0 && lstrcmp(hUser_PW,"") != 0) {
				SetWindowText(ID_EDIT, "");
				SetWindowText(PW_EDIT, "");
				Change_Screen(1);
			}
			break;
		case UTIL_BTN_ID + 1:
			// 로그인
			GetWindowText(ID_EDIT, hUser_ID, 30);
			GetWindowText(PW_EDIT, hUser_PW, 30);

			if (lstrcmp(hUser_ID, "") != 0 && lstrcmp(hUser_PW, "") != 0) {
				SetWindowText(ID_EDIT, "");
				SetWindowText(PW_EDIT, "");
				Change_Screen(1);
			}
			break;
		case UTIL_BTN_ID + 2:
			// 자리 선택
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DLG_SEAT), hWnd, (DLGPROC)SeatDlgProc);
			break;
		default:
			
			break;
		}
		return 0;
	case WM_DESTROY:
		// brush 삭제
		if (hBrush) DeleteObject(hBrush);
		// 로그아웃 메세지 보내기
		//nReturn = send(clientsock, "CFF", sizeof("CFF"), 0);
		// 접속 종료 (소캣 객체)
		closesocket(clientsock);
		// 윈속 해제
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

		Send_Text("C00", "");

		// 임시 좌석세팅
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

/*--------------------------------------------------------
 Recv_Thread(LPVOID): 수신 스레드
--------------------------------------------------------*/
DWORD WINAPI Recv_Thread(LPVOID Param) {
	SOCKET* P = (SOCKET*)Param;
	TCHAR CODE[4], TEXT[256];

	for (;;) {
		// 접속 허용하기 (소켓 객체, 클라이언트 주소 정보, 주소 정보 구조 크기)

		nReturn = recv(*P, buf, 1024, 0);

		if (nReturn != 0 && nReturn != SOCKET_ERROR) {			
			Split_C_T(CODE, TEXT);

			if (lstrcmp(CODE, "S00") == 0) {
				Update_Seat(TEXT);
			}
			else if(lstrcmp(CODE, "S01") == 0){
				if (lstrcmp(TEXT, "SUCCESS") == 0) {
					MessageBox(hWndMain, "success", "!", MB_OK);
				}
				else {
					MessageBox(hWndMain, "fail", "알림", MB_OK);
				}
			}
		}
		else {
			MessageBox(hWndMain, "서버와 연결이 끊어졌습니다!", "server", MB_OK);
			return 0;
		}
	}
	return 0;
}

/*--------------------------------------------------------
 Split_C_T(TCHAR* CODE, TCHAR* TEXT) : 수신 받은 텍스트를
 CODE와 TEXT로 분리하는 함수
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
 Update_Seat(TCHAR* TEXT) : 좌석 현황 최신화
-------------------------------------------------------- */
void Update_Seat(TCHAR* TEXT) {
	int i;
	for (i = 0; i < MAX_SEAT + 1; i++) {
		if (TEXT[i] == '0') {
			if(!IsWindowEnabled(SEAT_BUTTON[i])) EnableWindow(SEAT_BUTTON[i], TRUE);
		}
		else if (TEXT[i] == '1') {
			if (IsWindowEnabled(SEAT_BUTTON[i])) EnableWindow(SEAT_BUTTON[i], FALSE);
		}
	}

}

/*--------------------------------------------------------
 Change_Screen(int ) : 화면 변경
-------------------------------------------------------- */
void Change_Screen(int state) {
	if (state == 0) {
		ShowWindow(JOIN_BTN, SW_SHOW);
		ShowWindow(LOGIN_BTN, SW_SHOW);
		ShowWindow(SELECTION_BTN, SW_HIDE);
		ShowWindow(STATIC_TEXT_ID, SW_SHOW);
		ShowWindow(STATIC_TEXT_PW, SW_SHOW);
		ShowWindow(ID_EDIT, SW_SHOW);
		ShowWindow(PW_EDIT, SW_SHOW);
	}
	else if (state == 1) {
		ShowWindow(JOIN_BTN, SW_HIDE);
		ShowWindow(LOGIN_BTN, SW_HIDE);
		ShowWindow(SELECTION_BTN, SW_SHOW);
		ShowWindow(STATIC_TEXT_ID, SW_HIDE);
		ShowWindow(STATIC_TEXT_PW, SW_HIDE);
		ShowWindow(ID_EDIT, SW_HIDE);
		ShowWindow(PW_EDIT, SW_HIDE);
	}
}

/*--------------------------------------------------------
 Send_Text(const char*, const char*, SOCKET*): 서버로
 텍스트 송신하는 함수
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
}