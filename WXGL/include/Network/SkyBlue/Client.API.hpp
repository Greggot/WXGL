#pragma once
#include "Types.hpp"

#include <vector>
#include <functional>

#include <TCP/client.hpp>

namespace SkyBlue
{
	class Device;
	/**
	* @brief Robot's part communication protocol 
	*	without connection to lower lever transport protocol (e.g. TCP, UDP, USB...)
	* 
	* @member read/write are callbacks, because there are several Modules at one end
	*	Thus, it's 'server' job to select appropriate module to retransmit data to
	*/
	class Module
	{
	public:
		using requestHandler = std::function<void(const void*, unsigned int)>;
		Module() = default;

		// Application polymorphism
		void setRead(requestHandler call) { read = call; }
		void setWrite(requestHandler call) { write = call; }
	private:
		friend class Device;
		requestHandler read, write;
	};
}

#include <map>
#include <thread>
namespace SkyBlue
{
	// Invariant - size is not limited by buffer size
	// TODO: Add templates for buffer and client
	class Collector
	{
	protected:
		buffer rx, tx;
		TCP::client client;
	public:
		void disconnect();
		void connect(void* connectdata);

		void receive();
		void transmit(const void* data, size_t size);
	};
}

namespace SkyBlue
{
	class Device : public Collector
	{
	private:
		std::map<ID, Module*> modules;
		bool islistening = false;
		std::thread main;
		
		void Report();
		void Execute();
	public:
		Device() = default;
		
		// Process receive
		void listen();
		void deaf();
		bool doeslisten() const;

		// Process transmit
		std::vector<ID> report();
		void write(ID, const void* data, unsigned int length);
		void read(ID, const void* request, unsigned int length);

		// process system of modules
		Module* get(ID);
		void add(ID, Module*);
		void remove(ID id) { modules.erase(id); }
		void clear();
	};
}
