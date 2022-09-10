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
		BaseModel& operator[](size_t i) { return *Models[i]; }

		void append(BaseModel* model) { Models.push_back(model); Active = model; }
		void remove(uint16_t index) { delete Models[index]; }
		void clear() { for (auto model : Models) delete model; }
		size_t size() const { return Models.size(); }

		BaseModel* active() const { return Active; }
		size_t activeindex() const { return Index; }
		void setActive(size_t Index) { this->Index = Index; Active = Models[Index]; }

		// Need to iterate throgh by ranged loops
		std::vector<BaseModel*>::iterator begin() { return Models.begin(); }
		std::vector<BaseModel*>::iterator end() { return Models.end(); }

		~Core() { clear(); }
	};
}
//
//void something()
//{
//	Assembly::Core core;
//}