
#include "main.h"

void cWorld::saveChunk(vec2i pos)
{
	//return;
	/*
	WARNING:
	This function is only to be used on the server side!
	Client does not store any world data on the disk.
	*/

	// Don't save undefined chunks
	if (map[pos.x][pos.y].type == CHUNK_UNDEFINED) { return; }

	cUnitEntry unitEntry;
	cItemEntry itemEntry;
	vector<cUnitEntry> unitList;
	vec2 anchor = getChunkCenter(pos);

	//access.lock();
	//game.access.lock();

	for (int i = 0; i < game.unitCounter; i++)
	{
		if (game.unit[i].chunkPos == pos && !game.unit[i].hasRef(REF_UNIT_NOUNLOAD))
		{
			unitEntry.pos = game.unit[i].pos - anchor;
			unitEntry.type = game.unit[i].type;
			unitEntry.globalId = game.unit[i].globalId;
			for (int y = 0; y < game.unit[i].container.itemCounter; y++)
			{
				itemEntry.type = game.unit[i].container.item[y].type;
				itemEntry.count = game.unit[i].container.amount[y];
				unitEntry.itemList.push_back(itemEntry);
			}
			unitList.push_back(unitEntry);
			unitEntry.itemList.clear();
		}
	}
	save.flushListToFile(map[pos.x][pos.y].type, unitList, save.getChunkFilePath(pos));

	//game.access.unlock();
	//access.unlock();
}

void cWorld::loadChunk(vec2i pos)
{
	/*
	WARNING:
	This function is only to be used on the server side!
	Client must request the chunk update from the server.
	*/
	//access.lock();
	//game.access.lock();

	int attachIndex = -1;
	vec2 anchor = getChunkCenter(pos);

	// Adding units
	int id = 0;
	vector<cUnitEntry> unitList = getChunkUnitList(pos);
	for (int i = 0; i < (int)unitList.size(); i++)
	{
		if (game.getUnitId(unitList[i].globalId) == -1)
		{
			id = game.addUnit(unitList[i].type, anchor + unitList[i].pos, -1, -1, false);
			// If the object has global id assigned, load additional information
			if (unitList[i].globalId != -1) {
				// Global Id
				game.getUnit(id).globalId = unitList[i].globalId;
				id = unitList[i].globalId;
				// Item data
				for (int y = 0; y < (int)unitList[i].itemList.size(); y++)
				{
					game.getUnit(id).addItem(unitList[i].itemList[y].type, unitList[i].itemList[y].count);
				}
			}
			// Tell unit to which chunk it belongs
			game.getUnit(id).chunkPos = pos;
			if (pos != world.getChunkInPos(anchor + unitList[i].pos))
			{
				console << "ALARMA: " << pos.x << "; " << pos.y << " / " << world.getChunkInPos(game.unit[game.getUnitId(id)].pos).x << "; "
					<< world.getChunkInPos(game.unit[game.getUnitId(id)].pos).y << endl;
			}
		}
	}
	map[pos.x][pos.y].isLoaded = true;

	//game.access.unlock();
	//access.unlock();
}

void cWorld::unloadChunk(vec2i pos)
{
	if (!map[pos.x][pos.y].isLoaded) { console << "[ERROR] Chunk (" << pos.x << "; " << pos.y << ") is not loaded!"; return; }
	//access.lock();
	//game.access.lock();
	for (int i = 0; i < game.unitCounter; i++)
	{
		if (game.unit[i].chunkPos == pos && !game.unit[i].hasRef(REF_UNIT_NOUNLOAD))
		{
			game.removeUnit(game.unit[i].globalId, false);
			i -= 1;
		}
	}
	map[pos.x][pos.y].isLoaded = false;
	//game.access.unlock();
	//access.unlock();
}

vector<cUnitEntry> cWorld::getChunkUnitList(vec2i pos)
{
	return save.getListFromFile(save.getChunkFilePath(pos));
}

vector<cUnitEntry> cWorld::getBlueprintUnitList(int index)
{
	vector<cUnitEntry> retVal;
	if (index == -1) { console << "[ERROR] Invalid blueprint id!" << "\n"; return retVal; }
	return save.getListFromFile("Data//Blueprints//" + blueprint[index].name);
}