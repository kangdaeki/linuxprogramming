// g++ client_endpoint.cpp -o client_endpoint -lboost_system -lboost_thread
#include <boost/asio.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 1111;

	boost::system::error_code ec;
	boost::asio::ip::address ip_address=boost::asio::ip::address::from_string(raw_ip_address,ec);

	if (0!=ec.value()) {
		std::cerr << "Fail to parse the IP address. Error code=" << ec.value() << ". Message:" << ec.message();
		return ec.value();
	}

	boost::asio::ip::tcp::endpoint ep(ip_address, port_num);

	return 0;
}

