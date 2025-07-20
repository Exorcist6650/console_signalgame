#include <iostream>
#include <cmath>
#include <string>
#include <thread>
#include <chrono>
#include <limits>
#include <time.h>
#include <Windows.h>

int gameTick = 150;

const int TELESCOPESCOUNT = 8; // ���������� ����������
const char mapSymbol = '`'; // C������� �����
const char radioTelescopeSymbol = '%';

// �����������
enum Direction{UP, DOWN, LEFT, RIGHT};

// ������� �������
void �ursorInvisible() {

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	if (console == INVALID_HANDLE_VALUE) return; // �������� �� �������������� �������

	CONSOLE_CURSOR_INFO cursorInfo; // ������ �������

	GetConsoleCursorInfo(console, &cursorInfo); // ���� ������
	cursorInfo.bVisible = false; // C������ �������

	SetConsoleCursorInfo(console, &cursorInfo); // ������ ������
}

// �������� std::cin ��� ������ �����
void clearConsoleInput() {
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	FlushConsoleInputBuffer(hStdin);
}

struct Player; // �������� ��������� ������
struct Map; // �������� ��������� �������

// ������� ����� �������
class Position {
	friend Player;
public:
	// �����������
	Position(const int x, const int y, const char symbol) {
		this->x = x;
		this->y = y;
		this->symbol = symbol;
	}

	// �������� ������� �� �������
	void positionCreate(Map& map);

	// �������� ������� �� �������
	void positionClear(Map& map);

protected:
	int x;
	int y;
	char symbol;
};


// ����� ���������
class Telescope : public Position {
public:
	// �����������
	Telescope(const int x, const int y, const char symbol, std::string name) : Position(x, y, symbol) {
		workStatus = true;
		this->name = name;
	};

	// ������� ���������
	void repair() {
		bool isOpen = true;

		while (isOpen) {
			if (!workStatus) {
				system("cls"); // ������� ������
				// ��������� �������
				int userInput;
				int a = std::rand() % 9;
				int b = std::rand() % 9;
				int result = a + b;

				// ����� �������
				std::cout << "\n\n\n\n\n\n\n\n\n\n\n\t\t" << a << " + " << b << " = ";
				clearConsoleInput(); // ������� std::cin �� ������� ������

				std::cin >> userInput; // ���������� ����� ������������

				// ���������� �����
				if (userInput == result) {
					workStatus = true;
					std::cout << " Congratulations! Telescope " << name << " is repair!" << std::endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(1500));
					isOpen = false;
				}
				// ������������ �����
				else {
					std::cout << "\t\tNot correct" << std::endl;
					isOpen = false;
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				}
			}
			// �������� ��� ��������
			else {
				system("cls");
				std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\t" << name << " telescope is already working" << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(1500));
				isOpen = false;
			}
		}
	}

	// ��������� �� �����
	void telescopeBreak() {
		workStatus = false;
	}

	bool GetWorkStatus() const { return workStatus; } // ������ �������

private:
	std::string name;
	bool workStatus; 
};


// C�������� �����
struct Map {
public:

	// �����������
	Map(const int HEIGHT, const int WIDTH) {
		this->HEIGHT = HEIGHT;
		this->WIDTH = WIDTH;

		field = new char*[HEIGHT]; // ��������� ������ ��� ������� ����������
		for (int i = 0; i < HEIGHT; i++) {
			field[i] = new char[WIDTH]; // ��������� ������ ��� ������� ���������
		}
	}
	// ����������
	~Map() {
		for (int i = 0; i < HEIGHT; i++) {
			delete [] field[i]; // �������� ������ ��� ������� ���������
		}
		delete[] field; // �������� ������ ��� ������� ����������
	}


	// ������������� �����
	void mapInitialize(Telescope telescopes[], Position obj1, Position obj2) {
		// ���������� ����� ���������
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				field[i][j] = mapSymbol;
			}
		}
		// ��������� ����������
		for (int i = 0; i < TELESCOPESCOUNT; i++) {
			telescopes[i].positionCreate(*this);
		}
		// ��������� ��������� ��������
		obj1.positionCreate(*this);
		obj2.positionCreate(*this);
	}

	// ����������� ����
	void mapDisplay() const {
		system("cls"); // ������� �������
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				std::cout << field[i][j] << ' ';
			}
			std::cout << std::endl;
		}
	}
	char** field; // ������ ����������
	int HEIGHT; // ����� 
	int WIDTH; // ������
};

// ��������� ������� ������ �� ��� �������
void Position::positionCreate(Map& map) {
	map.field[y][x] = symbol;
}
void Position::positionClear(Map& map)
{
	map.field[y][x] = mapSymbol;
}


// ��������� ����������
struct Computer : public Position {
	//�����������
	Computer(const int x, const int y, const char symbol) : Position(x, y, symbol) {};

	// ������������� ����������
	void use(Telescope telescopeObjects[]) {
		bool isOpen = true; // ������� ����
		bool savingMenu = true; // ��������� ����
		static int savedSignalsCounter = 0;

		while (isOpen) {
			int userInput; // ���������������� ����
			int searchTime = 3 + std::rand() % 8; // ��������� ����� ������ �������
			int chance = std::rand() % 3; // ���� ������� ���������
		
			bool isAllTelescopesOperational = true; // ���������������� ����������

			clearConsoleInput(); // ������� std::cin �� ������� ������
	
			system("cls"); // ������� ������

			// �������� ���� ���������� �� ����������������
			for (int i = 0; i < TELESCOPESCOUNT; i++) {
				if (telescopeObjects[i].GetWorkStatus() == false) isAllTelescopesOperational = false;
			}
			
			if (isAllTelescopesOperational) {
				std::cout << "\t\tComputer menu\n\nSaved signals - " << savedSignalsCounter << "\n1.Find signal\n2.Exit" << std::endl; // ������ ���� ����������

				std::cin >> userInput; // ���������� �����

				switch (userInput) {
				case 1:
					for (int counter = 0; counter < searchTime; counter++) { // �������� (������ ���� ����������)
						system("cls"); // ������� ������
						std::cout << "\n\t\tSearching " << counter << " seconds..." << std::endl;
						std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					}
					system("cls"); // ������� ������
					std::cout << "\t\tSignal is find!\n\n1. Save the signal\n2. Delete the signal" << std::endl; // ������ ���� ����������


					std::cin >> userInput; // ���������� �����

					while (savingMenu) {
						system("cls"); // ������� ������

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
						randomTelescopeBreak(telescopeObjects); // � ������ 33% �������� ���� ��������
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

	// ������� ���������� ���������
	void randomTelescopeBreak(Telescope telescopeObjects[]) {
		int i = std::rand() % 7;
		telescopeObjects[i].telescopeBreak();
	}
};


// ��������� ������
struct Player : public Position {
	// �����������
	Player(const int x, const int y, const char symbol) : Position(x, y, symbol) {};

	// ��������
	void move(const Direction direction, Map& map) {
		switch (direction) {
		case UP:
			// �������� �� ����� �� �����
			if (y) {
				// �������� �� ������������
				if (map.field[y - 1][x] == mapSymbol) {
					this->positionClear(map);
					y--;
					this->positionCreate(map);
				}
			}
			break;
		case DOWN:
			// �������� �� ����� �� �����
			if (y < map.HEIGHT - 1) {
				// �������� �� ������������
				if (map.field[y + 1][x] == mapSymbol) {
					this->positionClear(map);
					y++;
					this->positionCreate(map);
				}
			}
			break;
		case LEFT:
			// �������� �� ����� �� �����
			if (x) {
				// �������� �� ������������
				if (map.field[y][x - 1] == mapSymbol) {
					this->positionClear(map);
					x--;
					this->positionCreate(map);
				}
			}
			break;
		case RIGHT:
			// �������� �� ����� �� �����
			if (x < map.WIDTH - 1) {
				// �������� �� ������������
				if (map.field[y][x + 1] == mapSymbol) {
					this->positionClear(map);
					x++;
					this->positionCreate(map);
				}
			}
			break;
		}
	}
	// ��������� �� �������
	double objectDistance(const Position object) const {
		return sqrt((pow((object.x - x), 2) + pow((object.y - y), 2)));
	}
};


int main() {
	srand(time(NULL)); // ����� �������� �� �������

	Map map0(25, 25); // �����
	Player player(map0.WIDTH / 2, map0.WIDTH / 2, 'i'); // �����
	Computer computer(map0.WIDTH / 2 - 2, map0.HEIGHT / 2 - 2, 'L'); // ���������

	// ���������� ������� ����������
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

	map0.mapInitialize(telescopes, player, computer); // ������������� �����
	�ursorInvisible(); // ������� �������
	map0.mapDisplay(); // �������������� ����������� �����
	
	bool playing = true;
	while (playing) {

		// ���������� ������ ����������
		if (GetAsyncKeyState('W') & 0x8000)
			player.move(UP, map0);
		if (GetAsyncKeyState('S') & 0x8000)
			player.move(DOWN, map0);
		if (GetAsyncKeyState('A') & 0x8000)
			player.move(LEFT, map0);
		if (GetAsyncKeyState('D') & 0x8000)
			player.move(RIGHT, map0);
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) 
			playing = false; // ���������� ����

		if (GetAsyncKeyState('E') & 0x8000) {
			// ���������� ��������� ��� ������� ��������� (���������� 1)
			for (int i = 0; i < TELESCOPESCOUNT; i++) {
				// ������� �������� ���������� �� ������� �� ����������
				if (player.objectDistance(telescopes[i]) <= 1.5) telescopes[i].repair();
			}
			// ������� �������� ���������� �� ���������� �� ����������
			if (player.objectDistance(computer) <= 1.5) {
				computer.use(telescopes);
			}
		}

		map0.mapDisplay(); // ����������� �������
		std::this_thread::sleep_for(std::chrono::milliseconds(gameTick)); // ������� ���
	}
	system("cls");

	return 0;
}