#include "Common.h"
#include "Screen.h"
extern HWND hWnd;
extern HINSTANCE g_hInst;
//화면모드(0:자리선택/1:로그인/2:가입 및 수정/3:주문/4:사용) 설정변수
extern int selwindow;
//주문제어(남은 시간 0일때)
extern int selafterorder;																			

extern TCHAR tgId[10], tgName[10], tgPw[10], tgPn[10], tgAddr[10], tgBirth[10];																//아이디,이름,비번,전화번호,주소,생일 문자열 변수들
// 숫자 버튼(자리선택)
extern HWND hButton_SeatNum0;
extern HWND hButton_SeatNum1;
extern HWND hButton_SeatNum2;
extern HWND hButton_SeatNum3;
extern HWND hButton_SeatNum4;
extern HWND hButton_SeatNum5;
extern HWND hButton_SeatNum6;
extern HWND hButton_SeatNum7;
extern HWND hButton_SeatNum8;
extern HWND hButton_SeatNum9;

// 숫자 버튼(주문수량)
extern HWND hButton_OrderCount0;
extern HWND hButton_OrderCount1;
extern HWND hButton_OrderCount2;
extern HWND hButton_OrderCount3;
extern HWND hButton_OrderCount4;
extern HWND hButton_OrderCount5;
extern HWND hButton_OrderCount6;
extern HWND hButton_OrderCount7;
extern HWND hButton_OrderCount8;
extern HWND hButton_OrderCount9;
//


//자리선택
extern HWND hStatic_Sn;
extern HWND hEdit_Sn;
extern HWND hButton_SelSeat;
//로그인
extern HWND hStatic_Login_Id;
extern HWND hEdit_Login_Id;
extern HWND hStatic_Login_Pw;
extern HWND hEdit_Login_Pw;
extern HWND hButton_Login;
extern HWND hButton_Go_Join;

//사용화면
extern HWND hButton_User_Go_Update;	//유저에서 가입 및 수정가기 버튼
extern HWND hButton_Go_Order;
extern HWND hButton_Logout;
extern HWND hButton_Delete;
extern HWND hEdit_Timer;
extern HWND hStatic_Timer;

//주문화면
extern HWND hStatic_Item;
extern HWND hListBox_ItemList;
extern HWND hStatic_OrderCount;
extern HWND hEdit_OrderCount;
extern HWND hButton_Order;

//가입 및 수정
extern HWND hButton_UserUpdate;					//유저에서 업데이트하고 다시 유저로 가는 버튼
extern HWND hStatic_Id;
extern HWND hEdit_Id;
extern HWND hStatic_Pw;
extern HWND hEdit_Pw;
extern HWND hStatic_Name;
extern HWND hEdit_Name;
extern HWND hStatic_Pn;
extern HWND hEdit_Pn;
extern HWND hStatic_Addr;
extern HWND hEdit_Addr;
extern HWND hStatic_Birth;
extern HWND hEdit_Birth;
extern HWND hButton_Join;
extern HWND hButton_Update;
//수정화면에서 사용화면으로
void UserUpdateToUser() {
	selwindow = 4;
	//로그인화면 끄기
	if (selwindow != 1)ShowWindow(hStatic_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hStatic_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Login, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Join, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Order, SW_HIDE);
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
	if (selwindow != 2)ShowWindow(hButton_UserUpdate, SW_HIDE);
	///
	///사용화면 컨트롤
		//가입 및 수정 가기 버튼
	hButton_User_Go_Update = CreateWindow(TEXT("button"), TEXT("GOUPDATE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 400, 200, 65, hWnd, (HMENU)ID_B_USER_GO_UPDATE, g_hInst, NULL);
	//주문가기 버튼
	hButton_Go_Order = CreateWindow(TEXT("button"), TEXT("ORDER"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 200, 200, 50, hWnd, (HMENU)ID_B_GO_ORDER, g_hInst, NULL);
	///
//로그아웃 버튼
	hButton_Logout = CreateWindow(TEXT("button"), TEXT("LOGOUT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		610, 50, 200, 65, hWnd, (HMENU)ID_B_LOGOUT, g_hInst, NULL);
	//탈퇴 버튼
	hButton_Delete = CreateWindow(TEXT("button"), TEXT("DELETE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 50, 200, 65, hWnd, (HMENU)ID_B_DELETE, g_hInst, NULL);
	//타이머 에디트
	hEdit_Timer = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		610, 200, 100, 100, hWnd, (HMENU)ID_E_TIMER, g_hInst, NULL);
	// 읽기 전용 속성 적용
	SendMessage(hEdit_Timer, EM_SETREADONLY, TRUE, 0);
	//타이머 스태틱
	hStatic_Timer = CreateWindow(TEXT("static"), TEXT("TIME"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		610, 150, 40, 20, hWnd, (HMENU)ID_S_TIMER, g_hInst, NULL);
	///
}
//사용화면에서 수정화면으로
void UserUpdate() {
	selwindow = 2;
	///사용화면 컨트롤 끄기
	if (selwindow != 4)ShowWindow(hButton_User_Go_Update, SW_HIDE);
	if (selwindow != 4)ShowWindow(hButton_Go_Order, SW_HIDE);
	if (selwindow != 4)ShowWindow(hButton_Logout, SW_HIDE);
	if (selwindow != 4)ShowWindow(hButton_Delete, SW_HIDE);
	if (selwindow != 4)ShowWindow(hEdit_Timer, SW_HIDE);
	if (selwindow != 4)ShowWindow(hStatic_Timer, SW_HIDE);
	///
	//가입 및 수정화면으로 켜기
	///가입 및 수정화면 컨트롤
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
	//수정 버튼
	hButton_UserUpdate = CreateWindow(TEXT("button"), TEXT("UPDATE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 300, 300, 200, hWnd, (HMENU)ID_B_USERUPDATE, g_hInst, NULL);
	///
	SetWindowText(hEdit_Id, tgId);
	SetWindowText(hEdit_Name, tgName);
	SetWindowText(hEdit_Pw, tgPw);
	SetWindowText(hEdit_Pn, tgPn);
	SetWindowText(hEdit_Addr, tgAddr);
	SetWindowText(hEdit_Birth, tgBirth);
}
//자리선택화면
void SelSeat(HWND hWnd, WPARAM wParam,LPARAM lParam) {
	selwindow = 0;
	///
		///자리선택 컨트롤(홈화면)

		//자리번호 스태틱
	hStatic_Sn = CreateWindow(TEXT("static"), TEXT("SN(2자리)"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 0, 80, 20, hWnd, (HMENU)ID_S_SN, g_hInst, NULL);
	//자리번호 에디트
	hEdit_Sn = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 50, 100, 50, hWnd, (HMENU)ID_E_SN, g_hInst, NULL);
	// 읽기 전용 속성 적용
	SendMessage(hEdit_Sn, EM_SETREADONLY, TRUE, 0);
	//자리선택 버튼
	hButton_SelSeat = CreateWindow(TEXT("button"), TEXT("SELECTSEAT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 10, 200, 50, hWnd, (HMENU)ID_B_SELSEAT, g_hInst, NULL);

	//자리번호 버튼
	hButton_SeatNum0 = CreateWindow(TEXT("button"), TEXT("0"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM0, g_hInst, NULL);
	hButton_SeatNum1 = CreateWindow(TEXT("button"), TEXT("1"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM1, g_hInst, NULL);
	hButton_SeatNum2 = CreateWindow(TEXT("button"), TEXT("2"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		200, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM2, g_hInst, NULL);
	hButton_SeatNum3 = CreateWindow(TEXT("button"), TEXT("3"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		300, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM3, g_hInst, NULL);
	hButton_SeatNum4 = CreateWindow(TEXT("button"), TEXT("4"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		400, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM4, g_hInst, NULL);
	hButton_SeatNum5 = CreateWindow(TEXT("button"), TEXT("5"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		500, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM5, g_hInst, NULL);
	hButton_SeatNum6 = CreateWindow(TEXT("button"), TEXT("6"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		600, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM6, g_hInst, NULL);
	hButton_SeatNum7 = CreateWindow(TEXT("button"), TEXT("7"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		700, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM7, g_hInst, NULL);
	hButton_SeatNum8 = CreateWindow(TEXT("button"), TEXT("8"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		800, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM8, g_hInst, NULL);
	hButton_SeatNum9 = CreateWindow(TEXT("button"), TEXT("9"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		900, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM9, g_hInst, NULL);
}
//void SelSeatToLogin():자리선택화면->로그인화면
void SelSeatToLogin() {
	//자리선택화면->로그인화면
	selwindow = 1;								//로그인화면으로 설정
	//자리선택화면 끄기
	
	if (selwindow != 0)ShowWindow(hStatic_Sn, SW_HIDE);
	if (selwindow != 0)ShowWindow(hEdit_Sn, SW_HIDE);
	if (selwindow != 0)ShowWindow(hButton_SelSeat, SW_HIDE);

	if (selwindow != 0)ShowWindow(hButton_SeatNum0, SW_HIDE);
	if (selwindow != 0)ShowWindow(hButton_SeatNum1, SW_HIDE);
	if (selwindow != 0)ShowWindow(hButton_SeatNum2, SW_HIDE);
	if (selwindow != 0)ShowWindow(hButton_SeatNum3, SW_HIDE);
	if (selwindow != 0)ShowWindow(hButton_SeatNum4, SW_HIDE);
	if (selwindow != 0)ShowWindow(hButton_SeatNum5, SW_HIDE);
	if (selwindow != 0)ShowWindow(hButton_SeatNum6, SW_HIDE);
	if (selwindow != 0)ShowWindow(hButton_SeatNum7, SW_HIDE);
	if (selwindow != 0)ShowWindow(hButton_SeatNum8, SW_HIDE);
	if (selwindow != 0)ShowWindow(hButton_SeatNum9, SW_HIDE);

	//로그인화면 켜기
	/////로그인화면 컨트롤
//로그인 아이디 스태틱
	hStatic_Login_Id = CreateWindow(TEXT("static"), TEXT("ID"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 0, 80, 20, hWnd, (HMENU)ID_S_LOGIN_ID, g_hInst, NULL);
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

	//가입가기 버튼
	hButton_Go_Join = CreateWindow(TEXT("button"), TEXT("GOJOIN"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 100, 200, 50, hWnd, (HMENU)ID_B_GO_JOIN, g_hInst, NULL);
	///
}
//void LoginToUser() :로그인화면에서 사용화면으로
void LoginToUser() {
	selwindow = 4;							//사용화면
	InvalidateRect(hWnd, NULL, TRUE);
	//로그인화면 끄기
	if (selwindow != 1)ShowWindow(hStatic_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hStatic_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Login, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Join, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Order, SW_HIDE);
	//사용화면 켜기
	///사용화면 컨트롤
	//수정 가기 버튼
	hButton_User_Go_Update = CreateWindow(TEXT("button"), TEXT("GOUPDATE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 400, 200, 65, hWnd, (HMENU)ID_B_USER_GO_UPDATE, g_hInst, NULL);
	//주문가기 버튼
	hButton_Go_Order = CreateWindow(TEXT("button"), TEXT("ORDER"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 200, 200, 50, hWnd, (HMENU)ID_B_GO_ORDER, g_hInst, NULL);
	///
//로그아웃 버튼
	hButton_Logout = CreateWindow(TEXT("button"), TEXT("LOGOUT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		610, 50, 200, 65, hWnd, (HMENU)ID_B_LOGOUT, g_hInst, NULL);
	//탈퇴 버튼
	hButton_Delete = CreateWindow(TEXT("button"), TEXT("DELETE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 50, 200, 65, hWnd, (HMENU)ID_B_DELETE, g_hInst, NULL);
	//타이머 에디트
	hEdit_Timer = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		610, 200, 100, 100, hWnd, (HMENU)ID_E_TIMER, g_hInst, NULL);
	// 읽기 전용 속성 적용
	SendMessage(hEdit_Timer, EM_SETREADONLY, TRUE, 0);
	//타이머 스태틱
	hStatic_Timer = CreateWindow(TEXT("static"), TEXT("TIME"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		610, 150, 40, 20, hWnd, (HMENU)ID_S_TIMER, g_hInst, NULL);
	///
}
//void OrderToLoginOrUser():주문에서 남은시간유무에따라 로그인화면/사용화면으로
void OrderToLoginOrUser() {
	if (selafterorder == 1)selwindow = 1;				//남은 시간 0일때 로그인화면으로 설정
	else selwindow = 4;											//..아닐때 사용화면으로 설정
	InvalidateRect(hWnd, NULL, TRUE);
	//주문화면 끄기
	if (selwindow != 3)ShowWindow(hStatic_Item, SW_HIDE);
	if (selwindow != 3)ShowWindow(hListBox_ItemList, SW_HIDE);
	if (selwindow != 3)ShowWindow(hStatic_OrderCount, SW_HIDE);
	if (selwindow != 3)ShowWindow(hEdit_OrderCount, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_Order, SW_HIDE);

	if (selwindow != 3)ShowWindow(hButton_OrderCount0, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount1, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount2, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount3, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount4, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount5, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount6, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount7, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount8, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount9, SW_HIDE);
	//남은시간 0일때 로그인화면으로 
	if (selafterorder == 1) {
		selafterorder = 0;
		/////로그인화면 컨트롤
//로그인 아이디 스태틱
		hStatic_Login_Id = CreateWindow(TEXT("static"), TEXT("ID"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			10, 0, 80, 20, hWnd, (HMENU)ID_S_LOGIN_ID, g_hInst, NULL);
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

		//가입가기 버튼
		hButton_Go_Join = CreateWindow(TEXT("button"), TEXT("GOJOIN"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 100, 200, 50, hWnd, (HMENU)ID_B_GO_JOIN, g_hInst, NULL);
	}
	//남은 시간있을때 사용화면으로
	else {
		///사용화면 컨트롤
		//가입 및 수정 가기 버튼
		hButton_User_Go_Update = CreateWindow(TEXT("button"), TEXT("GOUPDATE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 400, 200, 65, hWnd, (HMENU)ID_B_USER_GO_UPDATE, g_hInst, NULL);
		//주문가기 버튼
		hButton_Go_Order = CreateWindow(TEXT("button"), TEXT("ORDER"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 200, 200, 50, hWnd, (HMENU)ID_B_GO_ORDER, g_hInst, NULL);
		///
//로그아웃 버튼
		hButton_Logout = CreateWindow(TEXT("button"), TEXT("LOGOUT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			610, 50, 200, 65, hWnd, (HMENU)ID_B_LOGOUT, g_hInst, NULL);
		//탈퇴 버튼
		hButton_Delete = CreateWindow(TEXT("button"), TEXT("DELETE"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			910, 50, 200, 65, hWnd, (HMENU)ID_B_DELETE, g_hInst, NULL);
		//타이머 에디트
		hEdit_Timer = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			610, 200, 100, 100, hWnd, (HMENU)ID_E_TIMER, g_hInst, NULL);
		// 읽기 전용 속성 적용
		SendMessage(hEdit_Timer, EM_SETREADONLY, TRUE, 0);
		//타이머 스태틱
		hStatic_Timer = CreateWindow(TEXT("static"), TEXT("TIME"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			610, 150, 40, 20, hWnd, (HMENU)ID_S_TIMER, g_hInst, NULL);
		///
	}
}
//void LoginToOrder():로그인에서 주문화면으로
void LoginToOrder() {
	selwindow = 3;																		//주문화면으로
	InvalidateRect(hWnd, NULL, TRUE);
	//로그인화면 끄기
	if (selwindow != 1)ShowWindow(hStatic_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hStatic_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Login, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Join, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Order, SW_HIDE);
	//주문화면 켜기
	///주문화면 컨트롤
//아이템 스태틱
	hStatic_Item = CreateWindow(TEXT("static"), TEXT("ITEM"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 0, 40, 20, hWnd, (HMENU)ID_S_ITEM, g_hInst, NULL);
	//아이템리스트 리스트박스
	hListBox_ItemList = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_SORT | LBS_NOTIFY,
		10, 100, 400, 200, hWnd, (HMENU)ID_L_ITEM, g_hInst, NULL);
	//아이템주문갯수 스태틱
	hStatic_OrderCount = CreateWindow(TEXT("static"), TEXT("Count(2자리)"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		510, 0, 100, 20, hWnd, (HMENU)ID_S_ORDERCOUNT, g_hInst, NULL);
	//아이템주문갯수 에디트
	hEdit_OrderCount = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		510, 50, 100, 50, hWnd, (HMENU)ID_E_ORDERCOUNT, g_hInst, NULL);
	// 읽기 전용 속성 적용
	SendMessage(hEdit_OrderCount, EM_SETREADONLY, TRUE, 0);
	//주문 버튼
	hButton_Order = CreateWindow(TEXT("button"), TEXT("ORDER"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 10, 200, 50, hWnd, (HMENU)ID_B_ORDER, g_hInst, NULL);
	///
	//주문수량 버튼
	hButton_OrderCount0 = CreateWindow(TEXT("button"), TEXT("0"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT0, g_hInst, NULL);
	hButton_OrderCount1 = CreateWindow(TEXT("button"), TEXT("1"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT1, g_hInst, NULL);
	hButton_OrderCount2 = CreateWindow(TEXT("button"), TEXT("2"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		200, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT2, g_hInst, NULL);
	hButton_OrderCount3 = CreateWindow(TEXT("button"), TEXT("3"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		300, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT3, g_hInst, NULL);
	hButton_OrderCount4 = CreateWindow(TEXT("button"), TEXT("4"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		400, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT4, g_hInst, NULL);
	hButton_OrderCount5 = CreateWindow(TEXT("button"), TEXT("5"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		500, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT5, g_hInst, NULL);
	hButton_OrderCount6 = CreateWindow(TEXT("button"), TEXT("6"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		600, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT6, g_hInst, NULL);
	hButton_OrderCount7 = CreateWindow(TEXT("button"), TEXT("7"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		700, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT7, g_hInst, NULL);
	hButton_OrderCount8 = CreateWindow(TEXT("button"), TEXT("8"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		800, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT8, g_hInst, NULL);
	hButton_OrderCount9 = CreateWindow(TEXT("button"), TEXT("9"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		900, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT9, g_hInst, NULL);
	///

	//아이템리스트 초기화
	SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"DRINK");
	SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"FOOD");
	SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"PC");
}
//void JoinOrUpdateToLogin():가입 및 수정에서 로그인으로
void JoinOrUpdateToLogin() {
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

	//가입가기 버튼
	hButton_Go_Join = CreateWindow(TEXT("button"), TEXT("GOJOIN"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 100, 200, 50, hWnd, (HMENU)ID_B_GO_JOIN, g_hInst, NULL);
}
//void ToSelSeat():다시 자리선택화면으로
void ToSelSeat() {
	//자리선택화면 삭제
	DestroyWindow(hStatic_Sn);
	DestroyWindow(hEdit_Sn);
	DestroyWindow(hButton_SelSeat);
	DestroyWindow(hButton_SeatNum0);
	DestroyWindow(hButton_SeatNum1);
	DestroyWindow(hButton_SeatNum2);
	DestroyWindow(hButton_SeatNum3);
	DestroyWindow(hButton_SeatNum4);
	DestroyWindow(hButton_SeatNum5);
	DestroyWindow(hButton_SeatNum6);
	DestroyWindow(hButton_SeatNum7);
	DestroyWindow(hButton_SeatNum8);
	DestroyWindow(hButton_SeatNum9);
	//가입화면 삭제
	DestroyWindow(hStatic_Id);
	DestroyWindow(hEdit_Id);
	DestroyWindow(hStatic_Pw);
	DestroyWindow(hEdit_Pw);
	DestroyWindow(hStatic_Name);
	DestroyWindow(hEdit_Name);
	DestroyWindow(hStatic_Pn);
	DestroyWindow(hEdit_Pn);
	DestroyWindow(hStatic_Addr);
	DestroyWindow(hEdit_Addr);
	DestroyWindow(hStatic_Birth);
	DestroyWindow(hEdit_Birth);
	//수정화면 삭제
	DestroyWindow(hStatic_Id);
	DestroyWindow(hEdit_Id);
	DestroyWindow(hStatic_Pw);
	DestroyWindow(hEdit_Pw);
	DestroyWindow(hStatic_Name);
	DestroyWindow(hEdit_Name);
	DestroyWindow(hStatic_Pn);
	DestroyWindow(hEdit_Pn);
	DestroyWindow(hStatic_Addr);
	DestroyWindow(hEdit_Addr);
	DestroyWindow(hStatic_Birth);
	DestroyWindow(hEdit_Birth);
	DestroyWindow(hButton_Join);
	DestroyWindow(hButton_Update);
	//사용화면 삭제
	DestroyWindow(hButton_User_Go_Update);
	DestroyWindow(hButton_Go_Order);
	DestroyWindow(hButton_Logout);
	DestroyWindow(hButton_Delete);
	DestroyWindow(hEdit_Timer);
	DestroyWindow(hStatic_Timer);
	//주문화면 삭제
	DestroyWindow(hStatic_Item);
	DestroyWindow(hListBox_ItemList);
	DestroyWindow(hStatic_OrderCount);
	DestroyWindow(hEdit_OrderCount);
	DestroyWindow(hButton_Order);
	DestroyWindow(hButton_OrderCount0);
	DestroyWindow(hButton_OrderCount1);
	DestroyWindow(hButton_OrderCount2);
	DestroyWindow(hButton_OrderCount3);
	DestroyWindow(hButton_OrderCount4);
	DestroyWindow(hButton_OrderCount5);
	DestroyWindow(hButton_OrderCount6);
	DestroyWindow(hButton_OrderCount7);
	DestroyWindow(hButton_OrderCount8);
	DestroyWindow(hButton_OrderCount9);
	//로그인화면 삭제
	DestroyWindow(hStatic_Login_Id);
	DestroyWindow(hEdit_Login_Id);
	DestroyWindow(hStatic_Login_Pw);
	DestroyWindow(hEdit_Login_Pw);
	DestroyWindow(hButton_Login);
	DestroyWindow(hButton_Go_Join);
	DestroyWindow(hButton_Go_Order);

	selwindow = 0;																						//자리선택화면 설정
	InvalidateRect(hWnd, NULL, TRUE);
	///가입 및 수정화면 끄기
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

	//사용화면 끄기
	if (selwindow != 4)ShowWindow(hButton_User_Go_Update, SW_HIDE);
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

	if (selwindow != 3)ShowWindow(hButton_OrderCount0, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount1, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount2, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount3, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount4, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount5, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount6, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount7, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount8, SW_HIDE);
	if (selwindow != 3)ShowWindow(hButton_OrderCount9, SW_HIDE);
	//로그인화면 끄기
	if (selwindow != 1)ShowWindow(hStatic_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hStatic_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Login, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Join, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Order, SW_HIDE);
	//자리선택화면 켜기
	///자리선택화면 컨트롤

//자리번호 스태틱
	hStatic_Sn = CreateWindow(TEXT("static"), TEXT("SN(2자리)"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 0, 80, 20, hWnd, (HMENU)ID_S_SN, g_hInst, NULL);
	//자리번호 에디트
	hEdit_Sn = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 50, 100, 50, hWnd, (HMENU)ID_E_SN, g_hInst, NULL);
	// 읽기 전용 속성 적용
	SendMessage(hEdit_Sn, EM_SETREADONLY, TRUE, 0);
	//자리선택 버튼
	hButton_SelSeat = CreateWindow(TEXT("button"), TEXT("SELECTSEAT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 10, 200, 50, hWnd, (HMENU)ID_B_SELSEAT, g_hInst, NULL);

	//자리번호 버튼
	hButton_SeatNum0 = CreateWindow(TEXT("button"), TEXT("0"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM0, g_hInst, NULL);
	hButton_SeatNum1 = CreateWindow(TEXT("button"), TEXT("1"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM1, g_hInst, NULL);
	hButton_SeatNum2 = CreateWindow(TEXT("button"), TEXT("2"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		200, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM2, g_hInst, NULL);
	hButton_SeatNum3 = CreateWindow(TEXT("button"), TEXT("3"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		300, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM3, g_hInst, NULL);
	hButton_SeatNum4 = CreateWindow(TEXT("button"), TEXT("4"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		400, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM4, g_hInst, NULL);
	hButton_SeatNum5 = CreateWindow(TEXT("button"), TEXT("5"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		500, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM5, g_hInst, NULL);
	hButton_SeatNum6 = CreateWindow(TEXT("button"), TEXT("6"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		600, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM6, g_hInst, NULL);
	hButton_SeatNum7 = CreateWindow(TEXT("button"), TEXT("7"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		700, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM7, g_hInst, NULL);
	hButton_SeatNum8 = CreateWindow(TEXT("button"), TEXT("8"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		800, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM8, g_hInst, NULL);
	hButton_SeatNum9 = CreateWindow(TEXT("button"), TEXT("9"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		900, 200, 50, 50, hWnd, (HMENU)ID_B_SEATNUM9, g_hInst, NULL);
}
//void LoginToJoin():로그인에서 가입 으로
void LoginToJoin() {
	selwindow = 2;																																	//가입 및 수정화면으로 설정
	InvalidateRect(hWnd, NULL, TRUE);
	//로그인화면 끄기
	if (selwindow != 1)ShowWindow(hStatic_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hStatic_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Login, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Join, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Order, SW_HIDE);
	//가입 및 수정화면으로 켜기
	///가입 및 수정화면 컨트롤
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
	///
}
//void UserToOrder():사용화면에서 주문화면으로
void UserToOrder() {
	selwindow = 3;														//주문화면으로 설정
	InvalidateRect(hWnd, NULL, TRUE);
	//로그인화면 끄기
	if (selwindow != 1)ShowWindow(hStatic_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Id, SW_HIDE);
	if (selwindow != 1)ShowWindow(hStatic_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hEdit_Login_Pw, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Login, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Join, SW_HIDE);
	if (selwindow != 1)ShowWindow(hButton_Go_Order, SW_HIDE);
	//사용화면끄기
	if (selwindow != 4)ShowWindow(hButton_User_Go_Update, SW_HIDE);
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
	//아이템리스트 리스트박스
	hListBox_ItemList = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_SORT | LBS_NOTIFY,
		10, 100, 400, 200, hWnd, (HMENU)ID_L_ITEM, g_hInst, NULL);
	//아이템주문갯수 스태틱
	hStatic_OrderCount = CreateWindow(TEXT("static"), TEXT("Count(2자리)"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		510, 0, 100, 20, hWnd, (HMENU)ID_S_ORDERCOUNT, g_hInst, NULL);
	//아이템주문갯수 에디트
	hEdit_OrderCount = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		510, 50, 100, 50, hWnd, (HMENU)ID_E_ORDERCOUNT, g_hInst, NULL);
	// 읽기 전용 속성 적용
	SendMessage(hEdit_OrderCount, EM_SETREADONLY, TRUE, 0);
	//주문 버튼
	hButton_Order = CreateWindow(TEXT("button"), TEXT("ORDER"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		910, 10, 200, 50, hWnd, (HMENU)ID_B_ORDER, g_hInst, NULL);
	///
	//주문수량 버튼
	hButton_OrderCount0 = CreateWindow(TEXT("button"), TEXT("0"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT0, g_hInst, NULL);
	hButton_OrderCount1 = CreateWindow(TEXT("button"), TEXT("1"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT1, g_hInst, NULL);
	hButton_OrderCount2 = CreateWindow(TEXT("button"), TEXT("2"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		200, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT2, g_hInst, NULL);
	hButton_OrderCount3 = CreateWindow(TEXT("button"), TEXT("3"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		300, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT3, g_hInst, NULL);
	hButton_OrderCount4 = CreateWindow(TEXT("button"), TEXT("4"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		400, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT4, g_hInst, NULL);
	hButton_OrderCount5 = CreateWindow(TEXT("button"), TEXT("5"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		500, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT5, g_hInst, NULL);
	hButton_OrderCount6 = CreateWindow(TEXT("button"), TEXT("6"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		600, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT6, g_hInst, NULL);
	hButton_OrderCount7 = CreateWindow(TEXT("button"), TEXT("7"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		700, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT7, g_hInst, NULL);
	hButton_OrderCount8 = CreateWindow(TEXT("button"), TEXT("8"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		800, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT8, g_hInst, NULL);
	hButton_OrderCount9 = CreateWindow(TEXT("button"), TEXT("9"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		900, 300, 50, 50, hWnd, (HMENU)ID_B_ORDERCOUNT9, g_hInst, NULL);
	//

	//아이템리스트 초기화
	SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"DRINK");
	SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"FOOD");
	SendMessage(hListBox_ItemList, LB_ADDSTRING, 0, (LPARAM)"PC");
}