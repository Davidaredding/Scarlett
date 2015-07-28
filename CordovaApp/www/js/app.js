var app = angular.module('jeeputer',[]);
app.run(function(){
	FastClick.attach(document.body);
});

app.controller('app',function($scope,$BlueTooth, $interval)
{
    $scope.status = [1,1,1,1,1,1,1,1];
    $scope.BlueToothDevices = [];
    $scope.ConnectedDevice_id = "";
    $scope.setStatus = function(index)
    {
        $scope.status[index] = !$scope.status[index];
        $scope.processStatus();
       
    }

    $scope.processStatus = function()
    {
    	 $BlueTooth.Write($scope.ConnectedDevice_id, $scope.status)
        .then(function(){})
        .fail(function(){});
    }

    $scope.FindDevices = function()
    {
    	$BlueTooth.ScanForPeripherals()
    		.then(function(devices){
    			$scope.BlueToothDevices = devices;
    		});
    }

    $scope.Connect = function(id)
    {
    	
    	$BlueTooth.ConnectToPeripheral(id)
    		.then(function(){//resolved
    			alert("Resolved");

    		},
    		function(error){//error
    			alert("Error: " + error);
    		},
    		function(p){//update
    			$scope.ConnectedDevice_id = p.id;
    			alert("Connected!");
    		})
    		;
    }

    $scope.remote_animate = function()
    {
    	var index = 7;
    	$scope.status = [1,1,1,1,1,1,1,1];
    	$interval(
    		function(){

    			$scope.status[index--] = 1;
    			if(index<0)
    				index = 7;
    			$scope.status[index] = 0;
				$scope.processStatus();
    		},
    	100);
    }
});

app.factory("$BlueTooth",function($q){
	return {
		ScanForPeripherals: function()
		{
			var deferred = $q.defer();
			var devices = [];
			
			setTimeout(ble.stopScan,
				3000,
				function(){deferred.resolve(devices);},
				function(){});

			ble.startScan(['FFE0'],
				function(bt_device){
					var d = {
						Name:bt_device.name,
						RSSI:bt_device.rssi,
						ID:bt_device.id
					};
					devices.push(d);
					
				}, 
				function(error){
					console.error(error);
					deferred.fail(error);
				}
			);
			return deferred.promise;
		},

		ConnectToPeripheral: function(BT_dev_id)
		{
			var deferred = $q.defer()
			alert("Attempting to connect to " + BT_dev_id);
			ble.connect(BT_dev_id,
					function(peripheral)
					{
						deferred.notify(peripheral);
					},
					function(error)
					{
						alert("Fail!");
						deferred.reject(error);
					}
				);

			return deferred.promise;
		},

		DisconnectFromPeripheral: function(BT_dev_id)
		{

		},

		Write: function(device_id, value){
			var deferred = $q.defer();
			var a = new Uint8Array(1);
			a[0] = 0
			for(i=0;i<value.length;i++)
			{
				a[0] = (a[0]<<1)|value[i];
			}
			

			ble.writeCommand(
				device_id, 
				'FFE0', 
				'FFE1',
				a.buffer,
				function(a){deferred.resolve();},
				function(a){deferred.fail();});

			return deferred.promise;
		}
	};

});