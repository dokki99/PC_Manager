//��ǰ�����Ҷ� ���̵� ������
//ShowWindow(hWnd, nCmdShow);
#include <Windows.h>
#include <stdio.h>
#include <CommCtrl.h>
#include <sql.h>
#include <sqlext.h>
#include "resource.h"
#define ID_B_GO_JOIN_OR_UPDATE 140
#define ID_B_GO_ORDER 141
HWND hButton_Go_Join_Or_Update;
HWND hButton_Go_Order;
//ȭ�� �ڵ�
HWND hWndMain;		// ����
HWND hWnd_C;			// ����
HWND hWnd_JU;		// ����/����
HWND hWnd_L;			// �α���
HWND hWnd_S;			// �ڸ�����
HWND hWnd_O;			// �ֹ� 
//ȭ�� ����
LPCTSTR C_Class = TEXT("����");					//����
LPCTSTR JU_Class = TEXT("ȸ������/����");//����/����
LPCTSTR L_Class = TEXT("�α���");				//�α���
LPCTSTR S_Class = TEXT("�ڸ�����");			//�ڸ�����
LPCTSTR O_Class = TEXT("�ֹ�");					//�ֹ�
////��Ʈ�� �ڵ�
//���ν���
HWND hEdit_Chat;									//ä�� ����Ʈ
HWND hButton_Logout;							//�α׾ƿ� ��ư
HWND hButton_Delete;							//Ż�� ��ư
HWND hEdit_Timer;								//Ż�� ����Ʈ
//����
HWND hEdit_Ip;										//IP ����Ʈ
HWND hEdit_Port;									//PORT ����Ʈ
HWND hButton_Connect;						//���� ��ư
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
//�ڸ�����
HWND hEdit_Sn;										//�ڸ���ȣ ����Ʈ
HWND hButton_SelSeat;						//�ڸ���ȣ���� ��ư
//�ֹ�
HWND hListBox_ItemList;						//�����۸���Ʈ ����Ʈ�ڽ�
HWND hEdit_OrderCount;						//�������ֹ����� ����Ʈ
HWND hButton_Order;							//�ֹ� ��ư
///����ƽ
//����
HWND hStatic_Ip;									//IP
HWND hStatic_Port;								//PORT
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
//���ν���
HWND hStatic_Chat;								//ä��
HWND hStatic_Timer;							//Ÿ�̸�
//���ν����Լ���
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);															//�⺻
LRESULT CALLBACK ConnectProc(HWND, UINT, WPARAM, LPARAM);														//����
LRESULT CALLBACK Join_Or_UpdateProc(HWND, UINT, WPARAM, LPARAM);										//����/����
LRESULT CALLBACK LoginProc(HWND, UINT, WPARAM, LPARAM);															//�α���
LRESULT CALLBACK SelSeatProc(HWND, UINT, WPARAM, LPARAM);														//�ڸ�����
LRESULT CALLBACK OrderProc(HWND, UINT, WPARAM, LPARAM);														//�ֹ�

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
//�������ֹ�����
TCHAR tgselitemi[10];																																				//�ε����� ���ڿ��� ��ȯ
int selitemi;																																								//����Ʈ �ڽ��� �ε���
TCHAR tgitemcount[10] = "";																																	//���� ���ڿ�
//
///
//���� üũ ����
int chkseat = 0;																																							//�ڸ�����
int chkjoin = 0;																																							//����
int chklogin = 0;																																						//�α���
int chktimer;																																							//Ÿ�̸� ����(1�̸� ����/0�̸� ����)	
int chkendbtn = 0;																																					//�α׾ƿ�/Ż�� 
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

//DWORD WINAPI ThreadFunc(LPVOID){}:���ú꽺����ν� ��û�� ����� �ٷ�
//�Ӹ����� ���� ���� ���ڰ�  1�� ���� ����/ 0�� ���� �Ұ�
DWORD WINAPI ThreadFunc(LPVOID Param) {
	HDC hdc;
	SOCKET* P = (SOCKET*)Param;
	for (;;) {
		lstrcpy(buf, "");
		nReturn = recv(*P, buf, 1024, 0);
		if (nReturn == 0 || nReturn == SOCKET_ERROR) {
			continue;
		}
		else {
			//orderbuf�� ���� Ŀ�ǵ尡 �ֹ��� ���Ѱ����� �Ǻ�
			orderi = 0;
			lstrcpy(orderbuf, "");
			for (int i = 0; i < 3; i++) {
				orderbuf[orderi++] = buf[i];
			}
			orderbuf[orderi] = '\0';
			//
			//�ڸ����� ���(����:11/�Ұ�:10)
			if (buf[0] == '1') {
				if (lstrcmp(buf, TEXT("11")) == 0) {
					chkseat = 1;																																			//�ڸ����� üũ
					lstrcpy(usersn, tgnum);																															//�ڸ���ȣ ���									
					wsprintf(buf, "%s", "�ڸ���밡��");
					if (chkseat == 1 && chklogin == 0) {
						hWnd_L = CreateWindow(L_Class, L_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
						ShowWindow(hWnd_L, SW_SHOW);
					}
					else {
						if (chkseat == 0)MessageBox(hWndMain, "�ڸ� ���� ���� �ϼ���!!", "����", MB_OK);
						if (chklogin == 1)MessageBox(hWndMain, "�α��� �Ǿ� �ֽ��ϴ�!!", "����", MB_OK);
					}
				}
				else wsprintf(buf, "%s", "�ڸ����Ұ�");
			}
			//ȸ������ ���(����:31/�Ұ�:30)
			else if (buf[0] == '3') {
				if (lstrcmp(buf, TEXT("31")) == 0) {
					chkjoin = 1;
					wsprintf(buf, "%s", "ȸ�����Լ���");
				}
				else wsprintf(buf, "%s", "ȸ�����Խ���");
			}
			//�α��� ���(����:41NAME,PN,ADDR,BIRTH/�Ұ�:4100 �Ǵ� 40)
			else if (buf[0] == '4') {
				if (buf[1] == '1') {
					//�����ð� ���
					pctimei = 0;
					for (int i = 2; i < 4; i++) {
						pctimebuf[pctimei++] = buf[i];
					}
					pctimebuf[pctimei] = '\0';
					pctime = atoi(pctimebuf);
					//
					//�����Ҷ� �Է��� ���̵� ����� �����ϰ� �̸�,��ȭ��ȣ,�ּ�,���ϵ��� ���
					if (pctime >= 0) {
						lstrcpy(userid, tgId);																											//���� ���̵�,��� ���
						lstrcpy(userpw, tgPw);
						i = 4;																																		//�α��� Ŀ�ǵ��� ������ ������  ������
						turn = -1;																																//������ ������ ������� �������� ����
						///�α��� Ŀ��忡�� ������ ������ ��� ���μ���(NAME-PN-ADDR-BIRTH��)
						while (buf[i] != '\0') {
							infoi = 0;
							if (buf[i] == ':') {																												///�� ������ ���������� �̵�
								ini = i + 1;
								turn++;
							}
							if (turn == 0) {
								while (buf[ini] != ':' && buf[ini] != '\0') {																		///":"������ ���
									info[infoi++] = buf[ini++];
								}
								if (infoi >= 2) infoi -= 2;																								///���� ���� Ŀ�ǵ�(PW,NAME..) ���� ��ŭ ����
								info[infoi] = '\0';
								lstrcpy(tgName, info);
							}
							else if (turn == 1) {
								while (buf[ini] != ':' && buf[ini] != '\0') {
									info[infoi++] = buf[ini++];
								}
								if (infoi >= 4) infoi -= 4;
								info[infoi] = '\0';
								lstrcpy(tgPn, info);
							}
							else if (turn == 2) {
								while (buf[ini] != ':' && buf[ini] != '\0') {
									info[infoi++] = buf[ini++];
								}
								if (infoi >= 5) infoi -= 5;
								info[infoi] = '\0';
								lstrcpy(tgAddr, info);
							}
							else if (turn == 3) {
								while (buf[ini] != '\0') {																									///���������� ���
									info[infoi++] = buf[ini++];
								}
								info[infoi] = '\0';
								lstrcpy(tgBirth, info);
								turn = 0;
								break;
							}
							i++;
						}

						i = 0;
						wsprintf(tpctime, "%d", pctime);																								//�����ð��� ����ϱ����� ���ڿ���

						if (pctime > 0) {
							chklogin = 1;
							chktimer = 1;																														//�α��εǸ� Ÿ�̸� ����
							wsprintf(buf, "%s", "�α��μ���");
						}
						else {
							hWnd_O = CreateWindow(O_Class, O_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
							ShowWindow(hWnd_O, SW_SHOW);
							wsprintf(buf, "%s", "�α��ν���(�ܿ��ð�����)");
						}
					}
				}
				else wsprintf(buf, "%s", "�α��ν���");
			}
			////�ֹ����(����:2��ǰ��ȣ(1,2,3)1/�Ұ�:2��ǰ��ȣ(1,2,3)0)
			else if (buf[0] == '2') {
				//����
				if (buf[1] == '1') {
					if (lstrcmp(orderbuf, TEXT("211")) == 0) {
						wsprintf(buf, "%s", "DRINK���Լ���");
					}
					else wsprintf(buf, "%s", "DRINK���Խ���");
				}
				//��ǰ
				else if (buf[1] == '2') {
					if (lstrcmp(orderbuf, TEXT("221")) == 0) {
						wsprintf(buf, "%s", "FOOD���Լ���");
					}
					else wsprintf(buf, "%s", "FOOD���Խ���");
				}
				//PC
				else if (buf[1] == '3') {
					if (lstrcmp(orderbuf, TEXT("231")) == 0) {
						wsprintf(buf, "%s", "PC���Լ���");
					}
					else wsprintf(buf, "%s", "PC���Խ���");
				}
			}
			//�α׾ƿ� ���(����:51/�Ұ�:50
			else if (buf[0] == '5') {
				if (lstrcmp(buf, TEXT("51")) == 0) {
					lstrcpy(userid, "");
					wsprintf(buf, "%s", "�α׾ƿ�����");
					chklogin = 0;
				}
				else 	wsprintf(buf, "%s", "�α׾ƿ�����");
			}
			//���� ���(����:61/�Ұ�:60)
			else if (buf[0] == '6') {
				if (lstrcmp(buf, TEXT("61")) == 0) {
					wsprintf(buf, "%s", "��������");
				}
				else wsprintf(buf, "%s", "��������");
			}
			//Ż�� ���(����:71/�Ұ�:70)
			else if (buf[0] == '7') {
				if (lstrcmp(buf, TEXT("71")) == 0) {
					chklogin = 0;
					wsprintf(buf, "%s", "Ż�𼺰�");
				}
				else wsprintf(buf, "%s", "Ż�����");
			}
			//�����κ��� ���� �޼����� ä�� ����Ʈ�� "������ �޼���:+��ȯ�� �޼���"�� ����
			if (lstrlen(buf) != 0) {
				MessageBox(hWndMain, buf, "!!", MB_OK);
			}
		}
	}
	return 0;
}
//int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int):�޴� ����, ��� �ٸ� ���ν��� ����
int APIENTRY WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
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
	WndClass.lpszMenuName = NULL;																			//�޴��ޱ�
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
//void Init_Wnd(WNDCLASS*, int):���������ν������� ��� �ٸ� ���ν��� ����
void Init_Wnd(WNDCLASS* Wnd, int Proc_No) {
	if (Proc_No > 2) {
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
	case 0:																																												//�α���ȭ��
		Wnd->lpfnWndProc = LoginProc;
		Wnd->lpszClassName = L_Class;
		break;
	case 1:																																												//����/����ȭ��
		Wnd->lpfnWndProc = Join_Or_UpdateProc;
		Wnd->lpszClassName = JU_Class;
		break;
	case 2:																																												//�ֹ�ȭ��
		Wnd->lpfnWndProc = OrderProc;
		Wnd->lpszClassName = O_Class;
		break;
	}
	Wnd->lpszMenuName = NULL;																																			//��� �ٸ� ���ν����� �޴��� ����
	Wnd->style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(Wnd);
}
//void CLIENT_Open():���� ���� �Լ�(���ὺ���� �����)
void CLIENT_Open() {
	nip = atoi(tgIp);
	nport = atoi(tgPort);
	nReturn = WSAStartup(WORD(2.0), &wsadata);
	clientsock = socket(AF_INET, SOCK_STREAM, 0);
	addr_client.sin_family = AF_INET;
	addr_client.sin_addr.s_addr = inet_addr(tgIp);
	addr_client.sin_port = htons(nport);
	hThread = CreateThread(NULL, 0, ThreadFunc, &clientsock, 0, &ThreadID);																	//���ὺ���� �����
	nReturn = connect(clientsock, (sockaddr*)&addr_client, addrlen_clt);
	if (nReturn) {
		MessageBox(hWndMain, "����Ǿ����ϴ�!!", "����", MB_OK);
	}
}
//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM):�⺻ȭ��(�ٸ� ȭ�� ����,�α��ε����� Ÿ�̸� ����)
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		InitCommonControls();

		CLIENT_Open();

		//��� �ٸ� ȭ���
		WNDCLASS Wnd_JU;																															//����/����
		WNDCLASS Wnd_L;																															//�α���
		WNDCLASS Wnd_O;																															//�ֹ�

		///������ ����
		// �α���
		Init_Wnd(&Wnd_L, 0);
		// ����/����
		Init_Wnd(&Wnd_JU, 1);
		// �ֹ�
		Init_Wnd(&Wnd_O, 2);
		///
		///��Ʈ��
		//�ڸ���ȣ ����ƽ
		hStatic_Sn = CreateWindow(TEXT("static"), TEXT("SN(2�ڸ�)"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 0, 80, 20, hWnd, (HMENU)ID_S_SN, g_hInst, NULL);
		if (chklogin == 1)ShowWindow(hStatic_Sn, SW_HIDE);
		//�ڸ���ȣ ����Ʈ
		hEdit_Sn = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 50, 100, 50, hWnd, (HMENU)ID_E_SN, g_hInst, NULL);
		if (chklogin == 1)ShowWindow(hEdit_Sn, SW_HIDE);
		//�ڸ����� ��ư
		hButton_SelSeat = CreateWindow(TEXT("button"), TEXT("SELECTSEAT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 10, 200, 50, hWnd, (HMENU)ID_B_SELSEAT, g_hInst, NULL);
		if (chklogin == 1)ShowWindow(hButton_SelSeat, SW_HIDE);
		//�ֹ����� ��ư
		hButton_Go_Order = CreateWindow(TEXT("button"), TEXT("ORDER"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 200, 200, 50, hWnd, (HMENU)ID_B_GO_ORDER, g_hInst, NULL);
		if (chklogin == 0)ShowWindow(hButton_Go_Order, SW_HIDE);
		///

		///��Ʈ��
		//�α׾ƿ� ��ư
		hButton_Logout = CreateWindow(TEXT("button"), TEXT("LOGOUT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			610, 50, 200, 65, hWnd, (HMENU)ID_B_LOGOUT, g_hInst, NULL);
		if (chklogin == 0)ShowWindow(hButton_Logout, SW_HIDE);
		//Ż�� ��ư
		hButton_Delete = CreateWindow(TEXT("button"), TEXT("DELETE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 50, 200, 65, hWnd, (HMENU)ID_B_DELETE, g_hInst, NULL);
		if (chklogin == 0)ShowWindow(hButton_Delete, SW_HIDE);
		//Ÿ�̸� ����Ʈ
		hEdit_Timer = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			610, 200, 100, 100, hWnd, (HMENU)ID_E_TIMER, g_hInst, NULL);
		if (chklogin == 0)ShowWindow(hEdit_Timer, SW_HIDE);
		//Ÿ�̸� ����ƽ
		hStatic_Timer = CreateWindow(TEXT("static"), TEXT("TIME"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			610, 150, 40, 20, hWnd, (HMENU)ID_S_TIMER, g_hInst, NULL);
		if (chklogin == 0)ShowWindow(hStatic_Timer, SW_HIDE);
		///
		//Ÿ�̸�(�α��� �� ����)
		SetTimer(hWnd, 1, 1000, NULL);			//�ð� Ÿ�̸�
		SetTimer(hWnd, 2, 1000, NULL);			//�ð� Ÿ�̸� �����ϴ� Ÿ�̸�
		return 0;
	case WM_TIMER:
		switch (wParam) {
			//���� �ð� Ÿ�̸�(0�� �ɶ����� �ð� �����ϰ� 0�̸� �α׾ƿ�ó���ϱ�)
		case 1:
			if (chktimer == 1) {
				pctime -= 1;
				wsprintf(tpctime, "%d", pctime);
				SetWindowText(hEdit_Timer, tpctime);																						//���ν����� Ÿ�̸� ����Ʈ�� ���� ���� �ð� ���
				if (pctime == 0) {
					wsprintf(tpctime, "%d", pctime);
					SetWindowText(hEdit_Timer, tpctime);
					lstrcpy(tgcmdserver, "");
					lstrcpy(tgcmdserver, "5ID:");
					lstrcat(tgcmdserver, userid);
					lstrcat(tgcmdserver, "PW:");
					lstrcat(tgcmdserver, userpw);
					lstrcat(tgcmdserver, "SN:");
					lstrcat(tgcmdserver, usersn);
					chkendbtn = 1;																															//�α׾ƿ�/Ż�� ó���� 
					chklogin = 0;																																//�α׾ƿ� �� ���·�
					nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);
					chktimer = 0;																															//Ÿ�̸� ���� ���·�
					break;
				}
			}
			break;
			//�������ư ��������/�����ð� 0�϶� �����ð� Ÿ�̸� �����ϰ� �ٽ� Ÿ�̸� �����(�α����� ��� �ޱ� ����)
		case 2:
			if (chkendbtn == 1) {
				KillTimer(hWnd, 1);																														//Ÿ�̸� ����
				chktimer = 0;																																//Ÿ�̸� ���� ���·�
				SetTimer(hWnd, 1, 1000, NULL);																									//���� ���� �ð� Ÿ�̸� �ٽ� ����
				chkendbtn = 0;																																//�α׾ƿ� üũ �ٽ� ���� ��� �ʱ�ȭ
				pctime = 0;
				wsprintf(tpctime, "%d", pctime);
				SetWindowText(hEdit_Timer, tpctime);																						//���ν����� Ÿ�̸� ����Ʈ�� ���� ���� �ð� 0����  ���
			}
			break;
		}
		return 0;

		//�޴� ������ �ش� ���ν��� ����
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			//�ڸ����� ��������
		case ID_B_GO_ORDER:
			hWnd_O = CreateWindow(O_Class, O_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
			ShowWindow(hWnd_O, SW_SHOW);
			break;
		case ID_B_SELSEAT:
			lstrcpy(tgcmdserver, "1");
			GetWindowText(hEdit_Sn, tgnum, sizeof(tgnum));																				//�ڸ� ����(02�� ���� 2�ڸ� ��������)
			lstrcat(tgcmdserver, tgnum);
			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);														//������ �ڸ����� Ŀ�ǵ� ������
			/*if (chkseat == 1 && chklogin == 0) {
				hWnd_L = CreateWindow(L_Class, L_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
				ShowWindow(hWnd_L, SW_SHOW);
			}
			else {
				if (chkseat == 0)MessageBox(hWnd, "�ڸ� ���� ���� �ϼ���!!", "����", MB_OK);
				if (chklogin == 1)MessageBox(hWnd, "�α��� �Ǿ� �ֽ��ϴ�!!", "����", MB_OK);
			}*/
			break;
		case ID_B_LOGOUT:																																			//�α׾ƿ�(�α��� ������)
			if (chklogin == 1) {
				lstrcpy(tgcmdserver, "");
				lstrcpy(tgcmdserver, "5ID:");
				lstrcat(tgcmdserver, userid);
				lstrcat(tgcmdserver, "PW:");
				lstrcat(tgcmdserver, userpw);
				lstrcat(tgcmdserver, "SN:");
				lstrcat(tgcmdserver, usersn);
				chkendbtn = 1;																																				//�α׾ƿ�/Ż�� ���� ��������
				nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);																//������ �α׾ƿ� Ŀ��� ������
				pctime = 0;
				wsprintf(tpctime, "%d", pctime);
				SetWindowText(hEdit_Timer, tpctime);																										//���ν����� Ÿ�̸� ����Ʈ�� ���� ���� �ð� 0���� ���
			}
			else MessageBox(hWnd, "�α����ϼ���!!", "����", MB_OK);
			break;
		case ID_B_DELETE:																																			//Ż�� ����(�α��� ������)
			if (chklogin == 1) {
				lstrcpy(tgcmdserver, "");
				lstrcpy(tgcmdserver, "7ID:");
				lstrcat(tgcmdserver, userid);
				lstrcat(tgcmdserver, "PW:");
				lstrcat(tgcmdserver, userpw);
				lstrcat(tgcmdserver, "SN:");
				lstrcat(tgcmdserver, usersn);
				chkendbtn = 1;																																				//�α׾ƿ�/Ż�� ���� ��������
				nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);																//������ Ż�� Ŀ�ǵ� ������
				pctime = 0;
				wsprintf(tpctime, "%d", pctime);
				SetWindowText(hEdit_Timer, tpctime);																										//���ν����� Ÿ�̸� ����Ʈ�� ���� ���� �ð� 0���� ���
			}
			else MessageBox(hWnd, "�α����ϼ���!!", "����", MB_OK);
			break;
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
//LRESULT CALLBACK ConnectProc(HWND, UINT, WPARAM, LPARAM):����ȭ��(���� ����)
LRESULT CALLBACK ConnectProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case WM_CREATE:
		///��Ʈ��
		//������ ����ƽ
		hStatic_Ip = CreateWindow(TEXT("static"), TEXT("IP"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 0, 20, 20, hWnd, (HMENU)ID_S_IP, g_hInst, NULL);
		//������ ����Ʈ
		hEdit_Ip = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 50, 200, 50, hWnd, (HMENU)ID_E_IP, g_hInst, NULL);
		//��Ʈ ����ƽ
		hStatic_Port = CreateWindow(TEXT("static"), TEXT("PORT"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			310, 0, 40, 20, hWnd, (HMENU)ID_S_PORT, g_hInst, NULL);
		//��Ʈ ����Ʈ
		hEdit_Port = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			310, 50, 200, 50, hWnd, (HMENU)ID_E_PORT, g_hInst, NULL);
		//���� ��ư
		hButton_Connect = CreateWindow(TEXT("button"), TEXT("CONNECT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			310, 200, 400, 100, hWnd, (HMENU)ID_B_CONNECT, g_hInst, NULL);
		///
		//������ IP�� PORT����Ʈ�� ����
		SetWindowText(hEdit_Ip, tgIp);
		SetWindowText(hEdit_Port, tgPort);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_B_CONNECT:																																			//���� ��ư �������� ���� ����
			CLIENT_Open();
			break;
		}
		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
//LRESULT CALLBACK Join_Or_UpdateProc(HWND, UINT, WPARAM, LPARAM):����/����ȭ��(����/���� �����ϰ� �α��ε� ������ ������ ����)
LRESULT CALLBACK Join_Or_UpdateProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case WM_CREATE:
		///��Ʈ��
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
		//���� ��ư
		hButton_Update = CreateWindow(TEXT("button"), TEXT("UPDATE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 300, 300, 200, hWnd, (HMENU)ID_B_UPDATE, g_hInst, NULL);
		///
		//�α��� �� ���� ���� ����
		SetWindowText(hEdit_Id, userid);																													//���̵�
		SetWindowText(hEdit_Pw, userpw);																												//���
		SetWindowText(hEdit_Name, tgName);																											//�̸�
		SetWindowText(hEdit_Pn, tgPn);																														//��ȭ��ȣ
		SetWindowText(hEdit_Addr, tgAddr);																											//�ּ�
		SetWindowText(hEdit_Birth, tgBirth);																											//����
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			//���Թ�ư ��������
		case ID_B_JOIN:
			//Ŀ�ǵ�,���� ���� ���ڿ� ��ĭ���� �ʱ�ȭ
			lstrcpy(tgcmdserver, "");																																//�������� Ŀ�ǵ�
			lstrcpy(tgId, "");																																			//���̵�
			lstrcpy(tgName, "");																																		//�̸�
			lstrcpy(tgPw, "");																																			//���
			lstrcpy(tgPn, "");																																			//��ȭ��ȣ
			lstrcpy(tgAddr, "");																																		//�ּ�
			lstrcpy(tgBirth, "");																																		//����
			//���� ���� ����Ʈ���� ���� ���ڿ��� ���
			GetWindowText(hEdit_Id, tgId, sizeof(tgId));
			GetWindowText(hEdit_Name, tgName, sizeof(tgName));
			GetWindowText(hEdit_Pw, tgPw, sizeof(tgPw));
			GetWindowText(hEdit_Pn, tgPn, sizeof(tgPn));
			GetWindowText(hEdit_Addr, tgAddr, sizeof(tgAddr));
			GetWindowText(hEdit_Birth, tgBirth, sizeof(tgBirth));
			lstrcpy(tgcmdserver, "3ID:");
			lstrcat(tgcmdserver, tgId);
			lstrcat(tgcmdserver, "PW:");
			lstrcat(tgcmdserver, tgPw);
			lstrcat(tgcmdserver, "NAME:");
			lstrcat(tgcmdserver, tgName);
			lstrcat(tgcmdserver, "PN:");
			lstrcat(tgcmdserver, tgPn);
			lstrcat(tgcmdserver, "ADDR:");
			lstrcat(tgcmdserver, tgAddr);
			lstrcat(tgcmdserver, "BIRTH:");
			lstrcat(tgcmdserver, tgBirth);
			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);																	//������ ���� Ŀ�ǵ� ������
			break;
			//������ư ��������
		case ID_B_UPDATE:
			//Ŀ�ǵ�,���� ���� ���ڿ� ��ĭ���� �ʱ�ȭ
			lstrcpy(tgcmdserver, "");																																	//Ŀ�ǵ�
			lstrcpy(tgId, "");																																				//���̵�
			lstrcpy(tgName, "");																																			//�̸�
			lstrcpy(tgPw, "");																																				//���
			lstrcpy(tgPn, "");																																				//��ȭ��ȣ
			lstrcpy(tgAddr, "");																																			//�ּ�
			lstrcpy(tgBirth, "");																																			//����
			//���� ���� ����Ʈ���� ���� ���ڿ��� ���
			GetWindowText(hEdit_Id, tgId, sizeof(tgId));
			GetWindowText(hEdit_Name, tgName, sizeof(tgName));
			GetWindowText(hEdit_Pw, tgPw, sizeof(tgPw));
			GetWindowText(hEdit_Pn, tgPn, sizeof(tgPn));
			GetWindowText(hEdit_Addr, tgAddr, sizeof(tgAddr));
			GetWindowText(hEdit_Birth, tgBirth, sizeof(tgBirth));
			lstrcpy(tgcmdserver, "6ID:");
			lstrcat(tgcmdserver, tgId);
			lstrcat(tgcmdserver, "PW:");
			lstrcat(tgcmdserver, tgPw);
			lstrcat(tgcmdserver, "NAME:");
			lstrcat(tgcmdserver, tgName);
			lstrcat(tgcmdserver, "PN:");
			lstrcat(tgcmdserver, tgPn);
			lstrcat(tgcmdserver, "ADDR:");
			lstrcat(tgcmdserver, tgAddr);
			lstrcat(tgcmdserver, "BIRTH:");
			lstrcat(tgcmdserver, tgBirth);
			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);																			//������ ���� Ŀ�ǵ� ������
			break;
		}
		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
//LRESULT CALLBACK LoginProc(HWND, UINT, WPARAM, LPARAM):�α���ȭ��(�α��� ����)
LRESULT CALLBACK LoginProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case WM_CREATE:
		///��Ʈ��
		//�α��� ���̵� ����ƽ
		hStatic_Login_Id = CreateWindow(TEXT("static"), TEXT("ID"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 0, 40, 20, hWnd, (HMENU)ID_S_LOGIN_ID, g_hInst, NULL);
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
		hButton_Go_Join_Or_Update = CreateWindow(TEXT("button"), TEXT("JOIN/UPDATE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 100, 200, 50, hWnd, (HMENU)ID_B_GO_JOIN_OR_UPDATE, g_hInst, NULL);
		///
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_B_GO_JOIN_OR_UPDATE:
			hWnd_JU = CreateWindow(JU_Class, JU_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
			ShowWindow(hWnd_JU, SW_SHOW);
			break;
			//�α��� ��ư ��������
		case ID_B_LOGIN:
			//Ŀ�ǵ�� ���� ���ڿ� ��ĭ���� �ʱ�ȭ
			lstrcpy(tgcmdserver, "");																																//Ŀ�ǵ�
			lstrcpy(tgId, "");																																			//���̵�
			lstrcpy(tgPw, "");																																			//���
			//�α��� ���� ����Ʈ���� ���� ���ڿ� ���
			GetWindowText(hEdit_Login_Id, tgId, sizeof(tgId));
			GetWindowText(hEdit_Login_Pw, tgPw, sizeof(tgPw));
			lstrcpy(tgcmdserver, "4ID:");
			lstrcat(tgcmdserver, tgId);
			lstrcat(tgcmdserver, "PW:");
			lstrcat(tgcmdserver, tgPw);
			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);																	//������ �α��� Ŀ�ǵ� ������
			break;
		}
		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
//LRESULT CALLBACK SelSeatProc(HWND, UINT, WPARAM, LPARAM):�ڸ�����ȭ��(�ڸ����� ����)

//LRESULT CALLBACK OrderProc(HWND, UINT, WPARAM, LPARAM):�ֹ�ȭ��(�ֹ� ����)
LRESULT CALLBACK OrderProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case WM_CREATE:
		///��Ʈ��
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
		//�ֹ� ��ư
		hButton_Order = CreateWindow(TEXT("button"), TEXT("ORDER"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 10, 200, 50, hWnd, (HMENU)ID_B_ORDER, g_hInst, NULL);
		///
		//�����۸���Ʈ �ʱ�ȭ
		SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"DRINK");
		SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"FOOD");
		SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"PC");
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
			//�����۸���Ʈ ��Ƽ��
		case ID_L_ITEM:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				selitemi = SendMessage(hListBox_ItemList, LB_GETCURSEL, 0, 0);													//�ֹ� ��ǰ ����(����Ʈ�ڽ��� �ε����� 1�ڸ� ��������)
				selitemi += 1;
				wsprintf(tgselitemi, "%d", selitemi);																									//������ �ε��� ���
			}
			break;
			//�ֹ� ��������
		case ID_B_ORDER:
			lstrcpy(tgcmdserver, "2");
			lstrcat(tgcmdserver, tgselitemi);
			GetWindowText(hEdit_OrderCount, tgitemcount, sizeof(tgitemcount));													//�������ֹ����� ���ڿ��� ���
			lstrcat(tgcmdserver, tgitemcount);
			lstrcat(tgcmdserver, "SN:");
			lstrcat(tgcmdserver, usersn);
			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);															//������ �ֹ� Ŀ�ǵ� ������
			break;
		}
		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
