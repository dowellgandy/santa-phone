#include <ArduinoSound.h>
#include <SPI.h>
#include <SD.h>

const int buttonPin = 7;
const char filename[]= "santa001.wav";

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

  delay(100);

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

  File testFile = SD.open("santa001.wav");
  if (testFile) {
    Serial.println("Successfully opened santa001.wav");
    // Read the first few bytes for verification
    while (testFile.available()) {
      Serial.write(testFile.read()); // Output the raw file data to Serial Monitor
    }
    testFile.close();
  } else {
    Serial.println("Error: Could not open santa001.wav directly.");
  }

  santaWave = SDWaveFile(filename);

  if(!santaWave) {
    Serial.println("Error opening santa001.wav");
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
