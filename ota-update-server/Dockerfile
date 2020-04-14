FROM node:12-alpine AS builder

WORKDIR /app

COPY package*.json ./
RUN npm ci --only=production

COPY . .
CMD ["node", "src/index.js"]