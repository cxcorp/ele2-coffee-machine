module.exports = {
  // Util method for migrations - adds a trigger to a table which updates
  // updated_at whenever the row is updated!
  // The stored proc on_update_timestamp is created in the first migration.
  // const { onUpdateTrigger } = require('../knexfile')
  /**
   * @param {string} table Name of the table to which the on_update trigger is
   *                       attached.
   * @returns {string} the raw PSQL CREATE TRIGGER query
   */
  onUpdateTrigger: table => `
    CREATE TRIGGER ${table}_updated_at
    BEFORE UPDATE ON ${table}
    FOR EACH ROW
    EXECUTE PROCEDURE on_update_timestamp();
  `
}
