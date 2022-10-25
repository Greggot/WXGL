#include <cstdint>
namespace PhysicalDevice
{
	static const uint8_t gamma = 5;
	union RGB565
	{
		uint16_t raw;
		struct
		{
			uint16_t blue : 5;
			uint16_t green : 6;
			uint16_t red : 5;
		};

		RGB565(uint16_t raw = 0) { this->raw = raw; }
		RGB565(uint8_t r, uint8_t g, uint8_t b) { red = r; green = g, blue = b; }

		uint8_t red8() const { return (red * 527 + 23) >> 6; }
		uint8_t green8() const { return (green * 259 + 33) >> 6; }
		uint8_t blue8() const { return (blue * 527 + 23) >> 6; }
	};
	static bool operator== (const RGB565& r, const RGB565& l) {
		int dr = r.red - l.red;
		int dg = r.green - l.green;
		int db = r.blue - l.blue;

		if (dr < 0)
			dr = -dr;
		if (dg < 0)
			dg = -dg;
		if (db < 0)
			db = -db;

		return (dr < gamma&& dg < gamma) || (dr < gamma&& db < gamma) ||
			(dg < gamma&& db < gamma);
	}
	static bool operator!= (const RGB565& r, const RGB565& l) {
		return !(r == l);
	}
}