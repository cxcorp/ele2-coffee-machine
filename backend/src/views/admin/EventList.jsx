const React = require('react')
const PropTypes = require('prop-types')
const Table = require('react-bootstrap/Table')
const Button = require('react-bootstrap/Button')
const Row = require('react-bootstrap/Row')
const Col = require('react-bootstrap/Col')
const Tab = require('react-bootstrap/Tab')
const Nav = require('react-bootstrap/Nav')

const propTypes = require('../propTypes')

const EventList = ({ sensors, events, deleteActionFormatter }) => {
  const eventsBySensor = events.reduce((acc, val) => {
    const sensorsEvents = acc[val.sensor_id] || []
    sensorsEvents.push(val)
    acc[val.sensor_id] = sensorsEvents
    return acc
  }, {})

  const activeSensors = sensors
    .filter(sensor => !!eventsBySensor[sensor.id])
    .sort((a, b) => a.name.localeCompare(b.name))

  const tabPillsId = 'event-list-table'

  return (
    <>
      <Tab.Container
        transition={false}
        id={tabPillsId}
        defaultActiveKey={
          activeSensors[0] ? `${activeSensors[0].id}` : undefined
        }
      >
        <Row noGutters>
          <Col sm={12} md="auto" className="event-list__nav">
            <Nav
              variant="pills"
              id={`${tabPillsId}-tab`}
              className="flex-column"
              aria-orientation="vertical"
            >
              {activeSensors.map(sensor => (
                <Nav.Link
                  key={sensor.id}
                  data-toggle="pill"
                  href={`#${tabPillsId}-tabpane-${sensor.id}`}
                  eventKey={`${sensor.id}`}
                >
                  {sensor.name}
                </Nav.Link>
              ))}
            </Nav>
          </Col>
          <Col>
            <Tab.Content>
              {activeSensors.map(sensor => {
                const sensorEvents = eventsBySensor[sensor.id]

                return (
                  <Tab.Pane key={sensor.id} eventKey={`${sensor.id}`}>
                    <Table className="event-list-table">
                      <thead>
                        <tr>
                          <th>Event Type</th>
                          <th>Timestamp</th>
                          <th>Data</th>
                          <th>ID</th>
                          <th></th>
                        </tr>
                      </thead>
                      <tbody>
                        {sensorEvents.map(event => {
                          return (
                            <tr key={event.id}>
                              <td>{event.type}</td>
                              <td>{event.timestamp.toISOString()}</td>
                              <td className="event-list__json-cell">
                                {JSON.stringify(event.data)}
                              </td>
                              <td>{event.id}</td>
                              <td style={{ textAlign: 'right' }}>
                                <form
                                  style={{ display: 'inline' }}
                                  method="post"
                                  action={deleteActionFormatter(event)}
                                >
                                  <Button
                                    type="submit"
                                    variant="link"
                                    size="sm"
                                  >
                                    Delete
                                  </Button>
                                </form>
                              </td>
                            </tr>
                          )
                        })}
                      </tbody>
                    </Table>
                  </Tab.Pane>
                )
              })}
            </Tab.Content>
          </Col>
        </Row>
      </Tab.Container>
    </>
  )
}

EventList.propTypes = {
  sensors: PropTypes.arrayOf(PropTypes.shape(propTypes.sensorShape)).isRequired,
  events: PropTypes.arrayOf(PropTypes.shape(propTypes.eventShape)).isRequired,
  deleteActionFormatter: PropTypes.func.isRequired
}

module.exports = EventList
