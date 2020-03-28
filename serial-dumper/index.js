const SerialPort = require("serialport");
const Readline = require("@serialport/parser-readline");
const fs = require("fs");

const port = new SerialPort("/dev/cu.usbmodem144101", { baudRate: 115200 });

const writer = fs.createWriteStream("./datadump.csv", "utf-8");

const start = async () => {
  const parser = new Readline();
  port.pipe(parser);

  parser.on("data", line => {
    const value = parseFloat(line);
    if (isNaN(value)) {
      console.error("NaN: ", line);
      return;
    }

    writer.write(`${new Date().getTime()};${value}\n`);
  });
};

process.on("SIGINT", function() {
  console.log("Caught interrupt signal");
  writer.close();
  port.close();
  process.exit();
});

start().then(
  () => {},
  err => {
    console.error(err);
  }
);
