const React = require('react')
const PropTypes = require('prop-types')
const Table = require('react-bootstrap/Table')
const Button = require('react-bootstrap/Button')

const propTypes = require('../propTypes')

const SensorList = ({ sensors, deleteActionFormatter }) => {
  return (
    <>
      <Table hover>
        <thead>
          <tr>
            <th>Name</th>
            <th>Created</th>
            <th>Updated</th>
            <th>ID</th>
            <th></th>
          </tr>
        </thead>
        <tbody>
          {sensors.map(sensor => {
            const { id, name, created_at, updated_at } = sensor
            return (
              <tr className="sensor-list-item" key={id}>
                <td>{name}</td>
                <td>{created_at ? created_at.toISOString() : '-'}</td>
                <td>{updated_at ? created_at.toISOString() : '-'}</td>
                <td>{id}</td>
                <td style={{ textAlign: 'right' }}>
                  <form
                    style={{ display: 'inline' }}
                    method="post"
                    action={deleteActionFormatter(sensor)}
                  >
                    <Button type="submit" variant="link" size="sm">
                      Delete
                    </Button>
                  </form>
                </td>
              </tr>
            )
          })}
        </tbody>
      </Table>
    </>
  )
}

SensorList.propTypes = {
  sensors: PropTypes.arrayOf(PropTypes.shape(propTypes.sensorShape)).isRequired,
  deleteActionFormatter: PropTypes.func
}

module.exports = SensorList
