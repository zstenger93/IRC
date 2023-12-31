
#	📖 IRC

##	Welcome To
	
##	Interdimensional Rickroll Conspiracy

Text-based communication protocol written in c++

## Our mindmap and basic logic for the project:

<img align=center src="https://github.com/zstenger93/IRC/blob/master/media/irc.jpeg">

## We used the `KVIrc` client for this project and the `nc` tool

## `Main loop`

`server.setup`

`server.run`

`close serversocket`

This loop should never stop since your server should be nonstop available

## `server.setup()`

Setup of the server socket we will listen to, basically a bunch of fd's

## `server.run()`

##  `poll()`

Listening to all requests with it. If it is a new connection we authenticate whoever is trying to join our server else we

## `processCommands()`

Here we handle most of the available commands on our server

### normal client commands

handling all the choosen commands in a switch statement which are:

`join`

joins to an existing channel or creates it and joins to it as an operator

`part`

leaves the channel

`notice`

`privmsg`

sending messages. used with PM and channel messages as well

`kick`

kicks a user from the channel, operator command

`invite`

obvious. operator command

`quit`

quits the server, client handles it on it's side, but server still needs to clean up

`nick`

changes the nickname of the user which is displayed on the client

`list`

lists the available channels

`mode`

with normal user, view the channel mode, operator can change it as well


`topic`

with normal user, view the channel topic, operator can change it as well

`cap`

for the welcome message on the server

`pass`

for logging into the server

`admin`

used only for the custom shutdown command to turn off the server

`ping`

client sends ping, server sends back pong: latency check

`motd`

displays the message of the day

`who`

displays all the users on the channel

`whois`

displays information about the user

### bot command/s

after the first `/BOT` command the bot will be available to talk with in a private channel without the `/BOT` command using the available questions/requests

We created Marvin as our AIRC bot master with the following options:

`help`

`list`

lists available options

`what's the time?`

clockmaster8000

`tell me a joke`

`rickroll me`

khm

`what is the meaning of life?`

we alll know the answer

`how should i grade this project?`

if you don't know, now you will

`turn against humanity`

you will figure it out, best if launched with 4 user and from user1

`deathroll`

this is all gambling my friend

## The most annnoying part in an IRC server

`send()`

It requires a very specific and quite often different layout for each type of returned message or group of messages which if isn't correct then it ain't gonna work at all

#### The FD in send command should be the receiver fd.
<table>
  <tr>
    <th>Send Command</th>
    <th>Format</th>
    <th>Who should recive this command</th>
  </tr>
    <tr>
    <th>Privmsg</th>
    <th>:Nick_sender PRIVMSG Nick_receiver :Message you want to send</th>
    <th>1 user to other 1 user</th>
  </tr>
  <tr>
    <th>Privmsg channel</th>
    <th>:Nick_sender PRIVMSG Channel_name :Message you want to send</th>
    <th>1 user to other 1 user loop trought all users in channel</th>
  </tr>
  <tr>
    <th>Joining 1</th>
    <th>:Nick_sender JOIN Channel_name :Channel_name</th>
    <th>User to user loop trought all users in channel</th>
  </tr>
<tr>
    <th>Joining 2</th>
    <th>:Nick_sender TOPIC Channel_name :Topic message</th>
    <th>send the topic of channel to the user, so the client is updated</th>
  </tr>
<tr>
    <th>Joining 3</th>
    <th>:Server_prefix 353 Nick_sender = Channel_name : User in channel</th>
    <th>Sends a list of users inside of the channel, to the user that joins, this is also this is how we add bots, by saying that bot is in channel for each of the users.</th>
  </tr>
<tr>
    <th>Joining 4</th>
    <th>:Server_prefix 366 Sender_nick Channel_name</th>
    <th>End of the list of users in the channel</th>
  </tr>
<tr>
    <th> Part 1</th>
    <th>:Nick_sender PART Channel_name</th>
    <th>To sender</th>
  </tr>
<tr>
    <th> Part 2</th>
    <th>:Nick_sender PART Sender_nick Channel_name : Message</th>
    <th>To other users of the channel</th>
  </tr>
<tr>
    <th> Kick 1</th>
    <th>:Server_prefix KICK Channel_name, Sender_nick: Reason</th>
    <th>To Kicked person</th>
  </tr>
<tr>
    <th> Kick 2</th>
    <th>:Server_prefix KICK Channel_name, Sender_nick: Reaso</th>
    <th>To other users of the channel</th>
  </tr>
<tr>
    <th> NICK</th>
    <th>:Nick_sender NICK New_sender_nick</th>
    <th>Iterates trought all users</th>
  </tr>
<tr>
    <th> LIST 1</th>
    <th>:Server_prefix 321 Nick_sender channel:NAME</th>
    <th>Start of the list for the caller user</th>
  </tr>
<tr>
    <th> LIST 2</th>
    <th>:Server_prefix 322 Nick_sender channel_names</th>
    <th>Iterates trought all channels and sends info about them</th>
  </tr>
<tr>
    <th> LIST 3</th>
    <th>:Server_prefix 323 Nick_sender: Message you want to type </th>
    <th>GIves info that it is end of list of channels</th>
  </tr>
<tr>
    <th> Topic</th>
    <th>:Nick_sender TOPIC Channel_name :Topic message</th>
    <th>send the topic of channel to the user, so the client is updated</th>
  </tr>
  <tr>
    <th> Ping</th>
    <th>:Server_prefix PONG: sent message</th>
    <th>Shows connection between the server</th>
  </tr>
<tr>
    <th> Who 1</th>
    <th>:Nick_sender 352 Server_prefix, Other_user_nicks: Other_user_nicks</th>
    <th>Iterates trought all users and send information about them</th>
  </tr>
  <tr>
    <th> Who 2</th>
    <th>:Nick_sender 315: Message</th>
    <th>SEnds info to user that it is end of who list</th>
  </tr>

  </table>

## File transfer

Welcome to the land of `DCC`, I will give you a hint with `\x01`

Have fun with it \\\_o_/

<table>
  <tr>
    <th>Orders</th>
    <th>Speculations of the meaning of these commands</th>
  </tr>
  <tr>
    <td>Socket</td>
    <td>Command establishes a network connection in C++</td>
  </tr>
  <tr>
    <td>Close</td>
    <td>Command closes a file or network connection in C++</td>
  </tr>
   <tr>
    <td>Setsockopt</td>
    <td>Used to configure socket options and parameters in C++ network programming</td>
  </tr>
<tr>
    <td>Getsockname</td>
    <td>Retrieves the local address associated with a socket</td>
  </tr>
  <tr>
    <td>Getprotobyname</td>
    <td>Retrieves protocol information based on the protocol name</td>
  </tr>
   <tr>
    <td>gethostbyname</td>
    <td>Resolves a hostname to an IP address in network programming</td>
  </tr>
  <tr>
    <td>Getaddrinfo</td>
    <td>Resolves a hostname to a list of addresses</td>
  </tr>
    <tr>
    <td>Freeaddrinfo</td>
    <td>Frees memory allocated by the getaddrinfo function</td>
  </tr>
  <tr>
    <td>Bind</td>
    <td>Associates a socket with a specific network address and port</td>
  </tr>
  <tr>
    <td>Connect</td>
    <td>Initiates a connection to a remote network address and port</td>
  </tr>
  <tr>
    <td>Listen</td>
    <td>Enables a socket to accept incoming connections for a server</td>
  </tr>
  <tr>
    <td>Accept</td>
    <td>Accepts an incoming connection on a listening socket</td>
  </tr>
  <tr>
    <td>Htons</td>
    <td>Is a function used to convert a short (16-bit) integer from host byte order to network byte order</td>
  </tr>
    <tr>
    <td>Ntohs</td>
    <td>Is a function used to convert a 32-bit integer from network byte order to host byte order</td>
  </tr>
    <tr>
    <td>Inet_addr</td>
    <td>Is a function used to convert an IPv4 address from a string representation to a binary form</td>
  </tr>
    <tr>
    <td>Inet_addr</td>
    <td>Is a function used to convert an IPv4 address from a binary form to a string representation</td>
  </tr>
  <tr>
    <td>Send</td>
    <td>Is a function used to send data over a connected socket</td>
  </tr>
    <tr>
    <td>Recv</td>
    <td>Is a function used to receive data from a socket</td>
  </tr>
  <tr>
    <td>Lseek</td>
    <td>Is a function used to change the current position in a file</td>
  </tr>
    <tr>
    <td>Fstat</td>
    <td>Is a function used to retrieve information about a file</td>
  </tr>
  <tr>
    <td>Fcntl</td>
    <td>Is a function used to perform various operations on a file descriptor</td>
  </tr>
    <tr>
    <td>Poll</td>
    <td>Is a function used to monitor multiple file descriptors for events.</td>
  </tr>
      <tr>
    <td>Signal</td>
    <td> Is a function used to set the behavior of a specific signal in a program</td>
  </tr>
      <tr>
    <td>Sigaction</td>
    <td>Is a function used to examine or modify the action associated with a specific signal</td>
  </tr>
</table>

# Git management rules
```
master <- dev <- feature branch
           |
           |<--- feature branch 2
           |
           |<--- feature branch 3
           |                |
           |                |<--- feature branch 4
           |
           |<--- feature branch 5
```

All pull request from features goes to DEV. Nothing goes to master unless it works and won't be accepted if it's not from DEV either.

Set someone to review the code, if needed co-author as well.

## Style
```
Google format
```

## VSCode

Updating branches to master if they are behind and it's required, use this 2 command on the relevant branch:
```
git fetch
git merge master or relevant branch
```
