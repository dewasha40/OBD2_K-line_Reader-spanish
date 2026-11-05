# Política de Seguridad

## Versiones compatibles

Las correcciones de seguridad se aplican al último código en la rama `main` y a la última versión publicada.

## Informar sobre una Vulnerabilidad

Si encuentras un problema de seguridad —por ejemplo, en el servidor web, el punto de actualización OTA o la gestión de la configuración WiFi—, **no abras un problema público**.

En su lugar, envía un correo electrónico a **muksin.muksin04@gmail.com** con:

- Una descripción del problema y su posible impacto
- Pasos para reproducirlo (placa, compilación, configuración)
- Cualquier solución sugerida, si la tiene

Recibirás una respuesta lo antes posible, y, si lo deseas, se te reconocerá en las notas de la versión.

## Recomendaciones de seguridad

- Cambia la contraseña predeterminada del punto de acceso (`AP_password` en `WebServer_Code/WEB_SERVER.ino`) antes de usarlo habitualmente.
- El panel web y el punto final de actualización OTA no tienen autenticación. Conecta el dispositivo únicamente a redes de confianza.
- Desconecta el dispositivo del vehículo cuando no lo uses.
