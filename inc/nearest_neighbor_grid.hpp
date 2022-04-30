#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/extented_min_max.hpp>
#include <glm/gtx/norm.hpp>

#include <memory.h>
#include <vector>
#include <algorithm>

class NearestNeighborGrid {
public:
    NearestNeighborGrid(glm::vec3 min_cord, glm::vec3 max_cord, float effective_radius, int num_particles);
    void Register(void* particles, unsigned int num_particles, int stride);
    void Search(glm::vec3 pos, void* particles, int stride, std::vector<int> neighbors, float effective_radius);

private:
    glm::uvec3 CalcIndices(glm::vec3 pos);
    unsigned int CalcHash(glm::uvec3 indices);
    unsigned int CalcHash(glm::vec3 pos);
    void CalcNeighborsInCell(glm::vec3 pos, void* particles, int stride, glm::uvec3 indices, std::vector<int> neighbors, float effective_radius);

    glm::uvec3 num_cells_;
    unsigned int num_all_cells_;
    glm::vec3 cell_width_;
    glm::vec3 origin_;

    std::vector<unsigned int> sorted_index_;
    std::vector<unsigned int> grid_hash_;
    std::vector<unsigned int> starts_;
    std::vector<unsigned int> ends_;
};