#include "Common.h"
#include "Request.h"
extern TCHAR tgId[10], tgName[10], tgPw[10], tgPn[10], tgAddr[10], tgBirth[10];																//���̵�,�̸�,���,��ȭ��ȣ,�ּ�,���� ���ڿ� ������

//�ֹ� �ð�
extern int orderpctime;

//���Ϻ���
extern int nReturn;

extern TCHAR tgcmdserver[200];

extern SOCKET clientsock;
//
extern HWND hWnd;

//Ÿ�̸�
extern HWND hEdit_Timer;
//�α���
extern HWND hEdit_Login_Id;
extern HWND hEdit_Login_Pw;
//�ֹ�
extern HWND hEdit_OrderCount;
//�ڸ�����
extern HWND hEdit_Sn;
//���� �� ����
extern HWND hEdit_Id;
extern HWND hEdit_Pw;
extern HWND hEdit_Name;
extern HWND hEdit_Addr;
extern HWND hEdit_Pn;
extern HWND hEdit_Birth;

//�ڸ���ȣ ��ư Ƚ�� ����
extern int snbtncount;

//�ֹ����� ��ư Ƚ�� ����
extern int ocbtncount;

//�α��λ�Ȳ��  ������ ������
extern TCHAR userid[10];																																					//���̵�
extern TCHAR usersn[10];																																					//�ڸ���ȣ
extern TCHAR userpw[10];																																					//���
extern int pctimei;																																								//�ð� �ε���
extern int pctime;																																								//�α��ε� ������ �ð�
//

//�������ֹ�����
extern TCHAR tgselitemi[10];																																				//�ε����� ���ڿ��� ��ȯ
extern int selitemi;																																								//����Ʈ �ڽ��� �ε���
extern TCHAR tgitemcount[10];																																	//���� ���ڿ�
//


//�����ð�
extern TCHAR pctimebuf[10];																																				//Ŀ�ǵ忡�� ��ƿ���
extern TCHAR tpctime[10];																																					//����ϱ� ���� ���ڿ�
//


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

//�¼���ȯ����
extern int returnsntime;																																//�ڸ��ݳ�Ÿ�̸ӽð�
extern BOOL returnsn;																																						//�ڸ��ݳ�����

//�ڸ��������� ������ ����
extern BOOL gohome;

//���ڿ� ���� ':' ������ üũ�ϱ�
BOOL ChkStringInfo(TCHAR* str) {
	int i = 0;
	for (i = 0; i < lstrlen(str); i++) {
		if (str[i] == ':')return 1;
	}
	return 0;
}

//void RequestLogin():�α��ν�û
void RequestLogin() {
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
}
//void RequestOrder():�ֹ���û(��ư �Ǵ� �ؽ�Ʈ�� ���ڸ� ����)
void RequestOrder() {
	//�߰��� ���: �α��� �� ���̵���� ���� ������
	lstrcpy(tgcmdserver, "2");
	lstrcat(tgcmdserver, tgselitemi);
	GetWindowText(hEdit_OrderCount, tgitemcount, sizeof(tgitemcount));													//�������ֹ����� ���ڿ��� ���
	orderpctime = atoi(tgitemcount);
	lstrcat(tgcmdserver, tgitemcount);
	lstrcat(tgcmdserver, "ID:");																														//�Է¾��̵� ���ڿ��� ���
	lstrcat(tgcmdserver, tgId);
	nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);															//������ �ֹ� Ŀ�ǵ� ������

	ocbtncount = 0;																																		//�ֹ����� ��ư Ƚ�� �ʱ�ȭ
	SetWindowText(hEdit_OrderCount, "");
}
//void RequestJoin():���Խ�û
void RequestJoin() {
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
}
//void RequestUpdate():������û
void RequestUpdate() {
	//Ŀ�ǵ�,���� ���� ���ڿ� ��ĭ���� �ʱ�ȭ
	lstrcpy(tgcmdserver, "");																																	//Ŀ�ǵ�
	SetWindowText(hEdit_Id, tgId);
	SetWindowText(hEdit_Name, tgName);
	SetWindowText(hEdit_Pw, tgPw);
	SetWindowText(hEdit_Pn, tgPn);
	SetWindowText(hEdit_Addr, tgAddr);
	SetWindowText(hEdit_Birth, tgBirth);
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
}
//void RequestSelSeat():�ڸ���ȣ��û(��ư �Ǵ� �ؽ�Ʈ�� ���ڸ�����)
void RequestSelSeat() {
	lstrcpy(tgcmdserver, "1");
	GetWindowText(hEdit_Sn, tgnum, sizeof(tgnum));																				//�ڸ� ����(02�� ���� 2�ڸ� ��������)
	lstrcat(tgcmdserver, tgnum);
	nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);														//������ �ڸ����� Ŀ�ǵ� ������

	snbtncount = 0;																																		//�ڸ���ȣ��ư Ƚ�� �ʱ�ȭ
}
//void RequestLogout():�α׾ƿ���û
void RequestLogout() {
	lstrcpy(tgcmdserver, "");
	lstrcpy(tgcmdserver, "5ID:");
	lstrcat(tgcmdserver, userid);
	lstrcat(tgcmdserver, "PW:");
	lstrcat(tgcmdserver, userpw);
	lstrcat(tgcmdserver, "SN:");
	lstrcat(tgcmdserver, usersn);
	lstrcat(tgcmdserver, ":");
	lstrcat(tgcmdserver, tpctime);																														//�����ð� ������
	chkendbtn = 1;																																				//�α׾ƿ�/Ż�� ���� ��������
	nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);																//������ �α׾ƿ� Ŀ��� ������
	pctime = 0;
	wsprintf(tpctime, "%d", pctime);
	SetWindowText(hEdit_Timer, tpctime);																										//���ν����� Ÿ�̸� ����Ʈ�� ���� ���� �ð� 0���� ���
}
//void RequestDelete():Ż���û
void RequestDelete() {
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