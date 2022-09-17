#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define ST_INIT         0
#define ST_ON           1
#define ST_OFF          2
#define ST_WARM         3
#define ST_COLD         4
#define ST_HOT          5
#define ST_TOOHOT       6
#define ST_TOOCOLD      7


#define IO_PWM_SPEED     9
#define IO_I_ON          2
#define LED1            10
#define LED2             7



#define POWERON 0
#define POWEROFF 1

int currentState = 0;
unsigned long timestamp;
float Voltage;
float analog_reading;
float temperature;
int motorSpeed;
int tempPin = A1;
int ON;
int tempMin = 20;
int tempMax = 50;
int tempWARM = 30;
int tempHOT = 40;


void setup()
{
  // Setting LCD'S:
  lcd.begin(16,3);
  //Message
  lcd.setCursor(3,1);
  lcd.print("Temperature");
  delay(2000);
  lcd.clear();
  currentState = ST_INIT;

  pinMode(IO_PWM_SPEED, OUTPUT);
  pinMode(IO_I_ON, INPUT );
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  Serial.begin(9600);

  currentState = ST_OFF;
  
}

void loop()
{
  analog_reading = analogRead(tempPin);
   Voltage = analog_reading*5/1024;
  temperature = 100* (Voltage - 0.5) ;
  lcd.print(temperature);
  
Serial.println("CURRENT STATE = ");
Serial.println(currentState);
if (currentState == ST_OFF)
  {
     /* detect and handle state transition  
        events here */

      ON = digitalRead(IO_I_ON);

      if (ON == POWERON)
      {
	   Serial.println("- Power on event detected. \n" );
  	   digitalWrite(LED1, HIGH);  /* turn on light fully */
          currentState = ST_ON;  /* go to next state */
      }
  else {
    digitalWrite(LED1,LOW);
  }
}
    if(currentState == ST_ON){
     ON = digitalRead(IO_I_ON);
     digitalWrite(LED1, HIGH);

    if (ON == POWEROFF); 
      {
	   Serial.println("- POWER OFF event detected. \n" ); 
          timestamp = millis();  /* log the time of entry to Power_off state */
          currentState = ST_OFF;  /* go to next state */
      }
      if(temperature < tempMin)//if the temp is lower than the min
      {
        Serial.println("Temp is too low!");
        currentState = ST_TOOCOLD;
      }
      if(temperature > tempMax){;
        Serial.println("Temp is too High!");
        currentState = ST_TOOHOT;
      }
      if((temperature > tempMin) && (temperature <= tempWARM)){
        currentState = ST_COLD;
      }
      if((temperature > tempWARM) && (temperature <= tempHOT)){
       
        currentState = ST_WARM;
      }

      if((temperature > tempHOT) && (temperature <= tempMax)){
        currentState = ST_HOT;
      }
  }
  if(currentState == ST_COLD){
    analogWrite(IO_PWM_SPEED, 51);
    lcd.setCursor(0,0);
    lcd.print("Motor speed: 30%");
    delay(1000);
    lcd.clear();
    
    if(temperature < tempMin)// if the temp is too low
    {
      lcd.setCursor(0,0);
      lcd.print("Temperature is too Low!");
      delay(3000);
      lcd.clear();
      currentState = ST_TOOCOLD;
    }
    if(temperature > tempMax){
      lcd.setCursor(0,0);
      lcd.print("Temperature is too High!");
      currentState = ST_TOOHOT;
    }
    if((temperature > tempMin) && (temperature <= tempWARM)){
      currentState = ST_COLD;
    }
    if((temperature > tempWARM) && (temperature <= tempHOT)){
      currentState = ST_WARM;
    }
    if((temperature > tempHOT) && (temperature <= tempMax)){
      currentState = ST_HOT;
    }
    if (ON == POWEROFF);{
      Serial.println("-POWER OFF event dected.");
      timestamp = millis();
      currentState = ST_OFF;
    }
  }
  if (currentState == ST_WARM){
    analogWrite(IO_PWM_SPEED, 100);
    lcd.print("Motor speed: 50%");
    delay(2000);
    lcd.clear();
    
    if(temperature < tempMin){
      Serial.println("Temp is too low!");
      currentState = ST_TOOCOLD;
    }
    if(temperature > tempMax){
      Serial.println("Temp is too HIGH!");
      delay(3000);
      currentState = ST_TOOHOT;
    }
    if((temperature > tempMin) && (temperature <= tempWARM)){
      currentState = ST_COLD;
    }
    if((temperature > tempWARM) && (temperature <= tempHOT)){
      currentState = ST_WARM;
    }
       if(( temperature > tempHOT) && (temperature <= tempMax)){
         currentState = ST_HOT;
      }
    if ( ON == POWEROFF);{
      Serial.println("- POWER OFF event detected");
      timestamp = millis();
      currentState = ST_OFF;
    }
  }
  if(currentState == ST_HOT){
    analogWrite(IO_PWM_SPEED,153);
    lcd.print("Motor speed 70%");
    delay(3000);
    lcd.clear();
    
    if( temperature < tempMin){
      Serial.println("The temp is too low");
      currentState = ST_TOOCOLD;
    }
    if(temperature > tempMax){
      Serial.println("The temp is too High!");
      currentState = ST_TOOHOT;
    }
    if((temperature > tempMin) && (temperature <= tempWARM)){
      currentState = ST_COLD;
    }
    if((temperature > tempWARM) && (temperature <= tempHOT)){
      currentState = ST_WARM;
    }
    if((temperature > tempHOT) && (temperature <= tempMax)){
      currentState = ST_HOT;
    }
    if(ON == POWEROFF);{
      Serial.println("POWER OFF event detected.");
      timestamp = millis();
      currentState = ST_OFF;
    }
  }
  if(currentState == ST_TOOCOLD){
    analogWrite(9,0);
    digitalWrite(LED2, HIGH);
    delay(3000);
    digitalWrite(LED2, LOW);
    currentState = ST_ON;
  }
  if(currentState == ST_TOOHOT){
    analogWrite(9,0);
    digitalWrite(LED2, HIGH);
    delay(3000);
    digitalWrite(LED2, LOW);
    currentState = ST_ON;
  }
}
