

//할것

#include <Windows.h>
#include<stdio.h>
#include<CommCtrl.h>
#include <sql.h>
#include <sqlext.h>
#include "resource.h"
#define ID_E_CHAT 101			//프로시저의 채팅 에디트 식별자
#define ID_B_LOGOUT 102	//프로시저의 나가기 버튼 식별자
#define ID_B_DELETE 103		//프로시저의 탈퇴 버튼 식별자
#define ID_E_TIMER 104		//프로시저의 타이머 에디트 식별자
#define ID_E_IP 105				//연결의 IP 에디트 식별자
#define ID_E_PORT 106			//연결의 PORT 에디트 식별자
#define ID_B_CONNECT 107	//연결의 연결 버튼 식별자
#define ID_E_ID 108				//가입/수정의 아이디 에디트 식별자
#define ID_E_PW 109			//가입/수정의 비번 에디트 식별자
#define ID_E_NAME 110		//가입/수정의 이름 에디트 식별자
#define ID_E_PN 111				//가입/수정의 전화번호 에디트 식별자
#define ID_E_ADDR 112			//가입/수정의 주소 에디트 식별자
#define ID_E_BIRTH 113		//가입/수정의 생일 에디트 식별자
#define ID_B_JOIN 114			//가입/수정의 가입 버튼 식별자
#define ID_B_UPDATE 115		//가입/수정의 수정 버튼 식별자
#define ID_E_LOGIN_ID 116//로그인의 아이디 에디트 식별자
#define ID_E_LOGIN_PW 117//로그인의 비번 에디트 식별자
#define ID_B_LOGIN 118		//로그인의 로그인 버튼 식별자
#define ID_E_SN 119				//자리의 자리번호 에디트 식별자
#define ID_B_SELSEAT 120	//자리의 자리번호선택 버튼 식별자
#define ID_L_ITEM 121			//주문의 아이템리스트 리스트박스 식별자
#define ID_E_ORDERCOUNT 122	//주문의 아이템주문갯수 에디트 식별자
#define ID_B_ORDER 123				//주문의 아이템 주문 버튼 식별자
#define ID_S_IP 124						//연결의 IP 스태틱 식별자
#define ID_S_PORT 125					//연결의 PORT 스태틱 식별자
#define ID_S_ID 126						//가입/수정의 아이디 스태틱 식별자
#define ID_S_PW 127					//가입/수정의 비번 스태틱 식별자
#define ID_S_NAME 128				//가입/수정의 이름 스태틱 식별자
#define ID_S_PN 129						//가입/수정의 전화번호 스태틱 식별자
#define ID_S_ADDR 130				//가입/수정의 주소 스태틱 식별자
#define ID_S_BIRTH 131				//가입/수정의 생일 스태틱 식별자
#define ID_S_LOGIN_ID 132		//로그인의 아이디 스태틱 식별자
#define ID_S_LOGIN_PW 133		//로그인의 비번 스태틱 식별자
#define ID_S_SN 134					//자리의 자리번호 스태틱 식별자
#define ID_S_ITEM 135				//주문의 아이템 스태틱 식별자
#define ID_S_ORDERCOUNT 136	//주문의 아이템주문갯수 스태틱 식별자
#define ID_S_CHAT 137				//프로시저의 채팅 스태틱 식별자
#define ID_S_TIMER 138				//프로시저의 타이머 스태틱 식별자
HWND hWndMain;		// 메인 화면 핸들
HWND hWnd_C;		// 연결 화면 핸들
HWND hWnd_JU;		// 가입/수정 화면 핸들
HWND hWnd_L;		// 로그인 화면 핸들
HWND hWnd_S;		// 자리선택 화면 핸들
HWND hWnd_O;		// 주문 화면 핸들
LPCTSTR C_Class = TEXT("연결");					//연결 제목
LPCTSTR JU_Class = TEXT("회원가입/수정");//가입/수정 제목
LPCTSTR L_Class = TEXT("로그인");				//로그인 제목
LPCTSTR S_Class = TEXT("자리선택");			//자리선택 제목
LPCTSTR O_Class = TEXT("주문");					//주문 제목
//핸들
HWND hEdit_Chat;									//프로시저의 채팅 에디트
HWND hButton_Logout;							//프로시저의 로그아웃 버튼
HWND hButton_Delete;							//프로시저의 탈퇴 버튼
HWND hEdit_Timer;								//프로시저의 탈퇴 에디트
HWND hEdit_Ip;										//연결의 IP 에디트
HWND hEdit_Port;									//연결의 PORT 에디트
HWND hButton_Connect;						//연결의 연결 버튼
HWND hEdit_Id;										//가입/수정의 아이디 에디트
HWND hEdit_Pw;										//가입/수정의 비번 에디트
HWND hEdit_Name;								//가입/수정의 이름 에디트
HWND hEdit_Pn;										//가입/수정의 전화번호 에디트
HWND hEdit_Addr;									//가입/수정의 주소 에디트
HWND hEdit_Birth;									//가입/수정의 생일 에디트
HWND hButton_Join;								//가입/수정의 가입 버튼
HWND hButton_Update;							//가입/수정의 수정 버튼
HWND hEdit_Login_Id;							//로그인의 아이디 에디트
HWND hEdit_Login_Pw;							//로그인의 비번 에디트
HWND hButton_Login;								//로그인의 로그인 버튼
HWND hEdit_Sn;										//자리의 자리번호 에디트
HWND hButton_SelSeat;						//자리의 자리번호선택 버튼
HWND hListBox_ItemList;						//주문의 아이템리스트 리스트박스
HWND hEdit_OrderCount;						//주문의 아이템주문갯수 에디트
HWND hButton_Order;							//주문의 주문 버튼
HWND hStatic_Ip;									//연결의 IP 스태틱
HWND hStatic_Port;								//연결의 PORT 스태틱
HWND hStatic_Id;									//가입/수정의 아이디 스태틱
HWND hStatic_Pw;									//가입/수정의 비번 스태틱
HWND hStatic_Name;								//가입/수정의 이름 스태틱
HWND hStatic_Addr;								//가입/수정의 주소 스태틱
HWND hStatic_Birth;								//가입/수정의 생일 스태틱
HWND hStatic_Pn;									//가입/수정의 전화번호 스태틱
HWND hStatic_Login_Id;						//로그인의 아이디 스태틱
HWND hStatic_Login_Pw;						//로그인의 비번 스태틱
HWND hStatic_Sn;									//자리의 자리번호 스태틱
HWND hStatic_Item;								//주문의 아이템 스태틱
HWND hStatic_OrderCount;					//주문의 아이템주문갯수 스태틱
HWND hStatic_Chat;								//프로시저의 채팅 스태틱
HWND hStatic_Timer;							//프로시저의 타이머 스태틱
//프로시저함수들
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);															//기본화면
LRESULT CALLBACK ConnectProc(HWND, UINT, WPARAM, LPARAM);														//연결화면
LRESULT CALLBACK Join_Or_UpdateProc(HWND, UINT, WPARAM, LPARAM);										//가입/수정
LRESULT CALLBACK LoginProc(HWND, UINT, WPARAM, LPARAM);															//로그인
LRESULT CALLBACK SelSeatProc(HWND, UINT, WPARAM, LPARAM);														//자리선택
LRESULT CALLBACK OrderProc(HWND, UINT, WPARAM, LPARAM);														//주문
//BOOL CALLBACK JoinDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);			
//BOOL CALLBACK LoginDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
//BOOL CALLBACK SelSeatDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
//BOOL CALLBACK OrderDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("PC방 클라이언트");																										//윈도우 프로시저 타이틀
////IP와 포트번호 설정
const char g_szlpAddress[17] = "127.0.0.1";
const unsigned short g_uPort = 7878;
TCHAR tgIp[17] = "127.0.0.1", tgPort[10] = "7878";																									//IP와 PORT번호
TCHAR tgId[10], tgName[10], tgPw[10],tgPn[10], tgAddr[10], tgBirth[10];										//아이디,이름,비번,전화번호,주소,생일 문자열 변수들
//TCHAR tgNick[10],TCHAR tgPass[10];
int nip, nport;																																	//IP와 PORT 연결 설정

HWND hChatEdit;																		

int chknickmsg = 0;

BOOL seljoinorloginordel = 0;
TCHAR selnum[10];

TCHAR nickandmsg[100];

DWORD ThreadID;																																					//스레드아이디
HANDLE hThread;																																					//스레드 핸들

HWND hWndDlg;

TCHAR tgmessage[30];
TCHAR tgcmdserver[200];																																		//서버와 통신하는 커멘드 문자열
TCHAR tgjoinloginmsg[100]="ID";
int nReturn;																																								//통신 결과 
WSADATA wsadata;																																				//소켓 연결
SOCKET clientsock;																																					//소켓번호 담기
sockaddr_in addr_client;																																			//소켓번호 담기
int addrlen_clt = sizeof(sockaddr);																															//소켓번호 담기
char buf[1024];																																						////메시지																						
char strTemp[1024];																																				//메세지를 채팅창에 수신한 메세지로 변환

//팀플 //주요변수
int turn = 0;																																								//유저의 정보를 순서대로 가져오게 하는 순서 제어
int ini;
TCHAR info[200];																																					//커멘드에서 요소를 뽑는 문자열
int infoi = 0;																																								//커멘드에서 요소를 뽑는 문자열의 인덱스
TCHAR usersn[10];																																					//로그인된 유저의 자리번호
TCHAR userpw[10];																																					//로그인된 유저의 비번
int pctimei;																																								//로그인된 유저의 시간 인덱스
int ngitemcount;																																						//아이템주문갯수 
TCHAR tgselitemi[10];																																				//아이템주문갯수 문자열
int selitemi;																																								//아이템주문갯수
int chkseat=0;																																							//자리선택 체크
int chkjoin = 0;																																							//가입 체크
int chklogin = 0;																																						//로그인 체크
int i;	
int pctime;																																								//로그인된 유저의 시간
int chkexit;																																								//로그아웃 체크
int userexit;
int chktimer;																																							//타이머 제어(1이면 실행/0이면 종료)
int pci = 0;																																								
TCHAR ordermsg[20];																																				
TCHAR tgitem[10] = "";																																			//아이템 문자열
TCHAR tgitemcount[10] = "";																																	//아이템 문자열 갯수 문자열
TCHAR tgSeatnum[10];																																			//자리번호 문자열
TCHAR tgnum[10];																																					//신청하는 자리번호 문자열
TCHAR userid[10];																																					//로그인 된 유저 아이디 문자열
TCHAR exitmsg[20];																									
TCHAR pcbuf[10];
TCHAR orderbuf[10];																																				//주문요소정보 담기
TCHAR itembuf[10];
TCHAR pctimebuf[10];																																				//유저 시간 담아오기
TCHAR exitclient[10];
TCHAR tpctime[10];																																					//유저시간 문자열
int pbi = 0;
int orderi = 0;																																							//주문요소정보 인덱스
int itemi = 0;																																								//상품요소정보 인덱스
int chkendbtn = 0;																																					//로그아웃/탈퇴 체크


//DB변수들
SQLHENV hEnv;
SQLHDBC hDbc;
SQLHSTMT hStmt;


DWORD WINAPI ThreadFunc(LPVOID Param) {//팀플//리시브스레드로써 요청의 결과를 다룸
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
			orderbuf[orderi] = '\0';												//팀플//주문을 담기
			if (buf[0] == '1') {
				if (lstrcmp(buf, TEXT("11")) == 0) {
					chkseat = 1;																	//팀플//자리
					lstrcpy(tgSeatnum, tgnum);										//자리번호 담기
					lstrcpy(usersn, tgSeatnum);										//유저의 자리번호 담기
					wsprintf(buf, "%s", "자리사용가능");
				}
				else wsprintf(buf, "%s", "자리사용불가");						//서버로부터 "10" 받았을때
			}
			else if (buf[0] == '3') {
				 if (lstrcmp(buf, TEXT("31")) == 0) {								//팀플//회원가입
					chkjoin = 1;
					wsprintf(buf, "%s", "회원가입성공");
				}
				 else wsprintf(buf, "%s", "회원가입실패");						//서버로부터 "30" 받았을때
			}
			else if (buf[0] == '4') {
				if (buf[1]=='1') {					//팀플//로그인되면 남은시간처리되고 아이디저장			//로그인실패 1.아이디/비번오류,2.남은 시간 0이하
					pctimei = 0;
					for (int i = 2; i < 4; i++) {
						pctimebuf[pctimei++] = buf[i];	//팀플//갯수나 번호는 1이아닌 01형식
					}
					pctimebuf[pctimei] = '\0';								//팀플//남은시간 담기
					pctime = atoi(pctimebuf);
					if (pctime >= 0) {
						lstrcpy(userid, tgId);									//팀플//계정 아이디,비번 담기
						lstrcpy(userpw, tgPw);
						i = 4;																//로그인 커맨드의 가져올 정보의  시작점
						turn = -1;														//가져올 정보를 순서대로 가져오게 제어
						while (buf[i] != '\0') { //팀플//전체 CMD						
							infoi = 0;
							if (buf[i] == ':') {										//각 정보의 시작점으로 이동
								ini = i + 1;
								turn++;
							}
							if (turn == 0) {			
								while (buf[ini] != ':' && buf[ini] != '\0') {			//":"전까지 담기
									info[infoi++] = buf[ini++];
								}
								if (infoi >= 2) infoi -= 2;								//다음 정보 커맨드(PW,NAME..) 길이 만큼 빼기
								info[infoi] = '\0';
								lstrcpy(tgName, info); //팀플//NAME꺼내기
							}
							else if (turn == 1) {
								while (buf[ini] != ':' && buf[ini] != '\0') {
									info[infoi++] = buf[ini++];
								}
								if (infoi >= 4) infoi -= 4;
								info[infoi] = '\0';
								lstrcpy(tgPn, info); //팀플//PN꺼내기
							}
							else if (turn == 2) {
								while (buf[ini] != ':' && buf[ini] != '\0') {
									info[infoi++] = buf[ini++];
								}
								if (infoi >= 5) infoi -= 5;
								info[infoi] = '\0';
								lstrcpy(tgAddr, info); //팀플//ADDR꺼내기
							}
							else if (turn == 3) {
								while (buf[ini] != '\0') {										//마지막까지 담기
									info[infoi++] = buf[ini++];
								}
								info[infoi] = '\0';
								lstrcpy(tgBirth, info); //팀플//BIRTH꺼내기
								turn = 0;
								break;
							}
							i++;
						}

						i = 0;
						//팀플//가입/수정에 계정의 정보들 띄우기
						wsprintf(tpctime, "%d", pctime);
						SetWindowText(hEdit_Timer, tpctime);				//프로시저의 타이머 에디트에 유저 시간 띄우기
						SetWindowText(hEdit_Id, userid);						//가입/수정의 아이디에 띄우기
						SetWindowText(hEdit_Pw, userpw);					//..비번
						SetWindowText(hEdit_Name, tgName);				//..이름
						SetWindowText(hEdit_Pn, tgPn);							//..전화번호
						SetWindowText(hEdit_Addr, tgAddr);				//..주소
						SetWindowText(hEdit_Birth, tgBirth);				//..생일
						if (pctime > 0) {												//유저의 시간이 0 초과여야 로그인 가능
							chklogin = 1;
							chktimer = 1;
							wsprintf(buf, "%s", "로그인성공");	//팀플//로그인과 타이머 동작 시작
						}
						else wsprintf(buf, "%s", "로그인실패(잔여시간없음)");	//유저의 시간이 0이하일때
					}
				}
				else wsprintf(buf, "%s", "로그인실패");					//팀플//cmd를 40으로 받으면 실패
			}
			else if (buf[0] == '2') {
				if (buf[1] == '1') {
					if (lstrcmp(orderbuf, TEXT("211")) == 0) {			//팀플//음료상품구매처리
						wsprintf(buf, "%s", "DRINK구입성공");
					}
					else wsprintf(buf, "%s", "DRINK구입실패");			//서버로부터  "210" 받았을 때
				}
				else if (buf[1] == '2') {
					if (lstrcmp(orderbuf, TEXT("221")) == 0) {			//팀플//식품구매처리
						wsprintf(buf, "%s", "FOOD구입성공");
					}
					else wsprintf(buf, "%s", "FOOD구입실패");			//서버로부터  "220" 받았을 때
				}
				else if (buf[1] == '3') {
					if (lstrcmp(orderbuf, TEXT("231")) == 0) {			//팀플//피시상품구매처리
						wsprintf(buf, "%s", "PC구입성공");
					}
					else wsprintf(buf, "%s", "PC구입실패");				//서버로부터  "230" 받았을 때
				}
			}
			else if (buf[0] == '5') {														//팀플//로그인
				if (lstrcmp(buf, TEXT("51")) == 0) {
					lstrcpy(userid, "");
					wsprintf(buf, "%s", "로그아웃성공");
					chklogin = 0;
				}
				else 	wsprintf(buf, "%s", "로그아웃실패");				////서버로부터  "50" 받았을 때
			}
			else if (buf[0] == '6') {													//팀플//수정
				if (lstrcmp(buf, TEXT("61")) == 0) {
					wsprintf(buf, "%s", "수정성공");
				}
				else wsprintf(buf, "%s", "수정실패");						//서버로부터  "60" 받았을 때
			}
			else if (buf[0] == '7') {													//팀플//탈퇴
				if (lstrcmp(buf, TEXT("71")) == 0) {
					wsprintf(buf, "%s", "탈퇴성공");
					SendMessage(hWndDlg, WM_CLOSE, 0, 0);
				}
				else wsprintf(buf, "%s", "탈퇴실패");						//서버로부터  "70" 받았을 때
			}
			else {
				MessageBox(hWndMain, buf,"server수신", MB_OK);					/////////////////////////////////////////////////////////////////// 박정현 테스트 문단////////////////////////
				// 이쪽에 서버에서 보내준 데이터를 기반으로 앉을수 있는 좌석을 선별하고 선택할수있게 요청		
}
			if (lstrlen(buf) != 0) {//팀플//모든 메세지 나타내기
				sprintf_s(strTemp, "수신한 메시지:%s", buf);								//팀플//위에서 cmd를 변환한 메세지를 프로시저의 채팅 에디트에 띄우기
				int len = GetWindowTextLength(hEdit_Chat);
				SendMessage(hEdit_Chat, EM_SETSEL, (WPARAM)len, (LPARAM)len);
				SendMessage(hEdit_Chat, EM_REPLACESEL, FALSE, (LPARAM)"\r\n");
				SendMessage(hEdit_Chat, EM_REPLACESEL, FALSE, (LPARAM)strTemp);												//프로시저의 채팅 에디트 아래방향으로 작성 시키기
			}
		}
	}
	return 0;
}
//대화상자용//팀플//대화상자 컨트롤 동작//모든 요청다루기
//BOOL CALLBACK ClientDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
//	switch (iMessage) {
//	case WM_INITDIALOG:
//		//DBConnect();
//		hWndDlg = hDlg;
//		InitCommonControls();
//		SetDlgItemText(hDlg, IDC_IPCLIENTEDIT, tgIp);
//		SetDlgItemText(hDlg, IDC_PORTCLIENTEDIT, tgPort);//팀플//192.168.0.13
//		//lstrcpy(tpctime, "");
//		SetTimer(hDlg, 1, 1000, NULL);
//		SetTimer(hDlg, 2, 1000, NULL);
//		return TRUE;
//	case WM_TIMER:
//		switch (wParam) {
//		case 1:												//팀플//남은 시간 타이머
//			if (chktimer == 1) {
//				pctime -= 1;
//				wsprintf(tpctime, "%d", pctime);
//				SetDlgItemText(hDlg, IDC_PCTIMEEDIT, tpctime);
//				if (pctime == 0) {																				//팀플//시간 0일때 로그아웃처리
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
//		case 2:																//팀플//나가기버튼 눌렸을때/남은시간 0일때 남은시간 타이머 종료하고 다시 타이머 만들기(로그인을 계속 받기 위해)
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
//		case IDC_ENTERCLIENTBUTTON://팀플//소켓연결
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
//				SendDlgItemMessage(hWndDlg, IDC_CHATCLIENTLIST, EM_REPLACESEL, FALSE, (LPARAM)"입장하였습니다.");
//			}
//			break;
//			return TRUE;
//	
//		case IDC_GOJOINBUTTON://팀플//회원가입 전 자리먼저잡기
//			if (chkseat == 1) {
//				DialogBox(g_hInst, MAKEINTRESOURCE(IDD_JOINDIALOG), HWND_DESKTOP, (DLGPROC)JoinDlgProc);
//			}
//			else {
//				MessageBox(hDlg, "자리 선택 먼저 하세요!!", "오류", MB_OK);
//			}
//			return TRUE;
//		case IDC_GOLOGINBUTTON://팀플//로그인 전 자리와 로그인 체크하기
//			if (chkseat == 1 && chklogin==0) {
//				DialogBox(g_hInst, MAKEINTRESOURCE(IDD_LOGINDIALOG), HWND_DESKTOP, (DLGPROC)LoginDlgProc);
//			}
//			else {
//				if(chkseat==0)MessageBox(hDlg, "자리 선택 먼저 하세요!!", "오류", MB_OK);
//				if(chklogin==1)MessageBox(hDlg, "로그인 되어 있습니다!!", "오류", MB_OK);
//			}
//			return TRUE;
//		case IDC_GOORDERBUTTON://팀플//주문 전 자리 잡기
//			if (chkseat == 1) {
//				DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ORDERDIALOG), HWND_DESKTOP, (DLGPROC)OrderDlgProc);
//			}
//			else {
//				MessageBox(hDlg, "자리 선택 먼저 하세요!!", "오류", MB_OK);
//			}
//			return TRUE;
//		case IDC_SEATBUTTON:
//			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_SELSEATDIALOG), HWND_DESKTOP, (DLGPROC)SelSeatDlgProc);
//			return TRUE;
//		case IDC_EXITBUTTON://팀플//나가기 전 로그인 체크하기
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
//			else MessageBox(hDlg, "로그인하세요!!", "오류", MB_OK);
//			return TRUE;
//		case IDC_DELBUTTON://팀플//탈퇴 전 로그인 체크하기
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
//			else MessageBox(hDlg, "로그인하세요!!", "오류", MB_OK);
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
//팀플//서버에 로그인과 회원가입 요청
//BOOL CALLBACK LoginDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
//	switch (iMessage) {
//	case WM_INITDIALOG:
//		//DBConnect();
//		InitCommonControls();
//		return TRUE;
//	case WM_COMMAND:
//		switch (LOWORD(wParam)) {
//		case IDC_LOGINBUTTON:		//팀플//로그인:아이디,비번 입력(다 입력 가능하지만 ':'은 입력 못 받게 하기)
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
////팀플//서버에 로그인과 회원가입 요청
//BOOL CALLBACK JoinDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
//	switch (iMessage) {
//	case WM_INITDIALOG:
//		//DBConnect();
//		InitCommonControls();
//		SetDlgItemText(hWndDlg, IDC_PCTIMEEDIT, tpctime);		//팀플//로그인할때 해당 정보 띄우기
//		SetDlgItemText(hWndDlg, IDC_IDJOINEDIT, userid);
//		SetDlgItemText(hWndDlg, IDC_PASSJOINEDIT, userpw);
//		SetDlgItemText(hWndDlg, IDC_NICKEDIT, tgNick);
//		SetDlgItemText(hWndDlg, IDC_PNEDIT, tgPn);
//		SetDlgItemText(hWndDlg, IDC_ADDREDIT, tgAddr);
//		SetDlgItemText(hWndDlg, IDC_BIRTHEDIT, tgBirth);
//		return TRUE;
//	case WM_COMMAND:
//		switch (LOWORD(wParam)) {
//		case IDC_JOINBUTTON:		//팀플//회원가입:아이디,닉네임,비번,전화번호,주소,생일 다 입력/(다 입력 가능하지만 ':'은 입력 못 받게 하기)
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
//		case IDC_UPDATEBUTTON:		//팀플//수정:아이디,닉네임,비번,전화번호,주소,생일 다 입력/(다 입력 가능하지만 ':'은 입력 못 받게 하기)(로그인 전에도 가능하게 하기(비번찾기 기능))
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
////팀플//자리 선택
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
//			GetDlgItemText(hDlg, IDC_SEATEDIT, tgnum, sizeof(tgnum));						//팀플//자리 선택(02등 2자리 형식으로)
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
////팀플//주문
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
//				selitemi = SendDlgItemMessage(hDlg, IDC_ITEMLIST, LB_GETCURSEL, 0, 0);				//팀플//주문 상품 선택(리스트박스의 인덱스로 1자리 형식으로)
//				selitemi += 1;
//				wsprintf(tgselitemi, "%d", selitemi);
//				//SendDlgItemMessage(hDlg, IDC_ITEMLIST, LB_GETTEXT, i, (LPARAM)tgitem);
//			}
//			break;
//			return TRUE;
//		case IDOK:																							//팀플//주문 수량 선택(02등 2자리 형식으로)
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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);					//윈도우 프로시저에 메뉴 달기
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

void Init_Wnd(WNDCLASS* Wnd, int Proc_No) {													//윈도우프로시저에서 띄울 다른 프로시저 설정
	if (Proc_No > 4) {
		MessageBox(hWndMain, "윈도우 초기화 오류!", "알림", MB_OK);
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
		Wnd->lpfnWndProc = ConnectProc;																	//연결화면
		Wnd->lpszClassName = C_Class;
		break;
	case 1:																													//가입/수정화면
		Wnd->lpfnWndProc = Join_Or_UpdateProc;
		Wnd->lpszClassName = JU_Class;
		break;
	case 2:																													//로그인화면
		Wnd->lpfnWndProc = LoginProc;
		Wnd->lpszClassName = L_Class;
		break;
	case 3:																													//자리선택화면
		Wnd->lpfnWndProc = SelSeatProc;
		Wnd->lpszClassName = S_Class;
		break;
	case 4:																													//주문화면
		Wnd->lpfnWndProc = OrderProc;
		Wnd->lpszClassName = O_Class;
		break;
	}
	Wnd->lpszMenuName = NULL;																				//띄울 다른 프로시저에 메뉴는 없음
	Wnd->style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(Wnd);
}
void CLIENT_Open() {																								//소켓 연결 함수
	GetWindowText(hEdit_Ip, tgIp, sizeof(tgIp));												//연결의 IP에디트에서 내용 가져오기
	GetWindowText(hEdit_Port, tgPort, sizeof(tgPort));										//연결의 PORT에디트에서 내용 가져오기
	nip = atoi(tgIp);
	nport = atoi(tgPort);
	nReturn = WSAStartup(WORD(2.0), &wsadata);
	clientsock = socket(AF_INET, SOCK_STREAM, 0);
	addr_client.sin_family = AF_INET;
	addr_client.sin_addr.s_addr = inet_addr(tgIp);
	addr_client.sin_port = htons(nport);
	hThread = CreateThread(NULL, 0, ThreadFunc, &clientsock, 0, &ThreadID);						//연결스레드 만들기
	nReturn = connect(clientsock, (sockaddr*)&addr_client, addrlen_clt);
	if (!nReturn) {
		int len = GetWindowTextLength(hEdit_Chat);
		SendMessage(hEdit_Chat, EM_SETSEL, (WPARAM)len, (LPARAM)len);
		SendMessage(hEdit_Chat, EM_REPLACESEL, FALSE, (LPARAM)TEXT("\r\n"));
		SendMessage(hEdit_Chat, EM_REPLACESEL, FALSE, (LPARAM)TEXT("입장하였습니다."));						//연결했으면 프로시저의 채팅 에디트에 "연결" 메세지 띄우기
	}
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		InitCommonControls();

		//띄울 다른 화면들
		WNDCLASS Wnd_C;				//연결
		WNDCLASS Wnd_JU;				//가입/수정
		WNDCLASS Wnd_L;				//로그인
		WNDCLASS Wnd_S;				//자리선택
		WNDCLASS Wnd_O;				//주문

		// 연결윈도우 설정
		Init_Wnd(&Wnd_C, 0);
		// 가입/수정 윈도우 설정
		Init_Wnd(&Wnd_JU, 1);
		// 로그인윈도우 설정
		Init_Wnd(&Wnd_L, 2);
		// 자리윈도우 설정
		Init_Wnd(&Wnd_S, 3);
		// 주문윈도우 설정
		Init_Wnd(&Wnd_O, 4);

		//채팅 에디트
		hEdit_Chat = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL| ES_MULTILINE,			
			10, 100, 500, 400, hWnd, (HMENU)ID_E_CHAT, g_hInst, NULL);									
		//로그아웃 버튼
		hButton_Logout = CreateWindow(TEXT("button"), TEXT("LOGOUT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			610, 50, 200, 65, hWnd, (HMENU)ID_B_LOGOUT, g_hInst, NULL);
		//탈퇴 버튼
		hButton_Delete = CreateWindow(TEXT("button"), TEXT("DELETE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 50, 200, 65, hWnd, (HMENU)ID_B_DELETE, g_hInst, NULL);
		//타이머 에디트
		hEdit_Timer = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			610, 200, 100, 100, hWnd, (HMENU)ID_E_TIMER, g_hInst, NULL);
		//채팅 스태틱
		hStatic_Chat = CreateWindow(TEXT("static"), TEXT("STATE"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 0, 60, 20, hWnd, (HMENU)ID_S_CHAT, g_hInst, NULL);
		//타이머 스태틱
		hStatic_Timer = CreateWindow(TEXT("static"), TEXT("TIME"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			610, 150, 40, 20, hWnd, (HMENU)ID_S_TIMER, g_hInst, NULL);
		//타이머
		SetTimer(hWnd, 1, 1000, NULL);			//로그인 유저 시간 타이머
		SetTimer(hWnd, 2, 1000, NULL);			//로그인 유저 시간 타이머 종료하는 타이머
		return 0;
	case WM_TIMER:
		switch (wParam) {
		case 1:												//팀플//남은 시간 타이머
			if (chktimer == 1) {
				pctime -= 1;										//남은시간 줄이기
				wsprintf(tpctime, "%d", pctime);
				SetWindowText(hEdit_Timer, tpctime);															//프로시저의 타이머 에디트에 유저 남은 시간 출력
				if (pctime == 0) {																				//팀플//시간 0일때 로그아웃처리
					wsprintf(tpctime, "%d", pctime);
					SetWindowText(hEdit_Timer, tpctime);
					lstrcpy(tgcmdserver, "");
					lstrcpy(tgcmdserver, "5ID:");
					lstrcat(tgcmdserver, userid);
					lstrcat(tgcmdserver, "PW:");
					lstrcat(tgcmdserver, userpw);
					lstrcat(tgcmdserver, "SN:");
					lstrcat(tgcmdserver, usersn);
					chkendbtn = 1;																				//로그아웃/탈퇴 처리로 
					chklogin = 0;																					//로그아웃 된 상태로
					nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);
					chktimer = 0;																				//타이머 끄는 상태로
					break;
				}
			}
			break;
		case 2:																//팀플//나가기버튼 눌렸을때/남은시간 0일때 남은시간 타이머 종료하고 다시 타이머 만들기(로그인을 계속 받기 위해)
			if (chkendbtn == 1) {
				KillTimer(hWnd, 1);											//타이머 끄기
				chktimer = 0;													//타이머 끄는 상태로
				SetTimer(hWnd, 1, 1000, NULL);					//유저 남은 시간 타이머 다시 생성
				chkendbtn = 0;												//로그아웃 체크 다시 원래 대로 초기화
				pctime = 0;
				wsprintf(tpctime, "%d", pctime);
				SetWindowText(hEdit_Timer, tpctime); //프로시저의 타이머 에디트에 유저 남은 시간 0으로  출력
			}
			break;
		}
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case ID_M_CONNECT:									//연결 오픈
			hWnd_C = CreateWindow(C_Class, C_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
			ShowWindow(hWnd_C, SW_SHOW);
			break;
		case ID_M_JOIN_OR_UPDATE:		// 가입/수정 오픈
			if (chkseat == 1) {							//자리 선택 수행했을때
				hWnd_JU = CreateWindow(JU_Class, JU_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
				ShowWindow(hWnd_JU, SW_SHOW);
			}
			else MessageBox(hWnd, "자리 선택 먼저 하세요!!", "오류", MB_OK);
			break;
		case ID_M_LOGIN:		// 로그인 오픈
			if (chkseat == 1 && chklogin == 0) {//자리선택과 로그인 수행했을때
				hWnd_L = CreateWindow(L_Class, L_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
				ShowWindow(hWnd_L, SW_SHOW);
			}
			else {
				if (chkseat == 0)MessageBox(hWnd, "자리 선택 먼저 하세요!!", "오류", MB_OK);
				if (chklogin == 1)MessageBox(hWnd, "로그인 되어 있습니다!!", "오류", MB_OK);
			}
			break;
		case ID_M_SELSEAT:		// 자리선택 오픈
			hWnd_S = CreateWindow(S_Class, S_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
			ShowWindow(hWnd_S, SW_SHOW);
			break;
		case ID_M_ORDER:		// 주문 오픈
			if (chkseat == 1) {			//자리선택했을때
				hWnd_O = CreateWindow(O_Class, O_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
				ShowWindow(hWnd_O, SW_SHOW);
			}
			else MessageBox(hWnd, "자리 선택 먼저 하세요!!", "오류", MB_OK);
			break;
		case ID_B_LOGOUT:							//로그아웃
			if (chklogin == 1) {							//로그인 했을때
				lstrcpy(tgcmdserver, "");
				lstrcpy(tgcmdserver, "5ID:");
				lstrcat(tgcmdserver, userid);
				lstrcat(tgcmdserver, "PW:");
				lstrcat(tgcmdserver, userpw);
				lstrcat(tgcmdserver, "SN:");
				lstrcat(tgcmdserver, usersn);
				chkendbtn = 1;											//로그아웃/탈퇴 수행 했을때
				nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);					//서버로 로그아웃 커멘드 보내기
				pctime = 0;										
				wsprintf(tpctime, "%d", pctime);
				SetWindowText(hEdit_Timer, tpctime);																	//프로시저의 타이머 에디트에 유저 남은 시간 0으로 출력
			}
			else MessageBox(hWnd, "로그인하세요!!", "오류", MB_OK);
			break;
		case ID_B_DELETE:																			//탈퇴 수행
			if (chklogin == 1) {													//로그인 했을때
				lstrcpy(tgcmdserver, "");
				lstrcpy(tgcmdserver, "7ID:");
				lstrcat(tgcmdserver, userid);
				lstrcat(tgcmdserver, "PW:");
				lstrcat(tgcmdserver, userpw);
				lstrcat(tgcmdserver, "SN:");
				lstrcat(tgcmdserver, usersn);
				chkendbtn = 1;											//로그아웃/탈퇴 수행 했을때
				nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);						//서버로 탈퇴 커맨드 보내기
				pctime = 0;
				wsprintf(tpctime, "%d", pctime);
				SetWindowText(hEdit_Timer, tpctime);												//프로시저의 타이머 에디트에 유저 남은 시간 0으로 출력
			}
			else MessageBox(hWnd, "로그인하세요!!", "오류", MB_OK);
			break;
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
LRESULT CALLBACK ConnectProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {					//연결화면
	switch (iMessage) {
	case WM_CREATE:
		//아이피 스태틱
		hStatic_Ip = CreateWindow(TEXT("static"), TEXT("IP"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 0, 20, 20, hWnd, (HMENU)ID_S_IP, g_hInst, NULL);
		//아이피 에디트
		hEdit_Ip = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 50, 200, 50, hWnd, (HMENU)ID_E_IP, g_hInst, NULL);
		//포트 스태틱
		hStatic_Port = CreateWindow(TEXT("static"), TEXT("PORT"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			310, 0, 40, 20, hWnd, (HMENU)ID_S_PORT, g_hInst, NULL);
		//포트 에디트
		hEdit_Port = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			310, 50, 200, 50, hWnd, (HMENU)ID_E_PORT, g_hInst, NULL);
		//연결 버튼
		hButton_Connect = CreateWindow(TEXT("button"), TEXT("CONNECT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			310, 200, 400, 100, hWnd, (HMENU)ID_B_CONNECT,g_hInst, NULL);
		//연결의 IP와 PORT에디트에 띄우기
		SetWindowText(hEdit_Ip, tgIp);
		SetWindowText(hEdit_Port, tgPort);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_B_CONNECT:					//연결의 연결 버튼 눌렸을때 연결 수행
			CLIENT_Open();
			break;
		}
		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
LRESULT CALLBACK Join_Or_UpdateProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {									//가입/수정
	switch (iMessage) {
	case WM_CREATE:
		//아이디 스태틱
		hStatic_Id = CreateWindow(TEXT("static"), TEXT("ID"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 0, 40, 20, hWnd, (HMENU)ID_S_ID, g_hInst, NULL);
		//아이디 에디트
		hEdit_Id = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 50, 100, 50, hWnd, (HMENU)ID_E_ID, g_hInst, NULL);
		//비번 스태틱
		hStatic_Pw = CreateWindow(TEXT("static"), TEXT("PW"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 150, 40, 20, hWnd, (HMENU)ID_S_PW, g_hInst, NULL);
		//비번 에디트
		hEdit_Pw = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 200, 100, 50, hWnd, (HMENU)ID_E_PW, g_hInst, NULL);
		//이름 스태틱
		hStatic_Name = CreateWindow(TEXT("static"), TEXT("NAME"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 300, 40, 20, hWnd, (HMENU)ID_S_NAME, g_hInst, NULL);
		//이름 에디트
		hEdit_Name = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 350, 100, 50, hWnd, (HMENU)ID_E_NAME, g_hInst, NULL);
		//전화번호 스태틱
		hStatic_Pn = CreateWindow(TEXT("static"), TEXT("PN"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 450, 40, 20, hWnd, (HMENU)ID_S_PN, g_hInst, NULL);
		//전화번호 에디트
		hEdit_Pn = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 500, 100, 50, hWnd, (HMENU)ID_E_PN, g_hInst, NULL);
		//주소 스태틱
		hStatic_Addr = CreateWindow(TEXT("static"), TEXT("ADDR"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			510, 0, 40, 20, hWnd, (HMENU)ID_S_ADDR, g_hInst, NULL);
		//주소 에디트
		hEdit_Addr = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			510, 50, 100, 50, hWnd, (HMENU)ID_E_ADDR, g_hInst, NULL);
		//생일 스태틱
		hStatic_Birth = CreateWindow(TEXT("static"), TEXT("BIRTH"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			510, 150, 50, 20, hWnd, (HMENU)ID_S_BIRTH, g_hInst, NULL);
		//생일 에디트
		hEdit_Birth = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			510, 200, 100, 50, hWnd, (HMENU)ID_E_BIRTH, g_hInst, NULL);
		//가입 버튼
		hButton_Join = CreateWindow(TEXT("button"), TEXT("JOIN"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 10, 300, 200, hWnd, (HMENU)ID_B_JOIN, g_hInst, NULL);
		//수정 버튼
		hButton_Update = CreateWindow(TEXT("button"), TEXT("UPDATE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 300, 300, 200, hWnd, (HMENU)ID_B_UPDATE, g_hInst, NULL);
		//로그인 된 유저 정보 띄우기
		SetWindowText(hEdit_Id, userid);						//아이디
		SetWindowText(hEdit_Pw, userpw);					//비번
		SetWindowText(hEdit_Name, tgName);				//이름
		SetWindowText(hEdit_Pn, tgPn);							//전화번호
		SetWindowText(hEdit_Addr, tgAddr);				//주소
		SetWindowText(hEdit_Birth, tgBirth);				//생일
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_B_JOIN:											//가입 버튼 눌렸을때
			//커맨드,계정 정보 문자열 빈칸으로 초기화
			lstrcpy(tgcmdserver, "");			//서버와의 커맨드
			lstrcpy(tgId, "");						//아이디
			lstrcpy(tgName, "");					//이름
			lstrcpy(tgPw, "");						//비번
			lstrcpy(tgPn, "");						//전화번호
			lstrcpy(tgAddr, "");					//주소
			lstrcpy(tgBirth, "");					//생일
			//계정 정보 에디트에서 정보 문자열에 담기
			GetWindowText(hEdit_Id, tgId, sizeof(tgId));					//아이디
			GetWindowText(hEdit_Name,tgName, sizeof(tgName));		//이름
			GetWindowText(hEdit_Pw,  tgPw, sizeof(tgPw));					//비번
			GetWindowText(hEdit_Pn,  tgPn, sizeof(tgPn));					//전화번호
			GetWindowText(hEdit_Addr, tgAddr, sizeof(tgAddr));		//주소
			GetWindowText(hEdit_Birth, tgBirth, sizeof(tgBirth));		//생일
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
			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);								//서버로 가입 커맨드 보내기
			break;
		case ID_B_UPDATE:																				//수정 버튼 눌렸을때
			//커맨드,계정 정보 문자열 빈칸으로 초기화
			lstrcpy(tgcmdserver, "");					//커맨드
			lstrcpy(tgId, "");								//아이디
			lstrcpy(tgName, "");							//이름
			lstrcpy(tgPw, "");								//비번
			lstrcpy(tgPn, "");								//전화번호
			lstrcpy(tgAddr, "");							//주소
			lstrcpy(tgBirth, "");							//생일
			//계정 정보 에디트에서 정보 문자열에 담기
			GetWindowText(hEdit_Id, tgId, sizeof(tgId));								//아이디
			GetWindowText(hEdit_Name, tgName, sizeof(tgName));				//이름
			GetWindowText(hEdit_Pw, tgPw, sizeof(tgPw));								//비번
			GetWindowText(hEdit_Pn, tgPn, sizeof(tgPn));								//전화번호
			GetWindowText(hEdit_Addr, tgAddr, sizeof(tgAddr));					//주소
			GetWindowText(hEdit_Birth, tgBirth, sizeof(tgBirth));					//생일
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
			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);												//서버로 수정 커맨드 보내기
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
		//로그인 아이디 스태틱
		hStatic_Login_Id = CreateWindow(TEXT("static"), TEXT("ID"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 0, 40, 20, hWnd, (HMENU)ID_S_LOGIN_ID, g_hInst, NULL);
		//로그인 아이디 에디트
		hEdit_Login_Id = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 50, 100, 50, hWnd, (HMENU)ID_E_LOGIN_ID, g_hInst, NULL);
		//로그인 비번 스태틱
		hStatic_Login_Pw = CreateWindow(TEXT("static"), TEXT("PW"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 150, 40, 20, hWnd, (HMENU)ID_S_LOGIN_PW, g_hInst, NULL);
		//로그인 비번 에디트
		hEdit_Login_Pw = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 200, 100, 50, hWnd, (HMENU)ID_E_LOGIN_PW, g_hInst, NULL);
		//로그인 버튼
		hButton_Login = CreateWindow(TEXT("button"), TEXT("LOGIN"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 10, 200, 50, hWnd, (HMENU)ID_B_LOGIN, g_hInst, NULL);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_B_LOGIN:									//로그인 버튼 눌렸을때
			//커맨드와 정보 문자열 빈칸으로 초기화
			lstrcpy(tgcmdserver, "");				//커맨드
			lstrcpy(tgId, "");							//아이디
			lstrcpy(tgPw, "");							//비번
			//로그인 정보 에디트에서 정보 문자열 담기
			GetWindowText(hEdit_Login_Id, tgId, sizeof(tgId));		//로그인 아이디
			GetWindowText(hEdit_Login_Pw, tgPw, sizeof(tgPw));	//로그인 비번
			lstrcpy(tgcmdserver, "4ID:");
			lstrcat(tgcmdserver, tgId);
			lstrcat(tgcmdserver, "PW:");
			lstrcat(tgcmdserver, tgPw);
			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);							//서버에 로그인 커맨드 보내기
			break;
		}
		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
LRESULT CALLBACK SelSeatProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {						//자리선택
	switch (iMessage) {
	case WM_CREATE:
		nReturn = send(clientsock, "S-", sizeof("S-"), 0);			///////////////////////////////////// 박정현 테스트//////////////////////////////////////
		//자리번호 스태틱
		hStatic_Sn = CreateWindow(TEXT("static"), TEXT("SN(2자리)"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 0, 80, 20, hWnd, (HMENU)ID_S_SN, g_hInst, NULL);
		//자리번호 에디트
		hEdit_Sn = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 50, 100, 50, hWnd, (HMENU)ID_E_SN, g_hInst, NULL);
		//자리선택 버튼
		hButton_SelSeat = CreateWindow(TEXT("button"), TEXT("SELECTSEAT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 10, 200, 50, hWnd, (HMENU)ID_B_SELSEAT, g_hInst, NULL);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_B_SELSEAT:					//자리선택 눌렸을때
			lstrcpy(tgcmdserver, "1");
			GetWindowText(hEdit_Sn,tgnum, sizeof(tgnum));						//팀플//자리 선택(02등 2자리 형식으로)
			lstrcat(tgcmdserver, tgnum);
			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);				//서버로 자리선택 커맨드 보내기
			break;
		}
		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
LRESULT CALLBACK OrderProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {															//주문
	switch (iMessage) {
	case WM_CREATE:
		//아이템 스태틱
		hStatic_Item = CreateWindow(TEXT("static"), TEXT("ITEM"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 0, 40, 20, hWnd, (HMENU)ID_S_ITEM, g_hInst, NULL);
		//아이템리스트 리스트박스
		hListBox_ItemList = CreateWindow(TEXT("listbox"), NULL,WS_CHILD | WS_VISIBLE | WS_BORDER| WS_VSCROLL| LBS_SORT | LBS_NOTIFY,
			10, 100, 400, 200,hWnd, (HMENU)ID_L_ITEM,g_hInst, NULL);
		//아이템주문갯수 스태틱
		hStatic_OrderCount = CreateWindow(TEXT("static"), TEXT("Count(2자리)"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			510, 0, 100, 20, hWnd, (HMENU)ID_S_ORDERCOUNT, g_hInst, NULL);
		//아이템주문갯수 에디트
		hEdit_OrderCount = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			510, 50, 100, 50, hWnd, (HMENU)ID_E_ORDERCOUNT, g_hInst, NULL);
		//주문 버튼
		hButton_Order = CreateWindow(TEXT("button"), TEXT("ORDER"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 10, 200, 50, hWnd, (HMENU)ID_B_ORDER, g_hInst, NULL);
		//아이템리스트 초기화
		SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"DRINK");
		SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"FOOD");
		SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"PC");
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_L_ITEM:							//아이템리스트 노티피
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				selitemi = SendMessage(hListBox_ItemList, LB_GETCURSEL, 0, 0);				//팀플//주문 상품 선택(리스트박스의 인덱스로 1자리 형식으로)
				selitemi += 1;
				wsprintf(tgselitemi, "%d", selitemi);																			//아이템 인덱스 담기
				//SendDlgItemMessage(hDlg, IDC_ITEMLIST, LB_GETTEXT, i, (LPARAM)tgitem);
			}
			break;
		case ID_B_ORDER:																			//주문 눌렸을때
			lstrcpy(tgcmdserver, "2");
			lstrcat(tgcmdserver, tgselitemi);
			GetWindowText(hEdit_OrderCount, tgitemcount, sizeof(tgitemcount));					//아이템주문갯수 문자열에 담기
			lstrcat(tgcmdserver, tgitemcount);
			lstrcat(tgcmdserver, "SN:");
			lstrcat(tgcmdserver, tgSeatnum);
			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);							//서버로 주문 커맨드 보내기
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
