#include "RelativeLayoutResolver.h"
#include "LayoutResolver.h"
#include "Display.h"
#include "DisplayNode.h"

#include <iostream>

namespace Dezel {
namespace Layout {

using std::cerr;
using std::min;
using std::max;

RelativeLayoutResolver::RelativeLayoutResolver(DisplayNode* node)
{
	this->node = node;
}

//------------------------------------------------------------------------------
// MARK: Private API
//------------------------------------------------------------------------------

bool
RelativeLayoutResolver::hasInvalidSize(DisplayNode* child)
{
	if (child->invalidSize) {
		return true;
	}

	if (child->shrinkRatio > 0 ||
		child->expandRatio > 0) {
		return true;
	}

	if (child->width.unit == kDisplayNodeSizeUnitPX &&
		child->height.unit == kDisplayNodeSizeUnitPX) {
		return false;
	}

	auto parent = child->parent;

	if (parent->measuredContentWidth != parent->resolvedContentWidth || child->hasNewParent()) {
		if (child->width.unit == kDisplayNodeSizeUnitPC ||
			child->width.type == kDisplayNodeSizeTypeFill) {
			return true;
		}
	}

	if (parent->measuredContentHeight != parent->resolvedContentHeight || child->hasNewParent()) {
		if (child->height.unit == kDisplayNodeSizeUnitPC ||
			child->height.type == kDisplayNodeSizeTypeFill) {
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

	if (child->display->hasNewViewportWidth()) {
		if (child->width.unit == kDisplayNodeSizeUnitVH ||
			child->height.unit == kDisplayNodeSizeUnitVH) {
			return true;
		}
	}

	return false;
}

bool
RelativeLayoutResolver::hasInvalidPosition(DisplayNode* node)
{
	return true;
}

double
RelativeLayoutResolver::measureWidth(DisplayNode* child, double remaining)
{
	const auto type = child->width.type;
	const auto unit = child->width.unit;

	double value = child->width.length;

	if (type == kDisplayNodeSizeTypeFill) {

		value = remaining - child->measuredMarginLeft - child->measuredMarginRight;

	} else if (type == kDisplayNodeSizeTypeWrap) {

		cerr << "The routine that handles measuring for this node does not handle the wrap value properly";
		abort();

	} else if (type == kDisplayNodeSizeTypeLength) {

		switch (unit) {
			case kDisplayNodeSizeUnitPC: value = scale(value, child->parent->measuredContentWidth - child->parent->measuredPaddingLeft - child->parent->measuredPaddingRight); break;
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

	return value;
}

double
RelativeLayoutResolver::measureHeight(DisplayNode* child, double remaining)
{
	const auto type = child->height.type;
	const auto unit = child->height.unit;

	double value = child->height.length;

	if (type == kDisplayNodeSizeTypeFill) {

		value = remaining - child->measuredMarginTop - child->measuredMarginBottom;

	} else if (type == kDisplayNodeSizeTypeWrap) {

		cerr << "The routine that handles measuring for this node does not handle the wrap value properly";
		abort();

	} else if (type == kDisplayNodeSizeTypeLength) {

		switch (unit) {
			case kDisplayNodeSizeUnitPC: value = scale(value, child->parent->measuredContentHeight - child->parent->measuredPaddingTop - child->parent->measuredPaddingBottom); break;
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

	return value;
}

double
RelativeLayoutResolver::resolveAlignment(DisplayNode* child, double remaining)
{
	double size = 0;
	double headOffset = 0;
	double tailOffset = 0;

	switch (child->parent->contentDirection) {

		case kDisplayNodeContentDirectionVertical:
			size = child->measuredWidth;
			headOffset = child->measuredMarginLeft;
			tailOffset = child->measuredMarginRight;
			break;

		case kDisplayNodeContentDirectionHorizontal:
			size = child->measuredHeight;
			headOffset = child->measuredMarginTop;
			tailOffset = child->measuredMarginBottom;
			break;
	}

	double offset = 0;

	switch (child->parent->contentAlignment) {

		case kDisplayNodeContentAlignmentStart:
			offset = headOffset;
			break;

		case kDisplayNodeContentAlignmentCenter:
			offset = this->alignMid(size, remaining) + headOffset - tailOffset;
			break;

		case kDisplayNodeContentAlignmentEnd:
			offset = this->alignEnd(size, remaining) - tailOffset;
			break;
	}

	return offset;
}

void
RelativeLayoutResolver::expandNodesVertically(const vector<DisplayNode*> &nodes, double space, double weights)
{
	double remainder = 0;

	for (auto &child : nodes) {
		double measuredH = child->measuredHeight + (child->expandRatio / weights * space);
		measuredH = round(measuredH, child->display->getScale(), remainder);
        child->measuredHeight = measuredH;
	}
}

void
RelativeLayoutResolver::expandNodesHorizontally(const vector<DisplayNode*> &nodes, double space, double weights)
{
	double remainder = 0;

	for (auto &child : nodes) {
		double measuredW = child->measuredWidth + (child->expandRatio / weights * space);
		measuredW = round(measuredW, child->display->getScale(), remainder);
        child->measuredWidth = measuredW;
	}
}

void
RelativeLayoutResolver::shrinkNodesVertically(const vector<DisplayNode*> &nodes, double space, double weights)
{
	double remainder = 0;

	for (auto &child : nodes) {
		double measuredH = child->measuredHeight + (child->shrinkRatio / weights * space);
		measuredH = round(measuredH, child->display->getScale(), remainder);
        child->measuredHeight = measuredH;
	}
}

void
RelativeLayoutResolver::shrinkNodesHorizontally(const vector<DisplayNode*> &nodes, double space, double weights)
{
	double remainder = 0;

	for (auto &child : nodes) {
		double measuredW = child->measuredWidth + (child->shrinkRatio / weights * space);
		measuredW = round(measuredW, child->display->getScale(), remainder);
        child->measuredWidth = measuredW;
	}
}

//------------------------------------------------------------------------------
// MARK: Public API
//------------------------------------------------------------------------------

void
RelativeLayoutResolver::resolve()
{
	if (this->nodes.size() == 0) {
		return;
	}

	this->extentTop = 0;
	this->extentLeft = 0;
	this->extentRight = 0;
	this->extentBottom = 0;

	const double scale = this->node->display->getScale();

	const double paddingT = this->node->measuredPaddingTop;
	const double paddingL = this->node->measuredPaddingLeft;
	const double paddingR = this->node->measuredPaddingRight;
	const double paddingB = this->node->measuredPaddingBottom;

	const double contentW = max(this->node->measuredContentWidth - paddingL - paddingR, 0.0);
	const double contentH = max(this->node->measuredContentHeight - paddingT - paddingB, 0.0);
	const double contentT = this->node->measuredContentTop + paddingT;
	const double contentL = this->node->measuredContentLeft + paddingL;

	double remainder = 0;
	double remainingW = contentW;
	double remainingH = contentH;
	double expandablesWeight = 0;
	double shrinkablesWeight = 0;

	vector<DisplayNode*> shrinkables;
	vector<DisplayNode*> expandables;
    
    double lastMeasuredW[this->nodes.size()];
    double lastMeasuredH[this->nodes.size()];
    
    for (size_t i = 0; i < this->nodes.size(); i++) {
        
        auto child = this->nodes[i];
        
        lastMeasuredW[i] = child->measuredWidth;
        lastMeasuredH[i] = child->measuredHeight;
        
        double measuredW = 0;
        double measuredH = 0;
        
        this->measure(
              child,
              measuredW,
              measuredH,
              remainingW,
              remainingH,
              remainder
          );
        
        /*
         * Assign the measured width and height immediately because operations
         * later will use these values. However, they are not final, this is
         * why are not calling the delegated at this point.
         */
        
        child->measuredWidth = measuredW;
        child->measuredHeight = measuredH;
              
		switch (this->node->contentDirection) {

			case kDisplayNodeContentDirectionVertical:
				remainingH -= child->measuredHeight;
				remainingH -= child->measuredMarginTop;
				remainingH -= child->measuredMarginBottom;
				break;

			case kDisplayNodeContentDirectionHorizontal:
				remainingW -= child->measuredWidth;
				remainingW -= child->measuredMarginLeft;
				remainingW -= child->measuredMarginRight;
				break;
		}

		if (child->shrinkRatio > 0) {
			shrinkables.push_back(child);
			shrinkablesWeight += child->shrinkRatio;
		}

		if (child->expandRatio > 0) {
			expandables.push_back(child);
			expandablesWeight += child->expandRatio;
		}
	}

    // should use isWrappingWidth / isWrappingHeight ?
	const auto wrapW = this->node->width.type == kDisplayNodeSizeTypeWrap;
	const auto wrapH = this->node->height.type == kDisplayNodeSizeTypeWrap;

	if (wrapW) remainingW = max(remainingW, 0.0);
	if (wrapH) remainingH = max(remainingH, 0.0);

	double directionSpace = 0;
	double alignmentSpace = 0;

	switch (this->node->contentDirection) {

		case kDisplayNodeContentDirectionVertical:
			directionSpace = remainingH;
			alignmentSpace = remainingW;
			break;

		case kDisplayNodeContentDirectionHorizontal:
			directionSpace = remainingW;
			alignmentSpace = remainingH;
			break;
	}

	if (directionSpace > 0) {

		if (expandables.size()) {

			switch (this->node->contentDirection) {

				case kDisplayNodeContentDirectionVertical:
					this->expandNodesVertically(expandables, remainingH, expandablesWeight);
					break;

				case kDisplayNodeContentDirectionHorizontal:
					this->expandNodesHorizontally(expandables, remainingW, expandablesWeight);
					break;
			}

			directionSpace = 0;
		}

	} else if (directionSpace < 0) {

		if (shrinkables.size()) {

			switch (this->node->contentDirection) {

				case kDisplayNodeContentDirectionVertical:
					this->shrinkNodesVertically(shrinkables, remainingH, shrinkablesWeight);
					break;

				case kDisplayNodeContentDirectionHorizontal:
					this->shrinkNodesHorizontally(shrinkables, remainingW, shrinkablesWeight);
					break;
			}

			directionSpace = 0;
		}
	}

	double offset = 0;
	double spacer = 0;

	switch (this->node->contentDisposition) {

		case kDisplayNodeContentDispositionStart:
			offset = 0;
			break;

		case kDisplayNodeContentDispositionEnd:
			offset = directionSpace;
			break;

		case kDisplayNodeContentDispositionCenter:
			offset = directionSpace / 2;
			break;

		case kDisplayNodeContentDispositionSpaceBetween:
			spacer = directionSpace / (nodes.size() - 1);
			offset = 0;
			break;

		case kDisplayNodeContentDispositionSpaceEvenly:
			spacer = directionSpace / (nodes.size() + 1);
			offset = spacer;
			break;

		case kDisplayNodeContentDispositionSpaceAround:
			spacer = directionSpace / (nodes.size() + 1);
			offset = spacer / 2;
			break;
	}
    
    for (size_t i = 0; i < this->nodes.size(); i++) {
        
        auto child = this->nodes[i];

        const auto measuredW = child->measuredWidth;
        const auto measuredH = child->measuredHeight;
        
        if (lastMeasuredW[i] != measuredW ||
            lastMeasuredH[i] != measuredH) {
            
            child->invalidSize = false;
             
            child->invalidateInnerSize();
        }
        
		child->resolveBorder();
		child->resolveInnerSize();
		child->resolveContentSize();
		child->resolveContentPosition();
		child->resolvePadding();
		
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
		
		if (lastMeasuredW[i] != child->measuredWidth ||
			lastMeasuredH[i] != child->measuredHeight) {
			child->didResolveSize();
		}
		
		const double marginT = child->measuredMarginTop;
		const double marginL = child->measuredMarginLeft;
		const double marginR = child->measuredMarginRight;
		const double marginB = child->measuredMarginBottom;
        
		double x = contentL;
		double y = contentT;

		switch (this->node->contentDirection) {

			case kDisplayNodeContentDirectionVertical:

				x = round(x + this->resolveAlignment(child, alignmentSpace), scale);
				y = round(y + offset + marginT, scale);

				offset = offset + measuredH + marginT + marginB + spacer;

				break;

			case kDisplayNodeContentDirectionHorizontal:

				x = round(x + offset + marginL, scale);
				y = round(y + this->resolveAlignment(child, alignmentSpace), scale);

				offset = offset + measuredW + marginL + marginR + spacer;

				break;

			default:
				cerr << "Invalid content direction";
				abort();
		}

		const double anchorTop = child->measureAnchorTop();
		const double anchorLeft = child->measureAnchorLeft();

		const double measuredT = y - anchorTop;
		const double measuredL = x - anchorLeft;
		const double measuredR = (child->parent->measuredContentWidth - child->measuredWidth - x) - anchorLeft;
		const double measuredB = (child->parent->measuredContentHeight - child->measuredHeight - y) - anchorTop;

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
				
		this->extentTop = min(this->extentTop, child->measuredTop + child->measuredMarginTop);
		this->extentLeft = min(this->extentLeft, child->measuredLeft + child->measuredMarginBottom);
		this->extentRight = max(this->extentRight, child->measuredLeft + child->measuredWidth + child->measuredMarginRight);
		this->extentBottom = max(this->extentBottom, child->measuredTop + child->measuredHeight + child->measuredMarginBottom);
	}

	this->extentRight += paddingR;
	this->extentBottom += paddingB;

	this->nodes.clear();
}

void
RelativeLayoutResolver::measure(DisplayNode* node, double &w, double &h, double &remainingW, double &remainingH, double &remainder)
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

	const auto scale = node->display->getScale();
	
    if (this->hasInvalidSize(node)) {

        if (wrapW == false) w = this->measureWidth(node, remainingW);
        if (wrapH == false) h = this->measureHeight(node, remainingH);

        switch (node->contentDirection) {

            case kDisplayNodeContentDirectionVertical:
                if (wrapW == false) w = round(w, scale);
                if (wrapH == false) h = round(h, scale, remainder);
                break;

            case kDisplayNodeContentDirectionHorizontal:
                if (wrapW == false) w = round(w, scale, remainder);
                if (wrapH == false) h = round(h, scale);
                break;
        }
    }

    /*
     * When wrapping on either the width or the height, we need
     * to measure the content of the node, which may have to
     * resolve its layout.
     */
    
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
		
		switch (node->contentDirection) {

			case kDisplayNodeContentDirectionVertical:
				if (wrapW) w = round(w, scale);
				if (wrapH) h = round(h, scale, remainder);
				break;

			case kDisplayNodeContentDirectionHorizontal:
				if (wrapW) w = round(w, scale, remainder);
				if (wrapH) h = round(h, scale);
				break;
		}
	
		/*
		 * The measured size changed between now and the time the
		 * content has been measured by performing a layout. Because of
		 * this, we need to perform a second layout pass.
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
