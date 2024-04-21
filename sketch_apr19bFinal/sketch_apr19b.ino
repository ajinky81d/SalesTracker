#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>

#define CASH_AMOUNT_ADDR 0
#define ONLINE_AMOUNT_ADDR sizeof(int)

 



int cashAmount;
int onlineAmount;
String buffer = "";
char operators[] = {'e', 'c', 'o', 'm', 'n', 'p', 'q'}; // Specify the delimiter character

LiquidCrystal_I2C lcd(0x27,16,2); // Adjust the pins according to your setup
 
void setup() {
  Serial.begin(9600);
  lcd.begin(); // Initialize the LCD with 16 columns and 2 rows
  EEPROM.get(CASH_AMOUNT_ADDR, cashAmount);
  EEPROM.get(ONLINE_AMOUNT_ADDR, onlineAmount);
    lcd.print("--SMART_CREDIT--");
   lcd.setCursor(0,1);
   lcd.print("---CALCULATOR---");
}
 
void loop() {
  
  while (Serial.available()) {
    char a = Serial.read();
    // Check if the received character is a delimiter
    bool isDelimiter = false;
    for (int i = 0; i < sizeof(operators); i++) {
      if (a == operators[i]) {
        isDelimiter = true;
        break;
      }
    }
    if (isDelimiter) {
      processInput(a);
      // Clear the buffer after processing
    } else {
      lcd.clear();
      buffer += a;
      lcd.print(buffer);
    }
  }
}

void processInput(char input) {
  if (input == 'e') {
    evaluateExpression();
  } else if (input == 'c') {
    addToCash();
  } else if (input == 'o') {
    addToOnline();
  } else if (input == 'm') {
    reduceFromCash();
  } else if (input == 'n') {
    reduceFromOnline();
  } else if (input == 'p') {
    printCashAmount();
  } else if (input == 'q') {
    printOnlineAmount();
  } else {
    lcd.clear(); // Clear the LCD display
    lcd.println("Syntax Error!");
  }
}

 
void evaluateExpression() {
  // Parse and evaluate the expression
  int result = parseAndEvaluateExpression(buffer);
  
  lcd.clear(); // Clear the LCD display
  lcd.print(buffer);
  lcd.print("=");
  lcd.print(result);
  buffer="";
}

int parseAndEvaluateExpression(String expression) {
  // Implement a simple expression parser and evaluator here
  // For simplicity, let's assume the expression consists of only two operands and one operator
  int operand1 = 0;
  int operand2 = 0;
  char op;
  sscanf(expression.c_str(), "%d%c%d", &operand1, &op, &operand2);
  
  switch(op) {
    case '+':
      return operand1 + operand2;
    case '-':
      return operand1 - operand2;
    case '*':
      return operand1 * operand2;
    case '/':
      return operand1 / operand2;
    default:
      return 0; // Invalid operator
  }
}
int evaluate(String expression) {
  return atoi(expression.c_str()); // Convert ASCII characters to integer
}

void addToCash() {
  int amount = atoi(buffer.c_str());
  
  cashAmount += amount;
  EEPROM.put(CASH_AMOUNT_ADDR, cashAmount);
  lcd.clear(); // Clear the LCD display
  lcd.print("Cash Added!");
  lcd.setCursor(0, 1); 
  lcd.print(amount);
  lcd.print(" Rs.");
  buffer="";
}

void addToOnline() {
  int amount = atoi(buffer.c_str());
  onlineAmount += amount;
  EEPROM.put(ONLINE_AMOUNT_ADDR, onlineAmount);
  lcd.clear(); // Clear the LCD display
  lcd.print("Online Added!");
  lcd.setCursor(0, 1); 
  lcd.print(amount);
  lcd.print(" Rs.");
  buffer="";
}

void reduceFromCash() {
  int amount = atoi(buffer.c_str());
  cashAmount -= amount;
  EEPROM.put(CASH_AMOUNT_ADDR, cashAmount);
  lcd.clear(); // Clear the LCD display
  lcd.print("Cash Reduced!");
  lcd.setCursor(0, 1); 
  lcd.print(amount);
  lcd.print(" Rs.");
  buffer="";
}

void reduceFromOnline() {
  int amount = atoi(buffer.c_str());
  onlineAmount -= amount;
  EEPROM.put(ONLINE_AMOUNT_ADDR, onlineAmount);
  lcd.clear(); // Clear the LCD display
  lcd.print("Online Reduced!");
   lcd.setCursor(0, 1); 
  lcd.print(amount);
  lcd.print(" Rs.");
  buffer="";
}

 void printCashAmount() {
  int cashAmountFromEEPROM;
  EEPROM.get(CASH_AMOUNT_ADDR, cashAmountFromEEPROM); // Read cash amount from EEPROM
  lcd.clear(); // Clear the LCD display
  lcd.print("Cash= ");
  lcd.print(cashAmountFromEEPROM);
  lcd.print(" Rs.");
  buffer="";
}

void printOnlineAmount() {
  int onlineAmountFromEEPROM;
  EEPROM.get(ONLINE_AMOUNT_ADDR, onlineAmountFromEEPROM); // Read online amount from EEPROM
  lcd.clear(); // Clear the LCD display
  lcd.print("Online= ");
  lcd.print(onlineAmountFromEEPROM);
  lcd.print(" Rs.");
  buffer="";
}
