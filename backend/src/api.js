const express = require('express')
const bodyParser = require('body-parser')
const knex = require('./db')

const apiRouter = express.Router()

apiRouter.use(bodyParser.json())

apiRouter.get('/sensors', async (req, res) => {
  res.json({ ok: 1 })
})

module.exports = apiRouter
