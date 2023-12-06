#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Estructura para almacenar información de cada batería
struct Bateria {
  float voltajeInicial;
  float voltajeActual;
  float porcentajeCarga;
  float tiempoRestanteCarga;
};

Bateria baterias[4];
int bateriaActual = 0;
const int pinInterruptor = 12;
const int sensorTempPlacaPin = A0;
const int sensorTempBateriasPin = A1;
const int ventiladorPlacaPin = 2;
const int ventiladorBateriasPin = 3;
const float porcentajeCargaMaximo = 100.0;
const int cargaBateriaPines[4] = {4, 5, 6, 7};
const int ledBateriaPines[4] = {8, 9, 10, 11};
bool estadoLED[4] = {LOW, LOW, LOW, LOW};
unsigned long previousMillis[4] = {0, 0, 0, 0};
const int pinesBaterias[4] = {A2, A3, A4, A5};
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(ventiladorPlacaPin, OUTPUT);
  pinMode(ventiladorBateriasPin, OUTPUT);
  pinMode(pinInterruptor, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  for (int i = 0; i < 4; i++) {
    baterias[i].voltajeInicial = leerVoltajeBateria(i);
    pinMode(cargaBateriaPines[i], OUTPUT);
    pinMode(ledBateriaPines[i], OUTPUT);
  }
}

void loop() {
  float tempPlaca = leerTemperatura(sensorTempPlacaPin);
  float tempBaterias = leerTemperatura(sensorTempBateriasPin);
  controlarVentilador(ventiladorPlacaPin, tempPlaca, 30.0);
  controlarVentilador(ventiladorBateriasPin, tempBaterias, 30.0);
  actualizarDisplay(tempPlaca, tempBaterias);
  for (int i = 0; i < 4; i++) {
    controlarCargaBateria(i, cargaBateria[i]);
    actualizarLEDBateria(i, cargaBateria[i]);
  }
  if (digitalRead(pinInterruptor) == LOW) {
    bateriaActual = (bateriaActual + 1) % 4;
    delay(300);
  }
  actualizarInfoBateria(bateriaActual);
  mostrarInfoBateria(bateriaActual);
  delay(1000);
}

// Lee y devuelve la temperatura medida por el sensor dado
float leerTemperatura(int pinSensor) {
  int valorADC = analogRead(pinSensor);
  float voltaje = (valorADC / 1023.0) * 5.0;
  float temperatura = voltaje * 100.0;
  return temperatura;
}

// Controla el estado del ventilador basándose en la temperatura dada
void controlarVentilador(int pinVentilador, float temperatura, float umbral) {
  digitalWrite(pinVentilador, temperatura > umbral ? HIGH : LOW);
}

// Actualiza la información en el display LCD
void actualizarDisplay(float tempPlaca, float tempBaterias) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp Placa: ");
  lcd.print(tempPlaca);
  lcd.setCursor(0, 1);
  lcd.print("Temp Bat: ");
  lcd.print(tempBaterias);
}

// Controla la carga de una batería basándose en su porcentaje de carga
void controlarCargaBateria(int indiceBateria) {
  float porcentajeCarga = baterias[indiceBateria].porcentajeCarga;
  digitalWrite(cargaBateriaPines[indiceBateria], porcentajeCarga < porcentajeCargaMaximo ? HIGH : LOW);
  actualizarLEDBateria(indiceBateria, porcentajeCarga);
}

// Actualiza el estado del LED de la batería basándose en su nivel de carga
void actualizarLEDBateria(int indiceBateria, float nivelCarga) {
  unsigned long intervalo;
  if (nivelCarga < 25.0) intervalo = 1000;
  else if (nivelCarga < 50.0) intervalo = 500;
  else if (nivelCarga < 75.0) intervalo = 400;
  else if (nivelCarga < 90.0) intervalo = 300;
  else if (nivelCarga < 100.0) intervalo = 200;
  else {
    digitalWrite(ledBateriaPines[indiceBateria], HIGH);
    return;
  }
  parpadearLED(indiceBateria, intervalo);
}

// Implementa un parpadeo no bloqueante para los LEDs
void parpadearLED(int indiceBateria, unsigned long intervalo) {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis[indiceBateria] >= intervalo) {
    previousMillis[indiceBateria] = currentMillis;
    estadoLED[indiceBateria] = !estadoLED[indiceBateria];
    digitalWrite(ledBateriaPines[indiceBateria], estadoLED[indiceBateria]);
  }
}

// Actualiza la información de carga de la batería seleccionada
void actualizarInfoBateria(int indiceBateria) {
  baterias[indiceBateria].voltajeActual = leerVoltajeBateria(indiceBateria);
  float voltajeMinimo = 1.0;
  float voltajeMaximo = 1.5;
  baterias[indiceBateria].porcentajeCarga = max(min((baterias[indiceBateria].voltajeActual - voltajeMinimo) / (voltajeMaximo - voltajeMinimo) * 100, 100.0), 0.0);
  baterias[indiceBateria].tiempoRestanteCarga = 0; // No aplicable para baterías no recargables
}

// Muestra la información de la batería seleccionada en el LCD
void mostrarInfoBateria(int indiceBateria) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bateria: ");
  lcd.print(indiceBateria + 1);
  lcd.setCursor(0, 1);
  lcd.print("Carga: ");
  lcd.print(baterias[indiceBateria].porcentajeCarga);
  lcd.print("%");
}

// Lee y devuelve el voltaje de la batería conectada al pin dado
float leerVoltajeBateria(int indiceBateria) {
  int valorADC = analogRead(pinesBaterias[indiceBateria]);
  float voltaje = (valorADC * 5.0) / 1023.0;
  return voltaje;
}
