#include "gridGeneration.h"

void generateGrid(float size, int divisions, std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
	float step = (2.0f * size) / divisions;

	for (int i = 0; i <= divisions; i++) {
		float z = -size + i * step;
		for (int j = 0; j <= divisions; j++) {
			float x = -size + j * step;
			vertices.push_back(x);
			vertices.push_back(0.0f);
			vertices.push_back(z);
		}
	}

	for (int i = 0; i <= divisions; i++)
		for (int j = 0; j <= divisions; j++) {
			unsigned int current = i * (divisions+1)+j;

			if (j < divisions) {
				indices.push_back(current);
				indices.push_back(current + 1);
			}

			if (i < divisions) {
				indices.push_back(current);
				indices.push_back(current + (divisions + 1));
			}
		}
}
