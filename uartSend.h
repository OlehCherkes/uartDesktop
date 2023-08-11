#pragma once
#include <boost/asio.hpp>
#include "qln_cmds.h"

void uartGet();
void asioConfig();
void asioTimer();
void callbackFunction(const boost::system::error_code& ec, boost::asio::steady_timer* timer);
void uartSet(const void* data, uint8_t nBytes);

extern const int i_;
enum Command
{
  GREEN_LED_ON = 0,
  GREEN_LED_OFF,
  RED_LED_ON,
  RED_LED_OFF,
  BLINK_LED
};

enum class OsdpTempCtrlCode : uint8_t
{
  NOP,
  SET_IMMEDIATELY,
  SET,
};

enum class OsdpPerCtrlCode : uint8_t
{
  NOP,
  SET,
};

enum class OsdpColor : uint8_t
{
  OFF,
  RED,
  GREEN,
  AMBER,
  BLUE
};

enum class WiegandNumber : uint8_t
{
  FIRST,
  SECOND
};

struct OsdpProto
{
  uint8_t readerNumber;		            //	[Reader Number: 1]
  uint8_t	ledNumber;			            //	[LED Number: 1]

  struct
  {
    OsdpTempCtrlCode controlCode;     //	[Control Code: 1]
    uint8_t onTime;                   //	[ON time: 1]
    uint8_t offTime;			            //	[OFF color: 1]	
    OsdpColor onColor;			          //	[ON color: 1]
    OsdpColor offColor;               //	[OFF color: 1]
    uint8_t timerLSB;			            //	[Timer LSB: 1]
    uint8_t timerMSB;			            //	[Timer MSB: 1]
  } Temporary;

  struct {
    OsdpPerCtrlCode controlCode;		  //	[Control Code: 1]
    uint8_t onTime;		                //	[ON time: 1]
    uint8_t offTime;			            //	[OFF time: 1]
    OsdpColor onColor;			          //	[ON color: 1]
    OsdpColor offColor;		            //	[OFF color: 1]
  } Permanet;
};
