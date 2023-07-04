
#	📖 IRC

##	Welcome To
	
##	Interdimensional Rickroll Conspiracy

Text-based communication protocol written in c++

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

Updating branches to master if they are behind, use this 2 command on the relevant branch:
```
git fetch
git merge master
```
