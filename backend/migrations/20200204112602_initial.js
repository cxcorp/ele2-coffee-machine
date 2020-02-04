const utils = require('../knex-helpers')

exports.up = async knex => {
  await knex.schema.createTable('sensors', async tb => {
    tb.increments('id')
    tb.string('name')
      .notNullable()
      .unique()
      .index()
    tb.timestamp('created_at')
      .notNullable()
      .defaultTo(knex.fn.now())
    tb.timestamp('updated_at')
  })

  await knex.raw(utils.onUpdateTrigger('sensors'))

  await knex.schema.createTable('events', async tb => {
    tb.bigIncrements('id')
    tb.integer('sensor_id')
      .notNullable()
      .index()
      .references('id')
      .inTable('sensors')
    tb.string('type')
      .notNullable()
      .index()
    tb.jsonb('data').nullable()
    tb.timestamp('timestamp')
      .notNullable()
      .index()
      .defaultTo(knex.fn.now())
  })
}

/**
 * @param {Knex} knex
 */
exports.down = async knex => {
  await knex.schema.dropTable('events')
  await knex.schema.dropTable('sensors')
}
