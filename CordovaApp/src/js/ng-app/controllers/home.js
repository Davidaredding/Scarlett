app.controller('homeController',function(BluetoothService,ButtonService)
{
    this.BTService = BluetoothService;
    this.ButtonService = ButtonService;
    this.connected = this.BTService.connectedDevice;
    //Status
    console.log('running!');
    this.scanning = false;
    this.connecting = false;

    this.statusChanged = function(dev_index){
        var dev = this.ButtonService.buttons[dev_index];
        dev.Status = !dev.Status;
        this.ButtonService.ToggleButton(dev);

        this.writeStatus();
    }

    this.writeStatus = function(){
        var self = this;
        this.BTService.Write(1)
            .then(function(){
                self.BTService.Write(self.ButtonService.RelayStatus);
            });

    }

    this.FindDevices = function(){
        this.BTService.ScanForPeripherals()
            .then(function(){});
    }

    this.Connect = function(id){
        this.BTService.ConnectToPeripheral(id)
            .then(function(){},
            function(error){//error
                conosle.log(error);
                this.connected = undefined;
                this.devices = [];  
            },
            function(p){//update
                console.log("Connected to " + p);
                this.connected = p;
            });
    }
});