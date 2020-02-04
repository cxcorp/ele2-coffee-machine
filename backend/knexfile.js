// Update with your config settings.

const config = {
  development: {
    client: 'postgresql',
    connection:
      process.env.PG_CONN_STRING ||
      'postgres://coffee:123@localhost:5432/coffee',
    pool: {
      min: 1,
      max: 10
    },
    migrations: {
      tableName: 'knex_migrations'
    }
  },
  production: {
    client: 'postgresql',
    connection: process.env.PG_CONN_STRING,
    pool: {
      min: 2,
      max: 10
    },
    migrations: {
      tableName: 'knex_migrations'
    }
  }
}

module.exports = config
