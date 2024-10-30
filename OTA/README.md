## Primeira aula / Projeto OTA
#### Esse código é para conectar a uma rede Wi-Fi e preparar o dispositivo para receber atualizações OTA

---------------------------------

### Configuração de Wi-Fi
#### O dispositivo tenta se conectar a uma rede Wi-Fi usando as credenciais fornecidas, caso a conexão falhe, o código tentará reconectar a cada 5 segundos

```cpp
void setup() {

  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

 while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Conexão falhou, tentando novamente")
    WiFi.begin(ssid, password);
    delay(5000);
 }
 ```

------------------------------------

### Configuração OTA
#### Após configurar o OTA, o código imprime o endereço IP do dispositivo

```cpp 
ArduinoOTA.begin();
Serial.println("Pronto para atualizações OTA.");
Serial.print("Endereço IP: ");
Serial.println(WiFi.localIP());
```
------------------------------------

### Inicializando o servidor Telnet

```cpp
server.begin();`
server.setNoDelay(true);`
Serial.println("Servidor Telnet iniciado...");
```

------------------------------------

### Loop principal
#### Esse código permite que o dispositivo seja atualizado remotamente enquanto está conectado à rede Wi-Fi
