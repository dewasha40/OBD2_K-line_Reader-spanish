String decodeDTC(char input_byte1, char input_byte2) {
  String ErrorCode = "";
  const static char type_lookup[4] = { 'P', 'C', 'B', 'U' };
  const static char digit_lookup[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

  ErrorCode += type_lookup[(input_byte1 >> 6) & 0b11];
  ErrorCode += digit_lookup[(input_byte1 >> 4) & 0b11];
  ErrorCode += digit_lookup[input_byte1 & 0b1111];
  ErrorCode += digit_lookup[input_byte2 >> 4];
  ErrorCode += digit_lookup[input_byte2 & 0b1111];

  return ErrorCode;
}

byte calculateChecksum(const byte data[], int length) {
  byte checksum = 0;
  for (int i = 0; i < length; i++) {
    checksum += data[i];
  }
  return checksum % 256;
}

bool isInArray(byte arr[], int size, byte value) {
  for (int i = 0; i < size; i++) {
    if (arr[i] == value) {
      return true;
    }
  }
  return false;
}

String convertHexToAscii(byte* hexBytes, size_t length) {
  String asciiString = "";
  for (int i = 0; i < length; i++) {
    if (hexBytes[i] >= 0x20 && hexBytes[i] <= 0x7E) {
      char asciiChar = (char)hexBytes[i];
      asciiString += asciiChar;
    }
  }
  return asciiString;
}

String convertBytesToHexString(byte* buffer, int length) {
  String hexString = "";
  for (int i = 0; i < length; i++) {
    if (buffer[i] < 0x10) {
      hexString += "0";
    }
    hexString += String(buffer[i], HEX);
  }
  hexString.toUpperCase();
  return hexString;
}

String convertBytesToHexStringWithComma(byte arr[], int length) {
  String hexString = "";
  for (int i = 0; i < length; i++) {
    if (arr[i] == 0) {
      break;
    }
    if (i > 0) {
      hexString += ", ";
    }
    if (arr[i] < 0x10) {
      hexString += "0";
    }
    hexString += String(arr[i], HEX);
  }
  hexString.toUpperCase();
  return hexString;
}

String joinStringsWithComma(String arr[], int length) {
  String result = "";
  for (int i = 0; i < length; i++) {
    if (arr[i] == 0) {
      break;
    }
    if (i > 0) {
      result += ", ";
    }
    result += arr[i];
  }
  return result;
}

void printHex(uint8_t val) {
  if (val < 0x10) Serial.print("0");
  Serial.print(val, HEX);
}

void setSerial(bool enabled) {
  if (enabled) {
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__)
    K_Serial.begin(10400);
#elif defined(ESP8266)
    K_Serial.begin(10400, SERIAL_8N1);
#elif defined(ESP32)
    K_Serial.begin(10400, SERIAL_8N1, K_line_RX, K_line_TX);
#endif
  } else {
    K_Serial.end();
    pinMode(K_line_RX, INPUT_PULLUP);
    pinMode(K_line_TX, OUTPUT);
    digitalWrite(K_line_TX, HIGH);
  }
}


void connectMelody() {
  tone(Buzzer, 660, 50);
  delay(80);
  tone(Buzzer, 1320, 55);
  delay(70);
  tone(Buzzer, 1760, 100);
}

void disconnectMelody() {
  tone(Buzzer, 660, 50);
  delay(80);
  tone(Buzzer, 1760, 55);
  delay(70);
  tone(Buzzer, 1320, 100);
}

void Melody1() {
  tone(Buzzer, 1100, 50);
  delay(10);
  tone(Buzzer, 1300, 50);
}

void Melody2() {
  tone(Buzzer, 1320, 50);
  delay(10);
  tone(Buzzer, 1570, 50);
}

void Melody3() {
  tone(Buzzer, 500, 40);
  tone(Buzzer, 600, 60);
}

void BlinkLed(int time, int count) {
  for (int i = 1; i <= count; i++) {
    digitalWrite(Led, LOW);
    delay(time);
    digitalWrite(Led, HIGH);
    if (i != count) {
      delay(time);
    }
  }
}

struct PidMapping {
  byte pid;
  const char* jsonKey;
  float value;
  const char* unit;
};

PidMapping liveDataMappings[] = {
  { 0x01, "Estado del Monitor Desde que se Eliminó el CÓDIGO DE FALLO", 0, "BIT" },
  { 0x03, "Estado del Sistema de Combustible", 0, "BIT" },
  { 0x04, "Carga del Motor", 0, "%" },
  { 0x05, "Temperatura del Líquido de Enfriamiento", 0, "°C" },
  { 0x06, "Ajuste de Combustible a Corto Plazo Banco 1", 0, "%" },
  { 0x07, "Ajuste de Combustible a Largo Plazo Banco 1", 0, "%" },
  { 0x08, "Ajuste de Combustible a Corto Plazo Banco 2", 0, "%" },
  { 0x09, "Ajuste de Combustible a Largo Plazo Banco 2", 0, "%" },
  { 0x0A, "Presión de Combustible", 0, "kPa" },
  { 0x0B, "Presión Absoluta en la Caja de Admisión", 0, "kPa" },
  { 0x0C, "RPM", 0, "rpm" },
  { 0x0D, "Velocidad", 0, "km/h" },
  { 0x0E, "Avance de Encendido (antes del PUNTO MUERTO SUPERIOR)", 0, "°" },
  { 0x0F, "Temperatura del Aire de Admisión", 0, "°C" },
  { 0x10, "Tasa de Flujo de Aire (MAF)", 0, "g/s" },
  { 0x11, "Posición de la Mariposa", 0, "%" },
  { 0x12, "Estado del Aire Secundario Comandado", 0, "BIT" },
  { 0x13, "Sensores de Oxígeno Presentes 2 Bancos", 0, "BIT" },
  { 0x14, "Sensor de Oxígeno 1A", 0, "V %" },
  { 0x15, "Sensor de Oxígeno 2A", 0, "V %" },
  { 0x16, "Sensor de Oxígeno 3A", 0, "V %" },
  { 0x17, "Sensor de Oxígeno 4A", 0, "V %" },
  { 0x18, "Sensor de Oxígeno 5A", 0, "V %" },
  { 0x19, "Sensor de Oxígeno 6A", 0, "V %" },
  { 0x1A, "Sensor de Oxígeno 7A", 0, "V %" },
  { 0x1B, "Sensor de Oxígeno 8A", 0, "V %" },
  { 0x1C, "Estándares OBD", 0, "BIT" },
  { 0x1D, "Sensores de Oxígeno Presentes 4 Bancos", 0, "BIT" },
  { 0x1E, "Estado de Entrada Auxiliar", 0, "BIT" },
  { 0x1F, "Tiempo de Funcionamiento Desde el Encendido del Motor", 0, "sec" },

  { 0x21, "Distancia Recorrida con MIL Encendido", 0, "km" },
  { 0x22, "Presión en la Tubería de Combustible", 0, "kPa" },
  { 0x23, "Presión en la Tubería de Combustible (Manométrica)", 0, "kPa" },
  { 0x24, "Sensor de Oxígeno 1B", 0, "ratio V" },
  { 0x25, "Sensor de Oxígeno 2B", 0, "ratio V" },
  { 0x26, "Sensor de Oxígeno 3B", 0, "ratio V" },
  { 0x27, "Sensor de Oxígeno 4B", 0, "ratio V" },
  { 0x28, "Sensor de Oxígeno 5B", 0, "ratio V" },
  { 0x29, "Sensor de Oxígeno 6B", 0, "ratio V" },
  { 0x2A, "Sensor de Oxígeno 7B", 0, "ratio V" },
  { 0x2B, "Sensor de Oxígeno 8B", 0, "ratio V" },
  { 0x2C, "EGR Comandado", 0, "%" },
  { 0x2D, "Error en EGR", 0, "%" },
  { 0x2E, "Evaporative Purge Comandado", 0, "%" },
  { 0x2F, "Nivel de Tanque de Combustible", 0, "%" },
  { 0x30, "Tiempo de Calentamiento UPS Desde que se Limpiaron los Códigos", 0, "count" },
  { 0x31, "Distancia Recorrida Desde que se Limpiaron los Códigos", 0, "km" },
  { 0x32, "Presión de Vapor del Sistema de Evaporación", 0, "Pa" },
  { 0x33, "Presión Barométrica ABS", 0, "kPa" },
  { 0x34, "Sensor de Oxígeno 1C", 0, "ratio mA" },
  { 0x35, "Sensor de Oxígeno 2C", 0, "ratio mA" },
  { 0x36, "Sensor de Oxígeno 3C", 0, "ratio mA" },
  { 0x37, "Sensor de Oxígeno 4C", 0, "ratio mA" },
  { 0x38, "Sensor de Oxígeno 5C", 0, "ratio mA" },
  { 0x39, "Sensor de Oxígeno 6C", 0, "ratio mA" },
  { 0x3A, "Sensor de Oxígeno 7C", 0, "ratio mA" },
  { 0x3B, "Sensor de Oxígeno 8C", 0, "ratio mA" },
  { 0x3C, "Temperatura del Catalizador Banco 1 Sensor 1", 0, "°C" },
  { 0x3D, "Temperatura del Catalizador Banco 2 Sensor 1", 0, "°C" },
  { 0x3E, "Temperatura del Catalizador Banco 1 Sensor 2", 0, "°C" },
  { 0x3F, "Temp. Catalizador Banco 2 Sensor 2", 0, "°C" },

  { 0x41, "Estado de Monitorización Este Ciclo de Conducción", 0, "BIT" },
  { 0x42, "Voltaje Módulo de Control", 0, "V" },
  { 0x43, "Valor de Carga Absoluta", 0, "%" },
  { 0x44, "Relación Equivalencia Combustible/Aire Comandada", 0, "ratio" },
  { 0x45, "Posición del Acelerador Relativa", 0, "%" },
  { 0x46, "Temperatura del Aire Ambiente", 0, "°C" },
  { 0x47, "Posición del Acelerador Absoluta B", 0, "%" },
  { 0x48, "Posición del Acelerador Absoluta C", 0, "%" },
  { 0x49, "Posición del Acelerador Absoluta D", 0, "%" },
  { 0x4A, "Posición del Acelerador Absoluta E", 0, "%" },
  { 0x4B, "Posición del Acelerador Absoluta F", 0, "%" },
  { 0x4C, "Actuador del Acelerador Comandado", 0, "%" },
  { 0x4D, "Tiempo de Conducción con MIL Encendido", 0, "min" },
  { 0x4E, "Tiempo Desde que se Limpiaron los Códigos de Error", 0, "min" },
  { 0x4F, "Valores Máximos: Relación Equivalencia, Voltaje, Corriente, Presión", 0, "ratio V mA kPa" },
  { 0x50, "Tasa Máxima de Flujo MAF", 0, "g/s" },
  { 0x51, "Tipo de Combustible", 0, "ref table" },
  { 0x52, "Porcentaje de Combustible Etanol", 0, "%" },
  { 0x53, "Presión Absoluta del Sistema de Evaporación", 0, "kPa" },
  { 0x54, "Presión de vapor del sistema de evaporación", 0, "Pa" },
  { 0x55, "Ajuste a corto plazo del sensor secundario de O2 Banco 1–3", 0, "%" },
  { 0x56, "Ajuste a largo plazo del sensor secundario de O2 Banco 1–3", 0, "%" },
  { 0x57, "Ajuste a corto plazo del sensor secundario de O2 Banco 2–4", 0, "%" },
  { 0x58, "Ajuste a largo plazo del sensor secundario de O2 Banco 2–4", 0, "%" },
  { 0x59, "Presión absoluta del conducto de combustible", 0, "kPa" },
  { 0x5A, "Posición relativa del pedal de aceleración", 0, "%" },
  { 0x5B, "Vida restante de la batería híbrida", 0, "%" },
  { 0x5C, "Temperatura del aceite del motor", 0, "°C" },
  { 0x5D, "Tiempo de inyección de combustible", 0, "°" },
  { 0x5E, "Consumo de combustible del motor", 0, "L/h" },
  { 0x5F, "Requisitos de emisiones", 0, "BIT" }
};

PidMapping freezeFrameMappings[] = {
  { 0x01, "Estado del monitor desde la última limpieza del código DTC", 0, "BIT" },
  { 0x03, "Estado del sistema de combustible", 0, "BIT" },
  { 0x04, "Carga del motor", 0, "%" },
  { 0x05, "Temperatura del líquido de enfriamiento", 0, "°C" },
  { 0x06, "Ajuste a corto plazo del inyector de combustible Banco 1", 0, "%" },
  { 0x07, "Ajuste a largo plazo del inyector de combustible Banco 1", 0, "%" },
  { 0x08, "Ajuste a corto plazo del inyector de combustible Banco 2", 0, "%" },
  { 0x09, "Ajuste a largo plazo del inyector de combustible Banco 2", 0, "%" },
  { 0x0A, "Presión de Combustible", 0, "kPa" },
  { 0x0B, "Presión Absoluta de la Caja de Entrada", 0, "kPa" },
  { 0x0C, "RPM", 0, "rpm" },
  { 0x0D, "Velocidad", 0, "km/h" },
  { 0x0E, "Avance de Encendido (antes del PMS)", 0, "°" },
  { 0x0F, "Temperatura del Aire de Entrada", 0, "°C" },
  { 0x10, "Caudal de Aire (MAF)", 0, "g/s" },
  { 0x11, "Posición de la Mariposa", 0, "%" },
  { 0x12, "Estado de la Succión Secundaria Comandada", 0, "BIT" },
  { 0x13, "Sensores de Oxígeno Presentes 2 Bancos", 0, "BIT" },
  { 0x14, "Sensor de Oxígeno 1A", 0, "V %" },
  { 0x15, "Sensor de Oxígeno 2A", 0, "V %" },
  { 0x16, "Sensor de Oxígeno 3A", 0, "V %" },
  { 0x17, "Sensor de Oxígeno 4A", 0, "V %" },
  { 0x18, "Sensor de Oxígeno 5A", 0, "V %" },
  { 0x19, "Sensor de Oxígeno 6A", 0, "V %" },
  { 0x1A, "Sensor de Oxígeno 7A", 0, "V %" },
  { 0x1B, "Sensor de Oxígeno 8A", 0, "V %" },
  { 0x1C, "Estándares OBD", 0, "BIT" },
  { 0x1D, "Sensores de Oxígeno Presentes 4 Bancos", 0, "BIT" },
  { 0x1E, "Estado de Entrada Auxiliar", 0, "BIT" },
  { 0x1F, "Tiempo de Funcionamiento Desde el Arranque del Motor", 0, "sec" },

  { 0x21, "Distancia Recorrida con MIL Encendido", 0, "km" },
  { 0x22, "Presión en la Tubería de Combustible", 0, "kPa" },
  { 0x23, "Presión en la Tubería de Combustible (Manométrica)", 0, "kPa" },
  { 0x24, "Sensor de Oxígeno 1B", 0, "ratio V" },
  { 0x25, "Sensor de Oxígeno 2B", 0, "ratio V" },
  { 0x26, "Sensor de Oxígeno 3B", 0, "ratio V" },
  { 0x27, "Sensor de Oxígeno 4B", 0, "ratio V" },
  { 0x28, "Sensor de Oxígeno 5B", 0, "ratio V" },
  { 0x29, "Sensor de Oxígeno 6B", 0, "ratio V" },
  { 0x2A, "Sensor de Oxígeno 7B", 0, "ratio V" },
  { 0x2B, "Sensor de Oxígeno 8B", 0, "ratio V" },
  { 0x2C, "EGR Comandada", 0, "%" },
  { 0x2D, "Error en EGR", 0, "%" },
  { 0x2E, "Evaporative Purge Comandada", 0, "%" },
  { 0x2F, "Entrada de Nivel del Tanque de Combustible", 0, "%" },
  { 0x30, "Tiempo de Calentamiento del UPS Desde que se Limpiaron los Códigos", 0, "count" },
  { 0x31, "Distancia Recorrida Desde que se Limpiaron los Códigos", 0, "km" },
  { 0x32, "Presión en el Sistema de Evaporación", 0, "Pa" },
  { 0x33, "Presión Barométrica ABS", 0, "kPa" },
  { 0x34, "Sensor de Oxígeno 1C", 0, "ratio mA" },
  { 0x35, "Sensor de Oxígeno 2C", 0, "ratio mA" },
  { 0x36, "Sensor de Oxígeno 3C", 0, "ratio mA" },
  { 0x37, "Sensor de Oxígeno 4C", 0, "ratio mA" },
  { 0x38, "Sensor de Oxígeno 5C", 0, "ratio mA" },
  { 0x39, "Sensor de Oxígeno 6C", 0, "ratio mA" },
  { 0x3A, "Sensor de Oxígeno 7C", 0, "ratio mA" },
  { 0x3B, "Sensor de Oxígeno 8C", 0, "ratio mA" },
  { 0x3C, "Temperatura del Catalizador Banco 1 Sensor 1", 0, "°C" },
  { 0x3D, "Temperatura del Catalizador Banco 2 Sensor 1", 0, "°C" },
  { 0x3E, "Temperatura del Catalizador Banco 1 Sensor 2", 0, "°C" },
  { 0x3F, "Temperatura del Catalizador Banco 2 Sensor 2", 0, "°C" },

  { 0x41, "Estado del Monitor Este Ciclo de Conducción", 0, "BIT" },
  { 0x42, "Tensión del Módulo de Control", 0, "V" },
  { 0x43, "Valor de Carga Absoluta", 0, "%" },
  { 0x44, "Relación de Combustible/Aire Comandada", 0, "ratio" },
  { 0x45, "Posición del Acelerador Relativa", 0, "%" },
  { 0x46, "Temperatura del Aire Ambiente", 0, "°C" },
  { 0x47, "Posición Absoluta del Acelerador B", 0, "%" },
  { 0x48, "Posición absoluta del acelerador C", 0, "%" },
  { 0x49, "Posición absoluta del acelerador D", 0, "%" },
  { 0x4A, "Posición absoluta del acelerador E", 0, "%" },
  { 0x4B, "Posición absoluta del acelerador F", 0, "%" },
  { 0x4C, "Actuador del acelerador comandado", 0, "%" },
  { 0x4D, "Tiempo de funcionamiento con MIL encendido", 0, "min" },
  { 0x4E, "Tiempo desde que se borraron los códigos de error", 0, "min" },
  { 0x4F, "Valores máximos: relación equivalente, voltaje, corriente, presión", 0, "ratio V mA kPa" },
  { 0x50, "Tasa máxima de flujo MAF", 0, "g/s" },
  { 0x51, "Tipo de combustible", 0, "ref table" },
  { 0x52, "Porcentaje de combustible etílico", 0, "%" },
  { 0x53, "Presión absoluta del sistema de evaporación", 0, "kPa" },
  { 0x54, "Presión del sistema de evaporación", 0, "Pa" },
  { 0x55, "Ajuste a corto plazo del sensor secundario O2 Banco 1–3", 0, "%" },
  { 0x56, "Ajuste a largo plazo del sensor secundario O2 Banco 1–3", 0, "%" },
  { 0x57, "Ajuste a corto plazo del sensor secundario O2 Banco 2–4", 0, "%" },
  { 0x58, "Ajuste a largo plazo del sensor secundario O2 Banco 2–4", 0, "%" },
  { 0x59, "Presión absoluta de la línea de combustible", 0, "kPa" },
  { 0x5A, "Posición relativa del pedal del acelerador", 0, "%" },
  { 0x5B, "Vida restante de la batería híbrida", 0, "%" },
  { 0x5C, "Temperatura del Aceite del Motor", 0, "°C" },
  { 0x5D, "Ajuste de Inyección de Combustible", 0, "°" },
  { 0x5E, "Consumo de Combustible del Motor", 0, "L/h" },
  { 0x5F, "Requisitos de Emisiones", 0, "BIT" }
};

void updatePIDMapping(uint8_t mode, uint8_t pid, float value) {
  PidMapping* mappings = (mode == read_LiveData) ? liveDataMappings : freezeFrameMappings;

  for (int i = 0; i < 64; i++) {
    if (mappings[i].pid == pid) {
      mappings[i].value = value;
      return;
    }
  }
}