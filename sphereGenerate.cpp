#include <vector>
#include "PhysicsConstants.h"

void generateSphere(float radius, int sectorCount, int stackCount, std::vector<float>& vertices, std::vector<unsigned int>& indices) {
	float x, y, z, xy;

	float sectorStep = 2 * pi / sectorCount;
	float stackStep = pi / stackCount;
	float sectorAngle, stackAngle;

	//generate vertices
	for (int i = 0; i <= stackCount; i++) {
		stackAngle = pi / 2 - i * stackStep;
		xy = radius * cosf(stackAngle);
		z = radius * sinf(stackAngle);
		
		for (int j = 0; j <= sectorCount; j++) {
			sectorAngle = j * sectorStep;

			x = xy * cosf(sectorAngle);
			y = xy * sinf(sectorAngle);
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
	}

	//generate indices
	int k1, k2;
	for (int i = 0; i < stackCount; i++) {
		k1 = i * (sectorCount + 1);
		k2 = k1 + sectorCount + 1;

		for (int j = 0; j < sectorCount; j++, k1++, k2++) {
			if (i != 0) {
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if (i != (stackCount - 1)) {
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}

	}
}