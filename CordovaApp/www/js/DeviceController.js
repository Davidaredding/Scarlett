app.Controller('DeviceDriver',['DeviceTypes','DeviceModel','DeviceGroup',
	function(DeviceTypes,Device,DeviceGroup){
		/* Hardware API
			http://picprojects.org.uk/projects/zcode/sadpktformat.htm
			one packet =  5 bytes (ignoring checksum)
			[Type][Address][Data][Data][Data]

			Byte 1 Message Type
				0 	= Command
				255 = Data

			Byte 2 Address
				0-127 		= specific channel
				128 		= Reserved, All Relays
				129 		= Reserved, All PWM
				130-255		= Groups

			Byte 3, 4 Data
				All Data is buffered.
				For message Type 255
					Color Data is read as
						RED   Blue  Green
						[0xFF][0xFF][0xFF]

					Relay Data is Read as LSB = High/Low
						Null  Null  Null Value
						[0000 0000][0000 0000][0000 0001]

					*Special*
					When Address is 129 Last byte is used to write data to all relays
					Most preformant way of toggling Relays
						[0000 0000][0000 0000][11111111]

					
			
			Commands:
				Address Byte = CommandID
					1 - WriteAllData() (writes buffered data to outputs)
					2 - WriteOnlyRelays()
					3 - WriteOnlyPwm()
					4 - AddToGroup(Address > 129, Address < 130, Address < 130) (Groups are cumulitive, Note non-nested groups)
					5 - ClearGroup

					 - ReadState(Address) (Returns Current (written) value of Address)
					 - ReadGroup(Address > 129) (Returns addresses in Address)
		*/

		
		var AvilableChannels = [
			//Halogen / Running Lights  represents a single hardware relay/transistor/mosfet
			{'Type':DeviceTypes.Binary},
			{'Type':DeviceTypes.Binary},
			{'Type':DeviceTypes.Binary},
			{'Type':DeviceTypes.Binary},
			{'Type':DeviceTypes.Binary},
			{'Type':DeviceTypes.Binary},
			{'Type':DeviceTypes.Binary},
			{'Type':DeviceTypes.Binary},

			{'Type':DeviceTypes.Color}, 
			{'Type':DeviceTypes.Color},
			{'Type':DeviceTypes.Color},
			{'Type':DeviceTypes.Color},
			{'Type':DeviceTypes.Color},
			{'Type':DeviceTypes.Color},
			{'Type':DeviceTypes.Color},
			{'Type':DeviceTypes.Color},
			{'Type':DeviceTypes.Color},
			{'Type':DeviceTypes.Color},
			{'Type':DeviceTypes.Color},
			{'Type':DeviceTypes.Color},
			{'Type':DeviceTypes.Color},
			{'Type':DeviceTypes.Color},
			{'Type':DeviceTypes.Color},
			{'Type':DeviceTypes.Color},
			//Extended TLC.
			{'Type':DeviceTypes.Color},
			{'Type':DeviceTypes.Color},
			{'Type':DeviceTypes.Color},
			{'Type':DeviceTypes.Color},
			{'Type':DeviceTypes.Color},

			//Remaining PWM Pin from the 4th TLC5940 and only "Dimmable" pin currently not implemented for use
			//{'Type': DeviceTypes.PWM, 'AddressMask': 0x2000000}

		]; 

}]);