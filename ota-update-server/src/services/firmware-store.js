const config = require('../config')
const Google = require('./google')
const { swrMemoized, lruMemoized } = require('./util')

/** @type {() => Promise<{version: string, filename: string}>} */
const getCurrentVersion = swrMemoized(
  {
    maxAge: config.FW_VERSION_CACHE_TTL,
  },
  async function getCurrentVersion() {
    const {
      data: { values: rows },
    } = await Google.sheets().spreadsheets.values.get({
      spreadsheetId: config.FW_LATEST_SHEET_ID,
      range: 'VersionTable',
    })

    const dataRow = rows[1]
    const [version, filename] = dataRow
    return { version, filename }
  }
)

/**
 * @param {string} filename
 * @returns {Promise<{id: string, size: string}>}
 */
const getFileMetadata = lruMemoized(
  {
    max: 5,
    maxAge: config.FW_METADATA_CACHE_TTL,
    keyFn: ([filename]) => filename,
    // don't cache the result if the file wasn't found
    shouldCache: (value) => value !== null,
  },
  async function getFileMetadata(filename) {
    // filename is passed inside single quotes, escape it
    const escapedFilename = filename.replace(/'/, "\\'")
    const {
      data: { files: firmwareFiles },
    } = await Google.drive().files.list({
      q: `'${config.FW_FOLDER_ID}' in parents and name='${escapedFilename}'`,
      fields: 'files(id,size)',
    })

    const metadata = firmwareFiles.length > 0 ? firmwareFiles[0] : null
    return metadata
  }
)

/**
 * @type {(fileId: string) => Promise<Buffer>}
 */
const getFileBuffer = lruMemoized(
  { max: 5, maxAge: config.FW_FILE_CACHE_TTL, keyFn: ([fileId]) => fileId },
  async function getFileBuffer(fileId) {
    const { data: arraybuffer } = await Google.drive().files.get(
      {
        alt: 'media',
        fileId: fileId,
      },
      { responseType: 'arraybuffer' }
    )

    return Buffer.from(arraybuffer)
  }
)

module.exports = {
  init: Google.init,
  // drive-db caches for us
  getCurrentVersion,
  getFileMetadata,
  getFileBuffer,
}
