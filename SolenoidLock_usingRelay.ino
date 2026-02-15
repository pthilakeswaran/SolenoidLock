#include <WiFi.h>
#include <WebServer.h>

#define SOLENOID_PIN 25

WebServer server(80);
bool isLocked = true;

const char* ssid     = "";
const char* password = "";

String getPage() {
  String stateText = isLocked ? "LOCKED" : "UNLOCKED";
  String stateColor = isLocked ? "#ef4444" : "#10b981";
  String glowColor = isLocked ? "239, 68, 68" : "16, 185, 129";

  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width,initial-scale=1'>";
  html += "<title>Smart Lock Control</title><style>";
  html += "* { margin: 0; padding: 0; box-sizing: border-box; }";
  html += "body { font-family: 'Segoe UI', system-ui, sans-serif; display: flex; justify-content: center; align-items: center; min-height: 100vh; ";
  html += "background: linear-gradient(135deg, #0f172a 0%, #1e293b 50%, #334155 100%); color: white; padding: 20px; }";
  html += ".container { background: rgba(30, 41, 59, 0.8); backdrop-filter: blur(10px); padding: 40px; border-radius: 24px; ";
  html += "box-shadow: 0 20px 60px rgba(0,0,0,0.5), 0 0 40px rgba(" + String(glowColor) + ", 0.1); width: 100%; max-width: 400px; border: 1px solid rgba(255,255,255,0.1); }";
  html += "h1 { font-size: 32px; margin-bottom: 10px; text-align: center; font-weight: 600; letter-spacing: 1px; }";
  html += ".subtitle { text-align: center; color: #94a3b8; font-size: 14px; margin-bottom: 35px; letter-spacing: 0.5px; }";
  html += ".status-card { background: linear-gradient(135deg, rgba(255,255,255,0.05), rgba(255,255,255,0.02)); padding: 30px; border-radius: 16px; ";
  html += "margin-bottom: 30px; text-align: center; border: 1px solid rgba(255,255,255,0.05); position: relative; overflow: hidden; }";
  html += ".status-card::before { content: ''; position: absolute; top: 0; left: 0; right: 0; height: 4px; ";
  html += "background: " + stateColor + "; box-shadow: 0 0 20px rgba(" + glowColor + ", 0.5); }";
  html += ".status-label { font-size: 14px; color: #94a3b8; margin-bottom: 10px; letter-spacing: 1px; }";
  html += ".status-text { font-size: 36px; font-weight: 700; color: " + stateColor + "; letter-spacing: 2px; text-shadow: 0 0 20px rgba(" + glowColor + ", 0.4); }";
  html += ".controls { display: flex; gap: 15px; margin-top: 20px; }";
  html += ".btn { flex: 1; padding: 18px; font-size: 16px; font-weight: 600; border: none; border-radius: 12px; cursor: pointer; ";
  html += "transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1); text-decoration: none; display: flex; align-items: center; justify-content: center; color: white; letter-spacing: 1px; }";
  html += ".btn:hover { transform: translateY(-2px); box-shadow: 0 10px 25px rgba(0,0,0,0.3); }";
  html += ".btn:active { transform: translateY(0); }";
  html += ".btn-unlock { background: linear-gradient(135deg, #10b981, #059669); box-shadow: 0 4px 15px rgba(16, 185, 129, 0.3); }";
  html += ".btn-lock { background: linear-gradient(135deg, #ef4444, #dc2626); box-shadow: 0 4px 15px rgba(239, 68, 68, 0.3); }";
  html += ".footer { text-align: center; margin-top: 25px; padding-top: 20px; border-top: 1px solid rgba(255,255,255,0.05); color: #64748b; font-size: 12px; }";
  html += "@media (max-width: 480px) { .container { padding: 30px 20px; } .controls { flex-direction: column; } }";
  html += "</style></head><body>";
  html += "<div class='container'>";
  html += "<h1>SMART LOCK</h1>";
  html += "<div class='subtitle'>Remote Access Control</div>";
  html += "<div class='status-card'>";
  html += "<div class='status-label'>CURRENT STATE</div>";
  html += "<div class='status-text'>" + stateText + "</div>";
  html += "</div>";
  html += "<div class='controls'>";
  html += "<a href='/unlock' class='btn btn-unlock'>UNLOCK</a>";
  html += "<a href='/lock' class='btn btn-lock'>LOCK</a>";
  html += "</div>";
  html += "<div class='footer'>Connected - " + WiFi.localIP().toString() + "</div>";
  html += "</div></body></html>";
  
  return html;
}

void handleRoot() {
  server.send(200, "text/html", getPage());
}

void handleLock() {
  digitalWrite(SOLENOID_PIN, HIGH);
  isLocked = true;
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleUnlock() {
  digitalWrite(SOLENOID_PIN, LOW);
  isLocked = false;
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  pinMode(SOLENOID_PIN, OUTPUT);
  digitalWrite(SOLENOID_PIN, HIGH);

  Serial.println("\nSmart Lock System Starting...");
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    Serial.print("Access at: http://");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi Connection Failed!");
  }

  server.on("/", handleRoot);
  server.on("/lock", handleLock);
  server.on("/unlock", handleUnlock);
  server.begin();
  Serial.println("Web Server Started\n");
}

void loop() {
  server.handleClient();

}
