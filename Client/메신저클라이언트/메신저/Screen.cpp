#include "Common.h"
#include "Screen.h"
extern HWND hWnd;
extern HINSTANCE g_hInst;
//ȭ����(0:�ڸ�����/1:�α���/2:���� �� ����/3:�ֹ�/4:���) ��������
extern int selwindow;
//�ֹ�����(���� �ð� 0�϶�)
extern int selafterorder;																			

// ���� ��ư(�ڸ�����)
extern HWND hButton_SeatNum0;
extern HWND hButton_SeatNum1;
extern HWND hButton_SeatNum2;
extern HWND hButton_SeatNum3;
extern HWND hButton_SeatNum4;
extern HWND hButton_SeatNum5;
extern HWND hButton_SeatNum6;
extern HWND hButton_SeatNum7;
extern HWND hButton_SeatNum8;
extern HWND hButton_SeatNum9;

// ���� ��ư(�ֹ�����)
extern HWND hButton_OrderCount0;
extern HWND hButton_OrderCount1;
extern HWND hButton_OrderCount2;
extern HWND hButton_OrderCount3;
extern HWND hButton_OrderCount4;
extern HWND hButton_OrderCount5;
extern HWND hButton_OrderCount6;
extern HWND hButton_OrderCount7;
extern HWND hButton_OrderCount8;
extern HWND hButton_OrderCount9;
//


//�ڸ�����
extern HWND hStatic_Sn;
extern HWND hEdit_Sn;
extern HWND hButton_SelSeat;
//�α���
extern HWND hStatic_Login_Id;
extern HWND hEdit_Login_Id;
extern HWND hStatic_Login_Pw;
extern HWND hEdit_Login_Pw;
extern HWND hButton_Login;
extern HWND hButton_Go_Join_Or_Update;

//���ȭ��
extern HWND hButton_Go_Order;
extern HWND hButton_Logout;
extern HWND hButton_Delete;
extern HWND hEdit_Timer;
extern HWND hStatic_Timer;

//�ֹ�ȭ��
extern HWND hStatic_Item;
extern HWND hListBox_ItemList;
extern HWND hStatic_OrderCount;
extern HWND hEdit_OrderCount;
extern HWND hButton_Order;

//���� �� ����
extern HWND hStatic_Id;
extern HWND hEdit_Id;
extern HWND hStatic_Pw;
extern HWND hEdit_Pw;
extern HWND hStatic_Name;
extern HWND hEdit_Name;
extern HWND hStatic_Pn;
extern HWND hEdit_Pn;
extern HWND hStatic_Addr;
extern HWND hEdit_Addr;
extern HWND hStatic_Birth;
extern HWND hEdit_Birth;
extern HWND hButton_Join;
extern HWND hButton_Update;
//void SelSeatToLogin():�ڸ�����ȭ��->�α���ȭ��
void SelSeatToLogin() {
	//�ڸ�����ȭ��->�α���ȭ��
	selwindow = 1;								//�α���ȭ������ ����
	//�ڸ�����ȭ�� ����
	if (selwindow != 0)ShowWindow(hStatic_Sn, SW_HIDE);
	if (selwindow != 0)ShowWindow(hEdit_Sn, SW_HIDE);
	if (selwindow != 0)ShowWindow(hButton_SelSeat, SW_HIDE);

	if (selwindow != 0)ShowWindow(hButton_SeatNum0, SW_HIDE);
	if (selwindow != 0)ShowWindow(hButton_SeatNum1, SW_HIDE);
	if (selwindow != 0)ShowWindow(hButton_SeatNum2, SW_HIDE);
	if (selwindow != 0)ShowWindow(hButton_SeatNum3, SW_HIDE);
	if (selwindow != 0)ShowWindow(hButton_SeatNum4, SW_HIDE);
	if (selwindow != 0)ShowWindow(hButton_SeatNum5, SW_HIDE);
	if (selwindow != 0)ShowWindow(hButton_SeatNum6, SW_HIDE);
	if (selwindow != 0)ShowWindow(hButton_SeatNum7, SW_HIDE);
	if (selwindow != 0)ShowWindow(hButton_SeatNum8, SW_HIDE);
	if (selwindow != 0)ShowWindow(hButton_SeatNum9, SW_HIDE);
	InvalidateRect(hWnd, NULL, TRUE);
	//�α���ȭ�� �ѱ�
	/////�α���ȭ�� ��Ʈ��
//�α��� ���̵� ����ƽ
	hStatic_Login_Id = CreateWindow(TEXT("static"), TEXT("ID"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 0, 80, 20, hWnd, (HMENU)ID_S_LOGIN_ID, g_hInst, NULL);
	if (selwindow != 1)ShowWindow(hStatic_Login_Id, SW_HIDE);
	//�α��� ���̵� ����Ʈ
	hEdit_Login_Id = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 50, 100, 50, hWnd, (HMENU)ID_E_LOGIN_ID, g_hInst, NULL);
	if (selwindow != 1)ShowWindow(hEdit_Login_Id, SW_HIDE);
	//�α��� ��� ����ƽ
	hStatic_Login_Pw = CreateWindow(TEXT("static"), TEXT("PW"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 150, 40, 20, hWnd, (HMENU)ID_S_LOGIN_PW, g_hInst, NULL);
	if (selwindow != 1)ShowWindow(hStatic_Login_Pw, SW_HIDE);
	//�α��� ��� ����Ʈ
	hEdit_Login_Pw = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 200, 100, 50, hWnd, (HMENU)ID_E_LOGIN_PW, g_hInst, NULL);
	if (selwindow != 1)ShowWindow(hEdit_Login_Pw, SW_HIDE);
	//�α��� ��ư
	hButton_Login = CreateWindow(TEXT("button"), TEXT("LOGIN"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 10, 200, 50, hWnd, (HMENU)ID_B_LOGIN, g_hInst, NULL);
	if (selwindow != 1)ShowWindow(hButton_Login, SW_HIDE);

	//����/���� ���� ��ư
	hButton_Go_Join_Or_Update = CreateWindow(TEXT("button"), TEXT("JOIN/UPDATE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 100, 200, 50, hWnd, (HMENU)ID_B_GO_JOIN_OR_UPDATE, g_hInst, NULL);;
	if (selwindow != 1)ShowWindow(hButton_Go_Join_Or_Update, SW_HIDE);
	///
}
//void LoginToUser() :�α���ȭ�鿡�� ���ȭ������
void LoginToUser() {
	selwindow = 4;							//���ȭ��
	InvalidateRect(hWnd, NULL, TRUE);
	//�α���ȭ�� ����
	if (selwindow != 1)ShowWindow(hStatic_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hStatic_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Login, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Join_Or_Update, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Order, SW_HIDE);
	//���ȭ�� �ѱ�
	///���ȭ�� ��Ʈ��
	//�ֹ����� ��ư
	hButton_Go_Order = CreateWindow(TEXT("button"), TEXT("ORDER"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 200, 200, 50, hWnd, (HMENU)ID_B_GO_ORDER, g_hInst, NULL);
	if (selwindow != 4)ShowWindow(hButton_Go_Order, SW_HIDE);
	///
//�α׾ƿ� ��ư
	hButton_Logout = CreateWindow(TEXT("button"), TEXT("LOGOUT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		610, 50, 200, 65, hWnd, (HMENU)ID_B_LOGOUT, g_hInst, NULL);
	if (selwindow != 4)ShowWindow(hButton_Logout, SW_HIDE);
	//Ż�� ��ư
	hButton_Delete = CreateWindow(TEXT("button"), TEXT("DELETE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 50, 200, 65, hWnd, (HMENU)ID_B_DELETE, g_hInst, NULL);
	if (selwindow != 4)ShowWindow(hButton_Delete, SW_HIDE);
	//Ÿ�̸� ����Ʈ
	hEdit_Timer = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		610, 200, 100, 100, hWnd, (HMENU)ID_E_TIMER, g_hInst, NULL);
	if (selwindow != 4)ShowWindow(hEdit_Timer, SW_HIDE);
	//Ÿ�̸� ����ƽ
	hStatic_Timer = CreateWindow(TEXT("static"), TEXT("TIME"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		610, 150, 40, 20, hWnd, (HMENU)ID_S_TIMER, g_hInst, NULL);
	if (selwindow != 4)ShowWindow(hStatic_Timer, SW_HIDE);
	///
}
//void OrderToLoginOrUser():�ֹ����� �����ð����������� �α���ȭ��/���ȭ������
void OrderToLoginOrUser() {
	if (selafterorder == 1)selwindow = 1;				//���� �ð� 0�϶� �α���ȭ������ ����
	else selwindow = 4;											//..�ƴҶ� ���ȭ������ ����
	InvalidateRect(hWnd, NULL, TRUE);
	//�ֹ�ȭ�� ����
	if (selwindow != 3)ShowWindow(hStatic_Item, SW_HIDE);
	if (selwindow != 3)ShowWindow(hListBox_ItemList, SW_HIDE);
	if (selwindow != 3)ShowWindow(hStatic_OrderCount, SW_HIDE);
	if (selwindow != 3)ShowWindow(hEdit_OrderCount, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_Order, SW_HIDE);

	if (selwindow != 3)ShowWindow(hButton_OrderCount0, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount1, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount2, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount3, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount4, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount5, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount6, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount7, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount8, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount9, SW_HIDE);
	//�����ð� 0�϶� �α���ȭ������ 
	if (selafterorder == 1) {
		selafterorder = 0;
		/////�α���ȭ�� ��Ʈ��
//�α��� ���̵� ����ƽ
		hStatic_Login_Id = CreateWindow(TEXT("static"), TEXT("ID"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 0, 80, 20, hWnd, (HMENU)ID_S_LOGIN_ID, g_hInst, NULL);
		if (selwindow != 1)ShowWindow(hStatic_Login_Id, SW_HIDE);
		//�α��� ���̵� ����Ʈ
		hEdit_Login_Id = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 50, 100, 50, hWnd, (HMENU)ID_E_LOGIN_ID, g_hInst, NULL);
		if (selwindow != 1)ShowWindow(hEdit_Login_Id, SW_HIDE);
		//�α��� ��� ����ƽ
		hStatic_Login_Pw = CreateWindow(TEXT("static"), TEXT("PW"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 150, 40, 20, hWnd, (HMENU)ID_S_LOGIN_PW, g_hInst, NULL);
		if (selwindow != 1)ShowWindow(hStatic_Login_Pw, SW_HIDE);
		//�α��� ��� ����Ʈ
		hEdit_Login_Pw = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 200, 100, 50, hWnd, (HMENU)ID_E_LOGIN_PW, g_hInst, NULL);
		if (selwindow != 1)ShowWindow(hEdit_Login_Pw, SW_HIDE);
		//�α��� ��ư
		hButton_Login = CreateWindow(TEXT("button"), TEXT("LOGIN"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 10, 200, 50, hWnd, (HMENU)ID_B_LOGIN, g_hInst, NULL);
		if (selwindow != 1)ShowWindow(hButton_Login, SW_HIDE);

		//����/���� ���� ��ư
		hButton_Go_Join_Or_Update = CreateWindow(TEXT("button"), TEXT("JOIN/UPDATE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 100, 200, 50, hWnd, (HMENU)ID_B_GO_JOIN_OR_UPDATE, g_hInst, NULL);;
		if (selwindow != 1)ShowWindow(hButton_Go_Join_Or_Update, SW_HIDE);
	}
	//���� �ð������� ���ȭ������
	else {
		///���ȭ�� ��Ʈ��
		//�ֹ����� ��ư
		hButton_Go_Order = CreateWindow(TEXT("button"), TEXT("ORDER"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 200, 200, 50, hWnd, (HMENU)ID_B_GO_ORDER, g_hInst, NULL);
		if (selwindow != 4)ShowWindow(hButton_Go_Order, SW_HIDE);
		///
//�α׾ƿ� ��ư
		hButton_Logout = CreateWindow(TEXT("button"), TEXT("LOGOUT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			610, 50, 200, 65, hWnd, (HMENU)ID_B_LOGOUT, g_hInst, NULL);
		if (selwindow != 4)ShowWindow(hButton_Logout, SW_HIDE);
		//Ż�� ��ư
		hButton_Delete = CreateWindow(TEXT("button"), TEXT("DELETE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 50, 200, 65, hWnd, (HMENU)ID_B_DELETE, g_hInst, NULL);
		if (selwindow != 4)ShowWindow(hButton_Delete, SW_HIDE);
		//Ÿ�̸� ����Ʈ
		hEdit_Timer = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			610, 200, 100, 100, hWnd, (HMENU)ID_E_TIMER, g_hInst, NULL);
		if (selwindow != 4)ShowWindow(hEdit_Timer, SW_HIDE);
		//Ÿ�̸� ����ƽ
		hStatic_Timer = CreateWindow(TEXT("static"), TEXT("TIME"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			610, 150, 40, 20, hWnd, (HMENU)ID_S_TIMER, g_hInst, NULL);
		if (selwindow != 4)ShowWindow(hStatic_Timer, SW_HIDE);
		///
	}
}
//void LoginToOrder():�α��ο��� �ֹ�ȭ������
void LoginToOrder() {
	selwindow = 3;																		//�ֹ�ȭ������
	InvalidateRect(hWnd, NULL, TRUE);
	//�α���ȭ�� ����
	if (selwindow != 1)ShowWindow(hStatic_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hStatic_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Login, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Join_Or_Update, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Order, SW_HIDE);
	//�ֹ�ȭ�� �ѱ�
	///�ֹ�ȭ�� ��Ʈ��
//������ ����ƽ
	hStatic_Item = CreateWindow(TEXT("static"), TEXT("ITEM"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 0, 40, 20, hWnd, (HMENU)ID_S_ITEM, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hStatic_Item, SW_HIDE);
	//�����۸���Ʈ ����Ʈ�ڽ�
	hListBox_ItemList = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_SORT | LBS_NOTIFY,
		10, 100, 400, 200, hWnd, (HMENU)ID_L_ITEM, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hListBox_ItemList, SW_HIDE);
	//�������ֹ����� ����ƽ
	hStatic_OrderCount = CreateWindow(TEXT("static"), TEXT("Count(2�ڸ�)"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		510, 0, 100, 20, hWnd, (HMENU)ID_S_ORDERCOUNT, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hStatic_OrderCount, SW_HIDE);
	//�������ֹ����� ����Ʈ
	hEdit_OrderCount = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		510, 50, 100, 50, hWnd, (HMENU)ID_E_ORDERCOUNT, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hEdit_OrderCount, SW_HIDE);
	//�ֹ� ��ư
	hButton_Order = CreateWindow(TEXT("button"), TEXT("ORDER"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 10, 200, 50, hWnd, (HMENU)ID_B_ORDER, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hButton_Order, SW_HIDE);
	///
	//�ֹ����� ��ư
	hButton_OrderCount0 = CreateWindow(TEXT("button"), TEXT("0"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT0, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hButton_OrderCount0, SW_HIDE);
	hButton_OrderCount1 = CreateWindow(TEXT("button"), TEXT("1"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT1, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hButton_OrderCount1, SW_HIDE);
	hButton_OrderCount2 = CreateWindow(TEXT("button"), TEXT("2"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		200, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT2, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hButton_OrderCount2, SW_HIDE);
	hButton_OrderCount3 = CreateWindow(TEXT("button"), TEXT("3"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		300, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT3, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hButton_OrderCount3, SW_HIDE);
	hButton_OrderCount4 = CreateWindow(TEXT("button"), TEXT("4"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		400, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT4, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hButton_OrderCount4, SW_HIDE);
	hButton_OrderCount5 = CreateWindow(TEXT("button"), TEXT("5"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		500, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT5, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hButton_OrderCount5, SW_HIDE);
	hButton_OrderCount6 = CreateWindow(TEXT("button"), TEXT("6"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		600, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT6, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hButton_OrderCount6, SW_HIDE);
	hButton_OrderCount7 = CreateWindow(TEXT("button"), TEXT("7"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		700, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT7, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hButton_OrderCount7, SW_HIDE);
	hButton_OrderCount8 = CreateWindow(TEXT("button"), TEXT("8"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		800, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT8, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hButton_OrderCount8, SW_HIDE);
	hButton_OrderCount9 = CreateWindow(TEXT("button"), TEXT("9"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		900, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT9, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hButton_OrderCount9, SW_HIDE);
	///

	//�����۸���Ʈ �ʱ�ȭ
	SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"DRINK");
	SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"FOOD");
	SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"PC");
}
//void JoinOrUpdateToLogin():���� �� �������� �α�������
void JoinOrUpdateToLogin() {
	selwindow = 1;												//�α���ȭ�� ����
	InvalidateRect(hWnd, NULL, TRUE);
	//���� �� ����ȭ�� ����
	if (selwindow != 2)ShowWindow(hStatic_Id, SW_HIDE);
	if (selwindow != 2)ShowWindow(hEdit_Id, SW_HIDE);
	if (selwindow != 2)ShowWindow(hStatic_Pw, SW_HIDE);
	if (selwindow != 2)ShowWindow(hEdit_Pw, SW_HIDE);
	if (selwindow != 2)ShowWindow(hStatic_Name, SW_HIDE);
	if (selwindow != 2)ShowWindow(hEdit_Name, SW_HIDE);
	if (selwindow != 2)ShowWindow(hStatic_Pn, SW_HIDE);
	if (selwindow != 2)ShowWindow(hEdit_Pn, SW_HIDE);
	if (selwindow != 2)ShowWindow(hStatic_Addr, SW_HIDE);
	if (selwindow != 2)ShowWindow(hEdit_Addr, SW_HIDE);
	if (selwindow != 2)ShowWindow(hStatic_Birth, SW_HIDE);
	if (selwindow != 2)ShowWindow(hEdit_Birth, SW_HIDE);
	if (selwindow != 2)ShowWindow(hButton_Join, SW_HIDE);
	if (selwindow != 2)ShowWindow(hButton_Update, SW_HIDE);
	//�α���ȭ�� �ѱ�
	/////�α���ȭ�� ��Ʈ��
//�α��� ���̵� ����ƽ
	hStatic_Login_Id = CreateWindow(TEXT("static"), TEXT("ID"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 0, 80, 20, hWnd, (HMENU)ID_S_LOGIN_ID, g_hInst, NULL);
	if (selwindow != 1)ShowWindow(hStatic_Login_Id, SW_HIDE);
	//�α��� ���̵� ����Ʈ
	hEdit_Login_Id = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 50, 100, 50, hWnd, (HMENU)ID_E_LOGIN_ID, g_hInst, NULL);
	if (selwindow != 1)ShowWindow(hEdit_Login_Id, SW_HIDE);
	//�α��� ��� ����ƽ
	hStatic_Login_Pw = CreateWindow(TEXT("static"), TEXT("PW"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 150, 40, 20, hWnd, (HMENU)ID_S_LOGIN_PW, g_hInst, NULL);
	if (selwindow != 1)ShowWindow(hStatic_Login_Pw, SW_HIDE);
	//�α��� ��� ����Ʈ
	hEdit_Login_Pw = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 200, 100, 50, hWnd, (HMENU)ID_E_LOGIN_PW, g_hInst, NULL);
	if (selwindow != 1)ShowWindow(hEdit_Login_Pw, SW_HIDE);
	//�α��� ��ư
	hButton_Login = CreateWindow(TEXT("button"), TEXT("LOGIN"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 10, 200, 50, hWnd, (HMENU)ID_B_LOGIN, g_hInst, NULL);
	if (selwindow != 1)ShowWindow(hButton_Login, SW_HIDE);

	//����/���� ���� ��ư
	hButton_Go_Join_Or_Update = CreateWindow(TEXT("button"), TEXT("JOIN/UPDATE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 100, 200, 50, hWnd, (HMENU)ID_B_GO_JOIN_OR_UPDATE, g_hInst, NULL);;
	if (selwindow != 1)ShowWindow(hButton_Go_Join_Or_Update, SW_HIDE);
}
//void ToSelSeat():�ڸ�����ȭ������
void ToSelSeat() {
	selwindow = 0;																						//�ڸ�����ȭ�� ����
	InvalidateRect(hWnd, NULL, TRUE);
	//���ȭ�� ����
	if (selwindow != 4)ShowWindow(hButton_Go_Order, SW_HIDE);
	if (selwindow != 4)ShowWindow(hButton_Logout, SW_HIDE);
	if (selwindow != 4)ShowWindow(hButton_Delete, SW_HIDE);
	if (selwindow != 4)ShowWindow(hEdit_Timer, SW_HIDE);
	if (selwindow != 4)ShowWindow(hStatic_Timer, SW_HIDE);
	//�ֹ�ȭ�� ����
	if (selwindow != 3)ShowWindow(hStatic_Item, SW_HIDE);
	if (selwindow != 3)ShowWindow(hListBox_ItemList, SW_HIDE);
	if (selwindow != 3)ShowWindow(hStatic_OrderCount, SW_HIDE);
	if (selwindow != 3)ShowWindow(hEdit_OrderCount, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_Order, SW_HIDE);

	if (selwindow != 3)ShowWindow(hButton_OrderCount0, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount1, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount2, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount3, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount4, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount5, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount6, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount7, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount8, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount9, SW_HIDE);
	//�α���ȭ�� ����
	if (selwindow != 1)ShowWindow(hStatic_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hStatic_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Login, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Join_Or_Update, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Order, SW_HIDE);
	//�ڸ�����ȭ�� �ѱ�
	///�ڸ�����ȭ�� ��Ʈ��

//�ڸ���ȣ ����ƽ
	hStatic_Sn = CreateWindow(TEXT("static"), TEXT("SN(2�ڸ�)"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 0, 80, 20, hWnd, (HMENU)ID_S_SN, g_hInst, NULL);
	if (selwindow != 0)ShowWindow(hStatic_Sn, SW_HIDE);
	//�ڸ���ȣ ����Ʈ
	hEdit_Sn = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 50, 100, 50, hWnd, (HMENU)ID_E_SN, g_hInst, NULL);
	if (selwindow != 0)ShowWindow(hEdit_Sn, SW_HIDE);
	//�ڸ����� ��ư
	hButton_SelSeat = CreateWindow(TEXT("button"), TEXT("SELECTSEAT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 10, 200, 50, hWnd, (HMENU)ID_B_SELSEAT, g_hInst, NULL);
	if (selwindow != 0)ShowWindow(hButton_SelSeat, SW_HIDE);

	//�ڸ���ȣ ��ư
	hButton_SeatNum0 = CreateWindow(TEXT("button"), TEXT("0"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM0, g_hInst, NULL);
	if (selwindow != 0)ShowWindow(hButton_SeatNum0, SW_HIDE);
	hButton_SeatNum1 = CreateWindow(TEXT("button"), TEXT("1"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM1, g_hInst, NULL);
	if (selwindow != 0)ShowWindow(hButton_SeatNum1, SW_HIDE);
	hButton_SeatNum2 = CreateWindow(TEXT("button"), TEXT("2"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		200, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM2, g_hInst, NULL);
	if (selwindow != 0)ShowWindow(hButton_SeatNum2, SW_HIDE);
	hButton_SeatNum3 = CreateWindow(TEXT("button"), TEXT("3"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		300, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM3, g_hInst, NULL);
	if (selwindow != 0)ShowWindow(hButton_SeatNum3, SW_HIDE);
	hButton_SeatNum4 = CreateWindow(TEXT("button"), TEXT("4"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		400, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM4, g_hInst, NULL);
	if (selwindow != 0)ShowWindow(hButton_SeatNum4, SW_HIDE);
	hButton_SeatNum5 = CreateWindow(TEXT("button"), TEXT("5"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		500, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM5, g_hInst, NULL);
	if (selwindow != 0)ShowWindow(hButton_SeatNum5, SW_HIDE);
	hButton_SeatNum6 = CreateWindow(TEXT("button"), TEXT("6"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		600, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM6, g_hInst, NULL);
	if (selwindow != 0)ShowWindow(hButton_SeatNum6, SW_HIDE);
	hButton_SeatNum7 = CreateWindow(TEXT("button"), TEXT("7"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		700, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM7, g_hInst, NULL);
	if (selwindow != 0)ShowWindow(hButton_SeatNum7, SW_HIDE);
	hButton_SeatNum8 = CreateWindow(TEXT("button"), TEXT("8"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		800, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM8, g_hInst, NULL);
	if (selwindow != 0)ShowWindow(hButton_SeatNum8, SW_HIDE);
	hButton_SeatNum9 = CreateWindow(TEXT("button"), TEXT("9"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		900, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM9, g_hInst, NULL);
	if (selwindow != 0)ShowWindow(hButton_SeatNum9, SW_HIDE);
}
//void LoginToJoinOrUpdate():�α��ο��� ���� �� ��������
void LoginToJoinOrUpdate() {
	selwindow = 2;																																	//���� �� ����ȭ������ ����
	InvalidateRect(hWnd, NULL, TRUE);
	//�α���ȭ�� ����
	if (selwindow != 1)ShowWindow(hStatic_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hStatic_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Login, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Join_Or_Update, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Order, SW_HIDE);
	//���� �� ����ȭ������ �ѱ�
	///���� �� ����ȭ�� ��Ʈ��
//���̵� ����ƽ
	hStatic_Id = CreateWindow(TEXT("static"), TEXT("ID"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 0, 40, 20, hWnd, (HMENU)ID_S_ID, g_hInst, NULL);
	if (selwindow != 2)ShowWindow(hStatic_Id, SW_HIDE);
	//���̵� ����Ʈ
	hEdit_Id = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 50, 100, 50, hWnd, (HMENU)ID_E_ID, g_hInst, NULL);
	if (selwindow != 2)ShowWindow(hEdit_Id, SW_HIDE);
	//��� ����ƽ
	hStatic_Pw = CreateWindow(TEXT("static"), TEXT("PW"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 150, 40, 20, hWnd, (HMENU)ID_S_PW, g_hInst, NULL);
	if (selwindow != 2)ShowWindow(hStatic_Pw, SW_HIDE);
	//��� ����Ʈ
	hEdit_Pw = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 200, 100, 50, hWnd, (HMENU)ID_E_PW, g_hInst, NULL);
	if (selwindow != 2)ShowWindow(hEdit_Pw, SW_HIDE);
	//�̸� ����ƽ
	hStatic_Name = CreateWindow(TEXT("static"), TEXT("NAME"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 300, 40, 20, hWnd, (HMENU)ID_S_NAME, g_hInst, NULL);
	if (selwindow != 2)ShowWindow(hStatic_Name, SW_HIDE);
	//�̸� ����Ʈ
	hEdit_Name = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 350, 100, 50, hWnd, (HMENU)ID_E_NAME, g_hInst, NULL);
	if (selwindow != 2)ShowWindow(hEdit_Name, SW_HIDE);
	//��ȭ��ȣ ����ƽ
	hStatic_Pn = CreateWindow(TEXT("static"), TEXT("PN"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 450, 40, 20, hWnd, (HMENU)ID_S_PN, g_hInst, NULL);
	if (selwindow != 2)ShowWindow(hStatic_Pn, SW_HIDE);
	//��ȭ��ȣ ����Ʈ
	hEdit_Pn = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 500, 100, 50, hWnd, (HMENU)ID_E_PN, g_hInst, NULL);
	if (selwindow != 2)ShowWindow(hEdit_Pn, SW_HIDE);
	//�ּ� ����ƽ
	hStatic_Addr = CreateWindow(TEXT("static"), TEXT("ADDR"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		510, 0, 40, 20, hWnd, (HMENU)ID_S_ADDR, g_hInst, NULL);
	if (selwindow != 2)ShowWindow(hStatic_Addr, SW_HIDE);
	//�ּ� ����Ʈ
	hEdit_Addr = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		510, 50, 100, 50, hWnd, (HMENU)ID_E_ADDR, g_hInst, NULL);
	if (selwindow != 2)ShowWindow(hEdit_Addr, SW_HIDE);
	//���� ����ƽ
	hStatic_Birth = CreateWindow(TEXT("static"), TEXT("BIRTH"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		510, 150, 50, 20, hWnd, (HMENU)ID_S_BIRTH, g_hInst, NULL);
	if (selwindow != 2)ShowWindow(hStatic_Birth, SW_HIDE);
	//���� ����Ʈ
	hEdit_Birth = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		510, 200, 100, 50, hWnd, (HMENU)ID_E_BIRTH, g_hInst, NULL);
	if (selwindow != 2)ShowWindow(hEdit_Birth, SW_HIDE);
	//���� ��ư
	hButton_Join = CreateWindow(TEXT("button"), TEXT("JOIN"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 10, 300, 200, hWnd, (HMENU)ID_B_JOIN, g_hInst, NULL);
	if (selwindow != 2)ShowWindow(hButton_Join, SW_HIDE);
	//���� ��ư
	hButton_Update = CreateWindow(TEXT("button"), TEXT("UPDATE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 300, 300, 200, hWnd, (HMENU)ID_B_UPDATE, g_hInst, NULL);
	if (selwindow != 2)ShowWindow(hButton_Update, SW_HIDE);
	///
}
//void UserToOrder():���ȭ�鿡�� �ֹ�ȭ������
void UserToOrder() {
	selwindow = 3;														//�ֹ�ȭ������ ����
	InvalidateRect(hWnd, NULL, TRUE);
	//�α���ȭ�� ����
	if (selwindow != 1)ShowWindow(hStatic_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hStatic_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Login, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Join_Or_Update, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Order, SW_HIDE);
	//���ȭ�����
	if (selwindow != 4)ShowWindow(hButton_Go_Order, SW_HIDE);
	if (selwindow != 4)ShowWindow(hButton_Logout, SW_HIDE);
	if (selwindow != 4)ShowWindow(hButton_Delete, SW_HIDE);
	if (selwindow != 4)ShowWindow(hEdit_Timer, SW_HIDE);
	if (selwindow != 4)ShowWindow(hStatic_Timer, SW_HIDE);
	//�ֹ�ȭ�� �ѱ�
	///�ֹ�ȭ�� ��Ʈ��
//������ ����ƽ
	hStatic_Item = CreateWindow(TEXT("static"), TEXT("ITEM"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 0, 40, 20, hWnd, (HMENU)ID_S_ITEM, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hStatic_Item, SW_HIDE);
	//�����۸���Ʈ ����Ʈ�ڽ�
	hListBox_ItemList = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_SORT | LBS_NOTIFY,
		10, 100, 400, 200, hWnd, (HMENU)ID_L_ITEM, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hListBox_ItemList, SW_HIDE);
	//�������ֹ����� ����ƽ
	hStatic_OrderCount = CreateWindow(TEXT("static"), TEXT("Count(2�ڸ�)"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		510, 0, 100, 20, hWnd, (HMENU)ID_S_ORDERCOUNT, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hStatic_OrderCount, SW_HIDE);
	//�������ֹ����� ����Ʈ
	hEdit_OrderCount = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		510, 50, 100, 50, hWnd, (HMENU)ID_E_ORDERCOUNT, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hEdit_OrderCount, SW_HIDE);
	//�ֹ� ��ư
	hButton_Order = CreateWindow(TEXT("button"), TEXT("ORDER"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 10, 200, 50, hWnd, (HMENU)ID_B_ORDER, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hButton_Order, SW_HIDE);
	///
	//�ֹ����� ��ư
	hButton_OrderCount0 = CreateWindow(TEXT("button"), TEXT("0"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT0, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hButton_OrderCount0, SW_HIDE);
	hButton_OrderCount1 = CreateWindow(TEXT("button"), TEXT("1"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT1, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hButton_OrderCount1, SW_HIDE);
	hButton_OrderCount2 = CreateWindow(TEXT("button"), TEXT("2"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		200, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT2, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hButton_OrderCount2, SW_HIDE);
	hButton_OrderCount3 = CreateWindow(TEXT("button"), TEXT("3"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		300, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT3, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hButton_OrderCount3, SW_HIDE);
	hButton_OrderCount4 = CreateWindow(TEXT("button"), TEXT("4"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		400, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT4, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hButton_OrderCount4, SW_HIDE);
	hButton_OrderCount5 = CreateWindow(TEXT("button"), TEXT("5"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		500, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT5, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hButton_OrderCount5, SW_HIDE);
	hButton_OrderCount6 = CreateWindow(TEXT("button"), TEXT("6"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		600, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT6, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hButton_OrderCount6, SW_HIDE);
	hButton_OrderCount7 = CreateWindow(TEXT("button"), TEXT("7"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		700, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT7, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hButton_OrderCount7, SW_HIDE);
	hButton_OrderCount8 = CreateWindow(TEXT("button"), TEXT("8"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		800, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT8, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hButton_OrderCount8, SW_HIDE);
	hButton_OrderCount9 = CreateWindow(TEXT("button"), TEXT("9"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		900, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT9, g_hInst, NULL);
	if (selwindow != 3)ShowWindow(hButton_OrderCount9, SW_HIDE);
	//

	//�����۸���Ʈ �ʱ�ȭ
	SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"DRINK");
	SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"FOOD");
	SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"PC");
}