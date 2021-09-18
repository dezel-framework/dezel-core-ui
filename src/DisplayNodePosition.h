#ifndef DisplayNodePosition_h
#define DisplayNodePosition_h

#include "DisplayBase.h"

namespace Dezel {

class DisplayNodePosition {

public:

	DisplayNodePositionType type = kDisplayNodePositionTypeAuto;
	DisplayNodePositionUnit unit = kDisplayNodePositionUnitNone;

	double length = 0;
	double min = ABS_DBL_MIN;
	double max = ABS_DBL_MAX;

	bool equals(DisplayNodePositionType type, DisplayNodePositionUnit unit, double length) {
		return (
			this->type == type &&
			this->unit == unit &&
			this->length == length
		);
	}
};

}

#endif
