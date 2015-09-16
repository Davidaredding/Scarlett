app.factory('DeviceTypes',[function(){
	function DeviceTypes(){
		var types = {
			Binary: {desc: 'On/Off Device, typically run by relay.  Possible states are 0 and 1', name: 'On/Off', Code: 0x1},
			Color: {desc: 'Color Device;  RGB LEDs run via a PWM signal.  Possible states range from #00 00 00 to #FF FF FF', name:'Color', Code: 0x2}
		};
		return types;
	}

	return DeviceTypes();
}]);


app.factory('DeviceModel',['DeviceTypes',function(DeviceTypes){
	function DeviceModel(deviceData){
		if(deviceData)
			this.setData(deviceData);
	};

	DeviceModel.prototype = {
		setData: function(deviceData){
			angular.extend(this,deviceData);
		},
		Name: "Un-Named",
		Type: DeviceTypes.Binary,
		State: 0,
		Address: 0
	}
	return DeviceModel;
}]);

app.factory('DeviceGroup',[function(){

}]);

