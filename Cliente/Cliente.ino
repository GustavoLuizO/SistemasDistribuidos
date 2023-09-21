2#include "DHT.h"
#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht (DHTPIN, DHTTYPE);
#include <SPI.h>
#include <ESP8266WiFi.h>                             
#define       LedBoard   2
#define       BUTTON     0                     

char ssid[] = "Prof Celso";                                 
char pass[] = "aulasdist";                             
IPAddress server(192,168,10,40);                    
WiFiClient client;                                    

void setup() {
  pinMode(LedBoard, OUTPUT);                           // Inicializa o Led da placa como saída
  pinMode(BUTTON, INPUT_PULLUP);                       // Incializa o botão da placa como entrada
  digitalWrite(LedBoard, HIGH);                        // Inicializa o led da placa em off
 
  Serial.begin(9600);                                // define a taxa de transferência
  Serial.println("");
  Serial.print("Aguardando conexão: ");
  WiFi.begin(ssid, pass);                              // conecta na rede
  while (WiFi.status() != WL_CONNECTED) { //enquanto o status da conexão com a rede for não conectado, o led da placa piscará
  Serial.print(".");
  digitalWrite(LedBoard, LOW);
  delay(250);
  digitalWrite(LedBoard, HIGH);
  delay(250);
  dht.begin();
  }
  digitalWrite(LedBoard, HIGH); //após conectar  led apaga
  
//------------------------------------------------------------------------------------
// Parâmetros da rede
//------------------------------------------------------------------------------------
  Serial.println("");
  Serial.println("Cliente conectado");
  Serial.print("IP: ");       Serial.println(WiFi.softAPIP());
  Serial.print("SSID: ");     Serial.println(WiFi.SSID());
  Serial.print("Signal: ");   Serial.println(WiFi.RSSI());
}
//====================================================================================
void loop() {
 
  ContinuousConnection();
}
//====================================================================================

 void ContinuousConnection(){
  client.connect(server, 80);                          // Conexão com o servidor
  ReadButton();                                        // lê o botão do transmissor
 }
//====================================================================================

void ReadButton() {
  int reading = digitalRead(BUTTON);                   // Lê o estado do botão
  if (reading == LOW) {                                // Se o botão for pressionado
    client.print("Eu sou o transmissor\r");                // envia a mensagem Eu sou o transmissor para o servidor
    delay(200);
   }else{
   ClientContinue(); 
  } 
}

//====================================================================================
void ClientContinue(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  client.println("G5 " + String(t) + " CelsiusTIME");               
  String answer = client.readStringUntil('\r');       
  client.flush(); 
  int valor = 0;
  valor = answer.toInt();
  if (valor>900) {                    
    digitalWrite(LedBoard, digitalRead(HIGH));   // if it changes the status of the LED
    Serial.print("Data Received: ");
    Serial.println(valor);
    //delay(200);                                       // client will trigger the communication 200 milliseconds
  }else{
    digitalWrite(LedBoard, digitalRead(LOW));   // if it changes the status of the LED
    Serial.print("Data Received: ");
    Serial.println(valor);
  }
}