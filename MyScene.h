#include "stdafx.h"
#include "quaddraw.h"

struct MyScene 
{
	QuadTree tree;
	QuadDraw draw;

	MyScene() : tree(10), draw(&tree) {}

	void update(ID3D11Device *dev, ID3D11DeviceContext *devcon, int frame) {
		tree.clear();

		for (int i = 0; i < 1024; i += 100) {
			//tree.set(0, i, i, 2);
		}

		//tree.setRadius(abs(300 - frame % 600) + 100, 1, 512 + 128, 512 + 128);
		tree.setRadius(abs(300 - frame % 600) + 100, 1, 512 + 128, 512 + 128);
		float angle = (frame / 360.0f);
		tree.setCone(angle, 3.1415923f * 0.5f, 1, 512 - 128, 512 - 128);

		draw.update(dev);
	}

	void render(ID3D11DeviceContext *devcon) 
	{
		draw.draw(devcon);
	}
};

