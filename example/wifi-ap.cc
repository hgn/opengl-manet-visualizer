#include "ns3/core-module.h"
#include "ns3/common-module.h"
#include "ns3/node-module.h"
#include "ns3/helper-module.h"
#include "ns3/mobility-module.h"
#include "ns3/contrib-module.h"
#include "ns3/wifi-module.h"

#include <iostream>

using namespace ns3;

void
rx_packet(std::string context, Ptr < const Packet > p, Mac48Address address)
{
	NetDeviceContainer devs;
	NodeContainer nc = NodeContainer::GetGlobal();

	(void) context;

	for (NodeContainer::Iterator i = nc.Begin(); i != nc.End(); ++i) {
		Ptr < Node > node = *i;
		for (uint32_t j = 0; j < node->GetNDevices(); ++j) {
			Ptr < NetDevice > dev = node->GetDevice(j);
			Mac48Address a =
			    Mac48Address::ConvertFrom(dev->GetAddress());
			if (a == address) {
				Vector pos;
				Ptr < MobilityModel > mobility = node->GetObject < MobilityModel > ();
				pos = mobility->GetPosition();
				std::cout << "r " << node->GetId() << " " <<
				    pos.x << " " << pos.y << "size" << p->GetSize() << " " << std::endl;

				return;
			}
		}
	}

	// should not happend
	return;
}

static void SetPosition(Ptr < Node > node, Vector position)
{
	Ptr < MobilityModel > mobility = node->GetObject < MobilityModel > ();
	mobility->SetPosition(position);
}

static Vector GetPosition(Ptr < Node > node)
{
	Ptr < MobilityModel > mobility = node->GetObject < MobilityModel > ();
	return mobility->GetPosition();
}

static void AdvancePosition(Ptr < Node > node)
{
	Vector pos = GetPosition(node);
	pos.x += 5.0;
	if (pos.x >= 210.0) {
		return;
	}
	SetPosition(node, pos);
	Simulator::Schedule(Seconds(1.0), &AdvancePosition, node);
}

int main(int argc, char *argv[])
{
	(void) argc; (void) argv;

	Packet::EnablePrinting();

	// enable rts cts all the time.
	Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold",
			   StringValue("0"));
	// disable fragmentation
	Config::
	    SetDefault("ns3::WifiRemoteStationManager::FragmentationThreshold",
		       StringValue("2200"));

	WifiHelper wifi = WifiHelper::Default();
	MobilityHelper mobility;
	NodeContainer stas;
	NodeContainer ap;
	NetDeviceContainer staDevs;
	PacketSocketHelper packetSocket;

	stas.Create(2);
	ap.Create(1);

	// give packet socket powers to nodes.
	packetSocket.Install(stas);
	packetSocket.Install(ap);

	YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default();
	YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default();
	wifiPhy.SetChannel(wifiChannel.Create());
	Ssid ssid = Ssid("wifi-default");
	wifi.SetRemoteStationManager("ns3::ArfWifiManager");
	// setup stas.
	wifi.SetMac("ns3::NqstaWifiMac",
		    "Ssid", SsidValue(ssid),
		    "ActiveProbing", BooleanValue(false));
	staDevs = wifi.Install(wifiPhy, stas);
	// setup ap.
	wifi.SetMac("ns3::NqapWifiMac", "Ssid", SsidValue(ssid),
		    "BeaconGeneration", BooleanValue(true),
		    "BeaconInterval", TimeValue(Seconds(2.5)));
	wifi.Install(wifiPhy, ap);

	// mobility.
	mobility.Install(stas);
	mobility.Install(ap);

	Simulator::Schedule(Seconds(1.0), &AdvancePosition, ap.Get(2));

	PacketSocketAddress socket;
	socket.SetSingleDevice(staDevs.Get(0)->GetIfIndex());
	socket.SetPhysicalAddress(staDevs.Get(1)->GetAddress());
	socket.SetProtocol(1);

	OnOffHelper onoff("ns3::PacketSocketFactory", Address(socket));
	onoff.SetAttribute("OnTime", RandomVariableValue(ConstantVariable(42)));
	onoff.SetAttribute("OffTime", RandomVariableValue(ConstantVariable(0)));

	ApplicationContainer apps = onoff.Install(stas.Get(0));
	apps.Start(Seconds(0.5));
	apps.Stop(Seconds(43.0));

	Simulator::Stop(Seconds(44.0));

	Config::Connect("/NodeList/*/DeviceList/*/Rx",
			MakeCallback(&rx_packet));

	Simulator::Run();

	Simulator::Destroy();

	return 0;
}
