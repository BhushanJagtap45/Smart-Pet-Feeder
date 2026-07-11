#include <WiFi.h>
#include <ESP32Servo.h>

const char* ssid = "WiFi_Name";
const char* password = "Wifi_Password";   

Servo feederServo;
int feedAngle = 60;
int feedCount = 0;
String lastFeed = "Never";
WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  feederServo.setPeriodHertz(50);
  feederServo.attach(13);
  feederServo.write(0);

  WiFi.begin(ssid, password);

  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected! IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {

  WiFiClient client = server.available();

  if (!client) {
    return;
  }

  while (client.connected() && !client.available()) {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  client.flush();
  if (request.indexOf("/set?angle=") != -1) {

  int start = request.indexOf("=") + 1;
  int end = request.indexOf(" ", start);

  feedAngle = request.substring(start, end).toInt();

  if (feedAngle < 30) feedAngle = 30;
  if (feedAngle > 90) feedAngle = 90;
}
if (request.indexOf("/feed") != -1) {

  feederServo.write(feedAngle);
  delay(1500);
  feederServo.write(0);

  feedCount++;
  lastFeed = "Just Now";
}
  client.println("HTTP/1.1 200 OK");
client.println("Content-type:text/html");
client.println();

client.println("<!DOCTYPE html>");
client.println("<html>");
client.println("<head>");
client.println("<meta charset='UTF-8'>");
client.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");
client.println("<title>Smart Pet Feeder Pro</title>");

client.println("<style>");
client.println("*{margin:0;padding:0;box-sizing:border-box;}");
client.println("body{font-family:Arial;background:linear-gradient(135deg,#00c6ff,#0072ff);display:flex;justify-content:center;align-items:center;height:100vh;}");
client.println(".card{background:rgba(255,255,255,0.15);backdrop-filter:blur(15px);padding:25px;border-radius:20px;width:360px;color:white;text-align:center;box-shadow:0 8px 30px rgba(0,0,0,.3);}");
client.println("h1{margin-bottom:15px;font-size:30px;}");
client.println(".status{color:#00ff88;font-weight:bold;margin-bottom:20px;}");
client.println("input[type=range]{width:100%;}");
client.println("button{margin-top:20px;background:#00c853;color:white;border:none;padding:15px 35px;font-size:20px;border-radius:12px;cursor:pointer;}");
client.println("button:hover{background:#00e676;}");
client.println("@keyframes glow{0%{box-shadow:0 0 5px #00ff99;}50%{box-shadow:0 0 25px #00ff99;}100%{box-shadow:0 0 5px #00ff99;}}");
client.println("button{animation:glow 2s infinite;}");
client.println(".footer{margin-top:20px;font-size:14px;opacity:0.9;}");
client.println("</style>");

client.println("</head>");
client.println("<body>");

client.println("<div class='card'>");
client.println("<h1>🐶 Smart Pet Feeder</h1>");
client.println("<div class='status'>🟢 WiFi Connected</div>");

client.println("<h3>🍖 Food Quantity</h3>");
client.println("<form action='/set'>");
client.println("<input type='range' min='30' max='90' value='60' name='angle'>");
client.println("<br><br>");
client.println("<input type='submit' value='Save Quantity' style='background:#ff9800;color:white;padding:10px 25px;border:none;border-radius:8px;'>");
client.println("</form>");

client.println("<br><br>");

client.println("<a href='/feed'><button>🍽 Feed Now</button></a>");

client.println("<br><br>");
client.print("<h3>📊 Feed Count : ");
client.print(feedCount);
client.println("</h3>");

client.print("<h3>⏰ Last Feed : ");
client.print(lastFeed);
client.println("</h3>");
client.println("<h3 style='color:#00ff88;'>🟢 ESP32 Online</h3>");
client.println("<div class='footer'>");
client.println("🐾 Smart Pet Feeder Pro<br>");
client.println("Developed by <b>Bhushan Jagtap</b>");
client.println("</div>");

client.println("</div>");
client.println("</body>");
client.println("</html>");

client.stop();
  }

  

