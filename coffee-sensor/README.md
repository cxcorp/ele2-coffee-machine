# coffee-sensor

## Config server assets

The config server's assets (index.html, bootstrap.css) are served directly from the ESP's flash memory.
The `bootstrap.css` file is pre-compressed with gzip, and the gzipped bytes are compiled directly into PROGMEM (see `ConfigWebAssets.cpp`). The `index.html` file is simply ran through a minifier.

These files' unminified or uncompressed versions can be found from the `assets` directory.

To modify `index.html`, edit the HTML, then minify it [here](https://kangax.github.io/html-minifier/) (from the right, untick _Remove optional tags_!). If the minifier doesn't minify the JavaScript code inside the `<script>` tags, use [this](https://skalman.github.io/UglifyJS-online/) JS minifier to minify it and replace it with the minified version. Copy this minified output to your text editor of choice, and do the following replacements with Replace All: `\` -> `\\` and `"` -> `\"`. These escapes are needed so that the string can be copy pasted into the `ConfigWebAssets.cpp` file.

As for `bootstrap.css`, should there be a need to modify it, get the minified version (`bootstrap.min.css`), then run `gzip -k9 index.min.html && xxd -i index.min.html.gz > copyme.txt` to compress the HTML with gzip and generate almost copypasteable C code. Replace the new length and bytes in the `ConfigWebAssets.cpp` file. Remember to keep the `static const char` and `PROGMEM` words that are present in the code currently.
