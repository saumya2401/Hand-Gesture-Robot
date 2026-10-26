Receiver code :-
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(8, 10);   // CE = D8, CSN = D10

const byte address[6] = "00001";

int ENA = 3;
int ENB = 9;

int MotorA1 = 4;
int MotorA2 = 5;
int MotorB1 = 6;
int MotorB2 = 7;

struct data {
  int xAxis;
  int yAxis;
};

data receive_data;

void stopMotors() {
  digitalWrite(MotorA1, LOW);
  digitalWrite(MotorA2, LOW);
  digitalWrite(MotorB1, LOW);
  digitalWrite(MotorB2, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void setup() {

  Serial.begin(9600);

  if (!radio.begin()) {
    Serial.println("nRF24L01 NOT detected!");
    while (1);
  }

  Serial.println("nRF24L01 DETECTED!");

  radio.openReadingPipe(0, address);

  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);

  radio.startListening();

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(MotorA1, OUTPUT);
  pinMode(MotorA2, OUTPUT);
  pinMode(MotorB1, OUTPUT);
  pinMode(MotorB2, OUTPUT);

  stopMotors();

  Serial.println("RECEIVER READY");
}

void loop() {

  if (radio.available()) {

    radio.read(&receive_data, sizeof(receive_data));

    Serial.print("X = ");
    Serial.print(receive_data.xAxis);

    Serial.print("   Y = ");
    Serial.println(receive_data.yAxis);


    // FORWARD
    if (receive_data.yAxis > 400) {

      Serial.println("FORWARD");

      digitalWrite(MotorA1, LOW);
      digitalWrite(MotorA2, HIGH);

      digitalWrite(MotorB1, HIGH);
      digitalWrite(MotorB2, LOW);

      analogWrite(ENA, 150);
      analogWrite(ENB, 150);
    }


    // BACKWARD
    else if (receive_data.yAxis < 320) {

      Serial.println("BACKWARD");

      digitalWrite(MotorA1, HIGH);
      digitalWrite(MotorA2, LOW);

      digitalWrite(MotorB1, LOW);
      digitalWrite(MotorB2, HIGH);

      analogWrite(ENA, 150);
      analogWrite(ENB, 150);
    }


    // LEFT
    else if (receive_data.xAxis < 320) {

      Serial.println("LEFT");

      digitalWrite(MotorA1, HIGH);
      digitalWrite(MotorA2, LOW);

      digitalWrite(MotorB1, HIGH);
      digitalWrite(MotorB2, LOW);

      analogWrite(ENA, 150);
      analogWrite(ENB, 150);
    }


    // RIGHT
    else if (receive_data.xAxis > 400) {

      Serial.println("RIGHT");

      digitalWrite(MotorA1, LOW);
      digitalWrite(MotorA2, HIGH);

      digitalWrite(MotorB1, LOW);
      digitalWrite(MotorB2, HIGH);

      analogWrite(ENA, 150);
      analogWrite(ENB, 150);
    }


    // STOP
    else {

      Serial.println("STOP");

      stopMotors();
    }
  }
}