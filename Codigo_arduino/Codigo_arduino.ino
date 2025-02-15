#include <DHT.h>
#include <DHT_U.h>
#include <WiFiClientSecure.h>
#include <WiFi.h>
//#include "password.h"
 
// Dirección del servidor sin el esquema (https://)
const char* server = "5000-camoreno368-taller1gitp-u3rpuhdisw7.ws-us117.gitpod.io";
const int httpsPort = 443; // Puerto estándar para HTTPS
const char* Endpoint = "/data";
const char* ssid = "CAMP";
const char* password = "Moreno_368";
 
#define dhtpin 15
#define DHTTYPE DHT22
#define led 2
 
 
float h;
float t;
 
WiFiClientSecure client;
DHT dht(dhtpin, DHTTYPE);
 
void setup()
{
  Serial.begin(115200);
  dht.begin();
  pinMode(led, OUTPUT);
 
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
 
  // Intentar conectar a la red WiFi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
 
  Serial.println("\nWiFi connected");
  Serial.print("Connected to IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Connected to ");
  Serial.println(ssid);
 
  // Ignorar errores de certificado
  client.setInsecure();
}
 
void loop()
{
  // Reconectar si la conexión WiFi se pierde
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost. Reconnecting...");
    WiFi.begin(ssid, password);
  }
 
  delay(20000);
 
  // Leer temperatura y humedad
  h = dht.readHumidity();
  t = dht.readTemperature();
 
  // Mostrar datos en el monitor serial
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.println("%");
  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));
 
  // Enviar datos al servidor
  if (client.connect(server, httpsPort)) {
    Serial.println("Connected to server");
   
    // Preparar datos POST
    String postData = "humidity=" + String(h) + "&temperature=" + String(t);  
    client.println("POST " + String(Endpoint) + " HTTP/1.1");  
    client.println("Host: " + String(server)); // Solo dominio sin esquema
    client.println("Content-Type: application/x-www-form-urlencoded");  
    client.println("Connection: close");  
    client.println("Content-Length: " + String(postData.length()));  
    client.println();  
    client.println(postData);  
 
    // Leer respuesta del servidor
    while (client.connected() || client.available()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("Headers received, response:");
      } else {
        Serial.println(line);
      }
    }
    client.stop();
    Serial.println("Data sent and connection closed");  
  } else {
    Serial.println("Connection failed to server");
  }
 
  // Controlar el LED según la temperatura
  if (t >= 33) {
    digitalWrite(led, HIGH);    
  } else {
    digitalWrite(led, LOW);
  }
}
