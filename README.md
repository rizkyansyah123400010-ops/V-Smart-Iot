# V-SMART IoT

## Deskripsi
V-SMART (Vehicle Speed and Noise Monitoring System) merupakan sistem monitoring kendaraan berbasis Internet of Things (IoT) yang berfungsi untuk mendeteksi kecepatan kendaraan dan tingkat kebisingan secara realtime.

Sistem ini menggunakan Arduino Uno sebagai pusat pengolahan data sensor dan ESP32-CAM sebagai modul komunikasi untuk sistem monitoring berbasis IoT.

---

## Tujuan
Sistem ini dibuat untuk:
- Mengukur tingkat kebisingan kendaraan.
- Mengukur kecepatan kendaraan.
- Memberikan informasi status pelanggaran berdasarkan batas yang telah ditentukan.
- Mengirimkan data hasil pengukuran ke sistem IoT.

---

## Komponen yang Digunakan

### Mikrokontroler
- Arduino Uno
- ESP32-CAM

### Sensor
- Sensor MAX4466 sebagai pendeteksi tingkat kebisingan suara.
- Sensor HB100 sebagai pendeteksi kecepatan kendaraan.

### Komponen Output
- LCD I2C 16x2 sebagai tampilan nilai pengukuran dan status sistem.

### Komponen Pendukung Rangkaian
- Adaptor sebagai sumber tegangan sistem.
- Modul LM2596S sebagai regulator step-down untuk mengatur tegangan.
- IC LM358 sebagai penguat dan pengolah sinyal.
- Resistor 1KΩ.
- Resistor 2KΩ.
- Kabel jumper.
- Breadboard.

---

## Struktur Program

### V_SMART_Arduino.ino

Program Arduino Uno digunakan untuk:

- Melakukan inisialisasi sensor.
- Membaca data sensor MAX4466.
- Membaca pulsa dari sensor HB100.
- Menghitung nilai kecepatan kendaraan.
- Menentukan status aman atau pelanggaran.
- Menampilkan hasil pada LCD I2C.
- Mengirim data ke ESP32-CAM melalui komunikasi serial.


### V_SMART_ESP32CAM.ino

Program ESP32-CAM digunakan untuk:

- Menerima data dari Arduino Uno.
- Menghubungkan sistem ke jaringan WiFi.
- Melakukan monitoring data berbasis Internet of Things.

---

## Cara Kerja Sistem

1. Adaptor memberikan sumber tegangan ke rangkaian.
2. LM2596S mengatur tegangan sesuai kebutuhan setiap komponen.
3. Sensor MAX4466 membaca tingkat kebisingan kendaraan.
4. Sensor HB100 membaca kecepatan kendaraan berdasarkan pulsa.
5. LM358 membantu penguatan sinyal sensor.
6. Arduino Uno melakukan pengolahan data.
7. Hasil pembacaan ditampilkan pada LCD I2C.
8. Data dikirimkan ke ESP32-CAM untuk monitoring IoT.

---

## Batas Deteksi Sistem

- Kebisingan ≥ 70 dB → PELANGGARAN
- Kecepatan ≥ 30 km/h → PELANGGARAN

Jika nilai tidak melewati batas maka sistem memberikan status:

AMAN

---

## File Program

Repository ini berisi:

- V_SMART_Arduino.ino
- V_SMART_ESP32CAM.ino

Kode lengkap program dapat dilihat pada file yang tersedia dalam repository ini.

---

## Author

Project Internet of Things  
V-SMART (Vehicle Speed and Noise Monitoring System)
