const React = require('react')
const PropTypes = require('prop-types')
const Form = require('react-bootstrap/Form')
const Button = require('react-bootstrap/Button')
const InputGroup = require('react-bootstrap/InputGroup')

const AddSensorForm = ({ action }) => {
  return (
    <Form inline method="post" action={action}>
      <fieldset style={{ width: '100%' }}>
        <InputGroup size="sm">
          <Form.Control
            type="text"
            placeholder="Sensor name"
            name="name"
            required
          />
          <InputGroup.Append>
            <Button variant="primary" type="submit">
              Register new sensor
            </Button>
          </InputGroup.Append>
        </InputGroup>
      </fieldset>
    </Form>
  )
}

AddSensorForm.propTypes = {
  action: PropTypes.string.isRequired
}

module.exports = AddSensorForm
