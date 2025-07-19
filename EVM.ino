#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

#define S1 7  // MOHIT
#define S2 6  // VARU
#define S3 5  // KAI
#define S4 4  // DARSH
#define S5 3  // Result / Reset / Power

int vote1 = 0, vote2 = 0, vote3 = 0, vote4 = 0;
bool isOn = true;
bool voteInProgress = false;

void showWelcomeAnimation() {
  lcd.clear();
  for (int i = 0; i < 16; i++) {
    lcd.setCursor(i, 0);
    lcd.print(">");
    delay(50);
  }
  lcd.setCursor(2, 0); lcd.print("Electronic");
  lcd.setCursor(1, 1); lcd.print("Voting Machine");
  delay(2000);
  lcd.clear();
}

void setup() {
  pinMode(S1, INPUT_PULLUP);
  pinMode(S2, INPUT_PULLUP);
  pinMode(S3, INPUT_PULLUP);
  pinMode(S4, INPUT_PULLUP);
  pinMode(S5, INPUT_PULLUP);

  lcd.begin(16, 2);
  showWelcomeAnimation();
}

void thankYouMessage() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Thank you for");
  lcd.setCursor(4, 1);
  lcd.print("voting!");
  delay(5000); // Show message for 5 seconds
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Please Wait...");
  delay(1000);
  lcd.clear();
}

void showAllVotes() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MO:");
  lcd.print(vote1);
  lcd.setCursor(8, 0);
  lcd.print("VA:");
  lcd.print(vote2);
  lcd.setCursor(0, 1);
  lcd.print("KA:");
  lcd.print(vote3);
  lcd.setCursor(8, 1);
  lcd.print("DA:");
  lcd.print(vote4);
  delay(3000);
}

void showWinner() {
  int totalVotes = vote1 + vote2 + vote3 + vote4;
  lcd.clear();

  if (totalVotes == 0) {
    lcd.setCursor(2, 0);
    lcd.print("No Voting...");
    delay(2000);
    lcd.clear();
    return;
  }

  String winner;
  int maxVotes = vote1;
  winner = "MOHIT";

  if (vote2 > maxVotes) {
    maxVotes = vote2;
    winner = "VARU";
  }
  if (vote3 > maxVotes) {
    maxVotes = vote3;
    winner = "KAI";
  }
  if (vote4 > maxVotes) {
    maxVotes = vote4;
    winner = "DARSH";
  }

  int count = 0;
  if (vote1 == maxVotes) count++;
  if (vote2 == maxVotes) count++;
  if (vote3 == maxVotes) count++;
  if (vote4 == maxVotes) count++;

  lcd.clear();
  if (count > 1) {
    lcd.setCursor(4, 0); lcd.print("TIE");
    lcd.setCursor(1, 1); lcd.print("No Clear Winner");
  } else {
    lcd.setCursor(0, 0);
    lcd.print(winner + " Wins!");
    lcd.setCursor(0, 1);
    lcd.print("Votes: ");
    lcd.print(maxVotes);
    lcd.print("/");
    lcd.print(totalVotes);
  }

  delay(4000);
  lcd.clear();
}

void loop() {
  if (!isOn) return;

  // Don't show votes while voting — LCD stays blank

  if (!voteInProgress) {
    if (digitalRead(S1) == LOW) {
      vote1++;
      voteInProgress = true;
      thankYouMessage();
    }
    else if (digitalRead(S2) == LOW) {
      vote2++;
      voteInProgress = true;
      thankYouMessage();
    }
    else if (digitalRead(S3) == LOW) {
      vote3++;
      voteInProgress = true;
      thankYouMessage();
    }
    else if (digitalRead(S4) == LOW) {
      vote4++;
      voteInProgress = true;
      thankYouMessage();
    }
  }

  if (voteInProgress) {
    if (digitalRead(S1) == HIGH && digitalRead(S2) == HIGH &&
        digitalRead(S3) == HIGH && digitalRead(S4) == HIGH) {
      voteInProgress = false;
    }
    return;
  }

  if (digitalRead(S5) == LOW) {
    unsigned long startTime = millis();
    while (digitalRead(S5) == LOW);
    unsigned long duration = millis() - startTime;

    if (duration >= 3000) {
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Powering Off");
      delay(1000);
      lcd.noDisplay();
      isOn = false;
      return;
    }
    else if (duration >= 2000) {
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Votes Reset");
      vote1 = vote2 = vote3 = vote4 = 0;
      delay(2000);
      lcd.clear();
    }
    else {
      showAllVotes();
      showWinner();
    }
  }
}
