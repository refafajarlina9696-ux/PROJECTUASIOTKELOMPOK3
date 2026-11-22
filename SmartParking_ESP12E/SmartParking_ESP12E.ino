#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>

// Ganti dengan nama Wi-Fi / Hotspot HP dan Password Anda
const char* ssid     = "Galaxy";
const char* password = "opo?????";

// Terima data dari Arduino di Pin D6 (RX), D5 (TX)
SoftwareSerial arduinoSerial(D6, D5);
ESP8266WebServer server(80);

String dataJarak  = "-";
String dataPalang = "Menunggu data...";
String dataApi    = "AMAN";

void tampilkanHalaman() {
  String warnaApi = (dataApi == "AMAN") ? "#28a745" : "#dc3545";
  String warnaPalang = (dataPalang.indexOf("TERBUKA") >= 0) ? "#28a745" : "#ffc107";
  if (dataPalang == "TERTUTUP") warnaPalang = "#dc3545";

  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<meta http-equiv='refresh' content='2'>";
  html += "<title>Pantau Parkir Pintar</title>";
  html += "<style>body{font-family:Arial,sans-serif;text-align:center;background:#f4f4f9;margin:20px;}";
  html += ".card{background:white;padding:20px;margin:15px auto;max-width:400px;border-radius:12px;box-shadow:0 4px 8px rgba(0,0,0,0.1);}";
  html += ".badge{display:inline-block;padding:10px 20px;color:white;font-weight:bold;border-radius:8px;font-size:18px;}</style></head><body>";

  html += "<h2>Smart Parking Monitor</h2>";

  html += "<div class='card'><h3>Status Gerbang</h3>";
  html += "<div class='badge' style='background:" + warnaPalang + ";'>" + dataPalang + "</div></div>";

  html += "<div class='card'><h3>Jarak Kendaraan</h3>";
  html += "<p style='font-size:28px;font-weight:bold;margin:5px;'>" + dataJarak + " cm</p></div>";

  html += "<div class='card'><h3>Sensor Keamanan Api</h3>";
  html += "<div class='badge' style='background:" + warnaApi + ";'>" + dataApi + "</div></div>";

  html += "<p style='color:gray;font-size:12px;'>Pembaruan otomatis setiap 2 detik</p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  arduinoSerial.begin(9600);

  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi Terhubung!");
  Serial.print("Alamat IP: http://");
  Serial.println(WiFi.localIP());

  server.on("/", tampilkanHalaman);
  server.begin();
}

void loop() {
  server.handleClient();

  if (arduinoSerial.available()) {
    String masuk = arduinoSerial.readStringUntil('\n');
    masuk.trim();

    int koma1 = masuk.indexOf(',');
    int koma2 = masuk.lastIndexOf(',');

    if (koma1 > 0 && koma2 > koma1) {
      dataJarak  = masuk.substring(0, koma1);
      dataPalang = masuk.substring(koma1 + 1, koma2);
      dataApi    = masuk.substring(koma2 + 1);
    }
  }
}
