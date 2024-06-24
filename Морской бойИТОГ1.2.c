#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

	//���������� ����������� �������
void PrintMenu();
void MainMenu();
void NewGame(int ships_count_main);
void PLAYER_VS_COMPUTER(int ships_count_main);
void PLAYER_VS_PLAYER(int ships_count_main);
void SetShips(char *field_owner, int ships_count);
void SetOneShip(char *field_owner, int ship_len);
void InputErrorMessage();
void InputErrorMessageText();
void PRINT_INSTRUCTION(int ships_count_main);
void draw_fields(char *first, char *second, int owner_c, int enemy_c);
void draw_row(char *field, int y, int owner);
void draw_top_letters();
void PrintWordNTimes(char *word, int n);
int ChangeShipsCount(int ships_count_main);
void PrintShipsCount(int ships_count_main);
int ResetShipsCount(int ships_count_main);
void PrintColored(char *string, int ColorID);
void PrintAvailableColors();
int StrikeTurn(char *owner, char *enemy, char *who, int color, int color2, int *enemy_ships);
int strike(char *field_owner, char *field_for_strike, int color1, int color2, int *ships_remain);
unsigned int GetNumberBySeed(int seed, int limit);
int SetShotAroundShips(char *field, int side, int len, int x, int y, int *array);
void SetShipsRandom(char *players_field, int ships_count);
void SetInSide(char *field, int set_what, int side, int ship_len, int x, int y);
int CalculateWhatInSide(char *field, int x, int y, int side, int what);
void ChangeMaxLen();

	//���������� ��������
//������ �������� ����
#define FIELD_SIZE 10	//���������� = 10
		//������������� ������� �������� �� 10 �� 26, ����� ���������� ��������:
		//��� ��������� ������ 10 ���� ���������� ����������� �������� �� ����� ����������� �������� ��-�� �������� �����;
		//������������ �����������: 27-�� ���������� ����� '[' � ��������� ��������� �������� ���� �� ���������;
		//������� � 40, ����������� �������� ����� ��� �������� ��������� ����� ����������� �������.
//������������ ����� ������� + 1
int ship_len_limit = 5; 	//���������� = 5


typedef enum eFIELD_INFO{ //����������� ������ ����� �������� ����
	EMPTY = 0,
	SHOT = 1,
	STRIKE = 2,
	KILL = 3,
	SHIP = 4,
	eFIELD_INFO_LEN
}eFIELD_INFO;
char draw_symbol[eFIELD_INFO_LEN] = { //������� ����������� �� ����� ����
	'-',
	'*',
	'X',
	'#',
	'S'
};

	//������� ����
typedef enum MENU{ 	//��������
	NEW_GAME = 0,
	CHANGE_SHIPS,
	RESET_SHIPS_COUNT,
	CHANGE_MAX_SHIP_LEN,
	INSTRUCTION,
	EXIT,
	MENU_LEN
}MENU;
void PrintMenu(){ 	//���������
	printf("\t%d ������ ����� ����\n", NEW_GAME);
	printf("\t%d �������� ���������� ��������\n", CHANGE_SHIPS);
	printf("\t%d �������� ���������� ��������\n", RESET_SHIPS_COUNT);
	printf("\t%d �������� ������������ ����� ������� � ����\n", CHANGE_MAX_SHIP_LEN);
	printf("\t%d ���������� �� ����\n", INSTRUCTION);
	printf("\t%d �����\n", EXIT);
}
void MainMenu(){ 	//��������������; ���������� �������� � ����
	//������� �������� ������ ����� ����� � ���� � ������� 
	//���������: (10^(����� - 1)) * ���������� ��������.
	//�����: ���������� ������ �������� � ����� �� ������ 20.
	int ships_count_main = (	1	*	4 + 10	 *	3
							+	100	*	2 + 1000 *	1); //���������� 1234
	int lim = ship_len_limit;	//���������� = 5
	int main_ships_value = ships_count_main; //�������� ��� ������ ���������� �������� ��� �������������
	int MENU_STATUS = 1;
	CountShips(ships_count_main);
	while(1)
	{
		while
		(
			printf("�� ���������� � ������� ����. ������� ����� ���� ��������, ������� ������ ���������.\n")
			  ,PrintMenu(), fflush(stdin), scanf("%d", &MENU_STATUS) != 1 || (MENU_STATUS < 0 || MENU_STATUS >= MENU_LEN)
		)
			InputErrorMessage();
		system("cls");
		switch(MENU_STATUS)
		{
			case NEW_GAME:		//����� ���� ����� ����
			{
				NewGame(ships_count_main);
				break;
			}
			case CHANGE_SHIPS:	//�������� ���������� ��������
			{
				MENU_STATUS = ChangeShipsCount(ships_count_main);
				if (MENU_STATUS != -1)
					ships_count_main = MENU_STATUS;
				break;
			}
			case RESET_SHIPS_COUNT:	//�������� �� ����������� ���������� ��������
			{
				if (ResetShipsCount(ships_count_main))
				{
					ships_count_main = main_ships_value;
					ship_len_limit = lim;
				}
				break;
			}
			case CHANGE_MAX_SHIP_LEN:
			{
				ChangeMaxLen(&ships_count_main);
				break;
			}
			case INSTRUCTION:	//������� ����������
			{
				PRINT_INSTRUCTION(main_ships_value);
				break;
			}
			case EXIT:	//����� �� �������� ����
			{
				if (Exit())
					return;
			}
		}
	}
}

	//������ �������� ����
void NewGame(int ships_count_main){ 			//��������� � ��������������
	int i=-1;
	while(printf("�������� ����� ����:\n\t%d ������� � ���������\n\t%d ����� � ����\n", 1, 2),
			fflush(stdin), scanf("%d", &i)!=1 || (i<1 || i>2))
		InputErrorMessage();
	switch(i)
	{
		case 1:
		{
			system("cls");
			PLAYER_VS_PLAYER(ships_count_main);
			break;
		}
		case 2:
		{
			system("cls");
			return;
		}
	}
}
int ChangeShipsCount(int ships_count_main){		//�������� ���������� ��������
	int i, n, y;
	printf("�������, ������� ����� � ����:\n");
	PrintShipsCount(ships_count_main);
	if (Confirm("�� ����� ������ �������� ���������� �������� � ����?", 0))
	{
		system("cls");
		return ships_count_main;
	}
	else
	{
		system("cls");
		while(1)
		{
			for(n = ship_len_limit - 1, i = 0; n > 0; n--)
				i += ((ships_count_main / IntPow(10, n-1))%10 * n);
			y = -1;
			PrintShipsCount(ships_count_main);
			printf("� ���� � ����� ����� ���� �� ����� 20 ������ ��������\n");
			printf(	"(�������� ������: %d) � �� ����� 9 ���������� ��������.\n", 20 - i);
			printf(	"������� ����� �������, ���������� �������� � ���� ������ ��������:\n");
			fflush(stdin);
			if (scanf("%d", &y)!=1 || y < 1 || y >= ship_len_limit)
				InputErrorMessage();
			else
			{
				printf("������� �������� ����� %d �� ������, ����� ���� � ����?\n", y);
				printf("(�������� ��� ���������� ������ ��������: %d)\n", 20 - i);
				if (scanf("%d", &n)!=1)
					InputErrorMessage();
				else
					if (n < 0 || (n * y > (20 - i) + y * ((ships_count_main / IntPow(10, y-1))%10)) || (n > 9))
					{
						system("cls");
						printf("������ ������� ����� ���������� ��������.\n");
						continue;
					}
					else
						ships_count_main += (IntPow(10, y-1) * (n - (ships_count_main/IntPow(10, y-1))%10));
			}
			PrintShipsCount(ships_count_main);
			if (!Confirm("��������� ��������� ���������� ��������?", 1))
			{
				system("cls");
				if (Confirm("��������� ��������� ���������� ��������?", 1))
				{
					system("cls");
					printf("���������� �������� �������� � ���������� ���������.\n");
					return -1; //�������� ���������� ������ �������� ��������
				}
				system("cls");
				if (!CountShips(ships_count_main))
				{
					printf("����������, ����� � ���� ��� ���� �� 1 �������!\n");
					printf("���������� �������� �������� � ���������� ���������.\n");
					return -1;
				}
				printf("���������� �������� ������� ��������.\n");
				return ships_count_main;
			}
			else
				system("cls");
		}
	}
}
int ResetShipsCount(int ships_count_main){		//������ �� ����� ���������� �������� � ����
	printf("������� ������� ��� ����:\n");
	PrintShipsCount(ships_count_main);
	printf("������� ������������ ����� ������� � ����: %d\n", ship_len_limit - 1);
	if (Confirm("�������� ���������� �������� � ������������ ����� �������\n� ���� � ����������� ���������?\n", 0))
	{
		system("cls");
		printf("�������� �������� � ����������� ���������.\n");
		return 0;
	}
	system("cls");
	printf("�������� �� ���� ��������.\n");
	return 1;
}
void ChangeMaxLen(int *ships_count){				//��������� ������������ ����� ������� � ����
	int i;
	while(1)
	{
		printf("������� ������������ ����� ������� � ����: %d\n", ship_len_limit - 1);
		printf("������������ ����� ����� ��������� �� 1 �� 9.\n");
		fflush(stdin);
		if (scanf("%d", &i)!=1 || i < 1 || i > 9)
		{
			InputErrorMessage();
			continue;
		}
		if (Confirm("��������� ��������� ������������ ����� �������?", 1))
		{
			system("cls");
			printf("������������ ����� �������� � ���������� ���������.\n");
		}
		else
		{
			if (ship_len_limit > i + 1)
				*ships_count -= (*ships_count / IntPow(10, i)) * IntPow(10, i);
			ship_len_limit = i + 1;
			system("cls");
			printf("������������ ����� ������� ��������.\n");
		}
		break;
	}
}
void PRINT_INSTRUCTION(int ships_count_main){ 	//������� �� ����� ���������� �� ����
	int x, y;
	char field[4][4]={0};
	system("cls");
	printf("///////////////////////////////////����������///////////////////////////////////\n\n");
	printf( "(��� ���������� ��� ���� ���������� ����. �������� ������� ����, ���� �\n"
			"���������� �������� ������� ��� ����������� ��������).\n\n"
			"'������� ���' - ��� ���� ��� ���� ����������, � ������� ������ ����� �� �������,\n"
			"��� ������� ����� �������� ����������� ���� �������� ���������.\n"
			"������� ����� �������� ������� %d �� %d ������.\n"
			"�������, �������� ������������ ������ �������� ����:\n", FIELD_SIZE, FIELD_SIZE);
	printf("'%c' - ������ ������;\n", draw_symbol[EMPTY]);
	printf("'%c' - ������, ���� ��� ���������� �����;\n", draw_symbol[SHOT]);
	printf("'%c' - ������ � ������� �������;\n", draw_symbol[STRIKE]);
	printf("'%c' - ������ � ������������ �������;\n", draw_symbol[KILL]);
	printf("'%c' - ������ � �������.\n", draw_symbol[SHIP]);
	printf("'|' - �������������� ������� ����.\n����� �� A �� %c - �������������� ����������, "
		   "����� �� 0 �� %d - ������������ ����������.\n����� ��������� ���, ���� �������\n"
		   "������� ��������������, ����� ������������ ����������.\n"
		   "������ ���������� ����: 'G3' ���� 'g3', �� �� '3g' (� �������� �������)!\n"
		   "�� ���� ��������� %d �������� %d ����� ������ �����: \n", FIELD_SIZE - 1 + 'A', FIELD_SIZE - 1, CountShips(ships_count_main), ship_len_limit - 1);
	for(x = ship_len_limit - 1; x > 0; x--)
	{
		y = (ships_count_main / IntPow(10, x - 1))%10;
		if (y > 1)
			printf("\t%d �������� %d ������ � �����.\n", y, x);
		else
			printf("\t%d �������  %d ������ � �����.\n", y, x);
	}
	printf("��������� ������� ����� ����������� � �������������, �� �� �����������.\n"
		   "�� ������� ������������� ������� �� ���� 4 �� 4:\n");
	field[1][2] = SHIP;
	field[1][3] = SHIP;
	for(printf("��� ����� ��������� ������� (������� ���������� �������������):\n    A B C D\n"), y=0; y<4; y++, printf("|\n"))
		for(printf("%2d|", y), x=0; x<4 ;printf("%2c", draw_symbol[field[y][x]]), x++);	
	field[1][3] = SHIP;
	field[2][2] = SHIP;
	field[1][2] = EMPTY;
	for(printf("��� ������ ��������� ������� (������� ���������� �� ���������):\n    A B C D\n"), y=0; y<4; y++, printf("|\n"))
		for(printf("%2d|", y), x=0; x<4 ;printf("%2c", draw_symbol[field[y][x]]), x++);
	printf("������� ������ ����������� �����, ��� �� 1 ������ ���� �� ����� � ����� �����������.\n");
	printf("�� ������� ������������ �������� �� ���� 4 �� 4:\n");
	field[1][2] = SHIP;
	field[2][2] = EMPTY;
	field[2][3] = EMPTY;
	field[3][0] = SHIP;
	field[3][1] = SHIP;
	for(printf("��� ����� ��������� �������:\n    A B C D\n"), y=0; y<4; y++, printf("|\n"))
		for(printf("%2d|", y), x=0; x<4 ;printf("%2c", draw_symbol[field[y][x]]), x++);
	field[2][1] = SHIP;
	field[2][0] = SHIP;
	field[3][0] = EMPTY;
	field[3][1] = EMPTY;
	for(printf("��� ������ ��������� ������� (���������� �� ������� ������� ������ 1 ������):\n    A B C D\n"), y=0; y<4; y++, printf("|\n"))
		for(printf("%2d|", y), x=0; x<4 ;printf("%2c", draw_symbol[field[y][x]]), x++);
	printf("��� ����������� ������� ���������� ������� �� ������ ��� ����� 1 ���.\n"
			"����� ����, ��� ����� ������ �� ��������� ������� ��� ������ ���,\n"
			"�� ������� 1 �������������� ���. ������ ��������� ��������,\n"
			"���� ��������� �� ���� ������ �� ���������� �������.\n"
			"����� ����������� �������, ��-�� ������, ���� � ������� 1 ������ ������ �����\n"
			"������� ����� �������� ���, ����� ���� ��� ����������.\n");
	printf("\n////////////////////////////////����� ����������////////////////////////////////\n\n");
}
int Exit(){ 									//������ ������������� �� �����
	if (Confirm("�� ����� ������ �����? ", 1))
	{
		system("cls");
		return 0;	//��������� ��������� ������
	}
	system("pause");
	return 1; 		//��������� ����������
}

	//������ ���� ����� ����. ������������� ����
void PLAYER_VS_PLAYER(int ships_count_main){ 	//�������� � ���������
	char i = 0, flag, win = 0;
	int ships_count_player_1 = ships_count_main;
	int ships_count_player_2 = ships_count_main;
	char field_owner_player_1[FIELD_SIZE][FIELD_SIZE]={0};
	char field_owner_player_2[FIELD_SIZE][FIELD_SIZE]={0};
	int player_1_colorid = InputColorID("������ �����");
	int player_2_colorid = InputColorID("������ �����");
	printf("������ ������� "); 
	PrintColored("������� ������ ", player_1_colorid);
	printf("����������� �������.\n");
	if (Confirm("���������� ������� �������?", 0))
		SetShipsRandom(field_owner_player_1[0], ships_count_player_1);
	else
		SetShips(field_owner_player_1[0], ships_count_player_1);
	system("cls");
	printf("���� ������� ������� �����������.\n������ ������� ");
	PrintColored("������� ������ ", player_2_colorid);
	printf("����������� �������.\n");
	if (Confirm("���������� ������� �������?", 0))
		SetShipsRandom(field_owner_player_2[0], ships_count_player_2);
	else
		SetShips(field_owner_player_2[0], ships_count_player_2);
	system("cls");
	printf("��� ������� ������� �����������.\n"
			"����� ������� ���� ������� ������ ������ ��� ����� ��������� ����� � ���������������� �������� ������.\n"
			"����� ����������� ������ ����� ");
	PrintColored("������ �����", player_1_colorid);
	printf(", ������� ����� ����������� ����");
	PrintColored(" ������� ������", player_1_colorid);
	printf(",\n� ������������ �� �� ���������, ������ �����������!\n\n���������� ������� ���. �������� ����!\n");
	while(1)
	for(flag = 1, i = 0; 1; flag = 0, i = (!i))
	{
		if (!flag) //����� �������� ������� ������ ��� ����� ������ ����
		{
			printf("\a����� ������� ����!\n");
			if (!i)
			{
				printf("������ ����� ������ ");
				PrintColored("������ �����", player_1_colorid);
				printf(".\n");
			}
			else
			{
				printf("������ ����� ");
				PrintColored("������ �����", player_2_colorid);
				printf(".\n");
			}
		}
		system("pause");
		system("cls");
		if (!i) 	//������������� ����
			win = StrikeTurn(field_owner_player_1[0], field_owner_player_2[0], "������� ������", player_1_colorid, player_2_colorid, &ships_count_player_2);
		else
			win = StrikeTurn(field_owner_player_2[0], field_owner_player_1[0], "������� ������", player_2_colorid, player_1_colorid, &ships_count_player_1);
		if (win)
		{
			system("cls");
			printf("\a��� ������� ");
			if (i)
				PrintColored("������� ������ ", player_1_colorid);
			else
				PrintColored("������� ������ ", player_2_colorid);
			printf("���������!\n������ ������� ");
			if (!i)
				PrintColored("������ �����", player_1_colorid);
			else
				PrintColored("������ �����", player_2_colorid);		
			printf(".\n������� �� ����!\n");
			system("pause");
			system("cls");
			return;
		}
	}
}

	//��������� ������� �����
int PrintField(char *field){ 											//������, � ���� �������� ������
	if (!field)
		return 1;
	int x, y;
	for(draw_top_letters(), printf("\n"), y=0; y<FIELD_SIZE; y++, printf("|\n"))
	{
		printf("%2d|", y);
		for(x=0; x<FIELD_SIZE; x++)
			printf("%2d", field[FIELD_SIZE*y+x]);		
	}
	return 0;	
}
int PrintSymField(char *field){ 										//������, � ���� ������� ��������
	if (!field)
		return 1; //��������� �������� ���������
	int x, y;
	for(draw_top_letters(), printf("\n"), y=0; y<FIELD_SIZE; y++, printf("|\n"))
		for(printf("%2d|", y), x=0; x<FIELD_SIZE ;printf("%2c", draw_symbol[field[FIELD_SIZE*y+x]]), x++);
	return 0;	// ����� ������ �������
}
void draw_fields(char *first, char *second, int owner_c, int enemy_c){ 	//���� �����, � ������ �������� �����
	int y;
	PrintWordNTimes(" ", FIELD_SIZE - 1);
	PrintColored("���� ���� ", owner_c);
	PrintWordNTimes(" ", 2 * FIELD_SIZE + 3);
	PrintColored("���� �����", enemy_c);
	printf("\n");
	draw_top_letters();
	printf("          ");
	draw_top_letters();
	for(printf("\n"), y=0; y < FIELD_SIZE; y++, printf(" |\n"))
	{
		draw_row(first, y, 1);
		printf(" |        ");
		draw_row(second, y, 0);
	}
	printf("\n");
}
void draw_row(char *field, int y, int owner){ 	//������ � ������ �������� �����
	int x;
	for(printf("%2d|", y), x=0; x<FIELD_SIZE; x++)
		switch (field[FIELD_SIZE*y+x])
		{
			default:
				{
					printf("%2c", draw_symbol[field[FIELD_SIZE*y+x]]);
					break;
				}
			case EMPTY:
				{
					printf("%2c", draw_symbol[EMPTY]);
					break;
				}
			case SHIP:
				{
					if (owner)
						printf("%2c", draw_symbol[SHIP]);
					else
						printf("%2c", draw_symbol[EMPTY]);
					break;
				}
		}
}
void draw_top_letters(){						//��������� ������ �������������� ���������
	short i;
	for(printf("   "), i = 0; i < FIELD_SIZE; i++)
		printf(" %c", i + 'A');
}

	//��������� �� ������ � ������������ �����:
void InputErrorMessage(){ 		//� ��������������� ������� ������
	system("cls");
	InputErrorMessageText();
}
void InputErrorMessageText(){	//��� ������ ������
	printf("������������ ����! ���������� ��� ���.\n\n");
}

	//����, �����, ����������
int InputCoordinates(int *x, int *y){ 					//���� ��������� �������� � x � y
	char input_x = -1;
	if(fflush(stdin), scanf("%c%d", &input_x, y)!=2 || ((input_x < 'A' || input_x > 'J') &&
		(input_x < 'a' || input_x > 'j')) || (*y < 0 || *y >= FIELD_SIZE) //�������� ����� ���������
	  )
		return 1; //���� �����������
	if (input_x <= 'J') //������� 'x'-� �� ������� � �����
		*x = input_x - 'A';
	else 
		*x = input_x - 'a';
	printf("InputCoordinates: x = %d, y = %d\n", *x, *y);
	return 0; //���� ���������
}
int IntPow(int x, int y){ 								//int x � ������� int y
	int a = 1;
	for(y; y > 0; y--)
			a *= x;
	return a;
}
void PrintWordNTimes(char *word, int n){ 				//������� ������ n ���
	for(n; n > 0; n--)
		printf("%s", word);
}
void PrintShipsCount(int ships_count_main){				//������� ���������� �������� � ����
	char n;
	for(printf(	"����� ������� � �������:   |"), n = ship_len_limit - 1; n > 0; n--)
		printf(" %d |", n);
	printf( 	"\n                           |");
	PrintWordNTimes("---|", ship_len_limit - 1);
	for(printf( "\n���������� ��������:       |"), n = ship_len_limit - 1; n > 0; n--)
		printf(" %d |", (ships_count_main/IntPow(10, n - 1))%10);
	printf("\n");
}
int Confirm(char *string_before_confirm, int clear){ 	//������: 0 - ��, 1 - ���. clear 0 �� ������ �����
	int y;
	while(printf("%s\n'0' - ���� ��.\n'1' - ���� ���.\n", string_before_confirm)
			  ,fflush(stdin), scanf("%d", &y)!=1 || (y < 0 || y > 1))
			if (clear)
				InputErrorMessage(); 		//� �������� ������
			else
				InputErrorMessageText(); 	//��� �������
	return y;
}
int CountShips(int ships_count_main){					//��������� ���������� �������� � �����
	int i, s = 0;
	for(i = ship_len_limit - 1; i > 0; i--)
		s += (ships_count_main / IntPow(10, i - 1))%10;
	return s;
}
void PrintColored(char *string, int ColorID){			//������� ����� ������ ColorID
	printf("\033[1;3%dm", ColorID);
	printf("%s\033[0m", string);
}
void PrintAvailableColors(){							//������� ��������� ����� (ColorID)
								//ColorID:
	printf("\033[1;30m0 �����\n");		//0
	printf("\033[1;31m1 �������\n");	//1
	printf("\033[1;32m2 ������\n");	//2
	printf("\033[1;33m3 Ƹ����\n");		//3
	printf("\033[1;34m4 �����\n");		//4
	printf("\033[1;35m5 ����������\n");	//5
	printf("\033[1;36m6 �������\n");	//6
	printf("\033[1;37m7 �����\n");		//7
	
	printf("\033[0m"); //����������� � ������������ ����� ������
}
int InputColorID(char *word){							//������ �� ���� ColorID
	int i;
	while(1)
	{
		printf("��������� �����:\n");
		PrintAvailableColors();
		printf("������� ��� �����, ������� ����� ������������ ������� %s � �������� ����:\n", word);
		fflush(stdin);
		if (scanf("%d", &i)!=1 || i < 0 || i > 7)
		{
			InputErrorMessage();
			continue;
		}
		printf("������ �����������:\n");
		PrintColored(word, i);
		printf("\n");
		PrintColored("���� ����", i);
		printf("\n��� ��������� �������� ���� ����� ��������� ���: ");
		PrintColored("���� �����", i);
		if (!Confirm("\n��������� ���� �����������?", 0))
		{
			system("cls");
			return i;
		}
		system("cls");
	}
}
int XYChangesBySide(int x_is_0_y_is_1, int side){		//�������� ��������� x, y � ����������� �� �������
    if (x_is_0_y_is_1)	//� ����������� �� ����������� x = 0, 1, 0, -1;    y = 1, 0, -1, 0
    	return (side < 3) ? 0 - side%2 : (side == 3) ? 1 : 0;
	return (side%2) ? 0 : (side == 2) ? 1 : -1;
}
void DeleteObjectsFromField(char *field, int object){	//�������� �� ������� ��� �������� object �� EMPTY
	int i;
	if (object == -1)
		for(i = FIELD_SIZE * FIELD_SIZE - 1; i >= 0; i--)
			field[i] = EMPTY;
	else
		for(i = FIELD_SIZE * FIELD_SIZE - 1; i >= 0; i--)
			if (field[i] == object)
				field[i] = EMPTY;
}
void EquateArrays(char *from_this_array, char *to_this_array){	//���������� ������ ������ � ������
	int i = FIELD_SIZE * FIELD_SIZE - 1;
	for(i; i >= 0; i--)
		to_this_array[i] = from_this_array[i];
}
unsigned int GetNumberBySeed(int seed, int limit){		//������������� ����� �� ������ ������
	if (limit)
		return (unsigned int)(19257 * seed + 1234567) % limit;
	return (unsigned int)(19257 * seed + 1234567) % 1000000;
}


	//������ � ���������
void SetShips(char *field_owner, int ships_count){ 	//�������� ����������� �� �������� ships_count
	int i, n;
	printf("���������� �������.\n");
	for(n = ship_len_limit - 1; n > 0; n--)		//�� n = ������������ ����� �������
		for(i = (ships_count / IntPow(10, n-1))%10; i > 0; i--) // i = �������������� �� ships_count
			SetOneShip(field_owner, n); 					//�� ����� ���-�� �������� ��� ��������� 
}
void SetOneShip(char *field_owner, int ship_len){ 	//��������� ������� � ������ = ship_len 
	int x = -1, y = -1, int_s, av_s, x_2;
	char side;
	if (ship_len == 1)
		{
			while(printf("������� ���������� %d ��������� �������:\n", ship_len),
					PrintSymField(field_owner), InputCoordinates(&x, &y), CheckOneBlock(field_owner, x, y))
				InputErrorMessage();
			field_owner[FIELD_SIZE*y+x] = SHIP;
			system("cls");
			printf("������� ������� ����������.\n");
			return; //������� � 1 ������ ����� ���������� ����� �������� ��������, ������� ��������� �������
		}
	else
		while(1)
		{
			while(printf("������� ���������� ��������� ����� %d ��������� �������:\n", ship_len),
							PrintSymField(field_owner), InputCoordinates(&x, &y), CheckOneBlock(field_owner, x, y))
				InputErrorMessage();
			system("cls");
			side = field_owner[FIELD_SIZE*y+x]; //�� ������, ���� ���-�� �� ���, � �� (x, y) �� EMPTY
			field_owner[FIELD_SIZE*y+x] = SHIP;
			PrintSymField(field_owner);
			field_owner[FIELD_SIZE*y+x] = side; //�.�. ��� �����������, ��� ������� ��������� �����
			side = 5;
			av_s = AvailableSides(field_owner, x, y, ship_len, 0); //� av_s ���������� ��������� �������
			if (!av_s)	//���� �� ���� ������������ ������ ��� ������� ��� ������� ����
			{
				system("cls");
				printf("�� ������ ����� ������ ���������� %d �������� �������.\n", ship_len);
				continue;
			}
			while(AvailableSides(field_owner, x, y, ship_len, 1), printf("(��� ������ ��������� ������� ����� ������ 0)\n"
					"������� ����� �����������, � ������� ����� ���������� �������: "), fflush(stdin),
					scanf("%c", &side)!=1 || side < '0'|| side > '4')
				InputErrorMessageText();
			if (side == '0')
			{
				system("cls");
				continue;
			}
			int_s = side - '0';
			if(!((av_s/IntPow(10, 4 - int_s))%10)) 	//����������� ����������� ������� int_s � av_s
				printf("������ ��������� ������� � ���� �����������.\n");
			else
				break;
		}
	SetInSide(field_owner, SHIP, int_s, ship_len, x, y);
	printf("������� ������� ����������.\n");
	return;
}
void SetShipsRandom(char *players_field, int ships_count){	//����������� ������� � ��������� �������
	int i, array_len, s, seed = 1111, side, coordinate, x, y, available_sides, x2 = 1, y2 = 2;
	char random_field[FIELD_SIZE * FIELD_SIZE];
	int free_squares[FIELD_SIZE * FIELD_SIZE];
	while(1)
	{
		s = seed;
		DeleteObjectsFromField(random_field, -1);
		array_len = FIELD_SIZE * FIELD_SIZE - 1;
		for(i = array_len; i >= 0; i--)
			free_squares[i] = i;
		if (Confirm("������������� ���� �� \"�������\" ������ ��������� ���������?", 1))
		{
			srand(time(NULL));
			seed = rand();	//���������� ��������� �������� ����������, ����� �������
		}
		else
			while(printf("������� ����, �� �������� ����� ������������� ����: "), fflush(stdin),
					scanf("%d", &seed)!=1)
				InputErrorMessage();
		for(i = ship_len_limit - 1; i > 0; i--)
			for(s = (ships_count / IntPow(10, i - 1))%10; s > 0; seed += 17135467)
			{
				coordinate = free_squares[GetNumberBySeed(seed, array_len + 1)];
				x = coordinate % FIELD_SIZE;
				y = coordinate / FIELD_SIZE;
				if (GetNumberBySeed(seed, 2))
					side = 1;
				else
					side = -1;
				if (random_field[coordinate] == SHOT || random_field[coordinate] == SHIP)
					while((free_squares[coordinate] == 0) && !AvailableSides(random_field, x, y, i, 0))
					{
						coordinate += side;
						if (coordinate > array_len)
							coordinate = 0;
						else
							if (coordinate < 0)
								coordinate = array_len;
						x = coordinate % FIELD_SIZE;
						y = coordinate / FIELD_SIZE;
					}
				available_sides = AvailableSides(random_field, x, y, i, 0);
				if (!available_sides)
					continue;
				side = GetNumberBySeed(seed, 4) + 1;
				while(!((available_sides / IntPow(10, 4 - side))%10))
					if (side > 3)
						side = 1;
					else
						side++;
				x2 = XYChangesBySide(0, side);
				y2 = XYChangesBySide(1, side);
				SetInSide(random_field, SHIP, side, i, x, y);
				SetShotAroundShips(random_field, side, i, x, y, free_squares);
				for(available_sides = i, y -= y2, x -= x2; available_sides > 0; available_sides--, y += y2, x += x2)
					free_squares[FIELD_SIZE * (y + y2) + (x + x2)] = 0;
				s--;
			}
		DeleteObjectsFromField(random_field, SHOT);
		PrintSymField(random_field);
		if (!Confirm("����� ���� �������?", 0))
		{
			system("cls");
			EquateArrays(random_field, players_field);
			printf("���� ������� ���������.\n");
			break;
		}
		else
			system("cls");
	}
}
void SetInSide(char *field, int set_what, int side, int ship_len, int x, int y){	//������ set_what, len ������ � ����������� side
	int x_2 = XYChangesBySide(0, side), y_2 = XYChangesBySide(1, side);
	for(side = 0, x = x - x_2, y = y - y_2; side < ship_len; x = x + x_2, y = y + y_2, side++)
		field[FIELD_SIZE * (y + y_2) + (x + x_2)] = set_what;
}
int SetShotAroundShips(char *field, int side, int len, int x, int y, int *array){	//���������� �������� ���-�� ���. SHOT
	int i = 0;
	i += ShotToWallOfSide(field, side, x, y, 1, array);
	i += ShotToMiddleOfSide(field, side, x, y, len, array);
	x += (len - 1) * XYChangesBySide(0, side);
	y += (len - 1) * XYChangesBySide(1, side);
	i += ShotToWallOfSide(field, side, x, y, 0, array);
}
int ShotToWallOfSide(char *field, int side, int x, int y, int back, int *array){	//���������� ���������� ������������� SHOT
	if (back)	//����� ����������� �� side ������� ��� ��������� SHOT
		if (side > 2)
			side -= 2;
		else
			side += 2;
	int count = 0, i, n, x2 = XYChangesBySide(0, side), y2 = XYChangesBySide(1, side);
	if (x2)
	{
		if (x + x2 >= 0 && x + x2 < FIELD_SIZE)
			for(i = -1; i < 2; i++)
				if (y + i >= 0 && y + i < FIELD_SIZE)
					if (field[FIELD_SIZE * (y + i) + (x + x2)] == EMPTY)
					{
						field[FIELD_SIZE * (y + i) + (x + x2)] = SHOT;
						count++;
						if (array)
							array[FIELD_SIZE * (y + i) + (x + x2)] = 0;
					}
	}
	else
		if (y + y2 >= 0 && y + y2 < FIELD_SIZE)
			for(i = -1; i < 2; i++)
				if (x + i >= 0 && x + i < FIELD_SIZE)
					if (field[FIELD_SIZE * (y + y2) + (x + i)] == EMPTY)
					{
						field[FIELD_SIZE * (y + y2) + (x + i)] = SHOT;
						count++;
						if (array)
							array[FIELD_SIZE * (y + y2) + (x + i)] = 0;
					}
	return count;		
}
int ShotToMiddleOfSide(char *field, int side, int x, int y, int len, int *array){	//���������� ���������� ������������� SHOT
	int i, n = len, x2 = XYChangesBySide(0, side), y2 = XYChangesBySide(1, side);
	for(len = 0, x -= x2, y -= y2; n > 0; n--, x += x2, y += y2)
	{
		if (x2)
		{
			for(i = -1; i < 2; i += 2)
				if (y + i >= 0 && y + i < FIELD_SIZE)
					if (field[FIELD_SIZE * (y + i) + (x + x2)] == EMPTY)
					{
						field[FIELD_SIZE * (y + i) + (x + x2)] = SHOT;
						len++;
						if (array)
							array[FIELD_SIZE * (y + i) + (x + x2)] = 0;
					}
		}
		else
			for(i = -1; i < 2; i += 2)
				if (x + i >= 0 && x + i < FIELD_SIZE)
					if (field[FIELD_SIZE * (y + y2) + (x + i)] == EMPTY)
					{
						field[FIELD_SIZE * (y + y2) + (x + i)] = SHOT;
						len++;
						if (array)
							array[FIELD_SIZE * (y + y2) + (x + i)] = 0;
					}
	}
	return len;
}
int CheckOneBlock(char *field, int x, int y){	//0 - �� � ������� / ���������� �� ��������� ����, >0 - ���-�� ��������
	int x_2, y_2, ships = 0;
	for(x_2 = -1; x_2 < 2; x_2++)
		if(x_2 + x >= 0 && x_2 + x < FIELD_SIZE)
			for(y_2 = -1; y_2 < 2; y_2++)
				if(y_2 + y >= 0 && y_2 + y < FIELD_SIZE)
					if(field[FIELD_SIZE * (y_2 + y) + (x_2 + x)] == SHIP || field[FIELD_SIZE * (y_2 + y) + (x_2 + x)] == KILL
							|| field[FIELD_SIZE * (y_2 + y) + (x_2 + x)] == STRIKE)
						ships++;
	return ships;
}
int CheckSide(char *field, int in_s, int len, int x, int y){	//0 - �� � �������, 1 - �� ��������� ���� / �������
    int x_2 = XYChangesBySide(0, in_s), y_2 = XYChangesBySide(1, in_s);
	for(in_s = 0, len--, x += x_2, y += y_2; in_s < len; x += x_2, y += y_2, in_s++)
		if ((x < 0 || x >= FIELD_SIZE) || (y < 0 || y >= FIELD_SIZE) || CheckOneBlock(field, x, y))
			return 1;
	return 0;
}
int AvailableSides(char *field, int x, int y, int ship_len, int draw){ //���������� ��������� ������� ������
    int i, sum = 0;
    if (CheckOneBlock(field, x, y))
    	return 0;
    if (!draw)
    {	
    	for(i = 1; i < 5; i++)
			if (!CheckSide(field, i, ship_len, x, y))
				sum = sum + IntPow(10, 4 - i);
		return sum;
	}
	//�� �� �����, ��� � ����, �� � ���������� �����������
    printf(	"� ��������� ��� ��������� ����������� ������� ������:\n"
			"�����������: | ����� | ������|  ���� | ����� |\n             |");
	PrintWordNTimes("-------|", 4);
	printf( "\n�����:       |   ");
	for(i = 1; i < 5; i++, printf("   |   ")) //������� i; �� ����� �� ����� (�� 1 �� 4)
	{
		if (CheckSide(field, i, ship_len, x, y)) 	//���� ����������� i ����������
			printf(" ");
		else 										//���� ����������� i ��������
		{
			printf("%d", i);
			sum = sum + IntPow(10, 4 - i); //��������� ������� i ��� ���������
		}								//(����(1000) +...+ �����(1))
	}
	printf("\n");
	return sum;
}
int FindShipEdge(char *field, int *x, int *y){	//���������� ����������� ������� � � *x � *y ������� ���������� ���� ������ �������
	if ((field[FIELD_SIZE * *y + *x + 1] == SHIP || field[FIELD_SIZE * *y + *x + 1] == STRIKE) ||
		(field[FIELD_SIZE * *y + *x - 1] == SHIP || field[FIELD_SIZE * *y + *x - 1] == STRIKE))
	{
		*x += (CalculateWhatInSide(field, *x, *y, 2, -1) - 1) * XYChangesBySide(0, 2);	//�������������� ������������ �������
		return 4;
	}
	else
	{
		*y += (CalculateWhatInSide(field, *x, *y, 1, -1) - 1) * XYChangesBySide(1, 1);
		return 3;
	}
}
int CalculateWhatInSide(char *field, int x, int y, int side, int what){	//������� ���������� what � ����������� side �� EMPTY ��� SHOT ������
	int	x2 = XYChangesBySide(0, side), y2 = XYChangesBySide(1, side), pos;
	for(side = 0, pos = (FIELD_SIZE * y) + x; !(field[pos] == EMPTY || field[pos] == SHOT) && (pos >= 0 && pos < FIELD_SIZE * FIELD_SIZE); pos += y2 * FIELD_SIZE + x2)
		if (what == -1)
		{
			if (field[pos] == SHIP || field[pos] == STRIKE)
				side++;
		}
		else
			if (field[pos] == what)
				side++;
	return side;
}

	//��� ��� ��������
int StrikeTurn(char *owner, char *enemy, char *who, int color, int color2, int *enemy_ships){	//���������� 1 ����� ������� ������ �����������, ����� 0
	int hit = 1;
	while(hit)
		{
			PrintWordNTimes(" ", 2 * FIELD_SIZE + 1);
			printf("��� ");
			PrintColored(who, color);
			printf("\n");
			hit = strike(owner, enemy, color, color2, enemy_ships);
			system("cls");
			switch (hit)
				{
				case 0:
					{
						printf("������!\n");
						break;
					}
				case 1:
					{
						printf("���������! �� ��������� �������������� ���.\n");
						break;
					}
				case 2:
					{
						if (!(*enemy_ships))
							return 1;
						printf("������� �������!\n���������� ���������� ��������� ��������:\n");
						PrintShipsCount(*enemy_ships);
						break;
					}
				case 3:
					{
						system("cls");
						printf("���� ��� ������ ��������!\n");
						break;
					}
				}
		}
	return 0;
}
int strike(char *field_owner, char *field_for_strike, int color1, int color2, int *ships_remain){ 	//�������� �������� ���������� �������� �� �������!!!!
	int x, y, hit = 0, side = -1, len = -1;
	while(draw_fields(field_owner, field_for_strike, color1, color2), printf("������� ���������� ��� ��������: "), InputCoordinates(&x, &y))
		InputErrorMessage();
	switch(field_for_strike[FIELD_SIZE*y+x])
	{
		case EMPTY: //���� � ���� ������ �����
			{
			field_for_strike[FIELD_SIZE*y+x] = SHOT;
			break;
			}
		case SHIP:	//���� � ���� ������ �������
			{
			field_for_strike[FIELD_SIZE*y+x] = STRIKE;
			side = FindShipEdge(field_for_strike, &x, &y);
			len = CalculateWhatInSide(field_for_strike, x, y, side, -1);
			if (CalculateWhatInSide(field_for_strike, x, y, side, STRIKE) == len)
			{
				hit = 2;	//������
				SetInSide(field_for_strike, KILL, side, len, x, y);
				SetShotAroundShips(field_for_strike, side, len, x, y, 0);
				*ships_remain -= IntPow(10, len - 1);
			}
			else
				hit = 1;	//���������
			break;
			}
		default:
			{
				hit = 3;
				break;
			}
	}
	return hit;
}


int main(){ //������ ����������� � ����� �������� ����
	setlocale(LC_ALL, "Rus");
	printf("����� ���������� � ���� '������� ���'.\n");
	MainMenu();
	return 0;
}
