#include <WiFi.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

char ssid[] = "Wes";     //  your network SSID (name)
char pass[] = "12345678";    // your network password

int status = WL_IDLE_STATUS;     // the Wifi radio's status

char server[] = "agggiesinvent2.meteor.com"; 
//char server[] = "www.google.com";

// Initialize the Wifi client library
WiFiClient client;

// Define which pins you will use on the Arduino to communicate with your
// GPS. In this case, the GPS module's TX pin will connect to the
// Arduino's RXPIN which is pin 3.
#define RXPIN 3
#define TXPIN 5

// Set this value equal to the baud rate of your terminal program
#define TERMBAUD  9600

// Set this value equal to the baud rate of your GPS
#define GPSBAUD  4800

const int switchPin = 2; //pin number for push button
int switchState = 0; //Start off position (not sure this matters)
int Counter = 0; //Counts Number of times button has been pushed in cycles

// Create an instance of the TinyGPS object
TinyGPS gps;
// Initialize the NewSoftSerial library to the pins you defined above
SoftwareSerial uart_gps(RXPIN, TXPIN);

// This is where you declare prototypes for the functions that will be
// using the TinyGPS library.
void getgps(TinyGPS &gps);

// In the setup function, you need to initialize two serial ports; the
// standard hardware serial port (Serial()) to communicate with your
// terminal program an another serial port (NewSoftSerial()) for your
// GPS.
void setup()
{
  pinMode(switchPin, INPUT);
  // Sets baud rate of your terminal program
  Serial.begin(TERMBAUD);
  // Sets baud rate of your GPS
  uart_gps.begin(GPSBAUD);

 //WIFI SETUP
 // attempt to connect using WPA2 encryption:
  Serial.print("Attempting to connect to WPA network: ");
  Serial.println(ssid);
  status = WiFi.begin(ssid, pass);

  // If you're not connected, stop here:
  if ( status != WL_CONNECTED) {
    Serial.println("Couldn't get a wifi connection");
    while(true);
  }
  // if you are connected, print out info about the connection:
  else {
    Serial.print("Connected to: ");
    Serial.println(ssid);
  }

 //GPS SETUP
  Serial.println("");
  Serial.println("     ===== GPS Initialization =====     ");
  Serial.println("     =====  Waiting For Lock  =====     ");
  Serial.println("");
  delay(1000); //Delay used to ensure connection with satelites and resources.
}

// This is the main loop of the code. All it does is check for data on
// the RX pin of the ardiuno, makes sure the data is valid NMEA sentences,
// then jumps to the getgps() function.
void loop()
{
  switchState = digitalRead(switchPin);

  if (switchState == HIGH){
    Counter = 1;
    while(uart_gps.available())     // While there is data on the RX pin...
    {
        int c = uart_gps.read();    // load the data into a variable...
        if(gps.encode(c))      // if there is a new valid sentence...
        {
          getgps(gps);         // then grab the data.
        }
    }
  } else {
    //Serial.print("Not Pushed!!!: \n ");
  }
}

// The getgps function will get and print the values we want.
void getgps(TinyGPS &gps)
{
  // To get all of the data into varialbes that you can use in your code,
  // all you need to do is define variables and query the object for the
  // data. To see the complete list of functions see keywords.txt file in
  // the TinyGPS and NewSoftSerial libs.

  // Define the variables that will be used
  float latitude, longitude;
  // Then call this function
  gps.f_get_position(&latitude, &longitude);
  // You can now print variables latitude and longitude
  Serial.print("Lat/Long: ");
  Serial.print(latitude,5);
  Serial.print(", ");
  Serial.println(longitude,5);

  // Same goes for date and time
  int year;
  byte month, day, hour, minute, second, hundredths;
  gps.crack_datetime(&year,&month,&day,&hour,&minute,&second,&hundredths);
  // Print data and time
  Serial.print("Date: "); Serial.print(month, DEC); Serial.print("/");
  Serial.print(day, DEC); Serial.print("/"); Serial.print(year);
  Serial.print("  Time: "); Serial.print(hour, DEC); Serial.print(":");
  Serial.print(minute, DEC); Serial.print(":"); Serial.print(second, DEC);
  Serial.print("."); Serial.println(hundredths, DEC);
  //Since month, day, hour, minute, second, and hundr

  // Here you can print the altitude and course values directly since
  // there is only one value for the function
  Serial.print("Altitude (meters): "); Serial.println(gps.f_altitude());
  // Same goes for course
  Serial.print("Course (degrees): "); Serial.println(gps.f_course());
  // And same goes for speed
  Serial.print("Speed(kmph): "); Serial.println(gps.f_speed_kmph());
  //Serial.println();

  // Here you can print statistics on the sentences.
  unsigned long chars;
  unsigned short sentences, failed_checksum;
  gps.stats(&chars, &sentences, &failed_checksum);
  //Serial.print("Failed Checksums: ");Serial.print(failed_checksum);
  //Serial.println(); Serial.println();

  // Here you can print the number of satellites in view
  Serial.print("Satellites: ");
  Serial.println(gps.satellites());

  sendGPS(gps); //Runs code to Start looking for server
}

void sendGPS(TinyGPS &gps){
  Serial.print("Attempting to Connect to: ");
  Serial.println(server);

  if (client.connect(server, 80)){
    Serial.print("Connected to: ");
    Serial.println(server);
  
    delay(2000);

    String pass1 = "GET http://aggiesinvent2.meteor.com/test?latitude=30.0";
    String pass2 = "&longitude=-90.0 HTTP/1.1";
    client.println(pass1+pass2);
    client.println("Host: aggiesinvent2.meteor.com");
    client.println("User-Agent: Arduino/1.0");
    client.println("Connection: close");
    
    //client.println("GET /search?q=arduino HTTP/1.1");
    //client.println("Host: www.google.com");
    //client.println("Connection: close");


    Serial.println("Sent!");

    client.flush();
    client.stop();
    Serial.println("Client Disconnected");
    Serial.println("");

    /*
    while (client.connected()){
      if(client.available()) {
        Serial.println("Client Is Avaliable!");

        char c = client.read();
        Serial.write(c);
        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connnection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html><body>HELLO</body></html>");
          break;
        }
        if (c == '\n'){
          currentLineIsBlank = true;
        }
        else if (c != '\r'){
          currentLineIsBlank = false;
        }
      }
    }
    */
  }
  else {
    Serial.println("Sorry Unable to Connect To: ");
    Serial.println(server);
  }
}
