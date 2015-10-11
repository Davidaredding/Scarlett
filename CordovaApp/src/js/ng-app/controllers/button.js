app.controller('buttonController',function(ButtonService,$route,$routeParams, BluetoothService){

	this.ButtonService = ButtonService

	if($route.current.params.id){
		this.currentButton = this.ButtonService.buttons[$route.current.params.id];
		this.buttonName = this.currentButton.Name;
		this.onMask = this.currentButton.onMask;
	}

	this.IsActive=function(mask){
		return this.onMask&mask;
	}

	this.ToggleMask = function(mask){
		this.onMask ^= mask;
	}

	this.Test = function(){
		var self = this;
		BluetoothService.Write(1)
		.then(function(){
			BluetoothService.Write(self.onMask);
		});
	}

	this.Save = function(){
		this.currentButton.onMask = this.onMask;
		this.currentButton.Name = this.buttonName;
		$location.path('/Home');
	}

	this.EditButton = function(buttonId){
		$location.path('button/' + buttonId);

	}


});