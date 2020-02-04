const Knex = require('knex')
const knexfile = require('../knexfile')[process.env.NODE_ENV]

const knex = Knex(knexfile)

module.exports = knex
