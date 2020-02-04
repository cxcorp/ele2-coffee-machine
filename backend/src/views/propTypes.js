const PropTypes = require('prop-types')

const sensorShape = {
  id: PropTypes.number.isRequired,
  name: PropTypes.string.isRequired,
  created_at: PropTypes.instanceOf(Date).isRequired,
  updated_at: PropTypes.instanceOf(Date)
}

const eventShape = {
  id: PropTypes.string.isRequired,
  sensor_id: PropTypes.number.isRequired,
  type: PropTypes.string.isRequired,
  timestamp: PropTypes.instanceOf(Date).isRequired,
  data: PropTypes.object
}

module.exports = {
  sensorShape,
  eventShape
}
