#ifndef _DATA_HANDLER_H
#define _DATA_HANDLER_H

#include <stdint.h>

class DiscoveryHandler
{
	public:
		virtual void onDiscovery(uint8_t code) = 0;
		virtual void onDiscoveryChild(uint8_t code, uint16_t rid, uint8_t level) = 0;
};

#endif
