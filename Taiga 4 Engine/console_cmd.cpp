
#include "main.h"

// Help command
void cmd_help(string args[])
{
	console.echo << "[CMD] Available commands:" << "\n";
	for (int i = 0; i < LIMIT_CMD; i++)
	{
		if (console.cmdSyntax[i].length() > 0 && (!console.cmdServerOnly[i] || !client.connected || core.localServer)
			&& (args[0] == "0" || console.cmdSyntax[i].find(args[0]) != -1))
		{
			console.echo << "[CMD] " << console.cmdSyntax[i] << "\n";
		}
	}
}

// Echo
void cmd_echo(string args[])
{
	console.echo << args[0] << endl;
}

// Clear
void cmd_clear(string args[])
{
	console.clear();
}

// Macro command
void cmd_macro(string args[])
{
}

// Unit.add
void cmd_unit_add(string args[])
{
	sf::Packet data;
	float posX, posY;
	stringstream(args[1]) >> posX;
	stringstream(args[2]) >> posY;
	game.addUnit(args[0], sf::Vector2f(posX, posY));
}

// Unit.moveto
void cmd_unit_moveto(string args[])
{
	int id;
	float posX, posY;
	stringstream(args[0]) >> id;
	stringstream(args[1]) >> posX;
	stringstream(args[2]) >> posY;
	id = game.getUnitId(id);
	if (id != -1) {
		game.unit[id].pos = sf::Vector2f(posX, posY);
	}
}

// Unit.setspeed
void cmd_unit_setspeed(string args[])
{
	int id;
	float speed;
	stringstream(args[0]) >> id;
	stringstream(args[1]) >> speed;
	id = game.getUnitId(id);
	if (id != -1) {
		game.unit[id].movementSpeed = speed;
	}
}

// Unit.remove
void cmd_unit_remove(string args[])
{
	int id;
	stringstream(args[0]) >> id;
	game.removeUnit(id);
}

// Unit.getlist
void cmd_unit_getlist(string args[])
{
	string type;
	stringstream(args[0]) >> type;
	console.echo << "[CMD] [LocalId]: GlobalId / Type / PosX / PosY" << "\n";
	for (int i = 0; i < game.unitCounter; i++)
	{
		if (type == "0" || game.unit[i].type.find(type) != -1)
		{
			console.echo << "[CMD] [" << i << "]: " << game.unit[i].globalId << " / " << game.unit[i].type << " / "
				<< game.unit[i].pos.x << " / " << game.unit[i].pos.y << "\n";
		}
	}
}

// Unit.order.moveto
void cmd_unit_order_moveto(string args[])
{
	int id;	float targetX, targetY;	bool overwrite;
	stringstream(args[0]) >> id;
	stringstream(args[1]) >> targetX;
	stringstream(args[2]) >> targetY;
	stringstream(args[3]) >> overwrite;

	if (game.getUnitId(id) != -1)
	{
		game.unit[game.getUnitId(id)].addOrder_moveto(sf::Vector2f(targetX, targetY), overwrite);
	}
}

// Unit.order.clear
void cmd_unit_order_clear(string args[])
{
	int id;
	stringstream(args[0]) >> id;
	if (game.getUnitId(id) != -1)
	{
		game.unit[game.getUnitId(id)].orderCounter = 0;
	}
}

// Unit.order.getlist
void cmd_unit_order_getlist(string args[])
{
	int id;		string order;
	stringstream(args[0]) >> id;
	id = game.getUnitId(id);

	console.echo << "[CMD] [Id]: Type / Args..." << "\n";
	for (int i = 0; i < game.unit[id].orderCounter; i++)
	{
		if (game.unit[id].order[i].type == ORDER_IDLE) { order = "idle"; }
		if (game.unit[id].order[i].type == ORDER_MOVETO) { order = "moveto"; }

		console.echo << "[CMD] [" << i << "]: " << order << " / ";
		if (order == "moveto")
		{
			console.echo << game.unit[id].order[i].targetPos.x << " / " << game.unit[id].order[i].targetPos.y << "\n";
		}
	}
}

// Unit.item.add
void cmd_unit_item_add(std::string args[])
{
	int unitId, itemCount = 1;
	string itemType;
	stringstream(args[0]) >> unitId;
	itemType = args[1];
	if (args[2] != "0") { stringstream(args[2]) >> itemCount; }

	game.getUnit(unitId).container.add(itemType, itemCount);
}

// Unit.item.remove
void cmd_unit_item_remove(std::string args[])
{
	int unitId, itemCount = 1;
	string itemType;
	stringstream(args[0]) >> unitId;
	itemType = args[1];
	if (args[2] != "0") { stringstream(args[2]) >> itemCount; }

	game.getUnit(unitId).container.remove(itemType, itemCount);
}

// Unit.item.getlist
void cmd_unit_item_getlist(std::string args[])
{
	int unitId;
	stringstream(args[0]) >> unitId;
	cItemContainer* cont = &game.getUnit(unitId).container;

	console.echo << "[CMD] [Id]: Item type / Display name / Amount" << "\n";
	for (int i = 0; i < cont->itemCounter; i++)
	{
		console.echo << "[CMD] [" << i << "]: " << cont->item[i].type << " / " << cont->item[i].displayName << " / " << cont->amount[i] << "\n";
	}
}

// Camera.attach
void cmd_camera_attach(string args[])
{
	camera.moveWithCharacter = !camera.moveWithCharacter;
}

// Camera.lock
void cmd_camera_lock(string args[])
{
	camera.lockedToCharacter = !camera.lockedToCharacter;
}

// Game.timemod
void cmd_game_timemod(string args[])
{
	float value = 100;
	if (args[0] != "0") { stringstream(args[0]) >> value; }

	core.timeModifier = value / 100.00f;
}

// Game.settime
void cmd_game_settime(string args[])
{
	float time = 12.00f;
	stringstream(args[0]) >> time;
	game.timeOfDay = time;
}

// Game.setlight
void cmd_game_setlight(string args[])
{
	float light = 255.00f;
	stringstream(args[0]) >> light;
	game.ambientLight = light;
}

// Client.connect
void cmd_client_connect(string args[])
{
	string ip = args[0];
	short port = 21045;
	if (args[1] != "0") { stringstream(args[1]) >> port; }
	client.connect(ip, port);
}

// Client.disconnect
void cmd_client_disconnect(string args[])
{
	console.echo << "[CMD] Connection to server closed" << "\n";
	client.disconnect();
}

// Server.open
void cmd_server_open(string args[])
{
	//core.localServer = true;
	//console << "[CMD] Local server operational" << "\n";
	//console << "[CMD] Warning: Local server is not really implemented yet. Expect random bugs." << "\n";
}

// Player.setunit
void cmd_player_setunit(string args[])
{
	int player, unit;
	stringstream(args[0]) >> player;
	stringstream(args[1]) >> unit;
	server.player[player].unit = unit;
}

// Player.getlist
void cmd_player_getlist(string args[])
{
	console.echo << "[CMD] [Id]: Unit Id / Address" << "\n";
	for (int i = 0; i < LIMIT_SERVER_PLAYERS; i++)
	{
		if (server.player[i].connected)
		{
			console.echo << "[CMD] [" << i << "]: " << server.player[i].unit << " / " << server.player[i].socket.getRemoteAddress().toString() << "\n";
		}
	}
}

// Player.kick
void cmd_player_kick(string args[])
{
	int id;
	stringstream(args[0]) >> id;
	// Closing the connection
	console.echo << "[CMD] Player " << id << " kicked!" << "\n";
	server.player[id].disconnect();
}

// Ui.getlist
void cmd_ui_getlist(string args[])
{
	string type;
	stringstream(args[0]) >> type;
	console.echo << "[CMD] [Id]: Local id / Type / Pos X / Pos Y / Action / Args[0]" << "\n";
	for (int i = 0; i < LIMIT_UI_ELEMENTS; i++)
	{
		if (ui.element[i].isValid && (type == "0" || ui.element[i].type.find(type) != -1))
		{
			console.echo << "[CMD] [" << ui.element[i].globalId << "]: " << i << " / " << ui.element[i].type << " / " << ui.element[i].pos.x << " / " <<
				ui.element[i].pos.y << " / " << ui.element[i].button.action << " / " << ui.element[i].button.args[0] << "\n";
		}
	}
}

// Editor.enable
void cmd_editor_enable(string args[])
{
	editor.enable();
}

// Editor.save
void cmd_editor_save(string args[])
{
	editor.saveBlueprint(args[0]);
}

// Editor.load
void cmd_editor_load(string args[])
{
	editor.loadBlueprint(args[0]);
}

// Editor.autogen
void cmd_editor_autogen(string args[])
{
	int count, startIndex = 0;
	stringstream(args[0]) >> count;
	stringstream(args[1]) >> startIndex;

	editor.genBlueprint();
	for (int i = 0; i < count; i++)
	{
		editor.saveBlueprint("gen" + to_string(i + startIndex));
		editor.genBlueprint();
	}
}

// Editor.autogen.settype
void cmd_editor_setgentype(string args[])
{
	int type;
	stringstream(args[0]) >> type;

	editor.autogenType = type;
}

// Database.getunitlist
void cmd_database_getunitlist(string args[])
{
	string type;
	stringstream(args[0]) >> type;
	console.echo << "[CMD] Type" << "\n";
	for (int i = 0; i < LIMIT_DB_UNIT; i++)
	{
		if (database.unit[i].type.length() > 0 && (type == "0" || database.unit[i].type.find(type) != -1))
		{
			console << "- " << database.unit[i].type << endl;
		}
	}
}

// Database.reload
void cmd_database_reload(string args[])
{
	database.loadExternal();

	for (int y = 0; y < LIMIT_MAP; y++)
	{
		for (int x = 0; x < LIMIT_MAP; x++)
		{
			if (world.map[x][y].isLoaded)
			{
				world.saveChunk(vec2i(x, y));
				world.unloadChunk(vec2i(x, y));
				world.loadChunk(vec2i(x, y));
			}
		}
	}
}

// Exit command
void cmd_exit(string args[])
{
	console.online = false;
	core.shutdown = true;
}