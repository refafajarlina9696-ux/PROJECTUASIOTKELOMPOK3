# 🚗 Smart Parking — Arduino Uno + ESP-12E

## 📌 Deskripsi

**Smart Parking** adalah sistem parkir pintar berbasis **Internet of Things (IoT)** yang menggunakan **Arduino Uno** sebagai pengendali utama dan **ESP-12E (NodeMCU)** sebagai modul komunikasi Wi-Fi.

Sistem ini dirancang untuk mendeteksi keberadaan kendaraan, mengontrol palang parkir secara otomatis, mendeteksi kondisi darurat akibat api, serta menampilkan informasi kondisi parkir melalui halaman web.

## 🎯 Tujuan

Sistem Smart Parking dibuat untuk:

- Mendeteksi kendaraan yang mendekati area parkir.
- Membuka palang parkir menggunakan servo berdasarkan kondisi kendaraan dan sensor suara.
- Menutup palang secara otomatis setelah kendaraan meninggalkan area.
- Mendeteksi kondisi darurat menggunakan sensor api.
- Memberikan indikator kondisi melalui LED.
- Mengirim data dari Arduino Uno ke ESP-12E.
- Menampilkan status parkir melalui halaman web yang dapat diakses menggunakan jaringan Wi-Fi.

---

## 🧩 Komponen yang Digunakan

| No. | Komponen | Fungsi |
|---|---|---|
| 1 | Arduino Uno | Pengendali utama sistem |
| 2 | ESP-12E / NodeMCU | Komunikasi Wi-Fi dan web monitoring |
| 3 | HC-SR04 | Mendeteksi jarak kendaraan |
| 4 | KY-038 | Mendeteksi suara |
| 5 | Flame Sensor | Mendeteksi api |
| 6 | Servo SG90 | Menggerakkan palang parkir |
| 7 | LED Merah | Indikator kondisi tertutup/bahaya |
| 8 | LED Kuning | Indikator kondisi siaga |
| 9 | LED Hijau | Indikator palang terbuka |
| 10 | Baterai AA 6V | Sumber daya servo |

---

## 🔌 Konfigurasi Pin Arduino Uno

| Komponen | Pin Arduino Uno |
|---|---|
| Sensor Suara KY-038 DO | D2 |
| Sensor Api DO | D3 |
| LED Merah | D4 |
| LED Kuning | D5 |
| LED Hijau | D6 |
| HC-SR04 TRIG | D9 |
| HC-SR04 ECHO | D10 |
| Servo SG90 Signal | D11 |
| Komunikasi ke ESP-12E TX | D13 |
| Komunikasi ke ESP-12E RX | D12 |

Konfigurasi pin tersebut mengikuti source code Arduino Uno yang digunakan pada proyek.

---

## 📡 Konfigurasi ESP-12E

ESP-12E menggunakan komunikasi serial melalui:

| Fungsi | Pin ESP-12E |
|---|---|
| RX | D6 |
| TX | D5 |
| Serial Monitor | 115200 baud |

Komunikasi antara Arduino Uno dan ESP-12E menggunakan baud rate **9600 baud**.

ESP-12E digunakan untuk menghubungkan sistem ke Wi-Fi dan menyediakan halaman monitoring berbasis web.

---

## 🔋 Pengkabelan Daya

- Sensor HC-SR04, KY-038, dan Flame Sensor:
  - VCC → 5V Arduino
  - GND → GND Arduino
- Servo SG90:
  - Kabel merah (+) → 6V dari 4× baterai AA
  - Kabel cokelat (-) → GND baterai
- ESP-12E:
  - Dapat menggunakan Micro-USB terpisah atau sumber 5V melalui VIN.
- **Semua GND wajib disatukan**, yaitu:
  - GND Arduino Uno
  - GND baterai servo
  - GND ESP-12E

Konfigurasi daya ini mengikuti panduan perakitan proyek.

---

# ⚙️ Cara Kerja Sistem

### 1. Kondisi Normal

Ketika tidak terdapat kendaraan yang mendekati sensor:

**LED Merah → ON**

Status:

> TER TUTUP

Palang berada pada posisi tertutup.

### 2. Kendaraan Mendekat

HC-SR04 membaca jarak kendaraan.

Jika jarak kendaraan berada pada rentang **7–12 cm**, sistem masuk ke kondisi:

> SIAGA (MENDEKAT)

LED kuning menyala.

### 3. Kendaraan Sangat Dekat

Jika jarak kendaraan **kurang dari 7 cm**, sistem akan menunggu sensor suara.

Jika sensor suara mendeteksi suara:

> Palang terbuka

Servo bergerak dari **0° → 90°** dan LED hijau menyala.

### 4. Kendaraan Meninggalkan Area

Ketika palang terbuka dan kendaraan sudah berada lebih jauh dari **12 cm**, sistem mulai menghitung waktu.

Setelah sekitar **1 detik**, palang akan ditutup kembali.

Servo bergerak:

**90° → 0°**

### 5. Kondisi Darurat Api 🔥

Sensor api mempunyai prioritas paling tinggi.

Jika sensor mendeteksi api:

> **BAHAYA API!**

Maka:

- Palang otomatis terbuka.
- Servo bergerak ke 90°.
- LED merah memberikan indikator bahaya.
- Status dikirim ke ESP-12E.

---

# 🌐 Web Monitoring

ESP-12E menyediakan halaman web bernama:

**Smart Parking Monitor**

Informasi yang ditampilkan:

### 🚧 Status Gerbang

Menampilkan kondisi palang:

- TERBUKA
- TERTUTUP
- SIAGA
- DARURAT TERBUKA

### 📏 Jarak Kendaraan

Menampilkan hasil pembacaan HC-SR04 dalam satuan:

**cm**

### 🔥 Sensor Keamanan Api

Menampilkan:

- **AMAN**
- **BAHAYA API!**

Halaman web melakukan pembaruan otomatis setiap **2 detik**.

---

# 📁 Struktur Repository

```text
Smart-Parking/
│
├── README.md
│
├── SmartParking_ArduinoUno/
│   └── SmartParking_ArduinoUno.ino
│
└── SmartParking_ESP12E/
    └── SmartParking_ESP12E.ino
```

## 📄 Source Code

### Arduino Uno

File:

```text
SmartParking_ArduinoUno/SmartParking_ArduinoUno.ino
```

Upload file ini ke board **Arduino Uno**.

Source code menggunakan library:

```cpp
#include <Servo.h>
#include <SoftwareSerial.h>
```

### ESP-12E

File:

```text
SmartParking_ESP12E/SmartParking_ESP12E.ino
```

Upload file ini ke board:

**NodeMCU 1.0 (ESP-12E Module)**

Library yang digunakan:

```cpp
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
```

---

# 📶 Konfigurasi Wi-Fi

Pada program ESP-12E terdapat konfigurasi:

```cpp
const char* ssid     = "Galaxy";
const char* password = "opo?????";
```

**Ganti SSID dan password sesuai Wi-Fi atau hotspot yang digunakan.**

Setelah ESP-12E berhasil terhubung, buka **Serial Monitor** dengan baud rate:

```text
115200
```

ESP-12E akan menampilkan alamat IP yang diperoleh.

Contoh:

```text
Wi-Fi Terhubung!
Alamat IP: http://192.168.x.x
```

Masukkan alamat IP tersebut ke browser yang terhubung pada jaringan Wi-Fi yang sama.

---

# 🔄 Format Komunikasi Data

Arduino Uno mengirimkan data ke ESP-12E dengan format:

```text
jarak,status_palang,status_api
```

Contoh:

```text
5,TERBUKA,AMAN
```

atau:

```text
15,TERTUTUP,AMAN
```

atau pada kondisi darurat:

```text
8,DARURAT TERBUKA,BAHAYA API!
```

ESP-12E kemudian memproses data tersebut dan menampilkannya pada halaman web.

---

# 🛠️ Cara Instalasi

## 1. Upload Program Arduino Uno

1. Buka **Arduino IDE**.
2. Buka file:

```text
SmartParking_ArduinoUno.ino
```

3. Pilih board:

```text
Arduino Uno
```

4. Pilih COM Port Arduino.
5. Klik **Upload**.

## 2. Upload Program ESP-12E

1. Buka Arduino IDE.
2. Buka:

```text
SmartParking_ESP12E.ino
```

3. Pilih board:

```text
NodeMCU 1.0 (ESP-12E Module)
```

4. Masukkan konfigurasi Wi-Fi.
5. Pilih COM Port ESP-12E.
6. Klik **Upload**.
7. Buka Serial Monitor pada:

```text
115200 baud
```

## 3. Hubungkan Semua Komponen

Pastikan:

- Pin sensor sesuai konfigurasi.
- Servo mendapatkan sumber daya yang sesuai.
- ESP-12E mendapatkan sumber daya.
- Semua GND terhubung menjadi satu.

---

# 🧪 Pengujian Sistem

| Pengujian | Kondisi | Hasil yang Diharapkan |
|---|---|---|
| 1 | Tidak ada kendaraan | Palang tertutup, LED merah |
| 2 | Kendaraan 7–12 cm | LED kuning, kondisi siaga |
| 3 | Kendaraan <7 cm tanpa suara | Menunggu suara |
| 4 | Kendaraan <7 cm + suara | Palang terbuka |
| 5 | Kendaraan meninggalkan area | Palang menutup setelah ±1 detik |
| 6 | Sensor api aktif | Palang terbuka dan status bahaya |
| 7 | ESP terhubung Wi-Fi | Data tampil pada web monitor |

---

# 👥 Pembagian Sistem

### Arduino Uno

Bertanggung jawab terhadap:

- Pembacaan sensor jarak.
- Pembacaan sensor suara.
- Pembacaan sensor api.
- Pengontrolan LED.
- Pengontrolan servo.
- Logika buka/tutup palang.
- Pengiriman data ke ESP-12E.

### ESP-12E

Bertanggung jawab terhadap:

- Koneksi Wi-Fi.
- Menerima data dari Arduino Uno.
- Menyediakan web server.
- Menampilkan status Smart Parking.
- Menampilkan jarak kendaraan.
- Menampilkan status sensor api.

---

# 🧠 Teknologi

- **Arduino Uno**
- **ESP-12E / NodeMCU**
- **Arduino IDE**
- **C/C++**
- **ESP8266 Wi-Fi**
- **HC-SR04**
- **KY-038**
- **Flame Sensor**
- **Servo SG90**
- **HTML & CSS**
- **ESP8266WebServer**

---

# 📌 Catatan

Pastikan seluruh koneksi kabel diperiksa sebelum sistem dinyalakan.

Khususnya:

> **GND Arduino Uno + GND baterai servo + GND ESP-12E harus terhubung bersama.**

Servo juga menggunakan sumber daya terpisah sesuai rancangan agar tidak membebani Arduino Uno.

---

# 👩‍💻 Project

**Smart Parking — Sistem Parkir Pintar Berbasis IoT**

Project ini dibuat sebagai implementasi sistem IoT menggunakan Arduino Uno dan ESP-12E untuk otomatisasi serta monitoring area parkir.

---

## 📜 Lisensi

Project ini dibuat untuk keperluan **pembelajaran dan tugas akademik**.
