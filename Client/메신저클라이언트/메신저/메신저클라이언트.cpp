//�ϳ��� â�� ��Ʈ���� ���̵� ���Ѽ� ���
//��ǰ�����Ҷ� ���̵� ������
#include <Windows.h>
#include <stdio.h>
#include <CommCtrl.h>
#include <sql.h>
#include <sqlext.h>
#include "resource.h"
//ȭ�� �ڵ�
HWND hWndMain;		// ����
//ȭ�� ����
LPCTSTR C_Class = TEXT("����");					//����
LPCTSTR JU_Class = TEXT("ȸ������/����");//����/����
LPCTSTR L_Class = TEXT("�α���");				//�α���
LPCTSTR S_Class = TEXT("�ڸ�����");			//�ڸ�����
LPCTSTR O_Class = TEXT("�ֹ�");					//�ֹ�
////��Ʈ�� �ڵ�
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

HWND hButton_Go_Join_Or_Update;	//���� �� �������� ��ư
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

//DWORD WINAPI ThreadFunc(LPVOID){}:���ú꽺����ν� ��û�� ����� �ٷ�
//�Ӹ����� ���� ���� ���ڰ�  1�� ���� ����/ 0�� ���� �Ұ�
DWORD WINAPI ThreadFunc(LPVOID Param) {
	HDC hdc;
	SOCKET* P = (SOCKET*)Param;
	TCHAR CODE[3] = "",TEXT[256] = "";
	int j;
	for (;;) {
		lstrcpy(buf, "");
		nReturn = recv(*P, buf, 1024, 0);

		if (nReturn == 0 || nReturn == SOCKET_ERROR) {
			continue;
		}
		else {

			j = 0;
			for (int i = 0; i < lstrlen(buf); i++) {
				if (i > 2) {
					CODE[i] = buf[i];
				}
				else if (i != 3) {
					TEXT[j++] = buf[i];
				}
			}
			CODE[lstrlen(CODE)] = '\0';
			TEXT[lstrlen(TEXT)] = '\0';

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
					returnsntime = 100000000;																													//�ڸ��ݳ�Ÿ�̸ӽð� �ʱ�ȭ
					returnsn = 1;																																			//�ڸ��ݳ�����
					chkseat = 1;																																			//�ڸ����� üũ
					SetTimer(hWndMain, 0, 100, NULL);																									//�ڵ����� ȭ���� ���� Ÿ�̸�
					SetTimer(hWndMain, 3, 1000000, NULL);																							//�ڸ��ݳ�Ÿ�̸�
					//SendMessage(hWndMain, WM_TIMER, 0, 0);
					//SendMessage(hWndMain, WM_TIMER, 3, 0);
					lstrcpy(usersn, tgnum);																															//�ڸ���ȣ ���									
					wsprintf(buf, "%s", "�ڸ���밡��");
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
							wsprintf(buf, "%s", "�α��ν���(�ܿ��ð�����)");
							chkpctime = 1;
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
						chkorder = 1;
						wsprintf(buf, "%s", "DRINK���Լ���");
					}
					else wsprintf(buf, "%s", "DRINK���Խ���");
				}
				//��ǰ
				else if (buf[1] == '2') {
					if (lstrcmp(orderbuf, TEXT("221")) == 0) {
						chkorder = 1;
						wsprintf(buf, "%s", "FOOD���Լ���");
					}
					else wsprintf(buf, "%s", "FOOD���Խ���");
				}
				//PC
				else if (buf[1] == '3') {
					if (lstrcmp(orderbuf, TEXT("231")) == 0) {
						chkorder = 1;
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
				}
				else 	wsprintf(buf, "%s", "�α׾ƿ�����");
			}
			//���� ���(����:61/�Ұ�:60)
			else if (buf[0] == '6') {
				if (lstrcmp(buf, TEXT("61")) == 0) {
					wsprintf(buf, "%s", "��������");
					chkupdate = 1;
				}
				else wsprintf(buf, "%s", "��������");
			}
			//Ż�� ���(����:71/�Ұ�:70)
			else if (buf[0] == '7') {
				if (lstrcmp(buf, TEXT("71")) == 0) {
					wsprintf(buf, "%s", "Ż�𼺰�");
					gohome = 1;																																		//�ڸ� ���ð���
				}
				else wsprintf(buf, "%s", "Ż�����");
			}
			//�ݳ� ���(����:81/�Ұ�:80)
			else if (buf[0] == '8') {
				if (lstrcmp(buf, TEXT("81")) == 0) {
					wsprintf(buf, "%s", "��ȯ����");
					gohome = 1;																																		//�ڸ� ���ð���
				}
				else wsprintf(buf, "%s", "��ȯ����");
			}
			//�����κ��� ���� �޼����� �޼����ڽ��� ����
			if (lstrlen(buf) != 0) {
				MessageBox(hWndMain, buf, "������ �޽���", MB_OK);
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

		///
		///�ڸ����� ��Ʈ��(Ȩȭ��)
		
		//�ڸ���ȣ ����ƽ
		hStatic_Sn = CreateWindow(TEXT("static"), TEXT("SN(2�ڸ�)"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 0, 80, 20, hWnd, (HMENU)ID_S_SN, g_hInst, NULL);
		if (selwindow!=0 )ShowWindow(hStatic_Sn, SW_HIDE);
		//�ڸ���ȣ ����Ʈ
		hEdit_Sn = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 50, 100, 50, hWnd, (HMENU)ID_E_SN, g_hInst, NULL);
		if (selwindow!=0)ShowWindow(hEdit_Sn, SW_HIDE);
		//�ڸ����� ��ư
		hButton_SelSeat = CreateWindow(TEXT("button"), TEXT("SELECTSEAT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 10, 200, 50, hWnd, (HMENU)ID_B_SELSEAT, g_hInst, NULL);
		if (selwindow!=0)ShowWindow(hButton_SelSeat, SW_HIDE);

		
	
		//Ÿ�̸�(�α��� �� ����)
		SetTimer(hWnd, 1, 1000, NULL);			//�ð� Ÿ�̸�
		SetTimer(hWnd, 2, 1000, NULL);			//�ð� Ÿ�̸� �����ϴ� Ÿ�̸�
		return 0;
	case WM_TIMER:
		switch (wParam) {
		case 0:													//�ڵ����� ȭ���� �ٲٴ� Ÿ�̸�
			//�ڸ�����ȭ��->�α���ȭ��
			if (chkseat == 1) {								//�ڸ����� �� �Ǿ�����
				chkseat = 0;
				selwindow = 1;								//�α���ȭ������ ����
				//�ڸ�����ȭ�� ����
				if (selwindow != 0)ShowWindow(hStatic_Sn, SW_HIDE);
				if (selwindow != 0)ShowWindow(hEdit_Sn, SW_HIDE);
				if (selwindow != 0)ShowWindow(hButton_SelSeat, SW_HIDE);
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
			//�α���ȭ��->���ȭ��
			else if (chklogin == 1) {					//�α��� �� �Ǿ��� ��
				chklogin = 0;
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
			//�����ð� 0�϶� �ֹ� �� �α��� ȭ������/�����ð� ������ �ֹ� �� ���ȭ������
			else if (chkorder == 1) {										//�ֹ� �� �Ǿ��� ��
				chkorder = 0;
				if (selafterorder == 1)selwindow = 1;				//���� �ð� 0�϶� �α���ȭ������ ����
				else selwindow = 4;											//..�ƴҶ� ���ȭ������ ����
				InvalidateRect(hWnd, NULL, TRUE);
				//�ֹ�ȭ�� ����
				if (selwindow != 3)ShowWindow(hStatic_Item, SW_HIDE);
				if (selwindow != 3)ShowWindow(hListBox_ItemList, SW_HIDE);
				if (selwindow != 3)ShowWindow(hStatic_OrderCount, SW_HIDE);
				if (selwindow != 3)ShowWindow(hEdit_OrderCount, SW_HIDE);
				if (selwindow != 3)ShowWindow(hButton_Order, SW_HIDE);
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
			//�����ð� 0�϶� �α���ȭ�鿡�� �ֹ�ȭ������
			else if (chkpctime == 1) {															//�����ð� 0�϶�
				chkpctime = 0;
				selafterorder = 1;																	//�����ð�0�϶��� ����
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
				//�����۸���Ʈ �ʱ�ȭ
				SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"DRINK");
				SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"FOOD");
				SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"PC");
			}
			//���� �� ����ȭ�鿡�� �α���ȭ������
			else if (chkjoin == 1) {										//ȸ������ �� �Ǿ�����
				chkjoin = 0;
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
			//���� �� ����ȭ�鿡�� �α���ȭ������ 
			else if (chkupdate == 1) {										//���� �� �Ǿ�����
				chkupdate = 0;
				selwindow = 1;													//�α���ȭ�� ����
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
			//���ȭ��(�α׾ƿ�/Ż��)/�ֹ�ȭ��/�α���ȭ��(�ڸ��ݳ�)���� �ڸ�����ȭ������ 
		else if (gohome == 1) {																				//�ڸ�����ȭ������ ���� ��Ȳ
			gohome = 0;
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
		}
			// ȭ�� ���� ��û (����� ���� ���)
			break;
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
				gohome = 1;
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
		case 3:																																					//�ڸ��ݳ�Ÿ�̸�
			if (returnsn == 1) {																															//�ڸ��ݳ������϶�
				returnsntime -= 1000000;																											//�ڸ��ݳ��ð� ���̱�
				if (returnsntime <= 0) {																													//���ѽð� ���
					returnsntime = 1000000;																											//�ڸ��ݳ��ð� �ʱ�ȭ
					returnsn = 0;																															//�ڸ��ݳ����� ����
					lstrcpy(tgcmdserver, "8");
					lstrcat(tgcmdserver, "SN:");
					lstrcat(tgcmdserver, tgnum);
					nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);											//������ �ڸ��ݳ� Ŀ��� ������
					MessageBox(hWnd, "���ѽð�10�� �ʰ��� �¼���ȣ�ݳ���", "�¼��ݳ�", MB_OK);													
				}
			}
			break;
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		//�α���ȭ�鿡�� ���� �� ���� ��ư �������� �α��ο��� ���� �� ��������
		case ID_B_GO_JOIN_OR_UPDATE:
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
			break;
			//�α��� ��ư ��������
		case ID_B_LOGIN:
			returnsn = 0;																																				//�ڸ��ݳ� ���� ����
			KillTimer(hWnd, 3);																																		//�ڸ��ݳ� Ÿ�̸� ����
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
			//�߰��� ���: �α��� �� ���̵���� ���� ������
			lstrcpy(tgcmdserver, "2");
			lstrcat(tgcmdserver, tgselitemi);
			GetWindowText(hEdit_OrderCount, tgitemcount, sizeof(tgitemcount));													//�������ֹ����� ���ڿ��� ���
			lstrcat(tgcmdserver, tgitemcount);
			lstrcat(tgcmdserver, "ID:");																														//�Է¾��̵� ���ڿ��� ���
			lstrcat(tgcmdserver, tgId);
			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);															//������ �ֹ� Ŀ�ǵ� ������
			break;

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

			//���ȭ��ּ� �ֹ����� ��ư �������� ���ȭ�鿡�� �ֹ�ȭ������ 
		case ID_B_GO_ORDER:
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
			//�����۸���Ʈ �ʱ�ȭ
			SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"DRINK");
			SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"FOOD");
			SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"PC");
			break;
			//�ڸ�����ȭ�鿡�� �ڸ����ù�ư ��������
		case ID_B_SELSEAT:																																
			lstrcpy(tgcmdserver, "1");
			GetWindowText(hEdit_Sn, tgnum, sizeof(tgnum));																				//�ڸ� ����(02�� ���� 2�ڸ� ��������)
			lstrcat(tgcmdserver, tgnum);
			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);														//������ �ڸ����� Ŀ�ǵ� ������
			break;
		case ID_B_LOGOUT:																																			//�α׾ƿ�(�α��� ������)
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
			break;
		case ID_B_DELETE:																																			//Ż�� ����(�α��� ������)
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
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

