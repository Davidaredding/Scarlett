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

        if(navigator.userAgent.match(/(iPhone|iPod|iPad|Android)/))
            document.addEventListener('deviceready', this.onDeviceReady, false);
        else
            this.onDeviceReady();
    },
    // deviceready Event Handler
    //
    // The scope of 'this' is the event. In order to call the 'receivedEvent'
    // function, we must explicitly call 'app.receivedEvent(...);'
    onDeviceReady: function() {
        angular.element(document).ready(function(){
            angular.bootstrap(document,['jeeputer']);
        });

        if((typeof ble === "undefined") || navigator.platform.match("Simulator"))
            createMockBle();
    },
};



var app = angular.module('jeeputer',['ngRoute'])
app.run(function(){FastClick.attach(document.body)});

app.config(function($routeProvider, $compileProvider){
    $compileProvider.aHrefSanitizationWhitelist(/^\s*(https?|ftp|mailto|file|tel):/);
    $routeProvider
        .when('/home',{
            templateUrl:'views/home.html'
        })
        .when('/button/:id',{
            templateUrl: 'views/button.html',
        })
        .when('/button',{
            templateUrl: 'views/Buttons.html'
        })
        
        .otherwise({
            redirectTo:'/home'
        })

});

app.controller('AppController',['$scope', function($scope){
    $scope.state = true;
}]);










