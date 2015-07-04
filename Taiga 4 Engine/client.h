
#pragma once
#include "precomp.h"
#include "util.h"

class cClient
{
public:
	int dataQueueCounter;
	cPacketQueue dataQueue[LIMIT_SERVER_PACKETQUEUE];

	int unit;
	
	int packet;
	bool connected;
	sf::TcpSocket socket;
	int ping;
	int pingTimer;
	int lastPingTime;
	int lastPongTime;
	int mousePosTimer;
	sf::Vector2f lastMousePos;

	void sendEcho(std::string str);
	void sendPacket(sf::Packet data);
	void connect(std::string ip, short port);
	void disconnect();

	bool msgBig(sf::Packet data);
	bool msgUnit(sf::Packet data);
	bool msgOrder(sf::Packet data);
	bool msgGame(sf::Packet data);
	void pingServer();

	cClient() {
		unit = -1;
		ping = -1;
		packet = 0;
		connected = false;
		socket.setBlocking(false);
		pingTimer = 0;

		dataQueueCounter = 0;
	}
};

extern cClient client;

void clientReceiveMain();
void clientSendMain();