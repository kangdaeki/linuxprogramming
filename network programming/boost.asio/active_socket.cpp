// g++ active_socket.cpp -o active_socket -lboost_system -lboost_thread
#include <boost/asio.hpp>
#include <iostream>

int main(int argc, char* argv[], char* env[])
{
  boost::asio::io_service ios;
  boost::asio::ip::tcp protocol = boost::asio::ip::tcp::v4();
  boost::asio::ip::tcp::socket sock(ios);
  boost::system::error_code ec;
  sock.open(protocol, ec);
  if (0!=ec.value())
  {
    std::cout << "Failed to open the socket! " << "Error code = " << ec.value() << ". " << "Message : " << ec.message();
    return ec.value();
  }
  return 0;
}
