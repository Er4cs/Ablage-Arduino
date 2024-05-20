#include <Keypad.h> //Keypad library Ziffernblock 
#include <Servo.h> //Servo library -Motor



char P1='4';char P2='C';char P3='4';char P4='B'; // Akteulles Passwort für den Tresor (Bastwoord Joooonge)
char C1, C2, C3, C4; // Unter C1 bis C4 werden im Loop die vier eingegebenen Zeichen gespeichert

Servo Motor; //Servo wird ab jetzt mit „Motor“ angesprochen
int LEDR = 24; //Die gelböh LED ist an Pin 24 angeschlossen
int LEDG = 23; //Die grüne LED wird an Pin 23 angeschlossen
//Hier wird die größe des Keypads definiert
const byte COLS = 4; //4 Spalten
const byte ROWS = 4; //4 Zeilen
int z1=0, z2, z3, z4; // Diese Variablen werden verwendet um für die einzelnen Zahlencodes die EIngabe freizuschalten. Damit wird im Sketch verhindert, dass eine einzene Codeziffer einer falschen Position zugeordnet wird.
//Die Ziffern und Zeichen des Keypads werden eingegeben:
char hexaKeys[ROWS][COLS]={
{'D','#','0','*'},
{'C','9','8','7'},
{'B','6','5','4'},
{'A','3','2','1'}
};

byte colPins[COLS] = {2,3,4,5}; //Definition der Pins für die 3 Spalten
byte rowPins[ROWS] = {6,7,8,9}; //Definition der Pins für die 4 Zeilen
char Taste; //Taste ist die Variable für die jeweils gedrückte Taste.
Keypad Tastenfeld = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); //Das Keypad kann absofort mit "Tastenfeld" angesprochen werden

void setup() //Programm start
{
Serial.begin(9600); //Datenrate für die serielle Datenübertragung

//Die LEDs werden als Ausgang festgelegt
pinMode(LEDR, OUTPUT); // LED-gelb
pinMode(LEDG, OUTPUT); //LED-grün
Motor.attach(12); //Der Servo ist an Pin 12 angeschlossen
}
void loop() //Endlosschleife
{
Anfang: // Dies ist eine Markierung, zu der per "goto-"Befehl gesprungen werden kann.

Taste = Tastenfeld.getKey(); //Mit Unter der Variablen pressedKey entspricht der gedrückten Taste
if (Taste) //Wenn eine Taste gedrückt wurde...
//Ab hier werden die Eingaben des Tastenfeldes verarbeitet. Zunächst die "*"Taste, da diese eine Sonderfunktion für die Verriegelung besitzt und danach die #-Taste, 
//nach deren Eingabe der zuvor eingegebene Code auf Richtigkeit geprüft wird. 
 {
  if (Taste=='D') // Wenn die "´D" Taste gedrückt wurde...
   { 
   Serial.println ("Code zurückgesetzt, bitte erneut eingeben");
   digitalWrite(LEDR, HIGH); // Die gelb LED leuchtet
   digitalWrite(LEDG, LOW); // Die grüne LED is aus
   delay(3000); //warten
   z1=0; z2=1; z3=1; z4=1; // Der Zugang für die erste Zeicheneingabe wird wieder freigeschaltet
   goto Anfang; //zurück zu zeile 40 
   }

  if (Taste=='*') // Wenn die "*" Taste gedrückt wurde...
   {
    //Ausgabe 
   Serial.println("Tuer wurde verriegelt");
   delay(1000); //warten
   Motor.write(90); //Motor zum verriegeln auf 90 Grad ansteuern, --> Schloss steht Horizontal
   digitalWrite(LEDR, HIGH); //..die gelbe LED einschalten
   digitalWrite(LEDG, LOW); //..die grüne LED einschalten
   z1=0; z2=1; z3=1; z4=1; // Zugang zur ersten Zeicheneingabe freischalten
   goto Anfang; ////zurück zu zeile 40 
   
  if (Taste=='#') // Wenn die Rautetaste gedrückt wurde...
   {
   if (C1==P1&&C2==P2&&C3==P3&&C4==P4) //wird gepüft, ob die eingaben Codezeichen (C1 bis C4) mit den Zeichen des Passwortes (P1 bis P4) übereinstimmen. Falls der eingegebene Code richtig ist...
   {
    //Ausgabe
   Serial.println ("Code korrekt, Schloss offen"); 
   Motor.write(0); //Servo zum öffnen auf 0 Grad ansteuern Schloss steht vertikal
   digitalWrite(LEDR, LOW); //..die gelb LED nicht leuchten..
   digitalWrite(LEDG, HIGH); //..die grüne LED leuchten..
   }
   else // ist das nicht der Fall, bleibt das Schloss gesperrt
   {
    //Ausgabe
   Serial.println ("Code falsch, Schloss gesperrt");
   digitalWrite(LEDR, HIGH); // Die gelb LED leuchtet
   digitalWrite(LEDG, LOW); // Die grüne LED is aus
   delay(2000); //warten für 2 sek
   z1=0; z2=1; z3=1; z4=1; // Der Zugang für die erste Zeicheneingabe wird wieder freigeschaltet
   goto Anfang; ////zurück zu zeile 40 

// Ab hier werden die vier Code-positionen unter den Variablen C1 bis C4 abgespeichert. Damit die eingegebenen Zeichen auch an der richtigen Position des Passwortes gespeichert werden, wird mit den Variablen z1 bis z4 der Zugang zu den einzelnen Positinen freigegeben oder gesperrt.
  if (z1==0) // Wenn das erste Zeichen noch nicht gespeichert wurde...
  {
  C1=Taste; //Unter der Variablen "C1" wird nun die aktuell gedrückte Taste gespeichert
  Serial.print("Die Taste "); //Teile uns am Serial Monitor die gedrückte Taste mit
  Serial.print(C1);
  Serial.println(" wurde gedrueckt");
  z1=1; z2=0; z3=1; z4=1; // Zugang zur zweiten Zeicheneingabe freischalten
  goto Anfang; //zurück zu zeile 40 
  }
 
  if (z2==0) // Wenn das zweite Zeichen noch nicht gespeichert wurde...
  {
  C2=Taste; //Unter der Variablen "C2" wird nun die aktuell gedrückte Taste gespeichert
  Serial.print("Die Taste: "); //Teile uns am Serial Monitor die gedrückte Taste mit
  Serial.print(C2);
  Serial.println(" wurde gedrueckt");
  z1=1; z2=1; z3=0; z4=1; // Zugang zur dritten Zeicheneingabe freischalten
  goto Anfang; //zurück zu zeile 40 
  }

  if (z3==0) // Wenn das dritte Zeichen noch nicht gespeichert wurde...
  {
  C3=Taste; //Unter der Variablen "C3" wird nun die aktuell gedrückte Taste gespeichert
  Serial.print("Die Taste "); //Teile uns am Serial Monitor die gedrückte Taste mit
  Serial.print(C3);
  //Ausgabe
  Serial.println(" wurde gedrueckt");
  z1=1; z2=1; z3=1; z4=0; // Zugang zur vierten Zeicheneingabe freischalten
  goto Anfang; //zurück zu zeile 40 
  }
  
  if (z4==0) // Wenn das vierte Zeichen noch nicht gespeichert wurde...
  {
  C4=Taste; //Unter der Variablen "C4" wird nun die aktuell gedrückte Taste gespeichert
  Serial.print("Die Taste "); //Teile uns am Serial Monitor die gedrückte Taste mit
  Serial.print(C4);
  //Ausgabe
  Serial.println(" wurde gedrückt");
  z1=1; z2=1; z3=1; z4=1; // Zugang zur weiteren Zeicheneingabe sperren
  }
 }
}

// Funktion der Roten LED, NFC und der Buzzer habe kalte Lötstellen oder einen Wackelkontakt der das nutzen der Module nicht möglich macht.
//Diese Kalten Lötstellen sind im Schaltplan nicht extra hervor gehoben.
//17.05.2024 Bruno Pfleger, Eric Nieder 