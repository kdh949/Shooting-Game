/*
* 1. ¿ÞÂÊ ºñÇà±â ¾È´ê°Ô(1Ä­ ¶ç¿ì±â)
* 2. ºñÇà±â(l-o-l), ÀûµðÀÚÀÎ (¬Ø)
* 3. »ö»ó Á¤ÇÏ´Â °Í -ºñÇà±â : Èò»ö, ÃÑ¾Ë: ¹ÎÆ®/ÇÏ´Ã»ö, Å×µÎ¸®: ±×´ë·Î, Àû: ±×´ë·Î,
* 4. º®¸éÀÌ ¿òÁ÷ÀÌ°í, ÀûÀÌ ±ôºý°Å¸®°í -> ¾î·Á¿ò È®ÀÎ
* 5. ³­ÀÌµµ (ÀûÀÌ ÃâÇöÇÏ´Â ¼Óµµ)
* 6. ÃÑ¾Ë¼Óµµ Á¶Àý ¼³¸í  (+, - ¼³¸í)
* 7. Á¾·áÈ­¸éÀ» È­·ÁÇÏ°Ô
* 8. ÀûÇÑÅ× ºÎµúÇûÀ»¶§ ºñÇà±â »ö»ó »¡°£»öÀ¸·Î (0.3ÃÊ Á¤µµ) ¹Ù²ñ
* 9. °¨Á¡±âÁØÀÌ ºñÇà±â ¾îµðµçÁö ´ê¾ÒÀ» ¶§
* 10. *ÆÄÀÏÆ÷ÀÎÅÍ ÀÌ¿ë -> ¼øÀ§Ç¥(Á¡¼ö ³ôÀº ¼ø Á¤·Ä/µî¼ö), ´Ð³×ÀÓ ÀÔ·Â/10µî±îÁö¸¸, score.txt (´Ð³×ÀÓ Á¡¼ö\n) //¿Ï·á=¾Æ¸¶µµ
* 11. °è¼Ó ÇÏ°Ú´ÂÁö È®ÀÎ 
*/

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <WinUser.h>
#include <stdlib.h>
#include <time.h>

/* Áö¿¬ ÇÔ¼ö */
void delay(int n) {
	Sleep(n);
}

/* gotoxy ÇÔ¼ö */
void gotoxy(int x, int y) {
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

/* cmdÃ¢ Ä¿¼­ Ç¥½Ã Á¦¾îÇÔ¼ö */
void showcursor(int bShow) {
	CONSOLE_CURSOR_INFO CurInfo = { 20, bShow }; //Ä¿¼­ ±¸Á¶Ã¼ º¯¼ö·Î ¼±¾ðÇÏ°í, Ä¿¼­ÀÇ Å©±â¿Í ¼û±è¿©ºÎ·Î ÃÊ±âÈ­ 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void color(int n) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), n);
}

#define BX 20 
#define BY 1  
#define BW 80 // °ÔÀÓÆÇÀÇ ³ÐÀÌ
#define BH 26 // °ÔÀÓÆÇÀÇ ³ôÀÌ
#define UX 50 // ÇÃ·¹ÀÌ¾î ÃÊ±â À§Ä¡
#define UY 12 // ÇÃ·¹ÀÌ¾î ÃÊ±â À§Ä¡
#define MAXBULLET 8
#define MAXENEMY 10

BOOL IsKeyDown(int Key) {
	return ((GetAsyncKeyState(Key) & 0x8000) != 0);
	// GetAsyncKeyState : ÇöÀç ¹«½¼ Å°°¡ ÀÔ·ÂµÇ´ÂÁö È®ÀÎ
	// 0x8000 : ÀÌÀü¿¡ ´©¸¥ ÀûÀÌ ¾ø°í È£Ãâ ½ÃÁ¡¿¡´Â ´­·ÁÀÖ´Â »óÅÂ
}

struct player {
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

	ScoreBoard(1, 1, 2); //test °ð »èÁ¦¿¹Á¤

	char nick[10];

	printf("´Ð³×ÀÓÀ» ÀÔ·ÂÇØÁÖ¼¼¿ä!: ");
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
	showcursor(0); //Ä¿¼­ ¼û±â±â
	gotoxy(player.x, player.y);
	printf("U¡ÚU");

	PrintWall();

	int count = 0;
	while (1) {
		PrintFloor();
		PrintWall();
		gotoxy(player.x, player.y);
		printf("U¡ÜU");
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
					printf("U¡ÜU");
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
					printf("U¡ÜU");
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
					printf("U¡ÜU");
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
					printf("U¡ÜU");
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
		if (IsKeyDown(0x50)) // PÅ°
		{
			gotoxy(35, 28);
			system("PAUSE");
		}
		if (count % 10 == 0)
		{
			if (IsKeyDown(0x58)) // XÅ°
			{
				enemyframe -= 1;
				if (enemyframe <= 1)
					enemyframe = 1;
			}
			if (IsKeyDown(0x5A)) // ZÅ°
			{
				enemyframe += 1;
				if (enemyframe >= 6)
					enemyframe = 6;
			}
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

	ScoreBoard(nick, score, 1); //ÃÖÁ¾ Á¡¼ö¸¦ ÆÄÀÏ¿¡ ÀúÀå
	system("cls");

	char set;

	gotoxy(UX, UY);
	printf(" Game Over");

	gotoxy(UX, UY + 2);
	printf("ÃÖÁ¾Á¡¼ö : %d", score);

	gotoxy(UX, UY+10);
	ScoreBoard(nick, score, 2); //¼øÀ§ÆÇ Ãâ·Â

	gotoxy(UX, UY-10);
	printf("´Ù½ÃÇÏ±â_R / Á¾·á_Q");
	while (1) {
		if (IsKeyDown(0x52)) //RÅ°
			goto replay;
		if(IsKeyDown(0x51)) //QÅ°
			break;
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
	Sleep(1000);
	system("cls");
	Sleep(1000);
	getch();

	return 0;
}

void PlayerHit() {
	for (int i = 0; i < MAXENEMY; i++)
	{
		if (enemy[i].exist == FALSE) continue;
		if (enemy[i].y == player.y && abs(enemy[i].x - player.x) <= 0) //abs : Àý´ñ°ª ÇÔ¼ö
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
				case 1:// ¡è
					enemy[i].direction = rand() % 3 + 4;
					enemy[i].count += 1;
					break;
				case 2:// ¢Ö
					enemy[i].direction = 4;
					enemy[i].count += 1;
					break;
				case 3:// ¡æ
					enemy[i].direction = rand() % 3 + 6;
					enemy[i].count += 1;
					break;
				case 4:// ¢Ù
					enemy[i].direction = 2;
					enemy[i].count += 1;
					break;
				case 5:// ¡é
					enemy[i].direction = (rand() % 3 + 7) % 8 + 1;
					enemy[i].count += 1;
					break;
				case 6:// ¢×
					enemy[i].direction = 8;
					enemy[i].count += 1;
					break;
				case 7:// ¡ç
					enemy[i].direction = rand() % 3 + 2;
					enemy[i].count += 1;
					break;
				case 8:// ¢Ø
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
				case 1: // ¡è
					enemy[i].y -= 1;
					break;
				case 2: // ¢Ö
					enemy[i].y -= 1;
					enemy[i].x += 1;
					break;
				case 3: // ¡æ
					enemy[i].x += 2;
					break;
				case 4: // ¢Ù
					enemy[i].x += 1;
					enemy[i].y += 1;
					break;
				case 5: // ¡é
					enemy[i].y += 1;
					break;
				case 6: // ¢×
					enemy[i].x -= 1;
					enemy[i].y += 1;
					break;
				case 7: // ¡ç
					enemy[i].x -= 2;
					break;
				case 8: // ¢Ø
					enemy[i].x -= 1;
					enemy[i].y -= 1;
					break;
				}
				gotoxy(enemy[i].x, enemy[i].y);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
				puts("¡Ú");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			}
		}
	}
}

void Enemyfall() {
	for (int i = 0; i < MAXENEMY; i++)
	{
		if (enemy[i].exist == FALSE)
			continue;
		for (int j = 0; j < MAXBULLET; j++)
		{
			if (enemy[i].y == pBullet[j].y && abs(enemy[i].x - pBullet[j].x) <= 2)
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

/* ÃÑ ½î´Â ÇÔ¼ö */
void MoveBullet() {
	for (int i = 0; i < MAXBULLET; i++)
	{
		if (pBullet[i].exist == TRUE) {
			gotoxy(pBullet[i].x, pBullet[i].y);
			printf("  ");
			pBullet[i].y--;
			gotoxy(pBullet[i].x, pBullet[i].y);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
			printf("||");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			if (pBullet[i].y <= 1) {
				gotoxy(pBullet[i].x, pBullet[i].y);
				printf("  ");
				pBullet[i].exist = FALSE;
			}
		}
	}
}

void TextEnemyFrame(int frame) {
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
	printf("ÃÑ¾Ë ¼Óµµ : %d", textframe);
}

void PrintWall() {
	gotoxy(BX, 0);
	printf("¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì");
	gotoxy(BX, 27);
	printf("¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì¢Ì");

	//for (int i = BX; i < BW + 1; i++) {
	//	gotoxy(i, 0);
	//	printf("¢Ì");
	//	gotoxy(i, 27);
	//	printf("¢Ì");
	//}


	for (int i = BY; i < BH + 1; i++)
	{
		gotoxy(20, i);
		printf("¢Ì");
		gotoxy(80, i);
		printf("¢Ì");
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
		printf("¢¾ ¢¾ ¢¾");
		break;
	case 2:
		gotoxy(BW + 7, 10);
		printf("¢¾ ¢¾ ¢½");
		break;
	case 1:
		gotoxy(BW + 7, 10);
		printf("¢¾ ¢½ ¢½");
		break;
	case 0:
		gotoxy(BW + 7, 10);
		printf("¢½ ¢½ ¢½");
		break;
	}

	gotoxy(BW + 7, 13);
	printf("ÀÏ½ÃÁ¤Áö : P");

	gotoxy(BW + 7, 15);
	printf("ÃÑ¾Ë ¼Óµµ Á¶Àý : Z, X");

	gotoxy(BW + 7, 17);
	printf("°ø°Ý : SPACE");

	gotoxy(BW + 7, 7);
	TextEnemyFrame(enemyframe);

	gotoxy(35, 28);
	printf("                                        ");
}

/*½ºÄÚ¾îº¸µå ÇÔ¼ö, 1=Á¡¼ö ¾²±â, 2= Á¡¼ö ÀÐ±â*/
void ScoreBoard(char nick[10], int new_score, int mode) {
	FILE* fp;

	if (mode == 1) { //¾²±â¸ðµå
		fp = fopen("score.txt", "a");
		fprintf(fp, "%s %d\n", nick, new_score);
		fclose(fp); //Ãâ·Â½ºÆ®¸² ÇØÁ¦
	}

	if (mode == 2) { //ÀÐ±â¸ðµå
		scr list[50];

		fp = fopen("score.txt", "r");

		if (fp == NULL) {
			fp = fopen("score.txt", "w"); //score.txt°¡ ¾ø´Ù¸é »õ·Ó°Ô ÆÄÀÏ»ý¼º
			fclose(fp);
		}

		fp = fopen("score.txt", "r");

		if (fp == NULL) {
			fprintf(stderr, "¿À·ù¹ß»ý!!");
			Sleep(2000);
			exit(0);
		}

		/* µ¥ÀÌÅÍ¸¦ ºÒ·¯¿Í¼­ ¹è¿­¿¡ ÀúÀåÇÏ´Â ºÎºÐ */
		int n = 0; 
		while (feof(fp) == 0) {
			fscanf(fp, "%s %d\n", list[n].nick, &list[n].score);
			n++;
		}
		fclose(fp); //ÀÔ·Â ½ºÆ®¸² ÇØÁ¦

		/* °ª Á¤·Ä ÈÄ ¹è¿­ º¯°æ ºÎºÐ */
		for (int i = 0; i < n - 1; i++)	{
			for (int j = i + 1; j < n; j++)	{
				if (list[i].score < list[j].score) {
					struct_swap(&list[i], &list[j]);
				}
			}
		}

		printf("µî¼ö  %-12s½ºÄÚ¾î\n", "´Ð³×ÀÓ"); //Á¡¼öÆÇ Ãâ·Â »óÀ§ ¶óº§

		/* ¼øÀ§ÆÇ ÃÖÃÊ ½ÇÇà½Ã ¿¹¿Ü¸Þ½ÃÁö Ãâ·ÂºÎºÐ */
		if (list[0].score == 0) { //¼øÀ§¸¦ ­„·ÂÇÒ »ç¶÷ÀÌ ¾ø´Ù¸é
			printf("===== ¼øÀ§°¡ ¾ø½À´Ï´Ù! ====\n");
			return 0; //½ºÄÚ¾îº¸µå Á¾·á
		}

		/* µ¥ÀÌÅÍ Ãâ·Â ºÎºÐ */
		int rank = 0;
		int recursive = 0;
		for (int i = 0; i < n; i++) {
			/* µ¿Á¡ÀÚ ¹ß»ý½Ã ¿¹¿ÜÃ³¸® */
			if (i > 0 && (list[i].score == list[i - 1].score)) { //µ¿Á¡ÀÌ¶ó¸é
				printf(" %-6d%-12s%d\n", rank, list[i].nick, list[i].score);
				recursive++;
				continue;
			}

			rank++;
			rank += recursive;
			if (rank > 10) //10µî±îÁö¸¸ Ãâ·Â
				break;
			printf(" %-6d%-12s%d\n", rank, list[i].nick, list[i].score);
			recursive = 0;
		}
	}

}

/* ±¸Á¶Ã¼ ±³È¯ÇÔ¼ö */
void struct_swap(scr* a, scr* b) {
	scr tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}