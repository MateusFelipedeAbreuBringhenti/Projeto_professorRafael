### Quinta aula / Projeto com a Alexa
#### Esse código utiliza a plataforma SinricPro para integrar o ESP8266 com dispositivos inteligentes, como a Alexa

-------------------------------------------------------------

### Inclusão de Bibliotecas e Definições de Constantes
#### Arduino.h: Biblioteca padrão para o ESP8266.
#### ESP8266WiFi: Permite conectar o ESP8266 ao Wi-Fi.
#### SinricPro e SinricProSwitch: Integra o dispositivo com a plataforma SinricPro, usada para controle remoto e integração com assistentes virtuais.

```cpp
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>
```

------------------------------------------------

### Callback para Controle Remoto (SinricPro)
#### Essa função é chamada quando o estado do dispositivo é alterado pela SinricPro.
#### State: Indica o novo estado (true para ligado, false para desligado).
#### Atualiza o relé e exibe o estado atual no monitor serial.

```cpp
bool onPowerState(const String &deviceId, bool &state) {
  Serial.printf("Device %s turned %s (via SinricPro) \r\n", deviceId.c_str(), state?"on":"off");
  myPowerState = state;
  digitalWrite(RELE_PIN, myPowerState ? LOW : HIGH);
  return true;
}
```

-----------------------------------------------

### Controle Manual (Botão Físico)
#### erifica se o botão foi pressionado.
#### Altera o estado do dispositivo (liga/desliga) e atualiza o servidor da SinricPro.
#### Implementa debounce para evitar acionamentos múltiplos acidentais.

```cpp
void handleButtonPress() {
  unsigned long actualMillis = millis();
  if (digitalRead(BUTTON_PIN) == LOW && actualMillis - lastBtnPress > 1000) { 
    myPowerState = !myPowerState; 
    digitalWrite(RELE_PIN, myPowerState ? LOW : HIGH);
    SinricProSwitch& mySwitch = SinricPro[SWITCH_ID];
    mySwitch.sendPowerStateEvent(myPowerState);
    Serial.printf("Device %s turned %s (manually via button)\r\n",
                  mySwitch.getDeviceId().c_str(), myPowerState ? "on" : "off");
    lastBtnPress = actualMillis;
  }
}
```

---------------------------------------------------------------------------------

### Conexão com o Wi-Fi
#### Conecta o ESP8266 à rede Wi-Fi usando as credenciais definidas.
#### Mostra o status de conexão no monitor serial.

```cpp
void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }
  Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
}
```

-----------------------------------------------------------

### Configuração da SinricPro
#### Configura o dispositivo na plataforma SinricPro.
#### Inicializa a comunicação com a API.

```cpp
void setupSinricPro() {
  SinricProSwitch& mySwitch = SinricPro[SWITCH_ID];
  mySwitch.onPowerState(onPowerState);

  SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); });
  SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}
```

-------------------------------------------------------------

### Loop Principal
#### HandleButtonPress: Trata acionamentos manuais.
#### SinricPro.handle: Mantém a comunicação com a plataforma SinricPro para tratar comandos remotos.

```cpp
void loop() {
  handleButtonPress(); 
  SinricPro.handle(); 
}
```