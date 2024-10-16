
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
LPCTSTR lpszClass = TEXT("대화상자");


HWND hChatEdit;														//채팅 에디트 

int nonexistsock;															//잘못된 소켓번호

////소켓통신 설정위한 변수 선언
int nReturn;
WSADATA wsadata;
SOCKET listensock;																
SOCKET clientsock = 0;														//클라이언트 소켓

DWORD ThreadID;																//스레드아이디
HANDLE hThread;																//연결 스레드핸들
HANDLE hClientThread[10];												//리시브스레드 핸들
DWORD ClientThreadID;														//리시브스레드 아이디

SOCKET clientsockar[10];													//리시브스레드소켓배열

////스레드 종료 위한 플래그
BOOL g_bThreadExitFlag = FALSE;
volatile BOOL g_bClientThreadExitFlags[10] = { FALSE };

int csi = 0;																			//리시브 스레드가 멀티이므로 각각 식별위한 인덱스
int i, j;
////전원 버튼 눌렸는지 판별
BOOL serverpower = 0;
BOOL serveropt = 0;
////포트와 IP번호 설정
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
////메세지 길이 설정
const int buflen = 40960;						//버퍼 총 길이
////메세지
char buf[buflen];									//메세지

char strTemp[buflen];							//커멘드를 바꿔서 채팅창에 띄우기

HWND hWndDlg;									//대화상자 핸들



///변수들
//로그인 유저 변수들
TCHAR usertime[100];							//남은 시간
TCHAR username[100];						//이름					
TCHAR userpn[100];								//전화번호
TCHAR useraddr[100];							//주소
TCHAR userbirth[100];						//생일
//
//커멘드
TCHAR seatmsg[200];							//자리선택
TCHAR ordermsg[200];						//주문
TCHAR joinmsg[200];							//가입
TCHAR loginmsg[200];							//로그인
TCHAR logoutmsg[200];						//로그아웃
TCHAR updatemsg[200];						//수정
TCHAR deletemsg[200];						//삭제
//
//커멘드에서 꺼낸 정보 저장하는 문자열
TCHAR tgSn[20];									//자리번호
TCHAR tgId[50];									//아이디
TCHAR tgPw[50];									//비번
TCHAR tgName[50];							//이름
TCHAR tgPn[50];									//전화번호
TCHAR tgAddr[50];								//주소
TCHAR tgBirth[50];								//생일
TCHAR info[200];								//각 정보
//
//커맨드가 여러 정보가 있을때 꺼내기 위한 변수들
int turn;												//각 순서에 순서 제어
int infoi;												//정보의 인덱스
int ini;													//버퍼(메세지)의 인덱스
//
//커멘드가 주문일때의 주문상품 변수들
TCHAR itemn[10];								//인덱스 
TCHAR itemcount[10];							//개수
int ici;													//버퍼(메세지)의 인덱스
int itemni;												//인덱스의 인덱스
int itemcountn;										//개수(숫자)
//
//커멘드가 자리선택일때 자리번호 변수들
TCHAR seatnum[10];							//버퍼에서 자리번호 꺼내기
int seati = 0;											//인덱스
int seatn;												//자리번호(숫자)


//중요!!!!!!!!!!! 클라이언트와 커맨드 주고 받는 방법이므로 참고하여 그대로 이용하기 (피시방주문시스템.txt 필수 참고)
//DWORD WINAPI ClientThreadFunc(LPVOID):수신스레드(요청 판별하고 처리해주기)
DWORD WINAPI ClientThreadFunc(LPVOID Param) {						
	int i;
	int csi = (int)Param;																																	//각각 리시브스레드 식별 위한 인덱스
	SOCKET clientsock = clientsockar[csi];																									//각자 소켓번호 담기
	while (!g_bThreadExitFlag && !g_bClientThreadExitFlags[csi]) {																//정상일떄
		if (clientsock == nonexistsock) break;																									//소켓 이상하면 스레드 종료
		lstrcpy(buf, "");
		nReturn = recv(clientsock, buf, buflen, 0);
	
		//자리요청(자리번호 꺼내서 판별하기 (가능:11/불가:10))
		if (buf[0] == '1') {							
			for (i = 1; i < 3; i++) {
				seatnum[seati++] = buf[i];
			}
			seatnum[seati] = '\0';																														//자리번호 담기
			seatn = atoi(seatnum);
			seati = 0;
			//좌석번호체크
			lstrcpy(seatmsg, "11");
			//lstrcpy(seatmsg, "10");
			nReturn = send(clientsock,seatmsg, sizeof(seatmsg), 0);																	//클라이언트에 자리요청 결과 보내기
		}
		//주문요청(상품,수량,자리 꺼내서 판별하기 (가능:2상품번호(1,2,3)1/불가:2상품번호(1,2,3)0))
		else if (buf[0] == '2') {							
			itemni = 0;
			itemn[itemni++] = buf[1];
			itemn[itemni] = '\0';																														//주문상품받기
			for (i = 2; i < 4; i++) {
				itemcount[ici++] = buf[i];
			}
			itemcount[ici] = '\0';
			itemcountn = atoi(itemcount);																											//주문수량 받기
			for (i = 8; i < 10; i++) {
				seatnum[seati++] = buf[i];
			}
			seatnum[seati] = '\0';																														//자리 받기
			lstrcpy(ordermsg, "2");
			lstrcat(ordermsg, itemn);
			lstrcat(ordermsg, "1");																														
			//lstrcat(ordermsg, "0");
			nReturn = send(clientsock,ordermsg, sizeof(ordermsg), 0);																//클라이언트에 주문 결과 보내기
		}
		//회원가입 처리(여러 정보(아이디,비번,이름..)들을 빼서 저장하기 (가능:31/불가:30))
		else if (buf[0] == '3') {						
			turn = -1;																																			//순서대로 정보 받기 위해 제어
			i = 1;																																					//정보를 받는 시작점
			///커맨드에서 여러 정보를 뺴내는 프로세스
			while (buf[i] != '\0') {
				infoi = 0;
				if (buf[i] == ':') {																															///각 정보를 담는 시작 인덱스로 이동
					ini = i + 1;
					turn++;
				}
				if (turn == 0) {																						
					while (buf[ini] != ':' && buf[ini] != '\0') {																					///":"만날때까지 받기
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 2) infoi -= 2;																											///다음 요소 커멘드(PW,NAME...) 숫자만큼 빼기
					info[infoi] = '\0';
					lstrcpy(tgId, info);																													//*아이디
				}
				else if (turn == 1) {									
					while (buf[ini] != ':' && buf[ini] != '\0') {					
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 4) infoi -= 4;					
					info[infoi] = '\0';
					lstrcpy(tgPw, info);																													//*비번
				}
				else if (turn == 2) {									
					while (buf[ini] != ':' && buf[ini] != '\0') {				
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 2) infoi -= 2;				
					info[infoi] = '\0';
					lstrcpy(tgName, info);																												//*이름 담기
				}
				else if (turn == 3) {									
					while (buf[ini] != ':' && buf[ini] != '\0') {			
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 4) infoi -= 4;				
					info[infoi] = '\0';
					lstrcpy(tgPn, info);																													//*비번 담기
				}
				else if (turn == 4) {								
					while (buf[ini] != ':' && buf[ini] != '\0') {				
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 5) infoi -= 5;				
					info[infoi] = '\0';
					lstrcpy(tgAddr, info);																												//*주소 담기
				}
				else if (turn == 5) {									
					while (buf[ini] != '\0') {																											///마지막까지 담기
						info[infoi++] = buf[ini++];
					}
					info[infoi] = '\0';
					lstrcpy(tgBirth, info);																												//*생일 담기
					turn = 0;
					break;
				}
				i++;
			}
			lstrcpy(joinmsg, "31");										
			nReturn = send(clientsock, joinmsg, sizeof(joinmsg), 0);																	//클라이언트로 가입 결과 보내기
		}

		//로그인 처리(아이디와 비번 빼내어 해당 정보들을 보내기(가능:41/불가:40))
		else if (buf[0] == '4') {										
			i = 1;																																					//정보를 받는 시작점
			turn = -1;																																			//순서대로 정보 받기 위해 제어
			///커맨드에서 여러 정보들을 빼내는 프로세스
			while (buf[i] != '\0') { 
				infoi = 0;
				if (buf[i] == ':') {																															///각 정보를 담는 시작 인덱스로 이동
					ini = i + 1;
					turn++;
				}
				if (turn == 0) {
					while (buf[ini] != ':' && buf[ini] != '\0') {																					///":"만날때까지 받기
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 2) infoi -= 2;																											///다음 요소 커멘드(PW,NAME...) 숫자만큼 빼기
					info[infoi] = '\0';
					lstrcpy(tgId, info);																													//*ID꺼내기
				}
				else if (turn == 1) {
					while (buf[ini] != '\0') {																											///마지막까지 받기
						info[infoi++] = buf[ini++];
					}
					info[infoi] = '\0';
					lstrcpy(tgPw, info);																													//*PW꺼내기
					turn = 0;
					break;
				}
				i++;
			}
			//로그인한 유저의 정보 샘플들(DB에서 아이디와 비번으로 정보 가져오기)
			lstrcpy(loginmsg, "41");
			//lstrcpy(loginmsg, "40");
			lstrcpy(usertime, "10");			//남은 시간
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
			nReturn = send(clientsock, loginmsg, sizeof(loginmsg), 0);																//계정의 해당 정보들을 클라이언트로 보내주기
		}
		//로그아웃처리(아이디와 비번 빼서 판별하기(가능:51/불가:50))
		else if (buf[0] == '5') {						
			i = 1;																																					//정보를 담는 시작점
			turn = -1;																																			//순서대로 정보 받기 위해 제어
			///커맨드에서 여러 정보들을 빼내는 프로세스
			while (buf[i] != '\0') { 
				infoi = 0;
				if (buf[i] == ':') {																															///각 정보를 담는 시작 인덱스로 이동
					ini = i + 1;
					turn++;
				}
				if (turn == 0) {
					while (buf[ini] != ':' && buf[ini] != '\0') {																					///":"만날때까지 받기
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 2) infoi -= 2;																											///다음 요소 커멘드(PW,NAME...) 숫자만큼 빼기
					info[infoi] = '\0';
					lstrcpy(tgId, info);																													//*ID꺼내기
				}
				else if (turn == 1) {
					while (buf[ini] != ':' && buf[ini] != '\0') {
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 2) infoi -= 2;
					info[infoi] = '\0';
					lstrcpy(tgPw, info);																													//*PW꺼내기
				}
				else if (turn == 2) {
					while (buf[ini] != '\0') {																											///마지막까지 받기
						info[infoi++] = buf[ini++];
					}
					info[infoi] = '\0';
					lstrcpy(tgSn, info);																													//*자리 꺼내기
					turn = 0;
					break;
				}
				i++;
			}

			lstrcpy(logoutmsg, "51");
			//lstrcpy(logoutmsg, "50");		
			nReturn = send(clientsock,logoutmsg, sizeof(logoutmsg), 0);															//로그아웃 결과 보내기
		}
		//수정 처리(여러 정보들을 빼내고 저장하기(가능:61/불가:60))
		else if (buf[0] == '6') {						
			i = 1;																																					//정보를 담는 시작점
			turn = -1;																																			//순서대로 정보 받기 위해 제어
			///커맨드에서 여러 정보 뺴내는 프로세스
			while (buf[i] != '\0') {
				infoi = 0;
				if (buf[i] == ':') {																															///각 정보를 담는 시작 인덱스로 이동
					ini = i + 1;
					turn++;
				}
				if (turn == 0) {
					while (buf[ini] != ':' && buf[ini] != '\0') {																					///":"만날때까지 받기
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 2) infoi -= 2;																											///다음 요소 커멘드(PW,NAME...) 숫자만큼 빼기
					info[infoi] = '\0';
					lstrcpy(tgId, info);																													//*ID꺼내기
				}
				else if (turn == 1) {
					while (buf[ini] != ':' && buf[ini] != '\0') {
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 4) infoi -= 4;
					info[infoi] = '\0';
					lstrcpy(tgPw, info);																													//*PW꺼내기
				}
				else if (turn == 2) {
					while (buf[ini] != ':' && buf[ini] != '\0') {
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 2) infoi -= 2;
					info[infoi] = '\0';
					lstrcpy(tgName, info);																												//*NAME꺼내기
				}
				else if (turn == 3) {
					while (buf[ini] != ':' && buf[ini] != '\0') {
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 4) infoi -= 4;
					info[infoi] = '\0';
					lstrcpy(tgPn, info);																													//*PN꺼내기
				}
				else if (turn == 4) {
					while (buf[ini] != ':' && buf[ini] != '\0') {
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 5) infoi -= 5;
					info[infoi] = '\0';
					lstrcpy(tgAddr, info);																												//*ADDR꺼내기
				}
				else if (turn == 5) {
					while (buf[ini] != '\0') {																											///마지막까지 받기
						info[infoi++] = buf[ini++];
					}
					info[infoi] = '\0';
					lstrcpy(tgBirth, info);																												//*BIRTH꺼내기
					turn = 0;
					break;
				}
				i++;
			}

			lstrcpy(updatemsg, "61");
			//lstrcpy(updatemsg, "60");		
			nReturn = send(clientsock, updatemsg, sizeof(updatemsg), 0);															//수정 결과 보내기
		}
		//탈퇴처리(아이디,비번,자리번호를 빼내서 판별하기(가능:71/불가:70))
		else if (buf[0] == '7') {							
			i = 1;																																					//정보를 담는 시작점
			turn = -1;																																			//순서대로 정보 받기 위해 제어
			///커맨드에서 여러 정보를 뺴내는 프로세스
			while (buf[i] != '\0') {
				infoi = 0;
				if (buf[i] == ':') {																															///각 정보를 담는 시작 인덱스로 이동
					ini = i + 1;
					turn++;
				}
				if (turn == 0) {
					while (buf[ini] != ':' && buf[ini] != '\0') {																					///":"만날때까지 받기
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 2) infoi -= 2;																											///다음 요소 커멘드(PW,NAME...) 숫자만큼 빼기
					info[infoi] = '\0';
					lstrcpy(tgId, info);																													//*ID꺼내기
				}
				else if (turn == 1) {
					while (buf[ini] != ':' && buf[ini] != '\0') {
						info[infoi++] = buf[ini++];
					}
					if (infoi >= 2) infoi -= 2;
					info[infoi] = '\0';
					lstrcpy(tgPw, info);																													//*PW꺼내기
				}
				else if (turn == 2) {
					while (buf[ini] != '\0') {																											///마지막까지 받기
						info[infoi++] = buf[ini++];				
					}
					info[infoi] = '\0';
					lstrcpy(tgSn, info);																													//*SN(자리번호) 꺼내기
					turn = 0;
					break;
				}
				i++;
			}

			lstrcpy(deletemsg, "71");
			//lstrcpy(deletemsg, "70");		
			nReturn = send(clientsock, deletemsg, sizeof(deletemsg), 0);															//탈퇴 결과 보내기
		}
		//수신한 모든 메시지 에디트박스에 띄우기
			sprintf_s(strTemp, "수신한 메시지:%s : %d", buf, clientsock);	
			hChatEdit = GetDlgItem(hWndDlg, IDC_CHATSERVERLIST);
			int len = GetWindowTextLength(hChatEdit);
			SendDlgItemMessage(hWndDlg, IDC_CHATSERVERLIST, EM_SETSEL, (WPARAM)len, (LPARAM)len);
			SendDlgItemMessage(hWndDlg, IDC_CHATSERVERLIST, EM_REPLACESEL, FALSE, (LPARAM)"\r\n");
			SendDlgItemMessage(hWndDlg, IDC_CHATSERVERLIST, EM_REPLACESEL, FALSE, (LPARAM)strTemp);		
	}
	return 0;
}
//연결 스레드
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
			SendDlgItemMessage(hWndDlg, IDC_CHATSERVERLIST, EM_REPLACESEL, FALSE, (LPARAM)"손님이  입장하였습니다.");
			clientsockar[csi] = clientsock;
			g_bClientThreadExitFlags[csi] = FALSE;
			hClientThread[csi] = CreateThread(NULL, 0, ClientThreadFunc, (LPVOID)csi, 0, &ClientThreadID);
			csi++;
		}
	}
	return 0;
}
////대화상자 컨트롤들의 동작 수행
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
				MessageBox(hDlg, "영업시작", "server", MB_OK);
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
