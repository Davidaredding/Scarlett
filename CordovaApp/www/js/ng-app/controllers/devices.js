app.controller('devices',function($scope,$Firmware)
{
	$scope.relay_devices = $Firmware.relay_devices;
	$scope.virtual_devices = [];

});