#include "stdafx.h"
#include "Pool.h"

struct VERTEX{FLOAT X, Y, Z; BYTE R, G, B, A;};

struct QuadTreeNode {
	QuadTreeNode* children;
	int value;

	QuadTreeNode() : children(nullptr), value(0) {}

	static int offset(int x, int y, int rank) {
		return ((x >> rank) & 1) + ((y >> rank) & 1) * 2;
	}
};

struct QuadTree {
	Pool<QuadTreeNode> pool;
	QuadTreeNode* root;
	int rank;

	QuadTree(int rank) : root(pool.spawn(1)), rank(rank), pool(1000000) {}

	void clear() {
		pool.clear();
		root = pool.spawn(1);
	}

	void set(int depth, int x, int y, int value) {
		auto node = root;

		for (depth = rank - depth - 1; depth >= 0; --depth) {
			if (!node->children) {
				node->children = pool.spawn(4);
				node->value = 1;
			}

			auto index = QuadTreeNode::offset(x, y, depth);
			node = &node->children[index];
		}

		node->value = value;
	}

	void setRadiusDive(QuadTreeNode* node, int rank, int minRank, int radius, int x, int y, int xt, int yt) {
		int xo = ((x << (rank)) + (1 << (rank - 1)));
		int yo = ((y << (rank)) + (1 << (rank - 1)));
		int size = (int)((float)(1 << (rank - 1)) * 1.41421356f);
		int distance = (int)sqrtf((float)((xo - xt) * (xo - xt) + (yo - yt) * (yo - yt)));

		if (distance >= radius + size) {
			node->value = 2;
			return;
		}

		if (distance < radius - size) {
			node->value = 3;
			return;
		}

		if (rank == minRank) {
			node->value = 1;
			return;
		}

		if (!node->children) {
			node->children = pool.spawn(4);
		}

		for (auto i = 0; i < 4; ++i) {
			QuadTreeNode* child = &node->children[i];

			int xn = (x << 1) + (i % 2);
			int yn = (y << 1) + (i / 2);

			setRadiusDive(child, rank - 1, minRank, radius, xn, yn, xt, yt);
		}
	}

	int pointToLineDistance(float x1, float y1, float x2, float y2, float x, float y) {
		float A = x - x1;
		float B = y - y1;
		float C = x2 - x1;
		float D = y2 - y1;

		return (int)((A * D - C * B) / sqrt(C * C + D * D));
	}

	void setConeDive(QuadTreeNode* node, int rank, int minRank, float x1, float y1, float x2, float y2, float x3, float y3, int x, int y, float xt, float yt) {
		float xo = (float)((x << (rank)) + (1 << (rank - 1)));
		float yo = (float)((y << (rank)) + (1 << (rank - 1)));

		int distance1 = pointToLineDistance(xt, yt, x1, y1, xo, yo);
		int distance2 = pointToLineDistance(xt, yt, x2, y2, xo, yo);
		int distance3 = pointToLineDistance(xt, yt, x3, y3, xo, yo);

		int size = (int)((float)(1 << (rank - 1)) * 1.41421356f);

		if (abs(distance1) >= size && abs(distance2) >= size || distance3 >= size) {
			if (-distance1 >= 0 && distance2 >= 0) {
				node->value = 3;
			} else {
				node->value = 2;
			}

			return;
		}

		if (rank == minRank) {
			node->value = 1;
			return;
		}

		if (!node->children) {
			node->children = pool.spawn(4);
		}

		for (auto i = 0; i < 4; ++i) {
			QuadTreeNode* child = &node->children[i];

			int xn = (x << 1) + (i % 2);
			int yn = (y << 1) + (i / 2);

			setConeDive(child, rank - 1, minRank, x1, y1, x2, y2, x3, y3, xn, yn, xt, yt);
		}
	}

	void setRadius(int radius, int minRank, int x, int y) {
		setRadiusDive(root, rank, minRank, radius, 0, 0, x, y);
	}

	void setCone(float angle, float fov, int minRank, int x, int y) {
		setConeDive(root, rank, minRank, 
			sinf(angle - fov * 0.5f) + x, cosf(angle - fov * 0.5f) + y, 
			sinf(angle + fov * 0.5f) + x, cosf(angle + fov * 0.5f) + y, 
			sinf(angle - 3.1415923f * 0.5f) + x, cosf(angle - 3.1415923f * 0.5f) + y, 
			0, 0, (float)x, (float)y);
	}
};

