FROM node:12-alpine AS builder

WORKDIR /app

RUN apk add \
    --no-cache \
    --repository http://dl-3.alpinelinux.org/alpine/edge/testing \
    build-base \
    g++ \
    cairo-dev \
    jpeg-dev \
    pango-dev \
    giflib-dev \
    imagemagick \
    librsvg-dev \
    libmount \
    ttf-dejavu \
    fontconfig

COPY package*.json ./
RUN npm install --production --build-from-source

FROM node:12-alpine
RUN apk add \
    --no-cache \
    --repository http://dl-3.alpinelinux.org/alpine/edge/testing \
    pango-dev \
    imagemagick \
    librsvg-dev \
    ttf-dejavu \
    fontconfig

WORKDIR /app

COPY --from=builder /app .
COPY . .
CMD ["node_modules/.bin/ts-node", "src/index.ts"]