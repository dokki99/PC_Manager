//12/13 �ڸ��ݳ�Ÿ�̸� �缳��, �ǽû�ǰ �ֹ��� ���� �ð��� ��ġ�� �α׾ƿ� �� �ܿ��ð� ������ 
//12/4 ���Ϻκ� �ٲ����� Ŭ���̾�Ʈ�� �޽��� ���� �ٶ�
//�޼���ũ��Ŀ���� �Ϸ�
#include<Windowsx.h>
#include <Windows.h>
#include <WinUser.h>

#include <stdio.h>
#include <CommCtrl.h>
#include <sql.h>
#include <sqlext.h>
#include "resource.h"

#include "Socket.h"
#include "Screen.h"
#include "Timer.h"
#include "Request.h"
#include "Common.h"
WPARAM gwParam;
LPARAM glParam;
HWND hWnd;
//ȭ�� �ڵ�
HWND hWndMain;		// ����
//ȭ�� ����
LPCTSTR C_Class = TEXT("����");					//����
LPCTSTR JU_Class = TEXT("ȸ������/����");//����/����
LPCTSTR L_Class = TEXT("�α���");				//�α���
LPCTSTR S_Class = TEXT("�ڸ�����");			//�ڸ�����
LPCTSTR O_Class = TEXT("�ֹ�");					//�ֹ�

//���ú꽺���� ����
HDC hdc;
SOCKET* P;
TCHAR CODE[3] = "", TEXT[256] = "";
int j;
//

////��Ʈ�� �ڵ�
/// ���ڹ�ư(�ڸ���ȣ,�ֹ�����)
HWND hButton_SeatNum0;
HWND hButton_SeatNum1;
HWND hButton_SeatNum2;
HWND hButton_SeatNum3;
HWND hButton_SeatNum4;
HWND hButton_SeatNum5;
HWND hButton_SeatNum6;
HWND hButton_SeatNum7;
HWND hButton_SeatNum8;
HWND hButton_SeatNum9;

HWND hButton_OrderCount0;
HWND hButton_OrderCount1;
HWND hButton_OrderCount2;
HWND hButton_OrderCount3;
HWND hButton_OrderCount4;
HWND hButton_OrderCount5;
HWND hButton_OrderCount6;
HWND hButton_OrderCount7;
HWND hButton_OrderCount8;
HWND hButton_OrderCount9;
//���� ��ư ����(�ڸ���ȣ,�ֹ�����)
int snbtncount = 0;
int ocbtncount = 0;
//���ȭ��
HWND hButton_Logout;							//�α׾ƿ� ��ư
HWND hButton_Delete;							//Ż�� ��ư
HWND hEdit_Timer;								//Ż�� ����Ʈ

HWND hButton_Go_Order;						//�ֹ����� ��ư

//����/����
HWND hEdit_Id;										//���̵� ����Ʈ
HWND hEdit_Pw;										//��� ����Ʈ
HWND hEdit_Name;								//�̸� ����Ʈ
HWND hEdit_Pn;										//��ȭ��ȣ ����Ʈ
HWND hEdit_Addr;									//�ּ� ����Ʈ
HWND hEdit_Birth;									//���� ����Ʈ
HWND hButton_Join;								//���� ��ư
HWND hButton_Update;							//���� ��ư
//�α���
HWND hEdit_Login_Id;							//���̵� ����Ʈ
HWND hEdit_Login_Pw;							//��� ����Ʈ
HWND hButton_Login;								//�α��� ��ư

HWND hButton_UserUpdate;					//�������� ������Ʈ�ϰ� �ٽ� ������ ���� ��ư
HWND hButton_User_Go_Update;	//�������� �������� ��ư
HWND hButton_Go_Join;	//���԰��� ��ư
//�ڸ�����
HWND hEdit_Sn;										//�ڸ���ȣ ����Ʈ
HWND hButton_SelSeat;						//�ڸ���ȣ���� ��ư
//�ֹ�
HWND hListBox_ItemList;						//�����۸���Ʈ ����Ʈ�ڽ�
HWND hEdit_OrderCount;						//�������ֹ����� ����Ʈ
HWND hButton_Order;							//�ֹ� ��ư
///����ƽ
//����/����
HWND hStatic_Id;									//���̵�
HWND hStatic_Pw;									//���
HWND hStatic_Name;								//�̸�
HWND hStatic_Addr;								//�ּ�
HWND hStatic_Birth;								//����
HWND hStatic_Pn;									//��ȭ��ȣ
//�α���
HWND hStatic_Login_Id;						//���̵�
HWND hStatic_Login_Pw;						//���
//�ڸ�����
HWND hStatic_Sn;									//�ڸ���ȣ
//�ֹ�
HWND hStatic_Item;								//������
HWND hStatic_OrderCount;					//�������ֹ�����
//���ȭ��
HWND hStatic_Timer;							//Ÿ�̸�
//���ν����Լ���
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);															//�⺻

HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("PC�� Ŭ���̾�Ʈ");																										//������ ���ν��� Ÿ��Ʋ
////IP�� ��Ʈ��ȣ ����
const char g_szlpAddress[17] = "127.0.0.1";
const unsigned short g_uPort = 7878;
TCHAR tgIp[17] = "127.0.0.1", tgPort[10] = "7878";																									//IP�� PORT��ȣ
TCHAR tgId[10], tgName[10], tgPw[10], tgPn[10], tgAddr[10], tgBirth[10];																//���̵�,�̸�,���,��ȭ��ȣ,�ּ�,���� ���ڿ� ������
int nip, nport;																																							//IP�� PORT��ȣ ���ڷ� ��ȯ

DWORD ThreadID;																																					//��������̵�
HANDLE hThread;																																					//������ �ڵ�

TCHAR tgcmdserver[200];																																		//������ ����ϴ� Ŀ��� ���ڿ�
///���� ���� ����
int nReturn;																																								//��� ��� 
WSADATA wsadata;																																				//���� ����
SOCKET clientsock;																																					//���Ϲ�ȣ ���
sockaddr_in addr_client;
int addrlen_clt = sizeof(sockaddr);
///�����κ��� �޴� �޼���
char buf[1024];																																						//�޽��� ���ڿ�																						
char strTemp[1024];																																				//�޼����� ä��â�� ������ �޼����� ��ȯ
///

int selitemtrue;																																						//������ ���� üũ
int infotrue;																																								//�� ���� ��� ����
////�ֿ亯��
//Ŀ��忡 ���� ������ ������
int turn = 0;																																								//������ ������ ������� �������� �ϴ� ���� ����
int ini;																																										//�� �������� ����(�޼���)�� �ε���
TCHAR info[200];																																					//��Ҹ� �̴� ���ڿ�
int infoi = 0;																																								//��Ҹ� �̴� ���ڿ��� �ε���
//
//�α��λ�Ȳ��  ������ ������
TCHAR userid[10];																																					//���̵�
TCHAR usersn[10];																																					//�ڸ���ȣ
TCHAR userpw[10];																																					//���
int pctimei;																																								//�ð� �ε���
int pctime;																																								//�α��ε� ������ �ð�
//
///�ֹ���Ȳ�� ������
//�ֹ��ð�
int orderpctime;																																						//�ֹ��� �ð����� ������ ����
//�������ֹ�����
TCHAR tgselitemi[10];																																				//�ε����� ���ڿ��� ��ȯ
int selitemi;																																								//����Ʈ �ڽ��� �ε���
TCHAR tgitemcount[10] = "";																																	//���� ���ڿ�
//
///
//���� üũ ����
BOOL chkreturnsn = 0;																																					//�ڸ��ݳ�����
BOOL chkseat = 0;																																							//�ڸ�����
BOOL chkjoin = 0;																																							//����
BOOL chklogin = 0;																																						//�α���
BOOL chktimer;																																							//Ÿ�̸� ����(1�̸� ����/0�̸� ����)	
BOOL chkendbtn = 0;																																					//�α׾ƿ�/Ż�� 

BOOL chkorder = 0;																																						//�ֹ�
BOOL chkupdate = 0;																																					//����
BOOL chkpctime = 0;																																					//�����ð�0����
//
int i;
//�ڸ���ȣ ���� ������
TCHAR tgnum[10];																																					//��û�ϴ� �ڸ���ȣ ���ڿ�

//Ŀ�ǵ尡 �ֹ����� �Ǻ�
TCHAR orderbuf[10];																																				//�ֹ��������(�̸� �޼����� �ֹ����� �Ǻ�)
int orderi = 0;																																							//�ֹ�������� �ε���
//

//�����ð�
TCHAR pctimebuf[10];																																				//Ŀ�ǵ忡�� ��ƿ���
TCHAR tpctime[10];																																					//����ϱ� ���� ���ڿ�
//

//�¼���ȯ����
int returnsntime = 100000000;																																//�ڸ��ݳ�Ÿ�̸ӽð�
BOOL returnsn = 0;																																						//�ڸ��ݳ�����

//�ڸ��������� ������ ����
BOOL gohome = 0;

//�����ð�������  1:�α���ȭ��/0:���ȭ������ ������
int selafterorder = 0;

//ȭ����(0:�ڸ�����/1:�α���/2:���� �� ����/3:�ֹ�/4:���) ��������
int selwindow = 0;


//int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int):�޴� ����, ��� �ٸ� ���ν��� ����
int APIENTRY WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, LPSTR lpszCmdParam, int nCmdShow)
{
	//HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hinstance;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hinstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;																			
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hinstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

BOOL MsgCrk_OnCreate(HWND hWnd,LPCREATESTRUCT lpCreateStruct);
void MsgCrk_OnTimer(HWND hWnd,UINT id);
void MsgCrk_OnCommand(HWND hWnd,int,HWND,UINT id);
void MsgCrk_OnDestroy(HWND hWnd);
//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM):�⺻ȭ��(�ٸ� ȭ�� ����,�α��ε����� Ÿ�̸� ����)
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	gwParam = wParam;
	glParam = lParam;
	switch (iMessage) {
		HANDLE_MSG(hWnd, WM_CREATE, MsgCrk_OnCreate);
		HANDLE_MSG(hWnd, WM_TIMER, MsgCrk_OnTimer);
		HANDLE_MSG(hWnd, WM_COMMAND, MsgCrk_OnCommand);
		HANDLE_MSG(hWnd, WM_DESTROY, MsgCrk_OnDestroy);
	default:
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
	return 0;
}
BOOL MsgCrk_OnCreate(HWND hWnd,LPCREATESTRUCT lpCreateStruct) {
	hWndMain = hWnd;
	InitCommonControls();

	CLIENT_Open();

	SelSeat(hWnd,gwParam,glParam);



	//Ÿ�̸�(�α��� �� ����)
	SetTimer(hWnd, 0, 100, NULL);			//�ڵ����� ȭ���� ���� Ÿ�̸�
	SetTimer(hWnd, 1, 1000, NULL);			//�ð� Ÿ�̸�
	SetTimer(hWnd, 2, 1000, NULL);			//�ð� Ÿ�̸� �����ϴ� Ÿ�̸�
	SetTimer(hWnd, 3, 100000, NULL);	//�ڸ� �ݳ� Ÿ�̸�
	SetTimer(hWnd, 4, 1000, NULL);			//�ڸ� �ݳ� Ÿ�̸� �缳��
	SendMessage(hWnd, WM_TIMER, 0, 0);

	return TRUE;
}
void MsgCrk_OnTimer(HWND hWnd,UINT id) {
	switch (gwParam) {
	case 0:													//�ڵ����� ȭ���� �ٲٴ� Ÿ�̸�
		if (chkupdate == 1) {										//���� �� �Ǿ�����
			chkupdate = 0;
			UserUpdateToUser();
		}
		//�ڸ�����ȭ��->�α���ȭ��
		else if (chkseat == 1) {								//�ڸ����� �� �Ǿ�����
			chkseat = 0;

			SelSeatToLogin();
		}
		//�α���ȭ��->���ȭ��
		else if (chklogin == 1) {					//�α��� �� �Ǿ��� ��
			chklogin = 0;

			LoginToUser();
		}
		//�����ð� 0�϶� �ֹ� �� �α��� ȭ������/�����ð� ������ �ֹ� �� ���ȭ������
		else if (chkorder == 1) {										//�ֹ� �� �Ǿ��� ��
			chkorder = 0;

			OrderToLoginOrUser();
		}
		//�����ð� 0�϶� �α���ȭ�鿡�� �ֹ�ȭ������
		else if (chkpctime == 1) {															//�����ð� 0�϶�
			chkpctime = 0;
			selafterorder = 1;																	//�����ð�0�϶��� ����

			LoginToOrder();
		}
		//���� �� ����ȭ�鿡�� �α���ȭ������
		else if (chkjoin == 1) {										//ȸ������ �� �Ǿ�����
			chkjoin = 0;

			JoinOrUpdateToLogin();
		}
		//���� �� ����ȭ�鿡�� �α���ȭ������ 
		else if (chkupdate == 1) {										//���� �� �Ǿ�����
			chkupdate = 0;

			JoinOrUpdateToLogin();
		}
		//���ȭ��(�α׾ƿ�/Ż��)/�ֹ�ȭ��/�α���ȭ��(�ڸ��ݳ�)���� �ڸ�����ȭ������ 
		else if (gohome == 1) {																				//�ڸ�����ȭ������ ���� ��Ȳ
			gohome = 0;

			ToSelSeat();
		}
		break;
	case 1:
		if (chktimer == 1) {
			UserTimer(hWnd, gwParam, glParam);
			//chktimer = 0;																															//Ÿ�̸� ���� ���·�
		}
		break;
		//�������ư ��������/�����ð� 0�϶� �����ð� Ÿ�̸� �����ϰ� �ٽ� Ÿ�̸� �����(�α����� ��� �ޱ� ����)
	case 2:
		if (chkendbtn == 1) {

			SetUserTimer();
			chkendbtn = 0;
		}
		break;
	case 3:																																					//�ڸ��ݳ�Ÿ�̸�
		if (returnsn == 1) {																															//�ڸ��ݳ������϶�
			ReturnSeatTimer();
			ToSelSeat();
			chkreturnsn = 1;
		}
		break;
	case 4:
		if (chkreturnsn == 1) {
			SetReturnSeatTimer();																												//�ڸ��ݳ�Ÿ�̸� �缳��
			chkreturnsn = 0;
		}
		break;
	}
	return;
}
void MsgCrk_OnCommand(HWND hWnd,int unuseint,HWND unusehWnd,UINT id) {
	switch (LOWORD(gwParam))
	{
	case ID_B_SEATNUM0:
		if (snbtncount < 2) {
			snbtncount++;
			lstrcat(tgnum, "0");
			SetWindowText(hEdit_Sn, tgnum);
		}
		break;
	case ID_B_SEATNUM1:
		if (snbtncount < 2) {
			snbtncount++;
			lstrcat(tgnum, "1");
			SetWindowText(hEdit_Sn, tgnum);
		}
		break;
	case ID_B_SEATNUM2:
		if (snbtncount < 2) {
			snbtncount++;
			lstrcat(tgnum, "2");
			SetWindowText(hEdit_Sn, tgnum);
		}
		break;
	case ID_B_SEATNUM3:
		if (snbtncount < 2) {
			snbtncount++;
			lstrcat(tgnum, "3");
			SetWindowText(hEdit_Sn, tgnum);
		}
		break;
	case ID_B_SEATNUM4:
		if (snbtncount < 2) {
			snbtncount++;
			lstrcat(tgnum, "4");
			SetWindowText(hEdit_Sn, tgnum);
		}
		break;
	case ID_B_SEATNUM5:
		if (snbtncount < 2) {
			snbtncount++;
			lstrcat(tgnum, "5");
			SetWindowText(hEdit_Sn, tgnum);
		}
		break;
	case ID_B_SEATNUM6:
		if (snbtncount < 2) {
			snbtncount++;
			lstrcat(tgnum, "6");
			SetWindowText(hEdit_Sn, tgnum);
		}
		break;
	case ID_B_SEATNUM7:
		if (snbtncount < 2) {
			snbtncount++;
			lstrcat(tgnum, "7");
			SetWindowText(hEdit_Sn, tgnum);
		}
		break;
	case ID_B_SEATNUM8:
		if (snbtncount < 2) {
			snbtncount++;
			lstrcat(tgnum, "8");
			SetWindowText(hEdit_Sn, tgnum);
		}
		break;
	case ID_B_SEATNUM9:
		if (snbtncount < 2) {
			snbtncount++;
			lstrcat(tgnum, "9");
			SetWindowText(hEdit_Sn, tgnum);
		}
		break;
	case ID_B_ORDERCOUNT0:
		if (ocbtncount < 2) {
			ocbtncount++;
			lstrcat(tgitemcount, "0");
			SetWindowText(hEdit_OrderCount, tgitemcount);
		}
		break;
	case ID_B_ORDERCOUNT1:
		if (ocbtncount < 2) {
			ocbtncount++;
			lstrcat(tgitemcount, "1");
			SetWindowText(hEdit_OrderCount, tgitemcount);
		}
		break;
	case ID_B_ORDERCOUNT2:
		if (ocbtncount < 2) {
			ocbtncount++;
			lstrcat(tgitemcount, "2");
			SetWindowText(hEdit_OrderCount, tgitemcount);
		}
		break;
	case ID_B_ORDERCOUNT3:
		if (ocbtncount < 2) {
			ocbtncount++;
			lstrcat(tgitemcount, "3");
			SetWindowText(hEdit_OrderCount, tgitemcount);
		}
		break;
	case ID_B_ORDERCOUNT4:
		if (ocbtncount < 2) {
			ocbtncount++;
			lstrcat(tgitemcount, "4");
			SetWindowText(hEdit_OrderCount, tgitemcount);
		}
		break;
	case ID_B_ORDERCOUNT5:
		if (ocbtncount < 2) {
			ocbtncount++;
			lstrcat(tgitemcount, "5");
			SetWindowText(hEdit_OrderCount, tgitemcount);
		}
		break;
	case ID_B_ORDERCOUNT6:
		if (ocbtncount < 2) {
			ocbtncount++;
			lstrcat(tgitemcount, "6");
			SetWindowText(hEdit_OrderCount, tgitemcount);
		}
		break;
	case ID_B_ORDERCOUNT7:
		if (ocbtncount < 2) {
			ocbtncount++;
			lstrcat(tgitemcount, "7");
			SetWindowText(hEdit_OrderCount, tgitemcount);
		}
		break;
	case ID_B_ORDERCOUNT8:
		if (ocbtncount < 2) {
			ocbtncount++;
			lstrcat(tgitemcount, "8");
			SetWindowText(hEdit_OrderCount, tgitemcount);
		}
		break;
	case ID_B_ORDERCOUNT9:
		if (ocbtncount < 2) {
			ocbtncount++;
			lstrcat(tgitemcount, "9");
			SetWindowText(hEdit_OrderCount, tgitemcount);
		}
		break;
		//���� ��ư ��������
	case ID_B_USERUPDATE:
		GetWindowText(hEdit_Id, tgId, sizeof(tgId));
		GetWindowText(hEdit_Name, tgName, sizeof(tgName));
		GetWindowText(hEdit_Pw, tgPw, sizeof(tgPw));
		GetWindowText(hEdit_Pn, tgPn, sizeof(tgPn));
		GetWindowText(hEdit_Addr, tgAddr, sizeof(tgAddr));
		GetWindowText(hEdit_Birth, tgBirth, sizeof(tgBirth));
		if (lstrlen(tgId) != 0 && lstrlen(tgPw) != 0 && lstrlen(tgName) != 0 && lstrlen(tgPn) != 0 && lstrlen(tgAddr) != 0 && lstrlen(tgBirth) != 0 && ChkStringInfo(tgId) == 0 && ChkStringInfo(tgPw) == 0 && ChkStringInfo(tgName) == 0 && ChkStringInfo(tgPn) == 0 && ChkStringInfo(tgAddr) == 0 && ChkStringInfo(tgBirth) == 0)RequestUpdate();
		else MessageBox(hWnd, "�Է� ����!!", "ȸ������", MB_OK);
		break;
		//�������� �������� ��ư ��������
	case ID_B_USER_GO_UPDATE:
		UserUpdate();
		break;
		//�α���ȭ�鿡�� ���� �� ���� ��ư �������� �α��ο��� ���� �� ��������
	case ID_B_GO_JOIN:

		LoginToJoin();
		break;
		//�α��� ��ư ��������
	case ID_B_LOGIN:
		GetWindowText(hEdit_Login_Id, tgId, sizeof(tgId));
		GetWindowText(hEdit_Login_Pw, tgPw, sizeof(tgPw));
		if (lstrlen(tgId) != 0 && lstrlen(tgPw) != 0 && ChkStringInfo(tgId)==0 && ChkStringInfo(tgPw) == 0)RequestLogin();
		else MessageBox(hWnd, "�Է� ����!!", "�α���", MB_OK);
		break;

		//�����۸���Ʈ ��Ƽ��
	case ID_L_ITEM:
		switch (HIWORD(gwParam)) {
		case LBN_SELCHANGE:
			selitemi = SendMessage(hListBox_ItemList, LB_GETCURSEL, 0, 0);													//�ֹ� ��ǰ ����(����Ʈ�ڽ��� �ε����� 1�ڸ� ��������)
			selitemi += 1;
			wsprintf(tgselitemi, "%d", selitemi);																									//������ �ε��� ���
			selitemtrue = 1;
		}
		break;
		//�ֹ� ��������
	case ID_B_ORDER:
		if (selitemtrue == 1 && ocbtncount == 2 && lstrcmp(tgitemcount, "00") != 0)RequestOrder();																							//������ ���� üũ
		else {
			SendMessage(hListBox_ItemList, LB_SETCURSEL, (WPARAM)-1, 0);															//����Ʈ�ڽ��� ���� �� �ʱ�ȭ
			selitemtrue = 0;																																			//�Է¾����� �ʱ�ȭ
			lstrcpy(tgitemcount, "");																																//�Է¾����ۼ��� �ʱ�ȭ
			SetWindowText(hEdit_OrderCount, tgitemcount);
			MessageBox(hWnd, "�Է� ����!!", "�ֹ�", MB_OK);
			ocbtncount = 0;																																		//�ֹ����� ��ư Ƚ�� �ʱ�ȭ
		}
		lstrcpy(tgitemcount, "");																											//�ֹ����� ���ڿ� �ʱ�ȭ
		selitemtrue = 0;
		break;

		//���Թ�ư ��������
	case ID_B_JOIN:
		GetWindowText(hEdit_Id, tgId, sizeof(tgId));
		GetWindowText(hEdit_Name, tgName, sizeof(tgName));
		GetWindowText(hEdit_Pw, tgPw, sizeof(tgPw));
		GetWindowText(hEdit_Pn, tgPn, sizeof(tgPn));
		GetWindowText(hEdit_Addr, tgAddr, sizeof(tgAddr));
		GetWindowText(hEdit_Birth, tgBirth, sizeof(tgBirth));
		if (lstrlen(tgId) != 0 && lstrlen(tgPw) != 0 && lstrlen(tgName) != 0 && lstrlen(tgPn) != 0 && lstrlen(tgAddr) != 0 && lstrlen(tgBirth) != 0 && ChkStringInfo(tgId) == 0 && ChkStringInfo(tgPw) == 0 && ChkStringInfo(tgName) == 0 && ChkStringInfo(tgPn) == 0 && ChkStringInfo(tgAddr) == 0 && ChkStringInfo(tgBirth) == 0)RequestJoin();
		else MessageBox(hWnd, "�Է� ����!!", "ȸ������", MB_OK);
		break;

		//���ȭ��ּ� �ֹ����� ��ư �������� ���ȭ�鿡�� �ֹ�ȭ������ 
	case ID_B_GO_ORDER:

		UserToOrder();
		break;
		//�ڸ�����ȭ�鿡�� �ڸ����ù�ư ��������
	case ID_B_SELSEAT://																																

		if (snbtncount == 2 && lstrcmp(tgnum, "00") != 0)RequestSelSeat();
		else {
			lstrcpy(tgnum, "");																																//�Է¼��� �ʱ�ȭ
			SetWindowText(hEdit_Sn, tgnum);
			MessageBox(hWnd, "�Է� ����!!", "�ڸ���ȣ", MB_OK);
			snbtncount = 0;																																		//�ڸ���ȣ��ư Ƚ�� �ʱ�ȭ
		}
		break;
	case ID_B_LOGOUT://																																			//�α׾ƿ�(�α��� ������)

		RequestLogout();
		break;
	case ID_B_DELETE:																																			//Ż�� ����(�α��� ������)

		RequestDelete();
		break;
	}
	return;
}
void MsgCrk_OnDestroy(HWND hWnd) {
	PostQuitMessage(0);
	return;
}
