#include "stdafx.h"
#include "quadtree.h"

class QuadDraw {
	QuadTree* tree;
	ID3D11Buffer *pVBuffer;
	Pool<VERTEX> lines;
public:
	QuadDraw(QuadTree* tree) : tree(tree), pVBuffer(nullptr), lines(100000) {}

	~QuadDraw() {
		if (pVBuffer) {
			pVBuffer->Release();
			pVBuffer = 0;
		}
	}

	void addSquare(int xi, int yi, int rank, int value) {
		int r = (value & 1) * 255;
		int g = (value > 1 ? 255 : 0);
		int b = (value > 2 ? 255 : 0);

		float x = (float)(xi << (rank));
		float y = (float)(yi << (rank));
		float size = (float)(1 << rank);

		const VERTEX topLeft =     {x       , y       , 0.0f, r, g, b, 1};
		lines.push_back(topLeft);

		const VERTEX topRigtht =   {x + size, y       , 0.0f, r, g, b, 1};
		lines.push_back(topRigtht);
		lines.push_back(topRigtht);

		const VERTEX bottomRight = {x + size, y + size, 0.0f, r, g, b, 1};
		lines.push_back(bottomRight);
		lines.push_back(bottomRight);

		const VERTEX bottomLeft =  {x       , y + size, 0.0f, r, g, b, 1};
		lines.push_back(bottomLeft);
		lines.push_back(bottomLeft);

		// Close the square
		lines.push_back(topLeft);
	}

	void process(QuadTreeNode* node, int rank, int x, int y) {
		if (node->value) {
			addSquare(x, y, rank, node->value);
		}

		if (!node->children) return;

		for (auto i = 0; i < 4; ++i) {
			QuadTreeNode* child = &node->children[i];

			int xn = (x << 1) + (i % 2);
			int yn = (y << 1) + (i / 2);

			process(child, rank - 1, xn, yn);
		}
	}

	void update(ID3D11Device *dev) {
		lines.clear();
		process(tree->root, tree->rank, 0, 0);

		if (pVBuffer) {
			pVBuffer->Release();
			pVBuffer = 0;
		}

		if (lines.size() == 0) return;

		D3D11_BUFFER_DESC bd = {sizeof(VERTEX) * lines.size(), D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0};
		D3D11_SUBRESOURCE_DATA data = {lines.get(), bd.ByteWidth, 0};

		dev->CreateBuffer(&bd, &data, &pVBuffer);
	}

	void draw(ID3D11DeviceContext *devcon) {
		if (!pVBuffer) return;

        // select which vertex buffer to display
        UINT stride = sizeof(VERTEX);
        UINT offset = 0;

        devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
		devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
		devcon->Draw(lines.size(), 0);
	}
};