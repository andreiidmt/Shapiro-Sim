#include "lightPathfinder.h"
#include <cmath>
std::vector<glm::vec3> generateBentLight(glm::vec3 start, glm::vec3 destination, float massScale, bool isBlackHole, float scaledRadius, int steps)
{
    std::vector<glm::vec3> path;
    for (int i = 0; i <= steps; i++) {
        float t = i* 1.0f / steps;
        glm::vec3 pos = glm::mix(start, destination, t);

        float r = glm::length(glm::vec2(pos.x, pos.z));
        float yDisplacement = 0.0f;

        if (!isBlackHole) {
            yDisplacement = (-massScale / std::sqrt(r * r + scaledRadius * scaledRadius));
        }

        path.push_back(glm::vec3(pos.x, pos.y+yDisplacement, pos.z));
    }
    return path;
}

std::vector<glm::vec3> generateUnperturbedLight(glm::vec3 start, glm::vec3 destination, float dashLength, float gapLength)
{
    std::vector<glm::vec3> lines;
    float totalDist = glm::distance(start, destination);
    glm::vec3 dir = glm::normalize(destination - start);

    float currentDist = 0.0f;
    while (currentDist < totalDist) {
        glm::vec3 p1 = start + dir * currentDist;
        float nextDist = std::min(currentDist + dashLength,totalDist);
        glm::vec3 p2 = start + dir * nextDist;

        lines.push_back(p1);
        lines.push_back(p2);

        currentDist += dashLength + gapLength;
    }
    return lines;
}
