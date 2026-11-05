# 🌐 WebServer_Code: lector OBD2 K-Line con WiFi y panel web para ESP32

Este firmware convierte un **ESP32** o **ESP8266** en una herramienta autónoma de diagnóstico **OBD-II por WiFi** para vehículos K-Line (ISO 9141-2 y KWP2000 / ISO 14230). La placa aloja un panel web para móviles desde el que puedes consultar **datos en directo**, **códigos de avería**, **capturas congeladas**, **VIN** y más. No requiere instalar una aplicación.

← [Volver al README principal](../README.md)

<p align="center">
  <img src="../images/demo.gif" alt="ESP32 OBD2 web dashboard demo" width="340">
</p>

---

## ✨ Qué incluye

- 📶 **Punto de acceso WiFi** (`OBD2 Master`) o **modo Estación** en tu propia red, con IP estática opcional
- ⚡ **Actualizaciones en tiempo real** por WebSocket (cada unos 100 ms)
- 📊 Datos en directo con una **lista de PID seleccionable**, lectura / borrado de DTC, captura congelada, VIN / identificadores de calibración, temporizador de 0–100 km/h
- 🔀 **Selección de protocolo** (Automático, ISO 9141-2, KWP2000 lento / rápido), guardado en flash
- 🔄 **Actualizaciones OTA del firmware + interfaz web** desde el navegador (ESP32)
- 🔋 **Medición de tensión de batería** con sobremuestreo y suavizado
- 🔔 Melodías de buzzer y LED para el estado de conexión

## 🧰 Requisitos

| | |
|---|---|
| **Placa** | ESP32, ESP32-S3, ESP32-C3, ESP32-C6 o ESP8266 |
| **Interfaz** | Cualquier [esquema de interfaz K-Line](../README.md#-interface-schematics) |
| **Core de Arduino** | [Espressif ESP32](https://github.com/espressif/arduino-esp32) o [ESP8266](https://github.com/esp8266/Arduino) board package |
| **Bibliotecas** | [ESPAsyncWebServer](https://github.com/ESP32Async/ESPAsyncWebServer), [AsyncTCP](https://github.com/ESP32Async/AsyncTCP) (ESP32) o [ESPAsyncTCP](https://github.com/ESP32Async/ESPAsyncTCP) (ESP8266), [ArduinoJson](https://arduinojson.org/) v7 |

## 🛠️ Preparación del hardware

Asignación predeterminada de pines (edita los `#define` al principio de `WebServer_Code.ino` para adaptarlos a la placa):

| Señal | ESP32 / ESP32-S3 | ESP8266 |
|---|:---:|:---:|
| RX de K-Line | **GPIO 10** (`Serial1`) | **GPIO 3** (UART0) |
| TX de K-Line | **GPIO 11** (`Serial1`) | **GPIO 1** (UART0) |
| LED de estado | GPIO 6 | GPIO 2 |
| Zumbador | GPIO 8 | GPIO 4 |
| Tensión de batería (ADC) | GPIO 1 | GPIO 5 |

```cpp
#define K_Serial Serial1
#define K_line_RX 10
#define K_line_TX 11
#define Led 6
#define Buzzer 8
#define voltagePin 1
```

> [!NOTE]
> - En una placa clásica **ESP32-WROOM**, los GPIO 10 / 11 se utilizan para la memoria flash SPI, así que mueve RX / TX de K-Line a pines libres (por ejemplo, GPIO 16 / 17).
> - En **ESP8266**, K-Line utiliza UART0, que se comparte con el chip USB-serial. Por lo tanto, la salida de depuración está deshabilitada, y no debes utilizar el puerto USB mientras la interfaz K-Line esté conectada.

**La **tensión de batería** se mide con un divisor resistivo (valores predeterminados: **R1 = 47 kΩ**, **R2 = 10 kΩ**) entre el pin 16 de OBD-II y el pin ADC. Ajusta `R1`, `R2` y `CALIBRATION_FACTOR` en `WebServer_Code.ino` a tus componentes.

## ⚙️ Instalación

### 1. Cargar el firmware

1. Abre `WebServer_Code/WebServer_Code.ino` en el **Arduino IDE**.
2. Instala las bibliotecas mencionadas anteriormente mediante el *Library Manager*.
3. Selecciona tu placa. Para actualizaciones OTA en ESP32, elige un esquema de partición que incluya **OTA y SPIFFS** (el esquema predeterminado de 4 MB funciona).
4. *(ESP32-S3 / C3 / C6)* Activa **USB CDC On Boot** si deseas salida de depuración en el puerto USB nativo.
5. Haz clic en **Subir**.

### 2. Cargar el panel web en SPIFFS

El panel de control vive en [`data/`](data) (HTML, CSS, JS y fuentes precompresos). Carga el contenido en la partición SPIFFS de la placa con una de estas opciones:

| Herramienta | Cómo |
|---|---|
| **Arduino IDE 2.x** | Instala el plugin [arduino-spiffs-upload](https://github.com/espx-cz/arduino-spiffs-upload), luego pulsa `Ctrl` + `Shift` + `P` y elige **Subir SPIFFS a Pico/ESP8266/ESP32** |
| **Arduino IDE 1.8.x** | [Herramienta de subida de datos del esquema ESP32](https://randomnerdtutorials.com/install-esp32-filesystem-uploader-arduino-ide/) |
| **PlatformIO** | `pio run --target uploadfs` |

> [!IMPORTANT]
> Si SPIFFS no está presente o no puede montarse, el firmware se detiene y el LED parpadea continuamente. Sube la carpeta `data/` antes del primer uso.

### 3. Firmware precompilado *(opcional)*

Cuando se publican binarios precompilados en la página **[Releases](https://github.com/muki01/OBD2_K-line_Reader/releases)**, flashéalos con [esptool](https://docs.espressif.com/projects/esptool/) o con el [ESP Web Flasher](https://espressif.github.io/esptool-js/).

## 📱 Primera conexión

1. Conecta el dispositivo al puerto OBD-II y pon el contacto.
2. En el teléfono, conéctate a la WiFi **`OBD2 Master`**. La contraseña es **`12345678`**.
3. Abre **http://192.168.4.1** en el navegador.
4. Los dos iconos de la cabecera muestran el estado de conexión **WebSocket** y del **vehículo** (verde significa conectado).

### Conectarse a tu propia red WiFi (modo estación)

En **Ajustes → Configuración de red**, introduce el SSID y la contraseña (y, opcionalmente, una IP estática) y pulsa **Actualizar red**. El dispositivo se reinicia y se conecta a tu red. Si no logra conectarse en unos segundos, vuelve automáticamente al modo punto de acceso.

> [!TIP]
> Cambia `AP_password` en `WEB_SERVER.ino` antes de usar el dispositivo habitualmente.

## 🧭 Uso del panel

| Página | Función |
|---|---|
| **Menú principal** | Tensión de batería (verde > 12,6 V; amarillo 12,0–12,6 V; rojo < 12,0 V) y navegación |
| **Datos en directo** | Valores en tiempo real de los PID elegidos en *Ajustes → Parámetros monitorizados* |
| **Códigos de avería** | DTC almacenados con descripciones y opción **Borrar códigos de avería** (modo 04) |
| **Captura congelada** | Valores de sensores capturados por la ECU al guardar la avería |
| **Prueba de velocidad** | Pulsa **Iniciar** con el vehículo parado; el cronómetro comienza al superar 0 km/h y se detiene a 100 km/h |
| **Información del vehículo** | VIN, ID y versión de calibración, y mapas de PID admitidos |
| **Ajustes** | Tema, protocolo, selección de PID, WiFi y actualización del firmware |

> [!TIP]
> K-Line es lenta (10,4 kbaudios), por lo que cada PID añade latencia. **Selecciona solo los PID necesarios** en *Parámetros monitorizados* para actualizar más rápido los datos.

La lista de protocolos en Ajustes también muestra opciones CAN, porque el panel se comparte con el [Lector de bus CAN OBD2](https://github.com/muki01/OBD2_CAN_Bus_Reader). Este firmware utiliza las opciones de K-Line: **Automático**, **ISO 9141-2**, **ISO 14230-4 KWP (Lento)** y **ISO 14230-4 KWP (Rápido)**.

## 🔄 Actualizaciones OTA (ESP32)

Abre **Ajustes → Mantenimiento del sistema**, elige los archivos **Resource Pack (SPIFFS `.bin`)** y **Core Firmware (`.bin`)**, luego pulsa **Actualizar Firmware**. Una barra de progreso muestra la subida; cuando termina, el dispositivo se reinicia y te reconectas a su WiFi.

- Firmware binario: Arduino IDE → **Esquema → Exportar binario compilado**
- Imagen SPIFFS: generada por tu herramienta de subida de sistema de archivos, o `pio run --target buildfs` en PlatformIO

## 🔌 API (para tus aplicaciones)

Tu aplicación o tus scripts también pueden consultar todo lo que muestra el panel:
| Ruta | Tipo | Descripción |
|---|---|---|
| `ws://<device-ip>/ws` | WebSocket | Envía un marco JSON aproximadamente cada 100 ms. Envía `page0` … `page6` para elegir qué conjunto de datos se transmite, `clear_dtc` para borrar códigos, `beep` para activar el zumbador |
| `GET /api/getData` | REST | Captura instantánea JSON de un solo uso: datos en vivo seleccionados, códigos de diagnóstico, voltaje y protocolo |
| `GET /api/clearDTCs` | REST | Borra los códigos de fallo almacenados |
| `POST /protocolOptions` | Form | `protocol=Automatic \| ISO9141 \| ISO14230_Slow \| ISO14230_Fast` |
| `POST /pidSelect` | Form | Establece los PIDs mostrados en Datos en vivo |
| `POST /wifiOptions` | Form | `SSID`, `WifiPassword`, `ipAddr`, `subnetMask`, `gateway` (reinicio del dispositivo) |

Ejemplo de marco de datos en vivo (`page1`):

```json
{
  "LiveData": {
    "RPM":          { "value": 812,  "unit": "rpm" },
    "Coolant Temp": { "value": 88,   "unit": "°C" },
    "Engine Load":  { "value": 24.3, "unit": "%" }
  },
  "selectedProtocol": "Automatic",
  "connectedProtocol": "ISO14230_Fast",
  "Voltage": 14.08,
  "vehicleStatus": true
}
```

## 🩹 Solución de problemas

| Síntoma | Qué comprobar |
|---|---|
| El LED parpadea continuamente tras el arranque | No se ha cargado SPIFFS; consulta el paso 2 |
| No aparece la WiFi `OBD2 Master` | Espera unos 5 segundos; primero intenta conectarse a la red guardada en modo estación |
| Se abre la página, pero el icono del vehículo sigue rojo | ¿Está puesto el contacto? ¿Hay contacto en el pin 7? Prueba a seleccionar un protocolo en Ajustes |
| Los datos se actualizan despacio | Reduce la cantidad de PID seleccionados |
| La tensión de batería es incorrecta | Calibra `R1`, `R2` y `CALIBRATION_FACTOR` |

## 🎨 Código fuente de la interfaz web

La interfaz del panel se mantiene en un repositorio independiente: **[OBD2 Diagnostic UI](https://github.com/muki01/OBD2-Diagnostic-UI)**. Los archivos de `data/` contienen la versión de producción comprimida con gzip.
