
#include "main.h"
#include "util.h"

void cUtil::detectVideoCard()
{
	string vendor = reinterpret_cast< char const * >(glGetString(GL_VENDOR));
	if (vendor == "Intel") {
		core.videoCard = VIDEO_INTEL;
		console << "[MAIN] Detected Intel video card" << "\n";
	}
	else if (vendor.substr(0, 6) == "NVIDIA") {
		core.videoCard = VIDEO_NVIDIA;
		console << "[MAIN] Detected Nvidia video card" << "\n";
	}
	else if (vendor.substr(0, 3) == "ATI") { 
		core.videoCard = VIDEO_ATI;
		console << "[MAIN] Detected AMD video card" << "\n";
	}
	else if (vendor.substr(0, 9) == "Microsoft") {
		core.videoCard = VIDEO_MICROSOFT;
		console << "[MAIN] Detected Microsoft video card" << "\n";
	}
	else {
		core.videoCard = VIDEO_OTHER;
		console << "[MAIN] Unknown video card: " << vendor << "\n";
	}
}

int cUtil::getInventorySortingType()
{
	return SORT_BYNAME;
}

void cUtil::checkLogFiles()
{
	int fileSize;
	ifstream file;
	ofstream clr;

	// Game log
	if (!core.serverMode)
	{
		file.open("Logs//game.txt");
		file.seekg(0, file.end);
		fileSize = (int)file.tellg();
		file.close();
		// Filesize is more than a megabyte
		if (fileSize > 1000000) {
			ofstream clr;
			clr.open("Logs//game.txt");
			clr.close();
		}
	}
	// Server mode
	else
	{
		file.open("Logs//server.txt");
		file.seekg(0, file.end);
		fileSize = (int)file.tellg();
		file.close();
		// Filesize is more than a megabyte
		if (fileSize > 1000000) {
			ofstream clr;
			clr.open("Logs//server.txt");
			clr.close();
		}
	}
}

string cUtil::getCurrentTimeString()
{
	time_t t = time(0);
	struct tm time;
	localtime_s(&time, &t);
	string retVal = "";
	// Hours
	if (time.tm_hour < 10) {
		retVal += "0";
	}
	retVal += to_string(time.tm_hour);
	retVal += ":";
	// Minutes
	if (time.tm_min < 10) {
		retVal += "0";
	}
	retVal += to_string(time.tm_min);
	retVal += ":";
	// Seconds
	if (time.tm_sec < 10) {
		retVal += "0";
	}
	retVal += to_string(time.tm_sec);
	return retVal;
}

sf::Color cUtil::convertUnitHighlightColor(int code)
{
	switch (code)
	{
	case 0:
		return parseOldschoolColorCode("bbffcc");
		break;
	case 1:
		return parseOldschoolColorCode("00ffff");
		break;
	case 2:
		return parseOldschoolColorCode("00ff55");
		break;
	case 3:
		return parseOldschoolColorCode("ffff99");
		break;
	case 4:
		return parseOldschoolColorCode("ffcc99");
		break;
	case 5:
		return parseOldschoolColorCode("ffbbcc");
		break;
	case 6:
		return parseOldschoolColorCode("cccccc");
		break;
	case 7:
		return parseOldschoolColorCode("bbffff");
		break;
	}
	return sf::Color(255, 200, 50);
}

sf::Color cUtil::parseOldschoolColorCode(string code)
{
	sf::Color retVal;
	string val;
	int value, converted;
	for (int i = 0; i < 3; i++)
	{
		// First digit
		val = code.substr(i * 2, 1);
		value = 0;
		if (val == "a") { value += 16 * 10; }
		else if (val == "b") { value += 16 * 11; }
		else if (val == "c") { value += 16 * 12; }
		else if (val == "d") { value += 16 * 13; }
		else if (val == "e") { value += 16 * 14; }
		else if (val == "f") { value += 16 * 15; }
		else { stringstream(val) >> converted; value += converted * 16; }

		// Second digit
		val = code.substr(i * 2 + 1, 1);
		if (val == "a") { value += 10; }
		else if (val == "b") { value += 11; }
		else if (val == "c") { value += 12; }
		else if (val == "d") { value += 13; }
		else if (val == "e") { value += 14; }
		else if (val == "f") { value += 15; }
		else { stringstream(val) >> converted; value += converted; }

		// Flushing
		if (i == 0) { retVal.r = value; }
		if (i == 1) { retVal.g = value; }
		if (i == 2) { retVal.b = value; }
	}

	return retVal;
}

string cUtil::buildRefToType(int ref)
{
	if (ref == REF_ITEM_BUILD_CAMPFIRE) { return "campfire_full"; }
	else if (ref == REF_ITEM_BUILD_FURNACEBASIC) { return "furnace_basic"; }
	else if (ref == REF_ITEM_BUILD_FURNACEIND) { return "furnace_industrial"; }
	else if (ref == REF_ITEM_BUILD_TENT) { return "tent_basic"; }
	return "missingno";
}

int cUtil::typeToBuildRef(string str)
{
	if (str == "campfire_full") { return REF_ITEM_BUILD_CAMPFIRE; }
	else if (str == "furnace_basic") { return REF_ITEM_BUILD_FURNACEBASIC; }
	else if (str == "furnace_industrial") { return REF_ITEM_BUILD_FURNACEIND; }
	else if (str == "tent_basic") { return REF_ITEM_BUILD_TENT; }
	return -1;
}

bool cUtil::intersects(vec2f pos, vec2f rectPos, vec2f rectSize)
{
	if (pos.x < rectPos.x) { return false; }
	if (pos.y < rectPos.y) { return false; }
	if (pos.x > rectPos.x + rectSize.x) { return false; }
	if (pos.y > rectPos.y + rectSize.y) { return false; }
	return true;
}