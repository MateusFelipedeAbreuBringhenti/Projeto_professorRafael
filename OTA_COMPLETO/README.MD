## Quarta aula / Projeto OTA completo

-------------------------------------------------------

### Bibliotecas e configuras iniciais
#### ESP8266WiFi: Permite conectar o ESP8266 a uma rede Wi-Fi.
#### ArduinoOTA: Habilita atualizações OTA, para modificar o código sem usar cabos.
#### ESP8266WebServer: Implementa um servidor HTTP para controle remoto através de um navegador.


```cpp
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
```
---------------------------------------------

### Configuracoes de Rede
#### ssid e password: Nome e senha da rede Wi-Fi que o ESP8266 irá usar.
#### local_IP, gateway, subnet: Configurações de IP estático para o ESP8266.


```cpp
const char* ssid = "Funcionapfvr";
const char* password = "62838700";

IPAddress local_IP(192,168,99,63);
IPAddress gateway(192,168,99,167);
IPAddress subnet(255,255,255,0);
```

--------------------------------------------- 

### Funcao Setup
#### Serial.begin: Inicia a comunicação serial para debugging.
#### LED_BUILTIN: Configura o LED interno como saída.

```cpp
Serial.begin(9600);
pinMode(LED_BUILTIN, OUTPUT);
digitalWrite(LED_BUILTIN, LOW);
```

#### Conexao wi-fi

```cpp
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
```

#### Configuração do Servidor Web:

```cpp
server.on("/", HTTP_GET, handleRoot);
server.on("/5/on", HTTP_GET, handleOn);
server.on("/5/off", HTTP_GET, handleOff);
server.begin();
Serial.println("Servidor web iniciado...");
```

--------------------------------------------------------

### Funcao loop
#### Aceita conexões Telnet e executa comandos

```cpp
if (telnetServer.hasClient()) {
  if (!telnetClient || !telnetClient.connected()) {
    if (telnetClient) telnetClient.stop();
    telnetClient = telnetServer.available();
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
  } else if (command == '0') {
    digitalWrite(LED_BUILTIN, HIGH);
    telnetClient.println("LED desligado.");
  } else {
    telnetClient.println("Comando inválido.");
}
```

-----------------------------------------------------

### Handlers do Servidor HTTP
#### Gera uma interface HTML para configurar o servidor

```cpp
void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head>...</head>";
  html += "<body><h1>ESP8266 Web Server</h1>";
  html += "<button onclick=\"toggleLED('on')\" class=\"button\">ON</button>";
  html += "<button onclick=\"toggleLED('off')\" class=\"button button2\">OFF</button>";
  html += "<script>function toggleLED(state) { ... }</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}
```
------------------------------------------------------------