#include <WiFi.h>
#include <ESP32Servo.h>

const char* ssid = "Jagtap WiFi";
const char* password = "Bhushan@4321";

Servo feederServo;
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

  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {

  WiFiClient client = server.available();

  if (client) {

    String request = client.readStringUntil('\r');
    client.flush();

    if (request.indexOf("/FEED") != -1) {

      feederServo.write(90);
      delay(1500);
      feederServo.write(0);

    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type:text/html");
    client.println();

    client.println("<html>");
    client.println("<head><title>Smart Pet Feeder</title></head>");
    client.println("<body style='text-align:center;font-family:Arial;'>");
    client.println("<h1>🐶 Smart Pet Feeder</h1>");
    client.println("<a href='/FEED'><button style='font-size:30px;padding:20px;'>Feed Now</button></a>");
    client.println("</body>");
    client.println("</html>");

    delay(1);
    client.stop();

  }

}
