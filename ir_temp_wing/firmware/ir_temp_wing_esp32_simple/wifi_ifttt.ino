#include <WiFi.h>

const char* ssid = "OVERWRITE_ME";
const char* password = "OVERWRITE_ME";
const char* resource = "IFTTT_RESOURCE";
const char* server = "maker.ifttt.com";

void initWifi()
{
  int timeout = 10 * 4; // 10 seconds

  Serial.print("Connecting to: ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED && (timeout-- > 0)) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect!");
    return;
  }

  Serial.println("WiFi Connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
}

void makeIFTTTRequest(String ambient, String object)
{
  WiFiClient client;
  int retires = 5;
  int timeout = 5 * 10; // 5 seconds
  
  Serial.print("Connecting to ");
  Serial.print(server);

  while (!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();

  if (!!!client.connected()) {
    Serial.println("Failed to connect...");
    return;
  }

  Serial.print("Request resource: ");
  Serial.println(resource);

  String jsonObject = String("{\"value1\":\"") + ambient + "\",\"value2\":\"" + object + "\"}";

  client.println(String("POST ") + resource + " HTTP/1.1");
  client.println(String("Host: ") + server);
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject.length());
  client.println();
  client.println(jsonObject);

  while (!!!client.available() && (timeout-- > 0)) {
    delay(100);
  }
  if (!!!client.available()) {
    Serial.println("No response...");
  }
  while (client.available()) {
    Serial.write(client.read());
  }

  Serial.println("\nclosing connection");
  client.stop();
  
}
