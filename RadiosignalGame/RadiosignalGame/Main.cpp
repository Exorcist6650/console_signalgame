#include <iostream>
#include <cmath>
#include <string>
#include <thread>
#include <chrono>
#include <limits>
#include <time.h>
#include <Windows.h>
#include <memory>

int gameTick = 150;

const int TELESCOPESCOUNT = 8; // Telescopes quantity
const char mapSymbol = '`'; 
const char radioTelescopeSymbol = '%';

const int HEIGHT = 25; // Map Y size
const int WIDTH = 25; // Map X size

// Direction
enum Direction{UP, DOWN, LEFT, RIGHT};

// Console cursor invisible 
void сursorInvisible() {

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	if (console == INVALID_HANDLE_VALUE) return;

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(console, &cursorInfo); 
	cursorInfo.bVisible = false;

	SetConsoleCursorInfo(console, &cursorInfo); 
}

// std::cin clear
void clearConsoleInput() {
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	FlushConsoleInputBuffer(hStdin);
}

struct Player; 
struct Map; 

// Base Position class
class Position {
	friend Player;
public:
	Position(const int x, const int y, const char symbol) {
		this->x = x;
		this->y = y;
		this->symbol = symbol;
	}

	void positionCreate(Map& map); 

	void positionClear(Map& map);

protected:
	int x;
	int y;
	char symbol;
};


// Telescope class (Position inheritance)
class Telescope : public Position {
public:
	Telescope(const int x, const int y, const char symbol, std::string name) : Position(x, y, symbol) {
		workStatus = true;
		this->name = name;
	};
	
	// Open telescope menu
	void repair() {
		bool isOpen = true;

		while (isOpen) {
			if (!workStatus) {
				system("cls"); // Console clear
				// Example generation
				int userInput;
				int a = std::rand() % 9;
				int b = std::rand() % 9;
				int result = a + b;

				// Example display
				std::cout << "\n\n\n\n\n\n\n\n\n\n\n\t\t" << a << " + " << b << " = ";
				clearConsoleInput();

				std::cin >> userInput; 

				// Answer correct
				if (userInput == result) {
					workStatus = true;
					std::cout << " Congratulations! Telescope " << name << " is repair!" << std::endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(1500));
					isOpen = false;
				}
				// Answer incorrect
				else {
					std::cout << "\t\tNot correct" << std::endl;
					isOpen = false;
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				}
			}
			// Telescope is already working 
			else {
				system("cls");
				std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\t" << name << " telescope is already working" << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(1500));
				isOpen = false;
			}
		}
	}

	// Change work status
	void telescopeBreak() {
		workStatus = false;
	}

	bool GetWorkStatus() const { return workStatus; } 

private:
	std::string name;
	bool workStatus; 
};


struct Map {
	std::shared_ptr<char[][WIDTH]> field; // Map

	Map() : field(new char[HEIGHT][WIDTH]) {} // Allocate map memory

	// Map initilization
	void mapInitialize(Telescope telescopes[], Position obj1, Position obj2) {
		
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				field[i][j] = mapSymbol;
			}
		}
		// Telescopes map creating
		for (int i = 0; i < TELESCOPESCOUNT; i++) {
			telescopes[i].positionCreate(*this);
		}
		// Objects map creating
		obj1.positionCreate(*this);
		obj2.positionCreate(*this);
	}

	// Game display
	void mapDisplay() const {
		system("cls"); // Console clear
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				std::cout << field[i][j] << ' ';
			}
			std::cout << std::endl;
		}
	}
	
};

// Position methods
void Position::positionCreate(Map& map) {
	map.field[y][x] = symbol;
}
void Position::positionClear(Map& map)
{
	map.field[y][x] = mapSymbol;
}


// Computer struct (Position inheritance)
struct Computer : public Position {
	
	Computer(const int x, const int y, const char symbol) : Position(x, y, symbol) {};

	// Computer menu open
	void use(Telescope telescopeObjects[]) {
		bool isOpen = true; 
		bool savingMenu = true; 
		static int savedSignalsCounter = 0;

		while (isOpen) {
			int userInput; 
			int searchTime = 3 + std::rand() % 8; // Signal search random time 
			int chance = std::rand() % 3; // Telescope break chance
		
			bool isAllTelescopesOperational = true; // All telescopes is operational

			clearConsoleInput(); 
	
			system("cls"); // Console clear

			// Examination of working all the telescopes
			for (int i = 0; i < TELESCOPESCOUNT; i++) {
				if (telescopeObjects[i].GetWorkStatus() == false) isAllTelescopesOperational = false;
			}
			// All is operational
			if (isAllTelescopesOperational) {
				std::cout << "\t\tComputer menu\n\nSaved signals - " << savedSignalsCounter << "\n1.Find signal\n2.Exit" << std::endl; // Первый слой интерфейса

				std::cin >> userInput;

				switch (userInput) {
				case 1:
					for (int counter = 0; counter < searchTime; counter++) { // Saving process
						system("cls"); // Сonsole clear
						std::cout << "\n\t\tSearching " << counter << " seconds..." << std::endl;
						std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					}
					system("cls"); // Сonsole clear
					std::cout << "\t\tSignal is find!\n\n1. Save the signal\n2. Delete the signal" << std::endl; // Saving menu


					std::cin >> userInput; 

					while (savingMenu) {
						system("cls"); // Сonsole clear

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
						randomTelescopeBreak(telescopeObjects); // 33% telescope breaking chance
					break;

				case 2:
					isOpen = false;
					break;
				}
			}
			// One of the telescopes is not operational
			else {
				std::cout << "\n\t\tOne of the telescopes is not working" << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(1500));
				isOpen = false;
			}
			
		}
	}
	void randomTelescopeBreak(Telescope telescopeObjects[]) {
		int i = std::rand() % 8;
		telescopeObjects[i].telescopeBreak();
	}
};


// Player struct (inharitance)
struct Player : public Position {
	
	Player(const int x, const int y, const char symbol) : Position(x, y, symbol) {};

	// Player moving
	void move(const Direction direction, Map& map) {
		switch (direction) {
		case UP:
			// Map boundary check
			if (y) {
				// Collizion check
				if (map.field[y - 1][x] == mapSymbol) {
					this->positionClear(map);
					y--;
					this->positionCreate(map);
				}
			}
			break;
		case DOWN:
			// Map boundary check
			if (y < HEIGHT - 1) {
				// Collizion check
				if (map.field[y + 1][x] == mapSymbol) {
					this->positionClear(map);
					y++;
					this->positionCreate(map);
				}
			}
			break;
		case LEFT:
			// Map boundary check
			if (x) {
				// Collizion check
				if (map.field[y][x - 1] == mapSymbol) {
					this->positionClear(map);
					x--;
					this->positionCreate(map);
				}
			}
			break;
		case RIGHT:
			// Map boundary check
			if (x < WIDTH - 1) {
				// Collizion check
				if (map.field[y][x + 1] == mapSymbol) {
					this->positionClear(map);
					x++;
					this->positionCreate(map);
				}
			}
			break;
		}
	}
	// Distance to object
	double objectDistance(const Position object) const {
		return sqrt((pow((object.x - x), 2) + pow((object.y - y), 2)));
	}
};


int main() {
	srand(time(NULL));

	Map map0; // Map
	Player player(WIDTH / 2, WIDTH / 2, 'i'); // Player
	Computer computer(WIDTH / 2 - 2, HEIGHT / 2 - 2, 'L'); // Computer

	// Объявление массива телескопов
	Telescope telescopes[TELESCOPESCOUNT]{ 
		{WIDTH / 2, 1, radioTelescopeSymbol, "North"}, 
		{WIDTH - 3, 2, radioTelescopeSymbol, "North-East"},
		{WIDTH - 2, HEIGHT / 2, radioTelescopeSymbol, "East"},
		{2, HEIGHT - 3, radioTelescopeSymbol, "South-East"},
		{WIDTH / 2, HEIGHT - 2, radioTelescopeSymbol, "South"},
		{WIDTH - 3, HEIGHT - 3, radioTelescopeSymbol, "South-West"},
		{1, HEIGHT / 2, radioTelescopeSymbol, "West"},
		{2, 2, radioTelescopeSymbol, "North-West"}
	};

	map0.mapInitialize(telescopes, player, computer); // Инициализация карты
	сursorInvisible(); // Скрытие курсора
	map0.mapDisplay(); // Первоначальное отображение карты
	
	bool playing = true;
	while (playing) {

		// Game control keys
		if (GetAsyncKeyState('W') & 0x8000)
			player.move(UP, map0);
		if (GetAsyncKeyState('S') & 0x8000)
			player.move(DOWN, map0);
		if (GetAsyncKeyState('A') & 0x8000)
			player.move(LEFT, map0);
		if (GetAsyncKeyState('D') & 0x8000)
			player.move(RIGHT, map0);
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) 
			playing = false; // Quit the game

		if (GetAsyncKeyState('E') & 0x8000) {
			// Telescope distance checking
			for (int i = 0; i < TELESCOPESCOUNT; i++) {
				if (player.objectDistance(telescopes[i]) <= 1.5) telescopes[i].repair();
			}
			// Computer distance checking
			if (player.objectDistance(computer) <= 1.5) {
				computer.use(telescopes);
			}
		}

		map0.mapDisplay(); // Game display
		std::this_thread::sleep_for(std::chrono::milliseconds(gameTick)); // Game tick
	}
	system("cls"); // Console clear;

	return 0;
}
