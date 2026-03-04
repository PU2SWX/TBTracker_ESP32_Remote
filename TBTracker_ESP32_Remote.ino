#include <WiFi.h>
#include <WebServer.h>
#include <RadioLib.h>

// ===================== CONFIGURAÇÃO HARDWARE =====================
// Pinos para o ESP32 DEV KIT
#define SCK 18
#define MISO 19
#define MOSI 23
#define CS 5
#define LORA_NSS    5 
#define LORA_DIO0   34  
#define LORA_RESET  33
#define LORA_DIO1   32 
// pinos para o ESP32C3 super mini
//pinos para o esp32c3
/*
#define PIN_NSS 7
#define PIN_DIO0 3
#define PIN_BUSY -1     
#define PIN_RESET 10
#define PIN_DIO1 2
#define PIN_DIO2 -1  // Only used for AFSK / APRS
*/
#define MOCK_LORA 0 // 0 = Transmissão Real, 1 = Apenas simulação

// ===================== PARÂMETROS MODO 2 (TBTracker) =============
float currentFreq = 430.430;
float loraBW = 62.5;      
uint8_t loraSF = 8;       
uint8_t loraCR = 8;       
uint8_t loraSync = 0x12;

#if MOCK_LORA == 0
SX1278 radio = new Module(LORA_NSS, LORA_DIO0, LORA_RESET, LORA_DIO1);
#endif

// ===================== WIFI CONFIG ===============================
const char* ssid = "TBTracker_ESP32_Remote";
const char* password = "12345678";
WebServer server(80);

// ===================== HTML INTERFACE ============================
const char PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Base de Controle HAB</title>
  <style>
    body { font-family: 'Segoe UI', sans-serif; background: #0f0f0f; color: #e0e0e0; text-align: center; padding: 10px; }
    .container { max-width: 500px; margin: auto; }
    .card { background: #1a1a1a; padding: 20px; border-radius: 15px; margin-bottom: 15px; border: 1px solid #333; box-shadow: 0 4px 10px rgba(0,0,0,0.5); }
    h2, h3 { color: #3498db; margin-top: 0; }
    input { width: 80%; padding: 12px; margin: 10px 0; border-radius: 8px; border: 1px solid #444; background: #252525; color: white; font-size: 16px; }
    button { width: 90%; padding: 12px; margin: 5px 0; border: none; border-radius: 8px; font-weight: bold; cursor: pointer; transition: 0.3s; }
    .btn-blue { background: #2980b9; color: white; }
    .btn-green { background: #27ae60; color: white; }
    .btn-red { background: #c0392b; color: white; }
    .btn-msg { background: #8e44ad; color: white; }
    #log { background: #000; color: #00ff00; padding: 10px; height: 150px; overflow-y: auto; text-align: left; font-family: monospace; font-size: 12px; border-radius: 8px; border: 1px solid #333; }
    .status { font-size: 14px; color: #f1c40f; margin-bottom: 10px; }
  </style>
</head>
<body>
  <div class="container">
    <h2>📡 Base Multi-Sonda</h2>
    <div class="status" id="st-freq">Frequência: 432.662 MHz (Modo 2)</div>

    <div class="card">
      <h3>💬 Enviar Mensagem/Comando</h3>
      <input type="text" id="msgInput" placeholder="Digite aqui...">
      <button class="btn-msg" onclick="sendMsg()">ENVIAR TEXTO</button>
    </div>

    <div class="card">
      <h3>💡 Comandos Rápidos (GPIO)</h3>
      <button class="btn-green" onclick="execCmd(1,1)">LIGAR LED (1)</button>
      <button class="btn-red" onclick="execCmd(1,0)">DESLIGAR LED (1)</button>
    </div>

    <div class="card">
      <h3>⚙️ Ajuste de Frequência</h3>
      <input type="number" id="freqInput" step="0.001" value="430.430">
      <button class="btn-blue" onclick="updateFreq()">SINTONIZAR</button>
    </div>

    <div id="log">Console iniciado...<br></div>
  </div>

  <script>
    function log(t) {
      const d = document.getElementById("log");
      d.innerHTML += ">> " + t + "<br>";
      d.scrollTop = d.scrollHeight;
    }
   
    function sendMsg() {
      let m = document.getElementById("msgInput").value;
      if(!m) return;
      fetch("/send?txt=" + encodeURIComponent(m)).then(r => r.text()).then(t => {
        log(t);
        document.getElementById("msgInput").value = "";
      });
    }

    function execCmd(p, s) {
      let cmd = "CMD:" + p + ":" + s;
      fetch("/send?txt=" + encodeURIComponent(cmd)).then(r => r.text()).then(t => log(t));
    }

    function updateFreq() {
      let f = document.getElementById("freqInput").value;
      fetch("/setFreq?f=" + f).then(r => r.text()).then(t => {
        log(t);
        document.getElementById("st-freq").innerHTML = "Frequência: " + f + " MHz";
      });
    }
  </script>
</body>
</html>
)rawliteral";

// ===================== HANDLERS =====================
void handleRoot() { server.send(200, "text/html", PAGE); }

void handleSend() {
  String txt = server.arg("txt");
#if MOCK_LORA == 0
  int st = radio.transmit(txt.c_str());
  if (st == RADIOLIB_ERR_NONE) {
    server.send(200, "text/plain", "OK: " + txt);
  } else {
    server.send(500, "text/plain", "Erro TX: " + String(st));
  }
#else
  server.send(200, "text/plain", "[MOCK] Enviado: " + txt);
#endif
}

void handleSetFreq() {
  float f = server.arg("f").toFloat();
#if MOCK_LORA == 0
  int st = radio.setFrequency(f);
  if (st == RADIOLIB_ERR_NONE) {
    currentFreq = f;
    server.send(200, "text/plain", "Freq alterada para " + String(f, 3));
  } else {
    server.send(500, "text/plain", "Erro ao mudar freq");
  }
#else
  currentFreq = f;
  server.send(200, "text/plain", "[MOCK] Freq: " + String(f, 3));
#endif
}

void setup() {
  Serial.begin(115200);
 
  // Inicia WiFi
  WiFi.softAP(ssid, password);
  Serial.println("IP da Base: " + WiFi.softAPIP().toString());

  // Rotas
  server.on("/", handleRoot);
  server.on("/send", handleSend);
  server.on("/setFreq", handleSetFreq);
  server.begin();

#if MOCK_LORA == 0
  // Inicia Rádio com parâmetros do Modo 2 do TBTracker
  int st = radio.begin(currentFreq, loraBW, loraSF, loraCR, loraSync);
  if (st == RADIOLIB_ERR_NONE) {
    radio.setCRC(true);
    Serial.println("LoRa pronto para Modo 2!");
  } else {
    Serial.printf("Falha no rádio (Erro: %d). Verifique a fiação!\n", st);
  }
#endif
}

void loop() {
  server.handleClient();
}
