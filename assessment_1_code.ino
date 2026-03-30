#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int redLED = 2;
int greenLED = 3;
int attackBtn = 4;
int defendBtn = 5;
int specialBtn = 6;
int buzzer = 7;
int bossHP = 5;
int totalRounds = 6;

void setup() {
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(attackBtn, INPUT);
  pinMode(defendBtn, INPUT);
  pinMode(specialBtn, INPUT);
  pinMode(buzzer, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.display();
}

void loop() {
  // START SCREEN
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(5, 10);
  display.println("BOSS");
  display.setCursor(5, 30);
  display.println("BATTLE");
  display.setTextSize(1);
  display.setCursor(10, 55);
  display.println("Press ATTACK");
  display.display();

  while (digitalRead(attackBtn) == LOW);
  delay(200);

  // READY
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(20, 20);
  display.println("READY");
  display.display();
  delay(500);

  // COUNTDOWN
  for (int i = 3; i > 0; i--) {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(3);
    display.setCursor(50, 20);
    display.println(i);
    display.display();
    tone(buzzer, 800);
    delay(500);
    noTone(buzzer);
  }

  bossHP = 5;

  // GAME ROUNDS
  for (int round = 1; round <= totalRounds; round++) {
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, LOW);

    int move = random(1, 4);

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("HP: ");
    display.print(bossHP);
    display.print("  Round: ");
    display.print(round);
    display.print("/6");
    display.setTextSize(2);
    display.setCursor(10, 25);
    if (move == 1) display.println("ATTACK!");
    if (move == 2) display.println("DEFEND!");
    if (move == 3) display.println("SPECIAL!");
    display.display();

    // ⏱️ Response window — 4 seconds with a live countdown bar
    unsigned long startTime = millis();
    bool correct = false;
    unsigned long totalTime = 4000;

    while (millis() - startTime < totalTime) {
      // Live timer bar at the bottom of the screen
      unsigned long elapsed = millis() - startTime;
      int barWidth = map(elapsed, 0, totalTime, 128, 0);

      display.fillRect(0, 56, 128, 8, SSD1306_BLACK);   // clear bar area
      display.fillRect(0, 56, barWidth, 8, SSD1306_WHITE); // draw remaining time
      display.display();

      if (digitalRead(attackBtn) == HIGH) { if (move == 1) correct = true; break; }
      if (digitalRead(defendBtn) == HIGH) { if (move == 2) correct = true; break; }
      if (digitalRead(specialBtn) == HIGH) { if (move == 3) correct = true; break; }
    }

    // RESULT
    if (correct) {
      bossHP--;
      digitalWrite(greenLED, HIGH);
      tone(buzzer, 1200);
      display.clearDisplay();
      display.setTextColor(SSD1306_WHITE);
      display.setTextSize(2);
      display.setCursor(20, 20);
      display.println("HIT!");
      display.display();
    } else {
      digitalWrite(redLED, HIGH);
      tone(buzzer, 400);
      display.clearDisplay();
      display.setTextColor(SSD1306_WHITE);
      display.setTextSize(2);
      display.setCursor(20, 20);
      display.println("MISS!");
      display.display();
    }

    delay(800);
    noTone(buzzer);

    // WIN CHECK
    if (bossHP <= 0) {
      display.clearDisplay();
      display.setTextColor(SSD1306_WHITE);
      display.setTextSize(2);
      display.setCursor(10, 20);
      display.println("YOU WIN!");
      display.display();
      tone(buzzer, 1500);
      delay(2000);
      noTone(buzzer);
      return;
    }
  }

  // LOSE
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(10, 20);
  display.println("Better Luck");
  display.setCursor(20, 35);
  display.println("Next Time!");
  display.display();
  tone(buzzer, 300);
  delay(2000);
  noTone(buzzer);
}