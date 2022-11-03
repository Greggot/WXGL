#include <SkyBlue/Client.API.hpp>
using namespace SkyBlue;

void Collector::disconnect()  {
	client.Disconnect();
}

void Collector::connect(void* connectdata) {
	client.Connect(*(TCP::Address*)connectdata);
}

void Collector::receive() {
	client.Receive(&rx, sizeof(rx));
	// TODO: add part-by-part receive
}

void Collector::transmit(const void* data, size_t size)
{
	// TODO: add part-by-part send
	if (data)
	{
		memcpy(tx.data, data, tx.length);
		tx.length = size;
	}
	else
		tx.length = 0;
	client.Send(&tx, tx.length + BUFFER_HEADER_SiZE());
}