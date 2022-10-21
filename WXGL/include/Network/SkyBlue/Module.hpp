#pragma once
#include <cstdint>
#include <functional>

#include "Types.hpp"

#include <TCP/client.hpp>


using callback = std::function<void(const void*, size_t)>;
namespace SkyBlue
{
    bool operator< (const ID& r, const ID& l) { return r.raw < l.raw; }

    class Module
    {
    protected:
        const ID identifier;
        callback RX, TX;
    public:
        Module(ID id) : identifier(id) {}

        virtual void write(const void*, size_t) = 0;
        
        const ID& getID() { return identifier; }

        void setTX(callback call) { TX = call; }
        void setRX(callback call) { RX = call; }

        void callTX(const void* data, size_t size) { TX(data, size); }
        void callRX(const void* data, size_t size) { RX(data, size); }
    };

    class TCPclientModule : public Module
    {
    private:
        TCP::client& client;
    public:
        TCPclientModule(ID&& id, TCP::client& client) 
            : Module(id), client(client) {}
        
        void write(const void* data, size_t size) override {
            client.Send(data, size);
        }
    };
}