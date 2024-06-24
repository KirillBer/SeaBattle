#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

	//Объявление большинства функций
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

	//Глобальные значения
//размер игрового поля
#define FIELD_SIZE 10	//стандартно = 10
		//рекомендуется ставить значение от 10 до 26, иначе появляются проблемы:
		//при значениях меньше 10 есть повышенная вероятность застрять на этапе расстановки кораблей из-за нехватки места;
		//некорректное отображение: 27-ая координата будет '[' и несколько следующих символов тоже не буквенные;
		//начиная с 40, отображение названий полей над игровыми областями будет расположено неверно.
//максимальная длина корабля + 1
int ship_len_limit = 5; 	//стандартно = 5


typedef enum eFIELD_INFO{ //обозначения данных ячеек игрового поля
	EMPTY = 0,
	SHOT = 1,
	STRIKE = 2,
	KILL = 3,
	SHIP = 4,
	eFIELD_INFO_LEN
}eFIELD_INFO;
char draw_symbol[eFIELD_INFO_LEN] = { //символы отображения во время игры
	'-',
	'*',
	'X',
	'#',
	'S'
};

	//Главное меню
typedef enum MENU{ 	//значения
	NEW_GAME = 0,
	CHANGE_SHIPS,
	RESET_SHIPS_COUNT,
	CHANGE_MAX_SHIP_LEN,
	INSTRUCTION,
	EXIT,
	MENU_LEN
}MENU;
void PrintMenu(){ 	//отрисовка
	printf("\t%d Начать новую игру\n", NEW_GAME);
	printf("\t%d Изменить количество кораблей\n", CHANGE_SHIPS);
	printf("\t%d Сбросить количество кораблей\n", RESET_SHIPS_COUNT);
	printf("\t%d Изменить максимальную длину корабля в игре\n", CHANGE_MAX_SHIP_LEN);
	printf("\t%d Инструкция по игре\n", INSTRUCTION);
	printf("\t%d Выйти\n", EXIT);
}
void MainMenu(){ 	//взаимодействие; количество кораблей в игре
	//сколько кораблей каждой длины будет в игре с помощью 
	//кодировки: (10^(длина - 1)) * количество кораблей.
	//Важно: количество клеток кораблей в сумме не больше 20.
	int ships_count_main = (	1	*	4 + 10	 *	3
							+	100	*	2 + 1000 *	1); //стандартно 1234
	int lim = ship_len_limit;	//стандартно = 5
	int main_ships_value = ships_count_main; //значение для сброса количества кораблей при необходимости
	int MENU_STATUS = 1;
	CountShips(ships_count_main);
	while(1)
	{
		while
		(
			printf("Вы находитесь в главном меню. Введите номер того действия, которое хотите выполнить.\n")
			  ,PrintMenu(), fflush(stdin), scanf("%d", &MENU_STATUS) != 1 || (MENU_STATUS < 0 || MENU_STATUS >= MENU_LEN)
		)
			InputErrorMessage();
		system("cls");
		switch(MENU_STATUS)
		{
			case NEW_GAME:		//вызов меню новой игры
			{
				NewGame(ships_count_main);
				break;
			}
			case CHANGE_SHIPS:	//изменить количество кораблей
			{
				MENU_STATUS = ChangeShipsCount(ships_count_main);
				if (MENU_STATUS != -1)
					ships_count_main = MENU_STATUS;
				break;
			}
			case RESET_SHIPS_COUNT:	//сбросить до стандартных количество кораблей
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
			case INSTRUCTION:	//вывести инструкцию
			{
				PRINT_INSTRUCTION(main_ships_value);
				break;
			}
			case EXIT:	//выход из главного меню
			{
				if (Exit())
					return;
			}
		}
	}
}

	//Пункты Главного меню
void NewGame(int ships_count_main){ 			//отрисовка и взаимодействие
	int i=-1;
	while(printf("Выберите режим игры:\n\t%d Человек с человеком\n\t%d Назад в меню\n", 1, 2),
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
int ChangeShipsCount(int ships_count_main){		//изменить количество кораблей
	int i, n, y;
	printf("Корабли, которые будут в игре:\n");
	PrintShipsCount(ships_count_main);
	if (Confirm("Вы точно хотите изменить количество кораблей в игре?", 0))
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
			printf("В игре в сумме может быть не более 20 клеток кораблей\n");
			printf(	"(доступно клеток: %d) и не более 9 одинаковых кораблей.\n", 20 - i);
			printf(	"Введите длину корабля, количество которого в игре хотите изменить:\n");
			fflush(stdin);
			if (scanf("%d", &y)!=1 || y < 1 || y >= ship_len_limit)
				InputErrorMessage();
			else
			{
				printf("Сколько кораблей длины %d вы хотите, чтобы было в игре?\n", y);
				printf("(Доступно для добавления клеток кораблей: %d)\n", 20 - i);
				if (scanf("%d", &n)!=1)
					InputErrorMessage();
				else
					if (n < 0 || (n * y > (20 - i) + y * ((ships_count_main / IntPow(10, y-1))%10)) || (n > 9))
					{
						system("cls");
						printf("Нельзя сделать такое количество кораблей.\n");
						continue;
					}
					else
						ships_count_main += (IntPow(10, y-1) * (n - (ships_count_main/IntPow(10, y-1))%10));
			}
			PrintShipsCount(ships_count_main);
			if (!Confirm("Завершить изменение количества кораблей?", 1))
			{
				system("cls");
				if (Confirm("Сохранить изменения количества кораблей?", 1))
				{
					system("cls");
					printf("Количество кораблей сброшено к предыдущим значениям.\n");
					return -1; //отменить сохранение нового значения кораблей
				}
				system("cls");
				if (!CountShips(ships_count_main))
				{
					printf("Необходимо, чтобы в игре был хотя бы 1 корабль!\n");
					printf("Количество кораблей сброшено к предыдущим значениям.\n");
					return -1;
				}
				printf("Количество кораблей успешно изменено.\n");
				return ships_count_main;
			}
			else
				system("cls");
		}
	}
}
int ResetShipsCount(int ships_count_main){		//запрос на сброс количества кораблей в игре
	printf("Текущие корабли для игры:\n");
	PrintShipsCount(ships_count_main);
	printf("Текущая максимальная длина корабля в игре: %d\n", ship_len_limit - 1);
	if (Confirm("Сбросить количество кораблей и максимальную длину корабля\nв игре к стандартным значениям?\n", 0))
	{
		system("cls");
		printf("Значения сброшены к стандартным значениям.\n");
		return 0;
	}
	system("cls");
	printf("Значения не были сброшены.\n");
	return 1;
}
void ChangeMaxLen(int *ships_count){				//изменение максимальной длины корабля в игре
	int i;
	while(1)
	{
		printf("Текущая максимальная длина корабля в игре: %d\n", ship_len_limit - 1);
		printf("Максимальную длину можно поставить от 1 до 9.\n");
		fflush(stdin);
		if (scanf("%d", &i)!=1 || i < 1 || i > 9)
		{
			InputErrorMessage();
			continue;
		}
		if (Confirm("Сохранить изменение максимальной длины корабля?", 1))
		{
			system("cls");
			printf("Максимальная длина сброшена к предыдущим значениям.\n");
		}
		else
		{
			if (ship_len_limit > i + 1)
				*ships_count -= (*ships_count / IntPow(10, i)) * IntPow(10, i);
			ship_len_limit = i + 1;
			system("cls");
			printf("Максимальная длина успешно изменена.\n");
		}
		break;
	}
}
void PRINT_INSTRUCTION(int ships_count_main){ 	//вывести на экран инструкцию по игре
	int x, y;
	char field[4][4]={0};
	system("cls");
	printf("///////////////////////////////////ИНСТРУКЦИЯ///////////////////////////////////\n\n");
	printf( "(Это инструкция для этой реализации игры. Значения размера поля, длин и\n"
			"количества кораблей указаны для стандартных значений).\n\n"
			"'Морской бой' - это игра для двух участников, в которой игроки ходят по очереди,\n"
			"где главной целью является уничтожение всех кораблей оппонента.\n"
			"Игровым полем является квадрат %d на %d клеток.\n"
			"Символы, которыми отображаются клетки игрового поля:\n", FIELD_SIZE, FIELD_SIZE);
	printf("'%c' - пустая клетка;\n", draw_symbol[EMPTY]);
	printf("'%c' - клетка, куда уже выстрелили ранее;\n", draw_symbol[SHOT]);
	printf("'%c' - клетка с раненым кораблём;\n", draw_symbol[STRIKE]);
	printf("'%c' - клетка с уничтоженным кораблём;\n", draw_symbol[KILL]);
	printf("'%c' - клетка с кораблём.\n", draw_symbol[SHIP]);
	printf("'|' - горизонтальная граница поля.\nБуквы от A до %c - горизонтальные координаты, "
		   "цифры от 0 до %d - вертикальные координаты.\nЧтобы совершить ход, надо указать\n"
		   "сначала горизонтальную, потом вертикальную координату.\n"
		   "Пример доступного хода: 'G3' либо 'g3', но не '3g' (в обратном порядке)!\n"
		   "На поле размещают %d кораблей %d видов разной длины: \n", FIELD_SIZE - 1 + 'A', FIELD_SIZE - 1, CountShips(ships_count_main), ship_len_limit - 1);
	for(x = ship_len_limit - 1; x > 0; x--)
	{
		y = (ships_count_main / IntPow(10, x - 1))%10;
		if (y > 1)
			printf("\t%d кораблей %d клеток в длину.\n", y, x);
		else
			printf("\t%d корабль  %d клеток в длину.\n", y, x);
	}
	printf("Размещать корабли можно вертикально и горизонтально, но не диагонально.\n"
		   "На примере двухпалубного корабля на поле 4 на 4:\n");
	field[1][2] = SHIP;
	field[1][3] = SHIP;
	for(printf("Как МОЖНО размещать корабли (корабль расположен горизонтально):\n    A B C D\n"), y=0; y<4; y++, printf("|\n"))
		for(printf("%2d|", y), x=0; x<4 ;printf("%2c", draw_symbol[field[y][x]]), x++);	
	field[1][3] = SHIP;
	field[2][2] = SHIP;
	field[1][2] = EMPTY;
	for(printf("Как НЕЛЬЗЯ размещать корабли (корабль расположен по диагонали):\n    A B C D\n"), y=0; y<4; y++, printf("|\n"))
		for(printf("%2d|", y), x=0; x<4 ;printf("%2c", draw_symbol[field[y][x]]), x++);
	printf("Корабли нельзя расставлять ближе, чем на 1 клетку друг от друга в любом направлении.\n");
	printf("На примере двухпалубных кораблей на поле 4 на 4:\n");
	field[1][2] = SHIP;
	field[2][2] = EMPTY;
	field[2][3] = EMPTY;
	field[3][0] = SHIP;
	field[3][1] = SHIP;
	for(printf("Как МОЖНО размещать корабли:\n    A B C D\n"), y=0; y<4; y++, printf("|\n"))
		for(printf("%2d|", y), x=0; x<4 ;printf("%2c", draw_symbol[field[y][x]]), x++);
	field[2][1] = SHIP;
	field[2][0] = SHIP;
	field[3][0] = EMPTY;
	field[3][1] = EMPTY;
	for(printf("Как НЕЛЬЗЯ размещать корабли (расстояние до другого корабля меньше 1 клетки):\n    A B C D\n"), y=0; y<4; y++, printf("|\n"))
		for(printf("%2d|", y), x=0; x<4 ;printf("%2c", draw_symbol[field[y][x]]), x++);
	printf("Для уничтожения корабля необходимо попасть по каждой его части 1 раз.\n"
			"После того, как игрок попадёт во вражеский корабль или взорвёт его,\n"
			"он получит 1 дополнительный ход. Клетка считается ранением,\n"
			"если попадание по этой клетке не уничтожило корабль.\n"
			"После уничтожения корабля, из-за взрыва, поле в радиусе 1 клетки вокруг всего\n"
			"корабля будет отмечено так, будто туда уже выстрелили.\n");
	printf("\n////////////////////////////////КОНЕЦ ИНСТРУКЦИИ////////////////////////////////\n\n");
}
int Exit(){ 									//запрос подтверждения на выход
	if (Confirm("Вы точно хотите выйти? ", 1))
	{
		system("cls");
		return 0;	//программа продолжит работу
	}
	system("pause");
	return 1; 		//программа завершится
}

	//Пункты меню Новой игры. Инициирование игры
void PLAYER_VS_PLAYER(int ships_count_main){ 	//человека с человеком
	char i = 0, flag, win = 0;
	int ships_count_player_1 = ships_count_main;
	int ships_count_player_2 = ships_count_main;
	char field_owner_player_1[FIELD_SIZE][FIELD_SIZE]={0};
	char field_owner_player_2[FIELD_SIZE][FIELD_SIZE]={0};
	int player_1_colorid = InputColorID("Первый игрок");
	int player_2_colorid = InputColorID("Второй игрок");
	printf("Сейчас очередь "); 
	PrintColored("Первого игрока ", player_1_colorid);
	printf("расставлять корабли.\n");
	if (Confirm("Расставить корабли вручную?", 0))
		SetShipsRandom(field_owner_player_1[0], ships_count_player_1);
	else
		SetShips(field_owner_player_1[0], ships_count_player_1);
	system("cls");
	printf("Ваши корабли успешно установлены.\nТеперь очередь ");
	PrintColored("Второго игрока ", player_2_colorid);
	printf("расставлять корабли.\n");
	if (Confirm("Расставить корабли вручную?", 0))
		SetShipsRandom(field_owner_player_2[0], ships_count_player_2);
	else
		SetShips(field_owner_player_2[0], ships_count_player_2);
	system("cls");
	printf("Все корабли успешно расставлены.\n"
			"Перед началом хода другого игрока каждый раз будет ставиться пауза и воспроизводиться звуковой сигнал.\n"
			"После продолжения ходить будет ");
	PrintColored("Первый игрок", player_1_colorid);
	printf(", поэтому сразу отобразится поле");
	PrintColored(" Первого игрока", player_1_colorid);
	printf(",\nс отображёнными на нём кораблями, будьте внимательны!\n\nНачинается Морской бой. Приятной игры!\n");
	while(1)
	for(flag = 1, i = 0; 1; flag = 0, i = (!i))
	{
		if (!flag) //чтобы избежать лишнего текста при самом первом ходе
		{
			printf("\aСмена очереди хода!\n");
			if (!i)
			{
				printf("Сейчас будет ходить ");
				PrintColored("Первый игрок", player_1_colorid);
				printf(".\n");
			}
			else
			{
				printf("Теперь ходит ");
				PrintColored("Второй игрок", player_2_colorid);
				printf(".\n");
			}
		}
		system("pause");
		system("cls");
		if (!i) 	//переключатель хода
			win = StrikeTurn(field_owner_player_1[0], field_owner_player_2[0], "Первого игрока", player_1_colorid, player_2_colorid, &ships_count_player_2);
		else
			win = StrikeTurn(field_owner_player_2[0], field_owner_player_1[0], "Второго игрока", player_2_colorid, player_1_colorid, &ships_count_player_1);
		if (win)
		{
			system("cls");
			printf("\aВсе корабли ");
			if (i)
				PrintColored("Первого игрока ", player_1_colorid);
			else
				PrintColored("Второго игрока ", player_2_colorid);
			printf("потоплены!\nПобеду одержал ");
			if (!i)
				PrintColored("Первый игрок", player_1_colorid);
			else
				PrintColored("Второй игрок", player_2_colorid);		
			printf(".\nСпасибо за игру!\n");
			system("pause");
			system("cls");
			return;
		}
	}
}

	//Отрисовка игровых полей
int PrintField(char *field){ 											//одного, в виде хранимых данных
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
int PrintSymField(char *field){ 										//одного, в виде игровых символов
	if (!field)
		return 1; //указатель оказался ошибочным
	int x, y;
	for(draw_top_letters(), printf("\n"), y=0; y<FIELD_SIZE; y++, printf("|\n"))
		for(printf("%2d|", y), x=0; x<FIELD_SIZE ;printf("%2c", draw_symbol[field[FIELD_SIZE*y+x]]), x++);
	return 0;	// вывод прошёл успешно
}
void draw_fields(char *first, char *second, int owner_c, int enemy_c){ 	//двух рядом, с учётом владения полем
	int y;
	PrintWordNTimes(" ", FIELD_SIZE - 1);
	PrintColored("Ваше поле ", owner_c);
	PrintWordNTimes(" ", 2 * FIELD_SIZE + 3);
	PrintColored("Поле врага", enemy_c);
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
void draw_row(char *field, int y, int owner){ 	//строки с учётом владения полем
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
void draw_top_letters(){						//буквенная полоса горизонтальных координат
	short i;
	for(printf("   "), i = 0; i < FIELD_SIZE; i++)
		printf(" %c", i + 'A');
}

	//Сообщение об ошибке о неправильном вводе:
void InputErrorMessage(){ 		//с предварительной чисткой экрана
	system("cls");
	InputErrorMessageText();
}
void InputErrorMessageText(){	//без чистки экрана
	printf("Неправильный ввод! Попробуйте ещё раз.\n\n");
}

	//Ввод, Вывод, вычисления
int InputCoordinates(int *x, int *y){ 					//ввод координат напрямую в x и y
	char input_x = -1;
	if(fflush(stdin), scanf("%c%d", &input_x, y)!=2 || ((input_x < 'A' || input_x > 'J') &&
		(input_x < 'a' || input_x > 'j')) || (*y < 0 || *y >= FIELD_SIZE) //проверки ввода координат
	  )
		return 1; //ввод некорректен
	if (input_x <= 'J') //перевод 'x'-а из символа в число
		*x = input_x - 'A';
	else 
		*x = input_x - 'a';
	printf("InputCoordinates: x = %d, y = %d\n", *x, *y);
	return 0; //ввод корректен
}
int IntPow(int x, int y){ 								//int x в степень int y
	int a = 1;
	for(y; y > 0; y--)
			a *= x;
	return a;
}
void PrintWordNTimes(char *word, int n){ 				//вывести строку n раз
	for(n; n > 0; n--)
		printf("%s", word);
}
void PrintShipsCount(int ships_count_main){				//вывести количество кораблей в игре
	char n;
	for(printf(	"Длина корабля в клетках:   |"), n = ship_len_limit - 1; n > 0; n--)
		printf(" %d |", n);
	printf( 	"\n                           |");
	PrintWordNTimes("---|", ship_len_limit - 1);
	for(printf( "\nКоличество кораблей:       |"), n = ship_len_limit - 1; n > 0; n--)
		printf(" %d |", (ships_count_main/IntPow(10, n - 1))%10);
	printf("\n");
}
int Confirm(char *string_before_confirm, int clear){ 	//запрос: 0 - да, 1 - нет. clear 0 не чистит экран
	int y;
	while(printf("%s\n'0' - если да.\n'1' - если нет.\n", string_before_confirm)
			  ,fflush(stdin), scanf("%d", &y)!=1 || (y < 0 || y > 1))
			if (clear)
				InputErrorMessage(); 		//с очисткой экрана
			else
				InputErrorMessageText(); 	//без очистки
	return y;
}
int CountShips(int ships_count_main){					//посчитать количество кораблей в числе
	int i, s = 0;
	for(i = ship_len_limit - 1; i > 0; i--)
		s += (ships_count_main / IntPow(10, i - 1))%10;
	return s;
}
void PrintColored(char *string, int ColorID){			//вывести текст цветом ColorID
	printf("\033[1;3%dm", ColorID);
	printf("%s\033[0m", string);
}
void PrintAvailableColors(){							//вывести доступные цвета (ColorID)
								//ColorID:
	printf("\033[1;30m0 Серый\n");		//0
	printf("\033[1;31m1 Красный\n");	//1
	printf("\033[1;32m2 Зелёный\n");	//2
	printf("\033[1;33m3 Жёлтый\n");		//3
	printf("\033[1;34m4 Синий\n");		//4
	printf("\033[1;35m5 Фиолетовый\n");	//5
	printf("\033[1;36m6 Голубой\n");	//6
	printf("\033[1;37m7 Белый\n");		//7
	
	printf("\033[0m"); //возвращение к стандартному цвету вывода
}
int InputColorID(char *word){							//запрос на ввод ColorID
	int i;
	while(1)
	{
		printf("Доступные цвета:\n");
		PrintAvailableColors();
		printf("Введите код цвета, которым будет отображаться надпись %s и название поля:\n", word);
		fflush(stdin);
		if (scanf("%d", &i)!=1 || i < 0 || i > 7)
		{
			InputErrorMessage();
			continue;
		}
		printf("Пример отображения:\n");
		PrintColored(word, i);
		printf("\n");
		PrintColored("Ваше поле", i);
		printf("\nДля соперника название поля будет выглядеть так: ");
		PrintColored("Поле врага", i);
		if (!Confirm("\nСохранить цвет отображения?", 0))
		{
			system("cls");
			return i;
		}
		system("cls");
	}
}
int XYChangesBySide(int x_is_0_y_is_1, int side){		//значения изменения x, y в зависимости от стороны
    if (x_is_0_y_is_1)	//в зависимости от направления x = 0, 1, 0, -1;    y = 1, 0, -1, 0
    	return (side < 3) ? 0 - side%2 : (side == 3) ? 1 : 0;
	return (side%2) ? 0 : (side == 2) ? 1 : -1;
}
void DeleteObjectsFromField(char *field, int object){	//заменить из массива все значения object на EMPTY
	int i;
	if (object == -1)
		for(i = FIELD_SIZE * FIELD_SIZE - 1; i >= 0; i--)
			field[i] = EMPTY;
	else
		for(i = FIELD_SIZE * FIELD_SIZE - 1; i >= 0; i--)
			if (field[i] == object)
				field[i] = EMPTY;
}
void EquateArrays(char *from_this_array, char *to_this_array){	//приравнять правый массив к левому
	int i = FIELD_SIZE * FIELD_SIZE - 1;
	for(i; i >= 0; i--)
		to_this_array[i] = from_this_array[i];
}
unsigned int GetNumberBySeed(int seed, int limit){		//сгенерировать число на основе семени
	if (limit)
		return (unsigned int)(19257 * seed + 1234567) % limit;
	return (unsigned int)(19257 * seed + 1234567) % 1000000;
}


	//Работа с кораблями
void SetShips(char *field_owner, int ships_count){ 	//начинает расстановку по значению ships_count
	int i, n;
	printf("Расставьте корабли.\n");
	for(n = ship_len_limit - 1; n > 0; n--)		//из n = максимальная длина корабля
		for(i = (ships_count / IntPow(10, n-1))%10; i > 0; i--) // i = декодированное из ships_count
			SetOneShip(field_owner, n); 					//по длине кол-во кораблей для установки 
}
void SetOneShip(char *field_owner, int ship_len){ 	//установка корабля с длиной = ship_len 
	int x = -1, y = -1, int_s, av_s, x_2;
	char side;
	if (ship_len == 1)
		{
			while(printf("Введите координаты %d палубного корабля:\n", ship_len),
					PrintSymField(field_owner), InputCoordinates(&x, &y), CheckOneBlock(field_owner, x, y))
				InputErrorMessage();
			field_owner[FIELD_SIZE*y+x] = SHIP;
			system("cls");
			printf("Корабль успешно установлен.\n");
			return; //корабль в 1 клетку можно установить после круговой проверки, поэтому завершаем функцию
		}
	else
		while(1)
		{
			while(printf("Введите координаты начальной точки %d палубного корабля:\n", ship_len),
							PrintSymField(field_owner), InputCoordinates(&x, &y), CheckOneBlock(field_owner, x, y))
				InputErrorMessage();
			system("cls");
			side = field_owner[FIELD_SIZE*y+x]; //на случай, если что-то не так, и на (x, y) не EMPTY
			field_owner[FIELD_SIZE*y+x] = SHIP;
			PrintSymField(field_owner);
			field_owner[FIELD_SIZE*y+x] = side; //Т.к. нет уверенности, что корабль установят верно
			side = 5;
			av_s = AvailableSides(field_owner, x, y, ship_len, 0); //в av_s кодируются доступные стороны
			if (!av_s)	//если во всех направлениях мешает или корабль или граница поля
			{
				system("cls");
				printf("Из данной точки нельзя установить %d палубный корабль.\n", ship_len);
				continue;
			}
			while(AvailableSides(field_owner, x, y, ship_len, 1), printf("(Для отмены установки корабля можно ввести 0)\n"
					"Введите номер направления, в котором будет установлен корабль: "), fflush(stdin),
					scanf("%c", &side)!=1 || side < '0'|| side > '4')
				InputErrorMessageText();
			if (side == '0')
			{
				system("cls");
				continue;
			}
			int_s = side - '0';
			if(!((av_s/IntPow(10, 4 - int_s))%10)) 	//проверяется доступность стороны int_s в av_s
				printf("Нельзя поставить корабль в этом направлении.\n");
			else
				break;
		}
	SetInSide(field_owner, SHIP, int_s, ship_len, x, y);
	printf("Корабль успешно установлен.\n");
	return;
}
void SetShipsRandom(char *players_field, int ships_count){	//расставляет корабли в случайном порядке
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
		if (Confirm("Сгенерировать поле по \"семечку\" вместо случайной генерации?", 1))
		{
			srand(time(NULL));
			seed = rand();	//количество возможных вариаций ограничено, будут повторы
		}
		else
			while(printf("Введите семя, по которому будет сгенерировано поле: "), fflush(stdin),
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
		if (!Confirm("Такое поле подойдёт?", 0))
		{
			system("cls");
			EquateArrays(random_field, players_field);
			printf("Поле успешно сохранено.\n");
			break;
		}
		else
			system("cls");
	}
}
void SetInSide(char *field, int set_what, int side, int ship_len, int x, int y){	//ставит set_what, len клеток в направлении side
	int x_2 = XYChangesBySide(0, side), y_2 = XYChangesBySide(1, side);
	for(side = 0, x = x - x_2, y = y - y_2; side < ship_len; x = x + x_2, y = y + y_2, side++)
		field[FIELD_SIZE * (y + y_2) + (x + x_2)] = set_what;
}
int SetShotAroundShips(char *field, int side, int len, int x, int y, int *array){	//возвращает итоговое кол-во уст. SHOT
	int i = 0;
	i += ShotToWallOfSide(field, side, x, y, 1, array);
	i += ShotToMiddleOfSide(field, side, x, y, len, array);
	x += (len - 1) * XYChangesBySide(0, side);
	y += (len - 1) * XYChangesBySide(1, side);
	i += ShotToWallOfSide(field, side, x, y, 0, array);
}
int ShotToWallOfSide(char *field, int side, int x, int y, int back, int *array){	//возвращает количество установленных SHOT
	if (back)	//чтобы отзеркалить от side сторону для установки SHOT
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
int ShotToMiddleOfSide(char *field, int side, int x, int y, int len, int *array){	//возвращает количество установленных SHOT
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
int CheckOneBlock(char *field, int x, int y){	//0 - всё в порядке / координата за пределами поля, >0 - кол-во кораблей
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
int CheckSide(char *field, int in_s, int len, int x, int y){	//0 - всё в порядке, 1 - за пределами поля / корабли
    int x_2 = XYChangesBySide(0, in_s), y_2 = XYChangesBySide(1, in_s);
	for(in_s = 0, len--, x += x_2, y += y_2; in_s < len; x += x_2, y += y_2, in_s++)
		if ((x < 0 || x >= FIELD_SIZE) || (y < 0 || y >= FIELD_SIZE) || CheckOneBlock(field, x, y))
			return 1;
	return 0;
}
int AvailableSides(char *field, int x, int y, int ship_len, int draw){ //возвращает доступные стороны числом
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
	//то же самое, что и выше, но с отрисовкой направлений
    printf(	"У доступных для установки направлений указаны номера:\n"
			"Направление: | Вверх | Вправо|  Вниз | Влево |\n             |");
	PrintWordNTimes("-------|", 4);
	printf( "\nНомер:       |   ");
	for(i = 1; i < 5; i++, printf("   |   ")) //сторона i; от Вверх до Влево (от 1 до 4)
	{
		if (CheckSide(field, i, ship_len, x, y)) 	//если направление i недоступно
			printf(" ");
		else 										//если направление i доступно
		{
			printf("%d", i);
			sum = sum + IntPow(10, 4 - i); //кодировка стороны i как доступной
		}								//(Верх(1000) +...+ Влево(1))
	}
	printf("\n");
	return sum;
}
int FindShipEdge(char *field, int *x, int *y){	//возвращает направление корабля и в *x и *y запишет координаты края начала корабля
	if ((field[FIELD_SIZE * *y + *x + 1] == SHIP || field[FIELD_SIZE * *y + *x + 1] == STRIKE) ||
		(field[FIELD_SIZE * *y + *x - 1] == SHIP || field[FIELD_SIZE * *y + *x - 1] == STRIKE))
	{
		*x += (CalculateWhatInSide(field, *x, *y, 2, -1) - 1) * XYChangesBySide(0, 2);	//горизонтальное расположение корабля
		return 4;
	}
	else
	{
		*y += (CalculateWhatInSide(field, *x, *y, 1, -1) - 1) * XYChangesBySide(1, 1);
		return 3;
	}
}
int CalculateWhatInSide(char *field, int x, int y, int side, int what){	//считает количество what в направлении side до EMPTY или SHOT клетки
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

	//Ход для выстрела
int StrikeTurn(char *owner, char *enemy, char *who, int color, int color2, int *enemy_ships){	//возвращает 1 когда корабли игрока закончились, иначе 0
	int hit = 1;
	while(hit)
		{
			PrintWordNTimes(" ", 2 * FIELD_SIZE + 1);
			printf("Ход ");
			PrintColored(who, color);
			printf("\n");
			hit = strike(owner, enemy, color, color2, enemy_ships);
			system("cls");
			switch (hit)
				{
				case 0:
					{
						printf("Промах!\n");
						break;
					}
				case 1:
					{
						printf("Попадание! Вы получаете дополнительный ход.\n");
						break;
					}
				case 2:
					{
						if (!(*enemy_ships))
							return 1;
						printf("Корабль взорван!\nОставшееся количество вражеских кораблей:\n");
						PrintShipsCount(*enemy_ships);
						break;
					}
				case 3:
					{
						system("cls");
						printf("Туда нет смысла стрелять!\n");
						break;
					}
				}
		}
	return 0;
}
int strike(char *field_owner, char *field_for_strike, int color1, int color2, int *ships_remain){ 	//добавить проверку финального выстрела по кораблю!!!!
	int x, y, hit = 0, side = -1, len = -1;
	while(draw_fields(field_owner, field_for_strike, color1, color2), printf("Введите координаты для выстрела: "), InputCoordinates(&x, &y))
		InputErrorMessage();
	switch(field_for_strike[FIELD_SIZE*y+x])
	{
		case EMPTY: //если в этой клетке пусто
			{
			field_for_strike[FIELD_SIZE*y+x] = SHOT;
			break;
			}
		case SHIP:	//если в этой клетке корабль
			{
			field_for_strike[FIELD_SIZE*y+x] = STRIKE;
			side = FindShipEdge(field_for_strike, &x, &y);
			len = CalculateWhatInSide(field_for_strike, x, y, side, -1);
			if (CalculateWhatInSide(field_for_strike, x, y, side, STRIKE) == len)
			{
				hit = 2;	//подрыв
				SetInSide(field_for_strike, KILL, side, len, x, y);
				SetShotAroundShips(field_for_strike, side, len, x, y, 0);
				*ships_remain -= IntPow(10, len - 1);
			}
			else
				hit = 1;	//попадание
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


int main(){ //Первое приветствие и вызов главного меню
	setlocale(LC_ALL, "Rus");
	printf("Добро пожаловать в игру 'Морской бой'.\n");
	MainMenu();
	return 0;
}
