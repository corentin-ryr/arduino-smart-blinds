// Import required libraries
#ifdef ESP32
  #include <WiFi.h>

#else
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>

#endif

#include "WiFiTerm.h"
#include <FS.h>


// Replace with your network credentials
const char* ssid = "TP-Link_1C96";
const char* password = "10502167";

// Set web server port number to 80
ESP8266WebServer server;
ESP8266WebServer serverStatic(82);


String buttonNames[] = {"cuisine", "salon"};
bool buttonStates[] = {true, false};


/**********
    setup
 ***********/

void setup() {
  Serial.begin(115200);
  Serial.print("\nConnecting to WIFI");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print('.');
  }
  Serial.println("connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("webServer started");

  term.begin(server);
  term.link(Serial);
  Serial.println("term started");


  //For the static server
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  serverStatic.serveStatic("/", SPIFFS, "/");    
  serverStatic.begin();
}



/**********
   programme
 ***********/

void loop() {

  server.handleClient();
  term.handleClient();

  if (term.available())
  {
    String message = term.readString();

    if (String(message[0]) == "1") { //Asking for all buttons with states and names
      String states = allStates();
      term.println("1 " + states);
    }
    else if(String(message[0]) == "2") { //Update for one button state (command is "2 btnName btnstate")
      setBtnState(message);
      term.println("2 ok");
    }
  }

  //For the static server
  serverStatic.handleClient();


}
/*******************************************
   fin prog principal
 ****************************************/

String allStates() {

  String states = "[";
  for (byte i = 0; i < (sizeof(buttonNames) / sizeof(buttonNames[0])); i++) {
    // do something with myValues[i]
    if (i > 0) { states += ",";}
    states += "{'name': '" + buttonNames[i] + "',  'state': " + String(buttonStates[i]) + "}";
  }
  states += "]";
  return states;
}

void setBtnState(String message) {
  
  char buf[message.length() + 1];
  strcpy(buf, message.c_str());
  //char *p = buf;
  char * pch = strtok (buf, " ");

  String btnName;
  bool btnState;
  int i = 0;
  while (pch != NULL)
  {
    //printf("%s\n",pch);
    if(i==1) {
      btnName = pch;
    }
    if(i==2) {
      btnState= (strcmp(pch, "true") == 0);
    }
    
    pch = strtok (NULL, " ");
    i++;
  }

  //Set the button state with its name
  for (byte i = 0; i < (sizeof(buttonNames) / sizeof(buttonNames[0])); i++) {
    // do something with myValues[i]
    if (buttonNames[i] == btnName) { 
      buttonStates[i] = btnState;
      break;
    }
  }
}


/* On update of one of the switches
*/
void onUpdate(String btnName, String btnState) {
  term.println("3 {'name':" + btnName + ", 'state':true}");
}
