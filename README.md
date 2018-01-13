# homie-h801 led dimmer firmware

This firmware is for ESP8266 based LED RGBWW dimmer boards. You get them
branded as "H801 Dimmer" from ebay and aliexpress.

It uses homie and advertises a "strip" node with a "color" property.

It expects four integer numbers with a value between 0 and 1024 separated by
comma.

``mosquitto_pub -h $MQTTHOST -t 'homie/rgbwstrip1/strip/color/set' -m '1023,1000,1000,1000,0'``

The values are RED,GREEN,BLUE,W1,W2.
