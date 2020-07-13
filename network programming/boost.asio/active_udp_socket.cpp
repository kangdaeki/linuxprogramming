#include <boost/asio.hpp>
#include <iostream>

int main(int argc, char *argv[], char *env[])
{
  boost::asio::io_service ios;
  boost::asio::ip::udp protocol = boost::asio::ip::udp::v6();
  boost::asio::ip::udp::socket sock(ios);
  boost::system::error_code ec;
  sock.open(protocol, ec);
  if (0!=ec.value())
  {
    std::cerr << "Faile to open the socket! " << "Error code = " << ec.value() << ". " << "Message : " << ec.message();
    return ec.value();
  }
  return 0;
}
