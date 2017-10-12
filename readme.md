# btcWatcher
Menampilkan harga bitcoin dalam lcd 16x2 (kurs https://www.bitcoin.co.id/)

![header](https://github.com/fariswd/btcwatcher/raw/master/btcwatcher.jpg "Header")

Alat pemantau kurs bitcoin dengan bantuan ESP8266 dan lcd16x2. Memanfaatkan API yang disediakan oleh https://www.bitcoin.co.id/.

## ESP8266
Menggunakan library  
- LiquidCrystalI2C untuk lcd
Wire Map:  
ESP => LCDI2C  
D1 => SCL  
D2 => SDA  
Vin => 5V  
Gnd => Gnd

- ArduinoJson untuk parsing json

## Catatan
- untuk mengakses https melalui http client diperlukan fingerprint yang dapat digenerate pada: https://www.grc.com/fingerprints.htm
- dokumentasi API bitcoin.co.id: https://vip.bitcoin.co.id/downloads/BITCOINCOID-API-DOCUMENTATION.pdf
- video penampakan alat https://twitter.com/radenfaris/status/918487404943630336

