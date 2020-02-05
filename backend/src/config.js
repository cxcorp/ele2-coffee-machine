if (process.env.NODE_ENV === 'development') {
  require('dotenv').config()
}

const port = parseInt(process.env.PORT, 10)
const allowedUsers = new Set(
  (process.env.ALLOWED_USERS || '')
    .split(';')
    .map(s => s.trim())
    .filter(s => s)
)

module.exports = {
  PORT: isNaN(port) ? 8000 : port,
  BIND_HOST: process.env.BIND_HOST || '0.0.0.0',
  ALLOWED_USERS: allowedUsers,
  SIGN_OUT_ENDPOINT: process.env.SIGN_OUT_ENDPOINT || '/_signout'
}
