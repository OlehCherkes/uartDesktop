#include <iostream>
#include <conio.h>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/bind.hpp>
#include <chrono>
#include <thread>
#include <cstdint>
#include <array>
#include "uartSend.h"
#include "event/event.h"
#include "event_command.h"
#include "qln.h"

boost::asio::io_service m_ioService;
boost::asio::serial_port m_port(m_ioService);
boost::system::error_code ec;

extern Evt::Event event;

extern OsdpProto osdpPacket;

extern void osdpInit();
extern void testOsdpIndication();

int main()
{
  qlnInit('#', '@');
  qlnSubscribe(QS_WIEGAND, NULL, &uartSet, NULL);

  m_port.open("COM4", ec);

  if (ec)
  {
    std::cout << "Port open error: " << ec.message() << std::endl;
    return 0;
  }

  osdpInit();
  testOsdpIndication();

  asioConfig();
  asioTimer();

  return 0;
} 

void asioConfig()
{
  boost::asio::serial_port_base::baud_rate baud_rate1(115200);
  boost::asio::serial_port_base::character_size c(8);
  boost::asio::serial_port_base::stop_bits s(boost::asio::serial_port_base::stop_bits::one);
  boost::asio::serial_port_base::parity d(boost::asio::serial_port_base::parity::none);
  m_port.set_option(baud_rate1);
  m_port.set_option(c);
  m_port.set_option(s);
  m_port.set_option(d);
}

void uartGet()
{
  while (true)
  {
    char buff[1024];
    auto b = boost::asio::buffer(&buff[0], 1020);
    auto nByets = m_port.read_some(b);
    buff[nByets] = 0;
    std::cout << buff;
  }
}

void uartSet(const void* data, uint8_t nBytes)
{
  boost::asio::write(m_port, boost::asio::buffer(data, nBytes), ec);
#if 1
  char* str = (char*)data;
  for (int i = 0; i < nBytes; i++, str++)
  {
    std::cout << *str;
  }
#endif
}

void uartSetLedState()
{
  /*while (true)
  {
    std::cout << "Press the button: ";

    char ch = _getch();

    std::cout << "\nYou pressed: " << ch << std::endl;

    if (ch == 'q' || ch == 'Q') break;

    if (ch == '?')
    {
      std::cout << "G - GREEN ON" << std::endl;
      std::cout << "g - GREEN OFF" << std::endl;
      std::cout << "R - RED ON" << std::endl;
      std::cout << "r - RED OFF" << std::endl;
      std::cout << "Y - YELLOW ON" << std::endl;
      std::cout << "Y - YELLOW OFF" << std::endl;
      std::cout << "T - GREEN Interval 500/500" << std::endl;
      std::cout << "t - GREEN Interval 100/100" << std::endl;
      std::cout << "U - RED Interval 300/300" << std::endl;
      std::cout << "u - RED Interval 200/200" << std::endl;
      std::cout << "O - YELLOW Interval 500/500" << std::endl;
      std::cout << "o - YELLOW Interval 100/100" << std::endl;
    }

    uint8_t buff[2] = { '@' };
    switch (ch)
    {
    case 'G':
      buff[1] = QLN_GREEN_LED_ON;
      break;

    case 'g':
      buff[1] = QLN_GREEN_LED_OFF;
      break;

    case 'R':
      buff[1] = QLN_RED_LED_ON;
      break;

    case 'r':
      buff[1] = QLN_RED_LED_OFF;
      break;

    case 'T':
      buff[1] = QLN_GREEN_LED_BLINK_1;
      break;

    case 't':
      buff[1] = QLN_GREEN_LED_BLINK_2;
      break;

    case 'Y':
      buff[1] = QLN_RED_LED_BLINK_1;
      break;

    case 'y':
      buff[1] = QLN_RED_LED_BLINK_2;
      break;

    case 'U':
      buff[1] = QLN_YELLOW_LED_BLINK_1;
      break;

    case 'u':
      buff[1] = QLN_YELLOW_LED_BLINK_2;
      break;

    default:
      break;
    }

    boost::asio::write(m_port, boost::asio::buffer(&buff, sizeof(buff)), ec);
  }*/
}

void callbackFunction(const boost::system::error_code& ec, boost::asio::steady_timer* timer)
{
  if (!ec) {

    event.update(10);
    events();

    timer->expires_after(boost::asio::chrono::milliseconds(1));
    timer->async_wait(std::bind(callbackFunction, std::placeholders::_1, timer));
  }
  else
  {
    std::cout << "Timer error: " << ec.message() << std::endl;
  }
}

void asioTimer()
{
  boost::asio::io_context ioContext;
  boost::asio::steady_timer timer(ioContext, boost::asio::chrono::milliseconds(1));

  timer.async_wait(std::bind(callbackFunction, std::placeholders::_1, &timer));
  ioContext.run();
}