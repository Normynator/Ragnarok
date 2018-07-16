#!/usr/bin/env node

const net = require('net');
const Discord = require('discord.js');
const config = require("./config.json");

const client = new Discord.Client();
let sockets = [];

net.createServer(socket => {
	socket.setEncoding("utf8"); // parse buffer to utf8
	socket.on('data', async (msg) => {
		console.log('message: ', msg); // log messages?
		var parse_msg = msg;
		if (msg.indexOf('<') === 0) {
			var ign = msg.match(/\<(.*?)\>/);
			if (ign) {
				var ign_r = ign[1];
				console.log('ign: ', ign_r);
				await client.guilds.get(config.server).members.get(client.user.id).setNickname(ign_r).then(() => { // ensure client.login() was successfully called else channels.get will return undefined
					console.log('chang nick succes!'); // log messages?
				}).catch(err => {
					console.log(err); // handle error
				});
				parse_msg = parse_msg.replace(ign_r, '');
				parse_msg = parse_msg.replace('<>: ', '');
				console.log('message: ', parse_msg);
			}
		}
		await client.channels.get(config.channel).send(parse_msg).then(() => { // ensure client.login() was successfully called else channels.get will return undefined
			console.log('Message sent!'); // log messages?
			client.guilds.get(config.server).members.get(client.user.id).setNickname(client.user.username).then(() => { // ensure client.login() was successfully called else channels.get will return undefined
				console.log('revert chang nick succes!'); // log messages?
			}).catch(err => {
				console.log(err); // handle error
			});
		}).catch(err => {
			console.log(err); // handle error
		});


	})
	sockets.push(socket);
}).listen(1337);

client.on('message', msg => {
	if (msg.channel.name == 'general' && !msg.author.bot) { // filter for text messages only
		// filter only supported characters for herc (strip emojis or replace them for a nice ascii version)
		// <Haru> That's because the client expects text in a local charset (according to the windows legacy locale settings)
		// <Haru> it's reading the UTF8 stream it receives as latin1 or w/e charset is configured on that one specific computer
		console.log(msg.author.username,': ', msg.content); // also then authors username

		//console.log(sockets[0]);
		if (sockets[0] != undefined)
			sockets[0].write(`<${msg.author.username}> : ${msg.content}\0`); // check if socket exists and is hercules?
	};
});

client.login(config.token);