#include <gui/containers/ContainerLOFFMatrix.hpp>


extern uint8_t Loff_StatP;
extern uint8_t Loff_StatN;

ContainerLOFFMatrix::ContainerLOFFMatrix()
{

}

void ContainerLOFFMatrix::initialize()
{
    ContainerLOFFMatrixBase::initialize();

    toggleButtonLA.setTouchable(false);
    toggleButtonLL.setTouchable(false);
    toggleButtonRA.setTouchable(false);
    toggleButtonRL.setTouchable(false);
    toggleButtonV1.setTouchable(false);
    toggleButtonV2.setTouchable(false);
    toggleButtonV3.setTouchable(false);
    toggleButtonV4.setTouchable(false);
    toggleButtonV5.setTouchable(false);
    toggleButtonV6.setTouchable(false);

}

void ContainerLOFFMatrix::UpdateMatrixStatusLOFF()

{
	if(Loff_StatP & 0x01 ){//V6
		toggleButtonV6.forceState(false);
	}else{
		toggleButtonV6.forceState(true);
	}
	if(Loff_StatP & 0x02){//LA
		toggleButtonLA.forceState(false);
	}else{
		toggleButtonLA.forceState(true);
	}
	if(Loff_StatP & 0x04){//LL
		toggleButtonLL.forceState(false);
	}else{
		toggleButtonLL.forceState(true);
	}
	if(Loff_StatP & 0x08){//V2
		toggleButtonV2.forceState(false);
	}else{
		toggleButtonV2.forceState(true);
	}
	if(Loff_StatP & 0x10){//V3
		toggleButtonV3.forceState(false);
	}else{
		toggleButtonV3.forceState(true);
	}
	if(Loff_StatP & 0x20){//V4
		toggleButtonV4.forceState(false);
	}else{
		toggleButtonV4.forceState(true);
	}
	if(Loff_StatP & 0x40){//V5
		toggleButtonV5.forceState(false);
	}else{
		toggleButtonV5.forceState(true);
	}
	if(Loff_StatP & 0x80){//V1
		toggleButtonV1.forceState(false);
	}else{
		toggleButtonV1.forceState(true);
	}
	if(Loff_StatN & 0x02){//RA
		toggleButtonRA.forceState(false);
	}else{
		toggleButtonRA.forceState(true);
	}

}


