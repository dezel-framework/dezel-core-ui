#ifndef RelativeNodesResolver_h
#define RelativeNodesResolver_h

#include <vector>

namespace Dezel {
	class Display;
	class DisplayNode;
}

namespace Dezel {
namespace Layout {

using std::vector;

class LayoutResolver;

class RelativeLayoutResolver {

private:

	DisplayNode* node;

	vector<DisplayNode*> nodes;

	double extentTop = 0;
	double extentLeft = 0;
	double extentRight = 0;
	double extentBottom = 0;

	inline double alignMid(double size, double container) {
		return container / 2 - size / 2;
	}

	inline double alignEnd(double size, double container) {
		return container - size;
	}

	bool hasInvalidSize(DisplayNode* node);
	bool hasInvalidPosition(DisplayNode* node);

	double measureWidth(DisplayNode* node, double remaining);
	double measureHeight(DisplayNode* node, double remaining);

	double resolveAlignment(DisplayNode* node, double remaining);

	void expandNodesVertically(const vector<DisplayNode*> &nodes, double space, double weights);
	void expandNodesHorizontally(const vector<DisplayNode*> &nodes, double space, double weights);
	void shrinkNodesVertically(const vector<DisplayNode*> &nodes, double space, double weights);
	void shrinkNodesHorizontally(const vector<DisplayNode*> &nodes, double space, double weights);

public:

	friend class LayoutResolver;

	RelativeLayoutResolver(DisplayNode* node);

	void append(DisplayNode* node) {
		this->nodes.push_back(node);
	}

	void resolve();
    void measure(DisplayNode* node, double &w, double &h, double &remainingW, double &remainingH, double &remainder);

};

}
}

#endif
