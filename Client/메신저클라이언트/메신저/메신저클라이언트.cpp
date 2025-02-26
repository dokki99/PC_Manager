//12/13 자리반납타이머 재설정, 피시상품 주문시 기존 시간과 합치고 로그아웃 시 잔여시간 보내기 
//12/4 소켓부분 바꿨으니 클라이언트의 메신저 참고 바람
//메세지크래커까지 완료
#include<Windowsx.h>
#include <Windows.h>
#include <WinUser.h>

#include <stdio.h>
#include <CommCtrl.h>
#include <sql.h>
#include <sqlext.h>
#include "resource.h"

#include "Socket.h"
#include "Screen.h"
#include "Timer.h"
#include "Request.h"
#include "Common.h"
WPARAM gwParam;
LPARAM glParam;
HWND hWnd;
//화면 핸들
HWND hWndMain;		// 메인
//화면 제목
LPCTSTR C_Class = TEXT("연결");					//연결
LPCTSTR JU_Class = TEXT("회원가입/수정");//가입/수정
LPCTSTR L_Class = TEXT("로그인");				//로그인
LPCTSTR S_Class = TEXT("자리선택");			//자리선택
LPCTSTR O_Class = TEXT("주문");					//주문

//리시브스레드 변수
HDC hdc;
SOCKET* P;
TCHAR CODE[3] = "", TEXT[256] = "";
int j;
//

////컨트롤 핸들
/// 숫자버튼(자리번호,주문수량)
HWND hButton_SeatNum0;
HWND hButton_SeatNum1;
HWND hButton_SeatNum2;
HWND hButton_SeatNum3;
HWND hButton_SeatNum4;
HWND hButton_SeatNum5;
HWND hButton_SeatNum6;
HWND hButton_SeatNum7;
HWND hButton_SeatNum8;
HWND hButton_SeatNum9;

HWND hButton_OrderCount0;
HWND hButton_OrderCount1;
HWND hButton_OrderCount2;
HWND hButton_OrderCount3;
HWND hButton_OrderCount4;
HWND hButton_OrderCount5;
HWND hButton_OrderCount6;
HWND hButton_OrderCount7;
HWND hButton_OrderCount8;
HWND hButton_OrderCount9;
//숫자 버튼 제어(자리번호,주문수량)
int snbtncount = 0;
int ocbtncount = 0;
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

HWND hButton_UserUpdate;					//유저에서 업데이트하고 다시 유저로 가는 버튼
HWND hButton_User_Go_Update;	//유저에서 수정가기 버튼
HWND hButton_Go_Join;	//가입가기 버튼
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

int selitemtrue;																																						//아이템 선택 체크
int infotrue;																																								//각 정보 담기 제어
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
//주문시간
int orderpctime;																																						//주문한 시간으로 기존에 병합
//아이템주문갯수
TCHAR tgselitemi[10];																																				//인덱스를 문자열로 변환
int selitemi;																																								//리스트 박스의 인덱스
TCHAR tgitemcount[10] = "";																																	//개수 문자열
//
///
//상태 체크 변수
BOOL chkreturnsn = 0;																																					//자리반납여부
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


//int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int):메뉴 설정, 띄울 다른 프로시저 설정
int APIENTRY WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, LPSTR lpszCmdParam, int nCmdShow)
{
	//HWND hWnd;
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

BOOL MsgCrk_OnCreate(HWND hWnd,LPCREATESTRUCT lpCreateStruct);
void MsgCrk_OnTimer(HWND hWnd,UINT id);
void MsgCrk_OnCommand(HWND hWnd,int,HWND,UINT id);
void MsgCrk_OnDestroy(HWND hWnd);
//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM):기본화면(다른 화면 설정,로그인동작의 타이머 수행)
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	gwParam = wParam;
	glParam = lParam;
	switch (iMessage) {
		HANDLE_MSG(hWnd, WM_CREATE, MsgCrk_OnCreate);
		HANDLE_MSG(hWnd, WM_TIMER, MsgCrk_OnTimer);
		HANDLE_MSG(hWnd, WM_COMMAND, MsgCrk_OnCommand);
		HANDLE_MSG(hWnd, WM_DESTROY, MsgCrk_OnDestroy);
	default:
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
	return 0;
}
BOOL MsgCrk_OnCreate(HWND hWnd,LPCREATESTRUCT lpCreateStruct) {
	hWndMain = hWnd;
	InitCommonControls();

	CLIENT_Open();

	SelSeat(hWnd,gwParam,glParam);



	//타이머(로그인 시 수행)
	SetTimer(hWnd, 0, 100, NULL);			//자동으로 화면모드 설정 타이머
	SetTimer(hWnd, 1, 1000, NULL);			//시간 타이머
	SetTimer(hWnd, 2, 1000, NULL);			//시간 타이머 종료하는 타이머
	SetTimer(hWnd, 3, 100000, NULL);	//자리 반납 타이머
	SetTimer(hWnd, 4, 1000, NULL);			//자리 반납 타이머 재설정
	SendMessage(hWnd, WM_TIMER, 0, 0);

	return TRUE;
}
void MsgCrk_OnTimer(HWND hWnd,UINT id) {
	switch (gwParam) {
	case 0:													//자동으로 화면모드 바꾸는 타이머
		if (chkupdate == 1) {										//수정 잘 되었을때
			chkupdate = 0;
			UserUpdateToUser();
		}
		//자리선택화면->로그인화면
		else if (chkseat == 1) {								//자리선택 잘 되었을때
			chkseat = 0;

			SelSeatToLogin();
		}
		//로그인화면->사용화면
		else if (chklogin == 1) {					//로그인 잘 되었을 때
			chklogin = 0;

			LoginToUser();
		}
		//남은시간 0일때 주문 후 로그인 화면으로/남은시간 있을때 주문 후 사용화면으로
		else if (chkorder == 1) {										//주문 잘 되었을 때
			chkorder = 0;

			OrderToLoginOrUser();
		}
		//남은시간 0일때 로그인화면에서 주문화면으로
		else if (chkpctime == 1) {															//남은시간 0일때
			chkpctime = 0;
			selafterorder = 1;																	//남은시간0일때로 설정

			LoginToOrder();
		}
		//가입 및 수정화면에서 로그인화면으로
		else if (chkjoin == 1) {										//회원가입 잘 되었을때
			chkjoin = 0;

			JoinOrUpdateToLogin();
		}
		//가입 및 수정화면에서 로그인화면으로 
		else if (chkupdate == 1) {										//수정 잘 되었을때
			chkupdate = 0;

			JoinOrUpdateToLogin();
		}
		//사용화면(로그아웃/탈퇴)/주문화면/로그인화면(자리반납)에서 자리선택화면으로 
		else if (gohome == 1) {																				//자리선택화면으로 가는 상황
			gohome = 0;

			ToSelSeat();
		}
		break;
	case 1:
		if (chktimer == 1) {
			UserTimer(hWnd, gwParam, glParam);
			//chktimer = 0;																															//타이머 끄는 상태로
		}
		break;
		//나가기버튼 눌렸을때/남은시간 0일때 남은시간 타이머 종료하고 다시 타이머 만들기(로그인을 계속 받기 위해)
	case 2:
		if (chkendbtn == 1) {

			SetUserTimer();
			chkendbtn = 0;
		}
		break;
	case 3:																																					//자리반납타이머
		if (returnsn == 1) {																															//자리반납동작일때
			ReturnSeatTimer();
			ToSelSeat();
			chkreturnsn = 1;
		}
		break;
	case 4:
		if (chkreturnsn == 1) {
			SetReturnSeatTimer();																												//자리반납타이머 재설정
			chkreturnsn = 0;
		}
		break;
	}
	return;
}
void MsgCrk_OnCommand(HWND hWnd,int unuseint,HWND unusehWnd,UINT id) {
	switch (LOWORD(gwParam))
	{
	case ID_B_SEATNUM0:
		if (snbtncount < 2) {
			snbtncount++;
			lstrcat(tgnum, "0");
			SetWindowText(hEdit_Sn, tgnum);
		}
		break;
	case ID_B_SEATNUM1:
		if (snbtncount < 2) {
			snbtncount++;
			lstrcat(tgnum, "1");
			SetWindowText(hEdit_Sn, tgnum);
		}
		break;
	case ID_B_SEATNUM2:
		if (snbtncount < 2) {
			snbtncount++;
			lstrcat(tgnum, "2");
			SetWindowText(hEdit_Sn, tgnum);
		}
		break;
	case ID_B_SEATNUM3:
		if (snbtncount < 2) {
			snbtncount++;
			lstrcat(tgnum, "3");
			SetWindowText(hEdit_Sn, tgnum);
		}
		break;
	case ID_B_SEATNUM4:
		if (snbtncount < 2) {
			snbtncount++;
			lstrcat(tgnum, "4");
			SetWindowText(hEdit_Sn, tgnum);
		}
		break;
	case ID_B_SEATNUM5:
		if (snbtncount < 2) {
			snbtncount++;
			lstrcat(tgnum, "5");
			SetWindowText(hEdit_Sn, tgnum);
		}
		break;
	case ID_B_SEATNUM6:
		if (snbtncount < 2) {
			snbtncount++;
			lstrcat(tgnum, "6");
			SetWindowText(hEdit_Sn, tgnum);
		}
		break;
	case ID_B_SEATNUM7:
		if (snbtncount < 2) {
			snbtncount++;
			lstrcat(tgnum, "7");
			SetWindowText(hEdit_Sn, tgnum);
		}
		break;
	case ID_B_SEATNUM8:
		if (snbtncount < 2) {
			snbtncount++;
			lstrcat(tgnum, "8");
			SetWindowText(hEdit_Sn, tgnum);
		}
		break;
	case ID_B_SEATNUM9:
		if (snbtncount < 2) {
			snbtncount++;
			lstrcat(tgnum, "9");
			SetWindowText(hEdit_Sn, tgnum);
		}
		break;
	case ID_B_ORDERCOUNT0:
		if (ocbtncount < 2) {
			ocbtncount++;
			lstrcat(tgitemcount, "0");
			SetWindowText(hEdit_OrderCount, tgitemcount);
		}
		break;
	case ID_B_ORDERCOUNT1:
		if (ocbtncount < 2) {
			ocbtncount++;
			lstrcat(tgitemcount, "1");
			SetWindowText(hEdit_OrderCount, tgitemcount);
		}
		break;
	case ID_B_ORDERCOUNT2:
		if (ocbtncount < 2) {
			ocbtncount++;
			lstrcat(tgitemcount, "2");
			SetWindowText(hEdit_OrderCount, tgitemcount);
		}
		break;
	case ID_B_ORDERCOUNT3:
		if (ocbtncount < 2) {
			ocbtncount++;
			lstrcat(tgitemcount, "3");
			SetWindowText(hEdit_OrderCount, tgitemcount);
		}
		break;
	case ID_B_ORDERCOUNT4:
		if (ocbtncount < 2) {
			ocbtncount++;
			lstrcat(tgitemcount, "4");
			SetWindowText(hEdit_OrderCount, tgitemcount);
		}
		break;
	case ID_B_ORDERCOUNT5:
		if (ocbtncount < 2) {
			ocbtncount++;
			lstrcat(tgitemcount, "5");
			SetWindowText(hEdit_OrderCount, tgitemcount);
		}
		break;
	case ID_B_ORDERCOUNT6:
		if (ocbtncount < 2) {
			ocbtncount++;
			lstrcat(tgitemcount, "6");
			SetWindowText(hEdit_OrderCount, tgitemcount);
		}
		break;
	case ID_B_ORDERCOUNT7:
		if (ocbtncount < 2) {
			ocbtncount++;
			lstrcat(tgitemcount, "7");
			SetWindowText(hEdit_OrderCount, tgitemcount);
		}
		break;
	case ID_B_ORDERCOUNT8:
		if (ocbtncount < 2) {
			ocbtncount++;
			lstrcat(tgitemcount, "8");
			SetWindowText(hEdit_OrderCount, tgitemcount);
		}
		break;
	case ID_B_ORDERCOUNT9:
		if (ocbtncount < 2) {
			ocbtncount++;
			lstrcat(tgitemcount, "9");
			SetWindowText(hEdit_OrderCount, tgitemcount);
		}
		break;
		//수정 버튼 눌렸을때
	case ID_B_USERUPDATE:
		GetWindowText(hEdit_Id, tgId, sizeof(tgId));
		GetWindowText(hEdit_Name, tgName, sizeof(tgName));
		GetWindowText(hEdit_Pw, tgPw, sizeof(tgPw));
		GetWindowText(hEdit_Pn, tgPn, sizeof(tgPn));
		GetWindowText(hEdit_Addr, tgAddr, sizeof(tgAddr));
		GetWindowText(hEdit_Birth, tgBirth, sizeof(tgBirth));
		if (lstrlen(tgId) != 0 && lstrlen(tgPw) != 0 && lstrlen(tgName) != 0 && lstrlen(tgPn) != 0 && lstrlen(tgAddr) != 0 && lstrlen(tgBirth) != 0 && ChkStringInfo(tgId) == 0 && ChkStringInfo(tgPw) == 0 && ChkStringInfo(tgName) == 0 && ChkStringInfo(tgPn) == 0 && ChkStringInfo(tgAddr) == 0 && ChkStringInfo(tgBirth) == 0)RequestUpdate();
		else MessageBox(hWnd, "입력 오류!!", "회원수정", MB_OK);
		break;
		//유저에서 수정가기 버튼 눌렸을때
	case ID_B_USER_GO_UPDATE:
		UserUpdate();
		break;
		//로그인화면에서 가입 및 수정 버튼 눌렸을때 로그인에서 가입 및 수정으로
	case ID_B_GO_JOIN:

		LoginToJoin();
		break;
		//로그인 버튼 눌렸을때
	case ID_B_LOGIN:
		GetWindowText(hEdit_Login_Id, tgId, sizeof(tgId));
		GetWindowText(hEdit_Login_Pw, tgPw, sizeof(tgPw));
		if (lstrlen(tgId) != 0 && lstrlen(tgPw) != 0 && ChkStringInfo(tgId)==0 && ChkStringInfo(tgPw) == 0)RequestLogin();
		else MessageBox(hWnd, "입력 오류!!", "로그인", MB_OK);
		break;

		//아이템리스트 노티피
	case ID_L_ITEM:
		switch (HIWORD(gwParam)) {
		case LBN_SELCHANGE:
			selitemi = SendMessage(hListBox_ItemList, LB_GETCURSEL, 0, 0);													//주문 상품 선택(리스트박스의 인덱스로 1자리 형식으로)
			selitemi += 1;
			wsprintf(tgselitemi, "%d", selitemi);																									//아이템 인덱스 담기
			selitemtrue = 1;
		}
		break;
		//주문 눌렸을때
	case ID_B_ORDER:
		if (selitemtrue == 1 && ocbtncount == 2 && lstrcmp(tgitemcount, "00") != 0)RequestOrder();																							//아이템 선택 체크
		else {
			SendMessage(hListBox_ItemList, LB_SETCURSEL, (WPARAM)-1, 0);															//리스트박스의 현재 셀 초기화
			selitemtrue = 0;																																			//입력아이템 초기화
			lstrcpy(tgitemcount, "");																																//입력아이템숫자 초기화
			SetWindowText(hEdit_OrderCount, tgitemcount);
			MessageBox(hWnd, "입력 오류!!", "주문", MB_OK);
			ocbtncount = 0;																																		//주문수량 버튼 횟수 초기화
		}
		lstrcpy(tgitemcount, "");																											//주문수량 문자열 초기화
		selitemtrue = 0;
		break;

		//가입버튼 눌렸을때
	case ID_B_JOIN:
		GetWindowText(hEdit_Id, tgId, sizeof(tgId));
		GetWindowText(hEdit_Name, tgName, sizeof(tgName));
		GetWindowText(hEdit_Pw, tgPw, sizeof(tgPw));
		GetWindowText(hEdit_Pn, tgPn, sizeof(tgPn));
		GetWindowText(hEdit_Addr, tgAddr, sizeof(tgAddr));
		GetWindowText(hEdit_Birth, tgBirth, sizeof(tgBirth));
		if (lstrlen(tgId) != 0 && lstrlen(tgPw) != 0 && lstrlen(tgName) != 0 && lstrlen(tgPn) != 0 && lstrlen(tgAddr) != 0 && lstrlen(tgBirth) != 0 && ChkStringInfo(tgId) == 0 && ChkStringInfo(tgPw) == 0 && ChkStringInfo(tgName) == 0 && ChkStringInfo(tgPn) == 0 && ChkStringInfo(tgAddr) == 0 && ChkStringInfo(tgBirth) == 0)RequestJoin();
		else MessageBox(hWnd, "입력 오류!!", "회원가입", MB_OK);
		break;

		//사용화면애서 주문가기 버튼 눌렸을때 사용화면에서 주문화면으로 
	case ID_B_GO_ORDER:

		UserToOrder();
		break;
		//자리선택화면에서 자리선택버튼 눌렸을때
	case ID_B_SELSEAT://																																

		if (snbtncount == 2 && lstrcmp(tgnum, "00") != 0)RequestSelSeat();
		else {
			lstrcpy(tgnum, "");																																//입력숫자 초기화
			SetWindowText(hEdit_Sn, tgnum);
			MessageBox(hWnd, "입력 오류!!", "자리번호", MB_OK);
			snbtncount = 0;																																		//자리번호버튼 횟수 초기화
		}
		break;
	case ID_B_LOGOUT://																																			//로그아웃(로그인 했을때)

		RequestLogout();
		break;
	case ID_B_DELETE:																																			//탈퇴 수행(로그인 했을때)

		RequestDelete();
		break;
	}
	return;
}
void MsgCrk_OnDestroy(HWND hWnd) {
	PostQuitMessage(0);
	return;
}
