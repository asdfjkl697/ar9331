#include "packet.h"
#include <string.h>

int16_t packet_readInt16(uint8_t *data)
{
	return (data[0] & 0xFF) | (data[1] << 8);
}

int32_t packet_readInt32(uint8_t *data)
{
	return ((data[0] & 0xFF) | (data[1] << 8) | (data[2] << 16) | (data[3] << 24));
}

int32_t packet_data2Int(uint8_t *data)
{
	return ((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | (data[3] & 0xFF));
}

uint8_t *packet_writeInt16(uint8_t *data_ptr, uint16_t value)
{
	data_ptr[0] = value & 0xFF;
	data_ptr[1] = value >> 8;
	return data_ptr + 2;
}

uint8_t *packet_writeInt32(uint8_t *data_ptr, uint32_t value)
{
	data_ptr[0] = value & 0xFF;
	data_ptr[1] = (value >> 8);
	data_ptr[2] = (value >> 16);
	data_ptr[3] = (value >> 24);
	return data_ptr + 4;
}

uint8_t *packet_int2Data(uint8_t *data_ptr, uint32_t value)
{
	data_ptr[0] = (value >> 24);
	data_ptr[1] = (value >> 16);
	data_ptr[2] = (value >> 8);
	data_ptr[3] = value & 0xFF;
	return data_ptr + 4;
}

uint8_t *packet_encodeInt16(uint8_t *data_ptr, uint16_t value)
{
	*data_ptr++ = DATA_INT16;
	data_ptr = packet_writeInt16(data_ptr, value);
	return data_ptr;
}

uint8_t *packet_encodeInt32(uint8_t *data_ptr, uint32_t value)
{
	*data_ptr++ = DATA_INT32;
	data_ptr = packet_writeInt32(data_ptr, value);
	return data_ptr;
}

uint8_t *packet_encodeBoolean(uint8_t *data_ptr, uint8_t value)
{
	data_ptr[0] = DATA_BOOLEAN;
	data_ptr[1] = value;
	return data_ptr+2;
}

uint8_t *packet_encodeString(uint8_t *data_ptr, const char *value)
{
	uint16_t len = strlen(value);
	*data_ptr++ = DATA_STRING;
	data_ptr = packet_writeInt16(data_ptr, len);
	memcpy(data_ptr, value, len);

	return data_ptr+len;
}

uint8_t *packet_decodeByte(uint8_t *data_ptr, uint8_t *value)
{
	if(data_ptr[0] == DATA_BYTE){
		uint8_t val = data_ptr[1];
		*value = val;
		return data_ptr+2;
	}
	
	return data_ptr;
}

uint8_t *packet_decodeInt16(uint8_t *data_ptr, uint16_t *value)
{
	if(data_ptr[0] == DATA_INT16){
	   data_ptr++;
	   *value = packet_readInt16(data_ptr);
	   data_ptr+=2;
	}
	return data_ptr;
}

uint8_t *packet_decodeInt32(uint8_t *data_ptr, uint32_t *value)
{
	if(data_ptr[0] == DATA_INT32){
	   data_ptr++;
	   *value = packet_readInt32(data_ptr);
	   data_ptr+=4;
	}
	return data_ptr;
}

uint8_t *packet_decodeBoolean(uint8_t *data_ptr, uint8_t *value)
{
	if(data_ptr[0] == DATA_BOOLEAN){
	   *value = data_ptr[1]== 0x1;
	}
	return data_ptr+2;
}

uint8_t *packet_decodeString(uint8_t *data_ptr, str_st *value)
{
	if(data_ptr[0] == DATA_STRING){
		uint16_t len = packet_readInt16(data_ptr+1);
		data_ptr+=3;
		
        value->len = len;
	    value->str = data_ptr;

		return data_ptr+len;
	}

	return data_ptr;
}

int packet_decode(packet *pkt, uint8_t *data, int size)
{	
	if(size < 18) return 0;
	
	pkt->flag = packet_data2Int(data);
	data+=4;	
	pkt->version = packet_readInt32(data);
	data+=4;

	if(pkt->flag != PROTOCOL_FLAG) return 0;
	
	pkt->id = packet_readInt32(data);
	data+=4;
	
	pkt->command = *data++;
	pkt->ack = *data++; 
	pkt->length = packet_readInt32(data);
	data+=4;
	pkt->data = data;
	
	return pkt->length + 18;
}

int packet_readBuffer(packet *pkt, uint8_t *data)
{	
	data = packet_int2Data(data, PROTOCOL_FLAG);
	data = packet_writeInt32(data, PROTOCOL_VERSION);
	data = packet_writeInt32(data, pkt->id);
	*data++= pkt->command;
	*data++= pkt->ack;
	data = packet_writeInt32(data, pkt->length);

	if(pkt->length > 0){
	   memcpy(data, pkt->data, pkt->length);
	}
	
	return pkt->length+18; 
}

void packet_init_serial(serial_packet *pkt)
{
	memset(pkt, 0, sizeof(serial_packet));
}

void packet_free_serial(serial_packet *pkt)
{
	//nothing
}

uint8_t *packet_to_hardware(uint8_t *buffer, uint8_t *data, uint8_t size)
{
	uint8_t *enc = buffer;

	*enc++= 0xBC;
	*enc++= 0xAC;
	*enc++= size;
	memcpy(enc, data, size);
	enc+= size;
	*enc++= 0xAC;

	return enc;
}

uint8_t *packet_setfreq(uint8_t *buffer, uint8_t module, uint32_t freq)
{
	uint8_t data[8];
	uint8_t *enc = data;

	*enc++= 0x65; 
	*enc++= SYSTEM_SET; 
	*enc++= module; //module
	*enc++= SYSTEM_WIRTE;
	enc = packet_writeInt32(enc, freq);
	
	return packet_to_hardware(buffer, data, enc - data);
}
