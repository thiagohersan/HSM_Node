FROM node:4.8.7-wheezy

RUN dpkg --add-architecture armhf
RUN apt-get update

RUN apt-get install -y arduino-core
## RUN apt-get install -y arduino-core:armhf

ADD . /opt/build/

WORKDIR /opt/build/deps/esp8266/tools/
RUN python get.py

WORKDIR /opt/build/
RUN mkdir deps/tom-cube && cp *.* deps/tom-cube/

WORKDIR /opt/build/deps/tom-cube
RUN md5Hash=$(cat tom-cube.ino Trend.h Trend.cpp | md5sum | grep -oP [0-9a-fA-F]+) && \
    sed -i "s/deadbeef/${md5Hash}/g" parameters.h

WORKDIR /opt/build/deps/
RUN make -f ./makeEspArduino/makeEspArduino.mk \
         ESP_ROOT=./esp8266 \
         SKETCH=./tom-cube/tom-cube.ino \
         CUSTOM_LIBS=./libraries \
         EXCLUDE_DIRS=./libraries/Adafruit_NeoPixel/examples \
         BUILD_ROOT=./mkESP

WORKDIR /opt/build/
CMD ["cp", "deps/mkESP/tom-cube_generic/tom-cube.bin", "/opt/tom-cube/bin"]
