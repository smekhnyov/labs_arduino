#include <LiquidCrystal_I2C.h> //Дисплей - пины GND, 5V, SDA(20) и SCL(21)
#include <Keypad.h> //Клавиатура - пины строки 13-10, столбцы 7-4
#include <EEPROM.h>

// дисплей
#define I2C_ADDR    0x27 // для I2C
#define LCD_COLUMNS 16 // количество столбцов
#define LCD_LINES   2 // количество строк

// клавиатура
const byte ROWS = 4; // строки
const byte COLS = 4; // столбцы
const byte rowPins[ROWS] = {13, 12, 11, 10}; // распиновка строк
const byte colPins[COLS] = {7, 6, 5, 4}; // распиновка столбцов
 
char keys[ROWS][COLS] = { // устанавливаем значения кнопок
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
 
LiquidCrystal_I2C lcd_display(I2C_ADDR, LCD_COLUMNS, LCD_LINES); // init дисплей
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); // init клавиатуру

struct Data // структура для хранения вычислений, чтобы удобнее было записывать в память
{
  int first_number = 0;
  int second_number = 0;
  int result = 0;
};
 
void setup()
{
  Serial.begin(9600); // на всякий случай
  lcd_display.init();
  lcd_display.backlight();
  lcd_display.print("1) Calculator"); // изначально же должен гореть 1 пункт, правильно?
}
 
int menu_change = 0; // 0 - 1) Calculator, 1 - 2) Memory, 2 - 3) Reset

void changeMenu(char key)
{
    switch (key)
    {
      case '*': // промотка меню влево
        if (menu_change == 0)
        {
          menu_change = 2;
        }
        else
        {
          menu_change--;
        }
      break;
      case '#': // промотка меню вправо
        if (menu_change == 2)
        {
          menu_change = 0;
        }
        else
        {
          menu_change++;
        }
      break;
      case 'A': // подтверждение выбранного пункта меню
        confirmMenu();
      break;
    }
    mainMenu();
}

void confirmMenu() // если нажали подтверждение на пункте меню
{
  switch (menu_change) // запустится та функция, на пункте меню которой мы находимся
  {
    case 0:
    Calculate();
    break;
    case 2:
    Reset();
    break;
  }
}

Data calculator_data; // структура для результата вычислений
Data memory_data; // структура для считывания из памяти
Data default_data; // дефолтная структура для запуска в первый раз и сброса

void Calculate() // тут и так все ясно
{
  lcd_display.clear();
  lcd_display.print("First number:");
  lcd_display.setCursor(0, 1);
  calculator_data.first_number = inputNumber();
  if (calculator_data.first_number == 0) // если отказались от ввода хотя бы одного числа в inputNumber
  {
    return;
  }
  lcd_display.clear();
  lcd_display.print("Second number:");
  lcd_display.setCursor(0, 1);
  calculator_data.second_number = inputNumber();
  if (calculator_data.second_number == 0)
  {
    return;
  }
  calculator_data.result = calculator_data.first_number | calculator_data.second_number; // | - OR
  lcd_display.clear();
  lcd_display.print("Result:");
  lcd_display.print(calculator_data.result);
  lcd_display.setCursor(0, 1);
  lcd_display.print("Save result?");
  saveResult();
}

void saveResult()
{
  bool is_input = true;
  while(is_input)
  {
    char key = keypad.getKey();
    if (key != NO_KEY)
    {
      switch (key)
      {
        case 'A': // подтверждение сохранения
        is_input = false;
        EEPROM.put(0, calculator_data);
        break;
        case 'B': // отмена сохранения
        is_input = false;
        return;
        break;
      }
    }
  }
}

int inputNumber()
{
  bool is_input = true;
  int input = 0;
  while(is_input)
  {
    char key = keypad.getKey();
    if (key != NO_KEY)
    {
      switch (key)
      {
        case '1': // говнокод - есть говнокод
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '0':
        lcd_display.print(key); // пишем на экран цифру, а в число input добавляем по разряду
        if (input == 0)
        {
          input += key - '0';
        }
        else
        {
          input = input*10 + (key - '0');
        }
        break;
        case 'A': // подтверждение ввода числа
        is_input = false;
        return input;
        break;
        case 'B': // отмена ввода числа
        is_input = false;
        return 0;
        break;
      }
    }
  }
}

void Reset()
{
  bool is_input = true;
  lcd_display.clear();
  lcd_display.print("Are you sure?");
  while(is_input)
  {
    char key = keypad.getKey();
    if (key != NO_KEY)
    {
      switch (key)
      {
        case 'A': // подтверждение сброса
        is_input = false;
        EEPROM.put(0, default_data); // при сбросе запишется дефолтная структура 0 0 0
        break;
        case 'B': // отмена сброса
        is_input = false;
        return;
        break;
      }
    }
  }
}

void initialReset() // сброс при запуске
{
  EEPROM.get(0, memory_data); // читаем данные из ячеек, смотрим че там, если везде -1 - пишем дефолт
  if (memory_data.first_number == -1 && memory_data.second_number == -1 && memory_data.result == -1)
  {
    EEPROM.put(0, default_data);
  }
}

void mainMenu() // тоже все понятно
{
  lcd_display.clear();
  switch (menu_change)
  {
    case 0:
    lcd_display.print("1) Calculator");
    break;
    case 1:
    lcd_display.print("2) Memory");
    lcd_display.setCursor(0, 1);
    initialReset();
    EEPROM.get(0, memory_data);
    lcd_display.print(memory_data.first_number);
    lcd_display.print("OR");
    lcd_display.print(memory_data.second_number);
    lcd_display.print('=');
    lcd_display.print(memory_data.result);
    break;
    case 2:
    lcd_display.print("3) Reset");
    break;
  }
}

void loop() // ну тут вообще максимально понятно
{
  char key = keypad.getKey();
  if (key != NO_KEY)
  {
    changeMenu(key);
  }
}
