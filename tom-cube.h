String BINARY_VERSION = "deadbeef";
String BINARY_SERVER_ENDPOINT = "/bin/" + BINARY_VERSION;

String TREND_SERVER_ENDPOINT = "/panel/cubes/";

String OTA_HOSTNAME = "ToM-";
String TREND = "";

long TREND_UPDATE_PERIOD_MILLIS = 30e3L;
long nextTrendUpdate = 0L;

long BINARY_UPDATE_PERIOD_MILLIS = 120e3L;
long nextBinaryUpdate = 0L;

bool needsReset = false;

void checkForNewBinary() {
  t_httpUpdate_return ret = ESPhttpUpdate.update(BINARY_SERVER_ADDRESS, BINARY_SERVER_PORT, BINARY_SERVER_ENDPOINT, "", false, "", false);
  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.println("[UPDATE] failed.");
      break;
    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("[UPDATE] NO update.");
      break;
    case HTTP_UPDATE_OK:
      Serial.println("[UPDATE] OK.");
      needsReset = true;
      break;
  }
}

void setupAndStartOTA() {
  ArduinoOTA.setHostname(OTA_HOSTNAME.c_str());
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

void reset() {
  ESP.deepSleep(500e3);
}

String getTrendFromMacAddress(String mac) {
  if (mac.indexOf("2C:3A:E8:1C:C2:E7") != -1) return  "1";
  if (mac.indexOf("2C:3A:E8:1C:BB:6B") != -1) return  "2";
  if (mac.indexOf("2C:3A:E8:1C:BF:94") != -1) return  "3";
  if (mac.indexOf("2C:3A:E8:1C:C6:74") != -1) return  "4";
  if (mac.indexOf("2C:3A:E8:1C:8E:F0") != -1) return  "5";
  if (mac.indexOf("2C:3A:E8:1C:95:CB") != -1) return  "6";
  if (mac.indexOf("2C:3A:E8:0E:5A:B2") != -1) return  "7";
  if (mac.indexOf("2C:3A:E8:1C:C4:E7") != -1) return  "8";
  if (mac.indexOf("2C:3A:E8:0B:95:2D") != -1) return  "9";
  if (mac.indexOf("2C:3A:E8:1C:C8:2D") != -1) return  "10";
  if (mac.indexOf("60:01:94:42:09:4C") != -1) return  "11";
  if (mac.indexOf("2C:3A:E8:1C:C4:90") != -1) return  "12";
  if (mac.indexOf("2C:3A:E8:1C:C6:90") != -1) return  "16";
  if (mac.indexOf("2C:3A:E8:1B:02:18") != -1) return  "17";
  if (mac.indexOf("2C:3A:E8:1C:BF:D0") != -1) return  "18";
  if (mac.indexOf("2C:3A:E8:1C:6F:28") != -1) return  "19";
  if (mac.indexOf("2C:3A:E8:1C:BD:19") != -1) return  "21";
  if (mac.indexOf("2C:3A:E8:1C:CB:C0") != -1) return  "22";
  if (mac.indexOf("2C:3A:E8:1C:C7:CB") != -1) return  "23";
  if (mac.indexOf("2C:3A:E8:1C:C4:D6") != -1) return  "24";
  if (mac.indexOf("2C:3A:E8:1C:CA:E0") != -1) return  "64";
  return "0";
}

