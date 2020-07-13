// g++ acceptor_socket.cpp -o acceptor_socket -lboost_system -lboost_thread
#include <boost/asio.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
  boost::asio::io_service ios;
  boost::asio::ip::tcp protocol = boost::asio::ip::tcp::v6();
  boost::asio::ip::tcp::acceptor acceptor(ios);
  boost::system::error_code ec;
  if (0!=ec.value())
  {
    std::cerr << "Failed to open the socket! " << "Error code = " << ec.value() << ". " << "Message : " << ec.message();
    return ec.value();
  }
  return 0;
}
