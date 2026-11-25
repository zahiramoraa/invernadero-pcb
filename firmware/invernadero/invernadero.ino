#include <DHT.h>            // Librería para usar el sensor DHT11

DHT dht(5, DHT11);          // El DHT11 está conectado al pin 5

bool manual = false;        // false = modo automático, true = modo manual
bool bomba = false;         // Estado de la bomba en modo manual (false = apagada)
bool ventilador = false;    // Estado del ventilador en modo manual (false = apagado)

void setup() {
  Serial.begin(9600);       // Inicia el Monitor Serie a 9600
  dht.begin();              // Inicia el sensor DHT11
  pinMode(4, INPUT);        // Pin 4: sensor de luz (entrada)
  pinMode(8, OUTPUT);       // Pin 8: bomba (salida)
  pinMode(9, OUTPUT);       // Pin 9: ventilador (salida)
  Serial.println("Escribe: m = manual, a = automatico, b = bomba, v = ventilador");
}

void loop() {
  if (Serial.available() > 0) {
    char letra = Serial.read();       

    if (letra == 'm') {               // m = cambia a modo manual
      manual = true;
      Serial.println("MODO MANUAL");
    }
    if (letra == 'a') {               // a = regresa a modo automático
      manual = false;
      Serial.println("MODO AUTOMATICO");
    }
    if (letra == 'b' && manual) {     // b = prende o apaga la bomba (solo en manual)
      bomba = !bomba;                 // Cambia al estado contrario
    }
    if (letra == 'v' && manual) {     // v = prende o apaga el ventilador (solo en manual)
      ventilador = !ventilador;       // Cambia al estado contrario
    }
  }

  // Lee los sensores
  float temperatura = dht.readTemperature();   // Temperatura en °C
  float humedadAire = dht.readHumidity();      // Humedad del aire en %
  int suelo = analogRead(A0);                  // Humedad del suelo (valores arriba de 600 = más seco)
  int luz = digitalRead(4);                    // Luz (0 = hay luz, 1 = no hay luz)

  // Muestra las lecturas en el Monitor Serie
  Serial.print("Temp: ");
  Serial.print(temperatura);
  Serial.print("  Hum aire: ");
  Serial.print(humedadAire);
  Serial.print("  Suelo: ");
  Serial.print(suelo);
  Serial.print("  Luz: ");
  if (luz == 0) {
    Serial.print("si hay luz");
  } else {
    Serial.print("no hay luz");
  }
  if (manual) {                       
    Serial.println("  [MANUAL]");
  } else {
    Serial.println("  [AUTO]");
  }

  if (manual) {
    // MODO MANUAL: en base a letras
    digitalWrite(8, bomba);
    digitalWrite(9, ventilador);
  } else {
    // MODO AUTOMÁTICO: con sensores

    // Ventilador: se prende si hace más de 22 °C o hay más de 70 % de humedad
    if (temperatura > 22 || humedadAire > 70) {
      digitalWrite(9, HIGH);
    } else {
      digitalWrite(9, LOW);
    }

    // Bomba: si el suelo está seco (más de 600), riega 3 segundos
    if (suelo > 600) {
      Serial.println("Regando...");
      digitalWrite(8, HIGH);          // Prende la bomba
      delay(3000);                    // Espera 3 segundos
      digitalWrite(8, LOW);           // Apaga la bomba
    }
  }

  delay(2000);                        // Espera 2 segundos antes de volver a empezar
}