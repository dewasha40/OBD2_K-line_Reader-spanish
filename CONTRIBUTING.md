# Contribuyendo al OBD2 K-Line Reader

Gracias por dedicar tiempo para contribuir. 🎉 Cada informe de errores, prueba de vehículo, corrección e idea mejora este proyecto para toda la comunidad de hacking de coches y makers.

Al participar, aceptas seguir nuestro [Código de Conducta](CODE_OF_CONDUCT.md).

## Formas de Contribuir

| | |
|---|---|
| 🚗 **Informa sobre un vehículo probado** | Dinos qué coches funcionan (o no). Usa la plantilla de **Informe de compatibilidad de vehículos**. |
| 🐛 **Informa un error** | Usa la plantilla de **Informe de error** e incluye registros seriales siempre que sea posible. |
| 💡 **Sugiere una función** | Usa la plantilla de **Solicitud de función**. |
| 🔧 **Envía código** | Nuevos PIDs, puertos de placa (STM32, RP2040, …), correcciones de protocolo, mejoras de rendimiento. |
| 📝 **Mejora la documentación** | Errores tipográficos, instrucciones más claras, fotos de conexión, traducciones de guías. |

## Informes de Errores

Antes de abrir un problema, por favor busca [problemas existentes](https://github.com/muki01/OBD2_K-line_Reader/issues). Un buen informe incluye:

- Construcción (`Basic_Code` o `WebServer_Code`) y versión de commit / lanzamiento
- Placa (p. ej. ESP32-S3 DevKitC-1, Arduino Nano) y versión del núcleo de Arduino
- Circuito de interfaz utilizado (transistor, LM393, L9637D, …)
- Marca, modelo, año y motor del vehículo
- Protocolo seleccionado y detectado
- **Salida de depuración serial** (habilita `DEBUG_Serial`). Los bytes brutos `➡️ Sending Data` / recibidos son la información más útil.

## Flujo de Desarrollo

1. **Fork** el repositorio y crea una rama desde `main`:
   ```bash
   git checkout -b feature/my-improvement
   ```
2. Haz tus cambios, manteniéndolos **enfocados**: una corrección o función por solicitud de extracción.
3. **Prueba en hardware real** cuando tu cambio afecte código de comunicación, y di en la PR qué vehículo / protocolo probaste.
4. Asegúrate de que ambos esquemas aún **compilen** para las placas que soportan.
5. Haz un commit con un mensaje claro, p. ej. `Add PID 0x5C oil temperature`.
6. Sube y abre una **Solicitud de Extracción** contra `main`, rellenando la plantilla.

## Guías de Codificación

- Sigue el estilo existente del archivo que estás editando (nombres, indentación, densidad de comentarios).
- Envuelve cadenas de depuración constantes en `F()` para ahorrar RAM en placas AVR.
- Mantén el código crítico de temporización de K-Line **no bloqueante** cuando sea posible y no cambies los tiempos de protocolo sin probar.
- Coloca código específico de placa detrás de `#ifdef ESP32` / `#elif defined(ESP8266)` / `#elif defined(ARDUINO)`.
- No comprometas binarios, credenciales o datos personales (p. ej. tu propio VIN) en el código o en los registros.

## Cambios en el Panel Web

La fuente de la interfaz web se encuentra en **[OBD2 Diagnostic UI](https://github.com/muki01/OBD2-Diagnostic-UI)**. Por favor, abre solicitudes de extracción de UI allí; `WebServer_Code/data` solo contiene la construcción comprimida.

## Licencia

Al contribuir, aceptas que tus contribuciones se licencian bajo la [Licencia Pública General de GNU v3.0](LICENSE), y que el autor también puede ofrecerlas bajo una licencia comercial.
