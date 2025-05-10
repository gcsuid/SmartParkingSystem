#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "IOT_DEVICES";       // WiFi Name
const char* password = "iot_lab_devices"; // WiFi Password

ESP8266WebServer server(80);

// IR Sensor Pins
#define sensor1 D7
#define sensor2 D5
#define sensor3 D3

void setup() {
    Serial.begin(115200);

    // Connect to WiFi as a station
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Set up server endpoints
    server.on("/", handleDashboard);
    server.on("/status", handleStatus);
    server.begin();

    // Set IR sensor pins as input
    pinMode(sensor1, INPUT);
    pinMode(sensor2, INPUT);
    pinMode(sensor3, INPUT);

    Serial.println("Smart Parking System Started");
}

void loop() {
    server.handleClient();  // Handle incoming client requests
}

void handleDashboard() {
    String html = "<html><head><title>Smart Parking System</title>";
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; text-align: center; background-color: #f4f4f9; }";
    html += "h1 { color: #333; }";
    html += "table { width: 50%; margin: 20px auto; border-collapse: collapse; }";
    html += "th, td { padding: 15px; border: 1px solid #ddd; }";
    html += "th { background-color: #4CAF50; color: white; }";
    html += ".status-icon { width: 50px; height: 50px; }";
    html += "</style>";
    html += "<script>";
    html += "function fetchStatus() {";
    html += "  var xhr = new XMLHttpRequest();";
    html += "  xhr.onreadystatechange = function() {";
    html += "    if (xhr.readyState == 4 && xhr.status == 200) {";
    html += "      document.getElementById('statusTable').innerHTML = xhr.responseText;";
    html += "    }";
    html += "  };";
    html += "  xhr.open('GET', '/status', true);";
    html += "  xhr.send();";
    html += "}";
    html += "setInterval(fetchStatus, 1000);";  // Fetch status every second
    html += "</script>";
    html += "</head><body>";
    html += "<h1>Smart Parking System Dashboard</h1>";
    html += "<table>";
    html += "<tr><th>Parking Slot</th><th>Status</th></tr>";
    html += "<tbody id='statusTable'>";
    html += "<tr><td>Slot 1</td><td>Loading...</td></tr>";
    html += "<tr><td>Slot 2</td><td>Loading...</td></tr>";
    html += "<tr><td>Slot 3</td><td>Loading...</td></tr>";
    html += "</tbody>";
    html += "</table>";
    html += "</body></html>";

    server.send(200, "text/html", html);
}

void handleStatus() {
    // Read sensor values
    int status1 = digitalRead(sensor1);
    int status2 = digitalRead(sensor2);
    int status3 = digitalRead(sensor3);

    // Image URLs for status (replace with your own if needed)
    String carImg = "<img src='https://cdn-icons-png.flaticon.com/512/1946/1946436.png' class='status-icon'>";  // Car icon
    String emptyImg = "<img src='https://cdn-icons-png.flaticon.com/512/33/33736.png' class='status-icon'>";  // Empty slot icon

    // Determine slot status with images
    String slot1 = (status1 == LOW) ? carImg : emptyImg;
    String slot2 = (status2 == LOW) ? carImg : emptyImg;
    String slot3 = (status3 == LOW) ? carImg : emptyImg;

    // Return the HTML for the status table with images
    String statusHTML = "<tr><td>Slot 1</td><td>" + slot1 + "</td></tr>";
    statusHTML += "<tr><td>Slot 2</td><td>" + slot2 + "</td></tr>";
    statusHTML += "<tr><td>Slot 3</td><td>" + slot3 + "</td></tr>";

    server.send(200, "text/html", statusHTML);
}
