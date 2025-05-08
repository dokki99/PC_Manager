#include "Socket.h"
#include "Process.h"
#include "Stock.h"
#include "Revenue.h"
#include "Employee.h"
#include "Customer.h"
#include "Work.h"

// 타이머 상수 설정//////////////////////////////////////////
typedef enum { LocalT = 1 };
/////////////////////////////////////////////////////////////

void Init_Wnd(WNDCLASS*, int);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("카운터");
LPCTSTR S_Class = TEXT("재고");
LPCTSTR R_Class = TEXT("매출");
LPCTSTR E_Class = TEXT("직원");
LPCTSTR C_Class = TEXT("회원");
LPCTSTR W_Class = TEXT("근무");

HWND hWndMain;		// 메인 화면 핸들
HWND hWnd_S;		// 재고정보 화면 핸들
HWND hWnd_R;		// 매출정보 화면 핸들
HWND hWnd_E;		// 직원정보 화면 핸들
HWND hWnd_C;		// 회원정보 화면 핸들
HWND hWnd_W;		// 근무정보 화면 핸들

// 화면처리 관련 변수////////////////////////////////////////

HBITMAP hBit;
HIMAGELIST Image;

/////////////////////////////////////////////////////////////

// 화면처리 관련 함수////////////////////////////////////////

void OnTimer();								//타이머로 버퍼링
void DrawBitmap(HDC, int, int, HBITMAP);	//비트맵 출력

/////////////////////////////////////////////////////////////

// 리스트 뷰 관련 변수///////////////////////////////////////

HWND Stock_I_List;		// 재고 리스트뷰
HWND Revenue_I_List;	// 매출 리스트뷰
HWND Employee_I_List;	// 직원 리스트뷰
HWND Customer_I_List;	// 회원 리스트뷰
HWND Work_I_List;		// 근무 리스트뷰

/////////////////////////////////////////////////////////////

// 운영 관련 함수////////////////////////////////////////////

/////////////////////////////////////////////////////////////

// 운영 및 동기화 관련 변수//////////////////////////////////

MAP* hMap;								// 매핑 구조체
SEAT* hSeat[MAX_SEAT];					// 좌석 구조체

HANDLE Charge_Thread;					// 충전 스레드(충전 프로세스)
HANDLE Charge_Mutex;					// 충전 뮤텍스

HANDLE Seat_Thread[MAX_SEAT];			// 좌석 스레드
HANDLE Seat_Mutex[MAX_SEAT];			// 좌석 뮤텍스

HANDLE Send_Mutex;						// 송신 큐 INPUT 뮤택스
HANDLE Transform_Mutex;					// 가공 뮤택스

CQ *Charge_Front, *Charge_Rear;			// 충전 우선순위 큐
SQ *Send_Front, *Send_Rear;				// 송신 우선순위 큐

DWORD Charge_TID;						// 충전 스레드 ID
DWORD Send_TID;							// 송신 스레드 ID

int Local_Time;							// 60초 타이머 변수

/////////////////////////////////////////////////////////////

// 소켓관련 변수/////////////////////////////////////////////

SOCKET listensock;
SOCKET clientsock = 0;
SOCKET hSocket = 0;
sockaddr_in addr_server;
sockaddr_in addr_client;
int addrlen_clt = sizeof(sockaddr);
int nReturn;
WSADATA wsadata;
CS* C_S;										// 현재 접속 클라이언트 정보
CI* C_I;										// 현재 접속 고객 정보
TCHAR Seat_Code[MAX_SEAT+1] = "";				// 좌석 현황 코드

/////////////////////////////////////////////////////////////

// 서버관련 변수/////////////////////////////////////////////

BOOL CONN_ST = FALSE;						// 서버 오픈 상태
extern const unsigned short g_uPort = 7878;		// 포트 넘버
extern const int buflen = 4096;					// 버퍼 크기
TCHAR buf[buflen];

/////////////////////////////////////////////////////////////

// DB관련 변수///////////////////////////////////////////////
TCHAR DB_ID[10] = "commonPC";
TCHAR DB_PWD[10] = "PC123";
SQLHENV hEnv;
SQLHDBC hDbc;
SQLHSTMT hStmt;

/////////////////////////////////////////////////////////////

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
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;

}

/*--------------------------------------------------------
 Init_Wnd(WNDCLASS*, int): 윈도우 초기설정
--------------------------------------------------------*/
void Init_Wnd(WNDCLASS* Wnd, int Proc_No) {
	if (Proc_No > 4) {
		MessageBox(hWndMain, "윈도우 초기화 오류!", "알림", MB_OK);
		return;
	}

	Wnd->cbClsExtra = 0;
	Wnd->cbWndExtra = 0;
	Wnd->hbrBackground = (HBRUSH)RGB(0, 0, 0);
	Wnd->hCursor = LoadCursor(NULL, IDC_ARROW);
	Wnd->hIcon = LoadIcon(NULL, IDI_ASTERISK);
	Wnd->hInstance = g_hInst;
	switch (Proc_No)
	{
	case 0:
		Wnd->lpfnWndProc = Stock_Info_Proc;
		Wnd->lpszClassName = S_Class;
		break;
	case 1:
		Wnd->lpfnWndProc = Revenue_Info_Proc;
		Wnd->lpszClassName = R_Class;
		break;
	case 2:
		Wnd->lpfnWndProc = Employee_Info_Proc;
		Wnd->lpszClassName = E_Class;
		break;
	case 3:
		Wnd->lpfnWndProc = Customer_Info_Proc;
		Wnd->lpszClassName = C_Class;
		break;
	case 4:
		Wnd->lpfnWndProc = Work_Info_Proc;
		Wnd->lpszClassName = W_Class;
	}
	Wnd->lpszMenuName = NULL;
	Wnd->style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(Wnd);
}

/*--------------------------------------------------------
 WndProc: 메인 윈도우 프로시저
--------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR t[200], T[300] = "010-9275-xxx5";
	static HWND BUTTON[30];
	static int k = 0;
	char num[5];
	int E;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		InitCommonControls();
		
		if (DBConnect() == FALSE) {
			MessageBox(NULL, (LPCSTR)"데이터 베이스에 연결할 수 없습니다.", (LPCSTR)"에러", MB_OK);
			return 0;
		}

		Local_Time = 0;								// 타이머 변수 초기화
		SetTimer(hWnd, LocalT, 1, NULL);	// 타이머 생성

		WNDCLASS Wnd_S;
		WNDCLASS Wnd_R;
		WNDCLASS Wnd_E;
		WNDCLASS Wnd_C;
		WNDCLASS Wnd_W;

		// 제품 윈도우 설정
		Init_Wnd(&Wnd_S, 0);
		// 매출 윈도우 설정
		Init_Wnd(&Wnd_R, 1);
		// 직원 윈도우 설정
		Init_Wnd(&Wnd_E, 2);
		// 회원 윈도우 설정
		Init_Wnd(&Wnd_C, 3);
		// 근무기록 윈도우 설정
		Init_Wnd(&Wnd_W, 4);
		

		hMap = Create_MAP();

		// 초기화 단계
		for (int i = 0; i < MAX_SEAT; i++) {
			hSeat[i] = Create_SEAT();			// 좌석 초기화 30좌석 (나중에 가변으로 바꿀수있어야됌)
		}

		C_S = Create_CS();
		C_I = Create_CI();
		
		// 충전 우선순위 큐 초기화
		Charge_Front = Create_CQ();
		Charge_Rear = Create_CQ();

		// 송신 우선순위 큐 초기화
		Send_Front = Create_SQ();
		Send_Rear = Create_SQ();

		// 충전 뮤텍스 생성
		Charge_Mutex = CreateMutex(NULL, FALSE, NULL);
		if (Charge_Mutex == NULL) return 0;

		// 송신 뮤택스 생성
		Send_Mutex = CreateMutex(NULL, FALSE, NULL);
		if (Send_Mutex == NULL) return 0;
		

		// 가공 뮤택스 생성
		Transform_Mutex = CreateMutex(NULL, FALSE, NULL);
		if (Transform_Mutex == NULL) return 0;

		// 충전 프로세스 생성
		Charge_Thread = CreateThread(NULL, 0, Charge_Process, NULL, 0, &Charge_TID);

		// 임시 좌석세팅
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 6; j++) {
				k++;
				_itoa_s(k, num, 10);
				BUTTON[k - 1] = CreateWindow("button", num, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 50 + (j * 150), 50 + (i * 130), 150, 50, hWndMain, (HMENU)(BUTTON_ID + k - 1), g_hInst, NULL);
			}
		}

		Update_Seat_Code();

		// 좌석 뮤텍스 생성
		for (int i = 0; i < MAX_SEAT; i++) {
			Seat_Mutex[i] = CreateMutex(NULL, FALSE, NULL);
			if (Seat_Mutex[i] == NULL) return 0;
		}

		//DBExcuteSQL();		나중에 수정 개선할 함수입니다.

		SVR_Open();		// 서버오픈
		
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_S_INFO:		// 재고(Stock)정보 오픈
			hWnd_S = CreateWindow(S_Class, S_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
			ShowWindow(hWnd_S, SW_SHOW);
			break;
		case ID_R_INFO:		// 매출(Revenue)정보 오픈
			hWnd_R = CreateWindow(R_Class, R_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
			ShowWindow(hWnd_R, SW_SHOW);
			break;
		case ID_E_INFO:		// 직원(Employee)정보 오픈
			hWnd_E = CreateWindow(E_Class, E_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
			ShowWindow(hWnd_E, SW_SHOW);
			break;
		case ID_C_INFO:		// 회원(Customer)정보 오픈
			hWnd_C = CreateWindow(C_Class, C_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
			ShowWindow(hWnd_C, SW_SHOW);
			break;
		case ID_W_INFO:		// 근무(Work)정보 오픈
			hWnd_W = CreateWindow(W_Class, W_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
			ShowWindow(hWnd_W, SW_SHOW);
			break;
		case BUTTON_ID:
			// DB쪽 기능을 테스트할거야 일단 1번으로
			E = Time_To_Int("16:52:54");
			wsprintf(t, "%d",E);
			MessageBox(hWndMain, t, "1", MB_OK);
			break;
		}
		return 0;
	case WM_TIMER:
		switch (wParam) {
		case LocalT:
			//OnTimer();
			Local_Time++;
			if (60 == Local_Time) {
				// 이때 DB와 접속한 손님 좌석에 시간 갱신

				Local_Time = 0;
			}
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (hBit) {
			DrawBitmap(hdc, 0, 0, hBit);
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		closesocket(clientsock);
		SVR_Close();
		WSACleanup();
		DBDisconnect();
		if (hBit) {
			DeleteObject(hBit);
		}
		KillTimer(hWnd, LocalT);			// 타이머 Kill
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

/*--------------------------------------------------------
 OnTimer(): WndProc에서 화면 출력하며 동작
--------------------------------------------------------*/
void OnTimer() {
	RECT crt;
	HDC hdc, hMemDC;
	HBITMAP OldBit;
	static int i = 0;

	GetClientRect(hWndMain, &crt);
	hdc = GetDC(hWndMain);

	if (hBit == NULL) {
		hBit = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
	}
	hMemDC = CreateCompatibleDC(hdc);
	OldBit = (HBITMAP)SelectObject(hMemDC, hBit);

	FillRect(hMemDC, &crt, CreateSolidBrush(RGB(218, 220, 214)));
	/*
		좌석 비트맵 출력할 코드
	*/
	Ellipse(hMemDC, 100 + i, 100, 200 + i, 200);
	i++;


	SelectObject(hMemDC, OldBit);
	DeleteDC(hMemDC);
	ReleaseDC(hWndMain, hdc);

	InvalidateRect(hWndMain, NULL, FALSE);
}

/*--------------------------------------------------------
 DrawBitmap(HDC,int,int,HBITMAP): 지정좌표에 비트맵 출력
--------------------------------------------------------*/
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit) {
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}