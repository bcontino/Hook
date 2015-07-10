 /*Created by Brandon Contino
  Created for the China-US Maker Competition
  Created on July 3, 2015
  This code hosts html pages on the Intel Edison based on inputs from external sources.
  In this specific code, push buttons are utilized to determine what pages are opened. This
  code does not currently use interrupts and would be much more effiicient after thier implementation;
  however this code was created for proof of concept.
  Last Modification on July 7, 2015
*/
#include <SPI.h>
#include <WiFi.h>

char ssid[] = "Your Network SSID";          //  your network SSID (name)
char pass[] = "Your Network Password";   // your network password
int status = WL_IDLE_STATUS;
int buttonVal = 0;
int button1;
int button2;
int button3;
int button4;
char c;

WiFiServer server(1000); //Using port 1000, random port but must be set to 
                         //a port other than the traditional 80 because there is
                         //is a conflict with the Edison's prebuilt code. To access
                         //different port type: IPAddress:Port. E.g. 192.168.1.1:1000

void setup() {
  // initialize serial:
  pinMode(2, INPUT); //button1
  pinMode(4, INPUT); //button2
  pinMode(6, INPUT); //button3
  pinMode(8, INPUT); //button4
  Serial.begin(115200); //baud rate of 115200
  Serial.println("Attempting to connect to WPA network...");
  Serial.print("SSID: ");
  Serial.println(ssid);


  status = WiFi.begin(ssid, pass);
  if ( status != WL_CONNECTED) {
    Serial.println("Couldn't get a wifi connection");
    while (true);
  }
  else {
    server.begin();
    Serial.println("Connected to Network");
    printWifiStatus();

  }
}

void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
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
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 2");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head><style>body{background-color:#FAFAFA; width:75%; margin:auto auto; padding: 0 0; height:auto; overflow:auto;}h1{color:#171340; size:20%; font:""Helvetica"" align:center} h2{color:#171340; size:10%; font:""Helvetica"" align:center} img{text-align:center; display:block;}</style></head>");

          //Uncomment for Debugging
          // output the value of each analog input pin
          //client.print("Value of buttonVal is ");
          //client.println(buttonVal);
          if(buttonVal == 0){ //starting page
            client.println("<h1>Welcome to Hook's Webpage for Biological Thermodynamics!</h1>");
            client.println("<img src=""http://ecx.images-amazon.com/images/I/51T-elU0LtL._SX258_BO1,204,203,200_.jpg"" id=""BookTitle"">");
            client.println("<h2>Flip a Page to Begin</h2>");
          }
          if(buttonVal == 1){ //button1
            client.println("<h2>Here is the enlarged graph for your viewing pleasure</h2>");
            client.println("<img src= ""http://chemwiki.ucdavis.edu/@api/deki/files/1618/Reaction_Coordinate_%281%29.gif"">");
          }

          if(buttonVal == 2){ //button2
            client.println("<h1>Here is a list of articles that pertain to this page:</br> </h2>"); 
            client.println("<a href=""http://arxiv.org/pdf/physics/0012036.pdf""><h2>Biology and Thermodynamics: Seemingly Opposite Phenomena in Search of a Unified Paradigm</h2></a>");
            client.println("<a href=""http://newt.phys.unsw.edu.au/~jw/CellularThermodynamics.pdf""><h2>Cellular Thermodynamics: the molecular and macroscopic views</h2></a>"); 
            client.println("<a href=""http://link.springer.com/article/10.1007%2FBF00700438""><h2>Thermodynamics and Biological Evolution</h2></a>"); 
          }

          if(buttonVal == 3){ //button3
            client.println("<h2>Here's a photo from the kickoff event</h2>");
            client.println("<img src=""http://www.pitt.edu/~bmc95/Intel-China.jpg"" style=""width:90%;height:30%;"">");
          }

          if(buttonVal == 4){//button4
            client.println("<h2>Where we created this idea</h2>");
            client.println("<img src=""http://www.pitt.edu/~mam503/assets/img/space5.jpg"" style=""width:90%;height:40%;"">");
          }
          
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }

  //Check if any buttons are pressed
  button1 = digitalRead(2);
  button2 = digitalRead(4);
  button3 = digitalRead(6);
  button4 = digitalRead(8);

  //Assigns a pressed button to a variable that will maintain the value until another button is pressed
  if(button1 == 1){
    buttonVal = 1;
  }
  if(button2 == 1){
    buttonVal = 2;
  }
  if(button3 == 1){
    buttonVal = 3;
  }
  if(button4 == 1){
    buttonVal = 4;
  }

}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

