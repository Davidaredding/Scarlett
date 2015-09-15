app.controller('homeController',['$scope','$Bluetooth','$interval',function($scope,$Bluetooth,$interval)
{
    $scope.BlueToothDevices = [];
    $scope.ConnectedDevice_id = "";
    $scope.status = [0,0,0,0,0,0,0,0];

    //Status
    $scope.scanning = false;
    $scope.connecting = false;

    $scope.$watchCollection("status",function(n,o){
        if(n==o)
            return;
        $scope.processStatus();
    });

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
                $scope.scanning = false;
            });
    }

    $scope.Connect = function(id){
        $Bluetooth.ConnectToPeripheral(id)
            .then(
            function(){//resolved
                $scope.ConnectedDevice_id = "";
            },
            function(error){//error
                $scope.ConnectedDevice_id = "";
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