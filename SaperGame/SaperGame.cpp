#include <iostream>
#include <ctime>
#include <iomanip>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>

using namespace std;

void choice_level() {//выбор уровня игры
	cout << "Выберете уровень игры:" << endl;
	cout << "\t1 - Рядовой;\n\t2 - Сержант;\n\t3 - Офицер;" << endl;
	cout << "...: ";
}
int init_level(int l) {//инициализация уровня игры
	if (l == 1)
		l = 8;
	else
		if (l == 2)
			l = 13;
		else
			if (l == 3)
				l = 18;
			else
				cout << "Такого уровня нет!\nПопробуете выбрать еще раз? (Y/N): ";
	return l;
}
template <typename T>
T** get_pole_play(T** p, int l) {//создание игрового поля
	T** np = new T * [l];
	for (int i = 0; i < l; i++)
		np[i] = new T[l];
	for (int i = 0; i < l; i++)
		for (int j = 0; j < l; j++)
			np[i][j] = '-';
	delete[] p;
	return np;
}
template <typename T>
void show_pole(T** p, int l) {//печать игрового поля
	cout << setw(4) << " ";
	for (int i = 0; i < l; i++)
		cout << setw(4) << i + 1;
	cout << endl;
	cout << setw(4) << " ";
	for (int i = 0; i < l; i++)
		cout << "----";
	cout << endl;
	for (int i = 0; i < l; i++) {
		cout << setw(3) << i + 1 << "|";
		for (int j = 0; j < l; j++)
			cout << setw(4) << p[i][j];
		cout << endl;
	}
}
template <typename T>
T** get_pole_bomb(T** p, int l) {//создание минного поля
	T** np = new T * [l];
	for (int i = 0; i < l; i++)
		np[i] = new T[l];
	int bomb = 0.15 * l * l;
	int i, j;
	while (bomb != 0) {//расставляем мины
		i = rand() % l;
		j = rand() % l;
		if (np[i][j] != 'B' && i != 0 && j != 0) {
			np[i][j] = 'B';
			bomb--;
		}
	}
	for (i = 0; i < l; i++)
		for (j = 0; j < l; j++)
			if (np[i][j] != 'B')
				np[i][j] = '-';
	//расставляем цифры
	for (i = 0; i < l; i++) {
		for (j = 0; j < l; j++) {
			int n = 0;
			if (np[i][j] != 'B') {
				if (i - 1 >= 0 && j - 1 >= 0 && np[i - 1][j - 1] == 'B')
					n++;
				if (i - 1 >= 0 && j + 1 < l && np[i - 1][j + 1] == 'B')
					n++;
				if (i + 1 < l && j - 1 >= 0 && np[i + 1][j - 1] == 'B')
					n++;
				if (i + 1 < l && j + 1 < l && np[i + 1][j + 1] == 'B')
					n++;
				if (i - 1 >= 0 && np[i - 1][j] == 'B')
					n++;
				if (i + 1 < l && np[i + 1][j] == 'B')
					n++;
				if (j - 1 >= 0 && np[i][j - 1] == 'B')
					n++;
				if (j + 1 < l && np[i][j + 1] == 'B')
					n++;
			}
			if (n != 0)
				np[i][j] = (char)(n + 48);
			if (np[i][j] == '-')
				np[i][j] = ' ';
		}
	}
	delete[] p;
	return np;
}
template <typename T>
void open_near(T** pb, T** np, int l, int j, int i) {//открываем поля вокруг клетки
	if (i - 1 >= 0 && j - 1 >= 0 && np[i - 1][j - 1] != '+' && np[i - 1][j - 1] != 'a' && np[i - 1][j - 1] != '!')
		np[i - 1][j - 1] = pb[i - 1][j - 1];
	if (i - 1 >= 0 && j + 1 < l && np[i - 1][j + 1] != '+' && np[i - 1][j + 1] != 'a' && np[i - 1][j + 1] != '!')
		np[i - 1][j + 1] = pb[i - 1][j + 1];
	if (i + 1 < l && j - 1 >= 0 && np[i + 1][j - 1] != '+' && np[i + 1][j - 1] != 'a' && np[i + 1][j - 1] != '!')
		np[i + 1][j - 1] = pb[i + 1][j - 1];
	if (i + 1 < l && j + 1 < l && np[i + 1][j + 1] != '+' && np[i + 1][j + 1] != 'a' && np[i + 1][j + 1] != '!')
		np[i + 1][j + 1] = pb[i + 1][j + 1];
	if (i - 1 >= 0 && np[i - 1][j] != '+' && np[i - 1][j] != 'a' && np[i - 1][j] != '!')
		np[i - 1][j] = pb[i - 1][j];
	if (i + 1 < l && np[i + 1][j] != '+' && np[i + 1][j] != 'a' && np[i + 1][j] != '!')
		np[i + 1][j] = pb[i + 1][j];
	if (j - 1 >= 0 && np[i][j - 1] != '+' && np[i][j - 1] != 'a' && np[i][j - 1] != '!')
		np[i][j - 1] = pb[i][j - 1];
	if (j + 1 < l && np[i][j + 1] != '+' && np[i][j + 1] != 'a' && np[i][j + 1] != '!')
		np[i][j + 1] = pb[i][j + 1];
}

template <typename T>
T** open_empty_square(T** pb, T** np, int l, int x, int y) {//открытие пустых клеток
	x -= 1;
	y -= 1;
	if (pb[y][x] == ' ') {
		np[y][x] = ' ';
		open_near(pb, np, l, x, y);
		np[y][x] = '+';
	}
	else
		return np;

	bool b = true;
	while (b == true) {
		//открываем пустые клетки с рядом стоящих пустых клеток
		if (y + 1 < l && x + 1 < l && pb[y + 1][x + 1] == ' ' && np[y + 1][x + 1] != '!')
			open_near(pb, np, l, x + 1, y + 1);
		if (x + 1 < l && pb[y][x + 1] == ' ' && np[y][x + 1] != '!')
			open_near(pb, np, l, x + 1, y);
		if (y + 1 < l && pb[y + 1][x] == ' ' && np[y + 1][x] != '!')
			open_near(pb, np, l, x, y + 1);
		if (y - 1 >= 0 && x + 1 < l && pb[y - 1][x + 1] == ' ' && np[y - 1][x + 1] != '!')
			open_near(pb, np, l, x + 1, y - 1);
		if (y + 1 < l && x - 1 >= 0 && pb[y + 1][x - 1] == ' ' && np[y + 1][x - 1] != '!')
			open_near(pb, np, l, x - 1, y + 1);
		if (y - 1 >= 0 && pb[y - 1][x] == ' ' && np[y - 1][x] != '!')
			open_near(pb, np, l, x, y - 1);
		if (x - 1 >= 0 && pb[y][x - 1] == ' ' && np[y][x - 1] != '!')
			open_near(pb, np, l, x - 1, y);
		if (y - 1 >= 0 && x - 1 >= 0 && pb[y - 1][x - 1] == ' ' && np[y - 1][x - 1] != '!')
			open_near(pb, np, l, x - 1, y - 1);
		//шагаем по пустым полям и ставим метку +
		if (y + 1 < l && x + 1 < l && pb[y + 1][x + 1] == ' ' && np[y + 1][x + 1] != '+' && np[y + 1][x + 1] != 'a' && np[y + 1][x + 1] != '!') {
			y += 1;
			x += 1;
			np[y][x] = '+';
		}
		else
			if (x + 1 < l && pb[y][x + 1] == ' ' && np[y][x + 1] != '+' && np[y][x + 1] != 'a' && np[y][x + 1] != '!') {
				x += 1;
				np[y][x] = '+';
			}
			else
				if (y + 1 < l && pb[y + 1][x] == ' ' && np[y + 1][x] != '+' && np[y + 1][x] != 'a' && np[y + 1][x] != '!') {
					y += 1;
					np[y][x] = '+';
				}
				else
					if (y - 1 >= 0 && x + 1 < l && pb[y - 1][x + 1] == ' ' && np[y - 1][x + 1] != '+' && np[y - 1][x + 1] != 'a' && np[y - 1][x + 1] != '!') {
						y -= 1;
						x += 1;
						np[y][x] = '+';
					}
					else
						if (y + 1 < l && x - 1 >= 0 && pb[y + 1][x - 1] == ' ' && np[y + 1][x - 1] != '+' && np[y + 1][x - 1] != 'a' && np[y + 1][x - 1] != '!') {
							y += 1;
							x -= 1;
							np[y][x] = '+';
						}
						else
							if (y - 1 >= 0 && pb[y - 1][x] == ' ' && np[y - 1][x] != '+' && np[y - 1][x] != 'a' && np[y - 1][x] != '!') {
								y -= 1;
								np[y][x] = '+';
							}
							else
								if (x - 1 >= 0 && pb[y][x - 1] == ' ' && np[y][x - 1] != '+' && np[y][x - 1] != 'a' && np[y][x - 1] != '!') {
									x -= 1;
									np[y][x] = '+';
								}
								else
									if (y - 1 >= 0 && x - 1 >= 0 && pb[y - 1][x - 1] == ' ' && np[y - 1][x - 1] != '+' && np[y - 1][x - 1] != 'a' && np[y - 1][x - 1] != '!') {
										y -= 1;
										x -= 1;
										np[y][x] = '+';
									}
									else//если нет пустых полей шагаем назад по '+' и ставим другую метку 'а' (когда зашли в тупик)
										if (y - 1 >= 0 && x - 1 >= 0 && np[y - 1][x - 1] == '+' && np[y - 1][x - 1] != '!') {
											np[y][x] = 'a';
											y -= 1;
											x -= 1;
										}
										else
											if (x - 1 >= 0 && np[y][x - 1] == '+' && np[y][x - 1] != '!') {
												np[y][x] = 'a';
												x -= 1;
											}
											else
												if (y - 1 >= 0 && np[y - 1][x] == '+' && np[y - 1][x] != '!') {
													np[y][x] = 'a';
													y -= 1;
												}
												else
													if (y + 1 < l && x - 1 >= 0 && np[y + 1][x - 1] == '+' && np[y + 1][x - 1] != '!') {
														np[y][x] = 'a';
														y += 1;
														x -= 1;
													}
													else
														if (y - 1 >= 0 && x + 1 < l && np[y - 1][x + 1] == '+' && np[y - 1][x + 1] != '!') {
															np[y][x] = 'a';
															y -= 1;
															x += 1;
														}
														else
															if (y + 1 < l && np[y + 1][x] == '+' && np[y + 1][x] != '!') {
																np[y][x] = 'a';
																y += 1;
															}
															else
																if (x + 1 < l && np[y][x + 1] == '+' && np[y][x + 1] != '!') {
																	np[y][x] = 'a';
																	x += 1;
																}
																else
																	if (y + 1 < l && x + 1 < l && np[y + 1][x + 1] == '+' && np[y + 1][x + 1] != '!') {
																		np[y][x] = 'a';
																		y += 1;
																		x += 1;
																	}
																	else
																		b = false;
	}
	for (int i = 0; i < l; i++)//избавляемся от меток
		for (int j = 0; j < l; j++)
			if (np[i][j] == 'a' || np[i][j] == '+')
				np[i][j] = ' ';
	return np;
}

void action() {
	cout << "Выберите действие:" << endl;
	cout << "\t1 - ИГРАТЬ;\n\t2 - Показать таблицу лидеров;\n\t3 - Покинуть игру.\n...: ";
}

template <typename T>
bool check_win(T** pb, T** np, int l) {//проверяем победу
	int bomb = l * l * 0.15;
	int B = 0;
	int k = 0;
	bool win = false;
	for (int i = 0; i < l; i++)
		for (int j = 0; j < l; j++) {
			if (np[i][j] != '-' && np[i][j] != '!')
				k++;
			if (pb[i][j] == 'B' && np[i][j] == '!')
				B++;
		}
	if (B == bomb) {
		win = true;
	}
	else
		if (k == (l * l - bomb)) {
			win = true;
		}
	return win;
}

#define _CRT_SECURE_NO_WARNINGS

struct leader {
	char name[20];
	int time_min;
	int time_sec;
};

void include_player(leader l[], int level, char name[], int t_m, int t_s) {
	if (level == 8) {
		for (int i = 0; i < 3; i++) {
			if (t_m < l[i].time_min) {
				strcpy_s(l[i].name, name);
				l[i].time_min = t_m;
				l[i].time_sec = t_s;
				break;
			}
			else
				if (t_m == l[i].time_min)
					if (t_s < l[i].time_sec) {
						strcpy_s(l[i].name, name);
						l[i].time_sec = t_s;
						break;
					}
		}
	}
	else
		if (level == 13) {
			for (int i = 3; i < 6; i++) {
				if (t_m < l[i].time_min) {
					strcpy_s(l[i].name, name);
					l[i].time_min = t_m;
					l[i].time_sec = t_s;
					break;
				}
				else
					if (t_m == l[i].time_min)
						if (t_s < l[i].time_sec) {
							strcpy_s(l[i].name, name);
							l[i].time_sec = t_s;
							break;
						}
			}
		}
		else
			if (level == 18) {
				for (int i = 6; i < 9; i++) {
					if (t_m < l[i].time_min) {
						strcpy_s(l[i].name, name);
						l[i].time_min = t_m;
						l[i].time_sec = t_s;
						break;
					}
					else
						if (t_m == l[i].time_min)
							if (t_s < l[i].time_sec) {
								strcpy_s(l[i].name, name);
								l[i].time_sec = t_s;
								break;
							}
				}
			}
}

int main() {
	setlocale(LC_ALL, "rus");
	srand(time(0));

	HANDLE hWndConsole = GetStdHandle(-12);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	GetConsoleScreenBufferInfo(hWndConsole, &consoleInfo);
	short widht = consoleInfo.srWindow.Right + 1;
	short height = consoleInfo.srWindow.Bottom + 1;
	COORD coord = { widht / 2 - 11,height / 2 - 2 };
	SetConsoleCursorPosition(hWndConsole, coord);
	cout << "И Г Р А     С А П Е Р !" << endl;
	coord = { widht / 2 - 18, height / 2 - 1 };
	SetConsoleCursorPosition(hWndConsole, coord);
	cout << "(нажмите любую клавишу для продолжения)";

	_getch();
	system("cls");
	SetConsoleCP(1251);

	char exit = 'y';
	const int champions = 9;
	leader leader_list[champions]{};
	//читаем данные из файла и записываем в массив структур
	FILE* f;
	const char* path = "D:\\LeaderList.txt";

	if (fopen_s(&f, path, "r") != NULL)
		cout << "Файл " << path << " не может быть открыт!" << endl;
	else {
		char str1[10], str2[15], str3[10];
		int n;
		while (!feof(f)) {
			fscanf_s(f, "%s%s", &str1, (unsigned int)sizeof(str1), &str2, (unsigned int)sizeof(str2));
			fscanf_s(f, "%s%20s%30s", &str1, (unsigned int)sizeof(str1), &str2, (unsigned int)sizeof(str2), &str3, (unsigned int)sizeof(str3));
			for (int i = 0; i < 3; i++) {
				fscanf_s(f, "%d%s%d%s%d%s", &n, &leader_list[i].name, (unsigned int)sizeof(leader_list[i].name), &leader_list[i].time_min, &str1, (unsigned int)sizeof(str1), &leader_list[i].time_sec, &str2, (unsigned int)sizeof(str2));
			}
			fscanf_s(f, "%s%s", &str1, (unsigned int)sizeof(str1), &str2, (unsigned int)sizeof(str2));
			fscanf_s(f, "%s%20s%30s", &str1, (unsigned int)sizeof(str1), &str2, (unsigned int)sizeof(str2), &str3, (unsigned int)sizeof(str3));
			for (int i = 0; i < 3; i++) {
				fscanf_s(f, "%d%s%d%s%d%s", &n, &leader_list[i + 3].name, (unsigned int)sizeof(leader_list[i + 3].name), &leader_list[i + 3].time_min, &str1, (unsigned int)sizeof(str1), &leader_list[i + 3].time_sec, &str2, (unsigned int)sizeof(str2));
			}
			fscanf_s(f, "%s%s", &str1, (unsigned int)sizeof(str1), &str2, (unsigned int)sizeof(str2));
			fscanf_s(f, "%s%20s%30s", &str1, (unsigned int)sizeof(str1), &str2, (unsigned int)sizeof(str2), &str3, (unsigned int)sizeof(str3));
			for (int i = 0; i < 3; i++) {
				fscanf_s(f, "%d%s%d%s%d%s", &n, &leader_list[i + 6].name, (unsigned int)sizeof(leader_list[i + 6].name), &leader_list[i + 6].time_min, &str1, (unsigned int)sizeof(str1), &leader_list[i + 6].time_sec, &str2, (unsigned int)sizeof(str2));
			}
			fscanf_s(f, "%s", &str1, (unsigned int)sizeof(str1));
		}
		fclose(f);
	}
	// 

	while (exit == 'y' || exit == 'Y') {
		int choice;
		action();
		cin >> choice;
		system("cls");

		switch (choice) {
		case 1: {
			int level;
			choice_level();
			cin >> level;
			system("cls");
			level = init_level(level);
			char name[20];
			cout << "Введите свое имя: ";
			cin >> name;
			system("cls");
			char** pole_bomb = new char* [level];
			char** pole_play = new char* [level];
			pole_play = get_pole_play(pole_play, level);
			pole_bomb = get_pole_bomb(pole_bomb, level);
			bool win = true;
			clock_t t1 = clock();
			while (win == true) {
				//show_pole(pole_bomb, level); cout << endl;//для проверки раскомментировать
				show_pole(pole_play, level);
				int x, y;
				cout << "\nВведите координаты\n\tx = ";
				cin >> x;
				cout << "\ty = ";
				cin >> y;
				cout << "\n1 - открыть поле;\n2 - установить метку;\n3 - снять метку;\n4 - покинуть игру;\n...: ";
				int act;
				cin >> act;
				if (x >= 1 && x <= level && y >= 1 && y <= level) {
					switch (act) {
					case 1: {
						if (pole_bomb[y - 1][x - 1] == ' ') {
							pole_play = open_empty_square(pole_bomb, pole_play, level, x, y);
							system("cls");
						}
						else
							if (pole_bomb[y - 1][x - 1] == 'B') {
								pole_play[y - 1][x - 1] = pole_bomb[y - 1][x - 1];
								system("cls");
								clock_t t2 = clock();
								show_pole(pole_play, level);
								cout << "\nВремя игры: " << (t2 - t1) / 1000 / 60 << " мин. " << ((t2 - t1) / 1000) % 60 << " сек." << endl;
								cout << "\n\n\t\t\t\t\t\t\t\t\t\t\tВЫ ПPОИГРАЛИ!!!";
								cout << "\n\n\t\t\t\t\t\t\t\t\t\t(для продолжения нажмите любую клавишу)";
								_getch();
								win = false;
								system("cls");
							}
							else {
								pole_play[y - 1][x - 1] = pole_bomb[y - 1][x - 1];
								system("cls");
							}
						break;
					}
					case 2: {
						if (pole_play[y - 1][x - 1] == '-') {
							pole_play[y - 1][x - 1] = '!';
						}
						system("cls");

						break;
					}
					case 3: {
						if (pole_play[y - 1][x - 1] == '!') {
							pole_play[y - 1][x - 1] = '-';
						}
						system("cls");
						break;
					}
					case 4: {
						win = false;
						system("cls");
						break;
					}
					default: {
						system("cls");
						break;
					}
					}
					if (check_win(pole_bomb, pole_play, level) == 1) {
						clock_t t2 = clock();
						show_pole(pole_play, level);
						int time_m = (t2 - t1) / 1000 / 60;
						int time_s = ((t2 - t1) / 1000) % 60;
						cout << "\nВремя игры: " << time_m << " мин. " << time_s << " сек." << endl;
						//если выиграли проверяем ни призер ли игрок
						include_player(leader_list, level, name, time_m, time_s);
						//и записываем его результаты в файл
						FILE* f;
						const char* path = "D:\\LeaderList.txt";
						if ((fopen_s(&f, path, "w")) != NULL)
							cout << "Файл " << path << " не может быть октрыт!" << endl;
						else {
							fprintf(f, "%s", "Уровень \"Рядовой\":\n");
							fprintf(f, "%s%20s%30s\n", "Место", "Имя", "Время");
							for (int i = 0; i < 3; i++) {
								fprintf(f, "%3d%25s%24d мин %d сек \n", i + 1, leader_list[i].name, leader_list[i].time_min, leader_list[i].time_sec);
							}
							fprintf(f, "\n\n");
							fprintf(f, "%s", "Уровень \"Сержант\":\n");
							fprintf(f, "%s%20s%30s\n", "Место", "Имя", "Время");
							for (int i = 0; i < 3; i++) {
								fprintf(f, "%3d%25s%24d мин %d сек \n", i + 1, leader_list[i + 3].name, leader_list[i + 3].time_min, leader_list[i + 3].time_sec);
							}
							fprintf(f, "\n\n");
							fprintf(f, "%s", "Уровень \"Офицер\":\n");
							fprintf(f, "%s%20s%30s\n", "Место", "Имя", "Время");
							for (int i = 0; i < 3; i++) {
								fprintf(f, "%3d%25s%24d мин %d сек \n", i + 1, leader_list[i + 6].name, leader_list[i + 6].time_min, leader_list[i + 6].time_sec);
							}
							fclose(f);
						}
						//
						cout << "\t\t\t\t\t\t\t\t\t\t\tПОЗДРАВЛЯЮ!!!\n\t\t\t\t\t\t\t\t\t\t\tВы выиграли!!!" << endl;
						cout << "\n\n\t\t\t\t\t\t\t\t\t    (для продолжения нажмите любую клавишу)";
						_getch();
						system("cls");
						win = false;
					}
				}
				else
					system("cls");
			}
			delete[] pole_bomb;
			delete[] pole_play;
			break;
		}
		case 2: {
			FILE* f;
			const char* path = "D:\\LeaderList.txt";

			if (fopen_s(&f, path, "r") != NULL)
				cout << "Файл " << path << " не может быть открыт!" << endl;
			else {
				char str1[10], str2[15], str3[10];
				int n;
				while (!feof(f)) {
					fscanf_s(f, "%s%s", &str1, (unsigned int)sizeof(str1), &str2, (unsigned int)sizeof(str2));
					cout << str1 << " " << str2 << endl;
					fscanf_s(f, "%s%20s%30s", &str1, (unsigned int)sizeof(str1), &str2, (unsigned int)sizeof(str2), &str3, (unsigned int)sizeof(str3));
					cout << str1 << "\t\t" << str2 << "\t\t" << str3 << endl;
					for (int i = 0; i < 3; i++) {
						fscanf_s(f, "%d%s%d%s%d%s", &n, &leader_list[i].name, (unsigned int)sizeof(leader_list[i].name), &leader_list[i].time_min, &str1, (unsigned int)sizeof(str1), &leader_list[i].time_sec, &str2, (unsigned int)sizeof(str2));
						cout << "  " << n << setw(13 + strlen(leader_list[i].name)) << leader_list[i].name << setw(16 - strlen(leader_list[i].name) + 2) << leader_list[i].time_min << " " << str1 << " " << leader_list[i].time_sec << " " << str2 << endl;
					}
					cout << endl;
					fscanf_s(f, "%s%s", &str1, (unsigned int)sizeof(str1), &str2, (unsigned int)sizeof(str2));
					cout << str1 << " " << str2 << endl;
					fscanf_s(f, "%s%20s%30s", &str1, (unsigned int)sizeof(str1), &str2, (unsigned int)sizeof(str2), &str3, (unsigned int)sizeof(str3));
					cout << str1 << "\t\t" << str2 << "\t\t" << str3 << endl;
					for (int i = 0; i < 3; i++) {
						fscanf_s(f, "%d%s%d%s%d%s", &n, &leader_list[i + 3].name, (unsigned int)sizeof(leader_list[i + 3].name), &leader_list[i + 3].time_min, &str1, (unsigned int)sizeof(str1), &leader_list[i + 3].time_sec, &str2, (unsigned int)sizeof(str2));
						cout << "  " << n << setw(13 + strlen(leader_list[i + 3].name)) << leader_list[i + 3].name << setw(16 - strlen(leader_list[i + 3].name) + 2) << leader_list[i + 3].time_min << " " << str1 << " " << leader_list[i + 3].time_sec << " " << str2 << endl;
					}
					cout << endl;
					fscanf_s(f, "%s%s", &str1, (unsigned int)sizeof(str1), &str2, (unsigned int)sizeof(str2));
					cout << str1 << " " << str2 << endl;
					fscanf_s(f, "%s%20s%30s", &str1, (unsigned int)sizeof(str1), &str2, (unsigned int)sizeof(str2), &str3, (unsigned int)sizeof(str3));
					cout << str1 << "\t\t" << str2 << "\t\t" << str3 << endl;
					for (int i = 0; i < 3; i++) {
						fscanf_s(f, "%d%s%d%s%d%s", &n, &leader_list[i + 6].name, (unsigned int)sizeof(leader_list[i + 6].name), &leader_list[i + 6].time_min, &str1, (unsigned int)sizeof(str1), &leader_list[i + 6].time_sec, &str2, (unsigned int)sizeof(str2));
						cout << "  " << n << setw(13 + strlen(leader_list[i + 6].name)) << leader_list[i + 6].name << setw(16 - strlen(leader_list[i + 6].name) + 2) << leader_list[i + 6].time_min << " " << str1 << " " << leader_list[i + 6].time_sec << " " << str2 << endl;
					}
					fscanf_s(f, "%s", &str1, (unsigned int)sizeof(str1));
				}
				fclose(f);
			}
			cout << "Для продолжения нажмите любую клавишу...";
			_getch();
			system("cls");
			break;
		}
		case 3: {
			exit = 'N';
			break;
		}
		default: {
			cout << "Вы ввели не верное действие.\nПопробуете еще раз? (Y/N): ";
			cin >> exit;
			system("cls");
		}
		}
	}
	return 0;
}
