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
		virtual void write(ID, const void*, unsigned int) = 0;
		virtual void read(callback) = 0;
		virtual void askread(ID, const void*, unsigned int, callback) = 0;

		virtual bool isConnected() const { return false; }
	};

	class TCPclientAPI : public clientAPI
	{
	private:
		buffer rx, tx;
		TCP::client client;

		void send(ID id, command com, const void* data = nullptr, unsigned int size = 0);
		bool connected = false;
	public:
		TCPclientAPI() = default;

		void connect(const TCP::Address& address);
		std::vector<ID> report() override;

		void write(ID, const void* data, unsigned int size) override;
		void read(callback) override;
		void askread(ID, const void* request, unsigned int size, callback) override;

		bool isConnected() const override;
	};
}