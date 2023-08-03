
// Include necessay libraries
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
 
LiquidCrystal_I2C lcd(0x27, 16, 2); // Format -> (Address,Width,Height )

const byte ROWS = 4; // Define the number of rows on the keypad
const byte COLS = 3; // Define the number of columns on the keypad
char keys[ROWS][COLS] = { // Matrix defining character to return for each key
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'.','0','#'}
};
byte rowPins[ROWS] = {8, 7, 6, 5}; //connect to the row pins (R0-R3) of the keypad
byte colPins[COLS] = {4, 3, 2}; //connect to the column pins (C0-C2) of the keypad
//initialize an instance of class
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const int relayPins[] = {9, 10, 11, 12, 13}; // Relay out pins
const int numRelays = 5;

int cursorColumn = 12;
int voltageIndex = 0;
 
void setup() {
    // initialize the lcd
    lcd.init();
    // Turn on the Backlight
    lcd.backlight();
    // Clear the display buffer
    lcd.clear();
    // Set cursor (Column, Row)
    lcd.setCursor(0, 0);
    // print "Programmable" at (0, 0)       
    lcd.print("Programmable");
    // Set cursor (Column, Row)
    lcd.setCursor(0, 1);
    // print "Power Supply" at (0, 1)       
    lcd.print("Power Supply");
    delay(2500); // delay of 1s
    
    for (int i = 0; i < numRelays; i++) {
        pinMode(relayPins[i], OUTPUT);
        digitalWrite(relayPins[i], HIGH); // Ensure all relays are initially OFF
    }
}
 
void loop() {
    static char voltage[5]; // Array to store the entered voltage (3 digits + terminator)
    
    char key = keypad.getKey();// get the imput from keypad
    
    // Clear the display buffer
    lcd.clear();
    // Set cursor (Column, Row)
    lcd.setCursor(0, 0);
    // print "Enter Voltage" at (0, 0)       
    lcd.print("Enter Voltage:");
    
    //Get the input voltage and display it on LCD
    if (key) {
        lcd.setCursor(cursorColumn, 1); // move cursor to   (cursorColumn, 0)
        if(voltageIndex>4) {
            // Clear the display buffer
            lcd.clear();
            // Set cursor (Column, Row)
            lcd.setCursor(0, 0);
            // print "Invalid Input" at (0, 0)       
            lcd.print("Invalid Input");
        }
            
        if(key =='#') {
            cursorColumn = 12;
            voltageIndex = 0;
            
            float Voltage = atof(voltage); // Converts string to numerical float value 
            if(Voltage >30.0) {
                // Clear the display buffer
                lcd.clear();
                // Set cursor (Column, Row)
                lcd.setCursor(0, 0);
                // print "Invalid Input" at (0, 0)       
                lcd.print("Invalid Input");
            }
            else {
                if (Voltage >= 0 && Voltage <= 7) {
                    // Control the first relay based on the voltage range 0-7V
                    digitalWrite(relayPins[0], LOW); // Turns ON the first relay and the connected load
                    digitalWrite(relayPins[1], HIGH);  // Turns OFF the other relays and their connected loads
                    digitalWrite(relayPins[2], HIGH);
                    digitalWrite(relayPins[3], HIGH);
                    digitalWrite(relayPins[4], HIGH);
                }
                else if (Voltage > 7 && Voltage <= 14) {
                    // Control the second relay based on the voltage range 8-14V
                    digitalWrite(relayPins[0], HIGH);
                    digitalWrite(relayPins[1], LOW); // Turns ON the second relay and the connected load
                    digitalWrite(relayPins[2], HIGH);
                    digitalWrite(relayPins[3], HIGH);
                    digitalWrite(relayPins[4], HIGH);
                }
                else if (Voltage > 14 && Voltage <= 21) {
                    // Control the third relay based on the voltage range 15-21V
                    digitalWrite(relayPins[0], HIGH);
                    digitalWrite(relayPins[1], HIGH);
                    digitalWrite(relayPins[2], LOW); // Turns ON the third relay and the connected load
                    digitalWrite(relayPins[3], HIGH);
                    digitalWrite(relayPins[4], HIGH);
                } 
                else if (Voltage > 21 && Voltage <= 28) {
                    // Control the fourth relay based on the voltage range 22-28V
                    digitalWrite(relayPins[0], HIGH);
                    digitalWrite(relayPins[1], HIGH);
                    digitalWrite(relayPins[2], HIGH);
                    digitalWrite(relayPins[3], LOW); // Turns ON the fourth relay and the connected load
                    digitalWrite(relayPins[4], HIGH);
                }
                else if (Voltage > 28 && Voltage <= 35) {
                    // Control the fifth relay based on the voltage range 29-35V
                    digitalWrite(relayPins[0], HIGH);
                    digitalWrite(relayPins[1], HIGH);
                    digitalWrite(relayPins[2], HIGH);
                    digitalWrite(relayPins[3], HIGH);
                    digitalWrite(relayPins[4], LOW); // Turns ON the fifth relay and the connected load
                }
                else {
                    // If the voltage is out of the specified ranges, turn OFF all relays
                    for (int i = 0; i < numRelays; i++) {
                        digitalWrite(relayPins[i], HIGH);
                    }
                }
            }
        }
        else{
            lcd.print(key); // print key at (cursorColumn, 0)
            cursorColumn++;
            voltage[voltageIndex] = key; // Store the entered digit in the array
            voltageIndex++; // Move to the next position
        }
        delay(1000); // delay of 1 s
    }    
}
