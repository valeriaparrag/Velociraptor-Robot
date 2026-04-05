/*******************************************************
 *   CONTROL DE VELOCIRAPTOR CON ESP32 POR WIFI
 *   - Recibe comandos tipo: LETRA:NUM (A,R,I,D : 0–255)
 *   - Controla motores mediante puente H
 *   - Incluye freno automático con sensor ultrasónico
 *   - Servidor TCP en el ESP32 (puerto 8080)
 *   - Autores: Valeria Andrea Parra García - María Alejandra Díaz Ortiz
 *******************************************************/

//Inclusión de librerias
#include <Arduino.h> //permite usar las funciones básicas de Arduino.
#include <WiFi.h> //proporciona las funciones necesarias para que el ESP32 se conecte a redes WiFi y actúe como servidor.


/*------------------------------------------------------
  CONFIGURACIÓN DE RED WIFI
  El ESP32 se conectará a esta red como cliente.
-------------------------------------------------------*/

const char* ssid = "<ID_wifi>";
const char* password = "<wifi_password>";

const int serverPort = 8080;
WiFiServer server(serverPort);
WiFiClient client;

/*------------------------------------------------------
  PINES PARA CONTROL DE MOTORES (puente H L298N)
  Cada motor tiene 2 pines de dirección y 1 PWM.
-------------------------------------------------------*/

// Motor izquierdo
int IN1 = 25;
int IN2 = 33;
int ENA = 32;  // PWM

// Motor derecho
int IN3 = 26;
int IN4 = 27;
int ENB = 14;  // PWM

/*------------------------------------------------------
  PINES DEL SENSOR ULTRASÓNICO HC-SR04
-------------------------------------------------------*/
int trigPin = 12;
int echoPin = 13;

long duration;
float distance;

/*------------------------------------------------------
  VARIABLES DE CONTROL DEL MOVIMIENTO
-------------------------------------------------------*/
char comando = 'S';
int velocidad = 0;

/*------------------------------------------------------
 Configuración inicial del sistema
-------------------------------------------------------*/
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Iniciando...");

  // Pines motores
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Pines Ultrasonido
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Conexión WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

 // Mostrar conexión establecida
  Serial.println("\nWiFi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Iniciar el servidor TCP
  server.begin();
  Serial.println("Servidor iniciado");
}


/********************************************************
 *  MEDIR DISTANCIA CON EL HC-SR04
 *  Retorna distancia en centímetros
 ********************************************************/

float medirDistancia() {
  // Enviar pulso de 10 µs por TRIG
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Medir duración del pulso ECHO
  duration = pulseIn(echoPin, HIGH);
  // Convertir tiempo a distancia
  // distancia = (duración/2) * velocidad del sonido (0.034 cm/µs)
  distance = duration * 0.017;
  return distance;
}

/********************************************************
 *  FUNCIONES DE CONTROL DE LOS MOTORES
 ********************************************************/
void detener() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void izquierda(int vel) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, vel);
  analogWrite(ENB, vel);
}

void derecha(int vel) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, vel);
  analogWrite(ENB, vel);
}

void adelante(int vel) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, vel);
  analogWrite(ENB, vel);
}

void reversa(int vel) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, vel);
  analogWrite(ENB, vel);
}

/********************************************************
 *  PROCESAR COMANDO TIPO:  LETRA:NUMERO
 *  Ejemplo: "A:200"
 ********************************************************/

bool procesarComando(String data) {
  int sep = data.indexOf(':'); // buscar ":"
  if (sep < 0) return false;   // formato incorrecto

  comando = data.charAt(0); // extraer letra
  velocidad = data.substring(sep + 1).toInt(); // extraer número

  Serial.print("Comando = ");
  Serial.println(comando);
  Serial.print("Velocidad = ");
  Serial.println(velocidad);

  return true;
}

/********************************************************
 *  EJECUTAR MOVIMIENTO SEGÚN EL COMANDO
 ********************************************************/

void ejecutarMovimiento() {

   // Limitar valores de PWM
  if (velocidad < 50) velocidad = 50;
  if (velocidad > 255) velocidad = 255;

  switch (comando) {
    case 'A': 
      Serial.println("Movimiento: Adelante");
      adelante(velocidad); 
    break;
    case 'R': 
      Serial.println("Movimiento: Reversa");
      reversa(velocidad); 
    break;
    case 'I': 
      Serial.println("Movimiento: Izquierda");
      izquierda(velocidad); 
    break;
    case 'D': 
      Serial.println("Movimiento: Derecha");
      derecha(velocidad); 
    break;
    default: 
      detener(); 
    break;
  }
}

void loop() {

  // Esperar un cliente si no está conectado
  if (!client.connected()) {
    client = server.available();
    if (client) Serial.println("Cliente conectado");
  }

  // Leer datos del cliente
  if (client && client.available()) {
    String data = client.readStringUntil('\n');
    data.trim(); // eliminar espacios y saltos extra

    if (procesarComando(data)) {
      Serial.println("Comando recibido OK");
    }
  }

  // Seguridad: detectar obstáculos
  float d = medirDistancia();

  if (d <= 10 && d > 0) {
    Serial.println("OBSTÁCULO detectado, deteniendo!");
    detener(); // evitar cualquier movimiento
    return;
  }

  // Ejecutar el movimiento solicitado
  ejecutarMovimiento();
  // Retardo para seguridad
  delay(20);
}
