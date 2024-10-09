#include <windows.h>
#include <stdio.h>
#include <sql.h>
#include <sqlext.h>
#include <commctrl.h>
#include "resource.h"


// ���� ���� ����ü
typedef struct Client_Socket {
	TCHAR ID[128];
	SOCKET Client_Sock;
	struct Client_Socket* link;
}CS;

void Init_Wnd(WNDCLASS*, int);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK Stock_Info_Proc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK Revenue_Info_Proc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK Employee_Info_Proc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK Customer_Info_Proc(HWND, UINT, WPARAM, LPARAM);


HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("ī����");
LPCTSTR S_Class = TEXT("���");
LPCTSTR R_Class = TEXT("����");
LPCTSTR E_Class = TEXT("����");
LPCTSTR C_Class = TEXT("ȸ��");
HWND hWndMain;		// ���� ȭ�� �ڵ�
HWND hWnd_S;		// ������� ȭ�� �ڵ�
HWND hWnd_R;		// �������� ȭ�� �ڵ�
HWND hWnd_E;		// �������� ȭ�� �ڵ�
HWND hWnd_C;		// ȸ������ ȭ�� �ڵ�
HWND hToolBar;		// ���� �ڵ�

// DB���� �Լ�///////////////////////////////////////////////

BOOL DBConnect();
void DBDisconnect();
BOOL DBExcuteSQL();
BOOL Load_Stock_Data();
BOOL Load_Revenue_Data();
BOOL Load_Employee_Data();
BOOL Load_Customer_Data();
/////////////////////////////////////////////////////////////


// DB���� ����///////////////////////////////////////////////
TCHAR ID[10] = "commonPC";
TCHAR PWD[10] = "PC123";
SQLHENV hEnv;
SQLHDBC hDbc;
SQLHSTMT hStmt;

/////////////////////////////////////////////////////////////


// ���ϰ��� ����/////////////////////////////////////////////

SOCKET listensock;
SOCKET clientsock = 0;
CS* C_S;									// ���� Linked		
sockaddr_in addr_server;
sockaddr_in addr_client;
int addrlen_clt = sizeof(sockaddr);
int nReturn;
WSADATA wsadata;

/////////////////////////////////////////////////////////////


// ���ϰ��� �Լ�/////////////////////////////////////////////

CS* create();							// ���� ����ü �Ҵ�
void addsock(SOCKET);					// ���� ���� �߰�
void delsock(SOCKET);					// ���� ���� ����

/////////////////////////////////////////////////////////////


// �������� �Լ�/////////////////////////////////////////////

void SVR_Open();						// ���� ����
DWORD WINAPI Connect_Thread(LPVOID);	// ���� ���� ������
DWORD WINAPI Recv_Thread(LPVOID);		// ���� ������

/////////////////////////////////////////////////////////////

// �������� ����/////////////////////////////////////////////

BOOL CONN_ST = FALSE;						// ���� ���� ����
const unsigned short g_uPort = 7878;		// ��Ʈ �ѹ�
const int buflen = 4096;					// ���� ũ��
TCHAR buf[buflen];

/////////////////////////////////////////////////////////////

// ����Ʈ �� ���� ����///////////////////////////////////////

HWND Stock_I_List;
HWND Revenue_I_List;
HWND Employee_I_List;
HWND Customer_I_List;

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
 Init_Wnd: ������ �ʱ⼳��
--------------------------------------------------------*/
void Init_Wnd(WNDCLASS* Wnd, int Proc_No) {
	if(Proc_No > 3){
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
	}
	Wnd->lpszMenuName = NULL;
	Wnd->style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(Wnd);
}

/*--------------------------------------------------------
 WndProc: ���� ������ ���ν���
--------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		InitCommonControls();

		C_S = create();		// ���� ����ü ����

		WNDCLASS Wnd_S;
		WNDCLASS Wnd_R;
		WNDCLASS Wnd_E;
		WNDCLASS Wnd_C;

		// ��ǰ������ ����
		Init_Wnd(&Wnd_S, 0);
		// ���������� ����
		Init_Wnd(&Wnd_R, 1);
		// ���������� ����
		Init_Wnd(&Wnd_E, 2);
		// ȸ�������� ����
		Init_Wnd(&Wnd_C, 3);

		DBExcuteSQL();
		
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_CONNECT:
			SVR_Open();		// ��������
			break;
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
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

/*--------------------------------------------------------
 Stock_Info_Proc: ������� ���ν���
--------------------------------------------------------*/
LRESULT CALLBACK Stock_Info_Proc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	LVCOLUMN COL;
	switch (iMessage) {
	case WM_CREATE:
		Stock_I_List = CreateWindow(WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, 20, 20, 800, 400, hWnd, NULL, g_hInst, NULL);
		ListView_SetExtendedListViewStyle(Stock_I_List, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

		COL.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		COL.fmt = LVCFMT_LEFT;
		COL.cx = 130;

		COL.pszText = (LPSTR)"��ǰ�ڵ�";    //ù ��° ���
		COL.iSubItem = 0;
		SendMessage(Stock_I_List, LVM_INSERTCOLUMN, 0, (LPARAM)&COL);

		COL.pszText = (LPSTR)"��ǰ��";    //�� ��° ���
		COL.iSubItem = 1;
		SendMessage(Stock_I_List, LVM_INSERTCOLUMN, 1, (LPARAM)&COL);

		COL.pszText = (LPSTR)"����";    //�� ��° ���
		COL.iSubItem = 2;
		SendMessage(Stock_I_List, LVM_INSERTCOLUMN, 2, (LPARAM)&COL);

		COL.pszText = (LPSTR)"����";    //�� ��° ���
		COL.iSubItem = 3;
		SendMessage(Stock_I_List, LVM_INSERTCOLUMN, 3, (LPARAM)&COL);

		COL.pszText = (LPSTR)"ǰ��";    //�ټ� ��° ���
		COL.iSubItem = 4;
		SendMessage(Stock_I_List, LVM_INSERTCOLUMN, 4, (LPARAM)&COL);

		COL.pszText = (LPSTR)"������";    //���� ��° ���
		COL.iSubItem = 5;
		SendMessage(Stock_I_List, LVM_INSERTCOLUMN, 5, (LPARAM)&COL);

		Load_Stock_Data();

		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

/*--------------------------------------------------------
 Revenue_Info_Proc: �������� ���ν���
--------------------------------------------------------*/
LRESULT CALLBACK Revenue_Info_Proc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	LVCOLUMN COL;
	switch (iMessage) {
	case WM_CREATE:
		Revenue_I_List = CreateWindow(WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, 20, 20, 800, 400, hWnd, NULL, g_hInst, NULL);
		ListView_SetExtendedListViewStyle(Revenue_I_List, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

		COL.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		COL.fmt = LVCFMT_LEFT;
		COL.cx = 130;

		COL.pszText = (LPSTR)"��ȣ";    //ù ��° ���
		COL.iSubItem = 0;
		SendMessage(Revenue_I_List, LVM_INSERTCOLUMN, 0, (LPARAM)&COL);

		COL.pszText = (LPSTR)"��ǰ�ڵ�";    //�� ��° ���
		COL.iSubItem = 1;
		SendMessage(Revenue_I_List, LVM_INSERTCOLUMN, 1, (LPARAM)&COL);

		COL.pszText = (LPSTR)"����";    //�� ��° ���
		COL.iSubItem = 2;
		SendMessage(Revenue_I_List, LVM_INSERTCOLUMN, 2, (LPARAM)&COL);

		COL.pszText = (LPSTR)"�Ǹų�¥";    //�� ��° ���
		COL.iSubItem = 3;
		SendMessage(Revenue_I_List, LVM_INSERTCOLUMN, 3, (LPARAM)&COL);

		COL.pszText = (LPSTR)"�Ǹż���";    //�ټ� ��° ���
		COL.iSubItem = 4;
		SendMessage(Revenue_I_List, LVM_INSERTCOLUMN, 4, (LPARAM)&COL);

		Load_Revenue_Data();

		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

/*--------------------------------------------------------
 Employee_Info_Proc: �������� ���ν���
--------------------------------------------------------*/
LRESULT CALLBACK Employee_Info_Proc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){
	LVCOLUMN COL;
	switch (iMessage) {
	case WM_CREATE:
		Employee_I_List = CreateWindow(WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, 20, 20, 800, 400, hWnd, NULL, g_hInst, NULL);
		ListView_SetExtendedListViewStyle(Employee_I_List, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

		COL.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		COL.fmt = LVCFMT_LEFT;
		COL.cx = 130;

		COL.pszText = (LPSTR)"���̵�";    //ù ��° ���
		COL.iSubItem = 0;
		SendMessage(Employee_I_List, LVM_INSERTCOLUMN, 0, (LPARAM)&COL);

		COL.pszText = (LPSTR)"�̸�";    //�� ��° ���
		COL.iSubItem = 1;
		SendMessage(Employee_I_List, LVM_INSERTCOLUMN, 1, (LPARAM)&COL);

		COL.pszText = (LPSTR)"��й�ȣ";    //�� ��° ���
		COL.iSubItem = 2;
		SendMessage(Employee_I_List, LVM_INSERTCOLUMN, 2, (LPARAM)&COL);

		COL.pszText = (LPSTR)"����";    //�� ��° ���
		COL.iSubItem = 3;
		SendMessage(Employee_I_List, LVM_INSERTCOLUMN, 3, (LPARAM)&COL);
		
		Load_Employee_Data();
		
		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

/*--------------------------------------------------------
 Customer_Info_Proc: ȸ������ ���ν���
--------------------------------------------------------*/
LRESULT CALLBACK Customer_Info_Proc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case WM_CREATE:
		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}


/*--------------------------------------------------------
 DBConnect(): DB ����
--------------------------------------------------------*/
BOOL DBConnect() {
	// ���� ������ ���� ����
	SQLRETURN Ret;

	// ȯ�� �ڵ��� �Ҵ��ϰ� ���� �Ӽ��� �����Ѵ�.
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS) return FALSE;
	if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS) return FALSE;

	// ���� �ڵ��� �Ҵ��ϰ� �����Ѵ�
	if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS) return FALSE;
	Ret = SQLConnect(hDbc, (SQLCHAR*)"PCDB", SQL_NTS, (SQLCHAR*)ID, SQL_NTS, (SQLCHAR*)PWD, SQL_NTS);
	if ((Ret != SQL_SUCCESS) && (Ret != SQL_SUCCESS_WITH_INFO)) return FALSE;

	// ��� �ڵ��� �Ҵ��Ѵ�.
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS) return FALSE;

	return TRUE;
}


/*--------------------------------------------------------
 DBDisConnect(): DB ����
--------------------------------------------------------*/
void DBDisconnect() {
	// ������
	if (hStmt) SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	if (hDbc) SQLDisconnect(hDbc);
	if (hDbc) SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	if (hEnv) SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}

/*--------------------------------------------------------
 DBExcuteSQL(): ������ ��������
--------------------------------------------------------*/
BOOL DBExcuteSQL() {
	// ������� �����ޱ� ���� ������
	SQLCHAR S_Code[30], S_Name[30], S_Category[30];
	int S_Price, S_Amount;
	double S_Margin;
	SQLLEN I_S_Code, I_S_Name, I_S_Category, I_S_Price, I_S_Amount, I_S_Margin;

	// ȭ�� ����� ���� ������
	TCHAR str[255];

	// ����� �����ޱ� ���� ���ε��Ѵ�.
	SQLBindCol(hStmt, 1, SQL_C_CHAR, S_Code, sizeof(S_Code), &I_S_Code);
	SQLBindCol(hStmt, 2, SQL_C_CHAR, S_Name, sizeof(S_Name), &I_S_Name);
	SQLBindCol(hStmt, 3, SQL_C_ULONG, &S_Price, 0, &I_S_Price);
	SQLBindCol(hStmt, 4, SQL_C_ULONG, &S_Amount, 0, &I_S_Amount);
	SQLBindCol(hStmt, 5, SQL_C_CHAR, S_Category, sizeof(S_Category), &I_S_Category);
	SQLBindCol(hStmt, 6, SQL_C_ULONG, &S_Margin, 0, &I_S_Margin);

	// SQL���� �����Ѵ�
	if (SQLExecDirect(hStmt, (SQLCHAR*)"select S_Code, S_Name, S_Price, S_Amount,S_Category, S_Margin from dbo.Stock_Info", SQL_NTS) != SQL_SUCCESS) return FALSE;

	// �о�� ������ ���
	while (SQLFetch(hStmt) != SQL_NO_DATA) {
		wsprintf(str, "�̸�:%s\t����:%d", S_Name, S_Price);

	}

	if (hStmt) SQLCloseCursor(hStmt);
	return TRUE;
}

/*--------------------------------------------------------
 Load_Stock_Data(): ������� ��������
--------------------------------------------------------*/
BOOL Load_Stock_Data() {
	LVITEM LI;
	SQLCHAR S_Code[30], S_Name[30], S_Category[30];
	int S_Price, S_Amount;
	double S_Margin;
	SQLLEN I_S_Code, I_S_Name, I_S_Category, I_S_Price, I_S_Amount, I_S_Margin;
	TCHAR Price[100], Amount[100], Margin[100];

	SQLBindCol(hStmt, 1, SQL_C_CHAR, S_Code, sizeof(S_Code), &I_S_Code);
	SQLBindCol(hStmt, 2, SQL_C_CHAR, S_Name, sizeof(S_Name), &I_S_Name);
	SQLBindCol(hStmt, 3, SQL_C_ULONG, &S_Price, 0, &I_S_Price);
	SQLBindCol(hStmt, 4, SQL_C_ULONG, &S_Amount, 0, &I_S_Amount);
	SQLBindCol(hStmt, 5, SQL_C_CHAR, S_Category, sizeof(S_Category), &I_S_Category);
	SQLBindCol(hStmt, 6, SQL_C_DOUBLE, &S_Margin, 0, &I_S_Margin);

	if (SQLExecDirect(hStmt, (SQLCHAR*)"select S_Code, S_Name, S_Price, S_Amount, S_Category ,S_Margin from dbo.Stock_Info", SQL_NTS) != SQL_SUCCESS) return FALSE;

	LI.mask = LVIF_TEXT;

	while (SQLFetch(hStmt) != SQL_NO_DATA) {
		// ������
		LI.iItem = 0;
		LI.iSubItem = 0;
		LI.pszText = (LPSTR)S_Code;
		SendMessage(Stock_I_List, LVM_INSERTITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 1;
		LI.pszText = (LPSTR)S_Name;
		SendMessage(Stock_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 2;
		wsprintf(Price, "%d", S_Price);
		LI.pszText = Price;
		SendMessage(Stock_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 3;
		wsprintf(Amount, "%d", S_Amount);
		LI.pszText = Amount;
		SendMessage(Stock_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 4;
		LI.pszText = (LPSTR)S_Category;
		SendMessage(Stock_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 5;
		sprintf_s(Margin, "%.2f(%%)", S_Margin);
		LI.pszText = Margin;
		SendMessage(Stock_I_List, LVM_SETITEM, 0, (LPARAM)&LI);
	}

	if (hStmt) SQLCloseCursor(hStmt);
	return TRUE;
}

/*--------------------------------------------------------
 Load_Revenue_Data(): �������� ��������
--------------------------------------------------------*/
BOOL Load_Revenue_Data() {
	LVITEM LI;
	SQLCHAR S_Code[30], E_ID[30];
	TIMESTAMP_STRUCT R_Date;
	int R_No, R_Amount;
	SQLLEN I_R_No, I_S_Code, I_E_ID, I_R_Date, I_R_Amount;
	TCHAR No[100], Amount[100], Date[100];

	SQLBindCol(hStmt, 1, SQL_C_ULONG, &R_No, 0, &I_R_No);
	SQLBindCol(hStmt, 2, SQL_C_CHAR, S_Code, sizeof(S_Code), &I_S_Code);
	SQLBindCol(hStmt, 3, SQL_C_CHAR, E_ID, sizeof(E_ID), &I_E_ID);
	SQLBindCol(hStmt, 4, SQL_C_TYPE_TIMESTAMP, &R_Date, sizeof(R_Date), &I_R_Date);
	SQLBindCol(hStmt, 5, SQL_C_ULONG, &R_Amount, 0, &I_R_Amount);

	if (SQLExecDirect(hStmt, (SQLCHAR*)"select R_No, S_Code, E_ID, R_Date, R_Amount from dbo.Revenue_Info", SQL_NTS) != SQL_SUCCESS) return FALSE;

	LI.mask = LVIF_TEXT;

	while (SQLFetch(hStmt) != SQL_NO_DATA) {
		// ������
		LI.iItem = 0;
		LI.iSubItem = 0;
		wsprintf(No, "%d", R_No);
		LI.pszText = No;
		SendMessage(Revenue_I_List, LVM_INSERTITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 1;
		LI.pszText = (LPSTR)S_Code;
		SendMessage(Revenue_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 2;
		LI.pszText = (LPSTR)E_ID;
		SendMessage(Revenue_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 3;
		wsprintf(Date, "%d-%d-%d %d:%d:%d", R_Date.year, R_Date.month, R_Date.day, R_Date.hour, R_Date.minute, R_Date.second);
		LI.pszText = Date;
		SendMessage(Revenue_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 4;
		wsprintf(Amount, "%d", R_Amount);
		LI.pszText = Amount;
		SendMessage(Revenue_I_List, LVM_SETITEM, 0, (LPARAM)&LI);
	}

	if (hStmt) SQLCloseCursor(hStmt);
	return TRUE;
}

/*--------------------------------------------------------
 Load_Employee_Data(): �������� ��������
--------------------------------------------------------*/
BOOL Load_Employee_Data(){
	LVITEM LI;
	SQLCHAR E_ID[30], E_Name[30], E_PWD[30], E_Permission;
	SQLLEN I_E_ID, I_E_Name, I_E_PWD, I_E_Permission;
	TCHAR Permission[30];

	SQLBindCol(hStmt, 1, SQL_C_CHAR, E_ID, sizeof(E_ID), &I_E_ID);
	SQLBindCol(hStmt, 2, SQL_C_CHAR, E_Name, sizeof(E_Name), &I_E_Name);
	SQLBindCol(hStmt, 3, SQL_C_CHAR, E_PWD, sizeof(E_PWD), &I_E_PWD);
	SQLBindCol(hStmt, 4, SQL_C_BIT, &E_Permission, sizeof(E_Permission), &I_E_Permission);

	if (SQLExecDirect(hStmt, (SQLCHAR*)"select E_ID, E_Name, E_PWD, E_Permission from dbo.Employee_Info", SQL_NTS) != SQL_SUCCESS) return FALSE;

	LI.mask = LVIF_TEXT;

	while (SQLFetch(hStmt) != SQL_NO_DATA) {
		// ������
		LI.iItem = 0;
		LI.iSubItem = 0;
		LI.pszText = (LPSTR)E_ID;
		SendMessage(Employee_I_List, LVM_INSERTITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 1;
		LI.pszText = (LPSTR)E_Name;
		SendMessage(Employee_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 2;
		LI.pszText = (LPSTR)E_PWD;
		SendMessage(Employee_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 3;
		wsprintf(Permission, "%d", E_Permission);
		if (lstrcmp(Permission, "1") == 0) {
			LI.pszText = (LPSTR)"True";
		}
		else {
			LI.pszText = (LPSTR)"False";
		}
		SendMessage(Employee_I_List, LVM_SETITEM, 0, (LPARAM)&LI);
	}

	if (hStmt) SQLCloseCursor(hStmt);
	return TRUE;
}

/*--------------------------------------------------------
 Load_Customer_Data(): ȸ������ ��������
--------------------------------------------------------*/
BOOL Load_Customer_Data() {
	// ����� �ٲ�ߵ˴ϴ�.
	LVITEM LI;
	SQLCHAR S_Code[30], S_Name[30], S_Category[30];
	int S_Price, S_Amount;
	double S_Margin;
	SQLLEN I_S_Code, I_S_Name, I_S_Category, I_S_Price, I_S_Amount, I_S_Margin;
	TCHAR Price[100], Amount[100], Margin[100];

	SQLBindCol(hStmt, 1, SQL_C_CHAR, S_Code, sizeof(S_Code), &I_S_Code);
	SQLBindCol(hStmt, 2, SQL_C_CHAR, S_Name, sizeof(S_Name), &I_S_Name);
	SQLBindCol(hStmt, 3, SQL_C_ULONG, &S_Price, 0, &I_S_Price);
	SQLBindCol(hStmt, 4, SQL_C_ULONG, &S_Amount, 0, &I_S_Amount);
	SQLBindCol(hStmt, 5, SQL_C_CHAR, S_Category, sizeof(S_Category), &I_S_Category);
	SQLBindCol(hStmt, 6, SQL_C_DOUBLE, &S_Margin, 0, &I_S_Margin);

	if (SQLExecDirect(hStmt, (SQLCHAR*)"select S_Code, S_Name, S_Price, S_Amount, S_Category ,S_Margin from dbo.Stock_Info", SQL_NTS) != SQL_SUCCESS) return FALSE;

	LI.mask = LVIF_TEXT;

	while (SQLFetch(hStmt) != SQL_NO_DATA) {
		//ù��°������
		LI.iItem = 0;
		LI.iSubItem = 0;
		LI.pszText = (LPSTR)S_Code;
		SendMessage(Stock_I_List, LVM_INSERTITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 1;
		LI.pszText = (LPSTR)S_Name;
		SendMessage(Stock_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 2;
		wsprintf(Price, "%d", S_Price);
		LI.pszText = Price;
		SendMessage(Stock_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 3;
		wsprintf(Amount, "%d", S_Amount);
		LI.pszText = Amount;
		SendMessage(Stock_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 4;
		LI.pszText = (LPSTR)S_Category;
		SendMessage(Stock_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 5;
		sprintf_s(Margin, "%.2f(%%)", S_Margin);
		LI.pszText = Margin;
		SendMessage(Stock_I_List, LVM_SETITEM, 0, (LPARAM)&LI);
	}

	if (hStmt) SQLCloseCursor(hStmt);
	return TRUE;
}

/*--------------------------------------------------------
 create(): ���� ����ü �Ҵ�
--------------------------------------------------------*/
CS* create() {
	CS* N;

	N = (CS*)malloc(sizeof(CS));

	lstrcpy(N->ID, TEXT(""));
	N->Client_Sock = 0;
	N->link = NULL;

	return N;
}

/*--------------------------------------------------------
 SVR_Open(): ���� ����
--------------------------------------------------------*/
void SVR_Open() {
	static BOOL state = TRUE;
	HANDLE hThread;
	DWORD ThreadID;

	if (state) {
		// ���� �ʱ�ȭ (���� ���̺귯�� ����, ���� �ý��� ���� ����)
		nReturn = WSAStartup(WORD(2.0), &wsadata);

		// ���� ���� (IPv4: AF_INET | IPv6: AF_INET6 , ���� ��� Ÿ��, �������� ����)
		listensock = socket(AF_INET, SOCK_STREAM, 0);

		addr_server.sin_family = AF_INET;
		addr_server.sin_addr.s_addr = htons(INADDR_ANY);
		addr_server.sin_port = htons(g_uPort);

		// ���� ���ε� (���� ��ü, �Ұ� ��ü�� �ο��� �ּ� ���� ����ü, ����ü ����)
		nReturn = bind(listensock, (sockaddr*)&addr_server, sizeof(sockaddr));

		// ���� ��� (���� ��ü, ���� ��⿭ ũ��)
		nReturn = listen(listensock, 1);

		// accept ������ ����
		CloseHandle(hThread = CreateThread(NULL, 0, Connect_Thread, &listensock, 0, &ThreadID));

		CONN_ST = TRUE;

		state = FALSE;
	}
	InvalidateRect(hWndMain, NULL, FALSE);
	MessageBox(hWndMain, "�������¿Ϸ�", "�˸�", MB_OK);
}


/*--------------------------------------------------------
 Connect_Thread(LPVOID): ���� ���� ������
--------------------------------------------------------*/
DWORD WINAPI Connect_Thread(LPVOID Param) {
	SOCKET* P = (SOCKET*)Param;
	HANDLE hThread;
	DWORD ThreadID;
	CS* S;

	// ���� ����ϱ� (���� ��ü, Ŭ���̾�Ʈ �ּ� ����, �ּ� ���� ���� ũ��)
	for (;;) {
		clientsock = accept(*P, (sockaddr*)&addr_client, &addrlen_clt);

		if (clientsock != INVALID_SOCKET) {
			MessageBox(hWndMain, "Ŭ���̾�Ʈ�� �����Ͽ����ϴ�.", "server", MB_OK);

			addsock(clientsock);

			S = C_S;
			while (S->link != NULL) {
				S = S->link;
			}

			CloseHandle(hThread = CreateThread(NULL, 0, Recv_Thread, &(S->Client_Sock), 0, &ThreadID));
		}

	}
	return 0;
}

/*--------------------------------------------------------
 Recv_Thread(LPVOID): ���� ������
--------------------------------------------------------*/
DWORD WINAPI Recv_Thread(LPVOID Param) {
	SOCKET* P = (SOCKET*)Param;


	static int i;

	while (1) {
		// ������ ���� (��Ĺ ��ü, ���� ���ڿ�, ���ڿ� ũ��, �ɼ�);
		nReturn = recv(*P, buf, buflen, 0);

		if (nReturn == 0 || nReturn == SOCKET_ERROR) {
			MessageBox(hWndMain, "���ſ���", "server", MB_OK);
			return 0;
		}
		else {
		}
	}
	return 0;
}

/*--------------------------------------------------------
 addsock(SOCKET): ���� ���� �߰�
--------------------------------------------------------*/
void addsock(SOCKET S) {
	CS* N, * P;

	N = create();
	N->Client_Sock = S;

	P = C_S;

	while (P->link != NULL) {
		P = P->link;
	}

	P->link = N;

}

/*--------------------------------------------------------
 delsock(SOCKET): ���� ���� ����
--------------------------------------------------------*/
void delsock(SOCKET S) {
	if (C_S->link != NULL) {
		CS* E, * P;
		P = C_S;

		while (P->link != NULL) {
			E = P;
			P = P->link;
			if (P->Client_Sock == S) {
				E->link = P->link;
				break;
			}
		}
		free(P);
	}
}