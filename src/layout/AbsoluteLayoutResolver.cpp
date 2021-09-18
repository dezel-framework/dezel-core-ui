#include "AbsoluteLayoutResolver.h"
#include "LayoutResolver.h"
#include "Display.h"
#include "DisplayNode.h"
#include "InvalidOperationException.h"

#include <iostream>
#include <cstdlib>

namespace Dezel {
namespace Layout {

using std::min;
using std::max;

AbsoluteLayoutResolver::AbsoluteLayoutResolver(DisplayNode* node)
{
	this->node = node;
}

//------------------------------------------------------------------------------
// MARK: Private API
//------------------------------------------------------------------------------

bool
AbsoluteLayoutResolver::hasInvalidSize(DisplayNode* child)
{
	if (child->invalidSize) {
		return true;
	}

	if (child->width.unit == kDisplayNodeSizeUnitPX &&
		child->height.unit == kDisplayNodeSizeUnitPX) {
		return false;
	}

	const auto parent = child->parent;

	if (parent->measuredContentWidth != parent->resolvedContentWidth || child->hasNewParent()) {
		if (child->width.unit == kDisplayNodeSizeUnitPC ||
			child->width.type == kDisplayNodeSizeTypeFill ||
			child->right.type != kDisplayNodePositionTypeAuto) {
			return true;
		}
	}

	if (parent->measuredContentHeight != parent->resolvedContentHeight || child->hasNewParent()) {
		if (child->height.unit == kDisplayNodeSizeUnitPC ||
			child->height.type == kDisplayNodeSizeTypeFill ||
			child->bottom.type != kDisplayNodePositionTypeAuto) {
			return true;
		}
	}

	if (parent->measuredInnerWidth != parent->resolvedInnerWidth || child->hasNewParent()) {
		if (child->width.unit == kDisplayNodeSizeUnitPW ||
			child->height.unit == kDisplayNodeSizeUnitPW) {
			return true;
		}
	}

	if (parent->measuredInnerHeight != parent->resolvedInnerHeight || child->hasNewParent()) {
		if (child->width.unit == kDisplayNodeSizeUnitPH ||
			child->height.unit == kDisplayNodeSizeUnitPH) {
			return true;
		}
	}

	if (parent->measuredContentWidth != parent->resolvedContentWidth || child->hasNewParent()) {
		if (child->width.unit == kDisplayNodeSizeUnitCW ||
			child->height.unit == kDisplayNodeSizeUnitCW) {
			return true;
		}
	}

	if (parent->measuredContentHeight != parent->resolvedContentHeight || child->hasNewParent()) {
		if (child->width.unit == kDisplayNodeSizeUnitCH ||
			child->height.unit == kDisplayNodeSizeUnitCH) {
			return true;
		}
	}

	if (child->display->hasNewViewportWidth()) {
		if (child->width.unit == kDisplayNodeSizeUnitVW ||
			child->height.unit == kDisplayNodeSizeUnitVH) {
			return true;
		}
	}

	if (child->display->hasViewportHeightChanged()) {
		if (child->width.unit == kDisplayNodeSizeUnitVH ||
			child->height.unit == kDisplayNodeSizeUnitVH) {
			return true;
		}
	}
	
	return false;
}

bool
AbsoluteLayoutResolver::hasInvalidPosition(DisplayNode* child)
{
	if (child->invalidPosition) {
		return true;
	}

	if (child->top.unit == kDisplayNodePositionUnitPX &&
		child->left.unit == kDisplayNodePositionUnitPX &&
		child->right.type == kDisplayNodePositionTypeAuto &&
		child->bottom.type == kDisplayNodePositionTypeAuto) {
		return false;
	}

	if ((child->anchorTop.length > 0.0 && child->measuredHeight != child->resolvedHeight) ||
		(child->anchorLeft.length > 0.0 && child->measuredWidth != child->resolvedWidth)) {
		return true;
	}

	const auto parent = child->parent;

	if (parent->measuredContentWidth != parent->resolvedContentWidth || child->hasNewParent()) {
		if (child->left.unit == kDisplayNodePositionUnitPC ||
			child->right.unit == kDisplayNodePositionUnitPC) {
			return true;
		}
	}

	if (parent->measuredContentHeight != parent->resolvedContentHeight || child->hasNewParent()) {
		if (child->top.unit == kDisplayNodePositionUnitPC ||
			child->bottom.unit == kDisplayNodePositionUnitPC) {
			return true;
		}
	}

	if (parent->measuredInnerWidth != parent->resolvedInnerWidth || child->hasNewParent()) {
		if (child->top.unit == kDisplayNodePositionUnitPW ||
			child->left.unit == kDisplayNodePositionUnitPW ||
			child->right.unit == kDisplayNodePositionUnitPW ||
			child->bottom.unit == kDisplayNodePositionUnitPW) {
			return true;
		}
	}

	if (parent->measuredInnerHeight != parent->resolvedInnerHeight || child->hasNewParent()) {
		if (child->top.unit == kDisplayNodePositionUnitPH ||
			child->left.unit == kDisplayNodePositionUnitPH ||
			child->right.unit == kDisplayNodePositionUnitPH ||
			child->bottom.unit == kDisplayNodePositionUnitPH) {
			return true;
		}
	}

	if (parent->measuredContentWidth != parent->resolvedContentWidth || child->hasNewParent()) {
		if (child->top.unit == kDisplayNodePositionUnitCW ||
			child->left.unit == kDisplayNodePositionUnitCW ||
			child->right.unit == kDisplayNodePositionUnitCW ||
			child->bottom.unit == kDisplayNodePositionUnitCW) {
			return true;
		}
	}

	if (parent->measuredContentHeight != parent->resolvedContentHeight || child->hasNewParent()) {
		if (child->top.unit == kDisplayNodePositionUnitCH ||
			child->left.unit == kDisplayNodePositionUnitCH ||
			child->right.unit == kDisplayNodePositionUnitCH ||
			child->bottom.unit == kDisplayNodePositionUnitCH) {
			return true;
		}
	}

	if (child->display->hasNewViewportWidth()) {
		if (child->top.unit == kDisplayNodePositionUnitVW ||
			child->left.unit == kDisplayNodePositionUnitVW ||
			child->right.unit == kDisplayNodePositionUnitVW ||
			child->bottom.unit == kDisplayNodePositionUnitVW) {
			return true;
		}
	}

	if (child->display->hasNewViewportWidth()) {
		if (child->top.unit == kDisplayNodePositionUnitVH ||
			child->left.unit == kDisplayNodePositionUnitVH ||
			child->right.unit == kDisplayNodePositionUnitVH ||
			child->bottom.unit == kDisplayNodePositionUnitVH) {
			return true;
		}
	}

	return false;
}

double
AbsoluteLayoutResolver::measureTop(DisplayNode* child)
{
	const auto type = child->top.type;
	const auto unit = child->top.unit;

	double value = child->top.length;

	if (type == kDisplayNodePositionTypeLength) {

		switch (unit) {

			case kDisplayNodePositionUnitPC: value = scale(value, child->parent->measuredContentHeight); break;
			case kDisplayNodePositionUnitPW: value = scale(value, child->parent->measuredInnerWidth); break;
			case kDisplayNodePositionUnitPH: value = scale(value, child->parent->measuredInnerHeight); break;
			case kDisplayNodePositionUnitCW: value = scale(value, child->parent->measuredContentWidth); break;
			case kDisplayNodePositionUnitCH: value = scale(value, child->parent->measuredContentHeight); break;
			case kDisplayNodePositionUnitVW: value = scale(value, child->display->viewportWidth); break;
			case kDisplayNodePositionUnitVH: value = scale(value, child->display->viewportHeight); break;
			default: break;

		}
	}

	value = clamp(
		value,
		child->top.min,
		child->top.max
	);

	value += child->parent->measuredContentTop;

	return round(value, child->display->scale);;
}

double
AbsoluteLayoutResolver::measureLeft(DisplayNode* child)
{
	const auto type = child->left.type;
	const auto unit = child->left.unit;

	double value = child->left.length;

	if (type == kDisplayNodePositionTypeLength) {

		switch (unit) {

			case kDisplayNodePositionUnitPC: value = scale(value, child->parent->measuredContentWidth); break;
			case kDisplayNodePositionUnitPW: value = scale(value, child->parent->measuredInnerWidth); break;
			case kDisplayNodePositionUnitPH: value = scale(value, child->parent->measuredInnerHeight); break;
			case kDisplayNodePositionUnitCW: value = scale(value, child->parent->measuredContentWidth); break;
			case kDisplayNodePositionUnitCH: value = scale(value, child->parent->measuredContentHeight); break;
			case kDisplayNodePositionUnitVW: value = scale(value, child->display->viewportWidth); break;
			case kDisplayNodePositionUnitVH: value = scale(value, child->display->viewportHeight); break;
			default: break;

		}
	}

	value = clamp(
		value,
		child->left.min,
		child->left.max
	);

	value += child->parent->measuredContentLeft;

	return round(value, child->display->scale);
}

double
AbsoluteLayoutResolver::measureRight(DisplayNode* child)
{
	const auto type = child->right.type;
	const auto unit = child->right.unit;

	double value = child->right.length;

	if (type == kDisplayNodePositionTypeLength) {

		switch (unit) {

			case kDisplayNodePositionUnitPC: value = scale(value, child->parent->measuredContentWidth); break;
			case kDisplayNodePositionUnitPW: value = scale(value, child->parent->measuredInnerWidth); break;
			case kDisplayNodePositionUnitPH: value = scale(value, child->parent->measuredInnerHeight); break;
			case kDisplayNodePositionUnitCW: value = scale(value, child->parent->measuredContentWidth); break;
			case kDisplayNodePositionUnitCH: value = scale(value, child->parent->measuredContentHeight); break;
			case kDisplayNodePositionUnitVW: value = scale(value, child->display->viewportWidth); break;
			case kDisplayNodePositionUnitVH: value = scale(value, child->display->viewportHeight); break;
			default: break;

		}
	}

	value = clamp(
		value,
		child->right.min,
		child->right.max
	);

	value += child->parent->measuredContentLeft;

	return round(value, child->display->scale);
}

double
AbsoluteLayoutResolver::measureBottom(DisplayNode* child)
{
	const auto type = child->bottom.type;
	const auto unit = child->bottom.unit;

	double value = child->bottom.length;

	if (type == kDisplayNodePositionTypeLength) {

		switch (unit) {

			case kDisplayNodePositionUnitPC: value = scale(value, child->parent->measuredContentHeight); break;
			case kDisplayNodePositionUnitPW: value = scale(value, child->parent->measuredInnerWidth); break;
			case kDisplayNodePositionUnitPH: value = scale(value, child->parent->measuredInnerHeight); break;
			case kDisplayNodePositionUnitCW: value = scale(value, child->parent->measuredContentWidth); break;
			case kDisplayNodePositionUnitCH: value = scale(value, child->parent->measuredContentHeight); break;
			case kDisplayNodePositionUnitVW: value = scale(value, child->display->viewportWidth); break;
			case kDisplayNodePositionUnitVH: value = scale(value, child->display->viewportHeight); break;
			default: break;

		}
	}

	value = clamp(
		value,
		child->bottom.min,
		child->bottom.max
	);

	value += child->parent->measuredContentTop;

	return round(value, child->display->scale);
}

double
AbsoluteLayoutResolver::measureWidth(DisplayNode* child)
{
	const auto marginL = child->measuredMarginLeft;
	const auto marginR = child->measuredMarginRight;

	const auto type = child->width.type;
	const auto unit = child->width.unit;

	const auto l = child->left.type;
	const auto r = child->right.type;

	double value = child->width.length;
	double nodeL = 0;
	double nodeR = 0;

	if (l != kDisplayNodePositionTypeAuto) nodeL = this->measureLeft(child);
	if (r != kDisplayNodePositionTypeAuto) nodeR = this->measureRight(child);

	if (l != kDisplayNodePositionTypeAuto &&
		r != kDisplayNodePositionTypeAuto) {

		value = child->parent->measuredContentWidth - (nodeL + marginL) - (nodeR + marginR);

	} else if (type == kDisplayNodeSizeTypeFill) {

		 if (l == kDisplayNodePositionTypeAuto &&
		 	 r == kDisplayNodePositionTypeAuto) {

			value = child->parent->measuredContentWidth - marginL - marginR;

		} else if (l != kDisplayNodePositionTypeAuto) {

			value = child->parent->measuredContentWidth - (nodeL + marginL);

		} else if (r != kDisplayNodePositionTypeAuto) {

			value = child->parent->measuredContentWidth - (nodeR + marginR);

		}

	} else if (type == kDisplayNodeSizeTypeWrap) {

		throw InvalidOperationException("This method does not measuring wrapped node.");

	} else if (type == kDisplayNodeSizeTypeLength) {

		switch (unit) {
			case kDisplayNodeSizeUnitPC: value = scale(value, child->parent->measuredContentWidth); break;
			case kDisplayNodeSizeUnitPW: value = scale(value, child->parent->measuredInnerWidth); break;
			case kDisplayNodeSizeUnitPH: value = scale(value, child->parent->measuredInnerHeight); break;
			case kDisplayNodeSizeUnitCW: value = scale(value, child->parent->measuredContentWidth); break;
			case kDisplayNodeSizeUnitCH: value = scale(value, child->parent->measuredContentHeight); break;
			case kDisplayNodeSizeUnitVW: value = scale(value, child->display->viewportWidth); break;
			case kDisplayNodeSizeUnitVH: value = scale(value, child->display->viewportHeight); break;
			default: break;
		}

	}

	value = clamp(
		value,
		child->width.min,
		child->width.max
	);

	return round(value, child->display->scale);
}

double
AbsoluteLayoutResolver::measureHeight(DisplayNode* child)
{
	const auto marginT = child->measuredMarginTop;
	const auto marginB = child->measuredMarginBottom;

	const auto type = child->height.type;
	const auto unit = child->height.unit;

	const auto t = child->top.type;
	const auto b = child->bottom.type;

	double nodeT = 0;
	double nodeB = 0;
	double value = child->height.length;

	if (t != kDisplayNodePositionTypeAuto) nodeT = this->measureTop(child);
	if (b != kDisplayNodePositionTypeAuto) nodeB = this->measureBottom(child);

	if (t != kDisplayNodePositionTypeAuto &&
		b != kDisplayNodePositionTypeAuto) {

		value = child->parent->measuredContentHeight - (nodeT + marginT) - (nodeB + marginB);

	} else if (type == kDisplayNodeSizeTypeFill) {

		if (t == kDisplayNodePositionTypeAuto &&
			b == kDisplayNodePositionTypeAuto) {

			value = child->parent->measuredContentHeight - marginT - marginB;

		} else if (t != kDisplayNodePositionTypeAuto) {

			value = child->parent->measuredContentHeight - (nodeT + marginT);

		} else if (b != kDisplayNodePositionTypeAuto) {

			value = child->parent->measuredContentHeight - (nodeB + marginB);
		}

	} else if (type == kDisplayNodeSizeTypeWrap) {

		throw InvalidOperationException("This method does not measuring wrapped node.");

	} else if (type == kDisplayNodeSizeTypeLength) {

		switch (unit) {
			case kDisplayNodeSizeUnitPC: value = scale(value, child->parent->measuredContentHeight); break;
			case kDisplayNodeSizeUnitPW: value = scale(value, child->parent->measuredInnerWidth); break;
			case kDisplayNodeSizeUnitPH: value = scale(value, child->parent->measuredInnerHeight); break;
			case kDisplayNodeSizeUnitCW: value = scale(value, child->parent->measuredContentWidth); break;
			case kDisplayNodeSizeUnitCH: value = scale(value, child->parent->measuredContentHeight); break;
			case kDisplayNodeSizeUnitVW: value = scale(value, child->display->viewportWidth); break;
			case kDisplayNodeSizeUnitVH: value = scale(value, child->display->viewportHeight); break;
			default: break;
		}

	}

	value = clamp(
		value,
		child->height.min,
		child->height.max
	);

	return round(value, child->display->scale);
}

//------------------------------------------------------------------------------
// MARK: Public API
//------------------------------------------------------------------------------

void
AbsoluteLayoutResolver::resolve()
{
	if (this->nodes.size() == 0) {
		return;
	}

	for (auto child : this->nodes) {
			
		const auto lastMeasuredW = child->measuredWidth;
		const auto lastMeasuredH = child->measuredHeight;
		
        double measuredW = 0;
        double measuredH = 0;
            
        this->measure(
            child,
            measuredW,
            measuredH
        );
   
        if (measuredW != child->measuredWidth ||
            measuredH != child->measuredHeight) {
			
            child->measuredWidth = measuredW;
            child->measuredHeight = measuredH;
            child->invalidSize = false;
                   
            child->invalidateInnerSize();
        }
		
		child->resolveBorder();
		child->resolveInnerSize();
		child->resolveContentSize();
		child->resolveContentPosition();
		child->resolvePadding();
		
		/*
		 * When wrapping on either the with or height, the padding and
		 * border must be added the width or height.
		 */
		
		const auto wrapW = child->isWrappingWidth();
		const auto wrapH = child->isWrappingHeight();
				
		if (wrapW) {
			child->measuredWidth += (
				child->measuredBorderLeft +
				child->measuredBorderRight +
				child->measuredPaddingLeft +
				child->measuredPaddingRight
			);
		}
				
		if (wrapH) {
			child->measuredHeight += (
				child->measuredBorderTop +
				child->measuredBorderBottom +
				child->measuredPaddingTop +
				child->measuredPaddingBottom
			);
		}
		
		if (lastMeasuredW != child->measuredWidth ||
			lastMeasuredH != child->measuredHeight) {
			child->didResolveSize();
		}
        
		if (this->hasInvalidPosition(child)) {

			double measuredT = this->measureTop(child);
			double measuredL = this->measureLeft(child);
			double measuredR = this->measureRight(child);
			double measuredB = this->measureBottom(child);

			const auto t = child->top.type;
			const auto l = child->left.type;
			const auto r = child->right.type;
			const auto b = child->bottom.type;

			if (l == kDisplayNodePositionTypeAuto &&
				r == kDisplayNodePositionTypeLength) {
				measuredL = child->parent->measuredContentWidth - measuredR - child->measuredWidth;
			}

			if (t == kDisplayNodePositionTypeAuto &&
				b == kDisplayNodePositionTypeLength) {
				measuredT = child->parent->measuredContentHeight - measuredB - child->measuredHeight;
			}

			const auto anchorT = child->measureAnchorTop();
			const auto anchorL = child->measureAnchorLeft();

			measuredT = measuredT + child->measuredMarginTop - anchorT;
			measuredL = measuredL + child->measuredMarginLeft - anchorL;
			measuredR = measuredR + child->measuredMarginRight - anchorL;
			measuredB = measuredB + child->measuredMarginBottom - anchorT;

			if (measuredT != child->measuredTop ||
				measuredL != child->measuredLeft ||
                measuredR != child->measuredRight ||
                measuredB != child->measuredBottom) {

				child->measuredTop = measuredT;
				child->measuredLeft = measuredL;
				child->measuredRight = measuredR;
				child->measuredBottom = measuredB;
                child->invalidPosition = false;
                
				child->didResolvePosition();
			}
		}
	}

	this->nodes.clear();
}

void
AbsoluteLayoutResolver::measure(DisplayNode* node, double &w, double &h)
{
	/*
	 * Assigns the current measured width and height as default
	 * value for this method's result.
	 */
	
	w = node->measuredWidth;
	h = node->measuredHeight;
	
	/*
	 * Resolving the node margin must be done before measuring because
	 * it might influence its size, for instance, when using negatives
	 * margins on opposite sides.
	 */

	node->resolveMargin();
	
	const auto wrapW = node->isWrappingWidth();
	const auto wrapH = node->isWrappingHeight();
	
    if (this->hasInvalidSize(node)) {
        if (wrapW == false) w = this->measureWidth(node);
        if (wrapH == false) h = this->measureHeight(node);
    }
    
    if (wrapW || wrapH) {
	
        node->measureContent(w, h);

		auto measuredW = w;
		auto measuredH = h;
		
		if (wrapW) {

			measuredW = round(clamp(
				w,
				node->width.min,
				node->width.max
			), node->display->scale);
		
		}
		
		if (wrapH) {

			measuredH = round(clamp(
				h,
				node->height.min,
				node->height.max
			), node->display->scale);

		}
	
		/*
		 * The measured size changed between now and the time the
		 * content has been measured by performing a layout. Because of
		 * this, we need to schedule a second layout pass.
		 */
	
		if (w != measuredW ||
			h != measuredH) {
			
			node->invalidateLayout();
			
			w = measuredW;
			h = measuredH;
		}
	}
}

}
}
