#include <ArduinoSound.h>
#include <SPI.h>
#include <SD.h>

const int buttonPin = 7;
const char filename[] = "SANTA002.WAV";
bool isPlaying = false;

int buttonState = LOW;
int lastButtonState = LOW;

SDWaveFile santaWave;

void setup() {
  Serial.begin(9600);
  // #ifdef Serial
  //   while (!Serial) {
  //     ; // Wait only if Serial is available (USB connected)
  //   }
  // #endif

  pinMode(buttonPin, INPUT);

  Serial.print("Initializing SD card...");
  if (!SD.begin(SDCARD_SS_PIN)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

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

  AudioOutI2S.volume(1.0);

  // Preload with a quick playback to initialize
  AudioOutI2S.play(santaWave);
  delay(50);  // Play for a brief period
  AudioOutI2S.stop();

}

void loop() {
  buttonState = digitalRead(buttonPin);

  // Check if button state has changed
  if (buttonState != lastButtonState) {
    if (buttonState == LOW && !isPlaying) {  // Button pressed (transition from LOW to HIGH)
      delay(1200);
      Serial.println("Playing santa001.wav");
      AudioOutI2S.play(santaWave);
      isPlaying = true;
    } else if (buttonState == HIGH && isPlaying) {  // Button released (transition from HIGH to LOW)
      Serial.println("Stopping playback");
      AudioOutI2S.stop();
      isPlaying = false;
    }
  }

  // Save the current button state for the next loop iteration
  lastButtonState = buttonState;
}
