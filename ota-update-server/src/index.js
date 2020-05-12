const express = require('express')
require('express-async-errors')
const semver = require('semver')
const Status = require('http-status-codes')
if (process.env.NODE_ENV === 'development') {
  require('dotenv').config()
}
const config = require('./config')
const FirmwareStore = require('./services/firmware-store')
const {
  extractEspUpdateRequest,
  requireDeviceWhitelist,
} = require('./middleware')

const app = express()
app.disable('x-powered-by')

app.get(
  '/update',
  extractEspUpdateRequest,
  requireDeviceWhitelist,
  async function updateHandler(req, res) {
    const { device } = req.locals

    const currentVersion = await FirmwareStore.getCurrentVersion()
    if (semver.lt(currentVersion.version, device.firmwareVersion)) {
      console.log('Chip is running newer firmware than server provides')
      console.log(req.headers)
      console.log(req.ips)
      return res.sendStatus(Status.NOT_MODIFIED)
    }

    if (!semver.gt(currentVersion.version, device.firmwareVersion)) {
      // chip's firmware version was up to date
      return res.sendStatus(Status.NOT_MODIFIED)
    }

    const fileMetadata = await FirmwareStore.getFileMetadata(
      currentVersion.filename
    )
    if (!fileMetadata) {
      console.error(
        `Firmware file ${currentVersion.filename} not found in FW folder!`
      )
      return res.sendStatus(Status.NOT_FOUND)
    }

    const buffer = await FirmwareStore.getFileBuffer(fileMetadata.id)

    if (device.freeSpace < Number(buffer.length)) {
      console.log(
        `Client ${device.mac} running ${device.firmwareVersion} did not have enough space for update!`
      )
      console.log(req.headers)
      return res.status(Status.BAD_REQUEST).send('Not enough space for update')
    }

    res.header('X-OTA-FW-Version', currentVersion.version)
    res.header('Content-Type', 'application/octet-stream')
    res.header('Content-Length', `${buffer.length}`)
    res.status(200).end(buffer, 'binary')
  }
)

app.use((err, req, res, next) => {
  console.error(err)
  res.sendStatus(500)
})

console.log('Initializing Google Drive API client')
FirmwareStore.init()
  .then(() => {
    app.listen(config.PORT, config.HOST, (e) => {
      if (e) {
        console.error(e)
        process.exit(1)
      }

      console.log(`listening on ${config.HOST}:${config.PORT}`)
    })
  })
  .catch((e) => {
    console.error(e)
    process.exit(1)
  })
