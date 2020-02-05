const React = require('react')
const PropTypes = require('prop-types')

const Layout = ({ title, children, css = [], js = [] }) => {
  return (
    <html>
      <head>
        <meta charSet="UTF-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <meta httpEquiv="X-UA-Compatible" content="ie=edge" />
        <title>{title}</title>
        <link
          rel="stylesheet"
          href="/static/vendor/bootstrap.v4.3.1.min.css"
          type="text/css"
        />
        <link href="/static/css/main.css" rel="stylesheet" type="text/css" />
        {css.map(path => (
          <link key={path} href={path} rel="stylesheet" type="text/css" />
        ))}
      </head>
      <body>
        {children}
        {js.map(obj => {
          if (typeof obj === 'string') {
            return <script key={obj} src={obj} />
          }
          return <script {...obj} key={obj.src} />
        })}
      </body>
    </html>
  )
}

Layout.propTypes = {
  title: PropTypes.string.isRequired,
  children: PropTypes.node.isRequired,
  css: PropTypes.arrayOf(PropTypes.string),
  js: PropTypes.arrayOf(
    PropTypes.oneOfType([
      PropTypes.string,
      PropTypes.shape({
        src: PropTypes.string.isRequired
      })
    ])
  )
}

module.exports = Layout
