  /*
Author: Nirmal & Joel
Date: January, 24, 2018
Description: To simulate a real life street light intersection.
*/

//Initializes variables
int SLNorthGreen = 3;
int SLNorthYellow = 4;
int SLNorthRed = 5;
int SLLeftTurn = 6;
int SLEastGreen = 9;  
int SLEastYellow = 10;  
int SLEastRed = 11; 

//0 = north green; 1 = east green
int SLON = 0;
boolean quickswitchnorth = false;
boolean quickswitcheast = false;

int LeftTurn_A = 0;
int NorthRedLightRun = 2;
int SouthRedLightRun = 3;
int EastRedLightRun = 1;
int WestRedLightRun = 4;

//Method that sweeps frequencies (400-800 hz)across pin 7
void Alarm(){

    for(int x = 0; x <= 1; x++){
      for(int x = 400; x <= 800; x++){
        tone(7,x);
        delay(2);
      }

      for(int x = 800; x >= 400; x--){
        tone(7,x);
        delay(5);
      }

      noTone(7);
    }    
}

//Sets digital pins to output to be used later
void setup() {
  Serial.begin(9600);
  for(int x = 0; x <= 13 ; x++){
    pinMode(x,OUTPUT);
  }
}

//Custom delay method to scan 2 analog pins for a desired change in its value
//Runs the alarm method when value is met
void DelayWtihScan(int Time, int Apin1, int Apin2){
  for(int y = 0; y <= Time; y++){
    if(analogRead(Apin1) <= 60 || analogRead(Apin2) <= 60){
          Alarm();
    }
    else{
      Serial.println(analogRead(1));
      delay(20);
    }
  }

//Method to test if there is a car waiting to make a turn, if there is the left turn signal light comes on, if not, the cycle continues
void LeftTurn(){
  if(analogRead(0) < 16){
    Serial.println(analogRead(0));
    digitalWrite(SLLeftTurn, HIGH);
    delay(5000);
    digitalWrite(SLLeftTurn, LOW);
  }
}

//The sequence that is run when to turn the East/west street lights off and transision them to red
void lightswitchEastoff(){  
  //Turns east and west green light on, waits 5 seconds then turns off
  //Turns east and west yellow light on, waits 1 seconds then turns off
  digitalWrite(SLEastGreen, LOW);
  digitalWrite(SLEastYellow, HIGH);
  delay(1000);
  digitalWrite(SLEastYellow, LOW);
  digitalWrite(SLEastRed, HIGH);
}

//The sequence that is run when to turn the North/South street lights off and transision them to red
void lightswitchNorthoff(){
  digitalWrite(SLNorthGreen, LOW);
  digitalWrite(SLNorthYellow, HIGH);
  delay(1000);
  digitalWrite(SLNorthYellow, LOW);
  digitalWrite(SLNorthRed, HIGH);
  delay(2000);
  digitalWrite(SLEastRed, LOW);
}

//Main loop that changes the street lights
void loop() {
  SLON = 0;
  
  //Lights are grouped with their pair ie. North with south.
  //Turns all street lights to red to start the pattern
  digitalWrite(SLEastRed, HIGH);
  digitalWrite(SLNorthRed, HIGH);
  delay(2000);

  //begins the sequence by turning the North red light off
  digitalWrite(SLNorthRed, LOW);

  //Checks for a car waiting to make a left turn
  LeftTurn();

  //After a car has made the left turn, or it is confirmed that a car doesnt not want to the North/South green light turns on
  digitalWrite(SLNorthGreen, HIGH);

  //Waits 5 seconds while checking for people running red lights
  DelayWtihScan(750,NorthRedLightRun, SouthRedLightRun); 
  
  //runs the sequence to turn the North/South lights off.
  lightswitchNorthoff();
 
  //Turns east and west green light on, waits 5 seconds then turns off
  SLON = 1;

  //Begins the East/West sequence by turning the green light on
  digitalWrite(SLEastGreen, HIGH);
  
  //Waits 5 seconds while checking for people running red lights
  DelayWtihScan(500,WestRedLightRun, EastRedLightRun);

  //Runs the sequence to turn the East/West lights off and transision them to red.
  lightswitchEastoff();

  //Program loops
}