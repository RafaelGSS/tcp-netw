const net = require('net')

const client = new net.Socket()
client
  .connect(3333, '0.0.0.0', () => {
    console.log('Connected')
    setInterval(() => client.write('Hey bro\n'), 1000)
  })
  .on('close', () => {
    console.log('Connection closed');
  })
