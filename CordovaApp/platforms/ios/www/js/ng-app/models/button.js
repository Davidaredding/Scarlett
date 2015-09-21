app.factory('ButtonModel',[function(){
	function Button(buttonData){
		if(buttonData)
			this.setData(buttonData);
	};

	Button.prototype = {
		setData: function(buttonData){
			angular.extend(this,buttonData);
		},
		ID: 0,
		Name: "Un-Named",
		Status: 0,
		onMask: 0,
		offMask:0,
		onAction :function(RelaysState){return RelaysState |= this.onMask},
		offAction:function(RelaysState){return RelaysState ^= this.offMask}
	}
	return Button;
}]);