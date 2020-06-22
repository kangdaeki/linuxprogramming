// One shot echo server using boost.asio
// sudo apt-get install libboost-all-dev
// g++ server1.cpp -o server1 -lboost_system -lboost_thread
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

const int BUF_SIZE=1024;

int main(int argc, char *argv[])
{
	if (2!=argc) {
		std::cout << "USAGE: " << argv[0] << " <port>" << std::endl;
		return 1;
	}
	int port_no = atoi(argv[1]);
	boost::asio::io_service io_service;
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port_no);
	boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint);
	boost::asio::ip::tcp::socket socket(io_service);

	acceptor.accept(socket);
	std::cout << "Client connected" << std::endl;
	for (;;) {
		char buf[BUF_SIZE]={};
		boost::system::error_code error;
		socket.read_some(boost::asio::buffer(buf), error);
		if (error) {
			if (boost::asio::error::eof==error) std::cout << "Disconnected" << std::endl;
			else std::cout << "Error no: " << error.value() << ", error message: " << error.message() << std::endl;
			break;
		}
		std::cout << "From the client: " << buf << std::endl;
		char r_buf[BUF_SIZE]={};
		snprintf(r_buf, BUF_SIZE-1, "RE: %s", buf);
		int len_r_buf=strnlen(r_buf, BUF_SIZE-1);
		boost::system::error_code r_error;
		socket.write_some(boost::asio::buffer(buf,len_r_buf),r_error);
		std::cout << "Sent message: " << r_buf << std::endl;
	}
	return 0;
}
