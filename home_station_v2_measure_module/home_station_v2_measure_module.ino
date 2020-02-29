/*
 Name:		home_station_v2_measure_module.ino
 Created:	2/29/2020 7:33:33 PM
 Author:	alexbeard
*/

#include <SPI.h>
#include <RF24_config.h>
#include <RF24.h>
#include <printf.h>
#include <nRF24L01.h>
#include <DHT.h>
#include <BMP085.h>
#include <Wire.h> 

#define DHTPIN 8     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

//CE = 9
//CSN = 10
RF24 radio(9, 10);
const byte address[6] = { 0,0,0,0,1 };

BMP085 dps = BMP085();
DHT dht(DHTPIN, DHTTYPE);

float d;
long p;
long r;


void setup() {
    Serial.begin(9600);
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();

    d = 0;
    p = 0;
    dht.begin();
    delay(2000);
    Wire.begin();
    dps.init();
}

// the loop function runs over and over again until power down or reset
void loop() {
    long* data = new long[7];
    data[0] = 7;
    d = measureT();
    p = round(d * 100);
    r = p / 100;
    Serial.print(r);
    Serial.print('.');
    data[1] = r;
    r = p % 100;
    Serial.print(r);
    Serial.print(' ');
    data[2] = r;

    d = measureP();
    p = round(d * 100);
    r = p / 100;
    Serial.print(r);
    Serial.print('.');
    data[3] = r;
    r = p % 100;
    Serial.print(r);
    Serial.print(' ');
    data[4] = r;

    d = measureH();
    p = round(d * 100);
    r = p / 100;
    data[5] = r;
    Serial.print(r);
    Serial.print('.');
    r = p % 100;
    Serial.print(r);
    Serial.println();
    data[6] = r;

    //Serial.println(sizeof(data));
    for (size_t i = 0; i < 7; i++)
    {
        Serial.println(data[i]);
    }
    Serial.println("---------");
    radio.write(&data, sizeof(data));
    delay(1000);
    delete[] data;

}

float measureP() {
    dps.calcTruePressure(&p);
    return p / 133.3;
}

float measureT() {
    return dht.readTemperature();
}

float measureH() {
    return dht.readHumidity();
}
