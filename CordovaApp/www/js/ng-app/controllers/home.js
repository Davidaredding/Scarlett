app.controller('homeController',['$scope','$Bluetooth','$interval',function($scope,$Bluetooth,$interval)
{
    $scope.BlueToothDevices = [];
    $scope.relayStatus = 0;
    $scope.ConnectedDevice_id = "";
    $scope.buttonStatus = 
    [
        {Name: "Fog Lights", Status: 0, onAction :function(){$scope.relayStatus |= 192;}, offAction:function(){$scope.relayStatus ^= 192; }},
        {Name: "Spot Lights", Status: 0, onAction :function(){$scope.relayStatus |= 48;}, offAction:function(){$scope.relayStatus ^= 48;}},
        {Name: "3", Status: 0, onAction :function(){}, offAction:function(){}},
        {Name: "4", Status: 0, onAction :function(){}, offAction:function(){}}
        
    ];

    //Status
    console.log('running!');
    $scope.scanning = false;
    $scope.connecting = false;

    //statusChanged is called AFTER the status has changed.
    $scope.statusChanged = function(dev){
        if(!dev.Status)
            dev.offAction();
        else
            dev.onAction();

        $scope.writeStatus();
    }

    $scope.writeStatus = function(){
        if($scope.ConnectedDevice_id != "")
        {
            console.log($scope.relayStatus);
            $Bluetooth.Write($scope.ConnectedDevice_id, 1)
                .then(function(){
                    $Bluetooth.Write($scope.ConnectedDevice_id, $scope.relayStatus);
                });
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