

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
//#include  <string.h>
/* Set these to your desired credentials. */
const char *ssid = "your sid";

const char *password = "your password";
#define   servername "thermistor"  //you can change this
String webpage = "";
#define ServerVersion "1.0"
#include "CSS.h"
char y;
const char* z;
char hashchar = '#';

char temp1[10] = "\0";
char temp2[10] = "\0";
char temp3[10] = "\0";
char temp4[10] = "\0";

ESP8266WebServer server(80);
int inByte = 0;         // incoming serial byte


void handleNotFound(){
   String message404 = "";
   message404 += "<!DOCTYPE HTML>";
    message404 += "<html><head><title>404 ur a doof</title> </head><body>";
    message404 += "<h1>404 ur a doof</h1>";
    message404 += "  </br> <a href=""index.htm"">Home</a></body></html>";
  server.send(404, "text/html", message404);
  Serial.print(message404);
}

void setup() {
  delay(1000);
  Serial1.begin(115200);  //debug
  Serial.begin(9600);     // to receive the STM32f103 messages

Serial1.println("starting.............");
  
  WiFi.begin(ssid, password);
  Serial1.print("Connecting to ");
  Serial1.println(ssid);
  // Wait for connection
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 40) { //wait 20 seconds
    Serial1.print("Connecting ");
    Serial1.println(i);
    delay(500);
  }
  if(i == 41){
    Serial1.print("Could not connect to");
    Serial1.println(ssid);
     ESP.restart();
    while(1) delay(500);
  }

  
  Serial1.print("Connected! IP address: ");
  Serial1.println(WiFi.localIP());

  server.on("/",HomePage);
  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial1.println("HTTP server started");

  if (MDNS.begin(servername)) {
    MDNS.addService("http", "tcp", 80);
    Serial1.println("MDNS responder started");
    Serial1.print("You can now connect to http://");
    Serial1.print(servername);
    Serial1.println(".local");
  }

}

void loop() {
  server.handleClient();
  MDNS.update();

  if (Serial.available() > 0) {

    inByte = Serial.read();

    while ((inByte < 35) || (inByte > 70))  {  // the start char is a #
      inByte = Serial.read();  //getting noise or it could be timing on the TX from the STM32
    }
    y = char(inByte);
    Serial1.print(inByte);
    Serial1.print("$");
    Serial1.println(y);
    // format of the incoming string is  #xyy.yyCRLF
    // x is the sensor number
    // yy.yy is the temperature
    // CRLF is carriage return Line feed
    if (inByte == 35) {      // # is the start of a read

      Serial1.println("in the loop");
      inByte = Serial.read();
      y = char(inByte);     
      // now looking for a sensor number but there is noise or a timiong issue on the feed.
      while (!(inByte ==  49 || inByte ==  50 || inByte ==  51 || inByte ==  52)) {  
          //getting noise, maybe timing as uart speed is 9600   
        Serial1.print(inByte);
        Serial1.print(", noise , ");
        delay(1000);   // fix the timing
        if (Serial.available() > 0) {
          inByte = Serial.read();
          y = char(inByte);
        }
      }
      // droped out of the temperature id number loop, now read the temperature
      while (inByte !=  10) {  // not line feed
       
      
      switch (inByte) {
        case 49:    //temp 1
          temp1[0] = '\0';
          while (inByte !=  13) {    // carriage return
            inByte = Serial.read();
            y = char(inByte);
            addChar(temp1, y);                               
          }
          Serial1.println(temp1);
          break;
       case 50:   //temp 2
        temp2[0] = '\0';
         while (inByte !=  13) {    // carriage return
            inByte = Serial.read();
            y = char(inByte);
           addChar(temp2, y);         
         }
          Serial1.println(temp2);
          break;
        case 51:    //temp 3
          temp3[0] = '\0';
          while (inByte !=  13) {    // carriage return
            inByte = Serial.read();
              y = char(inByte);
           addChar(temp3, y);         
          }
         Serial1.println(temp3);
          break;
       case 52:  //temp 4
          temp4[0] = '\0';
         while (inByte != 13) {    // carriage return
            inByte = Serial.read();
           y = char(inByte);
           addChar(temp4, y);        
           }
          Serial1.print("temp4 ");
          Serial1.println(temp4);
         break;      
        default:
         // should not get here except for noise
          y = char(inByte);
         Serial1.print(inByte);
         Serial1.print("*");
         Serial1.println(y);
          break;
      }   //switch
          // this should just  read the line feed at the end of the string
        inByte = Serial.read();
        y = char(inByte);
        Serial1.print(inByte);
        Serial1.print(",");
        Serial1.println(y); 
     
      }   // while (inByte !=  10)
    
    }  //if #
    

  }  // if serial available
}


void HomePage(){
  SendHTML_Header();
  webpage += F("<table><tr><th>Temp 1</th><th>Temp 2</th><th>Temp 3</th><th>Temp 4</th></tr>");
   webpage += F("<tr><td>"); 
   webpage += String(temp1) + "</td>";
   webpage += "<td>" + String(temp2) + "</td>";
   webpage += "<td>" + String(temp3) + "</td>";
   webpage += "<td>" + String(temp4) + "</td></tr>";  
  webpage += F("</table><hr>");
  //webpage += F("");

  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop(); // Stop is needed because no content length was sent
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SendHTML_Header(){
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate"); 
  server.sendHeader("Pragma", "no-cache"); 
  server.sendHeader("Expires", "-1"); 
  server.setContentLength(CONTENT_LENGTH_UNKNOWN); 
  server.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves. 
  append_page_header();
  server.sendContent(webpage);
  webpage = "";
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SendHTML_Content(){
  server.sendContent(webpage);
  webpage = "";
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SendHTML_Stop(){
  server.sendContent("");
  server.client().stop(); // Stop is needed because no content length was sent
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void addChar(char *s, char c) {
  
    // Get the current length of the string
    int l = strlen(s);  
  
    // Add the new character at the end
    s[l] = c;
  
    // Add the null terminator
    s[l + 1] = '\0';
}
