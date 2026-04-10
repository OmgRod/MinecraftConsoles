#include "stdafx.h"
#include <iostream>
#include "InputOutputStream.h"
#include "PacketListener.h"
#include "ContainerOpenPacket.h"

const int ContainerOpenPacket::WORKBENCH;
const int ContainerOpenPacket::FURNACE;
const int ContainerOpenPacket::TRAP;
const int ContainerOpenPacket::ENCHANTMENT;
const int ContainerOpenPacket::BREWING_STAND;
const int ContainerOpenPacket::TRADER_NPC;
const int ContainerOpenPacket::BEACON;
const int ContainerOpenPacket::REPAIR_TABLE;
const int ContainerOpenPacket::HOPPER;
const int ContainerOpenPacket::DROPPER;
const int ContainerOpenPacket::HORSE;
const int ContainerOpenPacket::FIREWORKS;

void ContainerOpenPacket::_init(int containerId, int type, const wstring &title, int size, bool customName, int entityId)
{
	this->containerId = containerId;
	this->type = type;
	this->title = title;
	this->size = size;
	this->customName = customName;
	this->entityId = entityId;
}

ContainerOpenPacket::ContainerOpenPacket()
{
	_init(0, 0, L"", 0, false, 0);

}

ContainerOpenPacket::ContainerOpenPacket(int containerId, int type, const wstring &title, int size, bool customName) 
{
	_init(containerId, type, title, size, customName, 0);
}

ContainerOpenPacket::ContainerOpenPacket(int containerId, int type, const wstring &title, int size, bool customName, int entityId)
{
	_init(containerId, type, title, size, customName, entityId);
}

void ContainerOpenPacket::handle(PacketListener *listener)
{
	listener->handleContainerOpen(shared_from_this());
}


void ContainerOpenPacket::read(DataInputStream *dis) //throws IOException 
{
	containerId = dis->readByte() & 0xff;
	type = dis->readByte() & 0xff;
	size = dis->readByte() & 0xff;
	customName = dis->readBoolean();
	if (type == HORSE)
	{
		entityId = dis->readInt();
	}
	if(customName)
	{
		title = readUtf(dis,64);
	}
}

void ContainerOpenPacket::write(DataOutputStream *dos) //throws IOException
{
	dos->writeByte(containerId & 0xff);
	dos->writeByte(type & 0xff);
	dos->writeByte(size & 0xff);
	dos->writeBoolean(customName);
	if (type == HORSE)
	{
		dos->writeInt(entityId);
	}
	if(customName)
	{
		writeUtf(title, dos);
	}
}

int ContainerOpenPacket::getEstimatedSize() 
{
	if (type == HORSE)
	{
		return 10;
	}
	return 6;
}
