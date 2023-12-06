#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Variables para almacenar información de las baterías
struct Bateria {
  float voltajeInicial;
  float voltajeActual;
  float velocidadCarga;
  float porcentajeCarga;
  float tiempoRestanteCarga;
};

Bateria baterias[4];
int bateriaActual = 0;         // Índice de la batería actualmente en foco
const int pinInterruptor = 12; // Pin para el interruptor

// Define los pines y constantes
const int sensorTempPlacaPin = A0;
const int sensorTempBateriasPin = A1;
const int ventiladorPlacaPin = 2;
const int ventiladorBateriasPin = 3;

const float porcentajeCargaMaximo = 100.0; // Porcentaje máximo de carga
const int cargaBateriaPines[4] = {
    4, 5, 6, 7}; // Pines para los módulos de carga de las baterías
const int ledBateriaPines[4] = {8, 9, 10,
                                11}; // Pines para los LEDs de las baterías
bool estadoLED[4] = {LOW, LOW, LOW,
                     LOW}; // Estados para el parpadeo de cada LED
unsigned long previousMillis[4] = {0, 0, 0,
                                   0}; // Tiempos de inicio para cada LED
const int pinesBaterias[4] = {
    A2, A3, A4,
    A5}; // Asumiendo que estos pines están conectados a las baterías
float cargaBateria[4] = {0, 0, 0,
                         0}; // Estado de carga de las baterías (0 a 100%)

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(ventiladorPlacaPin, OUTPUT);
  pinMode(ventiladorBateriasPin, OUTPUT);
  pinMode(pinInterruptor, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  // Inicializa los pines de carga y LED
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

  // Lógica de carga y control de LEDs para las baterías
  for (int i = 0; i < 4; i++) {
    // Aquí añadirías tu lógica para determinar el estado de carga de cada
    // batería Por ejemplo, actualizar cargaBateria[i] basado en lecturas de
    // sensores

    // Controla la carga de la batería
    controlarCargaBateria(i, cargaBateria[i]);

    // Actualiza el estado del LED de la batería
    actualizarLEDBateria(i, cargaBateria[i]);
  }

  if (digitalRead(pinInterruptor) == LOW) {
    bateriaActual = (bateriaActual + 1) % 4;
    delay(300); // Debounce del interruptor
  }

  // Actualiza y muestra la información de la batería actual
  actualizarInfoBateria(bateriaActual);
  mostrarInfoBateria(bateriaActual);

  delay(1000);
}

float leerTemperatura(int pinSensor) {
  int valorADC = analogRead(pinSensor);
  float voltaje = (valorADC / 1023.0) * 5.0;
  float temperatura = voltaje * 100.0; // Ajustar según el sensor
  return temperatura;
}

void controlarVentilador(int pinVentilador, float temperatura, float umbral) {
  if (temperatura > umbral) {
    digitalWrite(pinVentilador, HIGH);
  } else {
    digitalWrite(pinVentilador, LOW);
  }
}

void actualizarDisplay(float tempPlaca, float tempBaterias) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp Placa: ");
  lcd.print(tempPlaca);
  lcd.setCursor(0, 1);
  lcd.print("Temp Bat: ");
  lcd.print(tempBaterias);
}

void controlarCargaBateria(int indiceBateria) {
  float porcentajeCarga = baterias[indiceBateria].porcentajeCarga;

  if (porcentajeCarga >= porcentajeCargaMaximo) {
    // La batería está completamente cargada o no es recargable, detener la
    // carga
    digitalWrite(cargaBateriaPines[indiceBateria], LOW);
    // Indicar carga completa o estado no recargable con LED fijo
    digitalWrite(ledBateriaPines[indiceBateria], HIGH);
  } else {
    // La batería aún necesita carga, continuar con el proceso
    // Nota: Esto es solo para baterías recargables. En el caso de alcalinas,
    // esta parte no se usaría.
    digitalWrite(cargaBateriaPines[indiceBateria], HIGH);
    // Actualizar el estado del LED según el porcentaje de carga
    actualizarLEDBateria(indiceBateria, porcentajeCarga);
  }
}

void actualizarLEDBateria(int indiceBateria, float nivelCarga) {
  unsigned long intervalo;
  if (nivelCarga < 25.0)
    intervalo = 1000;
  else if (nivelCarga < 50.0)
    intervalo = 500;
  else if (nivelCarga < 75.0)
    intervalo = 400;
  else if (nivelCarga < 90.0)
    intervalo = 300;
  else if (nivelCarga < 100.0)
    intervalo = 200;
  else {
    digitalWrite(ledBateriaPines[indiceBateria],
                 HIGH); // LED fijo para carga completa
    return;
  }
  parpadearLED(indiceBateria, intervalo);
}

void parpadearLED(int indiceBateria, unsigned long intervalo) {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis[indiceBateria] >= intervalo) {
    previousMillis[indiceBateria] = currentMillis;
    estadoLED[indiceBateria] = !estadoLED[indiceBateria];
    digitalWrite(ledBateriaPines[indiceBateria], estadoLED[indiceBateria]);
  }
}

void actualizarInfoBateria(int indiceBateria) {
  baterias[indiceBateria].voltajeActual = leerVoltajeBateria(indiceBateria);

  // Asumiendo que 1.5V es 100% y 1.0V es 0%
  float voltajeMinimo = 1.0;
  float voltajeMaximo = 1.5;

  // Calcular el porcentaje de carga
  baterias[indiceBateria].porcentajeCarga =
      (baterias[indiceBateria].voltajeActual - voltajeMinimo) /
      (voltajeMaximo - voltajeMinimo) * 100;

  // Asegurarse de que el porcentaje de carga esté dentro de 0% - 100%
  baterias[indiceBateria].porcentajeCarga =
      max(min(baterias[indiceBateria].porcentajeCarga, 100.0), 0.0);

  // Como no estamos cargando baterías alcalinas, el tiempo de carga restante no
  // es aplicable
  baterias[indiceBateria].tiempoRestanteCarga = 0;
}

void mostrarInfoBateria(int indiceBateria) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bateria: ");
  lcd.print(indiceBateria + 1);
  lcd.setCursor(0, 1);
  lcd.print("Carga: ");
  lcd.print(baterias[indiceBateria].porcentajeCarga);
  lcd.print("%");
  // Muestra más información según sea necesario
}

float leerVoltajeBateria(int indiceBateria) {
  int valorADC = analogRead(pinesBaterias[indiceBateria]);
  float voltaje = (valorADC * 5.0) / 1023.0; // Convierte el valor ADC a voltaje

  return voltaje; // Devuelve el voltaje de la batería
}
