# 🖥️ Basic_Code: OBD2 K-Line Reader para Monitor Serie

Programa ligero para **Arduino / ESP32** que se comunica con la ECU del coche mediante **K-Line** (ISO 9141-2 y KWP2000 / ISO 14230) y muestra datos de sensores, códigos de avería, captura congelada y PID admitidos en el **Monitor Serie**.

Úsalo para comprobar la interfaz K-Line, aprender el protocolo o como punto de partida para tu propio firmware. Para el panel WiFi, consulta **[WebServer_Code](../WebServer_Code/README.md)**.

← [Volver al README principal](../README.md)

---

## 📡 Cómo funciona

El programa utiliza **dos UART**:

| UART | Función | Arduino (AVR) | ESP32 |
|---|---|---|---|
| **Principal** (`Serial`) | Conexión USB al ordenador / Monitor Serie | `Serial` de hardware | `Serial` de hardware / USB-CDC |
| **K-Line** (`K_Serial`) | Comunicación con el coche a 10,4 kbaudios mediante el circuito de interfaz | `AltSoftSerial` (UART por software) | `Serial1` de hardware |

Al arrancar, el firmware activa la ECU (inicialización lenta a 5 baudios o rápida), detecta el protocolo y comienza a consultar datos. Si se pierde el enlace, intenta reconectarse automáticamente.

## 🛠️ Preparación del hardware

Construye uno de los [esquemas de interfaz K-Line](../README.md#-esquemas-de-interfaz) y conecta sus líneas **RX** y **TX** a:

| Placa | RX K-Line | TX K-Line | LED de estado | Notas |
|---|:---:|:---:|:---:|---|
| Arduino Uno / Nano / Pro Mini | **D8** | **D9** | D13 | Pines fijados por AltSoftSerial |
| ESP32-S3 / C3 / C6 | **GPIO 10** | **GPIO 11** | GPIO 6 | Configurable en `Basic_Code.ino` |

> [!NOTE]
> En un **ESP32-WROOM** clásico, GPIO 10 / 11 están conectados a la memoria flash SPI. Cambia `K_line_RX` / `K_line_TX` por pines libres (por ejemplo GPIO 16 / 17).

Conecta la interfaz al **pin 7 de OBD-II** (K-Line), al **pin 16** (+12 V) y a los **pines 4 / 5** (GND). Consulta el pinout completo en el [README principal](../README.md#conector-obd-ii-k-line).

## ⚙️ Instalación

### Opción 1: Arduino IDE

1. Instala **[Arduino IDE](https://www.arduino.cc/en/software)** (se recomienda la versión 2.x).
2. Para ESP32, añade el paquete de placas **Espressif ESP32** desde el *Gestor de placas*.
3. Para placas Arduino AVR, instala **AltSoftSerial** desde el *Gestor de bibliotecas*.
4. Abre `Basic_Code/Basic_Code.ino`. Los demás archivos `.ino` de la carpeta se abrirán automáticamente como pestañas.
5. Selecciona la placa y el puerto, y pulsa **Cargar**.
6. Abre el **Monitor Serie** a **9600 baudios**.

### Opción 2: Firmware precompilado

Cuando se publiquen binarios precompilados en la página de **[versiones](https://github.com/muki01/OBD2_K-line_Reader/releases)**, puedes cargarlos directamente con [esptool](https://docs.espressif.com/projects/esptool/) o [ESP Web Flasher](https://espressif.github.io/esptool-js/).

## 🔧 Configuración

Los ajustes están al principio de `Basic_Code.ino`:

```cpp
String selectedProtocol = "Automatic";   // "Automatic", "ISO9141", "ISO14230_Slow", "ISO14230_Fast"

int _byteWriteInterval = 5;   // Delay between transmitted bytes (5 - 20 ms)
int _interByteTimeout  = 60;  // Wait after the last received byte (55 - 5000 ms)
int _readTimeout       = 1000;
```

- Deja `selectedProtocol` en **`Automatic`** salvo que conozcas el protocolo del coche. Seleccionarlo manualmente omite la detección y acelera la conexión.
- Comenta `#define DEBUG_Serial` para desactivar toda la salida serie.

## 📋 Datos que muestra

Cada ciclo de consulta muestra:

- **Datos en directo**: velocidad, RPM del motor, temperatura del refrigerante, temperatura del aire de admisión, posición del acelerador, avance de encendido, carga del motor y caudal MAF
- **DTC almacenados** (modo 03) y **DTC pendientes** (modo 07)
- **Captura congelada** (modo 02): velocidad, RPM, temperatura del refrigerante y carga del motor en el momento de la avería
- **PID admitidos** para datos en directo, captura congelada e información del vehículo

## 🖼️ Ejemplo de salida

Ejemplo del Monitor Serie en un vehículo con **ISO 9141-2**:

<img src="https://github.com/user-attachments/assets/0ca043ea-6152-40a5-8b22-c516e84031bb" alt="Serial Monitor output of the OBD2 K-Line Reader showing live data and DTCs from an ISO 9141-2 car" width="320">

## 🩹 Solución de problemas

| Síntoma | Qué comprobar |
|---|---|
| Sin respuesta o falla la inicialización | Contacto puesto (motor apagado), pin 7 conectado, masa común y líneas RX/TX de la interfaz sin invertir |
| Caracteres ilegibles | Configura el Monitor Serie a **9600 baudios** |
| Se conecta y se desconecta | Aumenta `_interByteTimeout` y comprueba la alimentación de 12 V de la interfaz |
| El ESP32 se reinicia o no arranca | Evita pines de flash o de configuración de arranque para RX/TX K-Line |
