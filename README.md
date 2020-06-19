# file management

print_inodes.c -- print inode

softlink.c -- check if it is a soft link

hardlink.c -- check if it is a hard link

tac.c -- print from the last line to the first line

rev.c -- print from the right to the left

# directory management

ls_sort_size.c -- ls sorted by block size

print_subdir.c -- print subdirectories

# process management

proc_pipe1.c -- communication with pipe

proc_pipe2.c -- communication with pipe with exec at the child

power_sum.c -- x^y + sum(x to y)

myshell.c -- a little and simple shell program

pst.c -- process tree

# signal

signal.c -- basic signal processing using signal() (singnal() is obsolete. Use sigaction().)

sigaction.c -- basic signal processing using sigaction()

# network programming

client1.c and server1.c -- one shot server

echo_client.c and echo_server.c -- echo server

echo_mpclient.c and echo_mpserver.c -- multiple processes

chat_client.c and chat_server.c -- multi-threading

echo_selectserv.c -- multiplexing with select (use echo_client.c for communication)

echo_epollserv.c -- multiplexing with epoll (use echo_client.c for communication)
