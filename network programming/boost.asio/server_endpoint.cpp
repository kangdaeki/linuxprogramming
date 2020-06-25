// g++ server_endpoint.cpp -o server_endpoint -lboost_system -lboost_thread
#include <boost/asio.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
	unsigned short port_num = 1111;

	boost::asio::ip::address ip_address=boost::asio::ip::address_v6::any();

	boost::asio::ip::tcp::endpoint ep(ip_address, port_num);

	return 0;
}


