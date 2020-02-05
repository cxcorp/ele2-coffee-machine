const React = require('react')
const PropTypes = require('prop-types')
const cz = require('classnames')
const Container = require('react-bootstrap/Container')
const Row = require('react-bootstrap/Row')
const Col = require('react-bootstrap/Col')

const Layout = require('../Layout')
const AddEventForm = require('./AddEventForm')
const AddSensorForm = require('./AddSensorForm')
const SensorList = require('./SensorList')
const EventList = require('./EventList')
const propTypes = require('../propTypes')

const Block = ({ children, padding = true }) => (
  <div className={cz('block', { 'block--no-padding': !padding })}>
    {children}
  </div>
)
const SectionTitle = ({ children, ...rest }) => <h2 {...rest}>{children}</h2>

const AdminPage = ({ sensors, events }) => {
  const addSensorAction = '/admin/sensors'
  const deleteSensorAction = sensor => `/admin/sensors/${sensor.id}/delete`
  const addEventAction = '/admin/events'
  const deleteEventAction = event => `/admin/events/${event.id}/delete`

  return (
    <Layout
      title="Admin"
      css={['/static/css/admin.css']}
      js={[
        '/static/vendor/jquery-3.3.1.slim.min.js',
        '/static/vendor/bootstrap.v4.3.1.min.js',
        '/static/scripts/admin.js'
      ]}
    >
      <Container as="main" className="admin-page">
        <Row className="justify-content-md-center">
          <Col lg={9}>
            <Row className="my-5">
              <Col>
                <h1>Coffee Sensor System Administration</h1>
              </Col>
            </Row>

            <Row>
              <Col>
                <SectionTitle className="my-3">Sensors</SectionTitle>
              </Col>
            </Row>

            <Row>
              <Col>
                <Block>
                  <AddSensorForm action={addSensorAction} />
                </Block>
              </Col>
            </Row>
            <Row className="mb-3">
              <Col>
                <Block padding={false}>
                  <SensorList
                    sensors={sensors}
                    deleteActionFormatter={deleteSensorAction}
                  />
                </Block>
              </Col>
            </Row>

            <SectionTitle className="my-3">Events</SectionTitle>
            <Block>
              <AddEventForm sensors={sensors} action={addEventAction} />
            </Block>

            <Block padding={false}>
              <EventList
                sensors={sensors}
                events={events}
                deleteActionFormatter={deleteEventAction}
              />
            </Block>
          </Col>
        </Row>
      </Container>
    </Layout>
  )
}

AdminPage.propTypes = {
  sensors: PropTypes.arrayOf(PropTypes.shape(propTypes.sensorShape)).isRequired
}

module.exports = AdminPage
