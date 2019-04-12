int nivelHumedad = 0;           // Nivel de humedad a mostrar
int valorHumedad = 0;           // Valor sensado
int umbralHumedad = 300;        // Si la medicion baja de este valor, activa el riego
int bombaAguaPin = 7;           // Pin de la bomba de agua

unsigned long uiTiempoInicial;          // Tiempo de inicio del contador UI
unsigned long sensorTiempoInicial;      // Tiempo de inicio del contador Sensor
unsigned long riegoTiempoInicial;       // Tiempo de inicio del contador de Riego

unsigned long uiTiempoActual;         // Tiempo actual del contador UI
unsigned long sensorTiempoActual;     // Tiempo actual del contador Sensor
unsigned long riegoTiempoActual;       // Tiempo de actual del contador de Riego

const unsigned long uiTiempoRetardo = 1000;       // Duracion de intervalo UI
const unsigned long sensorTiempoRetardo = 1000;   // Duracion de intervalo Sensor
const unsigned long riegoTiempoRetardo = 500;       // Tiempo de actual del contador de Riego

void setup() {
  Serial.begin(9600);             // Para debuggear
  pinMode(0, INPUT);              // Sensor Humedad (nuestros clavos)
  pinMode(2, OUTPUT);  
  pinMode(3, OUTPUT);  
  pinMode(4, OUTPUT);  
  pinMode(bombaAguaPin, OUTPUT);  // Para activar la bomba de agua.
  #define VALOR_HUMEDAD_MIN 300   // Valor minimo para la lectura de la humedad.
  #define VALOR_HUMEDAD_MAX 800   // Valor maximo para la lectura de la humedad.
}

// dibujarHumedad:
// Recibe el nivel de humedad
// Hace un for para apagar los leds en pins 0 1 y 2.
// Luego prende el led correspondiente para representar el nivel de humedad.
void dibujarHumedad(int nivelHumedad) {
  for (int i = 2; i <= 4; i++) {
    digitalWrite(i, LOW);
  }
  
  if (nivelHumedad == 0) {
    digitalWrite(4, HIGH);
  }
  if (nivelHumedad == 1) {
    digitalWrite(3, HIGH);
  }
  if (nivelHumedad == 2) {
    digitalWrite(2, HIGH);
  }
}


void activarRiego() {
  riegoTiempoActual = millis();
  if (riegoTiempoActual - riegoTiempoInicial >= riegoTiempoRetardo) {
    digitalWrite(bombaAguaPin, HIGH);
    Serial.println("< ACTIVAR RIEGO >");     // Logeamos un mensaje
    riegoTiempoInicial = riegoTiempoActual; // Actualizamos el contador de tiempo
  }
}

void controlarUmbralHumedad(int valorHumedad) {
  Serial.println("[controlarUmbralHumedad]");     // Logeamos un mensaje
  Serial.println(valorHumedad);     // Logeamos un mensaje
  Serial.println("[/controlarUmbralHumedad]");     // Logeamos un mensaje
  if (valorHumedad <= umbralHumedad) {
    activarRiego();
  } else {
    Serial.println("< PARAR RIEGO >");     // Logeamos un mensaje
    digitalWrite(bombaAguaPin, LOW);
  }
}

void loop() {
  // Guardamos el tiempo actual en milisegundos en cada contador
  uiTiempoActual = millis();
  sensorTiempoActual = millis();

  // Si el timpo de retardo UI paso desde la ultima lectura
  if (uiTiempoActual - uiTiempoInicial >= uiTiempoRetardo) {
    Serial.println("[UI REFRESH]");     // Logeamos un mensaje
    dibujarHumedad(nivelHumedad);     // Dibujamos el nivel de humedad
    uiTiempoInicial = uiTiempoActual; // actualizamos el contador de tiempo.
  }

  // Si el timpo de retardo Sensor paso desde la ultima lectura
  if (sensorTiempoActual - sensorTiempoInicial >= sensorTiempoRetardo) {
    // A continuacion vamos a hacer varias cosas
    // Vamos a hacer a leer el sensor analogico seleccionado.
    // Hacemos un constrain para restringir la lectura del sensor.
    // Le decimos cual es el valor minimo y maximo que queremos obtener
    // Luego mapeamos el valor medido a un rango del 0 al 2
    // 0, 1 y 2 son los leds que indican la medicion del sensor (x/3)
    valorHumedad = analogRead(A0);
    nivelHumedad = map( constrain(valorHumedad, VALOR_HUMEDAD_MIN, VALOR_HUMEDAD_MAX) , VALOR_HUMEDAD_MIN, VALOR_HUMEDAD_MAX, 0, 2);
    Serial.print("[SENSOR REFRESH] Sensor value: "); // Logeamos un mensaje
    Serial.println(nivelHumedad);
    controlarUmbralHumedad(valorHumedad); // Chequeamos si hay que activar la bomba de agua.
    sensorTiempoInicial = sensorTiempoActual; // actualizamos el contador de tiempo.
  }
}
