const { NODE_ENV } = process.env

const getGApiAuthCredentials = () => {
  if (NODE_ENV === 'development') {
    const file = require('fs').readFileSync(
      require('path').join(__dirname, '../.service-account.json'),
      'utf-8'
    )
    return JSON.parse(file)
  }

  return JSON.parse(process.env.GAPI_AUTH_CREDENTIALS)
}

const {
  HOST = '127.0.0.1',
  PORT = '4000',
  FW_FOLDER_ID,
  FW_LATEST_SHEET_ID,
  FW_VERSION_CACHE_TTL = '10',
  FW_METADATA_CACHE_TTL = '20',
  FW_FILE_CACHE_TTL = '40',
} = process.env

const DEVICE_WHITELIST = Object.freeze(JSON.parse(process.env.DEVICE_WHITELIST))
if (!Array.isArray(DEVICE_WHITELIST)) {
  throw new Error('env var DEVICE_WHITELIST expected to contain JSON array')
}

const GAPI_AUTH_CREDENTIALS = Object.freeze(getGApiAuthCredentials())
if (typeof GAPI_AUTH_CREDENTIALS !== 'object') {
  throw new Error(
    'env var GAPI_AUTH_CREDENTIALS expected to contain JSON object'
  )
}

module.exports = {
  NODE_ENV,
  HOST,
  PORT,
  FW_FOLDER_ID,
  FW_LATEST_SHEET_ID,
  FW_VERSION_CACHE_TTL: Number(FW_VERSION_CACHE_TTL),
  FW_METADATA_CACHE_TTL: Number(FW_METADATA_CACHE_TTL),
  FW_FILE_CACHE_TTL: Number(FW_FILE_CACHE_TTL),
  DEVICE_WHITELIST,
  GAPI_AUTH_CREDENTIALS,
}
