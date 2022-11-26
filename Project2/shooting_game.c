/* 
* 1. 왼쪽 비행기 안닿게(1칸 띄우기)
* 2. 비행기(l-o-l), 적디자인 (ж)
* 3. 색상 정하는 것 -비행기 : 흰색, 총알: 민트/하늘색, 테두리: 그대로, 적: 그대로, 
* 4. 벽면이 움직이고, 적이 깜빡거리고 => 더블 버퍼링을 이용하여 해결 예정
* 5. 난이도 (적이 출현하는 속도)
* 6. 총알속도 조절 설명  (+, - 설명) 
* 7. 종료화면을 화려하게
* 8. 적한테 부딪혔을때 비행기 색상 빨간색으로 (0.3초 정도) 바뀜
* 9. 감점기준이 비행기 어디든지 닿았을 때
* 10. *파일포인터 이용 -> 순위표(점수 높은 순 정렬/등수), 닉네임 입력/10등까지만, score.txt (닉네임 점수\n) //일부완료
* 11. 계속 하겠는지 확인 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define BX 20 
#define BY 1  
#define BW 80 // 게임판의 넓이
#define BH 26 // 게임판의 높이
#define UX 50 // 플레이어 초기 위치
#define UY 12 // 플레이어 초기 위치
#define MAXBULLET 8
#define MAXENEMY 10

/*===============================*/
/* 더블 버퍼링 테스트 변수 부분(시작) */
HANDLE hBuffer[2];  //버퍼 핸들
int nScreenIndex;   //현재 선택 버퍼가 뭔지 저장
/* 더블 버퍼링 테스트 변수 부분(끝) */
/*===============================*/

/*===============================*/
/* 더블 버퍼링 테스트 함수 부분(시작) */
/* 1. 버퍼 생성 */
void CreatBuffer()
{
	COORD size = { BW, BH };
	CONSOLE_CURSOR_INFO cci;
	SMALL_RECT rect;
	rect.Bottom = 0;
	rect.Left = 0;
	rect.Right = BW;
	rect.Top = BH;

	hBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleScreenBufferSize(hBuffer[0], size);
	SetConsoleWindowInfo(hBuffer[0], TRUE, &rect);

	hBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleScreenBufferSize(hBuffer[1], size);
	SetConsoleWindowInfo(hBuffer[1], TRUE, &rect);

	cci.dwSize = 1;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hBuffer[0], &cci);
	SetConsoleCursorInfo(hBuffer[1], &cci);
}

/*2. 버퍼 쓰기 x, y, 문자 */
void WriteBuffer(int x, int y, char str[]) 
{
	DWORD dw;
	COORD CursorPosition = { x, y };
	SetConsoleCursorPosition(hBuffer[nScreenIndex], CursorPosition);
	WriteFile(hBuffer[nScreenIndex], str, strlen(str), &dw, NULL);
}


/*3. 버퍼 전환*/
void FlippingBuffer() 
{
	//Sleep(33);  //부드러운 플리핑을 위한 0.033초의 딜레이(사람은 1초에 33장의 연속적인 그림을 볼 때 가장 부드럽게 인식하고 합니다. 그래서 0.033초)
	SetConsoleActiveScreenBuffer(hBuffer[nScreenIndex]);
	nScreenIndex = !nScreenIndex;
}

/* 4. 버퍼 내용 지우기 */
void ClearBuffer()
{
	COORD Coor = { BH,BW };
	DWORD dw;
	FillConsoleOutputCharacter(hBuffer[!nScreenIndex], ' ', BH*BW, Coor, &dw);
}

/*5. 버퍼해제*/
void DeleteBuffer() 
{
	CloseHandle(hBuffer[0]);
	CloseHandle(hBuffer[1]);

}

/* [+]버퍼링 전용 색상지정*/
void SetColor(unsigned short color)
{
	SetConsoleTextAttribute(hBuffer[nScreenIndex], color);
}

/* 더블 버퍼링 테스트 함수 부분(끝) */
/*===============================*/


/* 지연 함수 */
void delay(int n) {
	Sleep(n);
}

void gotoxy(int x, int y) { //gotoxy함수 
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void clear() {
	system("cls");
}

/* cmd창 좌표 이동 함수 */
//void gotoxy(int x, int y) {
// 	COORD Cur = { x, y }; //좌표를 정의하는 구조체를 x, y로 초기화
//	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
//}

/* cmd창 커서 표시 제어함수 */
void showcursor(int bShow) {
	CONSOLE_CURSOR_INFO CurInfo = { 20, bShow }; //커서 구조체 변수로 선언하고, 커서의 크기와 숨김여부로 초기화 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void color(int n) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), n);
}


BOOL IsKeyDown(int Key)
{
	return ((GetAsyncKeyState(Key) & 0x8000) != 0);
	// GetAsyncKeyState : 현재 무슨 키가 입력되는지 확인
	// 0x8000 : 이전에 누른 적이 없고 호출 시점에는 눌려있는 상태
}

struct player
{
	int x;
	int y;
	int hp;
	BOOL exist;
} player;

struct bullet
{
	int x;
	int y;
	BOOL exist;
} pBullet[MAXBULLET];

struct Enemy
{
	int x, y;
	int direction;
	int count;
	BOOL exist;
} enemy[MAXENEMY];

struct score {
	char nick[10];
	int score;
}list[50];

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
void int_swap(int* a, int* b);
void char_swap(char a[10], char b[10]);

int score = 0;
int enemyframe = 3;

int main()
{
	CreatBuffer();
	
	player.x = UX;
	player.y = UY;
	player.hp = 3;

	for (int i = 0; i < MAXENEMY; i++)
		enemy[i].count = 0;

	srand((unsigned)time(NULL));
	system("cls");

	//ScoreBoard(1, 1, 2); //test 곧 삭제예정

	//char nick[10];

	//printf("닉네임을 입력해주세요!: ");
	//scanf("%s", nick);
	//system("cls");

	showcursor(0); //커서 숨기기
	//gotoxy(player.x, player.y);
	//printf("U★U");
	
	WriteBuffer(player.x, player.y, "U★U");
	
	
	int count = 0;
	while(1) {
		PrintWall();
		PrintFloor();
		/*gotoxy(player.x, player.y);
		printf("U●U");*/
		WriteBuffer(player.x, player.y, "U★U");

		if (count % 2 == 0)
		{
			if (IsKeyDown(VK_LEFT))
			{
				if (player.x > BX + 2)
				{
					/*gotoxy(player.x, player.y);
					printf("    ");*/
					WriteBuffer(player.x, player.y, "    ");
					FlippingBuffer();
					WriteBuffer(player.x, player.y, "    ");
					player.x--;
					/*gotoxy(player.x, player.y);
					printf("U●U");*/
					WriteBuffer(player.x, player.y, "U★U");
					FlippingBuffer();
					WriteBuffer(player.x, player.y, "U★U");
					
				}
			}
			if (IsKeyDown(VK_RIGHT))
			{
				if (player.x < BW - 5)
				{
					/*gotoxy(player.x, player.y);
					printf("    ");*/
					WriteBuffer(player.x, player.y, "    ");
					FlippingBuffer();
					WriteBuffer(player.x, player.y, "    ");
					player.x++;
					/*gotoxy(player.x, player.y);
					printf("U●U");*/
					WriteBuffer(player.x, player.y, "U★U");
					FlippingBuffer();
					WriteBuffer(player.x, player.y, "U★U");
				}
			}
			if (IsKeyDown(VK_UP))
			{
				if (player.y > 2)
				{
					/*gotoxy(player.x, player.y);
					printf("    ");*/
					WriteBuffer(player.x, player.y, "    ");
					FlippingBuffer();
					WriteBuffer(player.x, player.y, "    ");
					player.y--;
					/*gotoxy(player.x, player.y);
					printf("U●U");*/
					WriteBuffer(player.x, player.y, "U★U");
					FlippingBuffer();
					WriteBuffer(player.x, player.y, "U★U");
				}
			}
			if (IsKeyDown(VK_DOWN))
			{
				if (player.y < 25)
				{
					/*gotoxy(player.x, player.y);
					printf("    ");*/
					WriteBuffer(player.x, player.y, "    ");
					FlippingBuffer();
					WriteBuffer(player.x, player.y, "    ");
					player.y++;
					/*gotoxy(player.x, player.y);
					printf("U●U");*/
					WriteBuffer(player.x, player.y, "U★U");
					FlippingBuffer();
					WriteBuffer(player.x, player.y, "U★U");
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
		if (count % 10 == 0)
		{
			if (IsKeyDown(0x58)) // X키
			{
				enemyframe -= 1;
				if (enemyframe <= 1)
					enemyframe = 1;
			}
			if (IsKeyDown(0x5A)) // Z키
			{
				enemyframe += 1;
				if (enemyframe >= 6)
					enemyframe = 6;
			}
		}
		if (rand() % 5 == 0)
		{
			FlippingBuffer();		
		}
		if (count % enemyframe == 0) {
			MoveEnemy();
		}

		PlayerHit();
		MoveBullet();
		Enemyfall();

		score += 1;
		Sleep(20);
		count++;
		//ClearBuffer();

	}

	DeleteBuffer(); //버퍼링 삭제

	//ScoreBoard(nick, score, 1);


	system("cls");
	gotoxy(UX, UY);
	printf(" Game Over");
	gotoxy(UX, UY + 2);
	printf("최종점수 : %d", score);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
	Sleep(1000);
	system("cls");

	//ScoreBoard(nick, score, 2);

	Sleep(1000);
	getch();
	

	return 0;
}

void PlayerHit()
{
	for (int i = 0; i < MAXENEMY; i++)
	{
		if (enemy[i].exist == FALSE) continue;
		if (enemy[i].y == player.y && abs(enemy[i].x - player.x) <= 0) //abs : 절댓값 함수
		{
			player.hp -= 1;
			if (player.hp <= 0)
				player.hp = 0;
			return;
		}
	}
}

void CreateEnemy()
{
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

void CreateBullet()
{
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

void MoveEnemy()
{
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
				/*gotoxy(enemy[i].x, enemy[i].y);
				printf("  ");*/
				WriteBuffer(enemy[i].x, enemy[i].y, "  ");
				FlippingBuffer();
				WriteBuffer(enemy[i].x, enemy[i].y, "  ");
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
				/*gotoxy(enemy[i].x, enemy[i].y);*/
				//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
				/*puts("★");*/
				SetColor(6);
				WriteBuffer(enemy[i].x, enemy[i].y, "★");
				FlippingBuffer();
				WriteBuffer(enemy[i].x, enemy[i].y, "★");
				SetColor(7);
				//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			}
		}
	}
}

void Enemyfall()
{
	for (int i = 0; i < MAXENEMY; i++)
	{
		if (enemy[i].exist == FALSE)
			continue;
		for (int j = 0; j < MAXBULLET; j++)
		{
			if (enemy[i].y == pBullet[j].y && abs(enemy[i].x - pBullet[j].x) <= 2)
			{
				/*gotoxy(pBullet[j].x, pBullet[j].y);
				printf("  ");*/
				WriteBuffer(pBullet[j].x, pBullet[j].y, "  ");
				enemy[i].exist = FALSE;
				pBullet[j].exist = FALSE;
				/*gotoxy(enemy[i].x, enemy[i].y);
				printf("     ");*/
				WriteBuffer(enemy[i].x, enemy[i].y, "     ");
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
			/*gotoxy(pBullet[i].x, pBullet[i].y);
			printf("  ");*/
			WriteBuffer(pBullet[i].x, pBullet[i].y, "  ");
			pBullet[i].y--;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
			/*gotoxy(pBullet[i].x, pBullet[i].y);
			printf("||");*/
			SetColor(4);
			//WriteBuffer(pBullet[i].x, pBullet[i].y, "||");
			SetColor(7);
			//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			if (pBullet[i].y <= 1) {
				/*gotoxy(pBullet[i].x, pBullet[i].y);
				printf("  ");*/
				WriteBuffer(pBullet[i].x, pBullet[i].y, "  ");
				pBullet[i].exist = FALSE;
			}
		}
	}
}

void TextEnemyFrame(int frame)
{
	int textframe = 0;
	switch (frame)
	{
	case 1:
		textframe = 6;
		break;
	case 2:
		textframe = 5;
		break;
	case 3:
		textframe = 4;
		break;
	case 4:
		textframe = 3;
		break;
	case 5:
		textframe = 2;
		break;
	case 6:
		textframe = 1;
		break;
	}
	//printf("총알 속도 : %d", textframe);
	//WriteBuffer()
}

void PrintWall() {
	//gotoxy(BX, 0);
	//printf("▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩");
	//gotoxy(BX, 27);
	//printf("▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩");

	for (int i = BX; i < BW + 1; i++) {
		//gotoxy(i, 0);
		WriteBuffer(i, 0, "▩");
		//printf("▩");
		
		//gotoxy(i, 27);
		WriteBuffer(i, 27, "▩");
		//printf("▩");
	}

	for (int i = BY; i < BH + 1; i++)
	{
		WriteBuffer(20, i, "▩");
		//gotoxy(20, i);
		//printf("▩");
		WriteBuffer(80, i, "▩");
		//gotoxy(80, i);
		//printf("▩");
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
		printf("♥ ♥ ♥");
		break;
	case 2:
		gotoxy(BW + 7, 10);
		printf("♥ ♥ ♡");
		break;
	case 1:
		gotoxy(BW + 7, 10);
		printf("♥ ♡ ♡");
		break;
	case 0:
		gotoxy(BW + 7, 10);
		printf("♡ ♡ ♡");
		break;
	}

	gotoxy(BW + 7, 13);
	printf("일시정지 : P");

	gotoxy(BW + 7, 15);
	printf("총알 속도 조절 : Z, X");

	gotoxy(BW + 7, 17);
	printf("공격 : SPACE");

	gotoxy(BW + 7, 7);
	TextEnemyFrame(enemyframe);

	gotoxy(35, 28);
	printf("                                        ");
}

/*스코어보드 함수, 1=점수 쓰기, 2= 점수 읽기*/
void ScoreBoard(char nick[10], int new_score, int mode) {
	FILE* fp;
	
	if (mode == 1) { //쓰기모드
		fp = fopen("score.txt", "a");
		fprintf(fp, "%s %d\n", nick, new_score);
		fclose(fp);
	}

	if (mode == 2) { //읽기모드
		fp = fopen("score.txt", "r");

		if (fp == NULL) {
			fprintf(stderr, "오류발생!!");
			exit(0);
		}
		
		/* 데이터를 불러와서 배열에 저장하는 부분 */
		int n = 0;
		while (feof(fp) == 0) {
			fscanf(fp, "%s %d\n", list[n].nick, &list[n].score);
			n++;
		}
		fclose(fp);

		/* 값 정렬 후 배열 변경 부분 */
		for (int i = 0; i < n - 1; i++) 
		{
			for (int j = i + 1; j < n; j++) 
			{
				if (list[i].score < list[j].score) 
				{
					int_swap(&list[i].score, &list[j].score);
					char_swap(list[i].nick, list[j].nick);
				}
			}	
		}

		/* 데이터 출력 부분 */
		printf("등수  %-12s스코어\n", "닉네임"); //점수판 출력 상위 라벨
		if (n == 0) {
			printf("===== 순위가 없습니다! ====");
		}
		int rank = 0;
		int recursive = 0;
		for (int i = 0; i < n; i++) {
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

void int_swap(int *a, int *b) {
	int tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

void char_swap(char a[10], char b[10]) {
	char tmp[10];
	strcpy(tmp, a);
	strcpy(a, b);
	strcpy(b, tmp);
}

