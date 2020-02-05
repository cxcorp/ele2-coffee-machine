if (process.env.NODE_ENV === 'development') {
  require('dotenv').config()
}

const port = parseInt(process.env.PORT, 10)

module.exports = {
  PORT: isNaN(port) ? 8000 : port,
  BIND_HOST: process.env.BIND_HOST || '0.0.0.0'
}
