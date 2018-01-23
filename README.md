## ToM: Cube

Code for the [NodeMCU](https://nodemcu.readthedocs.io/en/master/) modules that connect the LEDs inside the cubes to the [app](https://github.com/thiagohersan/tom) and [binary](https://github.com/thiagohersan/tom-cube-binary-server) servers.

Their binary can be built using Docker.

--- 

1. **Clone repository:**
```
git clone https://github.com/thiagohersan/tom-cube.git
```

2. **Copy** ```parameters.h.example``` **to** ```parameters.h``` **and change values for WiFi network name, WiFi password, app server address:port and binary server address:port:**  
```
String WIFI_SSID = "my-network";
String WIFI_PASS = "my-password";
```
```
String BINARY_SERVER_ADDRESS = "127.0.0.1";
int BINARY_SERVER_PORT = 8000;
```
```
String TREND_SERVER_ADDRESS = "127.0.0.1";
int TREND_SERVER_PORT = 3000;
```

3. **Build docker images:**
```
docker-compose build
```

4. **Run docker service:**
```
docker-compose up
```

5. **Binary should show up at:**
```
bin/tom-cube.bin
```

6. **Binary can be flashed directly to NodeMCU using command-line tools or an [update server](https://github.com/thiagohersan/tom-cube-binary-server)**
