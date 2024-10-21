#include "Socket.h"
#include "Stock.h"
#include "Revenue.h"
#include "Employee.h"
#include "Customer.h"
#include "Work.h"

// Ÿ�̸� ��� ����//////////////////////////////////////////
typedef enum { BuffT = 1 };
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

SEAT* create_S();		// �¼� �ʱ�ȭ

/////////////////////////////////////////////////////////////

// � ���� ����////////////////////////////////////////////

SEAT* hSeat[30];

/////////////////////////////////////////////////////////////

// ���ϰ��� ����/////////////////////////////////////////////

SOCKET listensock;
SOCKET clientsock = 0;
SOCKET hSocket = 0;
CS* C_S;									// ���� Linked		
sockaddr_in addr_server;
sockaddr_in addr_client;
int addrlen_clt = sizeof(sockaddr);
int nReturn;
WSADATA wsadata;

/////////////////////////////////////////////////////////////

// �������� ����/////////////////////////////////////////////

BOOL CONN_ST = FALSE;						// ���� ���� ����
extern const unsigned short g_uPort = 7878;		// ��Ʈ �ѹ�
extern const int buflen = 4096;					// ���� ũ��
TCHAR buf[buflen];

/////////////////////////////////////////////////////////////

// DB���� ����///////////////////////////////////////////////
TCHAR ID[10] = "commonPC";
TCHAR PWD[10] = "PC123";
SQLHENV hEnv;
SQLHDBC hDbc;
SQLHSTMT hStmt;

/////////////////////////////////////////////////////////////

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpszCmdParam, int nCmdShow) {

	if (DBConnect() == FALSE) {
		MessageBox(NULL, (LPCSTR)"������ ���̽��� ������ �� �����ϴ�.", (LPCSTR)"����", MB_OK);
		return 0;
	}

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

	DBDisconnect();

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

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		InitCommonControls();
		
		Image = ImageList_LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1), 45, 1, RGB(255, 255, 255));		//�¼� �̹�������Ʈ ���� 
		SetTimer(hWnd, BuffT, 1, NULL);	//���۸� Ÿ�̸� ����
		C_S = create();		// ���� ����ü ����

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

		SVR_Open();		// ��������

		for (int i = 0; i < 30; i ++) {
			hSeat[i] = create_S();				// �¼� �ʱ�ȭ 30�¼�
		}

		//DBExcuteSQL();		���߿� ���� ������ �Լ��Դϴ�.

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
		}
		return 0;
	case WM_TIMER:
		switch (wParam) {
		case BuffT:
			OnTimer();
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
		WSACleanup();
		if (hBit) {
			DeleteObject(hBit);
		}
		KillTimer(hWnd, BuffT);			//���۸� Ÿ�̸� Kill
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
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
 * create_S(): �¼� �ʱ�ȭ �Լ�
--------------------------------------------------------*/
SEAT* create_S() {
	SEAT* N;

	N = (SEAT*)malloc(sizeof(SEAT));
	N->state = false;
	N->client = NULL;

	return N;
}