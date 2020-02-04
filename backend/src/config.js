if (process.env.NODE_ENV === 'development') {
  require('dotenv').config()
}

const port = parseInt(process.env.PORT, 10)

module.exports = {
  PORT: isNaN(port) ? 8000 : port,
  HOSTNAME: process.env.HOSTNAME || '0.0.0.0'
}
