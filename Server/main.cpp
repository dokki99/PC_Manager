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

// 서버관련 함수/////////////////////////////////////////////

void SVR_Open();							// 서버 오픈
void SVR_Close();							// 서버 클로즈

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

MAP *hMap;											// 매핑 구조체
SEAT **hSeat;										// 좌석 구조체

HANDLE Message_Thread[MAX_MESSAGE_THREAD];			// 메시지 처리 스레드
HANDLE Message_Mutex;								// 메시지 처리 뮤택스

HANDLE Charge_Thread;								// 충전 스레드(충전 프로세스)
HANDLE Charge_Mutex;								// 충전 뮤텍스

HANDLE Seat_Thread[MAX_SEAT];						// 좌석 스레드
HANDLE Seat_Mutex[MAX_SEAT];						// 좌석 뮤텍스

HANDLE Order_Thread;								// 주문 스레드
HANDLE Order_Mutex;									// 주문 뮤택스

HANDLE *Order_Sub_Thread;							// 주문 처리 스레드
HANDLE *Order_Sub_Mutex;							// 주문 처리 뮤택스

HANDLE Connect_Thread;								// 서버 연결 스레드

HANDLE Send_Thread[MAX_SEND_THREAD];				// 송신 스레드
HANDLE Send_Mutex;									// 송신 뮤택스

MQ *Message_Front, *Message_Rear;					// 메세지 처리 큐
CQ *Charge_Front, *Charge_Rear;						// 충전 큐
SQ *Send_Front, *Send_Rear;							// 송신 큐
SEAT_M_Q **Seat_Message_Front, **Seat_Message_Rear;	// 좌석 메시지 큐
OQ* Order_Front, * Order_Rear;						// 주문 큐
OSQ **Order_Sub_Front, **Order_Sub_Rear;			// 주문 서브 큐

DWORD Message_TID[MAX_MESSAGE_THREAD];				// 메시지 처리 스레드 ID
DWORD Order_TID;									// 주문 스레드 ID
DWORD *Order_Sub_TID;								// 주문 처리 스레드 ID
DWORD Charge_TID;									// 충전 스레드 ID
DWORD Send_TID[MAX_SEND_THREAD];					// 송신 스레드 ID
DWORD Connect_TID;									// 서버 연결 스레드 ID

int Local_Time;							// 60초 타이머 변수
int* Order_Sub_Num;						// 주문 처리 스레드 식별번호
/////////////////////////////////////////////////////////////

// 소켓관련 변수/////////////////////////////////////////////

SOCKET listensock;
SOCKET clientsock = INVALID_SOCKET;
SOCKET hSocket = INVALID_SOCKET;
sockaddr_in addr_server;
sockaddr_in addr_client;
int addrlen_clt = sizeof(sockaddr);
int nReturn;
WSADATA wsadata;
CCI* C_CI;										// 현재 접속 클라이언트 연결 정보
CI* C_I;										// 현재 접속 고객 정보
TCHAR Seat_Code[MAX_SEAT+1] = "";				// 좌석 현황 코드

/////////////////////////////////////////////////////////////

// 서버관련 변수/////////////////////////////////////////////

BOOL CONN_ST = FALSE;						// 서버 오픈 상태
extern const unsigned short g_uPort = 7878;		// 포트 넘버

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

		Local_Time = 0;						// 타이머 변수 초기화
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
		// 추후에 좌석 확장을 위하여 이중포인터로 생성
		hSeat = (SEAT**)malloc(MAX_SEAT * sizeof(SEAT*));

		for (int i = 0; i < MAX_SEAT; i++) {
			hSeat[i] = Create_SEAT();			// 좌석 초기화 30좌석 (나중에 가변으로 바꿀수있어야됌)
		}

		C_CI = Create_CCI();
		C_I = Create_CI();

		// 메시지 큐 초기화
		Message_Front = Create_MQ();
		Message_Rear = Create_MQ();

		// 충전 큐 초기화
		Charge_Front = Create_CQ();
		Charge_Rear = Create_CQ();

		// 송신 큐 초기화
		Send_Front = Create_SQ();
		Send_Rear = Create_SQ();

		// 좌석 메시지 큐 초기화
		Seat_Message_Front = (SEAT_M_Q**)malloc(MAX_SEAT * sizeof(SEAT_M_Q*));
		Seat_Message_Rear = (SEAT_M_Q**)malloc(MAX_SEAT * sizeof(SEAT_M_Q*));


		for (int i = 0; i < MAX_SEAT; i++) {
			Seat_Message_Front[i] = Create_SEAT_M_Q();
			Seat_Message_Rear[i] = Create_SEAT_M_Q();
		}

		// 주문 프로세스 생성
		Order_Thread = CreateThread(NULL, 0, Order_Process, NULL, 0, &Order_TID);
		
		// 주문 큐 초기화
		Order_Front = Create_OQ();
		Order_Rear = Create_OQ();
		
		// 주문 서브 큐 초기화(추후 메뉴확장을 위해 이중포인터로 작성)
		Order_Sub_Front = (OSQ**)malloc(MAX_MENU * sizeof(OSQ*));
		Order_Sub_Rear = (OSQ**)malloc(MAX_MENU * sizeof(OSQ*));

		for (int i = 0; i < MAX_MENU; i++) {
			Order_Sub_Front[i] = Create_OSQ();
			Order_Sub_Rear[i] = Create_OSQ();
		}

		// 주문 처리 프로세스 생성(확장 예정)
		Order_Sub_Thread = (HANDLE*)malloc(MAX_MENU * sizeof(HANDLE));
		Order_Sub_Mutex = (HANDLE*)malloc(MAX_MENU * sizeof(HANDLE));
		Order_Sub_TID = (DWORD*)calloc(MAX_MENU, sizeof(DWORD));
		Order_Sub_Num = (int*)calloc(MAX_MENU, sizeof(int));

		for (int i = 0; i < MAX_MENU; i++) {
			Order_Sub_Num[i] = i;
			Order_Sub_Thread[i] = CreateThread(NULL, 0, Order_Sub_Process, &Order_Sub_Num[i], 0, &Order_Sub_TID[i]);
			Order_Sub_Mutex[i] = CreateMutex(NULL, FALSE, NULL);
			if (Order_Sub_Mutex[i] == NULL) return 0;
		}

		// 충전 뮤텍스 생성
		Charge_Mutex = CreateMutex(NULL, FALSE, NULL);
		if (Charge_Mutex == NULL) return 0;

		// 송신 뮤택스 생성
		Send_Mutex = CreateMutex(NULL, FALSE, NULL);
		if (Send_Mutex == NULL) return 0;

		// 메세지 처리 뮤택스 생성
		Message_Mutex = CreateMutex(NULL, FALSE, NULL);
		if (Message_Mutex == NULL) return 0;
		
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

		Update_Seat_Code();	// 좌석 정보 업데이트

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
 SVR_Open(): 서버 오픈
--------------------------------------------------------*/
void SVR_Open() {
	int i;

	if (!CONN_ST) {
		// 소켓 초기화 (윈속 라이브러리 버전, 윈속 시스템 관련 정보)
		nReturn = WSAStartup(WORD(2.0), &wsadata);

		// 소켓 생성 (IPv4: AF_INET | IPv6: AF_INET6 , 소켓 통신 타입, 프로토콜 결정)
		listensock = socket(AF_INET, SOCK_STREAM, 0);

		// 서버 주소 설정
		addr_server.sin_family = AF_INET;
		addr_server.sin_addr.s_addr = htons(INADDR_ANY);
		addr_server.sin_port = htons(g_uPort);

		// 소켓 바인드 (소켓 객체, 소객 객체에 부여할 주소 정보 구조체, 구조체 길이)
		nReturn = bind(listensock, (sockaddr*)&addr_server, sizeof(sockaddr));

		// 접속 대기 (소켓 객체, 연결 대기열 크기)
		nReturn = listen(listensock, MAX_BACKLOG);

		// 좌석 Relay 스레드 생성 (일단 대기 상태로 생성)
		for (i = 0; i < MAX_SEAT; i++) {
			Seat_Thread[i] = CreateThread(NULL, 0, Relay_Thread, &(hSeat[i]->S_num), CREATE_SUSPENDED, &(hSeat[i]->Thread_ID));
		}

		// Send 스레드 (3개) 생성 = 한번에 많은 요청이 들어와 송신 큐에 지연이 발생할것을 고려
		for (i = 0; i < MAX_SEND_THREAD; i++) {
			Send_Thread[i] = CreateThread(NULL, 0, Send_Process, NULL, 0, &Send_TID[i]);
		}

		// Connect 스레드 생성(recv스레드 다중생성)
		Connect_Thread = CreateThread(NULL, 0, Connect_Process, &listensock, 0, &Connect_TID);

		// 메시지 처리 프로세스 생성 (3개) 생성
		for (i = 0; i < MAX_MESSAGE_THREAD; i++) {
			Message_Thread[i] = CreateThread(NULL, 0, Message_Process, NULL, 0, &Message_TID[i]);
		}

		//WSAAsyncSelect(listensock, hWndMain, WM_USER + 1, FD_ACCEPT | FD_READ | FD_CLOSE);

		CONN_ST = TRUE;
	}

	InvalidateRect(hWndMain, NULL, FALSE);
	MessageBox(hWndMain, "서버오픈완료", "알림", MB_OK);
}


/*--------------------------------------------------------
 SVR_Close(): 서버 종료
--------------------------------------------------------*/
void SVR_Close() {
	MAP* M;
	CCI* L_CI;
	CI* I;

	// 서버 연결 해제(서버내 에서 돌고 있는 스레드는 모두 종료됨)
	CONN_ST = FALSE;

	// 스레드 종료 대기
	WaitForMultipleObjects(MAX_MESSAGE_THREAD, Message_Thread, TRUE, INFINITE);		// 메시지 처리 스레드
	WaitForSingleObject(Charge_Thread, INFINITE);									// 충전 스레드(충전 프로세스)
	for (int i = 0; i < MAX_SEAT; i++) {
		if (hSeat[i]->S_State == 0) {
			ResumeThread(Seat_Thread[i]);
		}
	}
	WaitForMultipleObjects(MAX_SEAT, Seat_Thread, TRUE, INFINITE);					// 좌석 스레드
	WaitForSingleObject(Order_Thread, INFINITE);									// 주문 스레드
	WaitForMultipleObjects(MAX_SEND_THREAD, Send_Thread, TRUE, INFINITE);			// 송신 스레드
	WaitForMultipleObjects(MAX_MENU, Order_Sub_Thread, TRUE, INFINITE);				// 주문 스레드

	closesocket(listensock);
	WaitForSingleObject(Connect_Thread, INFINITE);									// 주문 스레드

	// 리시브 스레드 | 현재 접속 소켓정보 삭제
	L_CI = C_CI;
	while (C_CI->link != NULL) {
		L_CI = L_CI->link;
		Del_CCI(L_CI->Sock);
	}
	free(C_CI);

	// 주문처리 프로세스 할당해제		
	for (int i = 0; i < MAX_MENU; i++) {
		if (Order_Sub_Thread[i]) {
			CloseHandle(Order_Sub_Thread[i]);
			CloseHandle(Order_Sub_Mutex[i]);
		}
	}
	free(Order_Sub_Thread);
	free(Order_Sub_TID);
	free(Order_Sub_Num);

	// 좌석 스레드 / 뮤택스 / 메모리 해제
	for (int i = 0; i < MAX_SEAT; i++) {
		if (Seat_Thread[i]) {
			CloseHandle(Seat_Thread[i]);
			CloseHandle(Seat_Mutex[i]);
			free(hSeat[i]);
		}
	}
	free(hSeat);
	
	// 좌석 메시지 큐 할당해제 
	for (int i = 0; i < MAX_SEAT; i++) {
		while (!IsEmpty_SEAT_M_Q(i + 1)) {
			Deque_SEAT_M_Q(i + 1);
		}
		free(Seat_Message_Front[i]);
		free(Seat_Message_Rear[i]);
	}
	free(Seat_Message_Front);
	free(Seat_Message_Rear);

	// 충전 큐 할당 해제
	while (!IsEmpty_CQ()) {
		Deque_CQ();
	}
	free(Charge_Front);
	free(Charge_Rear);

	// 매핑 및 메모리 해제
	while (hMap->link != NULL) {
		M = hMap;
		hMap = hMap->link;
		free(M);
	}
	free(hMap);

	// 메세지 처리 큐 할당해제
	while (!IsEmpty_MQ()) {
		Deque_MQ();
	}

	// 송신 큐 할당 해제
	while (!IsEmpty_SQ()) {
		Deque_SQ();
	}
	free(Send_Front);
	free(Send_Rear);

	// 현재 접속 고객정보 삭제
	while (C_I->link != NULL) {
		I = C_I;
		C_I = C_I->link;
		free(I);
	}
	free(C_I);

	// 주문 서브 큐 핸들 반환
	for (int i = 0; i < MAX_MENU; i++) {
		while (!IsEmpty_OSQ(i)) {
			Deque_OSQ(i);
		}
		free(Order_Sub_Front[i]);
		free(Order_Sub_Rear[i]);
	}
	free(Order_Sub_Front);
	free(Order_Sub_Rear);

	// 주문 큐 할당 해제
	while (!IsEmpty_OQ()) {
		Deque_OQ();
	}
	free(Order_Front);
	free(Order_Rear);

	// 주문 프로세스 할당해제
	CloseHandle(Order_Thread);

	// 송신 뮤택스 핸들 반환
	CloseHandle(Send_Mutex);

	// 충전 뮤택스 핸들 반환
	CloseHandle(Charge_Mutex);

	// 충전 스레드 핸들 반환
	CloseHandle(Charge_Thread);

	// 메시지 처리 뮤택스 핸들 반환
	for (int i = 0; i < MAX_MESSAGE_THREAD; i++) {
		CloseHandle(Message_Thread[i]);
	}

	

	for (int i = 0; i < MAX_SEND_THREAD; i++) {
		CloseHandle(Send_Thread[i]);
	}

	CloseHandle(Connect_Thread);
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