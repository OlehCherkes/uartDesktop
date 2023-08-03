#include <iostream>
#include <boost/asio.hpp>
#include <chrono>
#include <thread>
#include <cstdint>
#include <array>
#include "uartSend.h"

boost::asio::io_service m_ioService;
boost::asio::serial_port m_port(m_ioService);
boost::system::error_code ec;

int main()
{
	m_port.open("COM4", ec);

	if (ec)
	{
		std::cout << "Port open error: " << ec.message() << std::endl;
		return 0;
	}

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

void uartSet()
{
	char ch;
	uint8_t buffTX[4];

	while (true)
	{
		std::cout << "Press the button: ";
		std::cin >> ch;
		std::cout << "Pressed: " << ch << std::endl;

		if (ch == 'q' || ch == 'Q') break;

		switch (ch)
		{
		case 'w':
		case 'W':
			buffTX[0] = '@';
			buffTX[1] = '1';
			buffTX[2] = '0' + (QLN_GREEN_LED_ON >> 4);
			buffTX[3] = '0' + (QLN_GREEN_LED_ON & 0x0f);
			break;

		case 'e':
		case 'E':
			buffTX[0] = '@';
			buffTX[1] = '1';
			buffTX[2] = '0' + (QLN_GREEN_LED_OFF >> 4);
			buffTX[3] = '0' + (QLN_GREEN_LED_OFF & 0x0f);
			break;

		case 'r':
		case 'R':
			buffTX[0] = '@';
			buffTX[1] = '1';
			buffTX[2] = '0' + (QLN_RED_LED_ON >> 4);
			buffTX[3] = '0' + (QLN_RED_LED_ON & 0x0f);
			break;

		case 't':
		case 'T':
			buffTX[0] = '@';
			buffTX[1] = '1';
			buffTX[2] = '0' + (QLN_RED_LED_OFF >> 4);
			buffTX[3] = '0' + (QLN_RED_LED_OFF & 0x0f);
			break;

		case 'y':
		case 'Y':
			buffTX[0] = '@';
			buffTX[1] = '1';
			buffTX[2] = BLINK_LED;
			break;

		default:
			break;
}

		boost::asio::write(m_port, boost::asio::buffer(&buffTX, 4), ec);

		if (ec)
		{
			std::cout << "Port write error: " << ec.message() << std::endl;
			break;
		}
	}
}

void osdpUartSendPacket()
{
	//std::array<osdpComand_t, 2> ?
	osdpComand_t osdp[] = {
		{0, 0, 0, 100, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 1, 0, 100, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};

	while (true)
	{
		boost::asio::write(m_port, boost::asio::buffer(&osdp[0], 14), ec);
		std::cout << "Command sent" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void callbackFunction(const boost::system::error_code& ec, boost::asio::steady_timer* timer)
{
	if (!ec) {
		std::cout << "Start timer!" << std::endl;

		timer->expires_after(boost::asio::chrono::microseconds(1));
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
	boost::asio::steady_timer timer(ioContext, boost::asio::chrono::microseconds(1));

	timer.async_wait(std::bind(callbackFunction, std::placeholders::_1, &timer));
	ioContext.run();
}