app.service("$Buttons",['ButtonModel',function(button){
	var Service = {};
	
	Service.RelayStatus = 0;

	Service.buttons = 
	[
		new button({Name: "Fog Lights", Status: 0, onAction :function(Relays){return Relays |= 192;}, offAction:function(Relays){return Relays ^= 192; }}),
        new button({Name: "Spot Lights", Status: 0, onAction :function(Relays){return Relays |= 48;}, offAction:function(Relays){return Relays ^= 48;}})
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

	Service.AddButton = function(button)
	{

	}

	return Service;
}]);