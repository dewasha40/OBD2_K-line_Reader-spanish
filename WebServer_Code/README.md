# 🌐 WebServer_Code: lector OBD2 K-Line con WiFi y panel web para ESP32

Este firmware convierte un **ESP32** o **ESP8266** en una herramienta autónoma de diagnóstico **OBD-II por WiFi** para vehículos K-Line (ISO 9141-2 y KWP2000 / ISO 14230). La placa aloja un panel web para móviles desde el que puedes consultar **datos en directo**, **códigos de avería**, **capturas congeladas**, **VIN** y más. No requiere instalar una aplicación.

← [Volver al README principal](../README.md)

<p align="center">
  <img src="../images/demo.gif" alt="ESP32 OBD2 web dashboard demo" width="340">
</p>

---

## ✨ Qué incluye

- 📶 **WiFi Access Point** (`OBD2 Master`) or **Station mode** on your own network, with optional static IP
- ⚡ **Real-time updates** over WebSocket (about every 100 ms)
- 📊 Live data with a **selectable PID list**, DTC read / clear, freeze frame, VIN / calibration IDs, 0–100 km/h timer
- 🔀 **Protocol selection** (Automatic, ISO 9141-2, KWP2000 slow / fast), saved to flash
- 🔄 **OTA firmware + web UI updates** from the browser (ESP32)
- 🔋 **Battery voltage** measurement with oversampling and smoothing
- 🔔 Buzzer melodies and LED for connection status

## 🧰 Requisitos

| | |
|---|---|
| **Board** | ESP32, ESP32-S3, ESP32-C3, ESP32-C6 or ESP8266 |
| **Interface** | Any [K-Line interface schematic](../README.md#-interface-schematics) |
| **Arduino core** | [Espressif ESP32](https://github.com/espressif/arduino-esp32) or [ESP8266](https://github.com/esp8266/Arduino) board package |
| **Libraries** | [ESPAsyncWebServer](https://github.com/ESP32Async/ESPAsyncWebServer), [AsyncTCP](https://github.com/ESP32Async/AsyncTCP) (ESP32) or [ESPAsyncTCP](https://github.com/ESP32Async/ESPAsyncTCP) (ESP8266), [ArduinoJson](https://arduinojson.org/) v7 |

## 🛠️ Preparación del hardware

Asignación predeterminada de pines (edita los `#define` al principio de `WebServer_Code.ino` para adaptarlos a la placa):

| Signal | ESP32 / ESP32-S3 | ESP8266 |
|---|:---:|:---:|
| K-Line RX | **GPIO 10** (`Serial1`) | **GPIO 3** (UART0) |
| K-Line TX | **GPIO 11** (`Serial1`) | **GPIO 1** (UART0) |
| Status LED | GPIO 6 | GPIO 2 |
| Buzzer | GPIO 8 | GPIO 4 |
| Battery voltage (ADC) | GPIO 1 | GPIO 5 |

```cpp
#define K_Serial Serial1
#define K_line_RX 10
#define K_line_TX 11
#define Led 6
#define Buzzer 8
#define voltagePin 1
```

> [!NOTE]
> - On a classic **ESP32-WROOM**, GPIO 10 / 11 are used by the SPI flash, so move K-Line RX / TX to free pins (for example GPIO 16 / 17).
> - On **ESP8266**, K-Line uses UART0, which is shared with the USB-serial chip. Debug output is therefore disabled, and you shouldn't use the USB port while the K-Line interface is connected.

**La **tensión de batería** se mide con un divisor resistivo (valores predeterminados: **R1 = 47 kΩ**, **R2 = 10 kΩ**) entre el pin 16 de OBD-II y el pin ADC. Ajusta `R1`, `R2` y `CALIBRATION_FACTOR` en `WebServer_Code.ino` a tus componentes.

## ⚙️ Instalación

### 1. Cargar el firmware

1. Open `WebServer_Code/WebServer_Code.ino` in the **Arduino IDE**.
2. Install the libraries listed above via *Library Manager*.
3. Select your board. For OTA updates on ESP32, pick a partition scheme that includes **OTA and SPIFFS** (the default 4 MB scheme works).
4. *(ESP32-S3 / C3 / C6)* Enable **USB CDC On Boot** if you want debug output on the native USB port.
5. Click **Upload**.

### 2. Cargar el panel web en SPIFFS

The dashboard lives in [`data/`](data) (pre-gzipped HTML, CSS, JS and fonts). Upload it to the board's SPIFFS partition with one of these:

| Tool | How |
|---|---|
| **Arduino IDE 2.x** | Install the [arduino-spiffs-upload](https://github.com/espx-cz/arduino-spiffs-upload) plugin, then press `Ctrl` + `Shift` + `P` and choose **Upload SPIFFS to Pico/ESP8266/ESP32** |
| **Arduino IDE 1.8.x** | [ESP32 Sketch Data Upload tool](https://randomnerdtutorials.com/install-esp32-filesystem-uploader-arduino-ide/) |
| **PlatformIO** | `pio run --target uploadfs` |

> [!IMPORTANT]
> If SPIFFS is missing or can't be mounted, the firmware halts and the LED blinks continuously. Upload the `data/` folder before first use.

### 3. Firmware precompilado *(opcional)*

When pre-compiled binaries are published on the **[Releases](https://github.com/muki01/OBD2_K-line_Reader/releases)** page, flash them with [esptool](https://docs.espressif.com/projects/esptool/) or the [ESP Web Flasher](https://espressif.github.io/esptool-js/).

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

The protocol list in Settings also shows CAN options, because the dashboard is shared with the [OBD2 CAN Bus Reader](https://github.com/muki01/OBD2_CAN_Bus_Reader). This firmware uses the K-Line options: **Automatic**, **ISO 9141-2**, **ISO 14230-4 KWP (Slow)** and **ISO 14230-4 KWP (Fast)**.

## 🔄 Actualizaciones OTA (ESP32)

Open **Settings → System Maintenance**, choose the **Resource Pack (SPIFFS `.bin`)** and **Core Firmware (`.bin`)** files, then press **Update Firmware**. A progress bar shows the upload; when it finishes the device reboots, and you reconnect to its WiFi.

- Firmware binary: Arduino IDE → **Sketch → Export Compiled Binary**
- SPIFFS image: generated by your filesystem upload tool, or `pio run --target buildfs` in PlatformIO

## 🔌 API (para tus aplicaciones)

Tu aplicación o tus scripts también pueden consultar todo lo que muestra el panel:

| Ruta | Tipo | Descripción |
|---|---|---|
| `ws://<device-ip>/ws` | WebSocket | Pushes a JSON frame about every 100 ms. Send `page0` … `page6` to choose which data set is streamed, `clear_dtc` to clear codes, `beep` to sound the buzzer |
| `GET /api/getData` | REST | One-shot JSON snapshot: selected live data, DTCs, voltage and protocol |
| `GET /api/clearDTCs` | REST | Clears stored trouble codes |
| `POST /protocolOptions` | Form | `protocol=Automatic \| ISO9141 \| ISO14230_Slow \| ISO14230_Fast` |
| `POST /pidSelect` | Form | Sets the PIDs shown in Live Data |
| `POST /wifiOptions` | Form | `SSID`, `WifiPassword`, `ipAddr`, `subnetMask`, `gateway` (device restarts) |

Example live-data frame (`page1`):

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
