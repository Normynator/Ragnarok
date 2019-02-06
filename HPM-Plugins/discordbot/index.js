#!/usr/bin/env node

const net = require('net');
const Discord = require('discord.js');

const client = new Discord.Client();
let sockets = [];

net.createServer(socket => {
  socket.setEncoding("utf8"); // parse buffer to utf8
  socket.on('data', (msg) => {
    client.channels.get('263830040909709332').send(msg).then(() => { // ensure client.login() was successfully called else channels.get will return undefined
      console.log('Message sent!'); // log messages?
    }).catch(err => {
      console.log(err); // handle error
    });
  })
  sockets.push(socket);
}).listen(1337, "127.0.0.1");

client.on('message', msg => {
  if (msg.channel.name == 'main' && !msg.author.bot) { // filter for text messages only
    // filter only supported characters for herc (strip emojis or replace them for a nice ascii version)
    // <Haru> That's because the client expects text in a local charset (according to the windows legacy locale settings)
    // <Haru> it's reading the UTF8 stream it receives as latin1 or w/e charset is configured on that one specific computer
    console.log(msg.author.username, msg.content); // also then authors username
    sockets[0].write(`${msg.content}\0`); // check if socket exists and is hercules?
  };
})

client.login('NDQzNzUwODA0MzY5NzAyOTI1.DdR7UQ.WD-2SoVLKDSFjx_jnPi_fnwhiPU');
