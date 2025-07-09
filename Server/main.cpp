#include "Socket.h"
#include "Process.h"
#include "Stock.h"
#include "Revenue.h"
#include "Employee.h"
#include "Customer.h"
#include "Work.h"

// Ÿ�̸� ��� ����//////////////////////////////////////////
typedef enum { LocalT = 1 };
/////////////////////////////////////////////////////////////

void Init_Wnd(WNDCLASS*, int);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("ī����");
LPCTSTR S_Class = TEXT("���");
LPCTSTR R_Class = TEXT("����");
LPCTSTR E_Class = TEXT("����");
LPCTSTR C_Class = TEXT("ȸ��");
LPCTSTR W_Class = TEXT("�ٹ�");

HWND hWndMain;		// ���� ȭ�� �ڵ�
HWND hWnd_S;		// ������� ȭ�� �ڵ�
HWND hWnd_R;		// �������� ȭ�� �ڵ�
HWND hWnd_E;		// �������� ȭ�� �ڵ�
HWND hWnd_C;		// ȸ������ ȭ�� �ڵ�
HWND hWnd_W;		// �ٹ����� ȭ�� �ڵ�

// ȭ��ó�� ���� ����////////////////////////////////////////

HBITMAP hBit;
HIMAGELIST Image;

/////////////////////////////////////////////////////////////

// �������� �Լ�/////////////////////////////////////////////

void SVR_Open();							// ���� ����
void SVR_Close();							// ���� Ŭ����

/////////////////////////////////////////////////////////////


// ȭ��ó�� ���� �Լ�////////////////////////////////////////

void OnTimer();								//Ÿ�̸ӷ� ���۸�
void DrawBitmap(HDC, int, int, HBITMAP);	//��Ʈ�� ���

/////////////////////////////////////////////////////////////

// ����Ʈ �� ���� ����///////////////////////////////////////

HWND Stock_I_List;		// ��� ����Ʈ��
HWND Revenue_I_List;	// ���� ����Ʈ��
HWND Employee_I_List;	// ���� ����Ʈ��
HWND Customer_I_List;	// ȸ�� ����Ʈ��
HWND Work_I_List;		// �ٹ� ����Ʈ��

/////////////////////////////////////////////////////////////

// � ���� �Լ�////////////////////////////////////////////

/////////////////////////////////////////////////////////////

// � �� ����ȭ ���� ����//////////////////////////////////

MAP *hMap;											// ���� ����ü
SEAT **hSeat;										// �¼� ����ü

HANDLE Message_Thread[MAX_MESSAGE_THREAD];			// �޽��� ó�� ������
HANDLE Message_Mutex;								// �޽��� ó�� ���ý�

HANDLE Charge_Thread;								// ���� ������(���� ���μ���)
HANDLE Charge_Mutex;								// ���� ���ؽ�

HANDLE Seat_Thread[MAX_SEAT];						// �¼� ������
HANDLE Seat_Mutex[MAX_SEAT];						// �¼� ���ؽ�

HANDLE Order_Thread;								// �ֹ� ������
HANDLE Order_Mutex;									// �ֹ� ���ý�

HANDLE *Order_Sub_Thread;							// �ֹ� ó�� ������
HANDLE *Order_Sub_Mutex;							// �ֹ� ó�� ���ý�

HANDLE Connect_Thread;								// ���� ���� ������

HANDLE Send_Thread[MAX_SEND_THREAD];				// �۽� ������
HANDLE Send_Mutex;									// �۽� ���ý�

MQ *Message_Front, *Message_Rear;					// �޼��� ó�� ť
CQ *Charge_Front, *Charge_Rear;						// ���� ť
SQ *Send_Front, *Send_Rear;							// �۽� ť
SEAT_M_Q **Seat_Message_Front, **Seat_Message_Rear;	// �¼� �޽��� ť
OQ* Order_Front, * Order_Rear;						// �ֹ� ť
OSQ **Order_Sub_Front, **Order_Sub_Rear;			// �ֹ� ���� ť

DWORD Message_TID[MAX_MESSAGE_THREAD];				// �޽��� ó�� ������ ID
DWORD Order_TID;									// �ֹ� ������ ID
DWORD *Order_Sub_TID;								// �ֹ� ó�� ������ ID
DWORD Charge_TID;									// ���� ������ ID
DWORD Send_TID[MAX_SEND_THREAD];					// �۽� ������ ID
DWORD Connect_TID;									// ���� ���� ������ ID

int Local_Time;							// 60�� Ÿ�̸� ����
int* Order_Sub_Num;						// �ֹ� ó�� ������ �ĺ���ȣ
/////////////////////////////////////////////////////////////

// ���ϰ��� ����/////////////////////////////////////////////

SOCKET listensock;
SOCKET clientsock = INVALID_SOCKET;
SOCKET hSocket = INVALID_SOCKET;
sockaddr_in addr_server;
sockaddr_in addr_client;
int addrlen_clt = sizeof(sockaddr);
int nReturn;
WSADATA wsadata;
CCI* C_CI;										// ���� ���� Ŭ���̾�Ʈ ���� ����
CI* C_I;										// ���� ���� �� ����
TCHAR Seat_Code[MAX_SEAT+1] = "";				// �¼� ��Ȳ �ڵ�

/////////////////////////////////////////////////////////////

// �������� ����/////////////////////////////////////////////

BOOL CONN_ST = FALSE;						// ���� ���� ����
extern const unsigned short g_uPort = 7878;		// ��Ʈ �ѹ�

/////////////////////////////////////////////////////////////

// DB���� ����///////////////////////////////////////////////
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
 Init_Wnd(WNDCLASS*, int): ������ �ʱ⼳��
--------------------------------------------------------*/
void Init_Wnd(WNDCLASS* Wnd, int Proc_No) {
	if (Proc_No > 4) {
		MessageBox(hWndMain, "������ �ʱ�ȭ ����!", "�˸�", MB_OK);
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
 WndProc: ���� ������ ���ν���
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
			MessageBox(NULL, (LPCSTR)"������ ���̽��� ������ �� �����ϴ�.", (LPCSTR)"����", MB_OK);
			return 0;
		}

		Local_Time = 0;						// Ÿ�̸� ���� �ʱ�ȭ
		SetTimer(hWnd, LocalT, 1, NULL);	// Ÿ�̸� ����

		WNDCLASS Wnd_S;
		WNDCLASS Wnd_R;
		WNDCLASS Wnd_E;
		WNDCLASS Wnd_C;
		WNDCLASS Wnd_W;

		// ��ǰ ������ ����
		Init_Wnd(&Wnd_S, 0);
		// ���� ������ ����
		Init_Wnd(&Wnd_R, 1);
		// ���� ������ ����
		Init_Wnd(&Wnd_E, 2);
		// ȸ�� ������ ����
		Init_Wnd(&Wnd_C, 3);
		// �ٹ���� ������ ����
		Init_Wnd(&Wnd_W, 4);
		
		hMap = Create_MAP();

		// �ʱ�ȭ �ܰ�
		// ���Ŀ� �¼� Ȯ���� ���Ͽ� ���������ͷ� ����
		hSeat = (SEAT**)malloc(MAX_SEAT * sizeof(SEAT*));

		for (int i = 0; i < MAX_SEAT; i++) {
			hSeat[i] = Create_SEAT();			// �¼� �ʱ�ȭ 30�¼� (���߿� �������� �ٲܼ��־�߉�)
		}

		C_CI = Create_CCI();
		C_I = Create_CI();

		// �޽��� ť �ʱ�ȭ
		Message_Front = Create_MQ();
		Message_Rear = Create_MQ();

		// ���� ť �ʱ�ȭ
		Charge_Front = Create_CQ();
		Charge_Rear = Create_CQ();

		// �۽� ť �ʱ�ȭ
		Send_Front = Create_SQ();
		Send_Rear = Create_SQ();

		// �¼� �޽��� ť �ʱ�ȭ
		Seat_Message_Front = (SEAT_M_Q**)malloc(MAX_SEAT * sizeof(SEAT_M_Q*));
		Seat_Message_Rear = (SEAT_M_Q**)malloc(MAX_SEAT * sizeof(SEAT_M_Q*));


		for (int i = 0; i < MAX_SEAT; i++) {
			Seat_Message_Front[i] = Create_SEAT_M_Q();
			Seat_Message_Rear[i] = Create_SEAT_M_Q();
		}

		// �ֹ� ���μ��� ����
		Order_Thread = CreateThread(NULL, 0, Order_Process, NULL, 0, &Order_TID);
		
		// �ֹ� ť �ʱ�ȭ
		Order_Front = Create_OQ();
		Order_Rear = Create_OQ();
		
		// �ֹ� ���� ť �ʱ�ȭ(���� �޴�Ȯ���� ���� ���������ͷ� �ۼ�)
		Order_Sub_Front = (OSQ**)malloc(MAX_MENU * sizeof(OSQ*));
		Order_Sub_Rear = (OSQ**)malloc(MAX_MENU * sizeof(OSQ*));

		for (int i = 0; i < MAX_MENU; i++) {
			Order_Sub_Front[i] = Create_OSQ();
			Order_Sub_Rear[i] = Create_OSQ();
		}

		// �ֹ� ó�� ���μ��� ����(Ȯ�� ����)
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

		// ���� ���ؽ� ����
		Charge_Mutex = CreateMutex(NULL, FALSE, NULL);
		if (Charge_Mutex == NULL) return 0;

		// �۽� ���ý� ����
		Send_Mutex = CreateMutex(NULL, FALSE, NULL);
		if (Send_Mutex == NULL) return 0;

		// �޼��� ó�� ���ý� ����
		Message_Mutex = CreateMutex(NULL, FALSE, NULL);
		if (Message_Mutex == NULL) return 0;
		
		// ���� ���μ��� ����
		Charge_Thread = CreateThread(NULL, 0, Charge_Process, NULL, 0, &Charge_TID);

		// �ӽ� �¼�����
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 6; j++) {
				k++;
				_itoa_s(k, num, 10);
				BUTTON[k - 1] = CreateWindow("button", num, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 50 + (j * 150), 50 + (i * 130), 150, 50, hWndMain, (HMENU)(BUTTON_ID + k - 1), g_hInst, NULL);
			}
		}

		Update_Seat_Code();	// �¼� ���� ������Ʈ

		// �¼� ���ؽ� ����
		for (int i = 0; i < MAX_SEAT; i++) {
			Seat_Mutex[i] = CreateMutex(NULL, FALSE, NULL);
			if (Seat_Mutex[i] == NULL) return 0;
		}

		//DBExcuteSQL();		���߿� ���� ������ �Լ��Դϴ�.

		SVR_Open();		// ��������
		
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_S_INFO:		// ���(Stock)���� ����
			hWnd_S = CreateWindow(S_Class, S_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
			ShowWindow(hWnd_S, SW_SHOW);
			break;
		case ID_R_INFO:		// ����(Revenue)���� ����
			hWnd_R = CreateWindow(R_Class, R_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
			ShowWindow(hWnd_R, SW_SHOW);
			break;
		case ID_E_INFO:		// ����(Employee)���� ����
			hWnd_E = CreateWindow(E_Class, E_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
			ShowWindow(hWnd_E, SW_SHOW);
			break;
		case ID_C_INFO:		// ȸ��(Customer)���� ����
			hWnd_C = CreateWindow(C_Class, C_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
			ShowWindow(hWnd_C, SW_SHOW);
			break;
		case ID_W_INFO:		// �ٹ�(Work)���� ����
			hWnd_W = CreateWindow(W_Class, W_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
			ShowWindow(hWnd_W, SW_SHOW);
			break;
		case BUTTON_ID:
			// DB�� ����� �׽�Ʈ�Ұž� �ϴ� 1������
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
				// �̶� DB�� ������ �մ� �¼��� �ð� ����

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
		KillTimer(hWnd, LocalT);			// Ÿ�̸� Kill
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

/*--------------------------------------------------------
 SVR_Open(): ���� ����
--------------------------------------------------------*/
void SVR_Open() {
	int i;

	if (!CONN_ST) {
		// ���� �ʱ�ȭ (���� ���̺귯�� ����, ���� �ý��� ���� ����)
		nReturn = WSAStartup(WORD(2.0), &wsadata);

		// ���� ���� (IPv4: AF_INET | IPv6: AF_INET6 , ���� ��� Ÿ��, �������� ����)
		listensock = socket(AF_INET, SOCK_STREAM, 0);

		// ���� �ּ� ����
		addr_server.sin_family = AF_INET;
		addr_server.sin_addr.s_addr = htons(INADDR_ANY);
		addr_server.sin_port = htons(g_uPort);

		// ���� ���ε� (���� ��ü, �Ұ� ��ü�� �ο��� �ּ� ���� ����ü, ����ü ����)
		nReturn = bind(listensock, (sockaddr*)&addr_server, sizeof(sockaddr));

		// ���� ��� (���� ��ü, ���� ��⿭ ũ��)
		nReturn = listen(listensock, MAX_BACKLOG);

		// �¼� Relay ������ ���� (�ϴ� ��� ���·� ����)
		for (i = 0; i < MAX_SEAT; i++) {
			Seat_Thread[i] = CreateThread(NULL, 0, Relay_Thread, &(hSeat[i]->S_num), CREATE_SUSPENDED, &(hSeat[i]->Thread_ID));
		}

		// Send ������ (3��) ���� = �ѹ��� ���� ��û�� ���� �۽� ť�� ������ �߻��Ұ��� ���
		for (i = 0; i < MAX_SEND_THREAD; i++) {
			Send_Thread[i] = CreateThread(NULL, 0, Send_Process, NULL, 0, &Send_TID[i]);
		}

		// Connect ������ ����(recv������ ���߻���)
		Connect_Thread = CreateThread(NULL, 0, Connect_Process, &listensock, 0, &Connect_TID);

		// �޽��� ó�� ���μ��� ���� (3��) ����
		for (i = 0; i < MAX_MESSAGE_THREAD; i++) {
			Message_Thread[i] = CreateThread(NULL, 0, Message_Process, NULL, 0, &Message_TID[i]);
		}

		//WSAAsyncSelect(listensock, hWndMain, WM_USER + 1, FD_ACCEPT | FD_READ | FD_CLOSE);

		CONN_ST = TRUE;
	}

	InvalidateRect(hWndMain, NULL, FALSE);
	MessageBox(hWndMain, "�������¿Ϸ�", "�˸�", MB_OK);
}


/*--------------------------------------------------------
 SVR_Close(): ���� ����
--------------------------------------------------------*/
void SVR_Close() {
	MAP* M;
	CCI* L_CI;
	CI* I;

	// ���� ���� ����(������ ���� ���� �ִ� ������� ��� �����)
	CONN_ST = FALSE;

	// ������ ���� ���
	WaitForMultipleObjects(MAX_MESSAGE_THREAD, Message_Thread, TRUE, INFINITE);		// �޽��� ó�� ������
	WaitForSingleObject(Charge_Thread, INFINITE);									// ���� ������(���� ���μ���)
	for (int i = 0; i < MAX_SEAT; i++) {
		if (hSeat[i]->S_State == 0) {
			ResumeThread(Seat_Thread[i]);
		}
	}
	WaitForMultipleObjects(MAX_SEAT, Seat_Thread, TRUE, INFINITE);					// �¼� ������
	WaitForSingleObject(Order_Thread, INFINITE);									// �ֹ� ������
	WaitForMultipleObjects(MAX_SEND_THREAD, Send_Thread, TRUE, INFINITE);			// �۽� ������
	WaitForMultipleObjects(MAX_MENU, Order_Sub_Thread, TRUE, INFINITE);				// �ֹ� ������

	closesocket(listensock);
	WaitForSingleObject(Connect_Thread, INFINITE);									// �ֹ� ������

	// ���ú� ������ | ���� ���� �������� ����
	L_CI = C_CI;
	while (C_CI->link != NULL) {
		L_CI = L_CI->link;
		Del_CCI(L_CI->Sock);
	}
	free(C_CI);

	// �ֹ�ó�� ���μ��� �Ҵ�����		
	for (int i = 0; i < MAX_MENU; i++) {
		if (Order_Sub_Thread[i]) {
			CloseHandle(Order_Sub_Thread[i]);
			CloseHandle(Order_Sub_Mutex[i]);
		}
	}
	free(Order_Sub_Thread);
	free(Order_Sub_TID);
	free(Order_Sub_Num);

	// �¼� ������ / ���ý� / �޸� ����
	for (int i = 0; i < MAX_SEAT; i++) {
		if (Seat_Thread[i]) {
			CloseHandle(Seat_Thread[i]);
			CloseHandle(Seat_Mutex[i]);
			free(hSeat[i]);
		}
	}
	free(hSeat);
	
	// �¼� �޽��� ť �Ҵ����� 
	for (int i = 0; i < MAX_SEAT; i++) {
		while (!IsEmpty_SEAT_M_Q(i + 1)) {
			Deque_SEAT_M_Q(i + 1);
		}
		free(Seat_Message_Front[i]);
		free(Seat_Message_Rear[i]);
	}
	free(Seat_Message_Front);
	free(Seat_Message_Rear);

	// ���� ť �Ҵ� ����
	while (!IsEmpty_CQ()) {
		Deque_CQ();
	}
	free(Charge_Front);
	free(Charge_Rear);

	// ���� �� �޸� ����
	while (hMap->link != NULL) {
		M = hMap;
		hMap = hMap->link;
		free(M);
	}
	free(hMap);

	// �޼��� ó�� ť �Ҵ�����
	while (!IsEmpty_MQ()) {
		Deque_MQ();
	}

	// �۽� ť �Ҵ� ����
	while (!IsEmpty_SQ()) {
		Deque_SQ();
	}
	free(Send_Front);
	free(Send_Rear);

	// ���� ���� ������ ����
	while (C_I->link != NULL) {
		I = C_I;
		C_I = C_I->link;
		free(I);
	}
	free(C_I);

	// �ֹ� ���� ť �ڵ� ��ȯ
	for (int i = 0; i < MAX_MENU; i++) {
		while (!IsEmpty_OSQ(i)) {
			Deque_OSQ(i);
		}
		free(Order_Sub_Front[i]);
		free(Order_Sub_Rear[i]);
	}
	free(Order_Sub_Front);
	free(Order_Sub_Rear);

	// �ֹ� ť �Ҵ� ����
	while (!IsEmpty_OQ()) {
		Deque_OQ();
	}
	free(Order_Front);
	free(Order_Rear);

	// �ֹ� ���μ��� �Ҵ�����
	CloseHandle(Order_Thread);

	// �۽� ���ý� �ڵ� ��ȯ
	CloseHandle(Send_Mutex);

	// ���� ���ý� �ڵ� ��ȯ
	CloseHandle(Charge_Mutex);

	// ���� ������ �ڵ� ��ȯ
	CloseHandle(Charge_Thread);

	// �޽��� ó�� ���ý� �ڵ� ��ȯ
	for (int i = 0; i < MAX_MESSAGE_THREAD; i++) {
		CloseHandle(Message_Thread[i]);
	}

	

	for (int i = 0; i < MAX_SEND_THREAD; i++) {
		CloseHandle(Send_Thread[i]);
	}

	CloseHandle(Connect_Thread);
}

/*--------------------------------------------------------
 OnTimer(): WndProc���� ȭ�� ����ϸ� ����
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
		�¼� ��Ʈ�� ����� �ڵ�
	*/
	Ellipse(hMemDC, 100 + i, 100, 200 + i, 200);
	i++;


	SelectObject(hMemDC, OldBit);
	DeleteDC(hMemDC);
	ReleaseDC(hWndMain, hdc);

	InvalidateRect(hWndMain, NULL, FALSE);
}

/*--------------------------------------------------------
 DrawBitmap(HDC,int,int,HBITMAP): ������ǥ�� ��Ʈ�� ���
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