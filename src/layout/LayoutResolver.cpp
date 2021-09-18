#include "LayoutResolver.h"
#include "Display.h"
#include "DisplayBase.h"
#include "DisplayNode.h"

#include <iostream>

namespace Dezel {
namespace Layout {

using std::min;
using std::max;

//------------------------------------------------------------------------------
// MARK: Public API
//------------------------------------------------------------------------------

LayoutResolver::LayoutResolver(DisplayNode* node) : relativeLayoutResolver(node), absoluteLayoutResolver(node)
{
	this->node = node;
}

void
LayoutResolver::prepare()
{
	this->node->didPrepareLayout();
}

void
LayoutResolver::resolve() {

	/*
	 * The layout prepare callback is allowed to update the content size
	 * thus we need to resolve it again in case.
	 */

	if (this->node->children.size() == 0) {
		return;
	}

	for (auto child : this->node->children) {

		if (child->visible == false) {
			continue;
		}

		child->resolveValues();

		if (child->isRelative()) {
			this->relativeLayoutResolver.append(child);
		} else {
			this->absoluteLayoutResolver.append(child);
		}
	}

	const bool autoContentW = this->node->contentWidth.type == kDisplayNodeContentSizeTypeAuto;
	const bool autoContentH = this->node->contentHeight.type == kDisplayNodeContentSizeTypeAuto;

	this->relativeLayoutResolver.resolve();

	if (autoContentW || autoContentH) {

        /*
         * The content size can be calculated automatically from
         * the extent of the display node.
         */
        
        const auto contentW = this->node->measuredContentWidth;
        const auto contentH = this->node->measuredContentHeight;
        
		if (autoContentW) this->node->measuredContentWidth = max(this->node->measuredContentWidth, this->getExtentRight());
		if (autoContentH) this->node->measuredContentHeight = max(this->node->measuredContentHeight, this->getExtentBottom());

		if (contentW != this->node->measuredContentWidth ||
			contentH != this->node->measuredContentHeight) {
			this->node->didResolveContentSize();
		}
	}

	this->absoluteLayoutResolver.resolve();

	this->node->didResolveLayout();
}

}
} 
