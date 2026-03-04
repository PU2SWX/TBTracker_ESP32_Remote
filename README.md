🇧🇷 Português: Estação de Controle Remoto LoRa via Web (ESP32)
Este projeto transforma um ESP32 com módulo LoRa (SX1278) em uma estação de controle baseada em interface web. Ele foi otimizado para se comunicar com trackers de balão (como o TBTracker) utilizando o Modo 2 de operação.
🚀 Funcionalidades
Servidor Web Integrado: Cria um Access Point WiFi com uma interface HTML/CSS moderna e responsiva.
Transmissão LoRa em Tempo Real: Envia mensagens de texto e comandos (ex: acionamento de GPIO) diretamente pelo navegador.
Ajuste Dinâmico de Frequência: Permite sintonizar a frequência de operação (ex: 430.430 MHz) sem precisar reiniciar o hardware.
Console de Log: Exibe o status das transmissões e erros diretamente na tela do celular ou PC.
Modo Simulação (MOCK): Permite testar a interface web mesmo sem um módulo LoRa conectado.
🛠️ Hardware e Pinagem
O código utiliza a pinagem padrão para placas ESP32 comuns conectadas a módulos LoRa:
SCK: 18 | MISO: 19 | MOSI: 23
NSS/CS: 5 | DIO0: 34 | RESET: 33 | DIO1: 32

🇺🇸 English: Web-Based LoRa Remote Control Station (ESP32)
This project turns an ESP32 equipped with a LoRa (SX1278) module into a web-interfaced ground station. It is optimized for communicating with high-altitude balloon trackers (such as TBTracker) using Mode 2 parameters.
🚀 Features
Integrated Web Server: Creates a WiFi Access Point with a modern, responsive HTML/CSS dashboard.
Real-Time LoRa Transmission: Send text messages and remote commands (e.g., triggering GPIOs) directly from your browser.
Dynamic Frequency Tuning: Adjust the operating frequency (e.g., 430.430 MHz) on-the-fly via the web interface.
Log Console: Real-time feedback on transmission status and errors displayed on the dashboard.
Mock Mode: Allows testing the web interface logic without physical LoRa hardware connected.
🛠️ Hardware & Pinout
The code is configured for standard ESP32 boards wired to LoRa modules:
SCK: 18 | MISO: 19 | MOSI: 23
NSS/CS: 5 | DIO0: 34 | RESET: 33 | DIO1: 32

<p align="center">
  <a href="https://youtube.com/shorts/JWPuELfmqUA?si=PwcyePewAzN6RedN">
    <img src="https://img.youtube.com/vi/JWPuELfmqUA/0.jpg" alt="Assista ao vídeo: Horus V3 no TBTRACKER-ESP32">
  </a>
</p>

https://youtube.com/shorts/JWPuELfmqUA?si=PwcyePewAzN6RedN
