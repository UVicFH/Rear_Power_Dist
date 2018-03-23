uint8_t port_input(uint8_t CAN_POS, uint8_t PIN_Position, uint8_t* bytes_array, uint8_t bytes_index){

	//Shifting bytes by CAN position 
	uint8_t shifted_bytes_array = bytes_array[bytes_index] >> CAN_POS;


	//And to check for oneness in LSB spot
	shifted_bytes_array = shifted_bytes_array & BIT_1_MASK;

	//Shift message to pin position
	shifted_bytes_array = shifted_bytes_array << PIN_Position;

	
	return shifted_bytes_array;
}


uint8_t port_output(uint8_t input_byte, uint8_t PORT, uint8_t PIN_Position){

	uint8_t output_byte;

	//shift a zero into the pin position
	uint8_t shfited_zero = !1U<<PIN_Position;

	output_byte = shfited_zero ^ input_byte;

	return output_byte ^ PORT;
}
