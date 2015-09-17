app.controller('homeController',['$scope','$Bluetooth','$Buttons','$interval',function($scope,$Bluetooth,$Buttons,$interval)
{
    $scope.BlueToothDevices = [];
    $scope.ConnectedDevice_id = "";
    $scope.buttons = $Buttons.buttons;

    //Status
    console.log('running!');
    $scope.scanning = false;
    $scope.connecting = false;

    $scope.statusChanged = function(dev_index){
        var dev = $scope.buttons[dev_index];
        dev.Status = !dev.Status;
        $Buttons.ToggleButton(dev);

        $scope.writeStatus();
    }

    $scope.writeStatus = function(){
        if($scope.ConnectedDevice_id != "")
        {
            console.log($scope.relayStatus);
            $Bluetooth.Write($scope.ConnectedDevice_id, 1)
                .then(function(){
                    $Bluetooth.Write($scope.ConnectedDevice_id, $Buttons.RelayStatus);
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
}]);