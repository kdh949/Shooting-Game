/*
* 1. 왼쪽 비행기 안닿게(1칸 띄우기)
* 2. 비행기(l-o-l), 적디자인 (ж)
* 3. 색상 정하는 것 -비행기 : 흰색, 총알: 민트/하늘색, 테두리: 그대로, 적: 그대로,
* 4. 벽면이 움직이고, 적이 깜빡거리고 -> 어려움 확인
* 5. 난이도 (적이 출현하는 속도)
* 6. 총알속도 조절 설명  (+, - 설명) => 삭제로 결정
* 7. 종료화면을 화려하게
* 8. 적한테 부딪혔을때 비행기 색상 빨간색으로 (0.3초 정도) 바뀜
* 9. 감점기준이 비행기 어디든지 닿았을 때
* 10. *파일포인터 이용 -> 순위표(점수 높은 순 정렬/등수), 닉네임 입력/10등까지만, score.txt (닉네임 점수\n) //완료=아마도
* 11. 계속 하겠는지 확인 => 완료
* 12. 총알속도가 빨라지면 적 속도도 빨라지는지 체크!!
* 
* 
* ***HP = pink
* ***Enemy = red
* ***Bullet = sky
*/

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <WinUser.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>


/* 지연 함수 */
void delay(int n) {
	Sleep(n);
}

/* gotoxy 함수 */
void gotoxy(int x, int y) {
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

/* cmd창 커서 표시 제어함수 */
void showcursor(int bShow) {
	CONSOLE_CURSOR_INFO CurInfo = { 20, bShow }; //커서 구조체 변수로 선언하고, 커서의 크기와 숨김여부로 초기화 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void color(int n) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), n);
}

#define BX 20 
#define BY 1  
#define BW 80 // 게임판의 넓이
#define BH 26 // 게임판의 높이
#define UX 50 // 플레이어 초기 위치
#define UY 12 // 플레이어 초기 위치
#define MAXBULLET 8
#define MAXENEMY 10

BOOL IsKeyDown(int Key) {
	return ((GetAsyncKeyState(Key) & 0x8000) != 0);
	// GetAsyncKeyState : 현재 무슨 키가 입력되는지 확인
	// 0x8000 : 이전에 누른 적이 없고 호출 시점에는 눌려있는 상태
}

struct _player {
	int x;
	int y;
	int hp;
	BOOL exist;
} player;

struct bullet {
	int x;
	int y;
	BOOL exist;
} pBullet[MAXBULLET];

struct Enemy {
	int x, y;
	int direction;
	int count;
	BOOL exist;
} enemy[MAXENEMY];

typedef struct _Score {
	char nick[10];
	int score;
}scr;

enum _Color { GREY = 7, DARK_GREY, BLUE, GREEN, SKYBLUE, RED, VIOLET, YELLOW, WHITE };

void PlayerHit();
void CreateEnemy();
void CreateBullet();
void PrintWall();
void PrintFloor();
void MoveEnemy();
void MoveBullet();
void TextEnemyFrame(int frame);
void Enemyfall();
void ScoreBoard(char nick[10], int score, int mode);
void struct_swap(scr* a, scr* b);

int score = 0;
int enemyframe = 3;

int main() {
	system("cls");

	printf("\n\n\n");
	printf("\t\t\t■■■ ■    ■   ■■■     ■■■  ■■■■ ■ ■     ■  ■■■ \n");
	printf("\t\t\t■     ■    ■  ■    ■   ■    ■    ■    ■ ■■   ■ ■      \n");
	printf("\t\t\t■■■ ■■■■ ■      ■ ■      ■   ■    ■ ■ ■  ■ ■  ■■\n");
	printf("\t\t\t    ■ ■    ■  ■    ■   ■    ■    ■    ■ ■  ■ ■ ■    ■\n");
	printf("\t\t\t■■■ ■    ■   ■■■     ■■■     ■    ■ ■   ■■  ■■■ \n");

	printf("\n");

	printf("\t\t\t\t       ■■■   ■■■  ■       ■ ■■■■\n");
	printf("\t\t\t\t      ■       ■    ■ ■■   ■■ ■      \n");
	printf("\t\t\t\t      ■  ■■ ■■■■ ■ ■ ■ ■ ■■■  \n");
	printf("\t\t\t\t      ■    ■ ■    ■ ■  ■   ■ ■      \n");
	printf("\t\t\t\t       ■■■  ■    ■ ■       ■ ■■■■\n");


	char nick[10];

	gotoxy(45, 20);
	printf("닉네임을 입력해주세요!: ");
	scanf("%s", nick);

replay:

	player.x = UX;
	player.y = UY;
	player.hp = 3;

	score = 0;
	enemyframe = 3;

	for (int i = 0; i < MAXENEMY; i++)
		enemy[i].count = 0;

	srand((unsigned)time(NULL));
	system("cls");
	showcursor(0); //커서 숨기기
	gotoxy(player.x, player.y);
	printf("U★U");

	PrintWall();

	int count = 0;
	while (1) {
		PrintFloor();
		PrintWall();
		gotoxy(player.x, player.y);
		printf("U●U");
		if (count % 2 == 0)
		{
			if (IsKeyDown(VK_LEFT))
			{
				if (player.x > BX + 2)
				{
					gotoxy(player.x, player.y);
					printf("    ");
					player.x--;
					gotoxy(player.x, player.y);
					printf("U●U");
				}
			}
			if (IsKeyDown(VK_RIGHT))
			{
				if (player.x < BW - 5)
				{
					gotoxy(player.x, player.y);
					printf("    ");
					player.x++;
					gotoxy(player.x, player.y);
					printf("U●U");
				}
			}
			if (IsKeyDown(VK_UP))
			{
				if (player.y > 2)
				{
					gotoxy(player.x, player.y);
					printf("    ");
					player.y--;
					gotoxy(player.x, player.y);
					printf("U●U");
				}
			}
			if (IsKeyDown(VK_DOWN))
			{
				if (player.y < 25)
				{
					gotoxy(player.x, player.y);
					printf("    ");
					player.y++;
					gotoxy(player.x, player.y);
					printf("U●U");
				}
			}
		}
		if (count % 4 == 0)
		{
			if (IsKeyDown(VK_SPACE))
			{
				CreateBullet();
			}
		}
		if (IsKeyDown(VK_ESCAPE) || player.hp <= 0)
			break;
		if (IsKeyDown(0x50)) // P키
		{
			gotoxy(35, 28);
			system("PAUSE");
		}
		if (rand() % 5 == 0)
		{
			CreateEnemy();
		}
		if (count % enemyframe == 0)
			MoveEnemy();

		PlayerHit();
		MoveBullet();
		Enemyfall();

		score += 1;
		Sleep(20);
		count++;
	}

	ScoreBoard(nick, score, 1); //최종 점수를 파일에 저장
	system("cls");

	char set;

	gotoxy(UX, UY);
	printf(" Game Over");

	gotoxy(UX, UY + 2);
	printf("최종점수 : %d", score);

	gotoxy(UX, UY+10);
	ScoreBoard(nick, score, 2); //순위판 출력

	gotoxy(UX, UY-10);
	printf("다시하기_R / 종료_Q");
	while (1) {
		if (IsKeyDown(0x52)) //R키
			goto replay;
		if(IsKeyDown(0x51)) //Q키
			break;
	}

	color(0);
	Sleep(1000);
	system("cls");
	Sleep(1000);
	getch();

	return 0;
}

/* 플레이어와 적의 접촉여부 판정 함수 */
void PlayerHit() {
	for (int i = 0; i < MAXENEMY; i++)
	{
		if (enemy[i].exist == FALSE) continue;
		if (enemy[i].y == player.y && abs(enemy[i].x - player.x) <= 1) //abs : 절댓값 함수
		{
			player.hp -= 1;
			if (player.hp <= 0)
				player.hp = 0;
			return;
		}
	}
}

void CreateEnemy() {
	int starting_point;
	for (int i = 0; i < MAXENEMY; i++)
	{
		if (enemy[i].exist == FALSE)
		{
			starting_point = rand() % 4;
			switch (starting_point)
			{
			case 0:
				enemy[i].x = rand() % 53 + 23;
				enemy[i].y = 3;
				enemy[i].direction = rand() % 3 + 4;
				break;
			case 1:
				enemy[i].x = 73;
				enemy[i].y = rand() % 23 + 2;
				enemy[i].direction = rand() % 3 + 6;
				break;
			case 2:
				enemy[i].x = rand() % 53 + 23;
				enemy[i].y = 24;
				enemy[i].direction = (rand() % 3 + 7) % 8 + 1;
				break;
			case 3:
				enemy[i].x = 23;
				enemy[i].y = rand() % 23 + 2;
				enemy[i].direction = rand() % 3 + 2;
				break;
			}
			enemy[i].exist = TRUE;
			return;
		}
	}
}

void CreateBullet() {
	for (int i = 0; i < MAXBULLET; i++)
	{
		if (pBullet[i].exist == FALSE)
		{
			pBullet[i].x = player.x;
			pBullet[i].y = player.y - 1;
			pBullet[i].exist = TRUE;
			return;
		}
	}
}

void MoveEnemy() {
	for (int i = 0; i < MAXENEMY; i++)
	{
		if (enemy[i].exist == TRUE)
		{
			if (enemy[i].y >= 25 || enemy[i].y <= 2 || enemy[i].x >= 75 || enemy[i].x <= 22)
			{
				switch (enemy[i].direction)
				{
				case 1:// ↑
					enemy[i].direction = rand() % 3 + 4;
					enemy[i].count += 1;
					break;
				case 2:// ↗
					enemy[i].direction = 4;
					enemy[i].count += 1;
					break;
				case 3:// →
					enemy[i].direction = rand() % 3 + 6;
					enemy[i].count += 1;
					break;
				case 4:// ↘
					enemy[i].direction = 2;
					enemy[i].count += 1;
					break;
				case 5:// ↓
					enemy[i].direction = (rand() % 3 + 7) % 8 + 1;
					enemy[i].count += 1;
					break;
				case 6:// ↙
					enemy[i].direction = 8;
					enemy[i].count += 1;
					break;
				case 7:// ←
					enemy[i].direction = rand() % 3 + 2;
					enemy[i].count += 1;
					break;
				case 8:// ↖
					enemy[i].direction = 6;
					enemy[i].count += 1;
					break;
				}
				if (enemy[i].count >= 3)
				{
					enemy[i].exist = FALSE;
					gotoxy(enemy[i].x, enemy[i].y);
					puts("   ");
					enemy[i].count = 0;
				}
			}

			if (enemy[i].exist == TRUE)
			{
				gotoxy(enemy[i].x, enemy[i].y);
				printf("  ");
				switch (enemy[i].direction)
				{
				case 1: // ↑
					enemy[i].y -= 1;
					break;
				case 2: // ↗
					enemy[i].y -= 1;
					enemy[i].x += 1;
					break;
				case 3: // →
					enemy[i].x += 2;
					break;
				case 4: // ↘
					enemy[i].x += 1;
					enemy[i].y += 1;
					break;
				case 5: // ↓
					enemy[i].y += 1;
					break;
				case 6: // ↙
					enemy[i].x -= 1;
					enemy[i].y += 1;
					break;
				case 7: // ←
					enemy[i].x -= 2;
					break;
				case 8: // ↖
					enemy[i].x -= 1;
					enemy[i].y -= 1;
					break;
				}
				gotoxy(enemy[i].x, enemy[i].y);
				color(RED);
				printf("★");
				color(GREY);
			}
		}
	}
}

/* 총알과 적의 접촉여부 판정함수 */
void Enemyfall() {
	for (int i = 0; i < MAXENEMY; i++)
	{
		if (enemy[i].exist == FALSE)
			continue;
		for (int j = 0; j < MAXBULLET; j++)
		{
			if (enemy[i].y == pBullet[j].y && abs(enemy[i].x - pBullet[j].x) <= 1)
			{
				gotoxy(pBullet[j].x, pBullet[j].y);
				printf("  ");
				enemy[i].exist = FALSE;
				pBullet[j].exist = FALSE;
				gotoxy(enemy[i].x, enemy[i].y);
				printf("     ");
				score += 10;
				break;
			}
		}
	}
}

/* 총 쏘는 함수 */
void MoveBullet() {
	for (int i = 0; i < MAXBULLET; i++)
	{
		if (pBullet[i].exist == TRUE) {
			gotoxy(pBullet[i].x, pBullet[i].y);
			printf("  ");
			pBullet[i].y--;
			gotoxy(pBullet[i].x, pBullet[i].y);
			color(SKYBLUE);
			printf("||");
			color(GREY);
			if (pBullet[i].y <= 1) {
				gotoxy(pBullet[i].x, pBullet[i].y);
				printf("  ");
				pBullet[i].exist = FALSE;
			}
		}
	}
}


void PrintWall() {
	gotoxy(BX, 0);
	printf("▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩");
	gotoxy(BX, 27);
	printf("▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩");

	//for (int i = BX; i < BW + 1; i++) {
	//	gotoxy(i, 0);
	//	printf("▩");
	//	gotoxy(i, 27);
	//	printf("▩");
	//}


	for (int i = BY; i < BH + 1; i++)
	{
		gotoxy(20, i);
		printf("▩");
		gotoxy(80, i);
		printf("▩");
	}
}

void PrintFloor()
{
	gotoxy(BW + 7, 5);
	printf("score : %d", score);

	gotoxy(BW + 7, 9);
	printf("Player HP");
	switch (player.hp)
	{
	case 3:
		gotoxy(BW + 7, 10);
		color(VIOLET);
		printf("♥ ♥ ♥");
		color(GREY);
		break;
	case 2:
		gotoxy(BW + 7, 10);
		color(VIOLET);
		printf("♥ ♥ ♡");
		color(GREY);
		break;
	case 1:
		gotoxy(BW + 7, 10);
		color(VIOLET);
		printf("♥ ♡ ♡");
		color(GREY);
		break;
	case 0:
		gotoxy(BW + 7, 10);
		color(VIOLET);
		printf("♡ ♡ ♡");
		color(GREY);
		break;
	}

	gotoxy(BW + 7, 13);
	printf("일시정지 : P");

	gotoxy(BW + 7, 15);
	//printf("총알 속도 조절 : Z, X");

	gotoxy(BW + 7, 17);
	printf("공격 : SPACE");

	gotoxy(BW + 7, 7);
	//TextEnemyFrame(enemyframe);

	gotoxy(35, 28);
	printf("                                        ");
}

/*스코어보드 함수, 1=점수 쓰기, 2= 점수 읽기*/
void ScoreBoard(char nick[10], int new_score, int mode) {
	FILE* fp;

	if (mode == 1) { //쓰기모드
		fp = fopen("score.txt", "a");
		fprintf(fp, "%s %d\n", nick, new_score);
		fclose(fp); //출력스트림 해제
	}

	if (mode == 2) { //읽기모드
		scr list[50];

		fp = fopen("score.txt", "r");

		/* 순위판 최초 실행시 파일생성+예외메시지 출력부분 */
		if (fp == NULL) {
			fp = fopen("score.txt", "w"); //score.txt가 없다면 새롭게 파일생성
			fclose(fp);

			printf("등수  %-12s스코어\n", "닉네임"); //점수판 출력 상위 라벨
			printf("===== 순위가 없습니다! =====\n");

			return 0;
		}

		fp = fopen("score.txt", "r");

		if (fp == NULL) {
			fprintf(stderr, "오류발생!!");
			Sleep(2000);
			exit(0);
		}

		/* 데이터를 불러와서 배열에 저장하는 부분 */
		int n = 0; 
		while (!feof(fp)) {
			fscanf(fp, "%s %d\n", list[n].nick, &list[n].score);
			n++;
		}
		fclose(fp); //입력 스트림 해제

		printf("등수  %-12s스코어\n", "닉네임"); //점수판 출력 상위 라벨

		/* 순위판 최초 실행시 예외메시지 출력부분 */
		if (n == 0) { //순위를 출력할 사람이 없다면
			printf("===== 순위가 없습니다! =====\n");
			return 0; //스코어보드 종료
		}

		/* 값 정렬 후 배열 변경 부분 */
		for (int i = 0; i < n - 1; i++)	{
			for (int j = i + 1; j < n; j++)	{
				if (list[i].score < list[j].score) {
					struct_swap(&list[i], &list[j]);
				}
			}
		}


		/* 데이터 출력 부분 */
		int rank = 0;
		int recursive = 0;
		for (int i = 0; i < n; i++) {
			/* 동점자 발생시 예외처리 */
			if (i > 0 && (list[i].score == list[i - 1].score)) { //동점이라면
				printf(" %-6d%-12s%d\n", rank, list[i].nick, list[i].score);
				recursive++;
				continue;
			}

			rank++;
			rank += recursive;
			if (rank > 10) //10등까지만 출력
				break;
			printf(" %-6d%-12s%d\n", rank, list[i].nick, list[i].score);
			recursive = 0;
		}
	}

}

/* 구조체 교환함수 */
void struct_swap(scr* a, scr* b) {
	scr tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}