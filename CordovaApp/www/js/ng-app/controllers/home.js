app.controller('homeController',['$scope','$Bluetooth','$Buttons','$interval',function($scope,$Bluetooth,$Buttons,$interval)
{
    $scope.BlueToothDevices = $Bluetooth.devices;
    $scope.buttons = $Buttons.buttons;
    $scope.connected = $Bluetooth.connectedDevice;
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
        $Bluetooth.Write(1)
            .then(function(){
                $Bluetooth.Write($Buttons.RelayStatus);
            });

    }

    $scope.FindDevices = function(){
        
        $scope.scanning = true;
        $Bluetooth.ScanForPeripherals()
            .then(function(){
                $scope.scanning = false;
                $scope.BlueToothDevices = $Bluetooth.devices;
            });
    }

    $scope.Connect = function(id){
        $Bluetooth.ConnectToPeripheral(id)
            .then(function(){},
            function(error){//error
                conosle.log(error);
                $scope.connected = undefined;
                $scope.devices = [];  
            },
            function(p){//update
                console.log("Connected to " + p);
                $scope.connected = p;
            });
    }
}]);