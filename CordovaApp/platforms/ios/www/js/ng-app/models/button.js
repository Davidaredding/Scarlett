app.factory('ButtonModel',[function(){
	function Button(buttonData){
		if(buttonData)
			this.setData(buttonData);
	};

	Button.prototype = {
		setData: function(buttonData){
			angular.extend(this,buttonData);
		},
		Name: "Un-Named",
		Status: 0,
		onAction :function(){console.log("On");},
		offAction:function(){console.log("Off");}
	}
	return Button;
}]);