const Status = require('http-status-codes')
const semver = require('semver')
const { isWhitelisted } = require('./services/device-whitelist')

const extractEspUpdateRequest = (req, res, next) => {
  const firmwareVersion = semver.valid(req.header('x-esp8266-version'))
  const mac = req.header('x-esp8266-sta-mac')

  if (
    req.header('user-agent') !== 'ESP8266-http-Update' ||
    req.header('x-esp8266-mode') !== 'sketch' ||
    !mac ||
    !req.header('x-esp8266-free-space') ||
    isNaN(req.header('x-esp8266-free-space')) ||
    !firmwareVersion
  ) {
    console.log('Invalid request')
    console.log(req.headers)
    console.log(req.ips)
    return res.sendStatus(Status.UNAUTHORIZED)
  }

  req.locals = {
    ...req.locals,
    device: {
      mac,
      firmwareVersion,
      freeSpace: parseInt(req.header('x-esp8266-free-space'), 10),
    },
  }
  next()
}

// assume placed after extractEspUpdateRequest
const requireDeviceWhitelist = (req, res, next) => {
  if (!isWhitelisted(req.locals.device.mac)) {
    console.log(
      `Disallowing device ${req.locals.device.mac} from update, MAC not whitelisted`
    )
    console.log(req.headers)
    return res.sendStatus(Status.UNAUTHORIZED)
  }

  next()
}
module.exports = { extractEspUpdateRequest, requireDeviceWhitelist }
