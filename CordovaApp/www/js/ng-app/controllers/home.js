app.controller('homeController',['$scope','$Bluetooth',function($scope,$Bluetooth,$interval)
{
    $scope.BlueToothDevices = [];
    $scope.ConnectedDevice_id = "";
    $scope.scanning = false;
    $scope.status = [0,0,0,0,0,0,0,0];

    $scope.setStatus = function(index){
        $scope.status[index] = !$scope.status[index];
        $scope.processStatus();
    }

    $scope.processStatus = function(){
        if($scope.ConnectedDevice_id != "")
        {
         $Bluetooth.Write($scope.ConnectedDevice_id, $scope.status)
            .then(function(){},function(){});
        }
        else
        {
            console.log("no connected device");
        }
    }

    $scope.FindDevices = function(){
        $scope.scanning = true;
        $Bluetooth.ScanForPeripherals()
            .then(function(devices){
                $scope.BlueToothDevices = devices;
            });
    }

    $scope.Connect = function(id){
        $Bluetooth.ConnectToPeripheral(id)
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
}]);