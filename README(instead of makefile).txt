Secure Coding Project README and man page


NAME:  Haotian Song

I am not using makefile because I do not know how to use makefile to run 2 terminal.

For my project, the command line will be:
gcc echo-server.c -o echo-server
./echo-server 7
gcc echo-client.c -o echo-client
./echo-client

after the connection using 2 terminals, all the input message will be sent back.
7 is the port number for echo.

gcc daytime-server.c -o daytime-server
./daytime-server 13
gcc daytime-client.c -o daytime-client
./daytime-client

after the connection using 2 terminals, the server will automatically send the time
and timezone to the client, and the connect will be aborted.
13 is the port number for daytime.
