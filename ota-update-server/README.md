# ota-update-server

Server which serves OTA updates to the coffee-sensor.

## Running locally under the same WiFi with the sensor

1. Install nodejs, preferably version 12 or newer.
2. Run `npm install`
3. In `config`, copy `latest.sample.json` to `latest.json`.
4. Copy a signed firmware produced by Arduino IDE to the `firmware/` directory
5. In `latest.json`, set the `version` to match the binary's version, and the `file` to match the filename of the binary in the `firmware/` directory
6. Set the sensor's firmware update URL to your computer's local IP address with the correct port and path, e.g.: `http://192.168.1.43:4000/update` (replace `192.168.1.43` with your own IP)
    - The port `4000` and path `/update` are default.
7. Make sure your computer's firewall doesn't block connections from the sensor
8. Wait for the sensor to check for updates
