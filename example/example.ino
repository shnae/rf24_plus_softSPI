#include <SPI.h>

#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>

#include <DigitalIO.h>
#include <DigitalPin.h>
#include <I2cConstants.h>
#include <nRF24L01.h>
#include <PinIO.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SoftI2cMaster.h>
#include <SoftSPI.h>



/*
This sketch receives strings from sending unit via nrf24 
and prints them out via serial.  The sketch waits until
it receives a specific value (2 in this case), then it 
prints the complete message and clears the message buffer.

The sketch serves the result to a webpage.
*/
int messageLength = 12;  //message checksum, Make sure to 
//  change it if your string is a different length

int msg[1];
RF24 radio(6,7);
const uint64_t pipe = 0xE8E8F0F0E1LL;
String theMessage = ""; 
String latestMessage = "";  //this is used in the html output

//Ethernet setup...
byte mac[] = { 
  0xDE, 0xAD, 0x00, 0xCC, 0xFE, 0xAA };
EthernetServer server(80);


void setup(void){
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
  Ethernet.begin(mac);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}
void loop(void){
  if (radio.available()){
    bool done = false;  
      done = radio.read(msg, 1); 
      char theChar = msg[0];
      if (msg[0] != 2){
        theMessage.concat(theChar);
        }
      else {
       
       if (theMessage.length() == messageLength) { 
       Serial.println(theMessage);
       latestMessage = theMessage;
       }
       theMessage= ""; 
      }
   }
 //BEGINNING OF SERVER CODE  
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connnection: close");
          client.println();
          client.println("<!DOCTYPE html>");
		  client.println("<html>");
		  client.println("<head>");
		  client.println("<title>Arduino Web Page</title>");
		  client.println("</head>");
		  client.println("<body>");
		  client.print("<h1>The last message we got was: ");
          client.print(latestMessage);      
          client.println("</h1>");
		  client.println("</body>");
		  client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
 //END OF SERVER CODE
   
}