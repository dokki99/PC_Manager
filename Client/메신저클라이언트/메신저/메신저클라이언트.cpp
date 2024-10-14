

//�Ұ�

#include <Windows.h>
#include<stdio.h>
#include<CommCtrl.h>
#include <sql.h>
#include <sqlext.h>
#include "resource.h"
#define ID_E_CHAT 101			//���ν����� ä�� ����Ʈ �ĺ���
#define ID_B_LOGOUT 102	//���ν����� ������ ��ư �ĺ���
#define ID_B_DELETE 103		//���ν����� Ż�� ��ư �ĺ���
#define ID_E_TIMER 104		//���ν����� Ÿ�̸� ����Ʈ �ĺ���
#define ID_E_IP 105				//������ IP ����Ʈ �ĺ���
#define ID_E_PORT 106			//������ PORT ����Ʈ �ĺ���
#define ID_B_CONNECT 107	//������ ���� ��ư �ĺ���
#define ID_E_ID 108				//����/������ ���̵� ����Ʈ �ĺ���
#define ID_E_PW 109			//����/������ ��� ����Ʈ �ĺ���
#define ID_E_NAME 110		//����/������ �̸� ����Ʈ �ĺ���
#define ID_E_PN 111				//����/������ ��ȭ��ȣ ����Ʈ �ĺ���
#define ID_E_ADDR 112			//����/������ �ּ� ����Ʈ �ĺ���
#define ID_E_BIRTH 113		//����/������ ���� ����Ʈ �ĺ���
#define ID_B_JOIN 114			//����/������ ���� ��ư �ĺ���
#define ID_B_UPDATE 115		//����/������ ���� ��ư �ĺ���
#define ID_E_LOGIN_ID 116//�α����� ���̵� ����Ʈ �ĺ���
#define ID_E_LOGIN_PW 117//�α����� ��� ����Ʈ �ĺ���
#define ID_B_LOGIN 118		//�α����� �α��� ��ư �ĺ���
#define ID_E_SN 119				//�ڸ��� �ڸ���ȣ ����Ʈ �ĺ���
#define ID_B_SELSEAT 120	//�ڸ��� �ڸ���ȣ���� ��ư �ĺ���
#define ID_L_ITEM 121			//�ֹ��� �����۸���Ʈ ����Ʈ�ڽ� �ĺ���
#define ID_E_ORDERCOUNT 122	//�ֹ��� �������ֹ����� ����Ʈ �ĺ���
#define ID_B_ORDER 123				//�ֹ��� ������ �ֹ� ��ư �ĺ���
#define ID_S_IP 124						//������ IP ����ƽ �ĺ���
#define ID_S_PORT 125					//������ PORT ����ƽ �ĺ���
#define ID_S_ID 126						//����/������ ���̵� ����ƽ �ĺ���
#define ID_S_PW 127					//����/������ ��� ����ƽ �ĺ���
#define ID_S_NAME 128				//����/������ �̸� ����ƽ �ĺ���
#define ID_S_PN 129						//����/������ ��ȭ��ȣ ����ƽ �ĺ���
#define ID_S_ADDR 130				//����/������ �ּ� ����ƽ �ĺ���
#define ID_S_BIRTH 131				//����/������ ���� ����ƽ �ĺ���
#define ID_S_LOGIN_ID 132		//�α����� ���̵� ����ƽ �ĺ���
#define ID_S_LOGIN_PW 133		//�α����� ��� ����ƽ �ĺ���
#define ID_S_SN 134					//�ڸ��� �ڸ���ȣ ����ƽ �ĺ���
#define ID_S_ITEM 135				//�ֹ��� ������ ����ƽ �ĺ���
#define ID_S_ORDERCOUNT 136	//�ֹ��� �������ֹ����� ����ƽ �ĺ���
#define ID_S_CHAT 137				//���ν����� ä�� ����ƽ �ĺ���
#define ID_S_TIMER 138				//���ν����� Ÿ�̸� ����ƽ �ĺ���
HWND hWndMain;		// ���� ȭ�� �ڵ�
HWND hWnd_C;		// ���� ȭ�� �ڵ�
HWND hWnd_JU;		// ����/���� ȭ�� �ڵ�
HWND hWnd_L;		// �α��� ȭ�� �ڵ�
HWND hWnd_S;		// �ڸ����� ȭ�� �ڵ�
HWND hWnd_O;		// �ֹ� ȭ�� �ڵ�
LPCTSTR C_Class = TEXT("����");					//���� ����
LPCTSTR JU_Class = TEXT("ȸ������/����");//����/���� ����
LPCTSTR L_Class = TEXT("�α���");				//�α��� ����
LPCTSTR S_Class = TEXT("�ڸ�����");			//�ڸ����� ����
LPCTSTR O_Class = TEXT("�ֹ�");					//�ֹ� ����
//�ڵ�
HWND hEdit_Chat;									//���ν����� ä�� ����Ʈ
HWND hButton_Logout;							//���ν����� �α׾ƿ� ��ư
HWND hButton_Delete;							//���ν����� Ż�� ��ư
HWND hEdit_Timer;								//���ν����� Ż�� ����Ʈ
HWND hEdit_Ip;										//������ IP ����Ʈ
HWND hEdit_Port;									//������ PORT ����Ʈ
HWND hButton_Connect;						//������ ���� ��ư
HWND hEdit_Id;										//����/������ ���̵� ����Ʈ
HWND hEdit_Pw;										//����/������ ��� ����Ʈ
HWND hEdit_Name;								//����/������ �̸� ����Ʈ
HWND hEdit_Pn;										//����/������ ��ȭ��ȣ ����Ʈ
HWND hEdit_Addr;									//����/������ �ּ� ����Ʈ
HWND hEdit_Birth;									//����/������ ���� ����Ʈ
HWND hButton_Join;								//����/������ ���� ��ư
HWND hButton_Update;							//����/������ ���� ��ư
HWND hEdit_Login_Id;							//�α����� ���̵� ����Ʈ
HWND hEdit_Login_Pw;							//�α����� ��� ����Ʈ
HWND hButton_Login;								//�α����� �α��� ��ư
HWND hEdit_Sn;										//�ڸ��� �ڸ���ȣ ����Ʈ
HWND hButton_SelSeat;						//�ڸ��� �ڸ���ȣ���� ��ư
HWND hListBox_ItemList;						//�ֹ��� �����۸���Ʈ ����Ʈ�ڽ�
HWND hEdit_OrderCount;						//�ֹ��� �������ֹ����� ����Ʈ
HWND hButton_Order;							//�ֹ��� �ֹ� ��ư
HWND hStatic_Ip;									//������ IP ����ƽ
HWND hStatic_Port;								//������ PORT ����ƽ
HWND hStatic_Id;									//����/������ ���̵� ����ƽ
HWND hStatic_Pw;									//����/������ ��� ����ƽ
HWND hStatic_Name;								//����/������ �̸� ����ƽ
HWND hStatic_Addr;								//����/������ �ּ� ����ƽ
HWND hStatic_Birth;								//����/������ ���� ����ƽ
HWND hStatic_Pn;									//����/������ ��ȭ��ȣ ����ƽ
HWND hStatic_Login_Id;						//�α����� ���̵� ����ƽ
HWND hStatic_Login_Pw;						//�α����� ��� ����ƽ
HWND hStatic_Sn;									//�ڸ��� �ڸ���ȣ ����ƽ
HWND hStatic_Item;								//�ֹ��� ������ ����ƽ
HWND hStatic_OrderCount;					//�ֹ��� �������ֹ����� ����ƽ
HWND hStatic_Chat;								//���ν����� ä�� ����ƽ
HWND hStatic_Timer;							//���ν����� Ÿ�̸� ����ƽ
//���ν����Լ���
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);															//�⺻ȭ��
LRESULT CALLBACK ConnectProc(HWND, UINT, WPARAM, LPARAM);														//����ȭ��
LRESULT CALLBACK Join_Or_UpdateProc(HWND, UINT, WPARAM, LPARAM);										//����/����
LRESULT CALLBACK LoginProc(HWND, UINT, WPARAM, LPARAM);															//�α���
LRESULT CALLBACK SelSeatProc(HWND, UINT, WPARAM, LPARAM);														//�ڸ�����
LRESULT CALLBACK OrderProc(HWND, UINT, WPARAM, LPARAM);														//�ֹ�
//BOOL CALLBACK JoinDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);			
//BOOL CALLBACK LoginDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
//BOOL CALLBACK SelSeatDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
//BOOL CALLBACK OrderDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("PC�� Ŭ���̾�Ʈ");																										//������ ���ν��� Ÿ��Ʋ
////IP�� ��Ʈ��ȣ ����
const char g_szlpAddress[17] = "127.0.0.1";
const unsigned short g_uPort = 7878;
TCHAR tgIp[17] = "127.0.0.1", tgPort[10] = "7878";																									//IP�� PORT��ȣ
TCHAR tgId[10], tgName[10], tgPw[10],tgPn[10], tgAddr[10], tgBirth[10];										//���̵�,�̸�,���,��ȭ��ȣ,�ּ�,���� ���ڿ� ������
//TCHAR tgNick[10],TCHAR tgPass[10];
int nip, nport;																																	//IP�� PORT ���� ����

HWND hChatEdit;																		

int chknickmsg = 0;

BOOL seljoinorloginordel = 0;
TCHAR selnum[10];

TCHAR nickandmsg[100];

DWORD ThreadID;																																					//��������̵�
HANDLE hThread;																																					//������ �ڵ�

HWND hWndDlg;

TCHAR tgmessage[30];
TCHAR tgcmdserver[200];																																		//������ ����ϴ� Ŀ��� ���ڿ�
TCHAR tgjoinloginmsg[100]="ID";
int nReturn;																																								//��� ��� 
WSADATA wsadata;																																				//���� ����
SOCKET clientsock;																																					//���Ϲ�ȣ ���
sockaddr_in addr_client;																																			//���Ϲ�ȣ ���
int addrlen_clt = sizeof(sockaddr);																															//���Ϲ�ȣ ���
char buf[1024];																																						////�޽���																						
char strTemp[1024];																																				//�޼����� ä��â�� ������ �޼����� ��ȯ

//���� //�ֿ亯��
int turn = 0;																																								//������ ������ ������� �������� �ϴ� ���� ����
int ini;
TCHAR info[200];																																					//Ŀ��忡�� ��Ҹ� �̴� ���ڿ�
int infoi = 0;																																								//Ŀ��忡�� ��Ҹ� �̴� ���ڿ��� �ε���
TCHAR usersn[10];																																					//�α��ε� ������ �ڸ���ȣ
TCHAR userpw[10];																																					//�α��ε� ������ ���
int pctimei;																																								//�α��ε� ������ �ð� �ε���
int ngitemcount;																																						//�������ֹ����� 
TCHAR tgselitemi[10];																																				//�������ֹ����� ���ڿ�
int selitemi;																																								//�������ֹ�����
int chkseat=0;																																							//�ڸ����� üũ
int chkjoin = 0;																																							//���� üũ
int chklogin = 0;																																						//�α��� üũ
int i;	
int pctime;																																								//�α��ε� ������ �ð�
int chkexit;																																								//�α׾ƿ� üũ
int userexit;
int chktimer;																																							//Ÿ�̸� ����(1�̸� ����/0�̸� ����)
int pci = 0;																																								
TCHAR ordermsg[20];																																				
TCHAR tgitem[10] = "";																																			//������ ���ڿ�
TCHAR tgitemcount[10] = "";																																	//������ ���ڿ� ���� ���ڿ�
TCHAR tgSeatnum[10];																																			//�ڸ���ȣ ���ڿ�
TCHAR tgnum[10];																																					//��û�ϴ� �ڸ���ȣ ���ڿ�
TCHAR userid[10];																																					//�α��� �� ���� ���̵� ���ڿ�
TCHAR exitmsg[20];																									
TCHAR pcbuf[10];
TCHAR orderbuf[10];																																				//�ֹ�������� ���
TCHAR itembuf[10];
TCHAR pctimebuf[10];																																				//���� �ð� ��ƿ���
TCHAR exitclient[10];
TCHAR tpctime[10];																																					//�����ð� ���ڿ�
int pbi = 0;
int orderi = 0;																																							//�ֹ�������� �ε���
int itemi = 0;																																								//��ǰ������� �ε���
int chkendbtn = 0;																																					//�α׾ƿ�/Ż�� üũ


//DB������
SQLHENV hEnv;
SQLHDBC hDbc;
SQLHSTMT hStmt;


DWORD WINAPI ThreadFunc(LPVOID Param) {//����//���ú꽺����ν� ��û�� ����� �ٷ�
	HDC hdc;
	SOCKET* P = (SOCKET*)Param;
	for (;;) {
		lstrcpy(buf, "");
		nReturn = recv(*P, buf, 1024, 0);
		//MessageBox(hWndDlg, buf, "chkbuf", MB_OK);
		if (nReturn == 0 || nReturn == SOCKET_ERROR) {
			continue;
		}
		else {								
			orderi = 0;
			lstrcpy(orderbuf, "");
			for (int i = 0; i < 3; i++) {
				orderbuf[orderi++] = buf[i];
			}
			orderbuf[orderi] = '\0';												//����//�ֹ��� ���
			if (buf[0] == '1') {
				if (lstrcmp(buf, TEXT("11")) == 0) {
					chkseat = 1;																	//����//�ڸ�
					lstrcpy(tgSeatnum, tgnum);										//�ڸ���ȣ ���
					lstrcpy(usersn, tgSeatnum);										//������ �ڸ���ȣ ���
					wsprintf(buf, "%s", "�ڸ���밡��");
				}
				else wsprintf(buf, "%s", "�ڸ����Ұ�");						//�����κ��� "10" �޾�����
			}
			else if (buf[0] == '3') {
				 if (lstrcmp(buf, TEXT("31")) == 0) {								//����//ȸ������
					chkjoin = 1;
					wsprintf(buf, "%s", "ȸ�����Լ���");
				}
				 else wsprintf(buf, "%s", "ȸ�����Խ���");						//�����κ��� "30" �޾�����
			}
			else if (buf[0] == '4') {
				if (buf[1]=='1') {					//����//�α��εǸ� �����ð�ó���ǰ� ���̵�����			//�α��ν��� 1.���̵�/�������,2.���� �ð� 0����
					pctimei = 0;
					for (int i = 2; i < 4; i++) {
						pctimebuf[pctimei++] = buf[i];	//����//������ ��ȣ�� 1�̾ƴ� 01����
					}
					pctimebuf[pctimei] = '\0';								//����//�����ð� ���
					pctime = atoi(pctimebuf);
					if (pctime >= 0) {
						lstrcpy(userid, tgId);									//����//���� ���̵�,��� ���
						lstrcpy(userpw, tgPw);
						i = 4;																//�α��� Ŀ�ǵ��� ������ ������  ������
						turn = -1;														//������ ������ ������� �������� ����
						while (buf[i] != '\0') { //����//��ü CMD						
							infoi = 0;
							if (buf[i] == ':') {										//�� ������ ���������� �̵�
								ini = i + 1;
								turn++;
							}
							if (turn == 0) {			
								while (buf[ini] != ':' && buf[ini] != '\0') {			//":"������ ���
									info[infoi++] = buf[ini++];
								}
								if (infoi >= 2) infoi -= 2;								//���� ���� Ŀ�ǵ�(PW,NAME..) ���� ��ŭ ����
								info[infoi] = '\0';
								lstrcpy(tgName, info); //����//NAME������
							}
							else if (turn == 1) {
								while (buf[ini] != ':' && buf[ini] != '\0') {
									info[infoi++] = buf[ini++];
								}
								if (infoi >= 4) infoi -= 4;
								info[infoi] = '\0';
								lstrcpy(tgPn, info); //����//PN������
							}
							else if (turn == 2) {
								while (buf[ini] != ':' && buf[ini] != '\0') {
									info[infoi++] = buf[ini++];
								}
								if (infoi >= 5) infoi -= 5;
								info[infoi] = '\0';
								lstrcpy(tgAddr, info); //����//ADDR������
							}
							else if (turn == 3) {
								while (buf[ini] != '\0') {										//���������� ���
									info[infoi++] = buf[ini++];
								}
								info[infoi] = '\0';
								lstrcpy(tgBirth, info); //����//BIRTH������
								turn = 0;
								break;
							}
							i++;
						}

						i = 0;
						//����//����/������ ������ ������ ����
						wsprintf(tpctime, "%d", pctime);
						SetWindowText(hEdit_Timer, tpctime);				//���ν����� Ÿ�̸� ����Ʈ�� ���� �ð� ����
						SetWindowText(hEdit_Id, userid);						//����/������ ���̵� ����
						SetWindowText(hEdit_Pw, userpw);					//..���
						SetWindowText(hEdit_Name, tgName);				//..�̸�
						SetWindowText(hEdit_Pn, tgPn);							//..��ȭ��ȣ
						SetWindowText(hEdit_Addr, tgAddr);				//..�ּ�
						SetWindowText(hEdit_Birth, tgBirth);				//..����
						if (pctime > 0) {												//������ �ð��� 0 �ʰ����� �α��� ����
							chklogin = 1;
							chktimer = 1;
							wsprintf(buf, "%s", "�α��μ���");	//����//�α��ΰ� Ÿ�̸� ���� ����
						}
						else wsprintf(buf, "%s", "�α��ν���(�ܿ��ð�����)");	//������ �ð��� 0�����϶�
					}
				}
				else wsprintf(buf, "%s", "�α��ν���");					//����//cmd�� 40���� ������ ����
			}
			else if (buf[0] == '2') {
				if (buf[1] == '1') {
					if (lstrcmp(orderbuf, TEXT("211")) == 0) {			//����//�����ǰ����ó��
						wsprintf(buf, "%s", "DRINK���Լ���");
					}
					else wsprintf(buf, "%s", "DRINK���Խ���");			//�����κ���  "210" �޾��� ��
				}
				else if (buf[1] == '2') {
					if (lstrcmp(orderbuf, TEXT("221")) == 0) {			//����//��ǰ����ó��
						wsprintf(buf, "%s", "FOOD���Լ���");
					}
					else wsprintf(buf, "%s", "FOOD���Խ���");			//�����κ���  "220" �޾��� ��
				}
				else if (buf[1] == '3') {
					if (lstrcmp(orderbuf, TEXT("231")) == 0) {			//����//�ǽû�ǰ����ó��
						wsprintf(buf, "%s", "PC���Լ���");
					}
					else wsprintf(buf, "%s", "PC���Խ���");				//�����κ���  "230" �޾��� ��
				}
			}
			else if (buf[0] == '5') {														//����//�α���
				if (lstrcmp(buf, TEXT("51")) == 0) {
					lstrcpy(userid, "");
					wsprintf(buf, "%s", "�α׾ƿ�����");
					chklogin = 0;
				}
				else 	wsprintf(buf, "%s", "�α׾ƿ�����");				////�����κ���  "50" �޾��� ��
			}
			else if (buf[0] == '6') {													//����//����
				if (lstrcmp(buf, TEXT("61")) == 0) {
					wsprintf(buf, "%s", "��������");
				}
				else wsprintf(buf, "%s", "��������");						//�����κ���  "60" �޾��� ��
			}
			else if (buf[0] == '7') {													//����//Ż��
				if (lstrcmp(buf, TEXT("71")) == 0) {
					wsprintf(buf, "%s", "Ż�𼺰�");
					SendMessage(hWndDlg, WM_CLOSE, 0, 0);
				}
				else wsprintf(buf, "%s", "Ż�����");						//�����κ���  "70" �޾��� ��
			}
			else {
				MessageBox(hWndMain, buf,"server����", MB_OK);					/////////////////////////////////////////////////////////////////// ������ �׽�Ʈ ����////////////////////////
				// ���ʿ� �������� ������ �����͸� ������� ������ �ִ� �¼��� �����ϰ� �����Ҽ��ְ� ��û		
}
			if (lstrlen(buf) != 0) {//����//��� �޼��� ��Ÿ����
				sprintf_s(strTemp, "������ �޽���:%s", buf);								//����//������ cmd�� ��ȯ�� �޼����� ���ν����� ä�� ����Ʈ�� ����
				int len = GetWindowTextLength(hEdit_Chat);
				SendMessage(hEdit_Chat, EM_SETSEL, (WPARAM)len, (LPARAM)len);
				SendMessage(hEdit_Chat, EM_REPLACESEL, FALSE, (LPARAM)"\r\n");
				SendMessage(hEdit_Chat, EM_REPLACESEL, FALSE, (LPARAM)strTemp);												//���ν����� ä�� ����Ʈ �Ʒ��������� �ۼ� ��Ű��
			}
		}
	}
	return 0;
}
//��ȭ���ڿ�//����//��ȭ���� ��Ʈ�� ����//��� ��û�ٷ��
//BOOL CALLBACK ClientDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
//	switch (iMessage) {
//	case WM_INITDIALOG:
//		//DBConnect();
//		hWndDlg = hDlg;
//		InitCommonControls();
//		SetDlgItemText(hDlg, IDC_IPCLIENTEDIT, tgIp);
//		SetDlgItemText(hDlg, IDC_PORTCLIENTEDIT, tgPort);//����//192.168.0.13
//		//lstrcpy(tpctime, "");
//		SetTimer(hDlg, 1, 1000, NULL);
//		SetTimer(hDlg, 2, 1000, NULL);
//		return TRUE;
//	case WM_TIMER:
//		switch (wParam) {
//		case 1:												//����//���� �ð� Ÿ�̸�
//			if (chktimer == 1) {
//				pctime -= 1;
//				wsprintf(tpctime, "%d", pctime);
//				SetDlgItemText(hDlg, IDC_PCTIMEEDIT, tpctime);
//				if (pctime == 0) {																				//����//�ð� 0�϶� �α׾ƿ�ó��
//					wsprintf(tpctime, "%d", pctime);
//					SetDlgItemText(hDlg, IDC_PCTIMEEDIT, tpctime);
//					lstrcpy(tgcmdserver, "");
//					lstrcpy(tgcmdserver, "5ID:");
//					lstrcat(tgcmdserver, userid);
//					lstrcat(tgcmdserver, "PW:");
//					lstrcat(tgcmdserver, userpw);
//					lstrcat(tgcmdserver, "SN:");
//					lstrcat(tgcmdserver, usersn);
//					chkendbtn = 1;
//					chklogin = 0;
//					nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);
//					chktimer = 0;
//					break;
//				}
//			}
//			break;
//		case 2:																//����//�������ư ��������/�����ð� 0�϶� �����ð� Ÿ�̸� �����ϰ� �ٽ� Ÿ�̸� �����(�α����� ��� �ޱ� ����)
//			if (chkendbtn == 1) {
//				KillTimer(hDlg, 1);
//				SetTimer(hDlg, 1, 1000, NULL);
//				chkendbtn = 0;
//			}
//			break;
//		}
//		return TRUE;
//	case WM_COMMAND:
//		switch (LOWORD(wParam)) {
//		case IDC_ENTERCLIENTBUTTON://����//���Ͽ���
//			GetDlgItemText(hDlg, IDC_IPCLIENTEDIT, tgIp, sizeof(tgIp));
//			GetDlgItemText(hDlg, IDC_PORTCLIENTEDIT, tgPort, sizeof(tgPort));
//			nip = atoi(tgIp);
//			nport = atoi(tgPort);
//			nReturn = WSAStartup(WORD(2.0), &wsadata);
//			clientsock = socket(AF_INET, SOCK_STREAM, 0);
//			addr_client.sin_family = AF_INET;
//			addr_client.sin_addr.s_addr = inet_addr(tgIp);
//			addr_client.sin_port = htons(nport);
//			hThread = CreateThread(NULL, 0, ThreadFunc, &clientsock, 0, &ThreadID);
//			nReturn = connect(clientsock, (sockaddr*)&addr_client, addrlen_clt);
//			if (!nReturn) {
//				hChatEdit = GetDlgItem(hWndDlg, IDC_CHATCLIENTLIST);
//				int len = GetWindowTextLength(hChatEdit);
//				SendDlgItemMessage(hWndDlg, IDC_CHATCLIENTLIST, EM_SETSEL, (WPARAM)len, (LPARAM)len);
//				SendDlgItemMessage(hWndDlg, IDC_CHATCLIENTLIST, EM_REPLACESEL, FALSE, (LPARAM)"\r\n");
//				SendDlgItemMessage(hWndDlg, IDC_CHATCLIENTLIST, EM_REPLACESEL, FALSE, (LPARAM)"�����Ͽ����ϴ�.");
//			}
//			break;
//			return TRUE;
//	
//		case IDC_GOJOINBUTTON://����//ȸ������ �� �ڸ��������
//			if (chkseat == 1) {
//				DialogBox(g_hInst, MAKEINTRESOURCE(IDD_JOINDIALOG), HWND_DESKTOP, (DLGPROC)JoinDlgProc);
//			}
//			else {
//				MessageBox(hDlg, "�ڸ� ���� ���� �ϼ���!!", "����", MB_OK);
//			}
//			return TRUE;
//		case IDC_GOLOGINBUTTON://����//�α��� �� �ڸ��� �α��� üũ�ϱ�
//			if (chkseat == 1 && chklogin==0) {
//				DialogBox(g_hInst, MAKEINTRESOURCE(IDD_LOGINDIALOG), HWND_DESKTOP, (DLGPROC)LoginDlgProc);
//			}
//			else {
//				if(chkseat==0)MessageBox(hDlg, "�ڸ� ���� ���� �ϼ���!!", "����", MB_OK);
//				if(chklogin==1)MessageBox(hDlg, "�α��� �Ǿ� �ֽ��ϴ�!!", "����", MB_OK);
//			}
//			return TRUE;
//		case IDC_GOORDERBUTTON://����//�ֹ� �� �ڸ� ���
//			if (chkseat == 1) {
//				DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ORDERDIALOG), HWND_DESKTOP, (DLGPROC)OrderDlgProc);
//			}
//			else {
//				MessageBox(hDlg, "�ڸ� ���� ���� �ϼ���!!", "����", MB_OK);
//			}
//			return TRUE;
//		case IDC_SEATBUTTON:
//			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_SELSEATDIALOG), HWND_DESKTOP, (DLGPROC)SelSeatDlgProc);
//			return TRUE;
//		case IDC_EXITBUTTON://����//������ �� �α��� üũ�ϱ�
//			if (chklogin == 1) {
//				lstrcpy(tgcmdserver, "");
//				lstrcpy(tgcmdserver, "5ID:");
//				lstrcat(tgcmdserver, userid);
//				lstrcat(tgcmdserver, "PW:");
//				lstrcat(tgcmdserver, userpw);
//				lstrcat(tgcmdserver, "SN:");
//				lstrcat(tgcmdserver, usersn);
//				chkendbtn = 1;
//				nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);
//			}
//			else MessageBox(hDlg, "�α����ϼ���!!", "����", MB_OK);
//			return TRUE;
//		case IDC_DELBUTTON://����//Ż�� �� �α��� üũ�ϱ�
//			if (chklogin == 1) {
//				lstrcpy(tgcmdserver, "");
//				lstrcpy(tgcmdserver, "7ID:");
//				lstrcat(tgcmdserver, userid);
//				lstrcat(tgcmdserver, "PW:");
//				lstrcat(tgcmdserver, userpw);
//				lstrcat(tgcmdserver, "SN:");
//				lstrcat(tgcmdserver, usersn);
//				chkendbtn = 1;
//				nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);
//			}
//			else MessageBox(hDlg, "�α����ϼ���!!", "����", MB_OK);
//			return TRUE;
//		case IDCANCEL:
//			EndDialog(hDlg, IDCANCEL);
//			//KillTimer(hWndDlg, 1);
//			InvalidateRect(hWndDlg, NULL, TRUE);
//			return TRUE;
//		}
//		break;
//	}
//	return FALSE;
//}
//����//������ �α��ΰ� ȸ������ ��û
//BOOL CALLBACK LoginDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
//	switch (iMessage) {
//	case WM_INITDIALOG:
//		//DBConnect();
//		InitCommonControls();
//		return TRUE;
//	case WM_COMMAND:
//		switch (LOWORD(wParam)) {
//		case IDC_LOGINBUTTON:		//����//�α���:���̵�,��� �Է�(�� �Է� ���������� ':'�� �Է� �� �ް� �ϱ�)
//			lstrcpy(tgcmdserver, "");
//			lstrcpy(tgId, "");
//			lstrcpy(tgPass, "");
//			GetDlgItemText(hDlg, IDC_IDLOGINEDIT, tgId, sizeof(tgId));
//			GetDlgItemText(hDlg, IDC_PASSLOGINEDIT, tgPass, sizeof(tgPass));
//			lstrcpy(tgcmdserver, "4ID:");
//			lstrcat(tgcmdserver, tgId);
//			lstrcat(tgcmdserver, "PW:");
//			lstrcat(tgcmdserver, tgPass);
//			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);
//			break;
//			return TRUE;
//		case IDCANCEL:
//			EndDialog(hDlg, IDCANCEL);
//			InvalidateRect(hWndDlg, NULL, TRUE);
//			return TRUE;
//		}
//		break;
//	}
//	return FALSE;
//}
////����//������ �α��ΰ� ȸ������ ��û
//BOOL CALLBACK JoinDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
//	switch (iMessage) {
//	case WM_INITDIALOG:
//		//DBConnect();
//		InitCommonControls();
//		SetDlgItemText(hWndDlg, IDC_PCTIMEEDIT, tpctime);		//����//�α����Ҷ� �ش� ���� ����
//		SetDlgItemText(hWndDlg, IDC_IDJOINEDIT, userid);
//		SetDlgItemText(hWndDlg, IDC_PASSJOINEDIT, userpw);
//		SetDlgItemText(hWndDlg, IDC_NICKEDIT, tgNick);
//		SetDlgItemText(hWndDlg, IDC_PNEDIT, tgPn);
//		SetDlgItemText(hWndDlg, IDC_ADDREDIT, tgAddr);
//		SetDlgItemText(hWndDlg, IDC_BIRTHEDIT, tgBirth);
//		return TRUE;
//	case WM_COMMAND:
//		switch (LOWORD(wParam)) {
//		case IDC_JOINBUTTON:		//����//ȸ������:���̵�,�г���,���,��ȭ��ȣ,�ּ�,���� �� �Է�/(�� �Է� ���������� ':'�� �Է� �� �ް� �ϱ�)
//			lstrcpy(tgcmdserver, "");
//			lstrcpy(tgId, "");
//			lstrcpy(tgNick, "");
//			lstrcpy(tgPass, "");
//			lstrcpy(tgPn, "");
//			lstrcpy(tgAddr, "");
//			lstrcpy(tgBirth, "");
//			GetDlgItemText(hDlg, IDC_IDJOINEDIT, tgId, sizeof(tgId));
//			GetDlgItemText(hDlg, IDC_NICKEDIT, tgNick, sizeof(tgNick));
//			GetDlgItemText(hDlg, IDC_PASSJOINEDIT, tgPass, sizeof(tgPass));
//			GetDlgItemText(hDlg, IDC_PNEDIT, tgPn, sizeof(tgPn));
//			GetDlgItemText(hDlg, IDC_ADDREDIT, tgAddr, sizeof(tgAddr));
//			GetDlgItemText(hDlg, IDC_BIRTHEDIT, tgBirth, sizeof(tgBirth));
//			lstrcpy(tgcmdserver, "3ID:");
//			lstrcat(tgcmdserver, tgId);
//			lstrcat(tgcmdserver, "PW:");
//			lstrcat(tgcmdserver, tgPass);
//			lstrcat(tgcmdserver, "NICK:");
//			lstrcat(tgcmdserver, tgNick);
//			lstrcat(tgcmdserver, "PN:");
//			lstrcat(tgcmdserver, tgPn);
//			lstrcat(tgcmdserver, "ADDR:");
//			lstrcat(tgcmdserver, tgAddr);
//			lstrcat(tgcmdserver, "BIRTH:");
//			lstrcat(tgcmdserver, tgBirth);
//			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);
//			break;
//			return TRUE;
//		case IDC_UPDATEBUTTON:		//����//����:���̵�,�г���,���,��ȭ��ȣ,�ּ�,���� �� �Է�/(�� �Է� ���������� ':'�� �Է� �� �ް� �ϱ�)(�α��� ������ �����ϰ� �ϱ�(���ã�� ���))
//			lstrcpy(tgcmdserver, "");
//			lstrcpy(tgId, "");
//			lstrcpy(tgNick, "");
//			lstrcpy(tgPass, "");
//			lstrcpy(tgPn, "");
//			lstrcpy(tgAddr, "");
//			lstrcpy(tgBirth, "");
//			GetDlgItemText(hDlg, IDC_IDJOINEDIT, tgId, sizeof(tgId));
//			GetDlgItemText(hDlg, IDC_NICKEDIT, tgNick, sizeof(tgNick));
//			GetDlgItemText(hDlg, IDC_PASSJOINEDIT, tgPass, sizeof(tgPass));
//			GetDlgItemText(hDlg, IDC_PNEDIT, tgPn, sizeof(tgPn));
//			GetDlgItemText(hDlg, IDC_ADDREDIT, tgAddr, sizeof(tgAddr));
//			GetDlgItemText(hDlg, IDC_BIRTHEDIT, tgBirth, sizeof(tgBirth));
//			lstrcpy(tgcmdserver, "6ID:");
//			lstrcat(tgcmdserver, tgId);
//			lstrcat(tgcmdserver, "PW:");
//			lstrcat(tgcmdserver, tgPass);
//			lstrcat(tgcmdserver, "NICK:");
//			lstrcat(tgcmdserver, tgNick);
//			lstrcat(tgcmdserver, "PN:");
//			lstrcat(tgcmdserver, tgPn);
//			lstrcat(tgcmdserver, "ADDR:");
//			lstrcat(tgcmdserver, tgAddr);
//			lstrcat(tgcmdserver, "BIRTH:");
//			lstrcat(tgcmdserver, tgBirth);
//			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);
//			break;
//			return TRUE;
//		case IDCANCEL:
//			EndDialog(hDlg, IDCANCEL);
//			InvalidateRect(hWndDlg, NULL, TRUE);
//			return TRUE;
//		}
//		break;
//	}
//	return FALSE;
//}
////����//�ڸ� ����
//BOOL CALLBACK SelSeatDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
//	switch (iMessage) {
//	case WM_INITDIALOG:
//		//DBConnect();
//		InitCommonControls();
//		return TRUE;
//	case WM_COMMAND:
//		switch (LOWORD(wParam)) {
//		case IDOK:
//			lstrcpy(tgcmdserver, "1");
//			GetDlgItemText(hDlg, IDC_SEATEDIT, tgnum, sizeof(tgnum));						//����//�ڸ� ����(02�� 2�ڸ� ��������)
//			lstrcat(tgcmdserver, tgnum);
//			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);
//			break;
//		case IDCANCEL:
//			EndDialog(hDlg, IDCANCEL);
//			InvalidateRect(hWndDlg, NULL, TRUE);
//			return TRUE;
//		}
//		break;
//	}
//	return FALSE;
//}
////����//�ֹ�
//BOOL CALLBACK OrderDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
//	switch (iMessage) {
//	case WM_INITDIALOG:
//		//DBConnect();
//		InitCommonControls();
//		SendDlgItemMessage(hDlg, IDC_ITEMLIST, LB_ADDSTRING, 0, (LPARAM)"DRINK");
//		SendDlgItemMessage(hDlg, IDC_ITEMLIST, LB_ADDSTRING, 0, (LPARAM)"FOOD");
//		SendDlgItemMessage(hDlg, IDC_ITEMLIST, LB_ADDSTRING, 0, (LPARAM)"PC");
//		return TRUE;
//	case WM_COMMAND:
//		switch (LOWORD(wParam)) {
//		case IDC_ITEMLIST:
//			switch (HIWORD(wParam)) {
//			case LBN_SELCHANGE:
//				selitemi = SendDlgItemMessage(hDlg, IDC_ITEMLIST, LB_GETCURSEL, 0, 0);				//����//�ֹ� ��ǰ ����(����Ʈ�ڽ��� �ε����� 1�ڸ� ��������)
//				selitemi += 1;
//				wsprintf(tgselitemi, "%d", selitemi);
//				//SendDlgItemMessage(hDlg, IDC_ITEMLIST, LB_GETTEXT, i, (LPARAM)tgitem);
//			}
//			break;
//			return TRUE;
//		case IDOK:																							//����//�ֹ� ���� ����(02�� 2�ڸ� ��������)
//			lstrcpy(tgcmdserver, "2");
//			lstrcat(tgcmdserver, tgselitemi);
//			GetDlgItemText(hDlg, IDC_COUNTEDIT, tgitemcount, sizeof(tgitemcount));
//			lstrcat(tgcmdserver, tgitemcount);
//			lstrcat(tgcmdserver, "SN:");
//			lstrcat(tgcmdserver, tgSeatnum);
//			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);
//			break;
//			return TRUE;
//		case IDCANCEL:
//			EndDialog(hDlg, IDCANCEL);
//			InvalidateRect(hWndDlg, NULL, TRUE);
//			return TRUE;
//		}
//		break;
//	}
//	return FALSE;
//}


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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);					//������ ���ν����� �޴� �ޱ�
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

void Init_Wnd(WNDCLASS* Wnd, int Proc_No) {													//���������ν������� ��� �ٸ� ���ν��� ����
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
		Wnd->lpfnWndProc = ConnectProc;																	//����ȭ��
		Wnd->lpszClassName = C_Class;
		break;
	case 1:																													//����/����ȭ��
		Wnd->lpfnWndProc = Join_Or_UpdateProc;
		Wnd->lpszClassName = JU_Class;
		break;
	case 2:																													//�α���ȭ��
		Wnd->lpfnWndProc = LoginProc;
		Wnd->lpszClassName = L_Class;
		break;
	case 3:																													//�ڸ�����ȭ��
		Wnd->lpfnWndProc = SelSeatProc;
		Wnd->lpszClassName = S_Class;
		break;
	case 4:																													//�ֹ�ȭ��
		Wnd->lpfnWndProc = OrderProc;
		Wnd->lpszClassName = O_Class;
		break;
	}
	Wnd->lpszMenuName = NULL;																				//��� �ٸ� ���ν����� �޴��� ����
	Wnd->style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(Wnd);
}
void CLIENT_Open() {																								//���� ���� �Լ�
	GetWindowText(hEdit_Ip, tgIp, sizeof(tgIp));												//������ IP����Ʈ���� ���� ��������
	GetWindowText(hEdit_Port, tgPort, sizeof(tgPort));										//������ PORT����Ʈ���� ���� ��������
	nip = atoi(tgIp);
	nport = atoi(tgPort);
	nReturn = WSAStartup(WORD(2.0), &wsadata);
	clientsock = socket(AF_INET, SOCK_STREAM, 0);
	addr_client.sin_family = AF_INET;
	addr_client.sin_addr.s_addr = inet_addr(tgIp);
	addr_client.sin_port = htons(nport);
	hThread = CreateThread(NULL, 0, ThreadFunc, &clientsock, 0, &ThreadID);						//���ὺ���� �����
	nReturn = connect(clientsock, (sockaddr*)&addr_client, addrlen_clt);
	if (!nReturn) {
		int len = GetWindowTextLength(hEdit_Chat);
		SendMessage(hEdit_Chat, EM_SETSEL, (WPARAM)len, (LPARAM)len);
		SendMessage(hEdit_Chat, EM_REPLACESEL, FALSE, (LPARAM)TEXT("\r\n"));
		SendMessage(hEdit_Chat, EM_REPLACESEL, FALSE, (LPARAM)TEXT("�����Ͽ����ϴ�."));						//���������� ���ν����� ä�� ����Ʈ�� "����" �޼��� ����
	}
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		InitCommonControls();

		//��� �ٸ� ȭ���
		WNDCLASS Wnd_C;				//����
		WNDCLASS Wnd_JU;				//����/����
		WNDCLASS Wnd_L;				//�α���
		WNDCLASS Wnd_S;				//�ڸ�����
		WNDCLASS Wnd_O;				//�ֹ�

		// ���������� ����
		Init_Wnd(&Wnd_C, 0);
		// ����/���� ������ ����
		Init_Wnd(&Wnd_JU, 1);
		// �α��������� ����
		Init_Wnd(&Wnd_L, 2);
		// �ڸ������� ����
		Init_Wnd(&Wnd_S, 3);
		// �ֹ������� ����
		Init_Wnd(&Wnd_O, 4);

		//ä�� ����Ʈ
		hEdit_Chat = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL| ES_MULTILINE,			
			10, 100, 500, 400, hWnd, (HMENU)ID_E_CHAT, g_hInst, NULL);									
		//�α׾ƿ� ��ư
		hButton_Logout = CreateWindow(TEXT("button"), TEXT("LOGOUT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			610, 50, 200, 65, hWnd, (HMENU)ID_B_LOGOUT, g_hInst, NULL);
		//Ż�� ��ư
		hButton_Delete = CreateWindow(TEXT("button"), TEXT("DELETE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 50, 200, 65, hWnd, (HMENU)ID_B_DELETE, g_hInst, NULL);
		//Ÿ�̸� ����Ʈ
		hEdit_Timer = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			610, 200, 100, 100, hWnd, (HMENU)ID_E_TIMER, g_hInst, NULL);
		//ä�� ����ƽ
		hStatic_Chat = CreateWindow(TEXT("static"), TEXT("STATE"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 0, 60, 20, hWnd, (HMENU)ID_S_CHAT, g_hInst, NULL);
		//Ÿ�̸� ����ƽ
		hStatic_Timer = CreateWindow(TEXT("static"), TEXT("TIME"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			610, 150, 40, 20, hWnd, (HMENU)ID_S_TIMER, g_hInst, NULL);
		//Ÿ�̸�
		SetTimer(hWnd, 1, 1000, NULL);			//�α��� ���� �ð� Ÿ�̸�
		SetTimer(hWnd, 2, 1000, NULL);			//�α��� ���� �ð� Ÿ�̸� �����ϴ� Ÿ�̸�
		return 0;
	case WM_TIMER:
		switch (wParam) {
		case 1:												//����//���� �ð� Ÿ�̸�
			if (chktimer == 1) {
				pctime -= 1;										//�����ð� ���̱�
				wsprintf(tpctime, "%d", pctime);
				SetWindowText(hEdit_Timer, tpctime);															//���ν����� Ÿ�̸� ����Ʈ�� ���� ���� �ð� ���
				if (pctime == 0) {																				//����//�ð� 0�϶� �α׾ƿ�ó��
					wsprintf(tpctime, "%d", pctime);
					SetWindowText(hEdit_Timer, tpctime);
					lstrcpy(tgcmdserver, "");
					lstrcpy(tgcmdserver, "5ID:");
					lstrcat(tgcmdserver, userid);
					lstrcat(tgcmdserver, "PW:");
					lstrcat(tgcmdserver, userpw);
					lstrcat(tgcmdserver, "SN:");
					lstrcat(tgcmdserver, usersn);
					chkendbtn = 1;																				//�α׾ƿ�/Ż�� ó���� 
					chklogin = 0;																					//�α׾ƿ� �� ���·�
					nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);
					chktimer = 0;																				//Ÿ�̸� ���� ���·�
					break;
				}
			}
			break;
		case 2:																//����//�������ư ��������/�����ð� 0�϶� �����ð� Ÿ�̸� �����ϰ� �ٽ� Ÿ�̸� �����(�α����� ��� �ޱ� ����)
			if (chkendbtn == 1) {
				KillTimer(hWnd, 1);											//Ÿ�̸� ����
				chktimer = 0;													//Ÿ�̸� ���� ���·�
				SetTimer(hWnd, 1, 1000, NULL);					//���� ���� �ð� Ÿ�̸� �ٽ� ����
				chkendbtn = 0;												//�α׾ƿ� üũ �ٽ� ���� ��� �ʱ�ȭ
				pctime = 0;
				wsprintf(tpctime, "%d", pctime);
				SetWindowText(hEdit_Timer, tpctime); //���ν����� Ÿ�̸� ����Ʈ�� ���� ���� �ð� 0����  ���
			}
			break;
		}
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case ID_M_CONNECT:									//���� ����
			hWnd_C = CreateWindow(C_Class, C_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
			ShowWindow(hWnd_C, SW_SHOW);
			break;
		case ID_M_JOIN_OR_UPDATE:		// ����/���� ����
			if (chkseat == 1) {							//�ڸ� ���� ����������
				hWnd_JU = CreateWindow(JU_Class, JU_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
				ShowWindow(hWnd_JU, SW_SHOW);
			}
			else MessageBox(hWnd, "�ڸ� ���� ���� �ϼ���!!", "����", MB_OK);
			break;
		case ID_M_LOGIN:		// �α��� ����
			if (chkseat == 1 && chklogin == 0) {//�ڸ����ð� �α��� ����������
				hWnd_L = CreateWindow(L_Class, L_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
				ShowWindow(hWnd_L, SW_SHOW);
			}
			else {
				if (chkseat == 0)MessageBox(hWnd, "�ڸ� ���� ���� �ϼ���!!", "����", MB_OK);
				if (chklogin == 1)MessageBox(hWnd, "�α��� �Ǿ� �ֽ��ϴ�!!", "����", MB_OK);
			}
			break;
		case ID_M_SELSEAT:		// �ڸ����� ����
			hWnd_S = CreateWindow(S_Class, S_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
			ShowWindow(hWnd_S, SW_SHOW);
			break;
		case ID_M_ORDER:		// �ֹ� ����
			if (chkseat == 1) {			//�ڸ�����������
				hWnd_O = CreateWindow(O_Class, O_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
				ShowWindow(hWnd_O, SW_SHOW);
			}
			else MessageBox(hWnd, "�ڸ� ���� ���� �ϼ���!!", "����", MB_OK);
			break;
		case ID_B_LOGOUT:							//�α׾ƿ�
			if (chklogin == 1) {							//�α��� ������
				lstrcpy(tgcmdserver, "");
				lstrcpy(tgcmdserver, "5ID:");
				lstrcat(tgcmdserver, userid);
				lstrcat(tgcmdserver, "PW:");
				lstrcat(tgcmdserver, userpw);
				lstrcat(tgcmdserver, "SN:");
				lstrcat(tgcmdserver, usersn);
				chkendbtn = 1;											//�α׾ƿ�/Ż�� ���� ������
				nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);					//������ �α׾ƿ� Ŀ��� ������
				pctime = 0;										
				wsprintf(tpctime, "%d", pctime);
				SetWindowText(hEdit_Timer, tpctime);																	//���ν����� Ÿ�̸� ����Ʈ�� ���� ���� �ð� 0���� ���
			}
			else MessageBox(hWnd, "�α����ϼ���!!", "����", MB_OK);
			break;
		case ID_B_DELETE:																			//Ż�� ����
			if (chklogin == 1) {													//�α��� ������
				lstrcpy(tgcmdserver, "");
				lstrcpy(tgcmdserver, "7ID:");
				lstrcat(tgcmdserver, userid);
				lstrcat(tgcmdserver, "PW:");
				lstrcat(tgcmdserver, userpw);
				lstrcat(tgcmdserver, "SN:");
				lstrcat(tgcmdserver, usersn);
				chkendbtn = 1;											//�α׾ƿ�/Ż�� ���� ������
				nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);						//������ Ż�� Ŀ�ǵ� ������
				pctime = 0;
				wsprintf(tpctime, "%d", pctime);
				SetWindowText(hEdit_Timer, tpctime);												//���ν����� Ÿ�̸� ����Ʈ�� ���� ���� �ð� 0���� ���
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
LRESULT CALLBACK ConnectProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {					//����ȭ��
	switch (iMessage) {
	case WM_CREATE:
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
			310, 200, 400, 100, hWnd, (HMENU)ID_B_CONNECT,g_hInst, NULL);
		//������ IP�� PORT����Ʈ�� ����
		SetWindowText(hEdit_Ip, tgIp);
		SetWindowText(hEdit_Port, tgPort);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_B_CONNECT:					//������ ���� ��ư �������� ���� ����
			CLIENT_Open();
			break;
		}
		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
LRESULT CALLBACK Join_Or_UpdateProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {									//����/����
	switch (iMessage) {
	case WM_CREATE:
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
		//�α��� �� ���� ���� ����
		SetWindowText(hEdit_Id, userid);						//���̵�
		SetWindowText(hEdit_Pw, userpw);					//���
		SetWindowText(hEdit_Name, tgName);				//�̸�
		SetWindowText(hEdit_Pn, tgPn);							//��ȭ��ȣ
		SetWindowText(hEdit_Addr, tgAddr);				//�ּ�
		SetWindowText(hEdit_Birth, tgBirth);				//����
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_B_JOIN:											//���� ��ư ��������
			//Ŀ�ǵ�,���� ���� ���ڿ� ��ĭ���� �ʱ�ȭ
			lstrcpy(tgcmdserver, "");			//�������� Ŀ�ǵ�
			lstrcpy(tgId, "");						//���̵�
			lstrcpy(tgName, "");					//�̸�
			lstrcpy(tgPw, "");						//���
			lstrcpy(tgPn, "");						//��ȭ��ȣ
			lstrcpy(tgAddr, "");					//�ּ�
			lstrcpy(tgBirth, "");					//����
			//���� ���� ����Ʈ���� ���� ���ڿ��� ���
			GetWindowText(hEdit_Id, tgId, sizeof(tgId));					//���̵�
			GetWindowText(hEdit_Name,tgName, sizeof(tgName));		//�̸�
			GetWindowText(hEdit_Pw,  tgPw, sizeof(tgPw));					//���
			GetWindowText(hEdit_Pn,  tgPn, sizeof(tgPn));					//��ȭ��ȣ
			GetWindowText(hEdit_Addr, tgAddr, sizeof(tgAddr));		//�ּ�
			GetWindowText(hEdit_Birth, tgBirth, sizeof(tgBirth));		//����
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
			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);								//������ ���� Ŀ�ǵ� ������
			break;
		case ID_B_UPDATE:																				//���� ��ư ��������
			//Ŀ�ǵ�,���� ���� ���ڿ� ��ĭ���� �ʱ�ȭ
			lstrcpy(tgcmdserver, "");					//Ŀ�ǵ�
			lstrcpy(tgId, "");								//���̵�
			lstrcpy(tgName, "");							//�̸�
			lstrcpy(tgPw, "");								//���
			lstrcpy(tgPn, "");								//��ȭ��ȣ
			lstrcpy(tgAddr, "");							//�ּ�
			lstrcpy(tgBirth, "");							//����
			//���� ���� ����Ʈ���� ���� ���ڿ��� ���
			GetWindowText(hEdit_Id, tgId, sizeof(tgId));								//���̵�
			GetWindowText(hEdit_Name, tgName, sizeof(tgName));				//�̸�
			GetWindowText(hEdit_Pw, tgPw, sizeof(tgPw));								//���
			GetWindowText(hEdit_Pn, tgPn, sizeof(tgPn));								//��ȭ��ȣ
			GetWindowText(hEdit_Addr, tgAddr, sizeof(tgAddr));					//�ּ�
			GetWindowText(hEdit_Birth, tgBirth, sizeof(tgBirth));					//����
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
			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);												//������ ���� Ŀ�ǵ� ������
			break;
		}
		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
LRESULT CALLBACK LoginProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case WM_CREATE:
		/*SetTimer(hWnd, 1, 1000, NULL);
		SetTimer(hWnd, 2, 1000, NULL);*/
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
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_B_LOGIN:									//�α��� ��ư ��������
			//Ŀ�ǵ�� ���� ���ڿ� ��ĭ���� �ʱ�ȭ
			lstrcpy(tgcmdserver, "");				//Ŀ�ǵ�
			lstrcpy(tgId, "");							//���̵�
			lstrcpy(tgPw, "");							//���
			//�α��� ���� ����Ʈ���� ���� ���ڿ� ���
			GetWindowText(hEdit_Login_Id, tgId, sizeof(tgId));		//�α��� ���̵�
			GetWindowText(hEdit_Login_Pw, tgPw, sizeof(tgPw));	//�α��� ���
			lstrcpy(tgcmdserver, "4ID:");
			lstrcat(tgcmdserver, tgId);
			lstrcat(tgcmdserver, "PW:");
			lstrcat(tgcmdserver, tgPw);
			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);							//������ �α��� Ŀ�ǵ� ������
			break;
		}
		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
LRESULT CALLBACK SelSeatProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {						//�ڸ�����
	switch (iMessage) {
	case WM_CREATE:
		nReturn = send(clientsock, "S-", sizeof("S-"), 0);			///////////////////////////////////// ������ �׽�Ʈ//////////////////////////////////////
		//�ڸ���ȣ ����ƽ
		hStatic_Sn = CreateWindow(TEXT("static"), TEXT("SN(2�ڸ�)"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 0, 80, 20, hWnd, (HMENU)ID_S_SN, g_hInst, NULL);
		//�ڸ���ȣ ����Ʈ
		hEdit_Sn = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 50, 100, 50, hWnd, (HMENU)ID_E_SN, g_hInst, NULL);
		//�ڸ����� ��ư
		hButton_SelSeat = CreateWindow(TEXT("button"), TEXT("SELECTSEAT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 10, 200, 50, hWnd, (HMENU)ID_B_SELSEAT, g_hInst, NULL);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_B_SELSEAT:					//�ڸ����� ��������
			lstrcpy(tgcmdserver, "1");
			GetWindowText(hEdit_Sn,tgnum, sizeof(tgnum));						//����//�ڸ� ����(02�� 2�ڸ� ��������)
			lstrcat(tgcmdserver, tgnum);
			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);				//������ �ڸ����� Ŀ�ǵ� ������
			break;
		}
		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
LRESULT CALLBACK OrderProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {															//�ֹ�
	switch (iMessage) {
	case WM_CREATE:
		//������ ����ƽ
		hStatic_Item = CreateWindow(TEXT("static"), TEXT("ITEM"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 0, 40, 20, hWnd, (HMENU)ID_S_ITEM, g_hInst, NULL);
		//�����۸���Ʈ ����Ʈ�ڽ�
		hListBox_ItemList = CreateWindow(TEXT("listbox"), NULL,WS_CHILD | WS_VISIBLE | WS_BORDER| WS_VSCROLL| LBS_SORT | LBS_NOTIFY,
			10, 100, 400, 200,hWnd, (HMENU)ID_L_ITEM,g_hInst, NULL);
		//�������ֹ����� ����ƽ
		hStatic_OrderCount = CreateWindow(TEXT("static"), TEXT("Count(2�ڸ�)"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			510, 0, 100, 20, hWnd, (HMENU)ID_S_ORDERCOUNT, g_hInst, NULL);
		//�������ֹ����� ����Ʈ
		hEdit_OrderCount = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			510, 50, 100, 50, hWnd, (HMENU)ID_E_ORDERCOUNT, g_hInst, NULL);
		//�ֹ� ��ư
		hButton_Order = CreateWindow(TEXT("button"), TEXT("ORDER"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 10, 200, 50, hWnd, (HMENU)ID_B_ORDER, g_hInst, NULL);
		//�����۸���Ʈ �ʱ�ȭ
		SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"DRINK");
		SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"FOOD");
		SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"PC");
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_L_ITEM:							//�����۸���Ʈ ��Ƽ��
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				selitemi = SendMessage(hListBox_ItemList, LB_GETCURSEL, 0, 0);				//����//�ֹ� ��ǰ ����(����Ʈ�ڽ��� �ε����� 1�ڸ� ��������)
				selitemi += 1;
				wsprintf(tgselitemi, "%d", selitemi);																			//������ �ε��� ���
				//SendDlgItemMessage(hDlg, IDC_ITEMLIST, LB_GETTEXT, i, (LPARAM)tgitem);
			}
			break;
		case ID_B_ORDER:																			//�ֹ� ��������
			lstrcpy(tgcmdserver, "2");
			lstrcat(tgcmdserver, tgselitemi);
			GetWindowText(hEdit_OrderCount, tgitemcount, sizeof(tgitemcount));					//�������ֹ����� ���ڿ��� ���
			lstrcat(tgcmdserver, tgitemcount);
			lstrcat(tgcmdserver, "SN:");
			lstrcat(tgcmdserver, tgSeatnum);
			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);							//������ �ֹ� Ŀ�ǵ� ������
			break;
		}
		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

//int APIENTRY WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, LPSTR lpszCmdParam, int nCmdShow)
//{
//	g_hinst = hinstance;
//	DialogBox(g_hinst, MAKEINTRESOURCE(IDD_CLIENTDIALOG), HWND_DESKTOP, (DLGPROC)ClientDlgProc);
//	return 0;
//}
