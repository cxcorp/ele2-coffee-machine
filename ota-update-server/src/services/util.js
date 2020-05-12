const NodeCache = require('node-cache')
const LRU = require('lru-cache')
const { NODE_ENV } = require('../config')

const _debugLog = (prefix, msg, ...args) =>
  console.log(`[${prefix}] ${msg}`, ...args)
const _noOp = () => {}
const createDebugLog = (prefix) =>
  NODE_ENV === 'development' ? _debugLog.bind(null, prefix) : _noOp

const swrDebug = createDebugLog('swr')
const lruDebug = createDebugLog('lru')

/** A single function return value caching wrapper.
 * Basically memoize with stale-while-revalidate semantics
 * Does not pass arguments to `fn` through. If you need to cache
 * separately by arguments, use `node-lru-cache`.
 * @param {{maxAge: number}} options
 * @param {number} options.maxAge Cache time to live in milliseconds
 * @param {() => Promise<any>} fn The memoized function
 */
const swrMemoized = (options, fn) => {
  const { maxAge } = options
  const ttl = maxAge / 1000

  const CACHE_KEY = 'key'
  const cache = new NodeCache({
    stdTTL: ttl,
    deleteOnExpire: false,
    checkperiod: 0,
    useClones: false,
  })

  let isUpdating = false

  cache.on('expired', async (key, val) => {
    // since deleteOnExpire is false, node-cache
    // keeps the previous value in the cache forever
    if (isUpdating) {
      swrDebug(`${fn.name} expired, already updating...`)
      // already updating, keep serving old response
      return
    }

    isUpdating = true
    try {
      swrDebug(`${fn.name} expired, revalidating`)
      const newValue = await fn()
      cache.set(CACHE_KEY, newValue)
    } catch (e) {
      // just eat the exception, try again next time by setting
      // isUpdating = false
    }
    swrDebug(`${fn.name} revalidation complete`)
    isUpdating = false
  })

  return async () => {
    const cached = cache.get(CACHE_KEY)
    if (cached) {
      swrDebug(`${fn.name} cache hit`)
      return cached
    }

    // get initial value and persist to cache
    swrDebug(`${fn.name} cache miss, expires in ${ttl}`)
    setTimeout(() => {
      swrDebug(`${fn.name} is now expired`)
    }, ttl * 1000)
    const value = await fn()
    cache.set(CACHE_KEY, value)
    return value
  }
}

const lruMemoized = ({ max, maxAge, keyFn, shouldCache = () => true }, fn) => {
  const cache = new LRU({ max, maxAge })

  return async (...args) => {
    const cacheKey = keyFn(args)
    const cached = cache.get(cacheKey)
    if (cached) {
      lruDebug(`${fn.name} (${cacheKey}) cache hit`)
      return cached
    }

    lruDebug(`${fn.name} (${cacheKey}) cache miss`)
    const value = await fn(...args)
    if (shouldCache(value)) {
      lruDebug(`${fn.name} (${cacheKey}) revalidated and cached`)
      cache.set(cacheKey, value)
    } else {
      lruDebug(`${fn.name} (${cacheKey}) revalidated but not cached`)
    }
    return value
  }
}

module.exports = {
  swrMemoized,
  lruMemoized,
}
