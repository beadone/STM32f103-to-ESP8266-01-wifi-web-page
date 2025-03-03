
#include <Arduino.h>
#define ledPin1 PC13   // blue pill

int thermisterval;
int Temp_pin1 = PB0 ; // analog pin where the temperature sensor is connected
int Temp_pin2 = PA7 ;
int Temp_pin3 = PA6 ;
int Temp_pin4 = PA5 ;

float Temp_C; // variable to store temperature values in Celsius

double temp1;
double temp2;
double temp3;
double temp4;


void setup() {
    // Set up the built-in LED pin as an output:
    pinMode(ledPin1, OUTPUT);

    pinMode(Temp_pin1, INPUT_ANALOG);
    pinMode(Temp_pin2, INPUT_ANALOG);
    pinMode(Temp_pin3, INPUT_ANALOG);
    pinMode(Temp_pin4, INPUT_ANALOG);
    Serial1.begin(9600);      // debug
    Serial2.begin(9600);      // send to esp8266-01

}

void loop() {

      digitalWrite(ledPin1,!digitalRead(ledPin1));// Turn the LED from off to on, or on to off
   
      thermisterval = analogRead(Temp_pin1);
      temp1=ReadTemperature(thermisterval);
      Serial1.print(" temp1 serial 1: ");
      Serial1.println(temp1);
      Serial2.print("#1");
      Serial2.println(temp1); 

 
      thermisterval = analogRead(Temp_pin2);
      temp2=ReadTemperature(thermisterval);
      Serial1.print(" temp2 serial 1: ");
      Serial1.println(temp2); 
      Serial2.print("#2");
      Serial2.println(temp2);   

      thermisterval = analogRead(Temp_pin3);
      temp3=ReadTemperature(thermisterval);
      Serial1.print(" temp3 serial 1: ");
      Serial1.println(temp3);
      Serial2.print("#3"); 
      Serial2.println(temp3);  

      thermisterval = analogRead(Temp_pin4);
      temp4=ReadTemperature(thermisterval);
      Serial1.print(" temp4 serial 1: ");
      Serial1.println(temp4);
      Serial2.print("#4");    
      Serial2.println(temp4);
      delay(2000);
      

}



double ReadTemperature(int val) {
    double Temp;

    Serial1.print("Val: ");
    Serial1.print(val);
    // Converts the raw value to temperature in Celsius using the Steinhart-Hart Thermistor Equation 
    //stm32f103 adc resolution is 4096
    Temp = log(((40960000 / val) - 10000));
    Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp)) * Temp);

    // Conversion to Kelvin 
    Temp_C = Temp - 273.15; // Kelvin to Celsius conversion
   
    // Display in Serial Monitor
    Serial1.print(" Temperature: ");
    Serial1.print(Temp_C);
    Serial1.print(" °C, ");

return Temp_C;

}
