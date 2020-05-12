const { google } = require('googleapis')
const { GAPI_AUTH_CREDENTIALS } = require('../config')

const SCOPES = [
  'https://www.googleapis.com/auth/drive.metadata.readonly',
  'https://www.googleapis.com/auth/drive.readonly',
  'https://www.googleapis.com/auth/spreadsheets.readonly',
]

let drive = null
let sheets = null

module.exports = {
  init: async () => {
    const auth = await google.auth.getClient({
      credentials: GAPI_AUTH_CREDENTIALS,
      scopes: SCOPES,
    })
    drive = google.drive({
      version: 'v3',
      auth,
    })
    sheets = google.sheets({
      version: 'v4',
      auth,
    })
  },
  drive: () => drive,
  sheets: () => sheets,
}
