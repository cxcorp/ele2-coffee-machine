const React = require('react')

const Layout = ({ title, children, css = [], js = [] }) => {
  return (
    <html>
      <head>
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

module.exports = Layout
