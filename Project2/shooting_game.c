/* 
* 1. ���� ����� �ȴ��(1ĭ ����)
* 2. �����(l-o-l), �������� (��)
* 3. ���� ���ϴ� �� -����� : ���, �Ѿ�: ��Ʈ/�ϴû�, �׵θ�: �״��, ��: �״��, 
* 4. ������ �����̰�, ���� �����Ÿ���
* 5. ���̵� (���� �����ϴ� �ӵ�)
* 6. �Ѿ˼ӵ� ���� ����  (+, - ����) 
* 7. ����ȭ���� ȭ���ϰ�
* 8. ������ �ε������� ����� ���� ���������� (0.3�� ����) �ٲ�
* 9. ���������� ����� ������ ����� ��
* 10. *���������� �̿� -> ����ǥ(���� ���� �� ����/���), �г��� �Է�/10�������, score.txt (�г��� ����\n)
* 11. ��� �ϰڴ��� Ȯ��
*/

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

/* ���� �Լ� */
void delay(int n) {
	Sleep(n);
}

void gotoxy(int x, int y) { //gotoxy�Լ� 
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


/* cmdâ ��ǥ �̵� �Լ� */
//void gotoxy(int x, int y) {
// 	COORD Cur = { x, y }; //��ǥ�� �����ϴ� ����ü�� x, y�� �ʱ�ȭ
//	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
//}

/* cmdâ Ŀ�� ǥ�� �����Լ� */
void showcursor(int bShow) {
	CONSOLE_CURSOR_INFO CurInfo = { 20, bShow }; //Ŀ�� ����ü ������ �����ϰ�, Ŀ���� ũ��� ���迩�η� �ʱ�ȭ 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

#define BX 20 
#define BY 1  
#define BW 80 // �������� ����
#define BH 26 // �������� ����
#define UX 50 // �÷��̾� �ʱ� ��ġ
#define UY 12 // �÷��̾� �ʱ� ��ġ
#define MAXBULLET 8
#define MAXENEMY 10

BOOL IsKeyDown(int Key)
{
	return ((GetAsyncKeyState(Key) & 0x8000) != 0);
	// GetAsyncKeyState : ���� ���� Ű�� �ԷµǴ��� Ȯ��
	// 0x8000 : ������ ���� ���� ���� ȣ�� �������� �����ִ� ����
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
	player.x = UX;
	player.y = UY;
	player.hp = 3;

	for (int i = 0; i < MAXENEMY; i++)
		enemy[i].count = 0;

	srand((unsigned)time(NULL));
	system("cls");

	ScoreBoard(1, 1, 2); //test �� ��������

	char nick[10];

	printf("�г����� �Է����ּ���!: ");
	scanf("%s", nick);
	system("cls");

	showcursor(0); //Ŀ�� �����
	gotoxy(player.x, player.y);
	printf("U��U");
	
	PrintWall();
	
	int count = 0;
	while(1) {
		PrintFloor();
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
		if (count % 10 == 0)
		{
			if (IsKeyDown(0x58)) // XŰ
			{
				enemyframe -= 1;
				if (enemyframe <= 1)
					enemyframe = 1;
			}
			if (IsKeyDown(0x5A)) // ZŰ
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

	ScoreBoard(nick, score, 1);
	system("cls");
	gotoxy(UX, UY);
	printf(" Game Over");
	gotoxy(UX, UY + 2);
	printf("�������� : %d", score);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
	Sleep(1000);
	system("cls");
	ScoreBoard(nick, score, 2);
	Sleep(1000);
	getch();

	return 0;
}

void PlayerHit()
{
	for (int i = 0; i < MAXENEMY; i++)
	{
		if (enemy[i].exist == FALSE) continue;
		if (enemy[i].y == player.y && abs(enemy[i].x - player.x) <= 0) //abs : ���� �Լ�
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
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
				puts("��");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
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
	printf("�Ѿ� �ӵ� : %d", textframe);
}

void PrintWall() {
	//gotoxy(BX, 0);
	//printf("�̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢�");
	//gotoxy(BX, 27);
	//printf("�̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢̢�");

	for (int i = BX; i < BW + 1; i++) {
		gotoxy(i, 0);
		printf("��");
		gotoxy(i, 27);
		printf("��");
	}

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
		printf("�� �� ��");
		break;
	case 2:
		gotoxy(BW + 7, 10);
		printf("�� �� ��");
		break;
	case 1:
		gotoxy(BW + 7, 10);
		printf("�� �� ��");
		break;
	case 0:
		gotoxy(BW + 7, 10);
		printf("�� �� ��");
		break;
	}

	gotoxy(BW + 7, 13);
	printf("�Ͻ����� : P");

	gotoxy(BW + 7, 15);
	printf("�Ѿ� �ӵ� ���� : Z, X");

	gotoxy(BW + 7, 17);
	printf("���� : SPACE");

	gotoxy(BW + 7, 7);
	TextEnemyFrame(enemyframe);

	gotoxy(35, 28);
	printf("                                        ");
}

/*���ھ�� �Լ�, 1=���� ����, 2= ���� �б�*/
void ScoreBoard(char nick[10], int new_score, int mode) {
	FILE* fp;
	
	if (mode == 1) { //������
		fp = fopen("score.txt", "a");
		fprintf(fp, "%s %d\n", nick, new_score);
		fclose(fp);
	}

	if (mode == 2) { //�б���
		fp = fopen("score.txt", "r");

		if (fp == NULL) {
			fprintf(stderr, "�����߻�!!");
			exit(0);
		}

		printf("%-15s����\n", "�г���");
		
		/* �����͸� �ҷ��ͼ� �迭�� �����ϴ� �κ� */
		int n = 0;
		while (feof(fp) == 0) {
			fscanf(fp, "%s %d\n", list[n].nick, &list[n].score);
			n++;
		}
		fclose(fp);

		/* �� ���� �� �迭 ���� �κ� */
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

		/* ������ ��� �κ� */
		for (int i = 0; i < n; i++) {
			printf("%-14s%d\n", list[i].nick, list[i].score);
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