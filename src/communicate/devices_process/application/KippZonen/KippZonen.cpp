#include "KippZonen.h"

int KippZonen::id_count=1;

bool KippZonen::load(){
	status = false;
	if(readHreg(0, (uint16_t*)&d.DevType, 10) == 0){
		status = true;
	}
	if(readHreg(26, (uint16_t*)&d.Error_code, 2) == 0){
		status = true;
	}
	if(readHreg(41, (uint16_t*)&d.Batch_number, 5) == 0){
		status = true;
	}
	if(readHreg(332, (uint16_t*)&d.Calibration_date, 2) == 0){
		status = true;
	}
	return status;
}