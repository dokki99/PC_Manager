//하나의 창에 컨트롤을 하이드 시켜서 사용
//상품구매할때 아이디 보내기
#include <Windows.h>
#include <stdio.h>
#include <CommCtrl.h>
#include <sql.h>
#include <sqlext.h>
#include "resource.h"
//화면 핸들
HWND hWndMain;		// 메인
//화면 제목
LPCTSTR C_Class = TEXT("연결");					//연결
LPCTSTR JU_Class = TEXT("회원가입/수정");//가입/수정
LPCTSTR L_Class = TEXT("로그인");				//로그인
LPCTSTR S_Class = TEXT("자리선택");			//자리선택
LPCTSTR O_Class = TEXT("주문");					//주문
////컨트롤 핸들
//사용화면
HWND hButton_Logout;							//로그아웃 버튼
HWND hButton_Delete;							//탈퇴 버튼
HWND hEdit_Timer;								//탈퇴 에디트

HWND hButton_Go_Order;						//주문가기 버튼

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

HWND hButton_Go_Join_Or_Update;	//가입 및 수정가기 버튼
//자리선택
HWND hEdit_Sn;										//자리번호 에디트
HWND hButton_SelSeat;						//자리번호선택 버튼
//주문
HWND hListBox_ItemList;						//아이템리스트 리스트박스
HWND hEdit_OrderCount;						//아이템주문갯수 에디트
HWND hButton_Order;							//주문 버튼
///스태틱
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
//사용화면
HWND hStatic_Timer;							//타이머
//프로시저함수들
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);															//기본

HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("PC방 클라이언트");																										//윈도우 프로시저 타이틀
////IP와 포트번호 설정
const char g_szlpAddress[17] = "127.0.0.1";
const unsigned short g_uPort = 7878;
TCHAR tgIp[17] = "127.0.0.1", tgPort[10] = "7878";																									//IP와 PORT번호
TCHAR tgId[10], tgName[10], tgPw[10], tgPn[10], tgAddr[10], tgBirth[10];																//아이디,이름,비번,전화번호,주소,생일 문자열 변수들
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
BOOL chkseat = 0;																																							//자리선택
BOOL chkjoin = 0;																																							//가입
BOOL chklogin = 0;																																						//로그인
BOOL chktimer;																																							//타이머 제어(1이면 실행/0이면 종료)	
BOOL chkendbtn = 0;																																					//로그아웃/탈퇴 

BOOL chkorder = 0;																																						//주문
BOOL chkupdate = 0;																																					//수정
BOOL chkpctime = 0;																																					//남은시간0인지
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

//좌석반환변수
int returnsntime = 100000000;																																//자리반납타이머시간
BOOL returnsn = 0;																																						//자리반납수행

//자리선택으로 보내는 변수
BOOL gohome = 0;

//남은시간에따라  1:로그인화면/0:사용화면으로 보내기
int selafterorder = 0;

//화면모드(0:자리선택/1:로그인/2:가입 및 수정/3:주문/4:사용) 설정변수
int selwindow = 0;

//DWORD WINAPI ThreadFunc(LPVOID){}:리시브스레드로써 요청의 결과를 다룸
//머릿말인 숫자 다음 숫자가  1이 오면 가능/ 0이 오면 불가
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
					returnsntime = 100000000;																													//자리반납타이머시간 초기화
					returnsn = 1;																																			//자리반납동작
					chkseat = 1;																																			//자리선택 체크
					SetTimer(hWndMain, 0, 100, NULL);																									//자동으로 화면모드 설정 타이머
					SetTimer(hWndMain, 3, 1000000, NULL);																							//자리반납타이머
					//SendMessage(hWndMain, WM_TIMER, 0, 0);
					//SendMessage(hWndMain, WM_TIMER, 3, 0);
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
				if (buf[1] == '1') {
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
						else {
							wsprintf(buf, "%s", "로그인실패(잔여시간없음)");
							chkpctime = 1;
						}
					}
				}
				else wsprintf(buf, "%s", "로그인실패");
			}
			////주문결과(가능:2상품번호(1,2,3)1/불가:2상품번호(1,2,3)0)
			else if (buf[0] == '2') {
				//음료
				if (buf[1] == '1') {
					if (lstrcmp(orderbuf, TEXT("211")) == 0) {
						chkorder = 1;
						wsprintf(buf, "%s", "DRINK구입성공");
					}
					else wsprintf(buf, "%s", "DRINK구입실패");
				}
				//식품
				else if (buf[1] == '2') {
					if (lstrcmp(orderbuf, TEXT("221")) == 0) {
						chkorder = 1;
						wsprintf(buf, "%s", "FOOD구입성공");
					}
					else wsprintf(buf, "%s", "FOOD구입실패");
				}
				//PC
				else if (buf[1] == '3') {
					if (lstrcmp(orderbuf, TEXT("231")) == 0) {
						chkorder = 1;
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
				}
				else 	wsprintf(buf, "%s", "로그아웃실패");
			}
			//수정 결과(가능:61/불가:60)
			else if (buf[0] == '6') {
				if (lstrcmp(buf, TEXT("61")) == 0) {
					wsprintf(buf, "%s", "수정성공");
					chkupdate = 1;
				}
				else wsprintf(buf, "%s", "수정실패");
			}
			//탈퇴 결과(가능:71/불가:70)
			else if (buf[0] == '7') {
				if (lstrcmp(buf, TEXT("71")) == 0) {
					wsprintf(buf, "%s", "탈퇴성공");
					gohome = 1;																																		//자리 선택가기
				}
				else wsprintf(buf, "%s", "탈퇴실패");
			}
			//반납 결과(가능:81/불가:80)
			else if (buf[0] == '8') {
				if (lstrcmp(buf, TEXT("81")) == 0) {
					wsprintf(buf, "%s", "반환성공");
					gohome = 1;																																		//자리 선택가기
				}
				else wsprintf(buf, "%s", "반환실패");
			}
			//서버로부터 받은 메세지를 메세지박스로 띄우기
			if (lstrlen(buf) != 0) {
				MessageBox(hWndMain, buf, "수신한 메시지", MB_OK);
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
//void CLIENT_Open():소켓 연결 함수(연결스레드 만들기)
void CLIENT_Open() {
	nip = atoi(tgIp);
	nport = atoi(tgPort);
	nReturn = WSAStartup(WORD(2.0), &wsadata);
	clientsock = socket(AF_INET, SOCK_STREAM, 0);
	addr_client.sin_family = AF_INET;
	addr_client.sin_addr.s_addr = inet_addr(tgIp);
	addr_client.sin_port = htons(nport);
	hThread = CreateThread(NULL, 0, ThreadFunc, &clientsock, 0, &ThreadID);																	//연결스레드 만들기
	nReturn = connect(clientsock, (sockaddr*)&addr_client, addrlen_clt);
	if (nReturn) {
		MessageBox(hWndMain, "연결되었습니다!!", "오류", MB_OK);
	}
}
//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM):기본화면(다른 화면 설정,로그인동작의 타이머 수행)
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		InitCommonControls();

		CLIENT_Open();

		///
		///자리선택 컨트롤(홈화면)
		
		//자리번호 스태틱
		hStatic_Sn = CreateWindow(TEXT("static"), TEXT("SN(2자리)"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 0, 80, 20, hWnd, (HMENU)ID_S_SN, g_hInst, NULL);
		if (selwindow!=0 )ShowWindow(hStatic_Sn, SW_HIDE);
		//자리번호 에디트
		hEdit_Sn = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 50, 100, 50, hWnd, (HMENU)ID_E_SN, g_hInst, NULL);
		if (selwindow!=0)ShowWindow(hEdit_Sn, SW_HIDE);
		//자리선택 버튼
		hButton_SelSeat = CreateWindow(TEXT("button"), TEXT("SELECTSEAT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 10, 200, 50, hWnd, (HMENU)ID_B_SELSEAT, g_hInst, NULL);
		if (selwindow!=0)ShowWindow(hButton_SelSeat, SW_HIDE);

		
	
		//타이머(로그인 시 수행)
		SetTimer(hWnd, 1, 1000, NULL);			//시간 타이머
		SetTimer(hWnd, 2, 1000, NULL);			//시간 타이머 종료하는 타이머
		return 0;
	case WM_TIMER:
		switch (wParam) {
		case 0:													//자동으로 화면모드 바꾸는 타이머
			//자리선택화면->로그인화면
			if (chkseat == 1) {								//자리선택 잘 되었을때
				chkseat = 0;
				selwindow = 1;								//로그인화면으로 설정
				//자리선택화면 끄기
				if (selwindow != 0)ShowWindow(hStatic_Sn, SW_HIDE);
				if (selwindow != 0)ShowWindow(hEdit_Sn, SW_HIDE);
				if (selwindow != 0)ShowWindow(hButton_SelSeat, SW_HIDE);
				InvalidateRect(hWnd, NULL, TRUE);
				//로그인화면 켜기
				/////로그인화면 컨트롤
		//로그인 아이디 스태틱
				hStatic_Login_Id = CreateWindow(TEXT("static"), TEXT("ID"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
					10, 0, 80, 20, hWnd, (HMENU)ID_S_LOGIN_ID, g_hInst, NULL);
				if (selwindow != 1)ShowWindow(hStatic_Login_Id, SW_HIDE);
				//로그인 아이디 에디트
				hEdit_Login_Id = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
					10, 50, 100, 50, hWnd, (HMENU)ID_E_LOGIN_ID, g_hInst, NULL);
				if (selwindow != 1)ShowWindow(hEdit_Login_Id, SW_HIDE);
				//로그인 비번 스태틱
				hStatic_Login_Pw = CreateWindow(TEXT("static"), TEXT("PW"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
					10, 150, 40, 20, hWnd, (HMENU)ID_S_LOGIN_PW, g_hInst, NULL);
				if (selwindow != 1)ShowWindow(hStatic_Login_Pw, SW_HIDE);
				//로그인 비번 에디트
				hEdit_Login_Pw = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
					10, 200, 100, 50, hWnd, (HMENU)ID_E_LOGIN_PW, g_hInst, NULL);
				if (selwindow != 1)ShowWindow(hEdit_Login_Pw, SW_HIDE);
				//로그인 버튼
				hButton_Login = CreateWindow(TEXT("button"), TEXT("LOGIN"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					910, 10, 200, 50, hWnd, (HMENU)ID_B_LOGIN, g_hInst, NULL);
				if (selwindow != 1)ShowWindow(hButton_Login, SW_HIDE);

				//가입/수정 가기 버튼
				hButton_Go_Join_Or_Update = CreateWindow(TEXT("button"), TEXT("JOIN/UPDATE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					910, 100, 200, 50, hWnd, (HMENU)ID_B_GO_JOIN_OR_UPDATE, g_hInst, NULL);;
				if (selwindow != 1)ShowWindow(hButton_Go_Join_Or_Update, SW_HIDE);
				///
			}
			//로그인화면->사용화면
			else if (chklogin == 1) {					//로그인 잘 되었을 때
				chklogin = 0;
				selwindow = 4;							//사용화면
				InvalidateRect(hWnd, NULL, TRUE);
				//로그인화면 끄기
				if (selwindow != 1)ShowWindow(hStatic_Login_Id, SW_HIDE);
				if (selwindow != 1)ShowWindow(hEdit_Login_Id, SW_HIDE);
				if (selwindow != 1)ShowWindow(hStatic_Login_Pw, SW_HIDE);
				if (selwindow != 1)ShowWindow(hEdit_Login_Pw, SW_HIDE);
				if (selwindow != 1)ShowWindow(hButton_Login, SW_HIDE);
				if (selwindow != 1)ShowWindow(hButton_Go_Join_Or_Update, SW_HIDE);
				if (selwindow != 1)ShowWindow(hButton_Go_Order, SW_HIDE);
				//사용화면 켜기
				///사용화면 컨트롤
				//주문가기 버튼
				hButton_Go_Order = CreateWindow(TEXT("button"), TEXT("ORDER"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					910, 200, 200, 50, hWnd, (HMENU)ID_B_GO_ORDER, g_hInst, NULL);
				if (selwindow != 4)ShowWindow(hButton_Go_Order, SW_HIDE);
				///
		//로그아웃 버튼
				hButton_Logout = CreateWindow(TEXT("button"), TEXT("LOGOUT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					610, 50, 200, 65, hWnd, (HMENU)ID_B_LOGOUT, g_hInst, NULL);
				if (selwindow != 4)ShowWindow(hButton_Logout, SW_HIDE);
				//탈퇴 버튼
				hButton_Delete = CreateWindow(TEXT("button"), TEXT("DELETE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					910, 50, 200, 65, hWnd, (HMENU)ID_B_DELETE, g_hInst, NULL);
				if (selwindow != 4)ShowWindow(hButton_Delete, SW_HIDE);
				//타이머 에디트
				hEdit_Timer = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
					610, 200, 100, 100, hWnd, (HMENU)ID_E_TIMER, g_hInst, NULL);
				if (selwindow != 4)ShowWindow(hEdit_Timer, SW_HIDE);
				//타이머 스태틱
				hStatic_Timer = CreateWindow(TEXT("static"), TEXT("TIME"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
					610, 150, 40, 20, hWnd, (HMENU)ID_S_TIMER, g_hInst, NULL);
				if (selwindow != 4)ShowWindow(hStatic_Timer, SW_HIDE);
				///
			}
			//남은시간 0일때 주문 후 로그인 화면으로/남은시간 있을때 주문 후 사용화면으로
			else if (chkorder == 1) {										//주문 잘 되었을 때
				chkorder = 0;
				if (selafterorder == 1)selwindow = 1;				//남은 시간 0일때 로그인화면으로 설정
				else selwindow = 4;											//..아닐때 사용화면으로 설정
				InvalidateRect(hWnd, NULL, TRUE);
				//주문화면 끄기
				if (selwindow != 3)ShowWindow(hStatic_Item, SW_HIDE);
				if (selwindow != 3)ShowWindow(hListBox_ItemList, SW_HIDE);
				if (selwindow != 3)ShowWindow(hStatic_OrderCount, SW_HIDE);
				if (selwindow != 3)ShowWindow(hEdit_OrderCount, SW_HIDE);
				if (selwindow != 3)ShowWindow(hButton_Order, SW_HIDE);
				//남은시간 0일때 로그인화면으로 
				if (selafterorder == 1) {
					selafterorder = 0;
					/////로그인화면 컨트롤
		//로그인 아이디 스태틱
					hStatic_Login_Id = CreateWindow(TEXT("static"), TEXT("ID"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
						10, 0, 80, 20, hWnd, (HMENU)ID_S_LOGIN_ID, g_hInst, NULL);
					if (selwindow != 1)ShowWindow(hStatic_Login_Id, SW_HIDE);
					//로그인 아이디 에디트
					hEdit_Login_Id = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
						10, 50, 100, 50, hWnd, (HMENU)ID_E_LOGIN_ID, g_hInst, NULL);
					if (selwindow != 1)ShowWindow(hEdit_Login_Id, SW_HIDE);
					//로그인 비번 스태틱
					hStatic_Login_Pw = CreateWindow(TEXT("static"), TEXT("PW"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
						10, 150, 40, 20, hWnd, (HMENU)ID_S_LOGIN_PW, g_hInst, NULL);
					if (selwindow != 1)ShowWindow(hStatic_Login_Pw, SW_HIDE);
					//로그인 비번 에디트
					hEdit_Login_Pw = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
						10, 200, 100, 50, hWnd, (HMENU)ID_E_LOGIN_PW, g_hInst, NULL);
					if (selwindow != 1)ShowWindow(hEdit_Login_Pw, SW_HIDE);
					//로그인 버튼
					hButton_Login = CreateWindow(TEXT("button"), TEXT("LOGIN"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
						910, 10, 200, 50, hWnd, (HMENU)ID_B_LOGIN, g_hInst, NULL);
					if (selwindow != 1)ShowWindow(hButton_Login, SW_HIDE);

					//가입/수정 가기 버튼
					hButton_Go_Join_Or_Update = CreateWindow(TEXT("button"), TEXT("JOIN/UPDATE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
						910, 100, 200, 50, hWnd, (HMENU)ID_B_GO_JOIN_OR_UPDATE, g_hInst, NULL);;
					if (selwindow != 1)ShowWindow(hButton_Go_Join_Or_Update, SW_HIDE);
				}
				//남은 시간있을때 사용화면으로
				else {
					///사용화면 컨트롤
					//주문가기 버튼
					hButton_Go_Order = CreateWindow(TEXT("button"), TEXT("ORDER"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
						910, 200, 200, 50, hWnd, (HMENU)ID_B_GO_ORDER, g_hInst, NULL);
					if (selwindow != 4)ShowWindow(hButton_Go_Order, SW_HIDE);
					///
	//로그아웃 버튼
					hButton_Logout = CreateWindow(TEXT("button"), TEXT("LOGOUT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
						610, 50, 200, 65, hWnd, (HMENU)ID_B_LOGOUT, g_hInst, NULL);
					if (selwindow != 4)ShowWindow(hButton_Logout, SW_HIDE);
					//탈퇴 버튼
					hButton_Delete = CreateWindow(TEXT("button"), TEXT("DELETE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
						910, 50, 200, 65, hWnd, (HMENU)ID_B_DELETE, g_hInst, NULL);
					if (selwindow != 4)ShowWindow(hButton_Delete, SW_HIDE);
					//타이머 에디트
					hEdit_Timer = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
						610, 200, 100, 100, hWnd, (HMENU)ID_E_TIMER, g_hInst, NULL);
					if (selwindow != 4)ShowWindow(hEdit_Timer, SW_HIDE);
					//타이머 스태틱
					hStatic_Timer = CreateWindow(TEXT("static"), TEXT("TIME"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
						610, 150, 40, 20, hWnd, (HMENU)ID_S_TIMER, g_hInst, NULL);
					if (selwindow != 4)ShowWindow(hStatic_Timer, SW_HIDE);
					///
				}
			}
			//남은시간 0일때 로그인화면에서 주문화면으로
			else if (chkpctime == 1) {															//남은시간 0일때
				chkpctime = 0;
				selafterorder = 1;																	//남은시간0일때로 설정
				selwindow = 3;																		//주문화면으로
				InvalidateRect(hWnd, NULL, TRUE);
				//로그인화면 끄기
				if (selwindow != 1)ShowWindow(hStatic_Login_Id, SW_HIDE);
				if (selwindow != 1)ShowWindow(hEdit_Login_Id, SW_HIDE);
				if (selwindow != 1)ShowWindow(hStatic_Login_Pw, SW_HIDE);
				if (selwindow != 1)ShowWindow(hEdit_Login_Pw, SW_HIDE);
				if (selwindow != 1)ShowWindow(hButton_Login, SW_HIDE);
				if (selwindow != 1)ShowWindow(hButton_Go_Join_Or_Update, SW_HIDE);
				if (selwindow != 1)ShowWindow(hButton_Go_Order, SW_HIDE);
				//주문화면 켜기
				///주문화면 컨트롤
		//아이템 스태틱
				hStatic_Item = CreateWindow(TEXT("static"), TEXT("ITEM"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
					10, 0, 40, 20, hWnd, (HMENU)ID_S_ITEM, g_hInst, NULL);
				if (selwindow != 3)ShowWindow(hStatic_Item, SW_HIDE);
				//아이템리스트 리스트박스
				hListBox_ItemList = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_SORT | LBS_NOTIFY,
					10, 100, 400, 200, hWnd, (HMENU)ID_L_ITEM, g_hInst, NULL);
				if (selwindow != 3)ShowWindow(hListBox_ItemList, SW_HIDE);
				//아이템주문갯수 스태틱
				hStatic_OrderCount = CreateWindow(TEXT("static"), TEXT("Count(2자리)"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
					510, 0, 100, 20, hWnd, (HMENU)ID_S_ORDERCOUNT, g_hInst, NULL);
				if (selwindow != 3)ShowWindow(hStatic_OrderCount, SW_HIDE);
				//아이템주문갯수 에디트
				hEdit_OrderCount = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
					510, 50, 100, 50, hWnd, (HMENU)ID_E_ORDERCOUNT, g_hInst, NULL);
				if (selwindow != 3)ShowWindow(hEdit_OrderCount, SW_HIDE);
				//주문 버튼
				hButton_Order = CreateWindow(TEXT("button"), TEXT("ORDER"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					910, 10, 200, 50, hWnd, (HMENU)ID_B_ORDER, g_hInst, NULL);
				if (selwindow != 3)ShowWindow(hButton_Order, SW_HIDE);
				///
				//아이템리스트 초기화
				SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"DRINK");
				SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"FOOD");
				SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"PC");
			}
			//가입 및 수정화면에서 로그인화면으로
			else if (chkjoin == 1) {										//회원가입 잘 되었을때
				chkjoin = 0;
				selwindow = 1;												//로그인화면 설정
				InvalidateRect(hWnd, NULL, TRUE);
				//가입 및 수정화면 끄기
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
				//로그인화면 켜기
				/////로그인화면 컨트롤
		//로그인 아이디 스태틱
				hStatic_Login_Id = CreateWindow(TEXT("static"), TEXT("ID"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
					10, 0, 80, 20, hWnd, (HMENU)ID_S_LOGIN_ID, g_hInst, NULL);
				if (selwindow != 1)ShowWindow(hStatic_Login_Id, SW_HIDE);
				//로그인 아이디 에디트
				hEdit_Login_Id = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
					10, 50, 100, 50, hWnd, (HMENU)ID_E_LOGIN_ID, g_hInst, NULL);
				if (selwindow != 1)ShowWindow(hEdit_Login_Id, SW_HIDE);
				//로그인 비번 스태틱
				hStatic_Login_Pw = CreateWindow(TEXT("static"), TEXT("PW"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
					10, 150, 40, 20, hWnd, (HMENU)ID_S_LOGIN_PW, g_hInst, NULL);
				if (selwindow != 1)ShowWindow(hStatic_Login_Pw, SW_HIDE);
				//로그인 비번 에디트
				hEdit_Login_Pw = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
					10, 200, 100, 50, hWnd, (HMENU)ID_E_LOGIN_PW, g_hInst, NULL);
				if (selwindow != 1)ShowWindow(hEdit_Login_Pw, SW_HIDE);
				//로그인 버튼
				hButton_Login = CreateWindow(TEXT("button"), TEXT("LOGIN"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					910, 10, 200, 50, hWnd, (HMENU)ID_B_LOGIN, g_hInst, NULL);
				if (selwindow != 1)ShowWindow(hButton_Login, SW_HIDE);

				//가입/수정 가기 버튼
				hButton_Go_Join_Or_Update = CreateWindow(TEXT("button"), TEXT("JOIN/UPDATE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					910, 100, 200, 50, hWnd, (HMENU)ID_B_GO_JOIN_OR_UPDATE, g_hInst, NULL);;
				if (selwindow != 1)ShowWindow(hButton_Go_Join_Or_Update, SW_HIDE);
				
			}
			//가입 및 수정화면에서 로그인화면으로 
			else if (chkupdate == 1) {										//수정 잘 되었을때
				chkupdate = 0;
				selwindow = 1;													//로그인화면 설정
				InvalidateRect(hWnd, NULL, TRUE);
				//가입 및 수정화면 끄기
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
				//로그인화면 켜기
				/////로그인화면 컨트롤
		//로그인 아이디 스태틱
				hStatic_Login_Id = CreateWindow(TEXT("static"), TEXT("ID"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
					10, 0, 80, 20, hWnd, (HMENU)ID_S_LOGIN_ID, g_hInst, NULL);
				if (selwindow != 1)ShowWindow(hStatic_Login_Id, SW_HIDE);
				//로그인 아이디 에디트
				hEdit_Login_Id = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
					10, 50, 100, 50, hWnd, (HMENU)ID_E_LOGIN_ID, g_hInst, NULL);
				if (selwindow != 1)ShowWindow(hEdit_Login_Id, SW_HIDE);
				//로그인 비번 스태틱
				hStatic_Login_Pw = CreateWindow(TEXT("static"), TEXT("PW"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
					10, 150, 40, 20, hWnd, (HMENU)ID_S_LOGIN_PW, g_hInst, NULL);
				if (selwindow != 1)ShowWindow(hStatic_Login_Pw, SW_HIDE);
				//로그인 비번 에디트
				hEdit_Login_Pw = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
					10, 200, 100, 50, hWnd, (HMENU)ID_E_LOGIN_PW, g_hInst, NULL);
				if (selwindow != 1)ShowWindow(hEdit_Login_Pw, SW_HIDE);
				//로그인 버튼
				hButton_Login = CreateWindow(TEXT("button"), TEXT("LOGIN"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					910, 10, 200, 50, hWnd, (HMENU)ID_B_LOGIN, g_hInst, NULL);
				if (selwindow != 1)ShowWindow(hButton_Login, SW_HIDE);

				//가입/수정 가기 버튼
				hButton_Go_Join_Or_Update = CreateWindow(TEXT("button"), TEXT("JOIN/UPDATE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					910, 100, 200, 50, hWnd, (HMENU)ID_B_GO_JOIN_OR_UPDATE, g_hInst, NULL);;
				if (selwindow != 1)ShowWindow(hButton_Go_Join_Or_Update, SW_HIDE);
			}
			//사용화면(로그아웃/탈퇴)/주문화면/로그인화면(자리반납)에서 자리선택화면으로 
		else if (gohome == 1) {																				//자리선택화면으로 가는 상황
			gohome = 0;
			selwindow = 0;																						//자리선택화면 설정
			InvalidateRect(hWnd, NULL, TRUE);
			//사용화면 끄기
			if (selwindow != 4)ShowWindow(hButton_Go_Order, SW_HIDE);
			if (selwindow != 4)ShowWindow(hButton_Logout, SW_HIDE);
			if (selwindow != 4)ShowWindow(hButton_Delete, SW_HIDE);
			if (selwindow != 4)ShowWindow(hEdit_Timer, SW_HIDE);
			if (selwindow != 4)ShowWindow(hStatic_Timer, SW_HIDE);
			//주문화면 끄기
			if (selwindow != 3)ShowWindow(hStatic_Item, SW_HIDE);
			if (selwindow != 3)ShowWindow(hListBox_ItemList, SW_HIDE);
			if (selwindow != 3)ShowWindow(hStatic_OrderCount, SW_HIDE);
			if (selwindow != 3)ShowWindow(hEdit_OrderCount, SW_HIDE);
			if (selwindow != 3)ShowWindow(hButton_Order, SW_HIDE);
			//로그인화면 끄기
			if (selwindow != 1)ShowWindow(hStatic_Login_Id, SW_HIDE);
			if (selwindow != 1)ShowWindow(hEdit_Login_Id, SW_HIDE);
			if (selwindow != 1)ShowWindow(hStatic_Login_Pw, SW_HIDE);
			if (selwindow != 1)ShowWindow(hEdit_Login_Pw, SW_HIDE);
			if (selwindow != 1)ShowWindow(hButton_Login, SW_HIDE);
			if (selwindow != 1)ShowWindow(hButton_Go_Join_Or_Update, SW_HIDE);
			if (selwindow != 1)ShowWindow(hButton_Go_Order, SW_HIDE);
			//자리선택화면 켜기
			///자리선택화면 컨트롤

	//자리번호 스태틱
			hStatic_Sn = CreateWindow(TEXT("static"), TEXT("SN(2자리)"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
				10, 0, 80, 20, hWnd, (HMENU)ID_S_SN, g_hInst, NULL);
			if (selwindow != 0)ShowWindow(hStatic_Sn, SW_HIDE);
			//자리번호 에디트
			hEdit_Sn = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
				10, 50, 100, 50, hWnd, (HMENU)ID_E_SN, g_hInst, NULL);
			if (selwindow != 0)ShowWindow(hEdit_Sn, SW_HIDE);
			//자리선택 버튼
			hButton_SelSeat = CreateWindow(TEXT("button"), TEXT("SELECTSEAT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				910, 10, 200, 50, hWnd, (HMENU)ID_B_SELSEAT, g_hInst, NULL);
			if (selwindow != 0)ShowWindow(hButton_SelSeat, SW_HIDE);
		}
			// 화면 갱신 요청 (변경된 상태 출력)
			break;
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
				gohome = 1;
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
		case 3:																																					//자리반납타이머
			if (returnsn == 1) {																															//자리반납동작일때
				returnsntime -= 1000000;																											//자리반납시간 줄이기
				if (returnsntime <= 0) {																													//제한시간 경과
					returnsntime = 1000000;																											//자리반납시간 초기화
					returnsn = 0;																															//자리반납동작 끄기
					lstrcpy(tgcmdserver, "8");
					lstrcat(tgcmdserver, "SN:");
					lstrcat(tgcmdserver, tgnum);
					nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);											//서버로 자리반납 커멘드 보내기
					MessageBox(hWnd, "제한시간10초 초과로 좌석번호반납됨", "좌석반납", MB_OK);													
				}
			}
			break;
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		//로그인화면에서 가입 및 수정 버튼 눌렸을때 로그인에서 가입 및 수정으로
		case ID_B_GO_JOIN_OR_UPDATE:
			selwindow = 2;																																	//가입 및 수정화면으로 설정
			InvalidateRect(hWnd, NULL, TRUE);
			//로그인화면 끄기
			if (selwindow != 1)ShowWindow(hStatic_Login_Id, SW_HIDE);
			if (selwindow != 1)ShowWindow(hEdit_Login_Id, SW_HIDE);
			if (selwindow != 1)ShowWindow(hStatic_Login_Pw, SW_HIDE);
			if (selwindow != 1)ShowWindow(hEdit_Login_Pw, SW_HIDE);
			if (selwindow != 1)ShowWindow(hButton_Login, SW_HIDE);
			if (selwindow != 1)ShowWindow(hButton_Go_Join_Or_Update, SW_HIDE);
			if (selwindow != 1)ShowWindow(hButton_Go_Order, SW_HIDE);
			//가입 및 수정화면으로 켜기
			///가입 및 수정화면 컨트롤
		//아이디 스태틱
			hStatic_Id = CreateWindow(TEXT("static"), TEXT("ID"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
				10, 0, 40, 20, hWnd, (HMENU)ID_S_ID, g_hInst, NULL);
			if (selwindow != 2)ShowWindow(hStatic_Id, SW_HIDE);
			//아이디 에디트
			hEdit_Id = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
				10, 50, 100, 50, hWnd, (HMENU)ID_E_ID, g_hInst, NULL);
			if (selwindow != 2)ShowWindow(hEdit_Id, SW_HIDE);
			//비번 스태틱
			hStatic_Pw = CreateWindow(TEXT("static"), TEXT("PW"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
				10, 150, 40, 20, hWnd, (HMENU)ID_S_PW, g_hInst, NULL);
			if (selwindow != 2)ShowWindow(hStatic_Pw, SW_HIDE);
			//비번 에디트
			hEdit_Pw = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
				10, 200, 100, 50, hWnd, (HMENU)ID_E_PW, g_hInst, NULL);
			if (selwindow != 2)ShowWindow(hEdit_Pw, SW_HIDE);
			//이름 스태틱
			hStatic_Name = CreateWindow(TEXT("static"), TEXT("NAME"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
				10, 300, 40, 20, hWnd, (HMENU)ID_S_NAME, g_hInst, NULL);
			if (selwindow != 2)ShowWindow(hStatic_Name, SW_HIDE);
			//이름 에디트
			hEdit_Name = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
				10, 350, 100, 50, hWnd, (HMENU)ID_E_NAME, g_hInst, NULL);
			if (selwindow != 2)ShowWindow(hEdit_Name, SW_HIDE);
			//전화번호 스태틱
			hStatic_Pn = CreateWindow(TEXT("static"), TEXT("PN"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
				10, 450, 40, 20, hWnd, (HMENU)ID_S_PN, g_hInst, NULL);
			if (selwindow != 2)ShowWindow(hStatic_Pn, SW_HIDE);
			//전화번호 에디트
			hEdit_Pn = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
				10, 500, 100, 50, hWnd, (HMENU)ID_E_PN, g_hInst, NULL);
			if (selwindow != 2)ShowWindow(hEdit_Pn, SW_HIDE);
			//주소 스태틱
			hStatic_Addr = CreateWindow(TEXT("static"), TEXT("ADDR"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
				510, 0, 40, 20, hWnd, (HMENU)ID_S_ADDR, g_hInst, NULL);
			if (selwindow != 2)ShowWindow(hStatic_Addr, SW_HIDE);
			//주소 에디트
			hEdit_Addr = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
				510, 50, 100, 50, hWnd, (HMENU)ID_E_ADDR, g_hInst, NULL);
			if (selwindow != 2)ShowWindow(hEdit_Addr, SW_HIDE);
			//생일 스태틱
			hStatic_Birth = CreateWindow(TEXT("static"), TEXT("BIRTH"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
				510, 150, 50, 20, hWnd, (HMENU)ID_S_BIRTH, g_hInst, NULL);
			if (selwindow != 2)ShowWindow(hStatic_Birth, SW_HIDE);
			//생일 에디트
			hEdit_Birth = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
				510, 200, 100, 50, hWnd, (HMENU)ID_E_BIRTH, g_hInst, NULL);
			if (selwindow != 2)ShowWindow(hEdit_Birth, SW_HIDE);
			//가입 버튼
			hButton_Join = CreateWindow(TEXT("button"), TEXT("JOIN"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				910, 10, 300, 200, hWnd, (HMENU)ID_B_JOIN, g_hInst, NULL);
			if (selwindow != 2)ShowWindow(hButton_Join, SW_HIDE);
			//수정 버튼
			hButton_Update = CreateWindow(TEXT("button"), TEXT("UPDATE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				910, 300, 300, 200, hWnd, (HMENU)ID_B_UPDATE, g_hInst, NULL);
			if (selwindow != 2)ShowWindow(hButton_Update, SW_HIDE);
			///
			break;
			//로그인 버튼 눌렸을때
		case ID_B_LOGIN:
			returnsn = 0;																																				//자리반납 동작 끄기
			KillTimer(hWnd, 3);																																		//자리반납 타이머 끄기
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
			//추가될 기능: 로그인 된 아이디까지 같이 보내기
			lstrcpy(tgcmdserver, "2");
			lstrcat(tgcmdserver, tgselitemi);
			GetWindowText(hEdit_OrderCount, tgitemcount, sizeof(tgitemcount));													//아이템주문갯수 문자열에 담기
			lstrcat(tgcmdserver, tgitemcount);
			lstrcat(tgcmdserver, "ID:");																														//입력아이디 문자열에 담기
			lstrcat(tgcmdserver, tgId);
			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);															//서버로 주문 커맨드 보내기
			break;

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

			//사용화면애서 주문가기 버튼 눌렸을때 사용화면에서 주문화면으로 
		case ID_B_GO_ORDER:
			selwindow = 3;														//주문화면으로 설정
			InvalidateRect(hWnd, NULL, TRUE);
			//로그인화면 끄기
			if (selwindow != 1)ShowWindow(hStatic_Login_Id, SW_HIDE);
			if (selwindow != 1)ShowWindow(hEdit_Login_Id, SW_HIDE);
			if (selwindow != 1)ShowWindow(hStatic_Login_Pw, SW_HIDE);
			if (selwindow != 1)ShowWindow(hEdit_Login_Pw, SW_HIDE);
			if (selwindow != 1)ShowWindow(hButton_Login, SW_HIDE);
			if (selwindow != 1)ShowWindow(hButton_Go_Join_Or_Update, SW_HIDE);
			if (selwindow != 1)ShowWindow(hButton_Go_Order, SW_HIDE);
			//사용화면끄기
			if (selwindow != 4)ShowWindow(hButton_Go_Order, SW_HIDE);
			if (selwindow != 4)ShowWindow(hButton_Logout, SW_HIDE);
			if (selwindow != 4)ShowWindow(hButton_Delete, SW_HIDE);
			if (selwindow != 4)ShowWindow(hEdit_Timer, SW_HIDE);
			if (selwindow != 4)ShowWindow(hStatic_Timer, SW_HIDE);
			//주문화면 켜기
			///주문화면 컨트롤
	//아이템 스태틱
			hStatic_Item = CreateWindow(TEXT("static"), TEXT("ITEM"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
				10, 0, 40, 20, hWnd, (HMENU)ID_S_ITEM, g_hInst, NULL);
			if (selwindow != 3)ShowWindow(hStatic_Item, SW_HIDE);
			//아이템리스트 리스트박스
			hListBox_ItemList = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_SORT | LBS_NOTIFY,
				10, 100, 400, 200, hWnd, (HMENU)ID_L_ITEM, g_hInst, NULL);
			if (selwindow != 3)ShowWindow(hListBox_ItemList, SW_HIDE);
			//아이템주문갯수 스태틱
			hStatic_OrderCount = CreateWindow(TEXT("static"), TEXT("Count(2자리)"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
				510, 0, 100, 20, hWnd, (HMENU)ID_S_ORDERCOUNT, g_hInst, NULL);
			if (selwindow != 3)ShowWindow(hStatic_OrderCount, SW_HIDE);
			//아이템주문갯수 에디트
			hEdit_OrderCount = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
				510, 50, 100, 50, hWnd, (HMENU)ID_E_ORDERCOUNT, g_hInst, NULL);
			if (selwindow != 3)ShowWindow(hEdit_OrderCount, SW_HIDE);
			//주문 버튼
			hButton_Order = CreateWindow(TEXT("button"), TEXT("ORDER"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				910, 10, 200, 50, hWnd, (HMENU)ID_B_ORDER, g_hInst, NULL);
			if (selwindow != 3)ShowWindow(hButton_Order, SW_HIDE);
			///
			//아이템리스트 초기화
			SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"DRINK");
			SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"FOOD");
			SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"PC");
			break;
			//자리선택화면에서 자리선택버튼 눌렸을때
		case ID_B_SELSEAT:																																
			lstrcpy(tgcmdserver, "1");
			GetWindowText(hEdit_Sn, tgnum, sizeof(tgnum));																				//자리 선택(02등 숫자 2자리 형식으로)
			lstrcat(tgcmdserver, tgnum);
			nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);														//서버로 자리선택 커맨드 보내기
			break;
		case ID_B_LOGOUT:																																			//로그아웃(로그인 했을때)
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
			break;
		case ID_B_DELETE:																																			//탈퇴 수행(로그인 했을때)
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
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

