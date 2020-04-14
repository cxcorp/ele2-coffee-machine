# coffee-sensor

## Signing binaries

The OTA system requires all produced firmware binaries to be signed with our private key. To produce signed binaries, copy `private.key` into this directory. If `public.key` does not exist, generate it from the private key with `openssl rsa -in private.key -outform PEM -pubout -out public.key`. If both of these files are present, Arduino IDE will sign the binary after compiling.

To see where the signed binary is produced, enable Show verbose output during compilation from Arduino IDE's preferences:

> ![](https://i.imgur.com/wi1X482.png)

After compilation, you should see the following in the console:

> ![](https://i.imgur.com/jrFxLEf.png)

The path after "Signed binary:" is where the signed binary is.

## OTA

The sensor checks for firmware updates over HTTP every minute. This check happens intelligently so that the sensor sends its current version to the server, and the server either responds with a 304 Not Found if the version is up to date, or the updated binary if an update as found.

The binaries are signed via public key cryptography, and the sensor validates the signature of a downloaded binary so that only authentic updates are applied.

The public key used for verification is configured through `SigningKey.cpp`. A copy of this key is found in the file `public.key`. The Arduino IDE supposedly handles this if both `private.key` and `public.key` is found, but testing showed that the public key still needs to be applied manually on the ESP8266 Arduino board v2.6.3.

## Maintenance mode

If the sensor's reset button is clicked twice within two seconds, the server starts in maintenance mode.
In maintenance mode, the sensor starts its own WiFi network named `OH sensor admin` with the passphrase `12345678`. Connecting to this WiFi access point and navigating to http://192.168.1.1/ opens the configuration portal, from which the WiFi client credentials and WebSocket URL can be specified.

## Config server assets

The config server's assets (index.html, bootstrap.css) are served directly from the ESP's flash memory.
The `bootstrap.css` file is pre-compressed with gzip, and the gzipped bytes are compiled directly into PROGMEM (see `ConfigWebAssets.cpp`). The `index.html` file is simply ran through a minifier.

These files' unminified or uncompressed versions can be found from the `assets` directory.

To modify `index.html`, edit the HTML, then minify it [here](https://kangax.github.io/html-minifier/) (from the right, untick _Remove optional tags_!). If the minifier doesn't minify the JavaScript code inside the `<script>` tags, use [this](https://skalman.github.io/UglifyJS-online/) JS minifier to minify it and replace it with the minified version. Copy this minified output to your text editor of choice, and do the following replacements with Replace All: `\` -> `\\` and `"` -> `\"`. These escapes are needed so that the string can be copy pasted into the `ConfigWebAssets.cpp` file.

As for `bootstrap.css`, should there be a need to modify it, get the minified version (`bootstrap.min.css`), then run `gzip -k9 index.min.html && xxd -i index.min.html.gz > copyme.txt` to compress the HTML with gzip and generate almost copypasteable C code. Replace the new length and bytes in the `ConfigWebAssets.cpp` file. Remember to keep the `static const char` and `PROGMEM` words that are present in the code currently.
