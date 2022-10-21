#pragma once
#include <map>
#include <initializer_list>
#include <ServiceProtocol.hpp>

#include "Module.hpp"
#include "Types.hpp"

#include <TCP/client.hpp>
#include <TCP/server.hpp>

namespace SkyBlue
{
    class API : public ServiceProtocol<command, buffer>
    {
    private:
        void setupReport() {
            callbacks[report] = [this](const buffer& request) {
                buffoutput(request);

                printf("\nIDS: ");
                std::vector<ID> ids;
                for(auto module : modules) 
                {
                    auto id = module.second->getID();
                    printf("%u(%s) ", id.number, typeToString(id.type));
                    ids.push_back(id);
                }
                printf("\n");
                transmit(report, &ids[0], ids.size() * sizeof(ID));
            };
        }
        void setupWrite(){
            callbacks[write] = [this](const buffer& request){
                buffoutput(request);

                auto res = modules.find(request.id);
                if(res == modules.end())
                    return;
                res->second->callRX(request.data, request.length);
            };
        }

        void setupRead(){
            callbacks[read] = [this](const buffer& request){
                buffoutput(request);

                auto res = modules.find(request.id);
                if(res == modules.end())
                    return;
                res->second->callTX(request.data, request.length);
            };
        }
    protected:
        static void buffoutput(const buffer& request, size_t maxlen = 0) {
            printf("%s from %04X %04X called with size(%u)\n", 
                commandstring(request.service), request.id.number,
                static_cast<int>(request.id.type), request.length);
        }

        std::map<ID, Module*> modules;
        API(std::initializer_list<Module*> modulelist) 
        {
            for(auto module : modulelist)
                modules.insert({module->getID(), module});
            setupReport();
            setupRead();
            setupWrite();
        }
    public:
        void start() {
            xTaskCreate([](void* arg){
                API* api = (API*)arg;
                while(true)
                {
                    if(!api->receive())
                        continue;
                    api->call();
                    api->flush();
                }
                vTaskDelete(nullptr);
            }, "MainAPI", 4096, this, 0, nullptr);
        }
    };
    
    class TCPclientAPI : public API
    {
    private:
        TCP::client& client;
    public:
        TCPclientAPI(std::initializer_list<Module*> modules, TCP::client& client)
            : API(modules), client(client)
        {}

        bool receive() override {
            int length = client.Receive(&rx, sizeof(rx));
            if(length < 0)
                return false;
            buffoutput(rx);
            return true;
        }

        void transmit(command cmd, const void* data, size_t size) override {
            tx.service = cmd;
            tx.length = size > BUFFER_ARG_SIZE() ? BUFFER_ARG_SIZE() : size;
            memcpy(tx.data, data, tx.length);
            client.Send(&tx, BUFFER_HEADER_SiZE() + tx.length);
        }

        void flush() override {}
    };

    class TCPserverAPI : public API
    {
    private:
        TCP::server& server;
    public:
        TCPserverAPI(std::initializer_list<Module*> modules, TCP::server& server)
            : API(modules), server(server)
        {
        }

        bool receive() override {
            return server.Receive(&rx, sizeof(rx)) >= 0;
        }

        void transmit(command cmd, const void* data, size_t size) override {
            tx.service = cmd;
            tx.length = size > BUFFER_ARG_SIZE() ? BUFFER_ARG_SIZE() : size;
            memcpy(tx.data, data, tx.length);
            server.Send(&tx, BUFFER_HEADER_SiZE() + tx.length);
        }

        void flush() override {}
    };
}