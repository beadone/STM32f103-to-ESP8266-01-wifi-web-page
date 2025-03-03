# STM32f103-to-ESP8266-01-wifi-web-page
Demo Arduino code to get NTC 10k analogue readings on a STM32f103 to a web page using a ESP8266-01.
The setup of the STM32 is below

![stm32f103 ntc 10](https://github.com/user-attachments/assets/6de6d742-0849-4e15-ab87-ea7a3bbd83d6)

The code to read the thermistors has copied from
https://fabacademy.org/2024/labs/skylab/students/tafia-sabila/assignments/W11-Input-Devices/w11-2-input-devices/
with some modification.

The setup of the ESP8266-01

![esp8266-01 uart to wifi](https://github.com/user-attachments/assets/2bcf2697-49ce-4968-a051-f05dd2c329cc)

The programming was carried out using the Arduino IDE. I used StlinkV2 to program the STM32 and I setup a breadboard to manage the ESP8266-01 although it is probably easier to buy a programmer for it.

The ESP8266-01 has most of the web page info in css.h. This is included at the beggining of the program.

The temperatures are read every 2 seconds on the STM32f103 and the treansmitted to the ESP8266-01. The ESP8266-01 cycles through a loop and checks the serial input. If data is avaialble it started to read the data. It is expecting a string of the form   #xyy.yyCRLF
where:
# is the start of the string
x is the sensor number 1,2,3 or 4
yy.yy is the temparature. This can be any length
CRLF is the end of the string (carriage return Line feed 1310)

The loop will check for noise or an empty buffer at each step as there are timing issues between the send and receive. There is a 1 second delay at the top of the loop to slow down the reads.

      // now looking for a sensor number but there is noise or a timiong issue on the feed.
      while (!(inByte ==  49 || inByte ==  50 || inByte ==  51 || inByte ==  52)) {  
          //getting noise, maybe timing as uart speed is 9600   
        Serial1.print(inByte);
        Serial1.print(", noise , ");
        delay(1000);   // fix the timing


        


