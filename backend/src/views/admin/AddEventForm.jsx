const React = require('react')
const PropTypes = require('prop-types')
const Row = require('react-bootstrap/Row')
const Col = require('react-bootstrap/Col')
const Form = require('react-bootstrap/Form')
const Button = require('react-bootstrap/Button')
const InputGroup = require('react-bootstrap/InputGroup')

const propTypes = require('../propTypes')

const AddEventForm = ({ sensors, action }) => {
  return (
    <Form method="post" action={action}>
      <Row>
        <Form.Group as={Col} controlId="addEventForm.sensor" sm="6" md="4">
          <Form.Label>Sensor</Form.Label>
          <Form.Control size="sm" as="select" name="sensor_id" required>
            {[...sensors]
              .sort((a, b) => a.name.localeCompare(b.name))
              .map(({ id, name }) => (
                <option key={id} value={id}>
                  {name}
                </option>
              ))}
          </Form.Control>
        </Form.Group>
        <Form.Group as={Col} controlId="addEventForm.type" sm="6" md="3">
          <Form.Label>Event Type</Form.Label>
          <Form.Control
            size="sm"
            type="text"
            name="type"
            placeholder="measurement"
            required
          />
        </Form.Group>
        <Form.Group as={Col} sm="12" md="5" controlId="addEventForm.timestamp">
          <Form.Label>Timestamp</Form.Label>
          <InputGroup size="sm" className="datetime-with-now-button">
            <Form.Control
              className="datetime-with-now-button__input"
              type="datetime-local"
              name="timestamp"
              step="1"
              required
            />

            <InputGroup.Append>
              <Button
                className="datetime-with-now-button__button"
                variant="outline-secondary"
              >
                Now
              </Button>
            </InputGroup.Append>
          </InputGroup>
        </Form.Group>
      </Row>

      <Row>
        <Form.Group as={Col} controlId="addEventForm.data">
          <Form.Label>
            Data <span className="text-secondary font-italic">(optional)</span>
          </Form.Label>
          <InputGroup size="sm">
            <InputGroup.Prepend>
              <InputGroup.Text>JSON</InputGroup.Text>
            </InputGroup.Prepend>
            <Form.Control
              as="textarea"
              name="data"
              className="add-event-form__json"
            />
          </InputGroup>
        </Form.Group>
      </Row>

      <Row className="justify-content-end">
        <Col sm md="3">
          <Button size="sm" type="submit" variant="primary" block>
            Insert event
          </Button>
        </Col>
      </Row>
    </Form>
  )
}

AddEventForm.propTypes = {
  sensors: PropTypes.arrayOf(PropTypes.shape(propTypes.sensorShape)).isRequired,
  action: PropTypes.string.isRequired
}

module.exports = AddEventForm
