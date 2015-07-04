
#pragma once
#include "precomp.h"
#include "refSystem.h"
#include "icon.h"

class cItemDrop
{
public:
	std::string type;
	int count;

	cItemDrop() {
		type = "";
		count = 1;
	}

	cItemDrop(std::string item) {
		type = item;
		count = 1;
	}

	cItemDrop(const char* item) {
		type = item;
		count = 1;
	}

	cItemDrop(std::string item, int amount) {
		type = item;
		count = amount;
	}

	cItemDrop& operator=(std::string item) {
		type = item;
		count = 1;
		return *this;
	}
};

class cItemDismantle
{
public:
	int itemCounter;
	std::string item[LIMIT_DISMANTLE];
	int amount[LIMIT_DISMANTLE];

	void add(std::string type, int count) {
		item[itemCounter] = type;
		amount[itemCounter++] = count;
	}

	cItemDismantle() {
		itemCounter = 0;
	}
};

class cDropContainer
{
public:
	int itemCounter;
	std::string item[LIMIT_DROP];
	int amount[LIMIT_DROP];
	float chance[LIMIT_DROP];
	int powerLevel[LIMIT_DROP];

	void add(std::string type, int count = 1, float chance = 1.00f, int powerLevel = POWER_HAND);
	int flush(int unitId, int usedPowerLevel = POWER_IMBA);
	int flush(sf::Vector2f pos, int usedPowerLevel = POWER_IMBA);

	cDropContainer() {
		itemCounter = 0;
	}
};

class cItemConsume
{
public:
	int healthBalance;

	cItemConsume(){
		healthBalance = 0;
	}
};

class cItem : public cReference
{
public:
	int globalId;
	int category;
	std::string type;
	std::string displayName;
	int descrLinesUsed;
	std::string description[LIMIT_DESCR];

	bool equipped;
	float weight;
	int powerLevel;
	cIcon icon;
	cItemConsume consume;
	cItemDismantle dismantle;

	void addDescrLine(std::string line);
	void setName(std::string txt);

	void use();
	void equip();

	cItem() {
		powerLevel = 0;
		equipped = false;
		weight = 1.00f;
		descrLinesUsed = 0;
		category = CATEGORY_OTHER;
	}
};

class cItemContainer
{
public:
	float resource;

	bool openable;
	int itemLimit;
	int itemCounter;
	cItem item[LIMIT_ITEMS];
	int amount[LIMIT_ITEMS];

	int add(cItem target, int count = 1);
	int add(std::string type, int count = 1);
	// Get item from global id
	cItem& get(int id);
	// Get item from type
	cItem& get(std::string type, int begin = 0);
	// Get amount of items with that type
	int getAmount(std::string type);
	// Remove item using it's global id
	bool remove(int id, int count = -1);
	// Remove item with given type
	bool remove(std::string type, int count = -1, int begin = 0);

	// Get list number from global id
	int getId(int id);
	// Get list number from item type
	int getId(std::string type, int begin = 0);

	void sort(int sortType);

	cItemContainer() {
		openable = false;
		resource = 0.00f;
		itemLimit = 100;
		itemCounter = 0;
	}
};