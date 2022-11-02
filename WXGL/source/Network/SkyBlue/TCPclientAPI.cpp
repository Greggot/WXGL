#include <SkyBlue/Client.API.hpp>
using namespace SkyBlue;

void TCPclientAPI::connect(const TCP::Address& address)
{
	client.Connect(address);
	connected = true;
}

std::vector<ID> TCPclientAPI::report() 
{
	send(0, command::report);

	int length = client.Receive(&rx, sizeof(rx));
	ID* ids = (ID*)rx.data;
	std::vector<ID> idvector;

	rx.length /= sizeof(ID);
	while (rx.length--)
		idvector.push_back(*ids++);
	return idvector;
}

void TCPclientAPI::write(ID id, const void* data, unsigned int size) {
	send(id, command::write, data, size);
}

void TCPclientAPI::read(callback call) {
	if (client.Receive(&rx, sizeof(rx)) > 0)
		call(rx);
}
void TCPclientAPI::askread(ID id, const void* request, unsigned int size, callback call)
{
	send(id, command::read, request, size);
	read(call);
}

bool TCPclientAPI::isConnected() const  {
	return connected;
}

void TCPclientAPI::send(ID id, command com, const void* data, unsigned int size)
{
	tx.id = id;
	tx.service = com;
	if (data)
	{
		memcpy(tx.data, data, tx.length);
		tx.length = size;
	}
	else
		tx.length = 0;
	client.Send(&tx, tx.length + BUFFER_HEADER_SiZE());
}