#include "Common.h"
#include "Request.h"
extern TCHAR tgId[10], tgName[10], tgPw[10], tgPn[10], tgAddr[10], tgBirth[10];																//아이디,이름,비번,전화번호,주소,생일 문자열 변수들

//주문 시간
extern int orderpctime;

//소켓변수
extern int nReturn;

extern TCHAR tgcmdserver[200];

extern SOCKET clientsock;
//
extern HWND hWnd;

//타이머
extern HWND hEdit_Timer;
//로그인
extern HWND hEdit_Login_Id;
extern HWND hEdit_Login_Pw;
//주문
extern HWND hEdit_OrderCount;
//자리선택
extern HWND hEdit_Sn;
//가입 및 수정
extern HWND hEdit_Id;
extern HWND hEdit_Pw;
extern HWND hEdit_Name;
extern HWND hEdit_Addr;
extern HWND hEdit_Pn;
extern HWND hEdit_Birth;

//자리번호 버튼 횟수 제어
extern int snbtncount;

//주문수량 버튼 횟수 제어
extern int ocbtncount;

//로그인상황의  유저의 변수들
extern TCHAR userid[10];																																					//아이디
extern TCHAR usersn[10];																																					//자리번호
extern TCHAR userpw[10];																																					//비번
extern int pctimei;																																								//시간 인덱스
extern int pctime;																																								//로그인된 유저의 시간
//

//아이템주문갯수
extern TCHAR tgselitemi[10];																																				//인덱스를 문자열로 변환
extern int selitemi;																																								//리스트 박스의 인덱스
extern TCHAR tgitemcount[10];																																	//개수 문자열
//


//유저시간
extern TCHAR pctimebuf[10];																																				//커맨드에서 담아오기
extern TCHAR tpctime[10];																																					//출력하기 위한 문자열
//


//상태 체크 변수
extern BOOL chkseat;																																							//자리선택
extern BOOL chkjoin;																																							//가입
extern BOOL chklogin;																																						//로그인
extern BOOL chktimer;																																							//타이머 제어(1이면 실행/0이면 종료)	
extern BOOL chkendbtn;																																					//로그아웃/탈퇴 

extern BOOL chkorder;																																						//주문
extern BOOL chkupdate;																																					//수정
extern BOOL chkpctime;																																					//남은시간0인지
//
extern int i;
//자리번호 선택 변수들
extern TCHAR tgnum[10];																																					//신청하는 자리번호 문자열

//좌석반환변수
extern int returnsntime;																																//자리반납타이머시간
extern BOOL returnsn;																																						//자리반납수행

//자리선택으로 보내는 변수
extern BOOL gohome;

//문자열 정보 ':' 들어가는지 체크하기
BOOL ChkStringInfo(TCHAR* str) {
	int i = 0;
	for (i = 0; i < lstrlen(str); i++) {
		if (str[i] == ':')return 1;
	}
	return 0;
}

//void RequestLogin():로그인신청
void RequestLogin() {
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
}
//void RequestOrder():주문신청(버튼 또는 텍스트로 두자리 수로)
void RequestOrder() {
	//추가될 기능: 로그인 된 아이디까지 같이 보내기
	lstrcpy(tgcmdserver, "2");
	lstrcat(tgcmdserver, tgselitemi);
	GetWindowText(hEdit_OrderCount, tgitemcount, sizeof(tgitemcount));													//아이템주문갯수 문자열에 담기
	orderpctime = atoi(tgitemcount);
	lstrcat(tgcmdserver, tgitemcount);
	lstrcat(tgcmdserver, "ID:");																														//입력아이디 문자열에 담기
	lstrcat(tgcmdserver, tgId);
	nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);															//서버로 주문 커맨드 보내기

	ocbtncount = 0;																																		//주문수량 버튼 횟수 초기화
	SetWindowText(hEdit_OrderCount, "");
}
//void RequestJoin():가입신청
void RequestJoin() {
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
}
//void RequestUpdate():수정신청
void RequestUpdate() {
	//커맨드,계정 정보 문자열 빈칸으로 초기화
	lstrcpy(tgcmdserver, "");																																	//커맨드
	SetWindowText(hEdit_Id, tgId);
	SetWindowText(hEdit_Name, tgName);
	SetWindowText(hEdit_Pw, tgPw);
	SetWindowText(hEdit_Pn, tgPn);
	SetWindowText(hEdit_Addr, tgAddr);
	SetWindowText(hEdit_Birth, tgBirth);
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
}
//void RequestSelSeat():자리번호신청(버튼 또는 텍스트로 두자리수로)
void RequestSelSeat() {
	lstrcpy(tgcmdserver, "1");
	GetWindowText(hEdit_Sn, tgnum, sizeof(tgnum));																				//자리 선택(02등 숫자 2자리 형식으로)
	lstrcat(tgcmdserver, tgnum);
	nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);														//서버로 자리선택 커맨드 보내기

	snbtncount = 0;																																		//자리번호버튼 횟수 초기화
}
//void RequestLogout():로그아웃신청
void RequestLogout() {
	lstrcpy(tgcmdserver, "");
	lstrcpy(tgcmdserver, "5ID:");
	lstrcat(tgcmdserver, userid);
	lstrcat(tgcmdserver, "PW:");
	lstrcat(tgcmdserver, userpw);
	lstrcat(tgcmdserver, "SN:");
	lstrcat(tgcmdserver, usersn);
	lstrcat(tgcmdserver, ":");
	lstrcat(tgcmdserver, tpctime);																														//남은시간 보내기
	chkendbtn = 1;																																				//로그아웃/탈퇴 수행 했을때로
	nReturn = send(clientsock, tgcmdserver, sizeof(tgcmdserver), 0);																//서버로 로그아웃 커멘드 보내기
	pctime = 0;
	wsprintf(tpctime, "%d", pctime);
	SetWindowText(hEdit_Timer, tpctime);																										//프로시저의 타이머 에디트에 유저 남은 시간 0으로 출력
}
//void RequestDelete():탈퇴신청
void RequestDelete() {
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