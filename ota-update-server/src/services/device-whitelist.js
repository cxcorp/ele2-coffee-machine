const { DEVICE_WHITELIST } = require('../config')

// mac -> true lookup object
const deviceWhitelist = Object.freeze(
  DEVICE_WHITELIST.reduce((acc, val) => {
    acc[val.toUpperCase()] = true
    return acc
  }, Object.create(null))
)

module.exports = {
  isWhitelisted: (mac) => !!deviceWhitelist[mac],
}
