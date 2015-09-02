
app.controller('app',function($scope,$BlueTooth,$interval)
{
    $scope.status = [1,1,1,1,1,1,1,1];
    $scope.BlueToothDevices = [];
    $scope.ConnectedDevice_id = "";
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

