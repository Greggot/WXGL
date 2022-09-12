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
		size_t size() const { return Models.size(); }
		// Need to iterate throgh by ranged loops
		std::vector<BaseModel*>::iterator begin() { return Models.begin(); }
		std::vector<BaseModel*>::iterator end() { return Models.end(); }

		void append(BaseModel* model) { 
			Models.push_back(model); 
			setActive(size() - 1);
			if(size() > 1)
				model->LinkTo(Models[size() - 2]);
		}
		void remove(uint16_t index) { 
			delete Models[index];
			Models.erase(Models.begin() + index);
		}
		void clear() { 
			for (auto model : Models) 
				delete model; 
		}

		BaseModel* active() const { return Active; }
		size_t activeindex() const { return Index; }
		void setActive(size_t Index) { 
			this->Index = Index; 
			if(Active)
				Active->Active = false;
			Active = Models[Index]; 
			Active->Active = true;
		}

		~Core() { clear(); }
	};
}