#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>

const char* ssid = "Funcionapfvr";
const char* password = "62838700";

ESP8266WebServer server(80);
WiFiServer telnetServer(23);
WiFiClient telnetClient;

IPAddress local_IP(192,168,99,63);
IPAddress gateway(192,168,99,167);
IPAddress subnet(255,255,255,0);

void handleRoot();
void handleOn();
void handleOff();

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Falha na configuração do IP estático");
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n WiFi conectado.");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  telnetServer.begin();
  telnetServer.setNoDelay(true);
  Serial.println("Servidor Telnet iniciado...");

  ArduinoOTA.onStart([]() {
    Serial.println("Iniciando atualização OTA...");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nAtualização concluída.");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progresso: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Erro[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Falha na autenticação");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Falha no início");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Falha na conexão");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Falha no recebimento");
    else if (error == OTA_END_ERROR) Serial.println("Falha na finalização");
  });
  ArduinoOTA.begin();
  Serial.println("Pronto para atualizações OTA.");

  server.on("/", HTTP_GET, handleRoot);
  server.on("/5/on", HTTP_GET, handleOn);
  server.on("/5/off", HTTP_GET, handleOff);
  server.begin();
  Serial.println("Servidor web iniciado...");
}

void loop() {
  ArduinoOTA.handle();
  server.handleClient();

  if (telnetServer.hasClient()) {
    if (!telnetClient || !telnetClient.connected()) {
      if (telnetClient) telnetClient.stop();
      telnetClient = telnetServer.available();
      Serial.println("Cliente conectado via Telnet.");
      telnetClient.println("Digite '1' para ligar o LED e '0' para desligar o LED.");
    } else {
      telnetServer.available().stop();
    }
  }

  if (telnetClient && telnetClient.connected() && telnetClient.available()) {
    char command = telnetClient.read();
    if (command == '1') {
      digitalWrite(LED_BUILTIN, LOW); 
      telnetClient.println("LED ligado.");
      Serial.println("LED ligado.");
    } else if (command == '0') {
      digitalWrite(LED_BUILTIN, HIGH); 
      telnetClient.println("LED desligado.");
      Serial.println("LED desligado.");
    } else {
      telnetClient.println("Comando inválido. Digite '1' para ligar o LED e '0' para desligar o LED.");
    }
  }
}

void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<link rel=\"icon\" href=\"data:,\">";
  html += "<style>html { font-family: Helvetica; background-color: #808080; display: inline-block; margin: 0px auto; text-align: center;}";
  html += ".button { background-color: #ff0000; border: none; color: white; padding: 16px 40px;";
  html += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  html += ".button2 {background-color: #000;}</style></head>";
  html += "<body><h1>ESP8266 Web Server</h1>";
  html += "<h2>Alunos Mateus Bringhenti e Vitor Bortot</h2>";
  html += "<h3>Aula Projeto Integrador Extensionista</h3>";
  html += "<button onclick=\"toggleLED('on')\" class=\"button\">ON</button>";
  html += "<button onclick=\"toggleLED('off')\" class=\"button button2\">OFF</button>";
  html += "<script>";
  html += "function toggleLED(state) {";
  html += "  var xhr = new XMLHttpRequest();";
  html += "  xhr.open('GET', '/5/' + state, true);";
  html += "  xhr.send();";
  html += "}";
  html += "</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleOn() {
  digitalWrite(LED_BUILTIN, LOW);
  server.send(200, "text/plain", "LED Ligado!");
}

void handleOff() {
  digitalWrite(LED_BUILTIN, HIGH);
  server.send(200, "text/plain", "LED Desligado!");
}