app.service("$Firmware",function($q,$Bluetooth){
	var firmware = {};

	firmware.relay_devices = 
	[
		{name: 'Relay 1', address: 0x01, status: 0},
		{name: 'Relay 2', address: 0x02, status: 0},
		{name: 'Relay 3', address: 0x04, status: 0},
		{name: 'Relay 4', address: 0x08, status: 0},
		{name: 'Relay 5', address: 0x10, status: 0},
		{name: 'Relay 6', address: 0x20, status: 0},
		{name: 'Relay 7', address: 0x40, status: 0},
		{name: 'Relay 8', address: 0x80, status: 0}
	];


	firmware.latch = function(){};
	//Sets the relays and immediatly latches
	firmware.SetRelaysAndLatch = function(Relays){};
	//Returns a promise and does not automatically latch
	firmware.SetRelays = function(Relays){};
	//Queries the device it'self and updates the relay_devices
	firmware.GetRelayStatus = function(){};
	


	return firmware;

});