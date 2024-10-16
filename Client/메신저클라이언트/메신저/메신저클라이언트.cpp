
#include <Windows.h>
#include <stdio.h>
#include <CommCtrl.h>
#include <sql.h>
#include <sqlext.h>
#include "resource.h"
//화면 핸들
HWND hWndMain;		// 메인
HWND hWnd_C;			// 연결
HWND hWnd_JU;		// 가입/수정
HWND hWnd_L;			// 로그인
HWND hWnd_S;			// 자리선택
HWND hWnd_O;			// 주문 
//화면 제목
LPCTSTR C_Class = TEXT("연결");					//연결
LPCTSTR JU_Class = TEXT("회원가입/수정");//가입/수정
LPCTSTR L_Class = TEXT("로그인");				//로그인
LPCTSTR S_Class = TEXT("자리선택");			//자리선택
LPCTSTR O_Class = TEXT("주문");					//주문
////컨트롤 핸들
//프로시저
HWND hEdit_Chat;									//채팅 에디트
HWND hButton_Logout;							//로그아웃 버튼
HWND hButton_Delete;							//탈퇴 버튼
HWND hEdit_Timer;								//탈퇴 에디트
//연결
HWND hEdit_Ip;										//IP 에디트
HWND hEdit_Port;									//PORT 에디트
HWND hButton_Connect;						//연결 버튼
//가입/수정
HWND hEdit_Id;										//아이디 에디트
HWND hEdit_Pw;										//비번 에디트
HWND hEdit_Name;								//이름 에디트
HWND hEdit_Pn;										//전화번호 에디트
HWND hEdit_Addr;									//주소 에디트
HWND hEdit_Birth;									//생일 에디트
HWND hButton_Join;								//가입 버튼
HWND hButton_Update;							//수정 버튼
//로그인
HWND hEdit_Login_Id;							//아이디 에디트
HWND hEdit_Login_Pw;							//비번 에디트
HWND hButton_Login;								//로그인 버튼
//자리선택
HWND hEdit_Sn;										//자리번호 에디트
HWND hButton_SelSeat;						//자리번호선택 버튼
//주문
HWND hListBox_ItemList;						//아이템리스트 리스트박스
HWND hEdit_OrderCount;						//아이템주문갯수 에디트
HWND hButton_Order;							//주문 버튼
///스태틱
//연결
HWND hStatic_Ip;									//IP
HWND hStatic_Port;								//PORT
//가입/수정
HWND hStatic_Id;									//아이디
HWND hStatic_Pw;									//비번
HWND hStatic_Name;								//이름
HWND hStatic_Addr;								//주소
HWND hStatic_Birth;								//생일
HWND hStatic_Pn;									//전화번호
//로그인
HWND hStatic_Login_Id;						//아이디
HWND hStatic_Login_Pw;						//비번
//자리선택
HWND hStatic_Sn;									//자리번호
//주문
HWND hStatic_Item;								//아이템
HWND hStatic_OrderCount;					//아이템주문갯수
//프로시저
HWND hStatic_Chat;								//채팅
HWND hStatic_Timer;							//타이머
//프로시저함수들
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);															//기본
LRESULT CALLBACK ConnectProc(HWND, UINT, WPARAM, LPARAM);														//연결
LRESULT CALLBACK Join_Or_UpdateProc(HWND, UINT, WPARAM, LPARAM);										//가입/수정
LRESULT CALLBACK LoginProc(HWND, UINT, WPARAM, LPARAM);															//로그인
LRESULT CALLBACK SelSeatProc(HWND, UINT, WPARAM, LPARAM);														//자리선택
LRESULT CALLBACK OrderProc(HWND, UINT, WPARAM, LPARAM);														//주문

HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("PC방 클라이언트");																										//윈도우 프로시저 타이틀
////IP와 포트번호 설정
const char g_szlpAddress[17] = "127.0.0.1";
const unsigned short g_uPort = 7878;
TCHAR tgIp[17] = "127.0.0.1", tgPort[10] = "7878";																									//IP와 PORT번호
TCHAR tgId[10], tgName[10], tgPw[10],tgPn[10], tgAddr[10], tgBirth[10];																//아이디,이름,비번,전화번호,주소,생일 문자열 변수들
int nip, nport;																																							//IP와 PORT번호 숫자로 변환
																
DWORD ThreadID;																																					//스레드아이디
HANDLE hThread;																																					//스레드 핸들

TCHAR tgcmdserver[200];																																		//서버와 통신하는 커멘드 문자열
///소켓 관련 변수
int nReturn;																																								//통신 결과 
WSADATA wsadata;																																				//소켓 연결
SOCKET clientsock;																																					//소켓번호 담기
sockaddr_in addr_client;																																			
int addrlen_clt = sizeof(sockaddr);																															
///서버로부터 받는 메세지
char buf[1024];																																						//메시지 문자열																						
char strTemp[1024];																																				//메세지를 채팅창에 수신한 메세지로 변환
///

////주요변수
//커멘드에 여러 정보가 있을때
int turn = 0;																																								//유저의 정보를 순서대로 가져오게 하는 순서 제어
int ini;																																										//각 정보마다 버퍼(메세지)의 인덱스
TCHAR info[200];																																					//요소를 뽑는 문자열
int infoi = 0;																																								//요소를 뽑는 문자열의 인덱스
//
//로그인상황의  유저의 변수들
TCHAR userid[10];																																					//아이디
TCHAR usersn[10];																																					//자리번호
TCHAR userpw[10];																																					//비번
int pctimei;																																								//시간 인덱스
int pctime;																																								//로그인된 유저의 시간
//
///주문상황의 변수들
//아이템주문갯수
TCHAR tgselitemi[10];																																				//인덱스를 문자열로 변환
int selitemi;																																								//리스트 박스의 인덱스
TCHAR tgitemcount[10] = "";																																	//개수 문자열
//
///
//상태 체크 변수
int chkseat=0;																																							//자리선택
int chkjoin = 0;																																							//가입
int chklogin = 0;																																						//로그인
int chktimer;																																							//타이머 제어(1이면 실행/0이면 종료)	
int chkendbtn = 0;																																					//로그아웃/탈퇴 
//
int i;		
//자리번호 선택 변수들
TCHAR tgnum[10];																																					//신청하는 자리번호 문자열

//커맨드가 주문인지 판별
TCHAR orderbuf[10];																																				//주문요소정보(미리 메세지가 주문인지 판별)
int orderi = 0;																																							//주문요소정보 인덱스
//

//유저시간
TCHAR pctimebuf[10];																																				//커맨드에서 담아오기
TCHAR tpctime[10];																																					//출력하기 위한 문자열
//

//DWORD WINAPI ThreadFunc(LPVOID){}:리시브스레드로써 요청의 결과를 다룸
//머릿말인 숫자 다음 숫자가  1이 오면 가능/ 0이 오면 불가
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
			//orderbuf를 통해 커맨드가 주문에 관한것인지 판별
			orderi = 0;
			lstrcpy(orderbuf, "");
			for (int i = 0; i < 3; i++) {
				orderbuf[orderi++] = buf[i];
			}
			orderbuf[orderi] = '\0';
			//
			//자리선택 결과(가능:11/불가:10)
			if (buf[0] == '1') {
				if (lstrcmp(buf, TEXT("11")) == 0) {
					chkseat = 1;																																			//자리선택 체크
					lstrcpy(usersn, tgnum);																															//자리번호 담기									
					wsprintf(buf, "%s", "자리사용가능");
				}
				else wsprintf(buf, "%s", "자리사용불가");						
			}
			//회원가입 결과(가능:31/불가:30)
			else if (buf[0] == '3') {
				 if (lstrcmp(buf, TEXT("31")) == 0) {								
					chkjoin = 1;																										
					wsprintf(buf, "%s", "회원가입성공");
				}
				 else wsprintf(buf, "%s", "회원가입실패");						
			}
			//로그인 결과(가능:41NAME,PN,ADDR,BIRTH/불가:4100 또는 40)
			else if (buf[0] == '4') {
				if (buf[1]=='1') {			
					//남은시간 담기
					pctimei = 0;
					for (int i = 2; i < 4; i++) {
						pctimebuf[pctimei++] = buf[i];	
					}
					pctimebuf[pctimei] = '\0';								
					pctime = atoi(pctimebuf);
					//
					//가능할때 입력한 아이디 비번을 저장하고 이름,전화번호,주소,생일들을 담기
					if (pctime >= 0) {
						lstrcpy(userid, tgId);																											//계정 아이디,비번 담기
						lstrcpy(userpw, tgPw);
						i = 4;																																		//로그인 커맨드의 가져올 정보의  시작점
						turn = -1;																																//가져올 정보를 순서대로 가져오게 제어
						///로그인 커멘드에서 나머지 정보를 담는 프로세스(NAME-PN-ADDR-BIRTH순)
						while (buf[i] != '\0') { 						
							infoi = 0;
							if (buf[i] == ':') {																												///각 정보의 시작점으로 이동
								ini = i + 1;
								turn++;
							}
							if (turn == 0) {			
								while (buf[ini] != ':' && buf[ini] != '\0') {																		///":"전까지 담기
									info[infoi++] = buf[ini++];
								}
								if (infoi >= 2) infoi -= 2;																								///다음 정보 커맨드(PW,NAME..) 길이 만큼 빼기
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
								while (buf[ini] != '\0') {																									///마지막까지 담기
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
						wsprintf(tpctime, "%d", pctime);																								//남은시간을 출력하기위해 문자열로
						
						if (pctime > 0) {																						
							chklogin = 1;
							chktimer = 1;																														//로그인되면 타이머 동작
							wsprintf(buf, "%s", "로그인성공");	
						}
						else wsprintf(buf, "%s", "로그인실패(잔여시간없음)");
					}
				}
				else wsprintf(buf, "%s", "로그인실패");					
			}
			////주문결과(가능:2상품번호(1,2,3)1/불가:2상품번호(1,2,3)0)
			else if (buf[0] == '2') {
				//음료
				if (buf[1] == '1') {
					if (lstrcmp(orderbuf, TEXT("211")) == 0) {			
						wsprintf(buf, "%s", "DRINK구입성공");
					}
					else wsprintf(buf, "%s", "DRINK구입실패");			
				}
				//식품
				else if (buf[1] == '2') {
					if (lstrcmp(orderbuf, TEXT("221")) == 0) {			
						wsprintf(buf, "%s", "FOOD구입성공");
					}
					else wsprintf(buf, "%s", "FOOD구입실패");			
				}
				//PC
				else if (buf[1] == '3') {
					if (lstrcmp(orderbuf, TEXT("231")) == 0) {			
						wsprintf(buf, "%s", "PC구입성공");
					}
					else wsprintf(buf, "%s", "PC구입실패");				
				}
			}
			//로그아웃 결과(가능:51/불가:50
			else if (buf[0] == '5') {														
				if (lstrcmp(buf, TEXT("51")) == 0) {
					lstrcpy(userid, "");
					wsprintf(buf, "%s", "로그아웃성공");
					chklogin = 0;
				}
				else 	wsprintf(buf, "%s", "로그아웃실패");				
			}
			//수정 결과(가능:61/불가:60)
			else if (buf[0] == '6') {													
				if (lstrcmp(buf, TEXT("61")) == 0) {
					wsprintf(buf, "%s", "수정성공");
				}
				else wsprintf(buf, "%s", "수정실패");						
			}
			//탈퇴 결과(가능:71/불가:70)
			else if (buf[0] == '7') {													
				if (lstrcmp(buf, TEXT("71")) == 0) {
					wsprintf(buf, "%s", "탈퇴성공");							
				}
				else wsprintf(buf, "%s", "탈퇴실패");						
			}
			//서버로부터 받은 메세지를 채팅 에디트에 "수신한 메세지:+변환한 메세지"로 띄우기
			if (lstrlen(buf) != 0) {
				sprintf_s(strTemp, "수신한 메시지:%s", buf);								
				int len = GetWindowTextLength(hEdit_Chat);
				SendMessage(hEdit_Chat, EM_SETSEL, (WPARAM)len, (LPARAM)len);
				SendMessage(hEdit_Chat, EM_REPLACESEL, FALSE, (LPARAM)"\r\n");
				SendMessage(hEdit_Chat, EM_REPLACESEL, FALSE, (LPARAM)strTemp);												
			}
		}
	}
	return 0;
}
//int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int):메뉴 설정, 띄울 다른 프로시저 설정
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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);																			//메뉴달기
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
//void Init_Wnd(WNDCLASS*, int):윈도우프로시저에서 띄울 다른 프로시저 설정
void Init_Wnd(WNDCLASS* Wnd, int Proc_No) {																										
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
		Wnd->lpfnWndProc = ConnectProc;																																//연결화면
		Wnd->lpszClassName = C_Class;
		break;
	case 1:																																												//가입/수정화면
		Wnd->lpfnWndProc = Join_Or_UpdateProc;
		Wnd->lpszClassName = JU_Class;
		break;
	case 2:																																												//로그인화면
		Wnd->lpfnWndProc = LoginProc;
		Wnd->lpszClassName = L_Class;
		break;
	case 3:																																												//자리선택화면
		Wnd->lpfnWndProc = SelSeatProc;
		Wnd->lpszClassName = S_Class;
		break;
	case 4:																																												//주문화면
		Wnd->lpfnWndProc = OrderProc;
		Wnd->lpszClassName = O_Class;
		break;
	}
	Wnd->lpszMenuName = NULL;																																			//띄울 다른 프로시저에 메뉴는 없음
	Wnd->style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(Wnd);
}
//void CLIENT_Open():소켓 연결 함수(연결스레드 만들기)
void CLIENT_Open() {																								
	GetWindowText(hEdit_Ip, tgIp, sizeof(tgIp));																												//연결의 IP에디트에서 내용 가져오기
	GetWindowText(hEdit_Port, tgPort, sizeof(tgPort));																										//연결의 PORT에디트에서 내용 가져오기
	nip = atoi(tgIp);
	nport = atoi(tgPort);
	nReturn = WSAStartup(WORD(2.0), &wsadata);
	clientsock = socket(AF_INET, SOCK_STREAM, 0);
	addr_client.sin_family = AF_INET;
	addr_client.sin_addr.s_addr = inet_addr(tgIp);
	addr_client.sin_port = htons(nport);
	hThread = CreateThread(NULL, 0, ThreadFunc, &clientsock, 0, &ThreadID);																	//연결스레드 만들기
	nReturn = connect(clientsock, (sockaddr*)&addr_client, addrlen_clt);
	if (!nReturn) {
		int len = GetWindowTextLength(hEdit_Chat);
		SendMessage(hEdit_Chat, EM_SETSEL, (WPARAM)len, (LPARAM)len);
		SendMessage(hEdit_Chat, EM_REPLACESEL, FALSE, (LPARAM)TEXT("\r\n"));
		SendMessage(hEdit_Chat, EM_REPLACESEL, FALSE, (LPARAM)TEXT("입장하였습니다."));										//성공하면 채팅 에디트에 메세지 띄우기
	}
}
//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM):기본화면(다른 화면 설정,로그인동작의 타이머 수행)
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		InitCommonControls();

		//띄울 다른 화면들
		WNDCLASS Wnd_C;																															//연결
		WNDCLASS Wnd_JU;																															//가입/수정
		WNDCLASS Wnd_L;																															//로그인
		WNDCLASS Wnd_S;																															//자리선택
		WNDCLASS Wnd_O;																															//주문

		///윈도우 설정
		//연결
		Init_Wnd(&Wnd_C, 0);
		// 가입/수정
		Init_Wnd(&Wnd_JU, 1);
		// 로그인
		Init_Wnd(&Wnd_L, 2);
		// 자리
		Init_Wnd(&Wnd_S, 3);
		// 주문
		Init_Wnd(&Wnd_O, 4);
		///

		///컨트롤
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
		///
		//타이머(로그인 시 수행)
		SetTimer(hWnd, 1, 1000, NULL);			//시간 타이머
		SetTimer(hWnd, 2, 1000, NULL);			//시간 타이머 종료하는 타이머
		return 0;
	case WM_TIMER:
		switch (wParam) {
			//남은 시간 타이머(0이 될때까지 시간 감소하고 0이면 로그아웃처리하기)
		case 1:												
			if (chktimer == 1) {
				pctime -= 1;										
				wsprintf(tpctime, "%d", pctime);
				SetWindowText(hEdit_Timer, tpctime);																						//프로시저의 타이머 에디트에 유저 남은 시간 출력
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
					chkendbtn = 1;																															//로그아웃/탈퇴 처리로 
					chklogin = 0;																																//로그아웃 된 상태로
					nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);
					chktimer = 0;																															//타이머 끄는 상태로
					break;
				}
			}
			break;
			//나가기버튼 눌렸을때/남은시간 0일때 남은시간 타이머 종료하고 다시 타이머 만들기(로그인을 계속 받기 위해)
		case 2:																
			if (chkendbtn == 1) {
				KillTimer(hWnd, 1);																														//타이머 끄기
				chktimer = 0;																																//타이머 끄는 상태로
				SetTimer(hWnd, 1, 1000, NULL);																									//유저 남은 시간 타이머 다시 생성
				chkendbtn = 0;																																//로그아웃 체크 다시 원래 대로 초기화
				pctime = 0;
				wsprintf(tpctime, "%d", pctime);
				SetWindowText(hEdit_Timer, tpctime);																						//프로시저의 타이머 에디트에 유저 남은 시간 0으로  출력
			}
			break;
		}
		return 0;

		//메뉴 누르면 해당 프로시저 띄우기
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case ID_M_CONNECT:																																		//연결 오픈
			hWnd_C = CreateWindow(C_Class, C_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
			ShowWindow(hWnd_C, SW_SHOW);
			break;
		case ID_M_DISCONNECT:																																	//해제 눌렸을때
			SendMessage(hWndMain, WM_CLOSE, 0, 0);																									//창닫기
			break;
		case ID_M_JOIN_OR_UPDATE:																														// 가입/수정 오픈(자리 선택 수행했을때)
			if (chkseat == 1) {																																				
				hWnd_JU = CreateWindow(JU_Class, JU_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
				ShowWindow(hWnd_JU, SW_SHOW);
			}
			else MessageBox(hWnd, "자리 선택 먼저 하세요!!", "오류", MB_OK);
			break;
		case ID_M_LOGIN:																																				// 로그인 오픈(자리선택과 로그인 수행했을때)
			if (chkseat == 1 && chklogin == 0)	{																													
				hWnd_L = CreateWindow(L_Class, L_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
				ShowWindow(hWnd_L, SW_SHOW);
			}
			else {
				if (chkseat == 0)MessageBox(hWnd, "자리 선택 먼저 하세요!!", "오류", MB_OK);
				if (chklogin == 1)MessageBox(hWnd, "로그인 되어 있습니다!!", "오류", MB_OK);
			}
			break;
		case ID_M_SELSEAT:																																			// 자리선택 오픈
			hWnd_S = CreateWindow(S_Class, S_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
			ShowWindow(hWnd_S, SW_SHOW);
			break;
		case ID_M_ORDER:																																				// 주문 오픈(자리선택했을때)
			if (chkseat == 1) {																																				
				hWnd_O = CreateWindow(O_Class, O_Class, WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
				ShowWindow(hWnd_O, SW_SHOW);
			}
			else MessageBox(hWnd, "자리 선택 먼저 하세요!!", "오류", MB_OK);
			break;
		case ID_B_LOGOUT:																																			//로그아웃(로그인 했을때)
			if (chklogin == 1) {																																				
				lstrcpy(tgcmdserver, "");
				lstrcpy(tgcmdserver, "5ID:");
				lstrcat(tgcmdserver, userid);
				lstrcat(tgcmdserver, "PW:");
				lstrcat(tgcmdserver, userpw);
				lstrcat(tgcmdserver, "SN:");
				lstrcat(tgcmdserver, usersn);
				chkendbtn = 1;																																				//로그아웃/탈퇴 수행 했을때로
				nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);																//서버로 로그아웃 커멘드 보내기
				pctime = 0;										
				wsprintf(tpctime, "%d", pctime);
				SetWindowText(hEdit_Timer, tpctime);																										//프로시저의 타이머 에디트에 유저 남은 시간 0으로 출력
			}
			else MessageBox(hWnd, "로그인하세요!!", "오류", MB_OK);
			break;
		case ID_B_DELETE:																																			//탈퇴 수행(로그인 했을때)
			if (chklogin == 1) {																																			
				lstrcpy(tgcmdserver, "");
				lstrcpy(tgcmdserver, "7ID:");
				lstrcat(tgcmdserver, userid);
				lstrcat(tgcmdserver, "PW:");
				lstrcat(tgcmdserver, userpw);
				lstrcat(tgcmdserver, "SN:");
				lstrcat(tgcmdserver, usersn);
				chkendbtn = 1;																																				//로그아웃/탈퇴 수행 했을때로
				nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);																//서버로 탈퇴 커맨드 보내기
				pctime = 0;
				wsprintf(tpctime, "%d", pctime);
				SetWindowText(hEdit_Timer, tpctime);																										//프로시저의 타이머 에디트에 유저 남은 시간 0으로 출력
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
//LRESULT CALLBACK ConnectProc(HWND, UINT, WPARAM, LPARAM):연결화면(연결 수행)
LRESULT CALLBACK ConnectProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {					
	switch (iMessage) {
	case WM_CREATE:
		///컨트롤
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
		///
		//연결의 IP와 PORT에디트에 띄우기
		SetWindowText(hEdit_Ip, tgIp);
		SetWindowText(hEdit_Port, tgPort);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_B_CONNECT:																																			//연결 버튼 눌렸을때 연결 수행
			CLIENT_Open();
			break;
		}
		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
//LRESULT CALLBACK Join_Or_UpdateProc(HWND, UINT, WPARAM, LPARAM):가입/수정화면(가입/수정 수행하고 로그인된 유저의 정보를 띄우기)
LRESULT CALLBACK Join_Or_UpdateProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {									
	switch (iMessage) {
	case WM_CREATE:
		///컨트롤
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
		///
		//로그인 된 유저 정보 띄우기
		SetWindowText(hEdit_Id, userid);																													//아이디
		SetWindowText(hEdit_Pw, userpw);																												//비번
		SetWindowText(hEdit_Name, tgName);																											//이름
		SetWindowText(hEdit_Pn, tgPn);																														//전화번호
		SetWindowText(hEdit_Addr, tgAddr);																											//주소
		SetWindowText(hEdit_Birth, tgBirth);																											//생일
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			//가입버튼 눌렸을때
		case ID_B_JOIN:																																		
			//커맨드,계정 정보 문자열 빈칸으로 초기화
			lstrcpy(tgcmdserver, "");																																//서버와의 커맨드
			lstrcpy(tgId, "");																																			//아이디
			lstrcpy(tgName, "");																																		//이름
			lstrcpy(tgPw, "");																																			//비번
			lstrcpy(tgPn, "");																																			//전화번호
			lstrcpy(tgAddr, "");																																		//주소
			lstrcpy(tgBirth, "");																																		//생일
			//계정 정보 에디트에서 정보 문자열에 담기
			GetWindowText(hEdit_Id, tgId, sizeof(tgId));					
			GetWindowText(hEdit_Name,tgName, sizeof(tgName));		
			GetWindowText(hEdit_Pw,  tgPw, sizeof(tgPw));					
			GetWindowText(hEdit_Pn,  tgPn, sizeof(tgPn));					
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
			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);																	//서버로 가입 커맨드 보내기
			break;
			//수정버튼 눌렸을때
		case ID_B_UPDATE:																			
			//커맨드,계정 정보 문자열 빈칸으로 초기화
			lstrcpy(tgcmdserver, "");																																	//커맨드
			lstrcpy(tgId, "");																																				//아이디
			lstrcpy(tgName, "");																																			//이름
			lstrcpy(tgPw, "");																																				//비번
			lstrcpy(tgPn, "");																																				//전화번호
			lstrcpy(tgAddr, "");																																			//주소
			lstrcpy(tgBirth, "");																																			//생일
			//계정 정보 에디트에서 정보 문자열에 담기
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
			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);																			//서버로 수정 커맨드 보내기
			break;
		}
		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
//LRESULT CALLBACK LoginProc(HWND, UINT, WPARAM, LPARAM):로그인화면(로그인 수행)
LRESULT CALLBACK LoginProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case WM_CREATE:
		///컨트롤
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
		///
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			//로그인 버튼 눌렸을때
		case ID_B_LOGIN:									
			//커맨드와 정보 문자열 빈칸으로 초기화
			lstrcpy(tgcmdserver, "");																																//커맨드
			lstrcpy(tgId, "");																																			//아이디
			lstrcpy(tgPw, "");																																			//비번
			//로그인 정보 에디트에서 정보 문자열 담기
			GetWindowText(hEdit_Login_Id, tgId, sizeof(tgId));		
			GetWindowText(hEdit_Login_Pw, tgPw, sizeof(tgPw));	
			lstrcpy(tgcmdserver, "4ID:");
			lstrcat(tgcmdserver, tgId);
			lstrcat(tgcmdserver, "PW:");
			lstrcat(tgcmdserver, tgPw);
			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);																	//서버에 로그인 커맨드 보내기
			break;
		}
		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
//LRESULT CALLBACK SelSeatProc(HWND, UINT, WPARAM, LPARAM):자리선택화면(자리선택 수행)
LRESULT CALLBACK SelSeatProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {						
	switch (iMessage) {
	case WM_CREATE:
		///컨트롤
		//자리번호 스태틱
		hStatic_Sn = CreateWindow(TEXT("static"), TEXT("SN(2자리)"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 0, 80, 20, hWnd, (HMENU)ID_S_SN, g_hInst, NULL);
		//자리번호 에디트
		hEdit_Sn = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 50, 100, 50, hWnd, (HMENU)ID_E_SN, g_hInst, NULL);
		//자리선택 버튼
		hButton_SelSeat = CreateWindow(TEXT("button"), TEXT("SELECTSEAT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 10, 200, 50, hWnd, (HMENU)ID_B_SELSEAT, g_hInst, NULL);
		///
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			//자리선택 눌렸을때
		case ID_B_SELSEAT:					
			lstrcpy(tgcmdserver, "1");
			GetWindowText(hEdit_Sn,tgnum, sizeof(tgnum));																				//자리 선택(02등 숫자 2자리 형식으로)
			lstrcat(tgcmdserver, tgnum);
			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);														//서버로 자리선택 커맨드 보내기
			break;
		}
		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
//LRESULT CALLBACK OrderProc(HWND, UINT, WPARAM, LPARAM):주문화면(주문 수행)
LRESULT CALLBACK OrderProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {															
	switch (iMessage) {
	case WM_CREATE:
		///컨트롤
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
		///
		//아이템리스트 초기화
		SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"DRINK");
		SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"FOOD");
		SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"PC");
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
			//아이템리스트 노티피
		case ID_L_ITEM:							
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				selitemi = SendMessage(hListBox_ItemList, LB_GETCURSEL, 0, 0);													//주문 상품 선택(리스트박스의 인덱스로 1자리 형식으로)
				selitemi += 1;
				wsprintf(tgselitemi, "%d", selitemi);																									//아이템 인덱스 담기
			}
			break;
			//주문 눌렸을때
		case ID_B_ORDER:																			
			lstrcpy(tgcmdserver, "2");
			lstrcat(tgcmdserver, tgselitemi);
			GetWindowText(hEdit_OrderCount, tgitemcount, sizeof(tgitemcount));													//아이템주문갯수 문자열에 담기
			lstrcat(tgcmdserver, tgitemcount);
			lstrcat(tgcmdserver, "SN:");
			lstrcat(tgcmdserver, usersn);
			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);															//서버로 주문 커맨드 보내기
			break;
		}
		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
