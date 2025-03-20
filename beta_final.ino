 #include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

#define CASH_AMOUNT_ADDR 0
#define ONLINE_AMOUNT_ADDR sizeof(int)

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'm'},
  {'4', '5', '6', '+'},
  {'7', '8', '9', '-'},
  {'*', '0', '/', 'e'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int cashAmount;
int onlineAmount;
String buffer = "";
char operators[] = {'e', 'm'};

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.begin();
  EEPROM.get(CASH_AMOUNT_ADDR, cashAmount);
  EEPROM.get(ONLINE_AMOUNT_ADDR, onlineAmount);
  lcd.print("--SMART_CREDIT--");
  lcd.setCursor(0, 1);
  lcd.print("---CALCULATOR---");
  
  
}

void loop() {
  if (Serial.available()) {
    char a = Serial.read();
    if (a == 'x') {
      EEPROM.get(CASH_AMOUNT_ADDR, cashAmount);
      EEPROM.get(ONLINE_AMOUNT_ADDR, onlineAmount);
      Serial.print("Cash: ");
      Serial.print(cashAmount);
      Serial.print(" Rs, Online: ");
      Serial.print(onlineAmount);
      Serial.println(" Rs");
      lcd.clear();
      lcd.print("Data Transmitted");
      
      
    }
  }

  char key = keypad.getKey();
  if (key) {  // Process key only when pressed
    
    bool isDelimiter = false;
    for (int i = 0; i < sizeof(operators); i++) {
      if (key == operators[i]) {
        isDelimiter = true;
        break;
      }
    }
    if (isDelimiter) {
      processInput(key);
    } else {
      buffer += key;
      if(buffer=="++--"){
        EEPROM.put(CASH_AMOUNT_ADDR, 0);
        EEPROM.put(ONLINE_AMOUNT_ADDR, 0);
        lcd.clear();
        lcd.print("Amounts RESET");
        buffer="";
      }
      else{
      lcd.clear();
      lcd.print(buffer);}
    }
  }
}

void processInput(char input) {
  if (input == 'e') {
    evaluateExpression();
  } else if (input == 'm') {
    char lastChar = buffer.charAt(buffer.length() - 1);
    buffer.remove(buffer.length() - 1);

    if (lastChar == '+') {
      addToCash();
    } else if (lastChar == '*') {
      addToOnline();
    } else if (lastChar == '-') {
      reduceFromCash();
    } else if (lastChar == '/') {
      printAmount();
    } else {
      lcd.clear();
      lcd.print("Syntax Error!");
      buffer="";
    }
  } else {
    lcd.clear();
    lcd.print("Syntax Error!");
  }
}

void evaluateExpression() {
  int result = parseAndEvaluateExpression(buffer);
  lcd.clear();
  lcd.print(buffer);
  lcd.print("=");
  lcd.print(result);
  buffer = "";
}

int parseAndEvaluateExpression(String expression) {
  int operand1 = 0;
  int operand2 = 0;
  char op;
  sscanf(expression.c_str(), "%d%c%d", &operand1, &op, &operand2);
  switch (op) {
    case '+': return operand1 + operand2;
    case '-': return operand1 - operand2;
    case '*': return operand1 * operand2;
    case '/': return operand2 != 0 ? operand1 / operand2 : 0;
    default: return 0;
  }
}

void addToCash() {
  int amount = buffer.toInt();
  cashAmount += amount;
  EEPROM.put(CASH_AMOUNT_ADDR, cashAmount);
  lcd.clear();
  lcd.print("Cash Added!");
  lcd.setCursor(0, 1);
  lcd.print(amount);
  lcd.print(" Rs.");
  buffer = "";
  delay(1000);
  lcd.clear();
}

void addToOnline() {
  int amount = buffer.toInt();
  onlineAmount += amount;
  EEPROM.put(ONLINE_AMOUNT_ADDR, onlineAmount);
  lcd.clear();
  lcd.print("Online Added!");
  lcd.setCursor(0, 1);
  lcd.print(amount);
  lcd.print(" Rs.");
  buffer = "";
  delay(1000);
  lcd.clear();
}

void reduceFromCash() {
  int amount = buffer.toInt();
  cashAmount -= amount;
  EEPROM.put(CASH_AMOUNT_ADDR, cashAmount);
  lcd.clear();
  lcd.print("Cash Reduced!");
  lcd.setCursor(0, 1);
  lcd.print(amount);
  lcd.print(" Rs.");
  buffer = "";
  delay(1000);
  lcd.clear();
}

void printAmount() {
  EEPROM.get(CASH_AMOUNT_ADDR, cashAmount);
  EEPROM.get(ONLINE_AMOUNT_ADDR, onlineAmount);
  lcd.clear();
  lcd.print("Cash=");
  lcd.print(cashAmount);
  lcd.print(" Rs.");
  lcd.setCursor(0, 1);  
  lcd.print("Online=");
  lcd.print(onlineAmount);
  lcd.print(" Rs.");
  buffer = "";
   
}
