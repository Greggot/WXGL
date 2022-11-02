#pragma once
#include <cstdint>

namespace SkyBlue
{
    enum command : uint8_t
    {
        report,
        read,
        write,

        amount
    };
    inline const char* commandstring(command cmd) {
        static const char* descr[] = { "report", "read", "write" };
        return descr[cmd];
    }

    enum class type_t : uint16_t
    {
        none,
        steady,

        rotorservo,
        linearservo,
        camera,

        amount
    };
    inline const char** TypeDescriptions() {
        static const char* descr[] = { "none", "steady peace", "rotor", "linear", "camera" };
        return descr;
    }

    inline const char* typeToString(type_t type) {
        return TypeDescriptions()[static_cast<int>(type)];
    }

    union ID
    {
        uint32_t raw;
        struct {
            uint16_t number;
            type_t type;
        };

        ID(uint16_t number, type_t type) : number(number), type(type) {}
        ID(uint32_t raw = 0) : raw(raw) {}

        bool operator==(const ID& id) const {
            return raw == id.raw;
        }
    };

    #define BUFFER_SIZE 1440
    constexpr size_t BUFFER_HEADER_SiZE() { return sizeof(ID) + sizeof(command) + sizeof(size_t); }
    constexpr size_t BUFFER_ARG_SIZE() {
        return BUFFER_SIZE - BUFFER_HEADER_SiZE();
    }

    #pragma pack(push, 1)
    struct buffer
    {
        ID id;
        command service;
        uint32_t length;
        uint8_t data[BUFFER_ARG_SIZE()];
    };
    #pragma pack(pop)
}