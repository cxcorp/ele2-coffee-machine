FROM node:12

WORKDIR /app

COPY package.json package-lock.json /app/
RUN ["npm", "ci"]

COPY . .
CMD ["node", "src/index.js"]
