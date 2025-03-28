#include <LiquidCrystal.h>

// Configurare LCD: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
 
const int ledPins[] = {8,9,10};    
const int buttonPins[] = {14, 15, 16, 17}; 
const int buzzerPin = 11; // Pin pentru buzzer


int score = 0;
unsigned long gameStartTime;
unsigned long reactionStartTime;
float gameTime = 0.0;
float bestTime = 10000000;
bool gameRunning = false;

void setup() { 
  lcd.begin(16, 2);  
 
  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); //ledurile sunt stinse
  }
 
  for (int i = 0; i < 4; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  pinMode(buzzerPin, OUTPUT); // Configurare buzzer ca ieșire

  
  lcd.setCursor(0, 0);
  lcd.print("Reaction Game");
  lcd.setCursor(0, 1);
  lcd.print("Start in 3 sec");
  delay(1500);

  
  countdown();
}

void loop() {
  if (gameRunning) 
  {
    playGame();
  } 
  else if (digitalRead(buttonPins[3]) == LOW)
   { 
    resetGame();
  }
}




void countdown() {
  for (int i = 3; i > 0; i--) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Get Ready...");
    lcd.setCursor(7, 1);
    lcd.print(i);
    delay(800);
  }

  lcd.clear();
  gameStartTime = millis();
  gameRunning = true;
}

void playGame() { 
  int randomLed = random(0, 3);
  digitalWrite(ledPins[randomLed], HIGH);
  reactionStartTime = millis();
 
  while (true) {
    if (digitalRead(buttonPins[randomLed]) == LOW) { // Apăsat corect
      digitalWrite(ledPins[randomLed], LOW);
      score++;
      displayScore();  
      delay(500); // Debounce
      break;
    } else if (anyWrongButtonPressed(randomLed)) { // Apăsat greșit
      digitalWrite(ledPins[randomLed], LOW);
      showLoseMessage();
      return;
    }
  }
 
  if (score == 10) {
    showWinMessage();
    return;
  }
}

bool anyWrongButtonPressed(int correctButton) {
  for (int i = 0; i < 3; i++) {
    if (i != correctButton && digitalRead(buttonPins[i]) == LOW) {
      return true;
    }
  }
  return false;
}

void displayScore() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Score: ");
  lcd.print(score);  
}

void showLoseMessage() {
  gameRunning = false;
  gameTime = (millis()-gameStartTime) / 1000.0;

    // Sunet pentru Lose
  tone(buzzerPin, 300, 500); // Frecvență 300 Hz pentru 500 ms
  delay(500);
  tone(buzzerPin, 200, 500); // Frecvență 200 Hz pentru 500 ms
  delay(500);
  noTone(buzzerPin); // Oprire buzzer

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("You Lost!");
  lcd.setCursor(0, 1);
  lcd.print(score);
  lcd.print("pts, ");
  lcd.print(gameTime, 2);
  lcd.print("s");
}

void showWinMessage() {
  gameRunning = false;
  gameTime = (millis() - gameStartTime) / 1000.0;

  // Sunet pentru Win
  tone(buzzerPin, 1000, 500); // Frecvență 1000 Hz pentru 500 ms
  delay(500);
  tone(buzzerPin, 1200, 500); // Frecvență 1200 Hz pentru 500 ms
  delay(500);
  tone(buzzerPin, 1500, 500); // Frecvență 1500 Hz pentru 500 ms
  delay(500);
  noTone(buzzerPin); // Oprire buzzer

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Won! Time:");
  lcd.print(gameTime, 2);
  lcd.print("s");

  lcd.setCursor(0, 1);
  if (gameTime < bestTime) {
    bestTime = gameTime;
  }
  lcd.print("Best Time:");
  lcd.print(bestTime, 2);
  lcd.print("s");
}

void resetGame() { 
  score = 0;
  gameTime = 0.0;
  gameRunning = false;
 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Reaction Game");
  lcd.setCursor(0, 1);
  lcd.print("Start in 3 sec");
  delay(1500);
 
  countdown();
}
