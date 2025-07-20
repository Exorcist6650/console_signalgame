#include <iostream>
#include <cmath>
#include <string>
#include <thread>
#include <chrono>
#include <limits>
#include <time.h>
#include <Windows.h>

int gameTick = 150;

const int TELESCOPESCOUNT = 8; // Количество телескопов
const char mapSymbol = '`'; // Cимоволы карты
const char radioTelescopeSymbol = '%';

// Направление
enum Direction{UP, DOWN, LEFT, RIGHT};

// Скрытие курсора
void сursorInvisible() {

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	if (console == INVALID_HANDLE_VALUE) return; // Проверка на некорректность консоли

	CONSOLE_CURSOR_INFO cursorInfo; // Объект курсора

	GetConsoleCursorInfo(console, &cursorInfo); // Ищем курсор
	cursorInfo.bVisible = false; // Cкрытие курсора

	SetConsoleCursorInfo(console, &cursorInfo); // Задаем курсор
}

// Отчистка std::cin при вызове ввода
void clearConsoleInput() {
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	FlushConsoleInputBuffer(hStdin);
}

struct Player; // Прототип структуры игрока
struct Map; // Прототип структуры позиций

// Базовый класс позиции
class Position {
	friend Player;
public:
	// Конструктор
	Position(const int x, const int y, const char symbol) {
		this->x = x;
		this->y = y;
		this->symbol = symbol;
	}

	// Создание объекта на позиции
	void positionCreate(Map& map);

	// Удаление объекта на позиции
	void positionClear(Map& map);

protected:
	int x;
	int y;
	char symbol;
};


// Класс телескопа
class Telescope : public Position {
public:
	// Конструктор
	Telescope(const int x, const int y, const char symbol, std::string name) : Position(x, y, symbol) {
		workStatus = true;
		this->name = name;
	};

	// Починка телескопа
	void repair() {
		bool isOpen = true;

		while (isOpen) {
			if (!workStatus) {
				system("cls"); // Очистка экрана
				// Генерация примера
				int userInput;
				int a = std::rand() % 9;
				int b = std::rand() % 9;
				int result = a + b;

				// Вывод примера
				std::cout << "\n\n\n\n\n\n\n\n\n\n\n\t\t" << a << " + " << b << " = ";
				clearConsoleInput(); // Очистка std::cin от нажатия клавиш

				std::cin >> userInput; // Считывание ввода пользователя

				// Корректный ответ
				if (userInput == result) {
					workStatus = true;
					std::cout << " Congratulations! Telescope " << name << " is repair!" << std::endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(1500));
					isOpen = false;
				}
				// Некорректный ответ
				else {
					std::cout << "\t\tNot correct" << std::endl;
					isOpen = false;
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				}
			}
			// Телескоп уже работает
			else {
				system("cls");
				std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\t" << name << " telescope is already working" << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(1500));
				isOpen = false;
			}
		}
	}

	// Выведение из строя
	void telescopeBreak() {
		workStatus = false;
	}

	bool GetWorkStatus() const { return workStatus; } // Геттер статуса

private:
	std::string name;
	bool workStatus; 
};


// Cтруктура карты
struct Map {
public:

	// Конструктор
	Map(const int HEIGHT, const int WIDTH) {
		this->HEIGHT = HEIGHT;
		this->WIDTH = WIDTH;

		field = new char*[HEIGHT]; // Выделение памяти для массива указателей
		for (int i = 0; i < HEIGHT; i++) {
			field[i] = new char[WIDTH]; // Выделение памяти для каждого указателя
		}
	}
	// Деструктор
	~Map() {
		for (int i = 0; i < HEIGHT; i++) {
			delete [] field[i]; // Удаление памяти для каждого указателя
		}
		delete[] field; // Удаление памяти для массива указателей
	}


	// Инициализация карты
	void mapInitialize(Telescope telescopes[], Position obj1, Position obj2) {
		// Заполнение карты символами
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				field[i][j] = mapSymbol;
			}
		}
		// Генерация телескопов
		for (int i = 0; i < TELESCOPESCOUNT; i++) {
			telescopes[i].positionCreate(*this);
		}
		// Генерация остальных объектов
		obj1.positionCreate(*this);
		obj2.positionCreate(*this);
	}

	// Отображение игры
	void mapDisplay() const {
		system("cls"); // Очистка дисплея
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				std::cout << field[i][j] << ' ';
			}
			std::cout << std::endl;
		}
	}
	char** field; // Массив указателей
	int HEIGHT; // Длина 
	int WIDTH; // Ширина
};

// Вынесение методов класса за его границы
void Position::positionCreate(Map& map) {
	map.field[y][x] = symbol;
}
void Position::positionClear(Map& map)
{
	map.field[y][x] = mapSymbol;
}


// Структура компьютера
struct Computer : public Position {
	//Конструктор
	Computer(const int x, const int y, const char symbol) : Position(x, y, symbol) {};

	// Использование компьютера
	void use(Telescope telescopeObjects[]) {
		bool isOpen = true; // Внешнее меню
		bool savingMenu = true; // Вложенное меню
		static int savedSignalsCounter = 0;

		while (isOpen) {
			int userInput; // Пользовательский ввод
			int searchTime = 3 + std::rand() % 8; // Случайное время поиска сигнала
			int chance = std::rand() % 3; // Шанс поломки телескопа
		
			bool isAllTelescopesOperational = true; // Функциональность телескопов

			clearConsoleInput(); // Очистка std::cin от нажатия клавиш
	
			system("cls"); // Очистка экрана

			// Проверка всех телескопов на функциональность
			for (int i = 0; i < TELESCOPESCOUNT; i++) {
				if (telescopeObjects[i].GetWorkStatus() == false) isAllTelescopesOperational = false;
			}
			
			if (isAllTelescopesOperational) {
				std::cout << "\t\tComputer menu\n\nSaved signals - " << savedSignalsCounter << "\n1.Find signal\n2.Exit" << std::endl; // Первый слой интерфейса

				std::cin >> userInput; // Считывание ввода

				switch (userInput) {
				case 1:
					for (int counter = 0; counter < searchTime; counter++) { // Загрузка (второй слой интерфейса)
						system("cls"); // Очистка экрана
						std::cout << "\n\t\tSearching " << counter << " seconds..." << std::endl;
						std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					}
					system("cls"); // Очистка экрана
					std::cout << "\t\tSignal is find!\n\n1. Save the signal\n2. Delete the signal" << std::endl; // Третий слой интерфейса


					std::cin >> userInput; // Считывание ввода

					while (savingMenu) {
						system("cls"); // Очистка экрана

						switch (userInput) {
						case 1:
							std::cout << "\n\t\tSignal is save" << std::endl;
							savedSignalsCounter++;
							std::this_thread::sleep_for(std::chrono::milliseconds(1000));
							savingMenu = false;
							break;
						case 2:
							std::cout << "Signal is delete" << std::endl;
							savingMenu = false;
							std::this_thread::sleep_for(std::chrono::milliseconds(1000));
							break;
						}
					}

					if (!(chance))
						randomTelescopeBreak(telescopeObjects); // С шансом 33% ломается один телескоп
					break;

				case 2:
					isOpen = false;
					break;
				}
			}
			else {
				std::cout << "\n\t\tOne of the telescopes is not working" << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(1500));
				isOpen = false;
			}
			
		}
	}

	// Поломка рандомного телескопа
	void randomTelescopeBreak(Telescope telescopeObjects[]) {
		int i = std::rand() % 7;
		telescopeObjects[i].telescopeBreak();
	}
};


// Структура игрока
struct Player : public Position {
	// Конструктор
	Player(const int x, const int y, const char symbol) : Position(x, y, symbol) {};

	// Движение
	void move(const Direction direction, Map& map) {
		switch (direction) {
		case UP:
			// Проверка на выход за карту
			if (y) {
				// Проверка на столкновение
				if (map.field[y - 1][x] == mapSymbol) {
					this->positionClear(map);
					y--;
					this->positionCreate(map);
				}
			}
			break;
		case DOWN:
			// Проверка на выход за карту
			if (y < map.HEIGHT - 1) {
				// Проверка на столкновение
				if (map.field[y + 1][x] == mapSymbol) {
					this->positionClear(map);
					y++;
					this->positionCreate(map);
				}
			}
			break;
		case LEFT:
			// Проверка на выход за карту
			if (x) {
				// Проверка на столкновение
				if (map.field[y][x - 1] == mapSymbol) {
					this->positionClear(map);
					x--;
					this->positionCreate(map);
				}
			}
			break;
		case RIGHT:
			// Проверка на выход за карту
			if (x < map.WIDTH - 1) {
				// Проверка на столкновение
				if (map.field[y][x + 1] == mapSymbol) {
					this->positionClear(map);
					x++;
					this->positionCreate(map);
				}
			}
			break;
		}
	}
	// Дистанция до объекта
	double objectDistance(const Position object) const {
		return sqrt((pow((object.x - x), 2) + pow((object.y - y), 2)));
	}
};


int main() {
	srand(time(NULL)); // Сброс привязки ко времени

	Map map0(25, 25); // Карта
	Player player(map0.WIDTH / 2, map0.WIDTH / 2, 'i'); // Игрок
	Computer computer(map0.WIDTH / 2 - 2, map0.HEIGHT / 2 - 2, 'L'); // Компьютер

	// Объявление массива телескопов
	Telescope telescopes[TELESCOPESCOUNT]{ 
		{map0.WIDTH / 2, 1, radioTelescopeSymbol, "North"}, 
		{map0.WIDTH - 3, 2, radioTelescopeSymbol, "North-East"},
		{map0.WIDTH - 2, map0.HEIGHT / 2, radioTelescopeSymbol, "East"},
		{2, map0.HEIGHT - 3, radioTelescopeSymbol, "South-East"},
		{map0.WIDTH / 2, map0.HEIGHT - 2, radioTelescopeSymbol, "South"},
		{map0.WIDTH - 3, map0.HEIGHT - 3, radioTelescopeSymbol, "South-West"},
		{1, map0.HEIGHT / 2, radioTelescopeSymbol, "West"},
		{2, 2, radioTelescopeSymbol, "North-West"}
	};

	map0.mapInitialize(telescopes, player, computer); // Инициализация карты
	сursorInvisible(); // Скрытие курсора
	map0.mapDisplay(); // Первоначальное отображение карты
	
	bool playing = true;
	while (playing) {

		// Считывание клавиш управления
		if (GetAsyncKeyState('W') & 0x8000)
			player.move(UP, map0);
		if (GetAsyncKeyState('S') & 0x8000)
			player.move(DOWN, map0);
		if (GetAsyncKeyState('A') & 0x8000)
			player.move(LEFT, map0);
		if (GetAsyncKeyState('D') & 0x8000)
			player.move(RIGHT, map0);
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) 
			playing = false; // Завершение игры

		if (GetAsyncKeyState('E') & 0x8000) {
			// Считывание дистанции для каждого телескопа (расстояние 1)
			for (int i = 0; i < TELESCOPESCOUNT; i++) {
				// Функция подсчета расстояния до объекта по гипотенузе
				if (player.objectDistance(telescopes[i]) <= 1.5) telescopes[i].repair();
			}
			// Функция подсчета расстояния до компьютера по гипотенузе
			if (player.objectDistance(computer) <= 1.5) {
				computer.use(telescopes);
			}
		}

		map0.mapDisplay(); // Отображение дисплея
		std::this_thread::sleep_for(std::chrono::milliseconds(gameTick)); // Игровой тик
	}
	system("cls");

	return 0;
}