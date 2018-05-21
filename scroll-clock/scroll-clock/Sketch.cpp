#include <Arduino.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

void printToScreen(int stuff);
void lineFeed();
void brightnessAdjust();
void processSyncMessage();
void initTime();
void showTime();
void clearScreen();

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
SoftwareSerial radio(2, 3);

const byte dimmerPin = 10;

const byte HEIGHT = 2;
const byte WIDTH = 16;

const byte LINEBUFFERSIZE = (HEIGHT - 1) * WIDTH;

byte unsupportedPlaceholder[8] = {
    B11111,
    B11111,
    B10101,
    B10101,
    B10001,
    B11111,
    B11111
};

void setup() {
    lcd.createChar(0, unsupportedPlaceholder);

    lcd.begin(WIDTH, HEIGHT);
    radio.begin(9600);

    pinMode(dimmerPin, OUTPUT);
    analogWrite(dimmerPin, 255);

    clearScreen();
}

void loop() {
    if (radio.available()) {
        int stuff = radio.read();

        if (!isAscii(stuff)) {
            printToScreen(-1);
            return;
        }

        if (stuff == '\\') {
            while (!radio.available());
            int stuff = radio.read();
            switch (stuff) {
            case 'x':
                lcd.noDisplay();
                break;
            case 'o':
                lcd.display();
                break;
            case 'c':
                clearScreen();
                break;
            case 'n':
                lineFeed();
                break;
            case 'b':
                brightnessAdjust();
                break;
            case 's':
                // RESERVED FOR TIME SYNCHRONIZATION
                break;
            case 't':
                // RESERVED FOR TIME MODE
                break;
            }
        } else {
            printToScreen(stuff);
        }
    }
}

int buf[LINEBUFFERSIZE];

byte currentPos = 0;
byte currentLine = 0;

void printToScreen(int c) {
    if (currentPos >= WIDTH) {
        lineFeed();
    }

    if (c == -1)
        lcd.write(byte(0));
    else
        lcd.write(c);

    if (currentLine > 0) {
        buf[currentPos] = c;
    }

    currentPos++;
}

void lineFeed() {

    // if on last row
    if (currentLine >= HEIGHT - 1) {
        lcd.clear();

        currentLine = 0;
        currentPos = 0;
        lcd.setCursor(currentLine, currentPos);

        for (int i = 0; i < LINEBUFFERSIZE; i++)
            printToScreen(buf[i]);
    }

    currentPos = 0;
    currentLine++;
    lcd.setCursor(currentPos, currentLine);

    for (int i = LINEBUFFERSIZE - WIDTH; i < WIDTH; i++)
        buf[i] = ' ';
}


void clearScreen() {
    lcd.clear();
    currentPos = 0;
    currentLine = 0;
    lcd.setCursor(currentPos, currentLine);

    for (int i = 0; i < LINEBUFFERSIZE; i++)
        buf[i] = ' ';
}

void brightnessAdjust() {
    while (!radio.available());
    analogWrite(dimmerPin, radio.read());
}
