# ota-update-server

Server which serves OTA updates to the coffee-sensor.

## Running locally under the same WiFi with the sensor

1. Install nodejs, preferably version 12 or newer.
2. Run `npm install`
3. Copy a signed firmware produced by Arduino IDE to the `firmware/` directory
4. In `config`, copy `latest.sample.json` to `latest.json`.
5. In `latest.json`, set the `version` to match the binary's version, and the `file` to match the filename of the binary in the `firmware/` directory
6. In `config`, copy `device-whitelist.sample.json` to `device-whitelist.json` and include your device in the whitelist
    - protip: if the sensor is not in the whitelist, it'll log the MAC in the console so you can copy it
7. Start server with `npm start`
8. Set the sensor's firmware update URL to your computer's local IP address with the correct port and path, e.g.: `http://192.168.1.43:4000/update` (replace `192.168.1.43` with your own IP)
    - The port `4000` and path `/update` are default.
9. Make sure your computer's firewall doesn't block connections from the sensor
10. Wait for the sensor to check for updates

## Running with Docker

1. `docker build -t ota-update-server .`
2. `docker run -it -p 4000:4000 -v ./config:/app/config -v ./firmware:/app/firmware ota-update-server`
    - (optional) set env vars `-e HOST=127.0.0.1 -e PORT=9000`
3. Steps 3-10 excluding 7 from previous section
