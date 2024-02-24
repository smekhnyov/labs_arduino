#include <LiquidCrystal_I2C.h> 
#include <EEPROM.h>

static const char NO_KEY = '\0';

class Keypad
{
private:
	byte numRows;
	byte numCols;
	char* keymap;
	byte* rowPins;
	byte* colPins;
public:
	Keypad(byte* rowPins, byte* colPins, char* custom_km, byte rows, byte cols) : numRows(rows), numCols(cols), rowPins(rowPins), colPins(colPins)
	{
		keymap = new char[numRows * numCols];
		for (byte i = 0; i < numRows * numCols; i++)
		{
			keymap[i] = custom_km[i];
		}
	}

	void begin()
	{
		for (byte row = 0; row < numRows; ++row)
		{
			pinMode(rowPins[row], INPUT_PULLUP);
		}

		for (byte col = 0; col < numCols; ++col)
		{
			pinMode(colPins[col], OUTPUT);
			digitalWrite(colPins[col], LOW);
		}
	}

	char getKey()
	{
		char key = NO_KEY;
		int numkey = -10;


		for (byte col = 0; col < numCols; ++col)
			digitalWrite(colPins[col], HIGH);

		for (byte col = 0; col < numCols; ++col)
		{
			digitalWrite(colPins[col], LOW);

			for (byte row = 0; row < numRows; ++row)
			{
				if (digitalRead(rowPins[row]) == LOW)
				{
					numkey = row * numCols + col;
					key = keymap[numkey];
					//Serial.println(numkey);
					while (digitalRead(rowPins[row]) == LOW) {};
					for (byte i = 0; i < 100; ++i);
				}
			}
			digitalWrite(colPins[col], HIGH);
		}
		for (byte col = 0; col < numCols; ++col)
			digitalWrite(colPins[col], LOW);
		return key;
	}
};

struct Data
{
	int first_number = 0;
	int second_number = 0;
	int result = 0;
};

const byte ROWS = 4;
const byte COLS = 4;
const byte rowPins[ROWS] = { 2, 3, 4, 5 };
const byte colPins[COLS] = { 9, 8, 7, 6 };
const byte interruptPin = rowPins[0];
volatile bool Interrupt = false;
volatile char Pressed = NO_KEY;
volatile char key = NO_KEY;
int number_menu = 1;
int value = 0;
int mem_value = -2;
Data data;

char keys[ROWS * COLS] = {
  '1', '2', '3', 'A',
  '4', '5', '6', 'B',
  '7', '8', '9', 'C',
  '*', '0', '#', 'D'
};


LiquidCrystal_I2C display(0x27, 16, 2);
Keypad pad(rowPins, colPins, keys, ROWS, COLS);

void InitEPROM()
{
	EEPROM.get(0, mem_value);
}

void UpdateEPROM()
{
	EEPROM.put(0, data.result);
	mem_value = data.result;
	data.result = 0;
}

void DeleteEPROM()
{
	EEPROM.put(0, 0);
}

int inputNumber()
{
	key = NO_KEY;
	int result;
	value = 0;
	bool flag = true;
	while (flag)
	{
		noInterrupts();
		key = pad.getKey();
		interrupts();
		switch (key)
		{
		case '0'...'9':
			value = value * 10 + (key - '0');
			display.setCursor(0, 1);
			display.print(value);
			break;
		case 'A':
			result = value;
			flag = false;
			break;
		case 'B':
			result = 0;
			flag = false;
			break;
		case 'C':
        value = mem_value;
        display.setCursor(0, 1);
        display.print(value);
        break;
		}
	}
	return result;
}

void YesOrNo(void (*RunEPROM)())
{
	bool flag = true;
	while (flag)
	{
		noInterrupts();
		key = pad.getKey();
		interrupts();
		switch (key)
		{
		case 'A':
			RunEPROM();
		case 'B':
			flag = false;
		}
	}
}

void Menu1()
{
	display.clear();
	display.setCursor(0, 0);
	display.print("First number:");
	display.setCursor(0, 1);

	data.first_number = inputNumber();
	Serial.println(data.first_number);


	display.clear();
	display.setCursor(0, 0);
	display.print("Second number:");
	display.setCursor(0, 1);

	data.second_number = inputNumber();
	Serial.println(data.second_number);

	display.clear();
	display.setCursor(0, 0);
	display.print("Result:");

	data.result = data.first_number | data.second_number;
	display.print(data.result);

	display.setCursor(0, 1);
	display.print("Save result? A/B");

	data.first_number = 0;
	data.second_number = 0;

	YesOrNo(&UpdateEPROM);
}

void Menu3()
{
	display.setCursor(0, 0);
	display.print("Reset?");
	display.setCursor(0, 1);
	display.print("A-Yes, B-No");

	YesOrNo(&DeleteEPROM);
}

void InitEnter(int number_menu)
{
	switch (number_menu)
	{
	case 1:
		Menu1();
		break;
	case 3:
		Menu3();
		break;
	}
	MenuCalculate();
}

void MenuCalculate()
{
	display.clear();
	display.setCursor(0, 0);
	display.print("Menu Calculate:");
	display.setCursor(0, 1);
	display.print("A - enter");
}

void MenuMemory()
{
	display.clear();
	display.setCursor(0, 0);
	display.print("Menu Memory:");
	display.setCursor(0, 1);
	display.print("Save: " + String(mem_value));
}

void MenuReset()
{
	display.clear();
	display.setCursor(0, 0);
	display.print("Menu Reset:");
	display.setCursor(0, 1);
	display.print("A - enter");
}

void Menu()
{
	if (Interrupt)
	{
		switch (key)
		{
		case '1': // промотка меню влево
			if (number_menu == 1)
			{
				number_menu = 3;
			}
			else
			{
				number_menu--;
			}
			break;
		case '3': // промотка меню вправо
			if (number_menu == 3)
			{
				number_menu = 1;
			}
			else
			{
				number_menu++;
			}
			break;
		case 'A':
			InitEnter(number_menu);
			number_menu = 1;
			break;
		}
		if (number_menu != -1)
		{
			Serial.println(number_menu);
		}
		Interrupt = false;
		display.clear();
		switch (number_menu)
		{
		case 1:
			MenuCalculate();
			break;
		case 2:
			MenuMemory();
			break;
		case 3:
			MenuReset();
			break;
		}
	}
}

void handler()
{
	if (!Interrupt)
	{
		Interrupt = true;
		key = pad.getKey();
	}
	Serial.println("Interrupted");
}

void setup()
{
	Serial.begin(9600);
	pad.begin();
	display.init();
	display.backlight();
	InitEPROM();
	MenuCalculate();

	attachInterrupt(digitalPinToInterrupt(interruptPin), handler, FALLING);
}

void loop()
{
	Menu();
}
