// Arduino Gesture Control Robot
// Transmitter: Arduino Nano + nRF24L01

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Gesture sensor / joystick outputs
const int x_out = A0;
const int y_out = A1;

// nRF24L01
// CE = D8
// CSN = D10
RF24 radio(8, 10);

const byte address[6] = "00001";

// Data structure
struct data {
  int xAxis;
  int yAxis;
};

data send_data;

void setup() {

  Serial.begin(9600);

  // Initialize radio
  if (!radio.begin()) {
    Serial.println("nRF24L01 not detected!");
    while (1);
  }

  radio.openWritingPipe(address);

  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);

  radio.stopListening();

  Serial.println("Transmitter Ready");
}

void loop() {

  // Read X and Y values
  send_data.xAxis = analogRead(x_out);
  send_data.yAxis = analogRead(y_out);

  // Send data
  bool success = radio.write(&send_data, sizeof(send_data));

  // Display values for testing
  Serial.print("X: ");
  Serial.print(send_data.xAxis);

  Serial.print("  Y: ");
  Serial.print(send_data.yAxis);

  Serial.print("  Send: ");

  if (success) {
    Serial.println("OK");
  } else {
    Serial.println("FAILED");
  }

  delay(20);
}