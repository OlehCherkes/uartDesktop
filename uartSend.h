#pragma once

enum Command
{
	GREEN_LED_ON = 0,
	GREEN_LED_OFF,
	RED_LED_ON,
	RED_LED_OFF,
	BLINK_LED
};

typedef enum
{
	QLN_SYS_RESTART,
	//
	QLN_BLE_ADVERT_STOP,
	QLN_BLE_ADVERT_UCCESS,
	QLN_BLE_NOTIFIC_CB,
	QLN_BLE_RECEIVE,
	QLN_BLE_OBSERVER,
	//
	QLN_BLE_SET_NAME,
	QLN_BLE_SET_ADV_INT,
	QLN_BLE_SET_POWER,
	QLN_BLE_SET_ADV_EN,
	QLN_BLE_SET_ADV_DATA,
	QLN_BLE_SET_SCAN_DATA,
	QLN_BLE_START_PROFILE,
	QLN_BLE_ADV_UP_SUCCESS,
	QLN_BLE_START_DISCOVERY,
	QLN_BLE_TERMINATE_CONN,
	QLN_BLE_SEND_DATA,
	//
	QLN_ASK_PUMP_STS,
	QLN_ASK_PUMP_ON,
	QLN_ASK_PUMP_OFF,
	QLN_ASK_RESUME,
	//
	QLN_CARD_ID,
	//
	QLN_RED_LED_ON,
	QLN_RED_LED_OFF,
	QLN_GREEN_LED_ON,
	QLN_GREEN_LED_OFF,
	QLN_LED_OSDP_FORMAT,

	QLN_BUZZER,
	//
	QLN_END = 0xff,
} qlnCmd_t;

typedef struct
{
	uint8_t readerNumbe;		//	[Reader Number: 1]
	uint8_t	ledNumber;			//	[LED Number: 1]
	uint8_t controlCodeTem;     //	[Control Code: 1]
	uint8_t onTimeTem;          //	[ON time: 1]
	uint8_t offTimeTem;			//	[OFF color: 1]	
	uint8_t onColorTem;			//	[ON color: 1]
	uint8_t offColorTem;        //	[OFF color: 1]
	uint8_t timerLSB;			//	[Timer LSB: 1]
	uint8_t timerMSB;			//	[Timer MSB: 1]
	uint8_t controlCodePer;		//	[Control Code: 1]
	uint8_t onTimePer;		    //	[ON time: 1]
	uint8_t offTimePer;			//	[OFF time: 1]
	uint8_t onColorPer;			//	[ON color: 1]
	uint8_t offColorPer;		//	[OFF color: 1]
} osdpComand_t;

void uartGet();
void uartSet();
void asioConfig();
void asioTimer();
void osdpUartSendPacket();
void callbackFunction(const boost::system::error_code& ec, boost::asio::steady_timer* timer);
