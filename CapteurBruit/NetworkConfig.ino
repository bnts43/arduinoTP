// communicate with Xbee module 
// initializing begins with AT commands
// from '+++' to 'AT CN' ATCN which closes the configuration phase
// AT configuration is sequential, which means we'll have to 
// wait for 'OK' response

// this array will store AT commands 
// you may use your owns
String at_COMMANDS[50];

// response read from coordinator
String response;

// external library useful for exiting infinite loop
VirtualDelay singleDelay; // default = millis

// providing the ride type of configuration, 
// Personal is a quick setup
// AutoAndConnect will connect an encrypted network using defined key 
// 
void configuration(String type) {
  if (type = "Personal") {
    at_COMMANDS[0] = "ATCH C"; // Set or read the operating channel devices
    at_COMMANDS[1] = "ATRE"; // Restore device parameters to factory defaults
    at_COMMANDS[2] = "ATID 4D37"; // Set or read the Personal Area Network (PAN) ID
    at_COMMANDS[3] = "ATDH 0000"; // Set or read the upper 32 bits of the 64-bit destination address
    at_COMMANDS[4] = "ATDL FFFF"; // Set or display the lower 32 bits of the 64-bit destination address
    // To transmit using a 16-bit address, we set DH parameter to zero and DL less than 0xFFFF.
    // Doing so, it enables broadcasting 
    at_COMMANDS[5] = "ATMY "+MY_INFO; // Sets or displays the device's 16-bit source address
    at_COMMANDS[6] = "ATNI "+MY_INFO; // Stores the node identifier string for a device (user-defined)
    at_COMMANDS[7] = "ATWR"; // Writes parameter values to non-volatile memory
  } 
   
  if (type = "AutoAndConnect") {
    at_COMMANDS[0] = "ATRE";  // see above
    at_COMMANDS[1] = "ATA1 7";  //Sets the End Device association options 
                                // 7 should enable association with anything
    at_COMMANDS[2] = "ATEE 1"; // Enables or disables 128-bit Advanced Encryption Standard (AES) encryption
    at_COMMANDS[3] = "ATKY "+NETWORK_KEY; // Sets the 128-bit AES link key value that the device uses for encryption and decryption.
    at_COMMANDS[4] = "ATNI "+MY_INFO; // see above
    at_COMMANDS[5] = "ATMY "+MY_INFO; // see above
    at_COMMANDS[6] = "ATWR";  // see above
    at_COMMANDS[7] = "ATAI";   // Reads errors with the last association request -> awaited value = 0 
    at_COMMANDS[8] = "ATCN"; // Immediately exits Command Mode and applies pending changes
    at_COMMANDS[29] = "CONNECT_TO_PEER"; // non standard _ used to add a delay before starting peer association
    at_COMMANDS[30] = "+++"; // beginning transaction
    at_COMMANDS[31] = "ATDH "+DISTANT_SH; // Set or read the upper 32 bits of the 64-bit destination address of peer 
    at_COMMANDS[32] = "ATDL "+DISTANT_SL; // Set or read the upper 32 bits of the 64-bit destination address of peer
    at_COMMANDS[33] = "ATWR";   // see above
  }
}

// returns true if connection was successful
bool connectDistantXBee() {
  // beginning configuration
  Serial.print("+++");
  // have to wait for coordinator's answer
  delay(3000);
  // read first response from '+++'
  if (checkResponse()) {
    switchLeds();
    // will parse array with AT commands
    for (int at = 0; at< 50; at++) {
      if (at_COMMANDS[at]!="") {
        boolean shallConnect = true;
        switchLeds();
        printAT(at_COMMANDS[at]); // print command
        // add a delay if AT command is +++
        if (at_COMMANDS[at] == "+++") {
          delay(3000);
        }
        
        if (at_COMMANDS[at] == "CONNECT_TO_PEER") {
          delay(10000);
          shallConnect = false;
        }
        if (shallConnect) {
          
          if (!checkResponse() && at_COMMANDS[at]!="ATAI") {
            Serial.println("Unable to connect :-(");
          } else {
            if (at_COMMANDS[at]!="ATAI" ) {
              switchLeds();
              delay(400);
            }
          }
        }
      }
      if (at_COMMANDS[at] == "ATCN") {
        switchLeds();
        delay(400);
      }
    }
    Serial.println("ATCN");
    if (!checkResponse()) {
        Serial.println("Unable to connect :-(");
    } else {
        switchLeds();
    }

  } else {
    Serial.println("Unable to start network configuration.");
    return false;
  }
  return true;
}


// will read AT response from coordinator 
// true if response was OK
// or false otherwise
bool checkResponse() {
  VirtualDelay singleDelay;
  singleDelay.start(4000);
  // singleDelay will help exit infinite loop while
  while(!Serial.available()) {
    if(singleDelay.elapsed()) return false;
  }  
  // read and save response from coordinator and store it in response
  response = "";
  response = Serial.readString();
  // test if response contains OK and returns true or false depending on response
  return (response.indexOf("OK") >= 0);
}


// will print AT command 
// with +++ print will not end with a new line
void printAT(String AT_comm) {
    if (AT_comm.equals("+++")) {
      Serial.print(AT_comm);
    } else {
      Serial.println(AT_comm);
    }
}
