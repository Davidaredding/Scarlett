app.controller('buttonController',function($scope,$Buttons,$route,$routeParams, $timeout, $location, $Bluetooth){

	$scope.buttons = $Buttons.buttons;

	if($route.current.params.id){
		$scope.currentButton = $Buttons.buttons[$route.current.params.id];
		$scope.buttonName = $scope.currentButton.Name;
		$scope.onMask = $scope.currentButton.onMask;
	}

	$scope.IsActive=function(mask){
		return $scope.onMask&mask;
	}

	$scope.ToggleMask = function(mask){
		$scope.onMask ^= mask;
	}

	$scope.Test = function(){
		$Bluetooth.Write(1)
		.then(function(){
			$Bluetooth.Write($scope.onMask);
		});
	}

	$scope.Save = function(){
		$scope.currentButton.onMask = $scope.onMask;
		$scope.currentButton.Name = $scope.buttonName;
		$location.path('/Home');
	}

	$scope.EditButton = function(buttonId){
		$location.path('button/' + buttonId);

	}


});