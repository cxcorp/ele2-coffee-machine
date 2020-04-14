import TelegramBot from 'node-telegram-bot-api'
import Knex from 'knex'
import dateFormatDistance from 'date-fns/formatDistance'
import fiLocale from 'date-fns/locale/fi'
import { JSDOM } from 'jsdom'
const Anychart = require('anychart')
const AnychartExport = require('anychart-nodejs')

if (process.env.NODE_ENV === 'development') {
  require('dotenv').config()
}

const { NODE_ENV, TG_TOKEN } = process.env
const knexfile = require('../knexfile')
const knex = Knex(knexfile[NODE_ENV!])

const bot = new TelegramBot(TG_TOKEN!, { polling: true, filepath: false })

const generateImage = async (weights: { timestamp: any; weight: number }[]) => {
  const jsdom = new JSDOM('<body><div id="container"></div></body>')
  const window = jsdom.window
  const anychart = Anychart(window)
  const anychartExport = AnychartExport(anychart)

  const chart = anychart.line(
    weights.map(row => ({
      x: new Date(row.timestamp),
      value: row.weight
    }))
  )

  const dateScale = anychart.scales.dateTime()
  const dateTicks = dateScale.ticks()
  dateTicks.interval(0, 0, 0, 0, 1)
  dateScale.minimum(new Date(weights[0].timestamp).getTime())
  dateScale.maximum(new Date(weights[weights.length - 1].timestamp).getTime())
  chart.xScale(dateScale)

  const yScale = anychart.scales.linear()
  yScale.softMinimum(-0.5)
  yScale.softMaximum(3.5)
  chart.yScale(yScale)

  const labels = chart.xAxis().labels()
  labels.enabled(true)
  labels.fontFamily('"Dejavu Sans"')
  labels.fontSize(12)
  labels.format(function(value: any) {
    const date = new Date(value['tickValue'])
    return (
      date
        .getHours()
        .toString()
        .padStart(2, '0') +
      ':' +
      date
        .getMinutes()
        .toString()
        .padStart(2, '0')
    )
  })

  chart.bounds(0, 0, 800, 600)
  chart.container('container')
  chart.draw()

  const image = await anychartExport.exportTo(chart, 'png')
  return image
}

bot.onText(/^\/status/, async msg => {
  const chatId = msg.chat.id

  const weights = (
    await knex('weight')
      .where('timestamp', '>=', new Date(new Date().getTime() - 6 * 300000))
      .orderBy('timestamp', 'desc')
      .select(['timestamp', 'weight'])
  ).reverse()

  if (weights.length === 0) {
    bot.sendMessage(chatId, 'Ei tietoa sensorilta yli puoleen tuntiin!')
    return
  }

  bot.sendMessage(
    chatId,
    `Paino: ${weights[weights.length - 1].weight.toFixed(1)} kg (mitattu ${weights[
      weights.length - 1
    ].timestamp.toISOString()})`
  )

  try {
    const image = await generateImage(weights)

    const yMin = weights
      .map(w => w.weight)
      .reduce((acc, val) => (val < acc ? acc : val))
    const yMax = weights
      .map(w => w.weight)
      .reduce((acc, val) => (val > acc ? acc : val))
    ;(bot.sendPhoto as any)(
      chatId,
      image,
      {
        caption: `x-min: ${dateFormatDistance(
          new Date(weights[0].timestamp),
          new Date(),
          {
            locale: fiLocale,
            includeSeconds: true,
            addSuffix: true
          }
        )}, x-max: ${dateFormatDistance(
          new Date(weights[weights.length - 1].timestamp),
          new Date(),
          {
            locale: fiLocale,
            includeSeconds: true,
            addSuffix: true
          }
        )}\ny-min: ${yMin < -0.5 ? yMin : -0.5}, y-max: ${
          yMax > 3.5 ? yMax : 3.5
        }\n\nkeskiarvo: ${(
          weights.map(w => w.weight).reduce((acc, val) => acc + val) /
          weights.length
        ).toFixed(2)}`
      },
      { filename: 'chart.png', contentType: 'image/png' }
    )
  } catch (e) {
    console.error(e)
    bot.sendMessage(chatId, '*Error occurred while generating image*', {
      parse_mode: 'MarkdownV2'
    })
  }
})
