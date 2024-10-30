## Segunda aula / Projeto Telnet / Lampada
#### Esse código é para conectar a uma rede Wi-Fi e preparar o dispositivo para receber atualizações OTA
```cpp
void setup() {

  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Aguarda conexão Wi-Fi
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Conexão falhou, tentando novamente...");
    WiFi.begin(ssid, password);
    delay(5000);
  }
```
--------------------------------------

### Configuração para ascender a lampada
#### O dispositivo usa um codigo para definir as rotas de saida, controlando o estado do LED

```cpp
void loop() {
  ArduinoOTA.handle();
  if (server.hasClient()) {
    if (!client || !client.connected()) {
      if (client) client.stop();
      client = server.available();
      Serial.println("Cliente conectado via Telnet.");
      client.println("Digite '1' para ligar o LED e '0' para desligar o LED.");
    } else {
      server.available().stop();
    }
  }
```


--------------------------------------

### Telnet
#### Usa o servidor telnet para receber comandos remotos, sendo os comandos para ligar/desligar o LED (Lampada)

```cpp
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
    else if (error == OTA_END_ERROR) Serial.println("Falha no finalização");
  });
  ArduinoOTA.begin();
  Serial.println("Pronto para atualizações OTA.");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  server.begin();
  server.setNoDelay(true);
  Serial.println("Servidor Telnet iniciado...");
}
```

---------------------------------------

### Loop principal
#### Se o cliente estiver conectado, envia os comandos para 1 ligar o LED e 0 desligar o LED

```cpp
  if (client && client.connected() && client.available()) {
    char command = client.read();
    if (command == '1') {
      digitalWrite(LED_PIN, LOW); 
      client.println("LED ligado.");
      Serial.println("LED ligado.");
    } else if (command == '0') {
      digitalWrite(LED_PIN, HIGH); 
      client.println("LED desligado.");
      Serial.println("LED desligado.");
    } else {
      client.println("Comando inválido. Digite '1' para ligar o LED e '0' para desligar o LED.");
    }
  }
}
```
