const express = require('express')
const bodyParser = require('body-parser')

const config = require('../config')
const knex = require('../db')

const router = express.Router()

router.use(bodyParser.json())

router.get('/coffee-status', async (req, res) => {
  const { cutoff } = req.query

  const cutoffSeconds = isNaN(parseInt(cutoff, 10)) ? 30 : parseInt(cutoff, 10)

  const SENSOR = 'oh/coffee-1'
  const events = (
    await knex.raw(
      `
    SELECT
      *
    FROM events
      LEFT JOIN sensors
        ON sensors.id = events.sensor_id
    WHERE
      sensors.name = :sensorName
      AND events.timestamp > CURRENT_TIMESTAMP - cast(:cutoffSeconds || ' seconds' AS INTERVAL)
    ORDER BY events.timestamp
    LIMIT 1
  `,
      { sensorName: SENSOR, cutoffSeconds }
    )
  ).rows

  /*if (!event) {
    return res.json({ status: 'no-coffee' })
  }*/

  res.json(events)
})

router.use('*', (req, res) => {
  res.status(404).json({ error: `path ${req.path} not found` })
})

router.use((err, req, res, next) => {
  console.error(err)
  res.status(500).json({ error: 'internal server error' })
})

module.exports = router
