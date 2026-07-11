#include <WiFi.h>
#include <ESP32Servo.h>

// Replace with your network credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Create a web server on port 80
WiFiServer server(80);

// Servo setup
Servo myservo;
const int servoPin = 13;
int servoAngle = 90;  // Default angle

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected.");
  Serial.print("ESP32 IP address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();

  // Attach servo
  myservo.attach(servoPin);
  myservo.write(servoAngle);  // Initial position
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    // Parse angle from request
    if (request.indexOf("/servo?angle=") != -1) {
      int angleIndex = request.indexOf('=') + 1;
      int angle = request.substring(angleIndex).toInt();
      if (angle >= 0 && angle <= 180) {
        servoAngle = angle;
        myservo.write(servoAngle);
      }
    }

    // HTML response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();

    client.println("<!DOCTYPE html><html>");
    client.println("<head><meta name='viewport' content='width=device-width, initial-scale=1'>");
    client.println("<style>");
    client.println("body { background-color: #0f172a; color: #f1f5f9; font-family: 'Segoe UI', sans-serif; text-align: center; padding-top: 50px; }");
    client.println(".slider { width: 80%; }");
    client.println("h1 { color: #38bdf8; }");
    client.println(".footer { position: fixed; bottom: 10px; width: 100%; text-align: center; font-weight: bold; color: #60a5fa; font-size: 16px; }");
    client.println("</style></head>");
    
    client.println("<body>");
    client.println("<h1>ESP32 Servo Control</h1>");
    client.print("<p>Angle: <span id='val'>");
    client.print(servoAngle);
    client.println("</span>°</p>");
    client.println("<input type='range' min='0' max='180' value='" + String(servoAngle) + "' class='slider' id='servoRange'>");

    client.println("<script>");
    client.println("let slider = document.getElementById('servoRange');");
    client.println("let val = document.getElementById('val');");
    client.println("slider.oninput = function() {");
    client.println("val.innerHTML = this.value;");
    client.println("fetch('/servo?angle=' + this.value);");
    client.println("}");
    client.println("</script>");

    client.println("<div class='footer'>ROBOARMY</div>");
    client.println("</body></html>");

    client.stop();
  }
}
