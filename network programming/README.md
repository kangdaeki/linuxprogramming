# network programming

client1.c and server1.c -- one shot server

echo_client.c and echo_server.c -- echo server

echo_mpclient.c and echo_mpserver.c -- multiple processes

chat_client.c and chat_server.c -- multi-threading

echo_selectserv.c -- multiplexing with select (use echo_client.c for communication) (select() is Level Trigger)

echo_epollserv.c -- multiplexing with epoll (use echo_client.c for communication) (Level Trigger)

echo_EPLTserv.c -- multiplexing with epoll (use echo_client.c for communication) (Level Trigger with small buffer to see the behavior)

echo_EPETserv.c -- multiplexing with epoll (use echo_client.c for communication) (Edge Trigger with small buffer to see the behavior)
