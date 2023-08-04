
// Include necessay libraries
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
 
LiquidCrystal_I2C lcd(0x27, 16, 2); // Format -> (Address,Width,Height )

const byte rows = 4; // Define the number of rows on the keypad
const byte columns = 3; // Define the number of columns on the keypad
char keys[rows][columns] = { // Matrix defining character to return for each key
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'.','0','#'}
};
byte rowPins[rows] = {8, 7, 6, 5}; //connect to the row pins (R0-R3) of the keypad
byte columnPins[columns] = {4, 3, 2}; //connect to the column pins (C0-C2) of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, columnPins, rows, columns ); //initialize an instance of class

const int relayPins[] = { 9, 10, 11, 12,13}; // Relay out pins
const int numberOfRelays = 5;

String voltage = "";
double voltageInput = 0.0;

 
void setup() {
    lcd.init(); // initialize the lcd
    lcd.backlight(); // Turn on the Backlight
    lcd.clear(); // Clear the display buffer
    lcd.setCursor(0, 0); // Set cursor (Column, Row)
    lcd.print("Programmable"); // print "Programmable" at (0, 0)
    lcd.setCursor(0, 1); // Set cursor (Column, Row)
    lcd.print("Power Supply"); // print "Power Supply" at (0, 1)
    delay(2000); // delay of 2s
    lcd.clear(); // Clear the display buffer
    
    for (int i = 0; i < numberOfRelays; i++) {
        pinMode(relayPins[i], OUTPUT);
        digitalWrite(relayPins[i], HIGH); // Ensure all relays are initially OFF
    }
}

 
void loop() {
    input(); //function call input()
}


void input() { 
    lcd.clear(); // Clear the display buffer
    lcd.setCursor(0,0); // Set cursor (Column, Row)
    lcd.print("Enter Voltage: "); // print "Enter Voltage: " at (0, 0)
    relayControl(); // function call relayControl() 
}


void relayControl() {
    while(true) {
        char key = keypad.getKey();
        if(key) {
            if (key!='#') { // if key pressed is not '#'
                voltage += key; // append each character to string voltage 
                lcd.setCursor(12,1); // Set cursor (Column, Row)
                lcd.print("    "); // Clear column set by setCursor
                lcd.setCursor(12,1); // Set cursor (Column, Row)
                lcd.print(voltage); // print voltage at (12, 1)
            }
            else {
                voltageInput=voltage.toDouble(); // convert string value voltage to double type
                if(voltageInput >30.0) { 
                    lcd.clear(); // Clear the display buffer
                    lcd.setCursor(0, 0); // Set cursor (Column, Row)
                    lcd.print("Invalid Input"); // print "Invalid Input" at (0, 0) 
                    delay(2000); // delay of 2s
                    lcd.clear(); // Clear the display buffer
                    lcd.setCursor(0,0); // Set cursor (Column, Row)
                    lcd.print("Enter Voltage: "); // print "Enter Voltage: " at (0, 0)
                }
                else {
                    if (voltageInput >= 0 && voltageInput <= 7) {
                        // Control the first relay based on the voltage range 0-7V
                        digitalWrite(relayPins[0], LOW); // Turns ON the first relay and the connected load
                        digitalWrite(relayPins[1], HIGH);  
                        digitalWrite(relayPins[2], HIGH);
                        digitalWrite(relayPins[3], HIGH);
                        digitalWrite(relayPins[4], HIGH);
                    }
                    else if (voltageInput > 7 && voltageInput <= 14) {
                        // Control the second relay based on the voltage range 8-14V
                        digitalWrite(relayPins[0], HIGH);
                        digitalWrite(relayPins[1], LOW); // Turns ON the second relay and the connected load
                        digitalWrite(relayPins[2], HIGH);
                        digitalWrite(relayPins[3], HIGH);
                        digitalWrite(relayPins[4], HIGH);
                    }
                    else if (voltageInput > 14 && voltageInput <= 21) {
                        // Control the third relay based on the voltage range 15-21V
                        digitalWrite(relayPins[0], HIGH);
                        digitalWrite(relayPins[1], HIGH);
                        digitalWrite(relayPins[2], LOW); // Turns ON the third relay and the connected load
                        digitalWrite(relayPins[3], HIGH);
                        digitalWrite(relayPins[4], HIGH);
                    } 
                    else if (voltageInput > 21 && voltageInput <= 28) {
                        // Control the fourth relay based on the voltage range 22-28V
                        digitalWrite(relayPins[0], HIGH);
                        digitalWrite(relayPins[1], HIGH);
                        digitalWrite(relayPins[2], HIGH);
                        digitalWrite(relayPins[3], LOW); // Turns ON the fourth relay and the connected load
                        digitalWrite(relayPins[4], HIGH);
                    }
                    else if (voltageInput > 28 && voltageInput <= 35) {
                        // Control the fifth relay based on the voltage range 29-35V
                        digitalWrite(relayPins[0], HIGH);
                        digitalWrite(relayPins[1], HIGH);
                        digitalWrite(relayPins[2], HIGH);
                        digitalWrite(relayPins[3], HIGH);
                        digitalWrite(relayPins[4], LOW); // Turns ON the fifth relay and the connected load
                    }
                    else {
                        // If the voltage is out of the specified ranges, turn OFF all relays
                        for (int i = 0; i < numberOfRelays; i++) {
                            digitalWrite(relayPins[i], HIGH);
                        }
                    }
                }
                voltage = "";
            }
        }
    }
}
  
  
