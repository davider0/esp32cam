#include <WiFi.h>
#include <WebServer.h>

// Configuración de la red WiFi
const char* ssid = "Gracias por el wifi gratis, vecino";  // Nombre de la red WiFi
const char* password = "12345678";  // Contraseña de la red

// Crear un servidor web en el puerto 80
WebServer server(80);

// Página principal HTML
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32-CAM Access Point</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin: 20px; }
    h1 { color: #333; }
    button { padding: 10px 20px; font-size: 16px; margin: 10px; }
  </style>
</head>
<body>
  <h1>Bienvenido a mi web</h1>
  <p>Este ESP32 está funcionando como un punto de acceso.</p>
  <button onclick="alert('¡Botón 1 pulsado!')">Botón 1</button>
  <button onclick="alert('¡Botón 2 pulsado!')">Botón 2</button>
  <p><b>Dirección IP:</b> %IP%</p>
</body>
</html>
)rawliteral";


void handleRoot() {
  String page = htmlPage;
  page.replace("%IP%", WiFi.softAPIP().toString()); 
  server.send(200, "text/html", page);
}

void setup() {

  Serial.begin(115200);
  Serial.println("Inicializando...");

  WiFi.softAP(ssid, password);
  Serial.println("Punto de acceso creado:");
  Serial.print("  SSID: "); Serial.println(ssid);
  Serial.print("  Contraseña: "); Serial.println(password);

  Serial.print("  Dirección IP del AP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);

  server.begin();
  Serial.println("Servidor web iniciado");
}

void loop() {

  server.handleClient();
}