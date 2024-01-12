#include <WiFi.h>
#include <WebServer.h>

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

const char* ssid = "Oneplus 9R";  // Enter SSID here
const char* password = "11111111";  // Enter Password here

const int trigPin = 5;
const int echoPin = 18;

WebServer server(80);

long duration;
float distanceCm;
float distanceInch;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(100);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  
  Serial.println("Connecting to ");
  Serial.println(ssid);

  // Connect to your local Wi-Fi network
  WiFi.begin(ssid, password);

  // Check if Wi-Fi is connected to the Wi-Fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.on("/getdistance", getDistance);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}

void handle_OnConnect() {
  server.send(200, "text/plain", "{'distance:23}");
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

void getDistance(){
  server.handleClient();
    // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(200);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(200);
  digitalWrite(trigPin, LOW);
  
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;

  String txt = "Distance = ";
  txt+= (String)distanceCm;
  
  // Convert to inches
  // distanceInch = distanceCm * CM_TO_INCH;
  
  // Prints the distance in the Serial Monitor
  // Serial.print("Distance (cm): ");
  // Serial.println(distanceCm);
  // Serial.print("Distance (inch): ");
  // Serial.println(distanceInch);
  //  Serial.println(duration);
  // delay(1000);
  server.send(200, "text/plain", txt);
}
