// =========================
// CONFIGURARE PINI
// =========================
const int SENSOR_PIN = 34;      // D34 / GPIO34 -> senzor umiditate
const int MOSFET_PIN = 26;      // D26 / GPIO26 -> semnal catre MOSFET

// =========================
// CALIBRARE SENZOR
// valori orientative, le ajustam mai tarziu dupa masuratori reale
// =========================
const int DRY_VALUE = 3000;     // mai uscat
const int WET_VALUE = 1300;     // mai umed

// =========================
// PRAGURI DE CONTROL
// folosim 2 praguri ca sa evitam porniri/opririi rapide
// =========================
const int START_WATERING_BELOW_PERCENT = 35;   // daca scade sub 35%, porneste pompa
const int STOP_WATERING_ABOVE_PERCENT  = 45;   // daca trece peste 45%, opreste pompa

// =========================
// TIMPI
// =========================
const unsigned long SENSOR_READ_INTERVAL_MS = 200;    // citire foarte frecventa
const unsigned long DEBUG_PRINT_INTERVAL_MS = 1000;   // afisare mai aerisita in consola

// =========================
// VARIABILE GLOBALE
// =========================
bool pumpIsOn = false;

int lastRawValue = 0;
int lastHumidityPercent = 0;

unsigned long lastSensorReadTime = 0;
unsigned long lastDebugPrintTime = 0;
unsigned long readingCounter = 0;

// =========================
// FUNCTIE CONVERSIE PROCENT
// =========================
int convertToHumidityPercent(int rawValue) {
  int percent = map(rawValue, DRY_VALUE, WET_VALUE, 0, 100);

  if (percent < 0) {
    percent = 0;
  }

  if (percent > 100) {
    percent = 100;
  }

  return percent;
}

// =========================
// PORNIRE POMPA
// =========================
void turnPumpOn() {
  Serial.println("DEBUG: Functia turnPumpOn() a fost apelata");
  Serial.println("DEBUG: Setez MOSFET pe HIGH");
  digitalWrite(MOSFET_PIN, HIGH);
  pumpIsOn = true;
  Serial.println("DEBUG: Pompa este acum marcata ca PORNITA");
}

// =========================
// OPRIRE POMPA
// =========================
void turnPumpOff() {
  Serial.println("DEBUG: Functia turnPumpOff() a fost apelata");
  Serial.println("DEBUG: Setez MOSFET pe LOW");
  digitalWrite(MOSFET_PIN, LOW);
  pumpIsOn = false;
  Serial.println("DEBUG: Pompa este acum marcata ca OPRITA");
}

// =========================
// SETUP
// =========================
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("==================================================");
  Serial.println("DEBUG: Program pornit");
  Serial.println("DEBUG: Initializare ESP32");
  Serial.println("DEBUG: Intru in setup()");

  Serial.println("DEBUG: Configurez pinul senzorului...");
  pinMode(SENSOR_PIN, INPUT);
  Serial.print("DEBUG: SENSOR_PIN = ");
  Serial.println(SENSOR_PIN);
  Serial.println("DEBUG: Pinul senzorului a fost setat ca INPUT");

  Serial.println("DEBUG: Configurez pinul MOSFET...");
  pinMode(MOSFET_PIN, OUTPUT);
  Serial.print("DEBUG: MOSFET_PIN = ");
  Serial.println(MOSFET_PIN);
  Serial.println("DEBUG: Pinul MOSFET a fost setat ca OUTPUT");

  Serial.println("DEBUG: Pun pompa initial pe OPRIT");
  digitalWrite(MOSFET_PIN, LOW);
  pumpIsOn = false;

  Serial.println("DEBUG: Configurare praguri:");
  Serial.print("DEBUG: Pornire pompa sub = ");
  Serial.print(START_WATERING_BELOW_PERCENT);
  Serial.println("%");

  Serial.print("DEBUG: Oprire pompa peste = ");
  Serial.print(STOP_WATERING_ABOVE_PERCENT);
  Serial.println("%");

  Serial.println("DEBUG: Rezumat configurare:");
  Serial.println("DEBUG: - D34 citeste senzorul de umiditate");
  Serial.println("DEBUG: - D26 comanda MOSFET-ul");
  Serial.println("DEBUG: - Pompa porneste sub pragul inferior");
  Serial.println("DEBUG: - Pompa se opreste peste pragul superior");
  Serial.println("DEBUG: - Citirea se face continuu, fara delay mare de udare");
  Serial.println("DEBUG: Setup terminat");
  Serial.println("==================================================");
}

// =========================
// LOOP
// =========================
void loop() {
  unsigned long currentTime = millis();

  // =========================
  // 1. CITIRE SENZOR FOARTE DES
  // =========================
  if (currentTime - lastSensorReadTime >= SENSOR_READ_INTERVAL_MS) {
    lastSensorReadTime = currentTime;
    readingCounter++;

    lastRawValue = analogRead(SENSOR_PIN);
    lastHumidityPercent = convertToHumidityPercent(lastRawValue);

    // =========================
    // 2. LOGICA DE CONTROL CU HISTEREZIS
    // =========================
    if (!pumpIsOn) {
      // pompa este oprita, verific daca trebuie pornita
      if (lastHumidityPercent < START_WATERING_BELOW_PERCENT) {
        Serial.println("DEBUG: Umiditatea a scazut sub pragul inferior");
        Serial.println("DEBUG: Trebuie sa pornesc pompa");
        turnPumpOn();
      }
    } else {
      // pompa este pornita, verific daca trebuie oprita
      if (lastHumidityPercent > STOP_WATERING_ABOVE_PERCENT) {
        Serial.println("DEBUG: Umiditatea a depasit pragul superior");
        Serial.println("DEBUG: Trebuie sa opresc pompa");
        turnPumpOff();
      }
    }
  }

  // =========================
  // 3. AFISARE DEBUG PERIODICA
  // =========================
  if (currentTime - lastDebugPrintTime >= DEBUG_PRINT_INTERVAL_MS) {
    lastDebugPrintTime = currentTime;

    Serial.println();
    Serial.println("--------------------------------------------------");
    Serial.print("DEBUG: Numar total citiri = ");
    Serial.println(readingCounter);

    Serial.print("DEBUG: millis() = ");
    Serial.println(currentTime);

    Serial.print("DEBUG: Valoare analogica bruta = ");
    Serial.println(lastRawValue);

    Serial.print("DEBUG: Umiditate estimata = ");
    Serial.print(lastHumidityPercent);
    Serial.println("%");

    Serial.print("DEBUG: Prag pornire pompa = ");
    Serial.print(START_WATERING_BELOW_PERCENT);
    Serial.println("%");

    Serial.print("DEBUG: Prag oprire pompa = ");
    Serial.print(STOP_WATERING_ABOVE_PERCENT);
    Serial.println("%");

    Serial.print("DEBUG: Stare pompa = ");
    if (pumpIsOn) {
      Serial.println("PORNITA");
    } else {
      Serial.println("OPRITA");
    }

  }
}