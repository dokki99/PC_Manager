#include "Common.h"
#include "Timer.h"
extern HWND hWnd;
//소켓 변수
extern int nReturn;

extern TCHAR tgcmdserver[200];

extern SOCKET clientsock;
//
//타이머
extern HWND hEdit_Timer;
//로그인상황의  유저의 변수들
extern TCHAR userid[10];																																					//아이디
extern TCHAR usersn[10];																																					//자리번호
extern TCHAR userpw[10];																																					//비번
extern int pctimei;																																								//시간 인덱스
extern int pctime;																																								//로그인된 유저의 시간
//
///주문상황의 변수들
//아이템주문갯수
extern TCHAR tgselitemi[10];																																				//인덱스를 문자열로 변환
extern int selitemi;																																								//리스트 박스의 인덱스
extern TCHAR tgitemcount[10];																																	//개수 문자열
//
///
//상태 체크 변수
extern BOOL chklogin;																																						//로그인
extern BOOL chktimer;																																							//타이머 제어(1이면 실행/0이면 종료)	
extern BOOL chkendbtn;																																					//로그아웃/탈퇴 

extern BOOL chkpctime;																																					//남은시간0인지
//
extern int i;
//자리번호 선택 변수들
extern TCHAR tgnum[10];																																					//신청하는 자리번호 문자열
//유저시간
extern TCHAR pctimebuf[10];																																				//커맨드에서 담아오기
extern TCHAR tpctime[10];																																					//출력하기 위한 문자열
//

//좌석반환변수
extern int returnsntime;																																//자리반납타이머시간
extern BOOL returnsn;																																						//자리반납수행

//자리선택으로 보내는 변수
extern BOOL gohome;
//void UserTimer():유저시간타이머 0이되면 로그아웃 처리 신청
void UserTimer() {
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
	}
}
//void SetUserTimer():유저남은시간타이머 재설정
void SetUserTimer() {
	gohome = 1;
	KillTimer(hWnd, 1);																														//타이머 끄기
	chktimer = 0;																																//타이머 끄는 상태로
	SetTimer(hWnd, 1, 1000, NULL);																									//유저 남은 시간 타이머 다시 생성																															//로그아웃 체크 다시 원래 대로 초기화
	pctime = 0;
	wsprintf(tpctime, "%d", pctime);
	SetWindowText(hEdit_Timer, tpctime);																						//프로시저의 타이머 에디트에 유저 남은 시간 0으로  출력
}
//void ReturnSeatTimer():자리반납타이머(로그인버튼 제한시간안에 누르지 않으면 자리반납처리 신청)
void ReturnSeatTimer() {
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