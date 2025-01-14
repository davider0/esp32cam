#include <WiFi.h>
#include <esp_camera.h>

// ===================
// Select camera model
// ===================
//#define CAMERA_MODEL_WROVER_KIT // Has PSRAM
//#define CAMERA_MODEL_ESP_EYE  // Has PSRAM
//#define CAMERA_MODEL_ESP32S3_EYE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM // Has PSRAM
//#define CAMERA_MODEL_M5STACK_V2_PSRAM // M5Camera version B Has PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_ESP32CAM // No PSRAM
//#define CAMERA_MODEL_M5STACK_UNITCAM // No PSRAM
//#define CAMERA_MODEL_M5STACK_CAMS3_UNIT  // Has PSRAM
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
//#define CAMERA_MODEL_TTGO_T_JOURNAL // No PSRAM
//#define CAMERA_MODEL_XIAO_ESP32S3 // Has PSRAM
// ** Espressif Internal Boards **
//#define CAMERA_MODEL_ESP32_CAM_BOARD
//#define CAMERA_MODEL_ESP32S2_CAM_BOARD
//#define CAMERA_MODEL_ESP32S3_CAM_LCD
//#define CAMERA_MODEL_DFRobot_FireBeetle2_ESP32S3 // Has PSRAM
//#define CAMERA_MODEL_DFRobot_Romeo_ESP32S3 // Has PSRAM
#include "camera_pins.h"

// Configuración de red Wi-Fi
const char* ssid = "CAM-Network";
const char* password = "12345678";

// Función para inicializar la cámara
bool initCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // Inicializar cámara
  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Error al inicializar la cámara");
    return false;
  }
  return true;
}

// Configuración del servidor Wi-Fi
void setupWiFi() {
  WiFi.softAP(ssid, password);
  Serial.println("Red Wi-Fi creada");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
}

// Servidor básico para capturar imágenes
void startServer() {
  WiFiServer server(80);
  server.begin();
  Serial.println("Servidor iniciado");

  while (true) {
    WiFiClient client = server.available();
    if (client) {
      Serial.println("Cliente conectado");
      String request = client.readStringUntil('\r');
      client.flush();

      if (request.indexOf("/capture") != -1) {
        camera_fb_t *fb = esp_camera_fb_get();
        if (!fb) {
          Serial.println("Error al capturar la imagen");
          client.println("HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/plain\r\n\r\nError al capturar la imagen");
        } else {
          client.println("HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\n\r\n");
          client.write(fb->buf, fb->len);
          esp_camera_fb_return(fb);
        }
      } else {
        client.println("HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nComando no encontrado");
      }
      client.stop();
      Serial.println("Cliente desconectado");
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Inicializar cámara
  if (!initCamera()) {
    Serial.println("Fallo en la configuración de la cámara");
    while (true);
  }

  // Configurar Wi-Fi
  setupWiFi();

  // Iniciar servidor
  startServer();
}

void loop() {
  // El servidor maneja las peticiones en el bucle
}

