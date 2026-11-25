# Controlador de invernadero con ATmega328P y Arduino Nano

Proyecto final de Diseño de PCBs (Ingeniería Mecatrónica, Universidad Anáhuac Querétaro).

Es una PCB de dos capas que controla un invernadero pequeño. La placa lee la luz, la temperatura y la humedad del aire, y la humedad de la tierra, y con eso prende una bomba para regar y un ventilador. Tiene lugar para dos controladores que comparten las mismas señales, y solo se pone uno de los dos:

- **ATmega328P** (DIP-28) montado desde cero, con cristal de 16 MHz y conector ICSP para programarlo.
- **Arduino Nano** como respaldo, en caso de que el ATmega no funcione.

## Contenido

| Carpeta | Qué tiene |
|---|---|
| `hardware/altium/` | Proyecto de Altium Designer: esquemático (`esquematico.SchDoc`) y PCB (`pcb.PcbDoc`) |
| `hardware/altium/outputs/` | Gerbers y taladros generados en Altium, y el reporte de DRC (0 violaciones) |
| `hardware/gerbers-jlcpcb/` | Gerbers de la placa que se mandó a fabricar a JLCPCB (hecha en KiCad, 98 × 75 mm) |
| `firmware/invernadero/` | Programa de Arduino |
| `docs/` | Mapa de componentes, conexión del ICSP y acomodo del cristal |

## Conexiones

| Función | Arduino Nano | ATmega328P | Conector |
|---|---|---|---|
| Luz (salida digital del módulo LDR) | D4 | PD4 (pin 6) | J3 |
| DHT11 (temperatura y humedad del aire) | D5 | PD5 (pin 11) | J4 |
| Humedad de suelo (sensor capacitivo) | A0 | PC0 (pin 23) | J6 |
| Bomba | D8 | PB0 (pin 14) | J2 |
| Ventilador | D9 | PB1 (pin 15) | J5 |

- Conectores de sensores: pin 1 = +5 V, pin 2 = señal, pin 3 = GND.
- Borneras de actuadores: pin 1 = (−), pin 2 = +5 V.
- Entrada de 12 V en J1, regulada a 5 V con un R-78E5.0-1.0.

## Componentes

| Referencia | Valor |
|---|---|
| PS1 | Regulador conmutado R-78E5.0-1.0 |
| U1 / U2 | Arduino Nano / ATmega328P-PU con zócalo DIP-28 (solo uno) |
| Y1 | Cristal de 16 MHz |
| Q1, Q2 | PN2222A (TO-92) |
| D1, D2 | 1N4007 |
| R1, R2 | 330 Ω a 1 kΩ (base de los transistores) |
| R3, R4 | 10 kΩ (pull-up de RESET y del DHT11) |
| C1 | 10 µF / 50 V (entrada) |
| C2 | 10 µF / 16 V |
| C8 | 100 µF / 16 V (salida del regulador) |
| C3, C4 | 22 pF (cristal) |
| C5, C6, C7 | 100 nF (desacople y AREF) |
| J7 | Conector ICSP 2×3 |

La bomba y el ventilador se prenden con un transistor NPN del lado bajo, con un diodo en antiparalelo con el motor (cátodo a +5 V) para protegerlo de los picos de voltaje.

## Programa

El programa (`firmware/invernadero/invernadero.ino`) necesita la librería **DHT sensor library** de Adafruit. Funciona en dos modos, que se cambian desde el Monitor Serie a 9600 baudios:

- **Automático:** el ventilador se prende si la temperatura pasa de 22 °C o la humedad del aire pasa de 70 %, y la bomba riega 3 segundos cuando la lectura del suelo pasa de 600 (tierra seca).
- **Manual:** `m` entra al modo manual, `b` prende o apaga la bomba, `v` prende o apaga el ventilador y `a` regresa al modo automático.

### Cómo programar el ATmega328P

Se usa un segundo Arduino Nano como programador (ArduinoISP):

1. Subir el ejemplo **ArduinoISP** al Nano programador y poner un capacitor de 10 µF entre su RST y GND.
2. Conectar el Nano programador al conector J7 de la placa: D12 → pin 1 (MISO), D13 → pin 3 (SCK), D11 → pin 4 (MOSI), D10 → pin 5 (RESET) y GND → pin 6. El pin 2 (+5 V) no se conecta, porque la placa se alimenta con su propio eliminador de 12 V.
3. En el Arduino IDE elegir la placa *Arduino Nano (Old Bootloader)*, el programador *Arduino as ISP* y dar **Burn Bootloader**.
4. Abrir el programa del invernadero y subirlo con **Sketch → Upload Using Programmer**.

El diagrama de la conexión está en `docs/conexion_ICSP.svg`.

## Nota sobre las referencias

El diseño de Altium y la placa fabricada (KiCad) tienen los mismos circuitos, pero algunas referencias cambian de nombre. Por ejemplo, en Altium el regulador es U1, el Nano U2 y el ATmega U3, y en la placa fabricada son PS1, U1 y U2. Los valores de la tabla de componentes usan las referencias de la placa fabricada.
