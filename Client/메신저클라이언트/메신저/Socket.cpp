#include <Windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Common.h"
#include "Socket.h"

extern HWND hWndMain;
////IP�� ��Ʈ��ȣ ����
extern const char g_szlpAddress[17] = "127.0.0.1";
extern const unsigned short g_uPort = 7878;
extern TCHAR tgIp[17];
extern TCHAR tgPort[10];
extern TCHAR tgId[10], tgName[10], tgPw[10], tgPn[10], tgAddr[10], tgBirth[10];																//���̵�,�̸�,���,��ȭ��ȣ,�ּ�,���� ���ڿ� ������
extern int nip, nport;

extern DWORD ThreadID;																																					//��������̵�
extern HANDLE hThread;

extern TCHAR tgcmdserver[200];																																		//������ ����ϴ� Ŀ��� ���ڿ�
////
//���ú꽺���� �⺻ ����
extern HDC hdc;
extern SOCKET* P;
extern TCHAR CODE[3], TEXT[256];
extern int j;
//
///���� ���� ����
extern int nReturn;																																								//��� ��� 
extern WSADATA wsadata;																																				//���� ����
extern SOCKET clientsock;																																					//���Ϲ�ȣ ���
extern sockaddr_in addr_client;
extern int addrlen_clt;
///�����κ��� �޴� �޼���
extern char buf[1024];																																						//�޽��� ���ڿ�																						
extern char strTemp[1024];																																				//�޼����� ä��â�� ������ �޼����� ��ȯ
///

extern int infotrue;																																							//�� ���� ��� ����
////��Ȳ�� �ֿ亯��
//Ŀ��忡 ���� ������ ������
extern int turn;																																								//������ ������ ������� �������� �ϴ� ���� ����
extern int ini;																																										//�� �������� ����(�޼���)�� �ε���
extern TCHAR info[200];																																					//��Ҹ� �̴� ���ڿ�
extern int infoi;																																								//��Ҹ� �̴� ���ڿ��� �ε���
//
//�α��λ�Ȳ��  ������ ������
extern TCHAR userid[10];																																					//���̵�
extern TCHAR usersn[10];																																					//�ڸ���ȣ
extern TCHAR userpw[10];																																					//���
extern int pctimei;																																								//�ð� �ε���
extern int pctime;																																								//�α��ε� ������ �ð�
//
///�ֹ���Ȳ�� ������
//�ֹ��ð�
extern int orderpctime;																																						//�ֹ���  �ð����� ������ ����
//�������ֹ�����
extern TCHAR tgselitemi[10];																																				//�ε����� ���ڿ��� ��ȯ
extern int selitemi;																																								//����Ʈ �ڽ��� �ε���
extern TCHAR tgitemcount[10];																																	//���� ���ڿ�
//
///
//���� üũ ����
extern BOOL chkseat;																																							//�ڸ�����
extern BOOL chkjoin;																																							//����
extern BOOL chklogin;																																						//�α���
extern BOOL chktimer;																																							//Ÿ�̸� ����(1�̸� ����/0�̸� ����)	
extern BOOL chkendbtn;																																					//�α׾ƿ�/Ż�� 

extern BOOL chkorder;																																						//�ֹ�
extern BOOL chkupdate;																																					//����
extern BOOL chkpctime;																																					//�����ð�0����
//
extern int i;
//�ڸ���ȣ ���� ������
extern TCHAR tgnum[10];																																					//��û�ϴ� �ڸ���ȣ ���ڿ�

//Ŀ�ǵ尡 �ֹ����� �Ǻ�
extern TCHAR orderbuf[10];																																				//�ֹ��������(�̸� �޼����� �ֹ����� �Ǻ�)
extern int orderi;																																							//�ֹ�������� �ε���
//

//�����ð�
extern TCHAR pctimebuf[10];																																				//Ŀ�ǵ忡�� ��ƿ���
extern TCHAR tpctime[10];																																					//����ϱ� ���� ���ڿ�
//

//�¼���ȯ����
extern int returnsntime;																																//�ڸ��ݳ�Ÿ�̸ӽð�
extern BOOL returnsn;																																						//�ڸ��ݳ�����

//�ڸ��������� ������ ����
extern BOOL gohome;

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

//DWORD WINAPI ThreadFunc(LPVOID){}:���ú꽺����ν� ��û�� ����� �ٷ�
//�Ӹ����� ���� ���� ���ڰ�  1�� ���� ����/ 0�� ���� �Ұ�
DWORD WINAPI ThreadFunc(LPVOID Param) {
	//HDC hdc;
	//SOCKET* P = (SOCKET*)Param;
	//TCHAR CODE[3] = "", TEXT[256] = "";
	//int j;
	P = (SOCKET*)Param;
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

			//�ڸ����� ���(����:11/�Ұ�:10)
			if (buf[0] == '1') {
				if (lstrcmp(buf, TEXT("11")) == 0) {
					returnsntime = 100000;																													//�ڸ��ݳ�Ÿ�̸ӽð� �ʱ�ȭ
					returnsn = 1;																																			//�ڸ��ݳ�����
					chkseat = 1;																																			//�ڸ����� üũ
					//SetTimer(hWndMain, 0, 100, NULL);																									//�ڵ����� ȭ���� ���� Ÿ�̸�
					//SetTimer(hWndMain, 3, 1000000, NULL);																							//�ڸ��ݳ�Ÿ�̸�
					//SendMessage(hWndMain, WM_TIMER, 0, 0);
					//SendMessage(hWndMain, WM_TIMER, 3, 0);
					lstrcpy(usersn, tgnum);																															//�ڸ���ȣ ���		
					lstrcpy(tgnum, "");																																//�ڸ���ȣ �ʱ�ȭ
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
					infoi = 0;
					ini = 2;
					while (buf[ini] != ':' && buf[ini] != '\0') {																		///":"������ ���
						info[infoi++] = buf[ini++];
					}
					info[infoi] = '\0';
					lstrcpy(pctimebuf, info);
					//MessageBox(hWndMain, pctimebuf, "pctimebuf", MB_OK);
					pctime = atoi(pctimebuf);
					//
					//�����Ҷ� �Է��� ���̵� ����� �����ϰ� �̸�,��ȭ��ȣ,�ּ�,���ϵ��� ���
					if (pctime >= 0) {
						lstrcpy(userid, tgId);																											//���� ���̵�,��� ���
						lstrcpy(userpw, tgPw);
						i = ini+1;																																		//�α��� Ŀ�ǵ��� ������ ������  ������
						turn = -1;																																//������ ������ ������� �������� ����
						///�α��� Ŀ��忡�� ������ ������ ��� ���μ���(NAME-PN-ADDR-BIRTH��)
						while (buf[i] != '\0') {
							infoi = 0;
							if (buf[i] == ':') {																												///�� ������ ���������� �̵�
								ini = i + 1;
								turn++;
								infotrue = 0;
							}
							if (turn == 0&&infotrue==0) {
								while (buf[ini] != ':' && buf[ini] != '\0') {																		///":"������ ���
									info[infoi++] = buf[ini++];
								}
								if (infoi >= 2) infoi -= 2;																								///���� ���� Ŀ�ǵ�(PW,NAME..) ���� ��ŭ ����
								info[infoi] = '\0';
								lstrcpy(tgName, info);
								infotrue = 1;
							}
							else if (turn == 1&&infotrue==0) {
								while (buf[ini] != ':' && buf[ini] != '\0') {
									info[infoi++] = buf[ini++];
								}
								if (infoi >= 4) infoi -= 4;
								info[infoi] = '\0';
								lstrcpy(tgPn, info);
								infotrue = 1;
							}
							else if (turn == 2&&infotrue==0) {
								while (buf[ini] != ':' && buf[ini] != '\0') {
									info[infoi++] = buf[ini++];
								}
								if (infoi >= 5) infoi -= 5;
								info[infoi] = '\0';
								lstrcpy(tgAddr, info);
								infotrue = 1;
							}
							else if (turn == 3&&infotrue==0) {
								while (buf[ini] != '\0') {																									///���������� ���
									info[infoi++] = buf[ini++];
								}
								info[infoi] = '\0';
								lstrcpy(tgBirth, info);
								turn = -1;
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
					if (lstrcmp(buf, TEXT("211")) == 0) {
						chkorder = 1;
						wsprintf(buf, "%s", "DRINK���Լ���");
					}
					else wsprintf(buf, "%s", "DRINK���Խ���");
				}
				//��ǰ
				else if (buf[1] == '2') {
					if (lstrcmp(buf, TEXT("221")) == 0) {
						chkorder = 1;
						wsprintf(buf, "%s", "FOOD���Լ���");
					}
					else wsprintf(buf, "%s", "FOOD���Խ���");
				}
				//PC
				else if (buf[1] == '3') {
					if (lstrcmp(buf, TEXT("231")) == 0) {
						chkorder = 1;
						orderpctime = atoi(tgitemcount);
						pctime += orderpctime;
						wsprintf(tpctime, "%d", pctime);																								//�����ð��� ����ϱ����� ���ڿ���
						lstrcpy(tgitemcount, "");																											//�ֹ����� ���ڿ� �ʱ�ȭ
						wsprintf(buf, "%s", "PC���Լ���");
					}
					else wsprintf(buf, "%s", "PC���Խ���");
				}
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
			//�����κ��� ���� �޼����� �޼����ڽ��� ����
			if (lstrlen(buf) != 0) {
				MessageBox(hWndMain, buf, "������ �޽���", MB_OK);
			}
		}
	}
	return 0;
}