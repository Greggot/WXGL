#pragma once
#include <vector>
#include <cstdint>

#include "BaseModel.hpp"

namespace Assembly
{
	class Core
	{
	private:
		std::vector<BaseModel*> Models;
		BaseModel* Active = nullptr;
		size_t Index = 0;
	public:
		Core() {};
		// Need to iterate throgh by ranged loops
		std::vector<BaseModel*>::iterator begin() { return Models.begin(); }
		std::vector<BaseModel*>::iterator end() { return Models.end(); }

		size_t size() const { return Models.size(); }
		BaseModel& operator[](size_t i) { return *Models[i]; }
		void append(BaseModel* model);
		void remove(uint16_t index);
		void clear();

		BaseModel* active() const { return Active; }
		size_t activeindex() const { return Index; }
		void setActive(size_t Index);

		~Core() { clear(); }
	};
}