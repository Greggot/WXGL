#pragma once
#include <stdint.h>
#include <cstring>
#include <cstdio>

namespace Compression
{
	union RLEHeader
	{
		uint8_t raw;
		struct _bit
		{
			uint8_t length : 7;
			uint8_t repeated : 1;
		} bit;

		RLEHeader(bool repeated, uint8_t length){
			bit.repeated = repeated;
			bit.length = length;
		}
		RLEHeader() { raw = 0; }
		RLEHeader(uint8_t value) { raw = value; }

	};

	enum Sequence : uint8_t
	{
		original,
		repeated,
	};

	static inline void InitializeSequence(RLEHeader** headerptr, uint8_t* destination, size_t& index)
	{
		RLEHeader* header = *headerptr;

		header = (RLEHeader*)&destination[index];
		header->bit.length = 0;
		index += sizeof(RLEHeader);

		*headerptr = header;
	}

	static inline void FinishSequence(RLEHeader** headerptr, uint8_t* destination, size_t& index, bool repeated)
	{
		(*headerptr)->bit.repeated = repeated;
		InitializeSequence(headerptr, destination, index);
	}

	static inline bool headerIsOverflowed(const RLEHeader& header)
	{
		static const RLEHeader cmp(-1);
		return header.bit.length == cmp.bit.length;
	}

	template<class type>
	size_t RLE(void* destination, const type* source, size_t sourcesize, size_t destinationsize, size_t& handledsize)
	{
		uint8_t* dst = (uint8_t*)destination;

		RLEHeader* header;
		size_t index = 0;
		InitializeSequence(&header, dst, index);

		for (size_t i = 0; i < sourcesize; ++i)
		{
			if (source[i] != source[i + 1])
			{
				memcpy(&dst[index], &source[i], sizeof(type));
				index += sizeof(type);
				++header->bit.length;
				if (headerIsOverflowed(*header))
					FinishSequence(&header, dst, index, original);
			}
			else
			{
				if (header->bit.length)
					FinishSequence(&header, dst, index, original);

				type value = source[i];
				memcpy(&dst[index], &value, sizeof(type));
				index += sizeof(type);
				while (source[i++] == value)
				{
					++header->bit.length;
					if (headerIsOverflowed(*header))
					{
						FinishSequence(&header, dst, index, repeated);
						memcpy(&dst[index], &value, sizeof(type));
						index += sizeof(type);
					}
				}
				i -= 2;

				if (header->bit.length)
					FinishSequence(&header, dst, index, repeated);

				if (index >= destinationsize - sizeof(type) - sizeof(header))
				{
					handledsize = i;
					return index - sizeof(type);
				}
			}
		}
		return index - sizeof(type);
	}
}

namespace Decompression
{
	template <class type>
	size_t RLE(type* destination, const void* source, size_t size) {
		uint8_t* src = (uint8_t*)source;

		Compression::RLEHeader header;
		size_t i = 0;
		size_t j = 0;

		while (i < size)
		{
			memcpy(&header, &src[i++], sizeof(header));
			while (header.bit.length--)
			{
				memcpy(&destination[j++], &src[i], sizeof(type));
				if (!header.bit.repeated)
					i += sizeof(type);
			}
			if (header.bit.repeated)
				i += sizeof(type);
		}
		return j;
	}
}