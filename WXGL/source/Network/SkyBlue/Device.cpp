#include <SkyBlue/Client.API.hpp>
using namespace SkyBlue;

void Device::listen() {
	islistening = true;
	main = std::thread([this] {
		while (islistening)
		{
			receive();
			if (rx.command == command_t::report)
				Report();
			else
				Execute();
		}
	});
}

void Device::deaf() {
	islistening = false;
	main.join();
}

bool Device::doeslisten() const {
	return islistening;
}

std::vector<ID> Device::report() 
{
	static std::vector<ID> idvector;
	if (islistening)
		return idvector;

	tx.id = 0;
	transmit(nullptr, 0);
	receive();

	idvector.clear();
	ID* ids = (ID*)rx.data;
	rx.length /= sizeof(ID);
	while (rx.length--)
		idvector.push_back(*ids++);

	return idvector;
}

void Device::write(ID id, const void* data, unsigned int length) 
{
	tx.id = id;
	tx.command = command_t::write;
	transmit(data, length);
}

void Device::read(ID id, const void* request, unsigned int length) 
{
	tx.id = id;
	tx.command = command_t::read;
	transmit(request, length);
}

void Device::add(ID id, Module* mod) {
	auto res = modules.find(id);
	if (res == modules.end())
		modules.insert({ id, mod });
	else
		res->second = mod;
}

void Device::clear() {
	modules.clear();
}


void Device::Report() {
	std::vector<ID> ids;
	for (auto mod : modules)
		ids.push_back(mod.first);
	transmit(&ids[0], sizeof(ID) * ids.size());
}

void Device::Execute()
{
	auto res = modules.find(rx.id);
	if (res == modules.end())
		return;

	const auto& mod = *res->second;
	if (rx.command == command_t::read && mod.read)
		mod.read(rx.data, rx.length);
	else
		if(mod.write)
			mod.write(rx.data, rx.length);
}

Module* Device::get(ID id)
{
	auto res = modules.find(rx.id);
	return res == modules.end() ? nullptr : res->second;
}
