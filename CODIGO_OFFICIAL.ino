/*
Log
04/16/2024 - Removido "const int laserPin = 11;"
04/16/2024 - Removido "const int kySensorPinA = A3;"
*/
#include <Servo.h>
Servo servoMotorPaper;
Servo servoMotorGlass;
Servo servoMotorPlastic;

Servo servoMotorInductive;
Servo servoMotorKY;


const int photoresistorPin = A2;
const int trigPin = 8;
const int echoPin = 6;
const int inductivePin = 9;
const int kySensorPin = 7;
const int servoInductivePin = 10;
const int servoKYPin = 11;

int sensorState = 0;
bool kyStatus;
bool inductiveStatus;
const int numberOfMaterials = 5;

void setup() {
  //Sensores
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(kySensorPin, INPUT);
  pinMode(inductivePin, INPUT);
  //Servomotores
  servoMotorInductive.attach(servoInductivePin);
  servoMotorKY.attach(servoKYPin);
  
  Serial.begin(9600);
}

void loop() {
  bool objectExists = readInfrarred();
  resetCloseUp();

  if (objectExists) {
    /*Si el infrarojo detecta un objeto, leer los valores de sensores*/
    float ultrasonicDistanceValue = readDistance();
    closeUpKY(ultrasonicDistanceValue);
    closeUpInductive(ultrasonicDistanceValue);    
    float photoResistorValue = readPhotoresistor();
    float inductiveSensorValue = readInductiveSensor();
    float kySensorValue = readKYSensor();

    /*al tener todos los datos de sensores, identificar que material es*/
    char* identifiedMaterial = identifyMaterial(photoResistorValue, inductiveSensorValue, kySensorValue);
    Serial.println(identifiedMaterial);
    openDoorDependingOnMaterial(identifiedMaterial);
    delay(10000);
    
    resetBooleans();
  }
  delay(1000);  // Esperar 1 segundo antes de la siguiente lectura
}

boolean readInfrarred(){
  return true;
}

float readPhotoresistor() {

  int sensorValue = analogRead(photoresistorPin);

  // Print the value to the serial monitor
  Serial.print("Photoresistor Value: ");
  Serial.println(sensorValue);

  return sensorValue;
}

int readInductiveSensor() {
  int sensorValue = digitalRead(inductivePin);  // Read the sensor value
  int sensorValueAnalog = analogRead(A1);

  Serial.print("Sensor Inductivo valor Analogo: ");
  Serial.println(sensorValueAnalog);
  Serial.print("Sensor Inductivo valor: ");
  Serial.println(sensorValue);
  return sensorValue;
}

int readKYSensor() {
  int sensorValue = digitalRead(kySensorPin);  // Read the sensor value

  Serial.print("Sensor KY valor: ");
  Serial.println(sensorValue);

  return sensorValue;
}

float readDistance() {
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

  return distance_cm;
}

//Modificar condicionales para saber cual material elegir
const char* identifyMaterial(float photoResistorValue, float inductiveSensorValue, float kySensorValue) {

  if(inductiveSensorValue != 1){
    return "Metal";
  } else if (kySensorValue == 1){
    return "Organico";
  } else if (photoResistorValue > 100){
    return "Papel";
  } else{
    return "No identificado";
  } //Agregar opciones para Vidrio/Plastic

}


//Modificar grados para abrir/cerrar puertas de servo
void openDoorDependingOnMaterial(char* material) {

  int materialNumber = 0;

  //strcmp comparison between 2 strings. ">1 = equal". "<0 | >0 = not equal

  if (strcmp(material, "Papel") == 0) {
    materialNumber = 1;
  } else if (strcmp(material, "Metal") == 0) {
    materialNumber = 2;
  } else if (strcmp(material, "Organico") == 0) {
    materialNumber = 3;
  } else if (strcmp(material, "Vidrio") == 0) {
    materialNumber = 4;
  } else if (strcmp(material, "Plastico") == 0) {
    materialNumber = 5;
  }

  /*
  1 - Papel y Carton
  2 - Metales
  3 - Organico
  4 - Vidrio
  5 - Plastico
  */

  //Opens and closes material doors
  switch (materialNumber) {
    case 1:
      //openCloseDoor(servoMotorPaper, 0, 90);
      break;

    case 2:
      //openCloseDoor(servoMotorMetal, 0, 90);
      break;

    case 3:
      //openCloseDoor(servoMotorOrganic, 0, 90);
      break;

    case 4:
      //openCloseDoor(servoMotorGlass, 0, 90);
      break;
    case 5:
      //openCloseDoor(servoMotorPlastic, 0, 90);
      break;

    default:

      break;
  }

}

//Metodo Auxiliar
void closeUpInductive(float distance){
  servoMotorInductive.write(90);
  Serial.println("Gira Inductivo");
}

void closeUpKY(float distance){
  servoMotorKY.write(90);
  Serial.println("Gira KY");
}

void resetCloseUp(){
  servoMotorInductive.write(0);
  servoMotorKY.write(0);
}

//Metodo Auxiliar
void resetBooleans() {
  kyStatus = false;
  inductiveStatus = false;
}
