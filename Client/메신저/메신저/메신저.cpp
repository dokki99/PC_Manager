
#include <Windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<CommCtrl.h>
#include <sql.h>
#include <sqlext.h>
#include "resource.h"
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int isEmpty();
HINSTANCE g_hinst;
LPCTSTR lpszClass = TEXT("��ȭ����");


HWND hChatEdit;														//ä�� ����Ʈ 

int nonexistsock;															//�߸��� ���Ϲ�ȣ

////������� �������� ���� ����
int nReturn;
WSADATA wsadata;
SOCKET listensock;																
SOCKET clientsock = 0;														//Ŭ���̾�Ʈ ����

DWORD ThreadID;																//��������̵�
HANDLE hThread;																//���� �������ڵ�
HANDLE hClientThread[10];												//���ú꽺���� �ڵ�
DWORD ClientThreadID;														//���ú꽺���� ���̵�

SOCKET clientsockar[10];													//���ú꽺������Ϲ迭

////������ ���� ���� �÷���
BOOL g_bThreadExitFlag = FALSE;
volatile BOOL g_bClientThreadExitFlags[10] = { FALSE };

int csi = 0;																			//���ú� �����尡 ��Ƽ�̹Ƿ� ���� �ĺ����� �ε���
int i, j;
////���� ��ư ���ȴ��� �Ǻ�
BOOL serverpower = 0;
BOOL serveropt = 0;
////��Ʈ�� IP��ȣ ����
const char g_szlpAddress[17] = "127.0.0.1";
const unsigned short g_uPort = 7878;
TCHAR tgIp[10] = "127.0.0.1", tgPort[10] = "7878";
int nip, nport;

unsigned long argp;
sockaddr_in addr_server;
sockaddr_in addr_client;
int addrlen_srv = sizeof(sockaddr);
int addrlen_clt = sizeof(sockaddr);
in_addr in;
hostent* ht;
////�޼��� ���� ����
const int buflen = 40960;						//���� �� ����
////�޼���
char buf[buflen];									//�޼���

char strTemp[buflen];							//Ŀ��带 �ٲ㼭 ä��â�� ����

HWND hWndDlg;									//��ȭ���� �ڵ�



///������
//�α��� ���� ������
TCHAR usertime[100];							//���� �ð�
TCHAR username[100];						//�̸�					
TCHAR userpn[100];								//��ȭ��ȣ
TCHAR useraddr[100];							//�ּ�
TCHAR userbirth[100];						//����
//
//Ŀ���
TCHAR seatmsg[200];							//�ڸ�����
TCHAR ordermsg[200];						//�ֹ�
TCHAR joinmsg[200];							//����
TCHAR loginmsg[200];							//�α���
TCHAR logoutmsg[200];						//�α׾ƿ�
TCHAR updatemsg[200];						//����
TCHAR deletemsg[200];						//����
//
//Ŀ��忡�� ���� ���� �����ϴ� ���ڿ�
TCHAR tgSn[20];									//�ڸ���ȣ
TCHAR tgId[50];									//���̵�
TCHAR tgPw[50];									//���
TCHAR tgName[50];							//�̸�
TCHAR tgPn[50];									//��ȭ��ȣ
TCHAR tgAddr[50];								//�ּ�
TCHAR tgBirth[50];								//����
TCHAR info[200];								//�� ����
//
//Ŀ�ǵ尡 ���� ������ ������ ������ ���� ������
int turn;												//�� ������ ���� ����
int infoi;												//������ �ε���
int ini;													//����(�޼���)�� �ε���
//
//Ŀ��尡 �ֹ��϶��� �ֹ���ǰ ������
TCHAR itemn[10];								//�ε��� 
TCHAR itemcount[10];							//����
int ici;													//����(�޼���)�� �ε���
int itemni;												//�ε����� �ε���
int itemcountn;										//����(����)
//
//Ŀ��尡 �ڸ������϶� �ڸ���ȣ ������
TCHAR seatnum[10];							//���ۿ��� �ڸ���ȣ ������
int seati = 0;											//�ε���
int seatn;												//�ڸ���ȣ(����)


//�߿�!!!!!!!!!!! Ŭ���̾�Ʈ�� Ŀ�ǵ� �ְ� �޴� ����̹Ƿ� �����Ͽ� �״�� �̿��ϱ� (�ǽù��ֹ��ý���.txt �ʼ� ����)
//DWORD WINAPI ClientThreadFunc(LPVOID):���Ž�����(��û �Ǻ��ϰ� ó�����ֱ�)
DWORD WINAPI ClientThreadFunc(LPVOID Param) {						
	int i;
	int csi = (int)Param;																																	//���� ���ú꽺���� �ĺ� ���� �ε���
	SOCKET clientsock = clientsockar[csi];																									//���� ���Ϲ�ȣ ���
	while (!g_bThreadExitFlag && !g_bClientThreadExitFlags[csi]) {																//�����ϋ�
		if (clientsock == nonexistsock) break;																									//���� �̻��ϸ� ������ ����
		lstrcpy(buf, "");
		nReturn = recv(clientsock, buf, buflen, 0);
	
		//�ڸ���û(�ڸ���ȣ ������ �Ǻ��ϱ� (����:11/�Ұ�:10))
		if (buf[0] == '1') {							
			for (i = 1; i < 3; i++) {
				seatnum[seati++] = buf[i];
			}
			seatnum[seati] = '\0';																														//�ڸ���ȣ ���
			seatn = atoi(seatnum);
			seati = 0;
			//�¼���ȣüũ
			lstrcpy(seatmsg, "11");
			//lstrcpy(seatmsg, "10");
			nReturn = send(clientsock,seatmsg, sizeof(seatmsg), 0);																	//Ŭ���̾�Ʈ�� �ڸ���û ��� ������
		}
		//�ֹ���û(��ǰ,����,�ڸ� ������ �Ǻ��ϱ� (����:2��ǰ��ȣ(1,2,3)1/�Ұ�:2��ǰ��ȣ(1,2,3)0))
		else if (buf[0] == '2') {							
			itemni = 0;
			itemn[itemni++] = buf[1];
			itemn[itemni] = '\0';																														//�ֹ���ǰ�ޱ�
			for (i = 2; i < 4; i++) {
				itemcount[ici++] = buf[i];
			}
			itemcount[ici] = '\0';
			itemcountn = atoi(itemcount);																											//�ֹ����� �ޱ�
			for (i = 8; i < 10; i++) {
				seatnum[seati++] = buf[i];
			}
			seatnum[seati] = '\0';																														//�ڸ� �ޱ�
			lstrcpy(ordermsg, "2");
			lstrcat(ordermsg, itemn);
			lstrcat(ordermsg, "1");																														
			//lstrcat(ordermsg, "0");
			nReturn = send(clientsock,ordermsg, sizeof(ordermsg), 0);																//Ŭ���̾�Ʈ�� �ֹ� ��� ������
		}
		//ȸ������ ó��(���� ����(���̵�,���,�̸�..)���� ���� �����ϱ� (����:31/�Ұ�:30))
		else if (buf[0] == '3') {						
			turn = -1;																																			//������� ���� �ޱ� ���� ����
			i = 1;																																					//������ �޴� ������
			///Ŀ�ǵ忡�� ���� ������ ������ ���μ���
			while (buf[i] != '\0') {
				infoi = 0;
				if (buf[i] == ':') {																															///�� ������ ��� ���� �ε����� �̵�
					ini = i + 1;
					turn++;
				}
				if (turn == 0) {																						
					while (buf[ini] != ':' && buf[ini] != '\0') {																					///":"���������� �ޱ�
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 2) infoi -= 2;																											///���� ��� Ŀ���(PW,NAME...) ���ڸ�ŭ ����
					info[infoi] = '\0';
					lstrcpy(tgId, info);																													//*���̵�
				}
				else if (turn == 1) {									
					while (buf[ini] != ':' && buf[ini] != '\0') {					
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 4) infoi -= 4;					
					info[infoi] = '\0';
					lstrcpy(tgPw, info);																													//*���
				}
				else if (turn == 2) {									
					while (buf[ini] != ':' && buf[ini] != '\0') {				
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 2) infoi -= 2;				
					info[infoi] = '\0';
					lstrcpy(tgName, info);																												//*�̸� ���
				}
				else if (turn == 3) {									
					while (buf[ini] != ':' && buf[ini] != '\0') {			
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 4) infoi -= 4;				
					info[infoi] = '\0';
					lstrcpy(tgPn, info);																													//*��� ���
				}
				else if (turn == 4) {								
					while (buf[ini] != ':' && buf[ini] != '\0') {				
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 5) infoi -= 5;				
					info[infoi] = '\0';
					lstrcpy(tgAddr, info);																												//*�ּ� ���
				}
				else if (turn == 5) {									
					while (buf[ini] != '\0') {																											///���������� ���
						info[infoi++] = buf[ini++];
					}
					info[infoi] = '\0';
					lstrcpy(tgBirth, info);																												//*���� ���
					turn = 0;
					break;
				}
				i++;
			}
			lstrcpy(joinmsg, "31");										
			nReturn = send(clientsock, joinmsg, sizeof(joinmsg), 0);																	//Ŭ���̾�Ʈ�� ���� ��� ������
		}

		//�α��� ó��(���̵�� ��� ������ �ش� �������� ������(����:41/�Ұ�:40))
		else if (buf[0] == '4') {										
			i = 1;																																					//������ �޴� ������
			turn = -1;																																			//������� ���� �ޱ� ���� ����
			///Ŀ�ǵ忡�� ���� �������� ������ ���μ���
			while (buf[i] != '\0') { 
				infoi = 0;
				if (buf[i] == ':') {																															///�� ������ ��� ���� �ε����� �̵�
					ini = i + 1;
					turn++;
				}
				if (turn == 0) {
					while (buf[ini] != ':' && buf[ini] != '\0') {																					///":"���������� �ޱ�
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 2) infoi -= 2;																											///���� ��� Ŀ���(PW,NAME...) ���ڸ�ŭ ����
					info[infoi] = '\0';
					lstrcpy(tgId, info);																													//*ID������
				}
				else if (turn == 1) {
					while (buf[ini] != '\0') {																											///���������� �ޱ�
						info[infoi++] = buf[ini++];
					}
					info[infoi] = '\0';
					lstrcpy(tgPw, info);																													//*PW������
					turn = 0;
					break;
				}
				i++;
			}
			//�α����� ������ ���� ���õ�(DB���� ���̵�� ������� ���� ��������)
			lstrcpy(loginmsg, "41");
			//lstrcpy(loginmsg, "40");
			lstrcpy(usertime, "10");			//���� �ð�
			//lstrcpy(usertime, "00");	
			lstrcpy(username, "name01");				
			lstrcpy(userpn, "pn01");
			lstrcpy(useraddr, "addr01");
			lstrcpy(userbirth, "birth01");
			lstrcat(loginmsg, usertime);
			lstrcat(loginmsg, "NAME:");
			lstrcat(loginmsg, username);
			lstrcat(loginmsg, "PN:");
			lstrcat(loginmsg, userpn);
			lstrcat(loginmsg, "ADDR:");
			lstrcat(loginmsg, useraddr);
			lstrcat(loginmsg, "BIRTH:");
			lstrcat(loginmsg, userbirth);
			nReturn = send(clientsock, loginmsg, sizeof(loginmsg), 0);																//������ �ش� �������� Ŭ���̾�Ʈ�� �����ֱ�
		}
		//�α׾ƿ�ó��(���̵�� ��� ���� �Ǻ��ϱ�(����:51/�Ұ�:50))
		else if (buf[0] == '5') {						
			i = 1;																																					//������ ��� ������
			turn = -1;																																			//������� ���� �ޱ� ���� ����
			///Ŀ�ǵ忡�� ���� �������� ������ ���μ���
			while (buf[i] != '\0') { 
				infoi = 0;
				if (buf[i] == ':') {																															///�� ������ ��� ���� �ε����� �̵�
					ini = i + 1;
					turn++;
				}
				if (turn == 0) {
					while (buf[ini] != ':' && buf[ini] != '\0') {																					///":"���������� �ޱ�
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 2) infoi -= 2;																											///���� ��� Ŀ���(PW,NAME...) ���ڸ�ŭ ����
					info[infoi] = '\0';
					lstrcpy(tgId, info);																													//*ID������
				}
				else if (turn == 1) {
					while (buf[ini] != ':' && buf[ini] != '\0') {
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 2) infoi -= 2;
					info[infoi] = '\0';
					lstrcpy(tgPw, info);																													//*PW������
				}
				else if (turn == 2) {
					while (buf[ini] != '\0') {																											///���������� �ޱ�
						info[infoi++] = buf[ini++];
					}
					info[infoi] = '\0';
					lstrcpy(tgSn, info);																													//*�ڸ� ������
					turn = 0;
					break;
				}
				i++;
			}

			lstrcpy(logoutmsg, "51");
			//lstrcpy(logoutmsg, "50");		
			nReturn = send(clientsock,logoutmsg, sizeof(logoutmsg), 0);															//�α׾ƿ� ��� ������
		}
		//���� ó��(���� �������� ������ �����ϱ�(����:61/�Ұ�:60))
		else if (buf[0] == '6') {						
			i = 1;																																					//������ ��� ������
			turn = -1;																																			//������� ���� �ޱ� ���� ����
			///Ŀ�ǵ忡�� ���� ���� ������ ���μ���
			while (buf[i] != '\0') {
				infoi = 0;
				if (buf[i] == ':') {																															///�� ������ ��� ���� �ε����� �̵�
					ini = i + 1;
					turn++;
				}
				if (turn == 0) {
					while (buf[ini] != ':' && buf[ini] != '\0') {																					///":"���������� �ޱ�
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 2) infoi -= 2;																											///���� ��� Ŀ���(PW,NAME...) ���ڸ�ŭ ����
					info[infoi] = '\0';
					lstrcpy(tgId, info);																													//*ID������
				}
				else if (turn == 1) {
					while (buf[ini] != ':' && buf[ini] != '\0') {
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 4) infoi -= 4;
					info[infoi] = '\0';
					lstrcpy(tgPw, info);																													//*PW������
				}
				else if (turn == 2) {
					while (buf[ini] != ':' && buf[ini] != '\0') {
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 2) infoi -= 2;
					info[infoi] = '\0';
					lstrcpy(tgName, info);																												//*NAME������
				}
				else if (turn == 3) {
					while (buf[ini] != ':' && buf[ini] != '\0') {
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 4) infoi -= 4;
					info[infoi] = '\0';
					lstrcpy(tgPn, info);																													//*PN������
				}
				else if (turn == 4) {
					while (buf[ini] != ':' && buf[ini] != '\0') {
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 5) infoi -= 5;
					info[infoi] = '\0';
					lstrcpy(tgAddr, info);																												//*ADDR������
				}
				else if (turn == 5) {
					while (buf[ini] != '\0') {																											///���������� �ޱ�
						info[infoi++] = buf[ini++];
					}
					info[infoi] = '\0';
					lstrcpy(tgBirth, info);																												//*BIRTH������
					turn = 0;
					break;
				}
				i++;
			}

			lstrcpy(updatemsg, "61");
			//lstrcpy(updatemsg, "60");		
			nReturn = send(clientsock, updatemsg, sizeof(updatemsg), 0);															//���� ��� ������
		}
		//Ż��ó��(���̵�,���,�ڸ���ȣ�� ������ �Ǻ��ϱ�(����:71/�Ұ�:70))
		else if (buf[0] == '7') {							
			i = 1;																																					//������ ��� ������
			turn = -1;																																			//������� ���� �ޱ� ���� ����
			///Ŀ�ǵ忡�� ���� ������ ������ ���μ���
			while (buf[i] != '\0') {
				infoi = 0;
				if (buf[i] == ':') {																															///�� ������ ��� ���� �ε����� �̵�
					ini = i + 1;
					turn++;
				}
				if (turn == 0) {
					while (buf[ini] != ':' && buf[ini] != '\0') {																					///":"���������� �ޱ�
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 2) infoi -= 2;																											///���� ��� Ŀ���(PW,NAME...) ���ڸ�ŭ ����
					info[infoi] = '\0';
					lstrcpy(tgId, info);																													//*ID������
				}
				else if (turn == 1) {
					while (buf[ini] != ':' && buf[ini] != '\0') {
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 2) infoi -= 2;
					info[infoi] = '\0';
					lstrcpy(tgPw, info);																													//*PW������
				}
				else if (turn == 2) {
					while (buf[ini] != '\0') {																											///���������� �ޱ�
						info[infoi++] = buf[ini++];				
					}
					info[infoi] = '\0';
					lstrcpy(tgSn, info);																													//*SN(�ڸ���ȣ) ������
					turn = 0;
					break;
				}
				i++;
			}

			lstrcpy(deletemsg, "71");
			//lstrcpy(deletemsg, "70");		
			nReturn = send(clientsock, deletemsg, sizeof(deletemsg), 0);															//Ż�� ��� ������
		}
		//������ ��� �޽��� ����Ʈ�ڽ��� ����
			sprintf_s(strTemp, "������ �޽���:%s : %d", buf, clientsock);	
			hChatEdit = GetDlgItem(hWndDlg, IDC_CHATSERVERLIST);
			int len = GetWindowTextLength(hChatEdit);
			SendDlgItemMessage(hWndDlg, IDC_CHATSERVERLIST, EM_SETSEL, (WPARAM)len, (LPARAM)len);
			SendDlgItemMessage(hWndDlg, IDC_CHATSERVERLIST, EM_REPLACESEL, FALSE, (LPARAM)"\r\n");
			SendDlgItemMessage(hWndDlg, IDC_CHATSERVERLIST, EM_REPLACESEL, FALSE, (LPARAM)strTemp);		
	}
	return 0;
}
//���� ������
DWORD WINAPI ThreadFunc(LPVOID Param) {
	TCHAR chksockar[10];
	HDC hdc;
	SOCKET* P = (SOCKET*)Param;
	SOCKET popdata;
	TCHAR chkcsi[10];
	int* tp = (int*)Param;
	int ti = *tp;
	int qsize;
	int i;
	int poptrue = 0;
	for (;;) {
		clientsock = accept(*P, (sockaddr*)&addr_client, &addrlen_clt);
		if (clientsock != INVALID_SOCKET && clientsock != 0) {
			hChatEdit = GetDlgItem(hWndDlg, IDC_CHATSERVERLIST);
			int len = GetWindowTextLength(hChatEdit);
			SendDlgItemMessage(hWndDlg, IDC_CHATSERVERLIST, EM_SETSEL, (WPARAM)len, (LPARAM)len);
			SendDlgItemMessage(hWndDlg, IDC_CHATSERVERLIST, EM_REPLACESEL, FALSE, (LPARAM)"\r\n");
			SendDlgItemMessage(hWndDlg, IDC_CHATSERVERLIST, EM_REPLACESEL, FALSE, (LPARAM)"�մ���  �����Ͽ����ϴ�.");
			clientsockar[csi] = clientsock;
			g_bClientThreadExitFlags[csi] = FALSE;
			hClientThread[csi] = CreateThread(NULL, 0, ClientThreadFunc, (LPVOID)csi, 0, &ClientThreadID);
			csi++;
		}
	}
	return 0;
}
////��ȭ���� ��Ʈ�ѵ��� ���� ����
BOOL CALLBACK ServerDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case WM_INITDIALOG:
		//DBConnect();
		hWndDlg = hDlg;
		InitCommonControls();
		SetDlgItemText(hDlg, IDC_IPSERVEREDIT, tgIp);
		SetDlgItemText(hDlg, IDC_PORTSERVEREDIT, tgPort);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_SERVERPOWERBUTTON:
			if (serverpower == 0) {
				serverpower = 1;
				GetDlgItemText(hDlg, IDC_IPSERVEREDIT, tgIp, sizeof(tgIp));
				GetDlgItemText(hDlg, IDC_PORTSERVEREDIT, tgPort, sizeof(tgPort));
				nip = atoi(tgIp);
				nport = atoi(tgPort);
				nReturn = WSAStartup(WORD(2.0), &wsadata);
				listensock = socket(AF_INET, SOCK_STREAM, 0);
				addr_server.sin_family = AF_INET;
				addr_server.sin_addr.s_addr = htonl(INADDR_ANY);
				addr_server.sin_port = htons(nport);
				nReturn = bind(listensock, (sockaddr*)&addr_server, sizeof(sockaddr));
				nReturn = listen(listensock, 1);
				MessageBox(hDlg, "��������", "server", MB_OK);
				hThread = CreateThread(NULL, 0, ThreadFunc, &listensock, 0, &ThreadID);
			}
			else {
				serverpower = 0;
				MessageBox(hDlg, "end", "server", MB_OK);
				for (i = 0; i < csi; i++)TerminateThread(hClientThread[i], 0);
				TerminateThread(hThread, 0);
				for (i = 0; i < csi; i++) {
					clientsockar[i] = 0;
				}
				csi = 0;
				closesocket(clientsock);
				WSACleanup();
			}
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			InvalidateRect(hWndDlg, NULL, TRUE);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

int APIENTRY WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, LPSTR lpszCmdParam, int nCmdShow)
{
	g_hinst = hinstance;
	DialogBox(g_hinst, MAKEINTRESOURCE(IDD_SERVERDIALOG), HWND_DESKTOP, (DLGPROC)ServerDlgProc);
}
