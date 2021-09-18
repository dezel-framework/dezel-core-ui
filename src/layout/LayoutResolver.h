#ifndef Resolver_h
#define Resolver_h

#include "RelativeLayoutResolver.h"
#include "AbsoluteLayoutResolver.h"

#include <cmath>

namespace Dezel {
	class DisplayNode;
	class DisplayNodeFrame;
}

namespace Dezel {
namespace Layout {

class LayoutResolver {

private:

	DisplayNode* node;
	RelativeLayoutResolver relativeLayoutResolver;
	AbsoluteLayoutResolver absoluteLayoutResolver;

public:

	LayoutResolver(DisplayNode* node);

	double getExtentTop() const {
		return this->relativeLayoutResolver.extentTop;
	}

	double getExtentLeft() const {
		return this->relativeLayoutResolver.extentLeft;
	}

	double getExtentRight() const {
		return this->relativeLayoutResolver.extentRight;
	}

	double getExtentBottom() const {
		return this->relativeLayoutResolver.extentBottom;
	}

	double getExtentWidth() const {
		return this->getExtentRight() - this->getExtentLeft();
	}

	double getExtentHeight() const {
		return this->getExtentBottom() - this->getExtentTop();
	}

	void measureAbsoluteNode(DisplayNode* node, double &w, double &h) {
		this->absoluteLayoutResolver.measure(node, w, h);
	}

	void measureRelativeNode(DisplayNode* node, double &w, double &h, double &remainingW, double &remainingH, double &remainder) {
		this->relativeLayoutResolver.measure(node, w, h, remainingW, remainingH, remainder);
	}

	void prepare();
	void resolve();
};

static inline double round(double value, double scale) {
	return scale > 1 ? (std::round((value) * scale) / scale) : std::round(value);
}

static inline double round(double value, double scale, double &carry) {
	double number = value;
	value = value + carry;
	value = round(value, scale);
	carry = (number + carry) - value;
	return value;
}

static inline double scale(double value, double ratio) {
	return (value) / 100 * (ratio);
}

static inline double clamp(double value, double min, double max) {
	if (value < min) value = min;
	if (value > max) value = max;
	return value;
}

}
}

#endif
