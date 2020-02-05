FROM node:12-alpine

WORKDIR /app

COPY package.json package-lock.json /app/
RUN ["npm", "ci"]

COPY . .
CMD ["node", "src/index.js"]
