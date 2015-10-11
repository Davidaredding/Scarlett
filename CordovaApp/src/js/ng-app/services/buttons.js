app.service("ButtonService",['ButtonModel',function(button){
	var Service = {};
	
	Service.RelayStatus = 0;
	/*
		ow o = 8 1 	0000 1001
		gw b = 32 4	0010 0100
		
	*/
	Service.buttons = 
	[
		new button({ID: 0, Name: "Fog Lights", Status: 0, onMask : 9, offMask:9}),
        new button({ID: 1, Name: "Spot Lights", Status: 0, onMask:36, offMask:36}),
        new button({ID: 2, Name: "1",  Status: 0, onMask : 1, 	offMask : 1 }),
        new button({ID: 3, Name: "2",  Status: 0, onMask : 2, 	offMask : 2 }),
        new button({ID: 4, Name: "4",  Status: 0, onMask : 4, 	offMask : 4 }),
        new button({ID: 5, Name: "8",  Status: 0, onMask : 8, 	offMask : 8 }),
        new button({ID: 6, Name: "16", Status: 0, onMask : 16, 	offMask : 16 }),
        new button({ID: 7, Name: "32", Status: 0, onMask : 32, 	offMask : 32 }),
        new button({ID: 8, Name: "64", Status: 0,  onMask : 64, offMask : 64 }),
        new button({ID: 9, Name: "128", Status: 0, onMask : 128, offMask : 128 })

	];

	Service.ToggleButton = function(button)
	{
		if(!button.Status)
		{
			button.Status = true;
			this.RelayStatus = button.onAction(this.RelayStatus);
		}
		else
		{
			button.Status = false;
			this.RelayStatus = button.offAction(this.RelayStatus);
		}
	}

	

	return Service;
}]);