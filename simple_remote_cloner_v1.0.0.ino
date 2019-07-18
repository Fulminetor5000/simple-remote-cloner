
/*********************************************************************************

 simple-remote-cloner
 version 1.0.0 - 17 June 2019
  
 Written with love by: https://github.com/Fulminetor5000
  

 --- WARNING ---
 
 If the script doesn't work, try to change in IRremoteInt around the line 40
 the constant RAWBUF with a bigger value (at least 300),
 googling "irremote rawbuf" you can understand what it is.

*********************************************************************************/


#include <IRremote.h>
#include <EEPROM.h>


//IR transmitter
#define TRA_PIN 3

//IR recevier
#define REC_PIN 4

//Learning status led
#define LED_PIN 5

//Start and stop learning button
#define RESET_PIN 6

//Send IR message button
#define SEND_PIN 7


//Frequency of IR output message, in kHz
#define FREQUENCY 38


#define DEBOUNCE_TIME 400
#define WAIT_TIME 1000000


IRrecv recvIr(REC_PIN);
decode_results results;

IRsend sendIr;


bool resetPinState, sendPinState, flag;


void overwrite(decode_results *results) {
  int address = sizeof(int), rawlen = results -> rawlen;
  EEPROM.put(address, rawlen);
  for(int i=1; i < rawlen; i++) {
    address += sizeof(unsigned int);
    EEPROM.put(address, results -> rawbuf[i]*USECPERTICK);
  }
}

void sendIrMessage() {  
  int address = sizeof(int), rawlen;
  EEPROM.get(address, rawlen);
  rawlen--;
  unsigned int message[rawlen];
  for(int i=0; i < rawlen; i++) {    
    address += sizeof(unsigned int);
    EEPROM.get(address, message[i]);   
  }
  sendIr.sendRaw(message, sizeof(message) / sizeof(message[0]), FREQUENCY);  
}

void setup() { 
  pinMode(REC_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RESET_PIN, INPUT);
  pinMode(SEND_PIN, INPUT);
  recvIr.enableIRIn();  
}

void loop() { 
  resetPinState = digitalRead(RESET_PIN);
  sendPinState = digitalRead(SEND_PIN);
  
  if(resetPinState) {    
    delay(DEBOUNCE_TIME);
    recvIr.resume();
    digitalWrite(LED_PIN, HIGH);
    flag = true;
    for(long int i=0; i < WAIT_TIME && flag; i++) { 
      resetPinState = digitalRead(RESET_PIN);
      if(recvIr.decode(&results)) {       
        flag = false;
        overwrite(&results);               
      }
      if(resetPinState) {       
        delay(DEBOUNCE_TIME);
        flag = false;   
      }      
    }
    digitalWrite(LED_PIN, LOW);
  }
  
  if(sendPinState) {    
    sendIrMessage();
    recvIr.enableIRIn();
    delay(DEBOUNCE_TIME);    
  }  
}
