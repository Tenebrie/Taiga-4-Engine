
#include "define.h"

class cPath
{
public:
	int waypointCounter;
	sf::Vector2f waypoint[LIMIT_ORDERS];

	void addWaypoint(sf::Vector2f loc);
	void clear();
	bool isPointFree(sf::Vector2f loc, float dist, int unitId);
	sf::Vector2f validatePoint(sf::Vector2f loc, float dist, int unitId);
	void calculate(sf::Vector2f start, sf::Vector2f end, float collision, int unitId);
};