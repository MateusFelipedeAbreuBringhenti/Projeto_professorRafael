## Terceira aula / Projeto Servidor / Lampada
#### Esse código é para conectar a uma rede Wi-Fi e preparar o dispositivo para receber atualizações OTA
### Configuração do Servidor
#### Usa uma biblioteca para configurar um servidor web na porta ESP8266WebServer server(80);
### HTML
#### Cria uma página HTML com dois botões, que quando clicados, eles acionam a função para enviar uma requisição ao servidor e mudar o estado do LED
### Loop Principal
#### O servidor web permite responder a requisições HTTP, mantendo o placa pronta para interações