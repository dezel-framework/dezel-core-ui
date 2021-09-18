#include "DisplayNodeWalker.h"
#include "DisplayNode.h"

namespace Dezel {

DisplayNodeWalker::DisplayNodeWalker(DisplayNode* root)
{
    this->root = root;
    this->node = root;
	this->queue.push(root);
}

bool
DisplayNodeWalker::hasNext()
{
	if (this->queue.size() == 0) {
		return false;
	}

	this->consume();

	return true;
}

void
DisplayNodeWalker::getNext() {

	this->dequeue();

	auto children = this->node->getChildren();

	for (auto child : children) if (child->isVisible()) {
		this->enqueue(child);
	}
}

void
DisplayNodeWalker::reset()
{
    this->queue = std::queue<DisplayNode*>();
    this->queue.push(this->root);
}

}
