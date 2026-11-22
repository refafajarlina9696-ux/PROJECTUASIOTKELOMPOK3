#include <Servo.h>
#include <SoftwareSerial.h>

// Komunikasi Serial ke ESP-12E (RX = Pin 12, TX = Pin 13)
SoftwareSerial espSerial(12, 13);

// Definisi Pin
const int TRIG_PIN   = 9;
const int ECHO_PIN   = 10;
const int SOUND_PIN  = 2;
const int FLAME_PIN  = 3;

const int LED_RED    = 4;
const int LED_YELLOW = 5;
const int LED_GREEN  = 6;
const int SERVO_PIN  = 11;

Servo palangServo;

// Parameter Miniatur
const int SUDUT_TUTUP = 0;
const int SUDUT_BUKA  = 90;
const unsigned long JEDA_TUTUP = 1000;
const int JARAK_DEKAT = 7;
const int JARAK_SIAGA = 12;

int posisiServoSekarang = -1; // Menyimpan sudut terakhir agar tidak gerak berulang
bool palangTerbuka = false;
bool sedangHitungMundur = false;
unsigned long waktuObjekHilang = 0;
unsigned long waktuKirimTerakhir = 0;

// Gerakkan servo hanya saat sudut berubah, lalu lepas (detach) agar tidak kejang
void gerakkanServo(int sudutTarget) {
  if (posisiServoSekarang != sudutTarget) {
    palangServo.attach(SERVO_PIN);
    palangServo.write(sudutTarget);
    delay(400); // Beri waktu servo sampai ke posisi tujuan
    palangServo.detach(); // Lepas sinyal agar tidak bentrok dengan SoftwareSerial
    posisiServoSekarang = sudutTarget;
  }
}

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(SOUND_PIN, INPUT);
  pinMode(FLAME_PIN, INPUT);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  gerakkanServo(SUDUT_TUTUP);
}

long bacaJarakCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long durasi = pulseIn(ECHO_PIN, HIGH, 30000);
  if (durasi == 0) return 999;
  return durasi * 0.034 / 2;
}

void aturLED(bool merah, bool kuning, bool hijau) {
  digitalWrite(LED_RED, merah);
  digitalWrite(LED_YELLOW, kuning);
  digitalWrite(LED_GREEN, hijau);
}

void kirimKeESP(long jarak, String statusPalang, String statusApi) {
  if (millis() - waktuKirimTerakhir >= 500) {
    waktuKirimTerakhir = millis();
    espSerial.print(jarak);
    espSerial.print(",");
    espSerial.print(statusPalang);
    espSerial.print(",");
    espSerial.println(statusApi);
  }
}

void loop() {
  int statusApi   = digitalRead(FLAME_PIN);
  int statusSuara = digitalRead(SOUND_PIN);
  long jarak      = bacaJarakCM();

  // PRIORITAS 1: DARURAT KEBAKARAN
  if (statusApi == LOW) {
    gerakkanServo(SUDUT_BUKA);
    aturLED(HIGH, LOW, LOW);
    kirimKeESP(jarak, "DARURAT TERBUKA", "BAHAYA API!");
    delay(200);
    aturLED(LOW, LOW, LOW);
    delay(200);
    return;
  }

  String teksPalang = "TERTUTUP";

  // LOGIKA PALANG TERBUKA
  if (palangTerbuka) {
    teksPalang = "TERBUKA";
    aturLED(LOW, LOW, HIGH);
    gerakkanServo(SUDUT_BUKA);

    if (jarak <= JARAK_SIAGA) {
      sedangHitungMundur = false;
    } else {
      if (!sedangHitungMundur) {
        sedangHitungMundur = true;
        waktuObjekHilang = millis();
      }
      if (millis() - waktuObjekHilang >= JEDA_TUTUP) {
        palangTerbuka = false;
        sedangHitungMundur = false;
        gerakkanServo(SUDUT_TUTUP);
        aturLED(HIGH, LOW, LOW);
        teksPalang = "TERTUTUP";
      }
    }
  } 
  // LOGIKA PALANG TERTUTUP
  else {
    if (jarak < JARAK_DEKAT) {
      if (statusSuara == HIGH) {
        palangTerbuka = true;
        sedangHitungMundur = false;
        aturLED(LOW, LOW, HIGH);
        gerakkanServo(SUDUT_BUKA);
        teksPalang = "TERBUKA";
      } else {
        aturLED(LOW, HIGH, LOW);
        gerakkanServo(SUDUT_TUTUP);
        teksPalang = "SIAGA (TUNGGU SUARA)";
      }
    } 
    else if (jarak >= JARAK_DEKAT && jarak <= JARAK_SIAGA) {
      aturLED(LOW, HIGH, LOW);
      gerakkanServo(SUDUT_TUTUP);
      teksPalang = "SIAGA (MENDEKAT)";
    } 
    else {
      aturLED(HIGH, LOW, LOW);
      gerakkanServo(SUDUT_TUTUP);
      teksPalang = "TERTUTUP";
    }
  }

  kirimKeESP(jarak, teksPalang, "AMAN");
  delay(20);
}
