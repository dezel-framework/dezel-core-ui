#ifndef DisplayNodeContentPosition_h
#define DisplayNodeContentPosition_h

#include "DisplayBase.h"

namespace Dezel {

class DisplayNodeContentPosition {

public:

	DisplayNodeContentPositionType type = kDisplayNodeContentPositionTypeLength;
	DisplayNodeContentPositionUnit unit = kDisplayNodeContentPositionUnitNone;

	double length = 0;

	bool equals(DisplayNodeContentPositionType type, DisplayNodeContentPositionUnit unit, double length) {
		return (
			this->type == type &&
			this->unit == unit &&
			this->length == length
		);
	}
};

}

#endif
