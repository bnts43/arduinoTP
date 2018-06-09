int Entree_Analog = A0; // valeur relative calculée
int Entree_Numerique = 3; // renvoie si la valeur calculée par A0 dépasse la valeur maximale autorisée

int Led_Red = 10;
int Led_Green = 11;
boolean ledIsRed = true;

String DISTANT_SH = "0013A200";
String DISTANT_SL = "417C250F";

float Analog;
int Digital;

String at_COMMANDS[20];

String response;

String Wandy="24A4";

int potentioCurrentValue = 255;

// communiquer avec le module Xbee distant en initialisant avec commandes AT
// ne pas oublier de clôturer la communication
// on ne peut pas envoyer la commande suivante AT sans avoir reçu le OK
// c'est séquentiel !

void setup ()
{
    pinMode (Entree_Analog, INPUT);
    pinMode (Entree_Numerique, INPUT);
    pinMode (Led_Red, OUTPUT); 
    pinMode (Led_Green, OUTPUT); 
    
    Serial.begin (9600); // Serial output with 9600 bps
    
    switchLeds();

    configuration("Automatique");
    
    connectDistantXBee();

}

void loop ()
{
      // switchLeds();

      // Current values will be read from digital and analog entries
      Analog = analogRead (Entree_Analog)  * (5.0 / 1023.0);
      // http://sensorkit.en.joy-it.net/index.php?title=KY-037_Microphone_sensor_module_(high_sensitivity)
      Digital = digitalRead (Entree_Numerique);
        
      //... and printed here
      // Serial.print ("Valeur analogique lue (en V) : "); 
      // Serial.println ("Neuts ; "); 
      // Serial.println (Analog, 4); 
      // Serial.print ("Valeur extrême : ");
      // Serial.println (Digital);
      // Serial.println ("----------------------------------------------------------------");
      // delay (50);
      // delay(5000);
      while(!Serial.available()) {
        Serial.println("Hello World");
        delay(300);
        }
      printReceivedResponse();
      Serial.println("Coucou Wandy");
      // while(!Serial.available()) {} // Don't read unless
      //  you know there is data
     
}

bool checkResponse() {
  response = "";
  while(!Serial.available()) {} // Don't read unless
  // you know there is data
  
  response = Serial.readString();
  Serial.println("Response: " + response);
  return (response.indexOf("OK") >= 0);
}

void printReceivedResponse() {
  response = "";
  while(!Serial.available()) {} 
  response = Serial.readString();
  Serial.println("New message: " + response);
  switchLeds();
}

void connectDistantXBee() {

  Serial.print("+++");
  delay(3000);
  if (checkResponse()) {
    switchLeds();
    for (int at = 0; at< 20; at++) {
      switchLeds();
      if (at_COMMANDS[at]!="") {
        printAT(at_COMMANDS[at]);
        if (!checkResponse() && at_COMMANDS[at]!="ATAI") {
          Serial.println("Unable to connect :-(");
        } else {
          if (at_COMMANDS[at]!="ATAI") {
            switchLeds();
            delay(400);
          }
        }
      }
    }
    Serial.println("ATCN");
    if (!checkResponse()) {
        Serial.println("Unable to connect :-(");
    } else {
        switchLeds();
    }
  }
}

void printAT(String AT_comm) {
    if (AT_comm.equals("+++")) {
      Serial.print(AT_comm);
    } else {
      Serial.println(AT_comm);
    }
}

void switchLeds() {
  if (ledIsRed) {
    digitalWrite(Led_Green,LOW); // LED will be switched off
    delay(400);
    digitalWrite(Led_Red,HIGH); // LED will be switched on
    delay(400);
  } else {
    digitalWrite(Led_Red,LOW); // LED will be switched on
    delay(400);
    digitalWrite(Led_Green,HIGH); // LED will be switched off    
    delay(400);
  }
    ledIsRed = !ledIsRed;

}

void configuration(String type) {
  if (type = "Manuelle") {
    at_COMMANDS[0] = "ATCH C";
    at_COMMANDS[1] = "ATRE";
    at_COMMANDS[2] = "ATID 4D37";
    at_COMMANDS[3] = "ATDH 0000";
    at_COMMANDS[4] = "ATDL FFFF";
    at_COMMANDS[5] = "ATMY 24A3";    
    at_COMMANDS[6] = "ATNI 24A3";
    at_COMMANDS[7] = "ATWR";
  } 
  if (type = "Automatique") {
    at_COMMANDS[0] = "ATRE";
    at_COMMANDS[1] = "ATA1 7";
    at_COMMANDS[2] = "ATNI 24A3";
    at_COMMANDS[3] = "ATMY 24A3";
    at_COMMANDS[4] = "ATWR";
    at_COMMANDS[5] = "ATAI";    
    at_COMMANDS[6] = "ATCN";
    at_COMMANDS[7] = "+++";
    at_COMMANDS[8] = "ATDH "+DISTANT_SH;
    at_COMMANDS[9] = "ATDL "+DISTANT_SL;
    at_COMMANDS[10] = "ATWR";
  }    

}
