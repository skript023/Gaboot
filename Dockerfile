FROM node

WORKDIR /
EXPOSE 3000

RUN npm i --silent
RUN npm run build

ENTRYPOINT [ "node", "dist/main"]