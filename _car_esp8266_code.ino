#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define ENA   D5
#define ENB   D6
#define IN_1  D7
#define IN_2  D8
#define IN_3  D1
#define IN_4  D2

ESP8266WebServer server(80);

// 🔐 Enter your WiFi details here
const char* ssid = "Sai";
const char* password = "12345678";

int speedCar = 800;

// ============ MOTOR FUNCTIONS ============

void goAhead(){
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar);
}

void goBack(){
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

void goRight(){
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar);
}

void goLeft(){
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

void stopRobot(){
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
}

// ============ WEB PAGE ============

void handleRoot() {

  if (server.hasArg("State")) {
    String command = server.arg("State");

    if (command == "F") goAhead();
    else if (command == "B") goBack();
    else if (command == "L") goLeft();
    else if (command == "R") goRight();
    else if (command == "S") stopRobot();
    else if (command == "0") speedCar = 400;
    else if (command == "1") speedCar = 600;
    else if (command == "2") speedCar = 800;
    else if (command == "3") speedCar = 1023;
  }

  String page = "<html><head><title>WiFi Car</title></head><body>";
  page += "<h2 align='center'>NodeMCU WiFi Car</h2>";
  page += "<div align='center'>";
  page += "<a href='?State=F'><button style='width:100px;height:50px;'>Forward</button></a><br><br>";
  page += "<a href='?State=L'><button style='width:100px;height:50px;'>Left</button></a>";
  page += "<a href='?State=S'><button style='width:100px;height:50px;'>Stop</button></a>";
  page += "<a href='?State=R'><button style='width:100px;height:50px;'>Right</button></a><br><br>";
  page += "<a href='?State=B'><button style='width:100px;height:50px;'>Back</button></a><br><br>";
  page += "<h3>Speed Control</h3>";
  page += "<a href='?State=0'><button>Low</button></a>";
  page += "<a href='?State=1'><button>Medium</button></a>";
  page += "<a href='?State=2'><button>High</button></a>";
  page += "<a href='?State=3'><button>Max</button></a>";
  page += "</div></body></html>";

  server.send(200, "text/html", page);
}

// ============ SETUP ============

void setup() {

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

// ============ LOOP ============

void loop() {
  server.handleClient();
}