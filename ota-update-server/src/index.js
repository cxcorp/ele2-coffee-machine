const express = require('express')
const fs = require('fs')
const path = require('path')
const semver = require('semver')
const Status = require('http-status-codes')

const existsAsync = require('util').promisify(fs.exists)

const CONFIG_FILE = path.join(__dirname, '../config', 'latest.json')
const FIRMWARE_DIR = path.join(__dirname, '../firmware')

fs.mkdirSync(FIRMWARE_DIR, { recursive: true })

if (!fs.existsSync(FIRMWARE_DIR) || !fs.statSync(FIRMWARE_DIR).isDirectory()) {
  console.error(
    `FIRMWARE_DIR (${FIRMWARE_DIR}) does not exist, is not accessible, or is not a directory!`
  )
  process.exit(1)
}

const app = express()

const getCurrentConfig = async () => {
  const configExists = await existsAsync(CONFIG_FILE)
  if (!configExists) {
    throw new Error(`Config file (${CONFIG_FILE}) is missing!`)
  }

  const content = await fs.promises.readFile(CONFIG_FILE)
  return JSON.parse(content)
}

app.get('/update', async (req, res) => {
  const chipFirmwareVersion = semver.valid(req.header('x-esp8266-version'))
  if (
    req.header('user-agent') !== 'ESP8266-http-Update' ||
    req.header('x-esp8266-mode') !== 'sketch' ||
    !req.header('x-esp8266-sta-mac') ||
    !req.header('x-esp8266-free-space') ||
    isNaN(req.header('x-esp8266-free-space')) ||
    !chipFirmwareVersion
  ) {
    console.log('Invalid request')
    console.log(req.headers)
    console.log(req.ips)
    return res.sendStatus(Status.UNAUTHORIZED)
  }

  let currentConfig
  try {
    currentConfig = await getCurrentConfig()
  } catch (e) {
    console.error('Failed to read latest config:')
    console.error(e)
    return res.sendStatus(Status.INTERNAL_SERVER_ERROR)
  }

  if (semver.lt(currentConfig.version, chipFirmwareVersion)) {
    // Chip is running newer firmware than we provide?
    console.log('Chip is running newer firmware than server provides')
    console.log(req.headers)
    console.log(req.ips)
    return res.sendStatus(Status.NOT_MODIFIED)
  }
  if (!semver.gt(currentConfig.version, chipFirmwareVersion)) {
    // chip's firmware version was up to date - send NOT_MODIFIED
    return res.sendStatus(Status.NOT_MODIFIED)
  }

  const mac = req.header('x-esp8266-sta-mac')
  const freeSpace = parseInt(req.header('x-esp8266-free-space'), 10)

  console.log(
    `Sending update ${currentConfig.version} (${currentConfig.file}) to client ${mac} running ${chipFirmwareVersion}`
  )

  const filepath = path.join(FIRMWARE_DIR, currentConfig.file)
  const stats = await fs.promises.stat(filepath)
  const newFirmwareSize = stats.size

  if (freeSpace < newFirmwareSize) {
    console.log(
      `Client ${mac} running ${chipFirmwareVersion} did not have enough space for update!`
    )
    return res.status(Status.BAD_REQUEST).send('Not enough space')
  }

  if (!(await existsAsync(filepath))) {
    console.error(
      `Firmware file was specified in latest.json but not found on disk! ${filepath}`
    )
    return res.statusStatus(Status.INTERNAL_SERVER_ERROR)
  }

  res.status(Status.OK).download(filepath)
})

const hostname = process.env.HOST || '127.0.0.1'
const port = process.env.PORT || 4000
app.listen(port, hostname, (e) => {
  if (e) {
    console.error(e)
    process.exit(1)
  }

  console.log(`listening on ${hostname}:${port}`)
})
