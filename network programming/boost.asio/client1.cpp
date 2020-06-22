// One shot echo client using boost.asio
// sudo apt-get install libboost-all-dev
// g++ client1.cpp -o client1 -lboost_system -lboost_thread
#include <iostream>
#include <cstdlib>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

const int BUF_SIZE=1024;

int main(int argc, char *argv[])
{
	if (3!=argc) {
		std::cout << "USAGE: " << argv[0] << " <server ip> <port>" << std::endl;
		std::cout << "Note: <server ip> for localhost should be 127.0.0.1, not 127.1" << std::endl;
		return 1;
	}
	boost::asio::io_service io_service;
	boost::asio::ip::tcp::socket socket(io_service);
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(argv[1]), atoi(argv[2]));
	boost::system::error_code c_error;
	socket.connect(endpoint,c_error);
	if (c_error) {
		if (boost::asio::error::eof==c_error) {
			std::cout << "Disconnected" << std::endl;
			return 0;
		}
		else {
			std::cerr << "Error no: " << c_error.value() << ", error message: " << c_error.message() << std::endl;
			return 1;
		}
	}
	std::cout << "Connected to the server." << std::endl;

	for (int i=0;i<7;i++) {
		char s_buf[BUF_SIZE]={};
		snprintf(s_buf, BUF_SIZE-1, "Sending %d.", i);
		int len_buf=strnlen(s_buf,BUF_SIZE-1);
		boost::system::error_code s_error;
		socket.write_some(boost::asio::buffer(s_buf, len_buf),s_error);
		std::cout << "Sent message: " << s_buf << std::endl;

		char r_buf[BUF_SIZE]={};
		boost::system::error_code r_error;
		socket.read_some(boost::asio::buffer(r_buf),r_error);
		if (r_error)
			if (boost::asio::error::eof==r_error) std::cout << "Disconnected" << std::endl;
			else std::cerr << "Error no: " << r_error.value() << ", error message: " << r_error.message() << std::endl;
		std::cout << "From the server: " << r_buf << std::endl;
	}
	if (socket.is_open()) socket.close();

	std::cout << "Client terminated." << std::endl;
	return 0;
}

