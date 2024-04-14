#include <Servo.h>
Servo servoMotor;

const int servoPin = 12; 
const int laserPin = 5; 
const int photoresistorPin = A2;
const int trigPin = 2;
const int echoPin = 3;
const int inductivePin = 2;
const int kySensorPin = 10;

int sensorState = 0;

void setup() {
  pinMode(laserPin, OUTPUT);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  pinMode(kySensorPin, INPUT); // Set the sensor pin as input
  pinMode(inductivePin, INPUT); // Set the sensor pin as input
  servoMotor.attach(servoPin);
  Serial.begin(9600);     // Inicia la comunicación serial
}

void loop() {
  bool objectExists = manageDoor();

  //Si detectamos objeto, correr analisis
  if(objectExists){
      readPhotoresistor();
      readInductiveSensor();
      readKYSensor();
  }
  delay(1000);  // Esperar 1 segundo antes de la siguiente lectura
}

void readPhotoresistor(){
  digitalWrite(laserPin, HIGH);
  Serial.println(sensorState); // Imprime el estado del sensor en el puerto serie

  int photoresistorValue = analogRead(photoresistorPin);
 
  // Print the value to the serial monitor
  Serial.print("Photoresistor Value: ");
  Serial.println(photoresistorValue);
  
}

void readInductiveSensor(){
  int sensorValue = digitalRead(inductivePin); // Read the sensor value
  
  Serial.print("Sensor value: ");
  Serial.println(sensorValue); 
}

void readKYSensor(){
  int sensorValue = digitalRead(kySensorPin); // Read the sensor value
  
  Serial.print("Sensor value: ");
  Serial.println(sensorValue);
}

bool manageDoor(){
  // Clear the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Set the trigger pin high for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo pin and calculate the distance
  long duration = pulseIn(echoPin, HIGH);
  float distance_cm = duration * 0.034 / 2;
  
  // Print the distance to the serial monitor
  Serial.print("Distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  if(distance_cm > 20){    
      servoMotor.write(-20);
      return false;

  } else {
      servoMotor.write(80);
      return true;
  }
}






