#include "Common.h"
#include "Screen.h"
extern HWND hWnd;
extern HINSTANCE g_hInst;
//ȭ����(0:�ڸ�����/1:�α���/2:���� �� ����/3:�ֹ�/4:���) ��������
extern int selwindow;
//�ֹ�����(���� �ð� 0�϶�)
extern int selafterorder;																			

extern TCHAR tgId[10], tgName[10], tgPw[10], tgPn[10], tgAddr[10], tgBirth[10];																//���̵�,�̸�,���,��ȭ��ȣ,�ּ�,���� ���ڿ� ������
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
extern HWND hButton_Go_Join;

//���ȭ��
extern HWND hButton_User_Go_Update;	//�������� ���� �� �������� ��ư
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
extern HWND hButton_UserUpdate;					//�������� ������Ʈ�ϰ� �ٽ� ������ ���� ��ư
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
//����ȭ�鿡�� ���ȭ������
void UserUpdateToUser() {
	selwindow = 4;
	//�α���ȭ�� ����
	if (selwindow != 1)ShowWindow(hStatic_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hStatic_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Login, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Join, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Order, SW_HIDE);
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
	if (selwindow != 2)ShowWindow(hButton_UserUpdate, SW_HIDE);
	///
	///���ȭ�� ��Ʈ��
		//���� �� ���� ���� ��ư
	hButton_User_Go_Update = CreateWindow(TEXT("button"), TEXT("GOUPDATE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 400, 200, 65, hWnd, (HMENU)ID_B_USER_GO_UPDATE, g_hInst, NULL);
	//�ֹ����� ��ư
	hButton_Go_Order = CreateWindow(TEXT("button"), TEXT("ORDER"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 200, 200, 50, hWnd, (HMENU)ID_B_GO_ORDER, g_hInst, NULL);
	///
//�α׾ƿ� ��ư
	hButton_Logout = CreateWindow(TEXT("button"), TEXT("LOGOUT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		610, 50, 200, 65, hWnd, (HMENU)ID_B_LOGOUT, g_hInst, NULL);
	//Ż�� ��ư
	hButton_Delete = CreateWindow(TEXT("button"), TEXT("DELETE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 50, 200, 65, hWnd, (HMENU)ID_B_DELETE, g_hInst, NULL);
	//Ÿ�̸� ����Ʈ
	hEdit_Timer = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		610, 200, 100, 100, hWnd, (HMENU)ID_E_TIMER, g_hInst, NULL);
	// �б� ���� �Ӽ� ����
	SendMessage(hEdit_Timer, EM_SETREADONLY, TRUE, 0);
	//Ÿ�̸� ����ƽ
	hStatic_Timer = CreateWindow(TEXT("static"), TEXT("TIME"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		610, 150, 40, 20, hWnd, (HMENU)ID_S_TIMER, g_hInst, NULL);
	///
}
//���ȭ�鿡�� ����ȭ������
void UserUpdate() {
	selwindow = 2;
	///���ȭ�� ��Ʈ�� ����
	if (selwindow != 4)ShowWindow(hButton_User_Go_Update, SW_HIDE);
	if (selwindow != 4)ShowWindow(hButton_Go_Order, SW_HIDE);
	if (selwindow != 4)ShowWindow(hButton_Logout, SW_HIDE);
	if (selwindow != 4)ShowWindow(hButton_Delete, SW_HIDE);
	if (selwindow != 4)ShowWindow(hEdit_Timer, SW_HIDE);
	if (selwindow != 4)ShowWindow(hStatic_Timer, SW_HIDE);
	///
	//���� �� ����ȭ������ �ѱ�
	///���� �� ����ȭ�� ��Ʈ��
//���̵� ����ƽ
	hStatic_Id = CreateWindow(TEXT("static"), TEXT("ID"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 0, 40, 20, hWnd, (HMENU)ID_S_ID, g_hInst, NULL);
	//���̵� ����Ʈ
	hEdit_Id = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 50, 100, 50, hWnd, (HMENU)ID_E_ID, g_hInst, NULL);
	//��� ����ƽ
	hStatic_Pw = CreateWindow(TEXT("static"), TEXT("PW"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 150, 40, 20, hWnd, (HMENU)ID_S_PW, g_hInst, NULL);
	//��� ����Ʈ
	hEdit_Pw = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 200, 100, 50, hWnd, (HMENU)ID_E_PW, g_hInst, NULL);
	//�̸� ����ƽ
	hStatic_Name = CreateWindow(TEXT("static"), TEXT("NAME"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 300, 40, 20, hWnd, (HMENU)ID_S_NAME, g_hInst, NULL);
	//�̸� ����Ʈ
	hEdit_Name = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 350, 100, 50, hWnd, (HMENU)ID_E_NAME, g_hInst, NULL);
	//��ȭ��ȣ ����ƽ
	hStatic_Pn = CreateWindow(TEXT("static"), TEXT("PN"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 450, 40, 20, hWnd, (HMENU)ID_S_PN, g_hInst, NULL);
	//��ȭ��ȣ ����Ʈ
	hEdit_Pn = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 500, 100, 50, hWnd, (HMENU)ID_E_PN, g_hInst, NULL);
	//�ּ� ����ƽ
	hStatic_Addr = CreateWindow(TEXT("static"), TEXT("ADDR"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		510, 0, 40, 20, hWnd, (HMENU)ID_S_ADDR, g_hInst, NULL);
	//�ּ� ����Ʈ
	hEdit_Addr = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		510, 50, 100, 50, hWnd, (HMENU)ID_E_ADDR, g_hInst, NULL);
	//���� ����ƽ
	hStatic_Birth = CreateWindow(TEXT("static"), TEXT("BIRTH"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		510, 150, 50, 20, hWnd, (HMENU)ID_S_BIRTH, g_hInst, NULL);
	//���� ����Ʈ
	hEdit_Birth = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		510, 200, 100, 50, hWnd, (HMENU)ID_E_BIRTH, g_hInst, NULL);
	//���� ��ư
	hButton_UserUpdate = CreateWindow(TEXT("button"), TEXT("UPDATE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 300, 300, 200, hWnd, (HMENU)ID_B_USERUPDATE, g_hInst, NULL);
	///
	SetWindowText(hEdit_Id, tgId);
	SetWindowText(hEdit_Name, tgName);
	SetWindowText(hEdit_Pw, tgPw);
	SetWindowText(hEdit_Pn, tgPn);
	SetWindowText(hEdit_Addr, tgAddr);
	SetWindowText(hEdit_Birth, tgBirth);
}
//�ڸ�����ȭ��
void SelSeat(HWND hWnd, WPARAM wParam,LPARAM lParam) {
	selwindow = 0;
	///
		///�ڸ����� ��Ʈ��(Ȩȭ��)

		//�ڸ���ȣ ����ƽ
	hStatic_Sn = CreateWindow(TEXT("static"), TEXT("SN(2�ڸ�)"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 0, 80, 20, hWnd, (HMENU)ID_S_SN, g_hInst, NULL);
	//�ڸ���ȣ ����Ʈ
	hEdit_Sn = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 50, 100, 50, hWnd, (HMENU)ID_E_SN, g_hInst, NULL);
	// �б� ���� �Ӽ� ����
	SendMessage(hEdit_Sn, EM_SETREADONLY, TRUE, 0);
	//�ڸ����� ��ư
	hButton_SelSeat = CreateWindow(TEXT("button"), TEXT("SELECTSEAT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 10, 200, 50, hWnd, (HMENU)ID_B_SELSEAT, g_hInst, NULL);

	//�ڸ���ȣ ��ư
	hButton_SeatNum0 = CreateWindow(TEXT("button"), TEXT("0"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM0, g_hInst, NULL);
	hButton_SeatNum1 = CreateWindow(TEXT("button"), TEXT("1"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM1, g_hInst, NULL);
	hButton_SeatNum2 = CreateWindow(TEXT("button"), TEXT("2"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		200, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM2, g_hInst, NULL);
	hButton_SeatNum3 = CreateWindow(TEXT("button"), TEXT("3"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		300, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM3, g_hInst, NULL);
	hButton_SeatNum4 = CreateWindow(TEXT("button"), TEXT("4"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		400, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM4, g_hInst, NULL);
	hButton_SeatNum5 = CreateWindow(TEXT("button"), TEXT("5"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		500, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM5, g_hInst, NULL);
	hButton_SeatNum6 = CreateWindow(TEXT("button"), TEXT("6"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		600, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM6, g_hInst, NULL);
	hButton_SeatNum7 = CreateWindow(TEXT("button"), TEXT("7"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		700, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM7, g_hInst, NULL);
	hButton_SeatNum8 = CreateWindow(TEXT("button"), TEXT("8"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		800, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM8, g_hInst, NULL);
	hButton_SeatNum9 = CreateWindow(TEXT("button"), TEXT("9"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		900, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM9, g_hInst, NULL);
}
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

	//�α���ȭ�� �ѱ�
	/////�α���ȭ�� ��Ʈ��
//�α��� ���̵� ����ƽ
	hStatic_Login_Id = CreateWindow(TEXT("static"), TEXT("ID"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 0, 80, 20, hWnd, (HMENU)ID_S_LOGIN_ID, g_hInst, NULL);
	//�α��� ���̵� ����Ʈ
	hEdit_Login_Id = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 50, 100, 50, hWnd, (HMENU)ID_E_LOGIN_ID, g_hInst, NULL);
	//�α��� ��� ����ƽ
	hStatic_Login_Pw = CreateWindow(TEXT("static"), TEXT("PW"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 150, 40, 20, hWnd, (HMENU)ID_S_LOGIN_PW, g_hInst, NULL);
	//�α��� ��� ����Ʈ
	hEdit_Login_Pw = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 200, 100, 50, hWnd, (HMENU)ID_E_LOGIN_PW, g_hInst, NULL);
	//�α��� ��ư
	hButton_Login = CreateWindow(TEXT("button"), TEXT("LOGIN"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 10, 200, 50, hWnd, (HMENU)ID_B_LOGIN, g_hInst, NULL);

	//���԰��� ��ư
	hButton_Go_Join = CreateWindow(TEXT("button"), TEXT("GOJOIN"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 100, 200, 50, hWnd, (HMENU)ID_B_GO_JOIN, g_hInst, NULL);
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
	if (selwindow != 1)ShowWindow(hButton_Go_Join, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Order, SW_HIDE);
	//���ȭ�� �ѱ�
	///���ȭ�� ��Ʈ��
	//���� ���� ��ư
	hButton_User_Go_Update = CreateWindow(TEXT("button"), TEXT("GOUPDATE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 400, 200, 65, hWnd, (HMENU)ID_B_USER_GO_UPDATE, g_hInst, NULL);
	//�ֹ����� ��ư
	hButton_Go_Order = CreateWindow(TEXT("button"), TEXT("ORDER"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 200, 200, 50, hWnd, (HMENU)ID_B_GO_ORDER, g_hInst, NULL);
	///
//�α׾ƿ� ��ư
	hButton_Logout = CreateWindow(TEXT("button"), TEXT("LOGOUT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		610, 50, 200, 65, hWnd, (HMENU)ID_B_LOGOUT, g_hInst, NULL);
	//Ż�� ��ư
	hButton_Delete = CreateWindow(TEXT("button"), TEXT("DELETE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 50, 200, 65, hWnd, (HMENU)ID_B_DELETE, g_hInst, NULL);
	//Ÿ�̸� ����Ʈ
	hEdit_Timer = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		610, 200, 100, 100, hWnd, (HMENU)ID_E_TIMER, g_hInst, NULL);
	// �б� ���� �Ӽ� ����
	SendMessage(hEdit_Timer, EM_SETREADONLY, TRUE, 0);
	//Ÿ�̸� ����ƽ
	hStatic_Timer = CreateWindow(TEXT("static"), TEXT("TIME"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		610, 150, 40, 20, hWnd, (HMENU)ID_S_TIMER, g_hInst, NULL);
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
		//�α��� ���̵� ����Ʈ
		hEdit_Login_Id = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 50, 100, 50, hWnd, (HMENU)ID_E_LOGIN_ID, g_hInst, NULL);
		//�α��� ��� ����ƽ
		hStatic_Login_Pw = CreateWindow(TEXT("static"), TEXT("PW"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 150, 40, 20, hWnd, (HMENU)ID_S_LOGIN_PW, g_hInst, NULL);
		//�α��� ��� ����Ʈ
		hEdit_Login_Pw = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 200, 100, 50, hWnd, (HMENU)ID_E_LOGIN_PW, g_hInst, NULL);
		//�α��� ��ư
		hButton_Login = CreateWindow(TEXT("button"), TEXT("LOGIN"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 10, 200, 50, hWnd, (HMENU)ID_B_LOGIN, g_hInst, NULL);

		//���԰��� ��ư
		hButton_Go_Join = CreateWindow(TEXT("button"), TEXT("GOJOIN"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 100, 200, 50, hWnd, (HMENU)ID_B_GO_JOIN, g_hInst, NULL);
	}
	//���� �ð������� ���ȭ������
	else {
		///���ȭ�� ��Ʈ��
		//���� �� ���� ���� ��ư
		hButton_User_Go_Update = CreateWindow(TEXT("button"), TEXT("GOUPDATE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 400, 200, 65, hWnd, (HMENU)ID_B_USER_GO_UPDATE, g_hInst, NULL);
		//�ֹ����� ��ư
		hButton_Go_Order = CreateWindow(TEXT("button"), TEXT("ORDER"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 200, 200, 50, hWnd, (HMENU)ID_B_GO_ORDER, g_hInst, NULL);
		///
//�α׾ƿ� ��ư
		hButton_Logout = CreateWindow(TEXT("button"), TEXT("LOGOUT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			610, 50, 200, 65, hWnd, (HMENU)ID_B_LOGOUT, g_hInst, NULL);
		//Ż�� ��ư
		hButton_Delete = CreateWindow(TEXT("button"), TEXT("DELETE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 50, 200, 65, hWnd, (HMENU)ID_B_DELETE, g_hInst, NULL);
		//Ÿ�̸� ����Ʈ
		hEdit_Timer = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			610, 200, 100, 100, hWnd, (HMENU)ID_E_TIMER, g_hInst, NULL);
		// �б� ���� �Ӽ� ����
		SendMessage(hEdit_Timer, EM_SETREADONLY, TRUE, 0);
		//Ÿ�̸� ����ƽ
		hStatic_Timer = CreateWindow(TEXT("static"), TEXT("TIME"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			610, 150, 40, 20, hWnd, (HMENU)ID_S_TIMER, g_hInst, NULL);
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
	if (selwindow != 1)ShowWindow(hButton_Go_Join, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Order, SW_HIDE);
	//�ֹ�ȭ�� �ѱ�
	///�ֹ�ȭ�� ��Ʈ��
//������ ����ƽ
	hStatic_Item = CreateWindow(TEXT("static"), TEXT("ITEM"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 0, 40, 20, hWnd, (HMENU)ID_S_ITEM, g_hInst, NULL);
	//�����۸���Ʈ ����Ʈ�ڽ�
	hListBox_ItemList = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_SORT | LBS_NOTIFY,
		10, 100, 400, 200, hWnd, (HMENU)ID_L_ITEM, g_hInst, NULL);
	//�������ֹ����� ����ƽ
	hStatic_OrderCount = CreateWindow(TEXT("static"), TEXT("Count(2�ڸ�)"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		510, 0, 100, 20, hWnd, (HMENU)ID_S_ORDERCOUNT, g_hInst, NULL);
	//�������ֹ����� ����Ʈ
	hEdit_OrderCount = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		510, 50, 100, 50, hWnd, (HMENU)ID_E_ORDERCOUNT, g_hInst, NULL);
	// �б� ���� �Ӽ� ����
	SendMessage(hEdit_OrderCount, EM_SETREADONLY, TRUE, 0);
	//�ֹ� ��ư
	hButton_Order = CreateWindow(TEXT("button"), TEXT("ORDER"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 10, 200, 50, hWnd, (HMENU)ID_B_ORDER, g_hInst, NULL);
	///
	//�ֹ����� ��ư
	hButton_OrderCount0 = CreateWindow(TEXT("button"), TEXT("0"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT0, g_hInst, NULL);
	hButton_OrderCount1 = CreateWindow(TEXT("button"), TEXT("1"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT1, g_hInst, NULL);
	hButton_OrderCount2 = CreateWindow(TEXT("button"), TEXT("2"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		200, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT2, g_hInst, NULL);
	hButton_OrderCount3 = CreateWindow(TEXT("button"), TEXT("3"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		300, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT3, g_hInst, NULL);
	hButton_OrderCount4 = CreateWindow(TEXT("button"), TEXT("4"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		400, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT4, g_hInst, NULL);
	hButton_OrderCount5 = CreateWindow(TEXT("button"), TEXT("5"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		500, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT5, g_hInst, NULL);
	hButton_OrderCount6 = CreateWindow(TEXT("button"), TEXT("6"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		600, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT6, g_hInst, NULL);
	hButton_OrderCount7 = CreateWindow(TEXT("button"), TEXT("7"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		700, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT7, g_hInst, NULL);
	hButton_OrderCount8 = CreateWindow(TEXT("button"), TEXT("8"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		800, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT8, g_hInst, NULL);
	hButton_OrderCount9 = CreateWindow(TEXT("button"), TEXT("9"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		900, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT9, g_hInst, NULL);
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
	//�α��� ���̵� ����Ʈ
	hEdit_Login_Id = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 50, 100, 50, hWnd, (HMENU)ID_E_LOGIN_ID, g_hInst, NULL);
	//�α��� ��� ����ƽ
	hStatic_Login_Pw = CreateWindow(TEXT("static"), TEXT("PW"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 150, 40, 20, hWnd, (HMENU)ID_S_LOGIN_PW, g_hInst, NULL);
	//�α��� ��� ����Ʈ
	hEdit_Login_Pw = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 200, 100, 50, hWnd, (HMENU)ID_E_LOGIN_PW, g_hInst, NULL);
	//�α��� ��ư
	hButton_Login = CreateWindow(TEXT("button"), TEXT("LOGIN"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 10, 200, 50, hWnd, (HMENU)ID_B_LOGIN, g_hInst, NULL);

	//���԰��� ��ư
	hButton_Go_Join = CreateWindow(TEXT("button"), TEXT("GOJOIN"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 100, 200, 50, hWnd, (HMENU)ID_B_GO_JOIN, g_hInst, NULL);
}
//void ToSelSeat():�ٽ� �ڸ�����ȭ������
void ToSelSeat() {
	//�ڸ�����ȭ�� ����
	DestroyWindow(hStatic_Sn);
	DestroyWindow(hEdit_Sn);
	DestroyWindow(hButton_SelSeat);
	DestroyWindow(hButton_SeatNum0);
	DestroyWindow(hButton_SeatNum1);
	DestroyWindow(hButton_SeatNum2);
	DestroyWindow(hButton_SeatNum3);
	DestroyWindow(hButton_SeatNum4);
	DestroyWindow(hButton_SeatNum5);
	DestroyWindow(hButton_SeatNum6);
	DestroyWindow(hButton_SeatNum7);
	DestroyWindow(hButton_SeatNum8);
	DestroyWindow(hButton_SeatNum9);
	//����ȭ�� ����
	DestroyWindow(hStatic_Id);
	DestroyWindow(hEdit_Id);
	DestroyWindow(hStatic_Pw);
	DestroyWindow(hEdit_Pw);
	DestroyWindow(hStatic_Name);
	DestroyWindow(hEdit_Name);
	DestroyWindow(hStatic_Pn);
	DestroyWindow(hEdit_Pn);
	DestroyWindow(hStatic_Addr);
	DestroyWindow(hEdit_Addr);
	DestroyWindow(hStatic_Birth);
	DestroyWindow(hEdit_Birth);
	//����ȭ�� ����
	DestroyWindow(hStatic_Id);
	DestroyWindow(hEdit_Id);
	DestroyWindow(hStatic_Pw);
	DestroyWindow(hEdit_Pw);
	DestroyWindow(hStatic_Name);
	DestroyWindow(hEdit_Name);
	DestroyWindow(hStatic_Pn);
	DestroyWindow(hEdit_Pn);
	DestroyWindow(hStatic_Addr);
	DestroyWindow(hEdit_Addr);
	DestroyWindow(hStatic_Birth);
	DestroyWindow(hEdit_Birth);
	DestroyWindow(hButton_Join);
	DestroyWindow(hButton_Update);
	//���ȭ�� ����
	DestroyWindow(hButton_User_Go_Update);
	DestroyWindow(hButton_Go_Order);
	DestroyWindow(hButton_Logout);
	DestroyWindow(hButton_Delete);
	DestroyWindow(hEdit_Timer);
	DestroyWindow(hStatic_Timer);
	//�ֹ�ȭ�� ����
	DestroyWindow(hStatic_Item);
	DestroyWindow(hListBox_ItemList);
	DestroyWindow(hStatic_OrderCount);
	DestroyWindow(hEdit_OrderCount);
	DestroyWindow(hButton_Order);
	DestroyWindow(hButton_OrderCount0);
	DestroyWindow(hButton_OrderCount1);
	DestroyWindow(hButton_OrderCount2);
	DestroyWindow(hButton_OrderCount3);
	DestroyWindow(hButton_OrderCount4);
	DestroyWindow(hButton_OrderCount5);
	DestroyWindow(hButton_OrderCount6);
	DestroyWindow(hButton_OrderCount7);
	DestroyWindow(hButton_OrderCount8);
	DestroyWindow(hButton_OrderCount9);
	//�α���ȭ�� ����
	DestroyWindow(hStatic_Login_Id);
	DestroyWindow(hEdit_Login_Id);
	DestroyWindow(hStatic_Login_Pw);
	DestroyWindow(hEdit_Login_Pw);
	DestroyWindow(hButton_Login);
	DestroyWindow(hButton_Go_Join);
	DestroyWindow(hButton_Go_Order);

	selwindow = 0;																						//�ڸ�����ȭ�� ����
	InvalidateRect(hWnd, NULL, TRUE);
	///���� �� ����ȭ�� ����
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

	//���ȭ�� ����
	if (selwindow != 4)ShowWindow(hButton_User_Go_Update, SW_HIDE);
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
	if (selwindow != 1)ShowWindow(hButton_Go_Join, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Order, SW_HIDE);
	//�ڸ�����ȭ�� �ѱ�
	///�ڸ�����ȭ�� ��Ʈ��

//�ڸ���ȣ ����ƽ
	hStatic_Sn = CreateWindow(TEXT("static"), TEXT("SN(2�ڸ�)"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 0, 80, 20, hWnd, (HMENU)ID_S_SN, g_hInst, NULL);
	//�ڸ���ȣ ����Ʈ
	hEdit_Sn = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 50, 100, 50, hWnd, (HMENU)ID_E_SN, g_hInst, NULL);
	// �б� ���� �Ӽ� ����
	SendMessage(hEdit_Sn, EM_SETREADONLY, TRUE, 0);
	//�ڸ����� ��ư
	hButton_SelSeat = CreateWindow(TEXT("button"), TEXT("SELECTSEAT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 10, 200, 50, hWnd, (HMENU)ID_B_SELSEAT, g_hInst, NULL);

	//�ڸ���ȣ ��ư
	hButton_SeatNum0 = CreateWindow(TEXT("button"), TEXT("0"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM0, g_hInst, NULL);
	hButton_SeatNum1 = CreateWindow(TEXT("button"), TEXT("1"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM1, g_hInst, NULL);
	hButton_SeatNum2 = CreateWindow(TEXT("button"), TEXT("2"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		200, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM2, g_hInst, NULL);
	hButton_SeatNum3 = CreateWindow(TEXT("button"), TEXT("3"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		300, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM3, g_hInst, NULL);
	hButton_SeatNum4 = CreateWindow(TEXT("button"), TEXT("4"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		400, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM4, g_hInst, NULL);
	hButton_SeatNum5 = CreateWindow(TEXT("button"), TEXT("5"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		500, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM5, g_hInst, NULL);
	hButton_SeatNum6 = CreateWindow(TEXT("button"), TEXT("6"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		600, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM6, g_hInst, NULL);
	hButton_SeatNum7 = CreateWindow(TEXT("button"), TEXT("7"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		700, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM7, g_hInst, NULL);
	hButton_SeatNum8 = CreateWindow(TEXT("button"), TEXT("8"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		800, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM8, g_hInst, NULL);
	hButton_SeatNum9 = CreateWindow(TEXT("button"), TEXT("9"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		900, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM9, g_hInst, NULL);
}
//void LoginToJoin():�α��ο��� ���� ����
void LoginToJoin() {
	selwindow = 2;																																	//���� �� ����ȭ������ ����
	InvalidateRect(hWnd, NULL, TRUE);
	//�α���ȭ�� ����
	if (selwindow != 1)ShowWindow(hStatic_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hStatic_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Login, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Join, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Order, SW_HIDE);
	//���� �� ����ȭ������ �ѱ�
	///���� �� ����ȭ�� ��Ʈ��
//���̵� ����ƽ
	hStatic_Id = CreateWindow(TEXT("static"), TEXT("ID"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 0, 40, 20, hWnd, (HMENU)ID_S_ID, g_hInst, NULL);
	//���̵� ����Ʈ
	hEdit_Id = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 50, 100, 50, hWnd, (HMENU)ID_E_ID, g_hInst, NULL);
	//��� ����ƽ
	hStatic_Pw = CreateWindow(TEXT("static"), TEXT("PW"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 150, 40, 20, hWnd, (HMENU)ID_S_PW, g_hInst, NULL);
	//��� ����Ʈ
	hEdit_Pw = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 200, 100, 50, hWnd, (HMENU)ID_E_PW, g_hInst, NULL);
	//�̸� ����ƽ
	hStatic_Name = CreateWindow(TEXT("static"), TEXT("NAME"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 300, 40, 20, hWnd, (HMENU)ID_S_NAME, g_hInst, NULL);
	//�̸� ����Ʈ
	hEdit_Name = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 350, 100, 50, hWnd, (HMENU)ID_E_NAME, g_hInst, NULL);
	//��ȭ��ȣ ����ƽ
	hStatic_Pn = CreateWindow(TEXT("static"), TEXT("PN"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 450, 40, 20, hWnd, (HMENU)ID_S_PN, g_hInst, NULL);
	//��ȭ��ȣ ����Ʈ
	hEdit_Pn = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 500, 100, 50, hWnd, (HMENU)ID_E_PN, g_hInst, NULL);
	//�ּ� ����ƽ
	hStatic_Addr = CreateWindow(TEXT("static"), TEXT("ADDR"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		510, 0, 40, 20, hWnd, (HMENU)ID_S_ADDR, g_hInst, NULL);
	//�ּ� ����Ʈ
	hEdit_Addr = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		510, 50, 100, 50, hWnd, (HMENU)ID_E_ADDR, g_hInst, NULL);
	//���� ����ƽ
	hStatic_Birth = CreateWindow(TEXT("static"), TEXT("BIRTH"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		510, 150, 50, 20, hWnd, (HMENU)ID_S_BIRTH, g_hInst, NULL);
	//���� ����Ʈ
	hEdit_Birth = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		510, 200, 100, 50, hWnd, (HMENU)ID_E_BIRTH, g_hInst, NULL);
	//���� ��ư
	hButton_Join = CreateWindow(TEXT("button"), TEXT("JOIN"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 10, 300, 200, hWnd, (HMENU)ID_B_JOIN, g_hInst, NULL);
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
	if (selwindow != 1)ShowWindow(hButton_Go_Join, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Order, SW_HIDE);
	//���ȭ�����
	if (selwindow != 4)ShowWindow(hButton_User_Go_Update, SW_HIDE);
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
	//�����۸���Ʈ ����Ʈ�ڽ�
	hListBox_ItemList = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_SORT | LBS_NOTIFY,
		10, 100, 400, 200, hWnd, (HMENU)ID_L_ITEM, g_hInst, NULL);
	//�������ֹ����� ����ƽ
	hStatic_OrderCount = CreateWindow(TEXT("static"), TEXT("Count(2�ڸ�)"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		510, 0, 100, 20, hWnd, (HMENU)ID_S_ORDERCOUNT, g_hInst, NULL);
	//�������ֹ����� ����Ʈ
	hEdit_OrderCount = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		510, 50, 100, 50, hWnd, (HMENU)ID_E_ORDERCOUNT, g_hInst, NULL);
	// �б� ���� �Ӽ� ����
	SendMessage(hEdit_OrderCount, EM_SETREADONLY, TRUE, 0);
	//�ֹ� ��ư
	hButton_Order = CreateWindow(TEXT("button"), TEXT("ORDER"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 10, 200, 50, hWnd, (HMENU)ID_B_ORDER, g_hInst, NULL);
	///
	//�ֹ����� ��ư
	hButton_OrderCount0 = CreateWindow(TEXT("button"), TEXT("0"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT0, g_hInst, NULL);
	hButton_OrderCount1 = CreateWindow(TEXT("button"), TEXT("1"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT1, g_hInst, NULL);
	hButton_OrderCount2 = CreateWindow(TEXT("button"), TEXT("2"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		200, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT2, g_hInst, NULL);
	hButton_OrderCount3 = CreateWindow(TEXT("button"), TEXT("3"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		300, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT3, g_hInst, NULL);
	hButton_OrderCount4 = CreateWindow(TEXT("button"), TEXT("4"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		400, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT4, g_hInst, NULL);
	hButton_OrderCount5 = CreateWindow(TEXT("button"), TEXT("5"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		500, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT5, g_hInst, NULL);
	hButton_OrderCount6 = CreateWindow(TEXT("button"), TEXT("6"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		600, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT6, g_hInst, NULL);
	hButton_OrderCount7 = CreateWindow(TEXT("button"), TEXT("7"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		700, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT7, g_hInst, NULL);
	hButton_OrderCount8 = CreateWindow(TEXT("button"), TEXT("8"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		800, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT8, g_hInst, NULL);
	hButton_OrderCount9 = CreateWindow(TEXT("button"), TEXT("9"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		900, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT9, g_hInst, NULL);
	//

	//�����۸���Ʈ �ʱ�ȭ
	SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"DRINK");
	SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"FOOD");
	SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"PC");
}