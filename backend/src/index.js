const express = require('express')
require('express-async-errors')
const path = require('path')
const ExpressReactViews = require('express-react-views')
const config = require('./config')
const adminRouter = require('./admin')
const apiRouter = require('./api')

const app = express()

app.set('views', path.join(__dirname, '/views'))
app.set('view engine', 'jsx')
app.engine('jsx', ExpressReactViews.createEngine())

app.use('/static', express.static(path.join(__dirname, '../static')))

app.use('/admin', adminRouter)
app.use('/api', apiRouter)

app.use('*', (req, res, next) => {
  res
    .type('html')
    .send(
      '<!DOCTYPE html><html><head><title>404</title></head><body>Not found</body></html>'
    )
})

app.use((err, req, res, next) => {
  console.error(err)
  res
    .type('html')
    .send(
      '<!DOCTYPE html><html><head><title>500</title></head><body>Internal server error</body></html>'
    )
})

app.listen(config.PORT, config.BIND_HOST, e => {
  if (e) {
    throw e
  }

  console.log(`Server listening on ${config.BIND_HOST}:${config.PORT}`)
})
