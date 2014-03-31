//Serial Utilities Source
//	Created By:		Mike Moss
//	Modified On:	06/23/2013

//Definitions for "serial_util.hpp"
#include "serial_util.hpp"

//File Utility Header
#include "file_util.hpp"

//Serial Header
#include "serial.hpp"

//String Utility Header
#include "string_util.hpp"

//Linux Dependencies
#if(!defined(_WIN32)||defined(__CYGWIN__))

	//Linux Serial Port Types (Copyright 2010, Paul Stoffregen(paul@pjrc.com))
	static const unsigned int linux_port_types_size=52;
	static const char* linux_port_types[linux_port_types_size]=
	{
		"S",	// "normal" SerialPort Ports - MANY drivers using this
		"USB",	// USB to serial converters
		"ACM",	// USB serial modem, CDC class, Abstract Control Model
		"MI",	// MOXA Smartio/Industio family multiport serial... nice card, I have one :-)
		"MX",	// MOXA Intellio family multiport serial
		"C",	// Cyclades async multiport, no longer available, but I have an old ISA one! :-)
		"D",	// Digiboard (still in 2.6 but no longer supported), new Moschip MCS9901
		"P",	// Hayes ESP serial cards (obsolete)
		"M",	// PAM Software's multimodem & Multitech ISI-Cards
		"E",	// Stallion intelligent multiport (no longer made)
		"L",	// RISCom/8 multiport serial
		"W",	// specialix IO8+ multiport serial
		"X",	// Specialix SX series cards, also SI & XIO series
		"SR",	// Specialix RIO serial card 257+
		"n",	// Digi International Neo (yes lowercase 'n', drivers/serial/jsm/jsm_driver.c)
		"FB",	// serial port on the 21285 StrongArm-110 core logic chip
		"AM",	// ARM AMBA-type serial ports (no DTR/RTS)
		"AMA",	// ARM AMBA-type serial ports (no DTR/RTS)
		"AT",	// Atmel AT91 / AT32 SerialPort ports
		"BF",	// Blackfin 5xx serial ports (Analog Devices embedded DSP chips)
		"CL",	// CLPS711x serial ports (ARM processor)
		"A",	// ICOM SerialPort
		"SMX",	// Motorola IMX serial ports
		"SOIC",	// ioc3 serial
		"IOC",	// ioc4 serial
		"PSC",	// Freescale MPC52xx PSCs configured as UARTs
		"MM",	// MPSC (UART mode) on Marvell GT64240, GT64260, MV64340...
		"B",	// Mux console found in some PA-RISC servers
		"NX",	// NetX serial port
		"PZ",	// PowerMac Z85c30 based ESCC cell found in the "macio" ASIC
		"SAC",	// Samsung S3C24XX onboard UARTs
		"SA",	// SA11x0 serial ports
		"AM",	// KS8695 serial ports & Sharp LH7A40X embedded serial ports
		"TX",	// TX3927/TX4927/TX4925/TX4938 internal SIO controller
		"SC",	// Hitachi SuperH on-chip serial module
		"SG",	// C-Brick SerialPort Port (and console) SGI Altix machines
		"HV",	// SUN4V hypervisor console
		"UL",	// Xilinx uartlite serial controller
		"VR",	// NEC VR4100 series SerialPort Interface Unit
		"CPM",	// CPM (SCC/SMC) serial ports; core driver
		"Y",	// Amiga A2232 board
		"SL",	// Microgate SyncLink ISA and PCI high speed multiprotocol serial
		"SLG",	// Microgate SyncLink GT (might be sync HDLC only?)
		"SLM",	// Microgate SyncLink Multiport high speed multiprotocol serial
		"CH",	// Chase Research AT/PCI-Fast serial card
		"F",	// Computone IntelliPort serial card
		"H",	// Chase serial card
		"I",	// virtual modems
		"R",	// Comtrol RocketPort
		"SI",	// SmartIO serial card
		"T",	// Technology Concepts serial card
		"V",	// Comtrol VS-1000 serial controller
	};

#endif

//List Serial Ports Function (Returns a list of valid serial ports currently connected to the system)
std::vector<std::string> msl::list_serial_ports()
{
	//Return List Variable
	std::vector<std::string> list;

	//Windows
	#if(defined(_WIN32)&&!defined(__CYGWIN__))

		//Com Ports on Windows Range From 1-256
		for(unsigned int ii=1;ii<=256;++ii)
		{
			//Try to Connect to Port
			msl::serial port("COM"+msl::to_string(ii));
			port.connect();

			//If Connected Close Port and Add to List
			if(port.good())
			{
				port.close();
				list.push_back("COM"+msl::to_string(ii));
			}
		}

	//Unix
	#else

		//Get The File Listing for /dev (not the best way to do this...but it works...)
		std::vector<std::string> possible_ports=msl::list_directory("/dev");

		//Go Through Possible Ports
		for(unsigned int ii=0;ii<possible_ports.size();++ii)
		{
			//If Valid Port
			for(unsigned int jj=0;jj<linux_port_types_size;++jj)
			{
				//Add Port to Listing
				if(msl::starts_with(possible_ports[ii],std::string("tty")+linux_port_types[jj]))
				{
					//Try to Connect to Port
					msl::serial port("/dev/"+possible_ports[ii]);

					//Check if Port is in Use
					if(!port.good())
					{
						port.connect();

						//If Connected Close Port and Add to List
						if(port.good())
						{
							port.close();
							list.push_back("/dev/"+possible_ports[ii]);
						}
					}

					//Either Way Break
					break;
				}
			}
		}

	#endif

	//Return Valid Serial Ports
	return list;
}