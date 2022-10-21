#pragma once
#include "Types.hpp"

#include <vector>
#include <functional>

#include <TCP/client.hpp>

namespace SkyBlue
{
	class clientAPI
	{
	private:
	public:
		using callback = std::function<void(const buffer&)>;

		virtual std::vector<ID> report() = 0;
		virtual void write(ID, const void*, size_t) = 0;
		virtual void read(callback) = 0;
		virtual void askread(ID, const void*, size_t, callback) = 0;
	};

	class TCPclientAPI : public clientAPI
	{
	private:
		buffer rx, tx;
		TCP::client& client;

		void send(ID id, command com, const void* data = nullptr, size_t size = 0) {
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
	public:
		TCPclientAPI(TCP::client& client) 
			: client(client) 
		{

		}

		std::vector<ID> report() override {
			tx.service = command::report;
			tx.length = 0;
			client.Send(&tx, BUFFER_HEADER_SiZE());

			int length = client.Receive(&rx, sizeof(rx));
			ID* ids = (ID*)&rx;
			std::vector<ID> idvector;

			length /= sizeof(ID);
			while (length--)
				idvector.push_back(*ids++);
			return idvector;
		}

		void write(ID id, const void* data, size_t size) override {
			send(id, command::write, data, size);
		}
		void read(callback call) override {
			if (client.Receive(&rx, sizeof(rx)) > 0)
				call(rx);
		}
		void askread(ID id, const void* request, size_t size, callback call) override {
			send(id, command::read, request, size);
			read(call);
		}
	};
}