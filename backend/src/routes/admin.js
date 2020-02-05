const express = require('express')
const bodyParser = require('body-parser')

const config = require('../config')
const knex = require('../db')

const adminRouter = express.Router()

adminRouter.use(bodyParser.urlencoded({ extended: true }))

adminRouter.get('/', async (req, res) => {
  const sensors = await knex('sensors').select()
  const events = await knex('events').select()
  res.render('admin', {
    sensors,
    events,
    signOutHref: config.SIGN_OUT_ENDPOINT
  })
})

adminRouter.post('/sensors', async (req, res) => {
  const { name } = req.body
  if (!name) {
    return res.redirect('/admin')
  }

  const existing = await knex('sensors')
    .where({ name })
    .first()

  if (existing) {
    return res.redirect('/admin')
  }

  await knex('sensors').insert({ name })
  res.redirect('/admin')
})

adminRouter.post('/sensors/:id/delete', async (req, res) => {
  const { id } = req.params
  if (!id) {
    return res.redirect('/admin')
  }

  const parsedId = parseInt(id, 10)
  if (isNaN(parsedId)) {
    return res.redirect('/admin')
  }

  const sensor = await knex('sensors')
    .where({ id: parsedId })
    .first()
  if (!sensor) {
    return res.redirect('/admin')
  }

  await knex('events')
    .where({ sensor_id: parsedId })
    .delete()
  await knex('sensors')
    .where({ id: parsedId })
    .delete()
  res.redirect('/admin')
})

const isJson = obj => {
  try {
    JSON.parse(obj)
    return true
  } catch (e) {
    return false
  }
}

adminRouter.post('/events', async (req, res) => {
  const { sensor_id, type, data, timestamp } = req.body

  if (!sensor_id || !type || !timestamp) {
    // validation fail
    return res.redirect('/admin')
  }

  const parsedSensorId = parseInt(sensor_id, 10)
  const parsedTimestamp = new Date(timestamp)

  if (
    isNaN(parsedSensorId) ||
    isNaN(parsedTimestamp) ||
    (data && !isJson(data))
  ) {
    // validation fail
    return res.redirect('/admin')
  }

  const sensor = await knex('sensors')
    .where({ id: parsedSensorId })
    .first()

  if (!sensor) {
    // validation fail
    return res.redirect('/admin')
  }

  await knex('events').insert({
    sensor_id: parsedSensorId,
    type: type,
    data: data || null
  })

  return res.redirect('/admin')
})

adminRouter.post('/events/:id/delete', async (req, res) => {
  const { id } = req.params

  if (!id) {
    return res.redirect('/admin')
  }

  await knex('events')
    .where({ id })
    .delete()
  return res.redirect('/admin')
})

module.exports = adminRouter
