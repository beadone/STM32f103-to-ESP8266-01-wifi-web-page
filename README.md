# STM32f103-to-ESP8266-01-wifi-web-page
Demo Arduino code to get NTC 10k analogue readings on a STM32f103 to a web page using a ESP8266-01.
I needed see 4 analogue readings on a local webpage. The ESP8266-01 does not have enough analogue pins for this purpose but it still can provide a simple webpage on your local wifi. I added an spare STM32f103 that I had which has more than enough analogue pins.
All the pieces are very cheap.

The setup of the STM32 is below

![stm32f103 ntc 10](https://github.com/user-attachments/assets/6de6d742-0849-4e15-ab87-ea7a3bbd83d6)

The code to read the thermistors has been copied from
https://fabacademy.org/2024/labs/skylab/students/tafia-sabila/assignments/W11-Input-Devices/w11-2-input-devices/
with some modification.
The thermistors are connected to the 3.3V input and to the analogue pin that will read the resistance. The analogue pin is also connected to ground through a 10K resistor.
PA2 is the Serial2 TX pin. This is connected to the RX pin on the ESP8266-01.
The programming was carried out using the Arduino IDE. I used StlinkV2 to program the STM32f103

The setup of the ESP8266-01

![esp8266-01 uart to wifi](https://github.com/user-attachments/assets/2bcf2697-49ce-4968-a051-f05dd2c329cc)

 I setup a breadboard to program the ESP8266-01 although it is probably easier to buy a programmer for it.
 An example of the setup is below.
 https://www.allaboutcircuits.com/projects/breadboard-and-program-an-esp-01-circuit-with-the-arduino-ide/

The ESP8266-01 has most of the web page info in css.h. This is included at the beggining of the program.

The temperatures are read every 2 seconds on the STM32f103 and the transmitted to the ESP8266-01 using serial2. Serial2 is already setup in the arduino code to be PA2 and PA3.
The ESP8266-01 cycles through a loop and checks the serial input on the primary serial interface. If data is availble it starts to read the data. It is expecting a string of the form   #xyy.yyCRLF
where:

hash (#) is the start of the string
x is the sensor number 1,2,3 or 4
yy.yy is the temparature. This can be any length.
CRLF is the end of the string (carriage return Line feed 1310)

The loop will check for noise or an empty buffer at each step as there are timing issues between the send and receive. 
There is a 1 second delay at the top of the loop to slow down the reads.
```
     // now looking for a sensor number but there is noise or a timiong issue on the feed.
      while (!(inByte ==  49 || inByte ==  50 || inByte ==  51 || inByte ==  52)) {  
          //getting noise, maybe timing as uart speed is 9600   
        Serial1.print(inByte);
        Serial1.print(", noise , ");
        delay(1000);   // fix the timing
```        
The Arduino board library for ESP8266-01 sets up Serial1 on the GPIO2 pin. This is used for debug.

If you want to set this up on a breadboard and see all the debug messages, you will need 3 USB to TTL converters. One each for the STM32f103 and ESP8266-01 debug messages and one to program the ESP8266-01.


        


