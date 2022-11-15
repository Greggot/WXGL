#pragma once
#include <vector>
#include <cstdint>

#include <Model/BaseModel.hpp>

namespace Assembly
{
	class Core
	{
	protected:
		std::vector<DrawableModel*> Models;
		DrawableModel* Active = nullptr;
		size_t Index = 0;
	public:
		Core() {};
		// Need to iterate throgh by ranged loops
		std::vector<DrawableModel*>::iterator begin() { return Models.begin(); }
		std::vector<DrawableModel*>::iterator end() { return Models.end(); }

		size_t size() const { return Models.size(); }
		DrawableModel& operator[](size_t i) { return *Models[i]; }
		void append(DrawableModel* model);
		void remove(uint16_t index);
		void clear();

		DrawableModel* active() const { return Active; }
		size_t activeindex() const { return Index; }
		void setActive(size_t Index);
		void setActive(DrawableModel*);
		void dropSelection() { setActive(nullptr); }

		virtual ~Core() { clear(); }
	};
}