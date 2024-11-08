#include <ArduinoSound.h>
#include <SPI.h>
#include <SD.h>

const int buttonPin = 7;
const char filename[] = "SANTA002.WAV";

int buttonState = 0;

SDWaveFile santaWave;

void setup() {
  Serial.begin(9600);
  while(!Serial) {
    ;
  }

  pinMode(buttonPin, INPUT);

  Serial.print("Initializing SD card...");
  if (!SD.begin(SDCARD_SS_PIN)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  delay(1000);

  File root = SD.open("/");
  if (root) {
    while (true) {
      File entry = root.openNextFile();
      if (!entry) {
        break; // no more files
      }
      Serial.print("Found file: ");
      Serial.println(entry.name());
      entry.close();
    }
    root.close();
  } else {
    Serial.println("Error: Could not open root directory.");
  }

  if (SD.exists(filename)) {
  Serial.println("File exists on SD card.");
    } else {
  Serial.println("File does NOT exist on SD card.");
  }

  santaWave = SDWaveFile(filename);

  Serial.println(santaWave.bitsPerSample());

  long channels = santaWave.channels();
  Serial.print("Channels = ");
  Serial.println(channels);

  if(!santaWave) {
    Serial.println("Error assigning file to SDWaveFile object");
    while (true);
  }

}

void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    Serial.println("Playing santa001.wav");
    AudioOutI2S.play(santaWave);
  }


}
