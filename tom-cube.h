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

int find(int arr[], int arrSize, int item) {
  for (int i = 0; i < arrSize; i++) {
    if (arr[i] == item) {
      return i;
    }
  }
  return arrSize;
}

void reset() {
  ESP.deepSleep(500e3);
}

