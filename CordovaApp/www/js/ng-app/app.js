var cordovaApp = {
    // Application Constructor
    initialize: function() {
        this.bindEvents();

    },
    // Bind Event Listeners
    //
    // Bind any events that are required on startup. Common events are:
    // 'load', 'deviceready', 'offline', and 'online'.
    bindEvents: function() {
        document.addEventListener('deviceready', this.onDeviceReady, false);
    },
    // deviceready Event Handler
    //
    // The scope of 'this' is the event. In order to call the 'receivedEvent'
    // function, we must explicitly call 'app.receivedEvent(...);'
    onDeviceReady: function() {
        cordovaApp.receivedEvent('deviceready');

        },
    // Update DOM on a Received Event
    receivedEvent: function(id) {
        var parentElement = document.getElementById(id);
        
        console.log('Received Event: ' + id);
    }

    
};

cordovaApp.initialize();

var app = angular.module('jeeputer',['ngRoute'])
app.run(function(){FastClick.attach(document.body)});

app.config(function($routeProvider){
    $routeProvider
        .when('/',{
            templateUrl:'views/home.html',
            controller: 'homeController'
        })
        .when('/settings/devices',{
            templateUrl: 'views/devices.html'
        })
        .when('/settings/VirtualDevices',
        {
            templateUrl: 'views/virtualDevices.html'
        })
        .when('/settings/VirtualDevices/:vdId',
        {
            templateUrl: 'views/virtualDevice.html'
        })
        .otherwise({
            redirectTo:'/'
        })
});

app.controller('AppController',[AppController]);
function AppController(){
    
}









