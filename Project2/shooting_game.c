/*
* 1. ���� ����� �ȴ��(1ĭ ����)
* 2. �����(l-o-l), �������� (��)
* 3. ���� ���ϴ� �� -����� : ���, �Ѿ�: ��Ʈ/�ϴû�, �׵θ�: �״��, ��: �״��,
* 4. ������ �����̰�, ���� �����Ÿ��� -> ����� Ȯ��
* 5. ���̵� (���� �����ϴ� �ӵ�)
* 6. �Ѿ˼ӵ� ���� ����  (+, - ����) => ������ ����
* 7. ����ȭ���� ȭ���ϰ�
* 8. ������ �ε������� ����� ���� ���������� (0.3�� ����) �ٲ�
* 9. ���������� ����� ������ ����� ��
* 10. *���������� �̿� -> ����ǥ(���� ���� �� ����/���), �г��� �Է�/10�������, score.txt (�г��� ����\n) //�Ϸ�=�Ƹ���
* 11. ��� �ϰڴ��� Ȯ�� => �Ϸ�
* 12. �Ѿ˼ӵ��� �������� �� �ӵ��� ���������� üũ!!
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


/* ���� �Լ� */
void delay(int n) {
	Sleep(n);
}

/* gotoxy �Լ� */
void gotoxy(int x, int y) {
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

/* cmdâ Ŀ�� ǥ�� �����Լ� */
void showcursor(int bShow) {
	CONSOLE_CURSOR_INFO CurInfo = { 20, bShow }; //Ŀ�� ����ü ������ �����ϰ�, Ŀ���� ũ��� ���迩�η� �ʱ�ȭ 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void color(int n) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), n);
}

#define BX 20 
#define BY 1  
#define BW 80 // �������� ����
#define BH 26 // �������� ����
#define UX 50 // �÷��̾� �ʱ� ��ġ
#define UY 12 // �÷��̾� �ʱ� ��ġ
#define MAXBULLET 8
#define MAXENEMY 10

BOOL IsKeyDown(int Key) {
	return ((GetAsyncKeyState(Key) & 0x8000) != 0);
	// GetAsyncKeyState : ���� ���� Ű�� �ԷµǴ��� Ȯ��
	// 0x8000 : ������ ���� ���� ���� ȣ�� �������� �����ִ� ����
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
	printf("\t\t\t���� ��    ��   ����     ����  ����� �� ��     ��  ���� \n");
	printf("\t\t\t��     ��    ��  ��    ��   ��    ��    ��    �� ���   �� ��      \n");
	printf("\t\t\t���� ����� ��      �� ��      ��   ��    �� �� ��  �� ��  ���\n");
	printf("\t\t\t    �� ��    ��  ��    ��   ��    ��    ��    �� ��  �� �� ��    ��\n");
	printf("\t\t\t���� ��    ��   ����     ����     ��    �� ��   ���  ���� \n");

	printf("\n");

	printf("\t\t\t\t       ����   ����  ��       �� �����\n");
	printf("\t\t\t\t      ��       ��    �� ���   ��� ��      \n");
	printf("\t\t\t\t      ��  ��� ����� �� �� �� �� ����  \n");
	printf("\t\t\t\t      ��    �� ��    �� ��  ��   �� ��      \n");
	printf("\t\t\t\t       ����  ��    �� ��       �� �����\n");


	char nick[10];

	gotoxy(45, 20);
	printf("�г����� �Է����ּ���!: ");
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
	showcursor(0); //Ŀ�� �����
	gotoxy(player.x, player.y);
	printf("U��U");

	PrintWall();

	int count = 0;
	while (1) {
		PrintFloor();
		PrintWall();
		gotoxy(player.x, player.y);
		printf("U��U");
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
					printf("U��U");
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
					printf("U��U");
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
					printf("U��U");
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
					printf("U��U");
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
		if (IsKeyDown(0x50)) // PŰ
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

	ScoreBoard(nick, score, 1); //���� ������ ���Ͽ� ����
	system("cls");

	char set;

	gotoxy(UX, UY);
	printf(" Game Over");

	gotoxy(UX, UY + 2);
	printf("�������� : %d", score);

	gotoxy(UX, UY+10);
	ScoreBoard(nick, score, 2); //������ ���

	gotoxy(UX, UY-10);
	printf("�ٽ��ϱ�_R / ����_Q");
	while (1) {
		if (IsKeyDown(0x52)) //RŰ
			goto replay;
		if(IsKeyDown(0x51)) //QŰ
			break;
	}

	color(0);
	Sleep(1000);
	system("cls");
	Sleep(1000);
	getch();

	return 0;
}

/* �÷��̾�� ���� ���˿��� ���� �Լ� */
void PlayerHit() {
	for (int i = 0; i < MAXENEMY; i++)
	{
		if (enemy[i].exist == FALSE) continue;
		if (enemy[i].y == player.y && abs(enemy[i].x - player.x) <= 1) //abs : ���� �Լ�
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
				case 1:// ��
					enemy[i].direction = rand() % 3 + 4;
					enemy[i].count += 1;
					break;
				case 2:// ��
					enemy[i].direction = 4;
					enemy[i].count += 1;
					break;
				case 3:// ��
					enemy[i].direction = rand() % 3 + 6;
					enemy[i].count += 1;
					break;
				case 4:// ��
					enemy[i].direction = 2;
					enemy[i].count += 1;
					break;
				case 5:// ��
					enemy[i].direction = (rand() % 3 + 7) % 8 + 1;
					enemy[i].count += 1;
					break;
				case 6:// ��
					enemy[i].direction = 8;
					enemy[i].count += 1;
					break;
				case 7:// ��
					enemy[i].direction = rand() % 3 + 2;
					enemy[i].count += 1;
					break;
				case 8:// ��
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
				case 1: // ��
					enemy[i].y -= 1;
					break;
				case 2: // ��
					enemy[i].y -= 1;
					enemy[i].x += 1;
					break;
				case 3: // ��
					enemy[i].x += 2;
					break;
				case 4: // ��
					enemy[i].x += 1;
					enemy[i].y += 1;
					break;
				case 5: // ��
					enemy[i].y += 1;
					break;
				case 6: // ��
					enemy[i].x -= 1;
					enemy[i].y += 1;
					break;
				case 7: // ��
					enemy[i].x -= 2;
					break;
				case 8: // ��
					enemy[i].x -= 1;
					enemy[i].y -= 1;
					break;
				}
				gotoxy(enemy[i].x, enemy[i].y);
				color(RED);
				printf("��");
				color(GREY);
			}
		}
	}
}

/* �Ѿ˰� ���� ���˿��� �����Լ� */
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

/* �� ��� �Լ� */
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
	printf("�̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢�");
	gotoxy(BX, 27);
	printf("�̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢�");

	//for (int i = BX; i < BW + 1; i++) {
	//	gotoxy(i, 0);
	//	printf("��");
	//	gotoxy(i, 27);
	//	printf("��");
	//}


	for (int i = BY; i < BH + 1; i++)
	{
		gotoxy(20, i);
		printf("��");
		gotoxy(80, i);
		printf("��");
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
		printf("�� �� ��");
		color(GREY);
		break;
	case 2:
		gotoxy(BW + 7, 10);
		color(VIOLET);
		printf("�� �� ��");
		color(GREY);
		break;
	case 1:
		gotoxy(BW + 7, 10);
		color(VIOLET);
		printf("�� �� ��");
		color(GREY);
		break;
	case 0:
		gotoxy(BW + 7, 10);
		color(VIOLET);
		printf("�� �� ��");
		color(GREY);
		break;
	}

	gotoxy(BW + 7, 13);
	printf("�Ͻ����� : P");

	gotoxy(BW + 7, 15);
	//printf("�Ѿ� �ӵ� ���� : Z, X");

	gotoxy(BW + 7, 17);
	printf("���� : SPACE");

	gotoxy(BW + 7, 7);
	//TextEnemyFrame(enemyframe);

	gotoxy(35, 28);
	printf("                                        ");
}

/*���ھ�� �Լ�, 1=���� ����, 2= ���� �б�*/
void ScoreBoard(char nick[10], int new_score, int mode) {
	FILE* fp;

	if (mode == 1) { //������
		fp = fopen("score.txt", "a");
		fprintf(fp, "%s %d\n", nick, new_score);
		fclose(fp); //��½�Ʈ�� ����
	}

	if (mode == 2) { //�б���
		scr list[50];

		fp = fopen("score.txt", "r");

		/* ������ ���� ����� ���ϻ���+���ܸ޽��� ��ºκ� */
		if (fp == NULL) {
			fp = fopen("score.txt", "w"); //score.txt�� ���ٸ� ���Ӱ� ���ϻ���
			fclose(fp);

			printf("���  %-12s���ھ�\n", "�г���"); //������ ��� ���� ��
			printf("===== ������ �����ϴ�! =====\n");

			return 0;
		}

		fp = fopen("score.txt", "r");

		if (fp == NULL) {
			fprintf(stderr, "�����߻�!!");
			Sleep(2000);
			exit(0);
		}

		/* �����͸� �ҷ��ͼ� �迭�� �����ϴ� �κ� */
		int n = 0; 
		while (!feof(fp)) {
			fscanf(fp, "%s %d\n", list[n].nick, &list[n].score);
			n++;
		}
		fclose(fp); //�Է� ��Ʈ�� ����

		printf("���  %-12s���ھ�\n", "�г���"); //������ ��� ���� ��

		/* ������ ���� ����� ���ܸ޽��� ��ºκ� */
		if (n == 0) { //������ ����� ����� ���ٸ�
			printf("===== ������ �����ϴ�! =====\n");
			return 0; //���ھ�� ����
		}

		/* �� ���� �� �迭 ���� �κ� */
		for (int i = 0; i < n - 1; i++)	{
			for (int j = i + 1; j < n; j++)	{
				if (list[i].score < list[j].score) {
					struct_swap(&list[i], &list[j]);
				}
			}
		}


		/* ������ ��� �κ� */
		int rank = 0;
		int recursive = 0;
		for (int i = 0; i < n; i++) {
			/* ������ �߻��� ����ó�� */
			if (i > 0 && (list[i].score == list[i - 1].score)) { //�����̶��
				printf(" %-6d%-12s%d\n", rank, list[i].nick, list[i].score);
				recursive++;
				continue;
			}

			rank++;
			rank += recursive;
			if (rank > 10) //10������� ���
				break;
			printf(" %-6d%-12s%d\n", rank, list[i].nick, list[i].score);
			recursive = 0;
		}
	}

}

/* ����ü ��ȯ�Լ� */
void struct_swap(scr* a, scr* b) {
	scr tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}