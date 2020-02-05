const React = require('react')
const Container = require('react-bootstrap/Container')
const Row = require('react-bootstrap/Row')
const Col = require('react-bootstrap/Col')
const Button = require('react-bootstrap/Button')

const Layout = require('../Layout')

const AccessDeniedPage = () => {
  return (
    <Layout title="Access Denied">
      <Container as="main">
        <Row className="justify-content-md-center">
          <Col lg={9}>
            <Row>
              <Col>
                <h1 className="mt-5 mb-4">Access Denied</h1>
                <p className="lead">
                  You have not been granted rights to access this page.
                </p>
                <Button
                  className="mt-3"
                  as="a"
                  variant="primary"
                  href="/_signout"
                >
                  Log out
                </Button>
              </Col>
            </Row>
          </Col>
        </Row>
      </Container>
    </Layout>
  )
}

module.exports = AccessDeniedPage
