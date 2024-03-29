
function createMockBle(){
	console.log("Running from mock ble service.");
		ble = {
			self: this,
			startScan: function(services,success,error)
			{
				console.log("Starting scan");
				var p = this.Peripheral_Mock(false);
				p.name="Dummy BLE device";
				p.id = "D8479A4F-7517-BCD3-91B5-3302B2F81802";
				p.rssi = 50;
				p.advertising = services;

				success(p);
			},
			stopScan: function(success,fail){
				console.log("Stopping scan");
				success();
			},
			connect: function(device_id,success,error){
				console.log("Connecting");
				var p = this.Peripheral_Mock(true);
				var c = this.CharacteristicMock();

				c.service = "FFE0";
				c.characteristic = "FFE1";
				c.properties = ["read","writeWithoutNotify"];

				p.name="Dummy BLE device";
				p.id = "D8479A4F-7517-BCD3-91B5-3302B2F81802";
				p.rssi= 40;
				p.advertising = ["FFE0"]
				p.characteristics = [c]
				setTimeout(success,500,p);
				
			},
			disconnect: function(device_id,success,error){console.error("Disconnect is not implemented yet.")},
			writeWithoutResponse: function(device_id,service,characteristic,buffer,success,error)
			{
				setTimeout(success,0);
			},
			Peripheral_Mock: function(connected)
			{
				if(!connected)
				{
					var obj = 
					{
						name: "",
						id: "",
						advertising: [],
						rssi: 0
					}
					return obj;
				}
				if(connected)
				{
					var obj = 
					{
						name: "",
						id: "",
						advertising: [],
						rssi: 0,
						services: [],
						characteristics: []
					};
					return obj;
				}
			},

			CharacteristicMock: function()
			{
				var obj = 
				{
					service: "",
					characteristic: "",
					properties: []
				}
				return obj;
			}
		}
}

app.service("$Bluetooth",function($q){
		
		self = this;
		this.Scanning=false;
		this.service = "FFE0";
		this.characteristic = "FFE1";
		this.connectedDevice;
		this.devices = []
		

		this.ScanForPeripherals = function()
		{
			var deferred = $q.defer();
			this.devices = [];
			
			setTimeout(ble.stopScan,
				3000,
				function(){deferred.resolve();},
				function(){}
				);

			ble.startScan(['FFE0'],
				function(bt_device){
					var d = {
						Name:bt_device.name,
						RSSI:bt_device.rssi,
						ID:bt_device.id
					};

					self.devices.push(d);
					
				}, 
				function(error){
					console.error(error);
					deferred.fail(error);
				}
			);
			return deferred.promise;
		};

		this.ConnectToPeripheral= function(id)
		{
			var deferred = $q.defer()
			console.log("Attempting to connect to " + id);
			ble.connect(id,
					function(peripheral)
					{
						deferred.notify(peripheral);
						self.connectedDevice = peripheral;
					},
					function(error)
					{
						alert("Fail!");
						deferred.reject(error);
					}
				);

			return deferred.promise;
		};

		this.DisconnectFromPeripheral = function(BT_dev_id)
		{

		};

		this.Write= function(value){
			
			var deferred = $q.defer();
			console.log("Writing " + value + " to " + self.connectedDevice.id);
			var a = new Uint8Array(1);
			a[0] = value;


			ble.writeWithoutResponse(
				self.connectedDevice.id, 
				this.service, 
				this.characteristic,
				a.buffer,
				function(a){deferred.resolve();},
				function(a){deferred.fail();});

			return deferred.promise;
		}

});