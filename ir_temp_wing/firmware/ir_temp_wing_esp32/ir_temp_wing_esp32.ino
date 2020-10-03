#include <Wire.h>
//#include "BluetoothSerial.h"
#include "IR_Temp_Wing.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

//#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
//#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
//#endif

#define TEMP_STR_BUFF_SZ  20

#define TMP_DEV 8
//#define TMP_WAKE_UP_PIN 4

#define uS_TO_S_FACTOR 1000000ULL 
#define TIME_TO_SLEEP_S 2

//BluetoothSerial SerialBT;
BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
String temp_str[TEMP_STR_BUFF_SZ];
int temp_str_cntr = 0;

//int loops = 0;
RTC_DATA_ATTR int bootCount = 0;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received Value: ");
        for (int i = 0; i < rxValue.length(); i++)
          Serial.print(rxValue[i]);

        Serial.println();
        Serial.println("*********");
      }
    }
};

void setup() {
  Wire.begin();
  Serial.begin(115200);
  //SerialBT.begin("ESP32Temp");
  //pinMode(TMP_WAKE_UP_PIN, OUTPUT);
  //digitalWrite(TMP_WAKE_UP_PIN, HIGH);


  initializeBluetoothLE();


  
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP_S * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP_S) + " Seconds");
}

//void loop() {
//  Wire.requestFrom(TMP_DEV, 38);
//
//  while (Wire.available()) {
//    char c = Wire.read();
//    Serial.print(c);
//    SerialBT.write(c);
//  }
//
//  delay(2000);
//}

void loop() {
  int temp_cnt = 0;

  // wake up the temp sensor
  //digitalWrite(TMP_WAKE_UP_PIN, LOW);

  temp_cnt = getTemperaturesRecordedCount();
  //Serial.print("Temp count: ");
  //Serial.println(temp_cnt);
  delay(100);

//  if (temp_cnt > 0) {
//    beginTemperatureRead();
//    delay(100);
//    temp_str = getTemperatureString();
//    Serial.println(temp_str);
//    delay(100);
//    endTemperatureRead();
//  }


  if (temp_cnt > 0 && temp_str_cntr < TEMP_STR_BUFF_SZ) 
  {
    beginTemperatureRead();
    delay(100);
    while (temp_cnt > 0) {  
      temp_str[temp_str_cntr] = getTemperatureString();
      //Serial.println(temp_str[temp_str_cntr]);
      temp_str_cntr++;
      delay(100);
      //temp_cnt = getTemperaturesRecordedCount();
      //Serial.print("Temperature count: ");
      //Serial.println(temp_cnt);
      //delay(100);
      temp_cnt--;
    }
    delay(100);
    endTemperatureRead();
  }

  //delay(2000);
  handleBluetoothLE();
  temp_str_cntr = 0;
  
//  if (waitForBluetoothConnection())
//  {
//    while (temp_str_cntr > 0)
//    {
//      sendBluetoothData(temp_str[temp_str_cntr]);
//      temp_str_cntr--;
//    }
//  }
//  else
//  {
//    // need temp_str[] in RTC memory to survive deep sleeps
//  }

//  char temp_s[20];
//  if (temp_str_cntr > 0) {
//    temp_str[0].toCharArray(temp_s, 20);
//    pCharacteristic->setValue(temp_s);
//    temp_str_cntr = 0;
//  }

  // go to sleep
//  Serial.println("Going to sleep now");
//  Serial.flush();
//  esp_deep_sleep_start();
}

int getTemperaturesRecordedCount() {
  int tmp_cnt = 0;

  // request the number of temperatures recorded, since last read
  Wire.beginTransmission(TMP_DEV);
  Wire.write(MLX_MODE_RECORD_COUNT);
  //Serial.print("Mode: ");
  //Serial.println(MLX_MODE_RECORD_COUNT);
  Wire.endTransmission();
  
  Wire.requestFrom(TMP_DEV, 1);
  if (Wire.available() == 1) {
    tmp_cnt = Wire.read();
  }

  return tmp_cnt;
}

void beginTemperatureRead() {
  // request the temperatures
  Wire.beginTransmission(TMP_DEV);
  Wire.write(MLX_MODE_TEMP_READ);
  //Serial.print("Mode: ");
  //Serial.println(MLX_MODE_TEMP_READ);
  Wire.endTransmission();
}

String getTemperatureString() {
  String temp;
  char c;  

  Wire.requestFrom(TMP_DEV, 14); // 14 chars = ##.##,##.##,## --> obj_tmp,amb_tmp,min
  while (Wire.available()) {
    c = Wire.read();
    temp += c;
  }

  return temp;
}

void endTemperatureRead() {
  Wire.beginTransmission(TMP_DEV);
  Wire.write(MLX_MODE_TEMP_READ_END);
  //Serial.print("Mode: ");
  //Serial.println(MLX_MODE_TEMP_READ_END);
  Wire.endTransmission();
}

//bool waitForBluetoothConnection()
//{
//  while (!SerialBT.hasClient())
//  {
//    Serial.println("No one is knocking...");
//    delay(1000);
//  }
//
//  Serial.println("We've got a friend!!");
//  return true;
//}
//
//void sendBluetoothData(String dat)
//{
//  int index = 0;
//  Serial.println("Transmitting...");
//  while (index < dat.length())
//  {
//    SerialBT.write(dat.charAt(index));
//    index++;
//  }
//  Serial.println("Done");
//}

void initializeBluetoothLE()
{
  // Create the BLE Device
  BLEDevice::init("IR Temp Sensor");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
                    CHARACTERISTIC_UUID_TX,
                    BLECharacteristic::PROPERTY_NOTIFY
                  );
                      
  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
                       CHARACTERISTIC_UUID_RX,
                      BLECharacteristic::PROPERTY_WRITE
                    );

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
}

void handleBluetoothLE()
{
  char temp_s[15];
  if (deviceConnected) {
    temp_str[0].toCharArray(temp_s, 15);
    pTxCharacteristic->setValue(temp_s);
    Serial.println("Setting value");
    //pTxCharacteristic->setValue("77.7,99.9,55");
    pTxCharacteristic->notify();
    delay(10); // bluetooth stack will go into congestion, if too many packets are sent
  }

  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    delay(500); // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
    Serial.println("Connecting");
    oldDeviceConnected = deviceConnected;
  }
}
