const validateJson = text => {
  try {
    JSON.parse(text)
    return null
  } catch (e) {
    return e.message
  }
}

const textareaJsonValidator = e => {
  const value = e.target.value.trim()
  if (!value) {
    e.target.setCustomValidity('')
    return
  }

  const err = validateJson(value)
  const validity = err ? `Invalid JSON: ${err}` : ''
  e.target.setCustomValidity(validity)
}

for (const textarea of document.getElementsByClassName(
  'add-event-form__json'
)) {
  textarea.addEventListener('change', textareaJsonValidator)
}

const roundToSecond = dateMs => Math.floor(dateMs / 1000) * 1000

for (const datetimeWithNow of document.getElementsByClassName(
  'datetime-with-now-button'
)) {
  const input = datetimeWithNow.querySelector(
    '.datetime-with-now-button__input'
  )
  const button = datetimeWithNow.querySelector(
    '.datetime-with-now-button__button'
  )

  button.addEventListener('click', e => {
    e.preventDefault()
    input.valueAsNumber = roundToSecond(new Date().getTime())
  })
}
