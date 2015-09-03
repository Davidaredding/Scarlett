
app.controller('app',function($scope,$BlueTooth,$interval, DeviceModel)
{
    $scope.BlueToothDevices = [];
    $scope.ConnectedDevice_id = "";
    $scope.virtual_switches = 
    [
        {
            GroupName: 'Fog Lights',
            Devices: 
            [
                new DeviceModel({Name: "Driver Fog-light", Address:0x01}),
                new DeviceModel({Name: "Passenger Fog-light", Address:0x02})
            ],
            IconDescriptor: "Fog-Light"
        },
        {
            GroupName: 'Spot Lights',
            Devices: 
            [
                new DeviceModel({Name: "Driver Spot-light", Address:0x03}),
                new DeviceModel({Name: "Passenger Spot-light", Address:0x04})
            ],
            IconDescriptor: "Spot-Light"
        }
    ];


    console.log('loaded');


    $scope.setStatus = function(index){
        $scope.status[index] = !$scope.status[index];
        $scope.processStatus();
    }

    $scope.processStatus = function(){
    	 $BlueTooth.Write($scope.ConnectedDevice_id, $scope.status)
            .then(function(){},function(){});
    }

    $scope.FindDevices = function(){
    	$BlueTooth.ScanForPeripherals()
    		.then(function(devices){
    			$scope.BlueToothDevices = devices;
    		});
    }

    $scope.Connect = function(id){
    	
    	$BlueTooth.ConnectToPeripheral(id)
    		.then(
    		function(){//resolved
    			alert("Resolved");
    		},
    		function(error){//error
    			alert("Error: " + error);
    		},
    		function(p){//update
    			console.log("Connected!");
    			$scope.ConnectedDevice_id = p.id;
    		});
    }

    $scope.remote_animate = function(){
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

