#include "Common.h"
#include "Timer.h"
extern HWND hWnd;
//���� ����
extern int nReturn;

extern TCHAR tgcmdserver[200];

extern SOCKET clientsock;
//
//Ÿ�̸�
extern HWND hEdit_Timer;
//�α��λ�Ȳ��  ������ ������
extern TCHAR userid[10];																																					//���̵�
extern TCHAR usersn[10];																																					//�ڸ���ȣ
extern TCHAR userpw[10];																																					//���
extern int pctimei;																																								//�ð� �ε���
extern int pctime;																																								//�α��ε� ������ �ð�
//
///�ֹ���Ȳ�� ������
//�������ֹ�����
extern TCHAR tgselitemi[10];																																				//�ε����� ���ڿ��� ��ȯ
extern int selitemi;																																								//����Ʈ �ڽ��� �ε���
extern TCHAR tgitemcount[10];																																	//���� ���ڿ�
//
///
//���� üũ ����
extern BOOL chklogin;																																						//�α���
extern BOOL chktimer;																																							//Ÿ�̸� ����(1�̸� ����/0�̸� ����)	
extern BOOL chkendbtn;																																					//�α׾ƿ�/Ż�� 

extern BOOL chkpctime;																																					//�����ð�0����
//
extern int i;
//�ڸ���ȣ ���� ������
extern TCHAR tgnum[10];																																					//��û�ϴ� �ڸ���ȣ ���ڿ�
//�����ð�
extern TCHAR pctimebuf[10];																																				//Ŀ�ǵ忡�� ��ƿ���
extern TCHAR tpctime[10];																																					//����ϱ� ���� ���ڿ�
//

//�¼���ȯ����
extern int returnsntime;																																//�ڸ��ݳ�Ÿ�̸ӽð�
extern BOOL returnsn;																																						//�ڸ��ݳ�����

//�ڸ��������� ������ ����
extern BOOL gohome;
//void UserTimer():�����ð�Ÿ�̸� 0�̵Ǹ� �α׾ƿ� ó�� ��û
void UserTimer() {
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
	}
}
//void SetUserTimer():���������ð�Ÿ�̸� �缳��
void SetUserTimer() {
	gohome = 1;
	KillTimer(hWnd, 1);																														//Ÿ�̸� ����
	chktimer = 0;																																//Ÿ�̸� ���� ���·�
	SetTimer(hWnd, 1, 1000, NULL);																									//���� ���� �ð� Ÿ�̸� �ٽ� ����																															//�α׾ƿ� üũ �ٽ� ���� ��� �ʱ�ȭ
	pctime = 0;
	wsprintf(tpctime, "%d", pctime);
	SetWindowText(hEdit_Timer, tpctime);																						//���ν����� Ÿ�̸� ����Ʈ�� ���� ���� �ð� 0����  ���
}
//void ReturnSeatTimer():�ڸ��ݳ�Ÿ�̸�(�α��ι�ư ���ѽð��ȿ� ������ ������ �ڸ��ݳ�ó�� ��û)
void ReturnSeatTimer() {
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