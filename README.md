# ele2-coffee-machine

Coffee machine IoT project. Easily check the amount of coffee in the pan via a Telegram bot. Electronics II course project.

## Directories

| Directory         | Description                                                                                            |
|-------------------|--------------------------------------------------------------------------------------------------------|
| bot               | Telegram bot which serves the coffee status from the database                                          |
| coffee-sensor     | The code for the ESP8266 sensor                                                                        |
| kicad             | Electronic wiring schema for the sensor                                                                |
| ota-update-server | Coffee sensor over-the-air firmware update server                                                      |
| serial-dumper     | Utility for dumping numbers printed to serial from the coffee-sensor. Useful for generating test data. |

## Architecture

The project contains multiple components:
- The sensor attached to the coffee machine (`coffee-sensor`)
- An OTA firmware update server for the sensor (`ota-update-server`)
- A Telegram bot (`bot`)
- A [Node-RED](https://nodered.org/) instance responsible for receiving the data from the sensor and inserting it into the database
- A [PostgreSQL](https://www.postgresql.org/) database which contains the data sent by the sensor

![](https://github.com/cxcorp/ele2-coffee-machine/raw/master/documentation/architecture-v2.png)

## Sensor schematic

![](https://github.com/cxcorp/ele2-coffee-machine/raw/master/documentation/schematic-v2.png)
