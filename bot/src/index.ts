import TelegramBot from 'node-telegram-bot-api'

if (process.env.NODE_ENV === 'development') {
  require('dotenv').config()
}

const { TG_TOKEN } = process.env

const bot = new TelegramBot(TG_TOKEN!, { polling: true })

bot.onText(/^\/status/, msg => {
  const chatId = msg.chat.id

  bot.sendMessage(chatId, 'hello world')
})
