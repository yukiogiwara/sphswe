#include "nearest_neighbor_grid.hpp"

NearestNeighborGrid::NearestNeighborGrid(glm::vec3 min_cord, glm::vec3 max_cord, float effective_radius, int num_particles) {
    // Calculate cell width
    glm::vec3 size = max_cord - min_cord;
    float max_axis = glm::max(size.x, size.y, size.z);
    int d = (int)(log(max_axis/effective_radius)/log(2.0)+0.5);
    int m = (int)(pow(2.0f, (float)d)+0.5);
    float cell_width = max_axis / m;
    cell_width_ = glm::vec3(cell_width);
    origin_ = min_cord;

    // Calculate number of cells
    for(int i = 0; i < 3; i++) {
        if(size[i] < effective_radius) {
            num_cells_[i] = 1;
        } else {
            d = (int)(log(size[i]/cell_width)/log(2.0)+0.5);
            num_cells_[i] = (int)(pow(2.0, (float)d)+0.5);
        }
    }
    num_all_cells_ = num_cells_[0] * num_cells_[1] * num_cells_[2];

    sorted_index_.resize(num_particles);
    grid_hash_.resize(num_particles);
    starts_.resize(num_all_cells_);
    ends_.resize(num_all_cells_);
}

void NearestNeighborGrid::Register(void* particles, unsigned int num_particles, int stride) {
    std::fill(sorted_index_.begin(), sorted_index_.end(), 0);
    std::fill(grid_hash_.begin(), grid_hash_.end(), 0);
    std::fill(starts_.begin(), starts_.end(), 0xffffffff);
    std::fill(ends_.begin(), ends_.end(), 0xffffffff);

    float* float_particles = reinterpret_cast<float*>(particles);
    for(unsigned int i = 0; i < num_particles; i++) {
        glm::vec3 pos(*float_particles, *(float_particles+1), *(float_particles+2));
        float_particles += stride / sizeof(float);

        unsigned int hash = CalcHash(pos);

        sorted_index_[i] = i;
        grid_hash_[i] = hash;
    }

    std::vector<std::pair<unsigned int, unsigned int>> hash_and_value;
    hash_and_value.resize(num_particles);
    for(unsigned int i = 0; i < num_particles; i++) {
        hash_and_value[i].first = grid_hash_[i];
        hash_and_value[i].second = sorted_index_[i];
    }
    std::sort(hash_and_value.begin(), hash_and_value.end());
    for(unsigned int i = 0; i < num_particles; i++) {
        sorted_index_[i] = hash_and_value[i].second;
        grid_hash_[i] = hash_and_value[i].first;
    }

    for(unsigned int i = 0; i < num_particles; i++) {
        unsigned int hash = grid_hash_[i];

        if(i == 0) {
            starts_[hash] = i;
            ends_[hash] = i;
        } else {
            int prev_hash = grid_hash_[i-1];

            if(i == 0 || hash != prev_hash) {
                starts_[hash] = i;
                if(i > 0) {
                    ends_[prev_hash] = i;
                }
            }
            if(i == num_particles-1) {
                ends_[hash] = i+1;
            }
        }
    }
}

void NearestNeighborGrid::Search(glm::vec3 pos, void* particles, int stride, std::vector<int> &neighbors, float effective_radius) {
    glm::uvec3 indices = CalcIndices(pos);

    int range = (int)(effective_radius/cell_width_[0]) + 1;
    for(int k = -range; k <= range; k++) {
        for(int j = -range; j <= range; j++) {
            for(int i = -range; i <= range; i++) {
                glm::uvec3 neighbor_indices = indices + glm::uvec3(i, j, k);
                if(neighbor_indices[0] < 0 || neighbor_indices[0] >= num_cells_[0] || neighbor_indices[1] < 0 || neighbor_indices[1] >= num_cells_[1] || neighbor_indices[2] < 0 || neighbor_indices[2] >= num_cells_[2]) {
                    continue;
                }
                CalcNeighborsInCell(pos, particles, stride, indices, neighbors, effective_radius);
            }
        }
    }
}

void NearestNeighborGrid::CalcNeighborsInCell(glm::vec3 pos, void* particles, int stride, glm::uvec3 indices, std::vector<int> neighbors, float effective_radius) {

    float* float_particles = reinterpret_cast<float*>(particles);

    unsigned int hash = CalcHash(indices);
    unsigned int start_index = starts_[hash];

    if(start_index != 0xffffffff) {
        unsigned int end_index = ends_[hash];
        for(unsigned int j = start_index; j < end_index; j++) {
            unsigned int idx = sorted_index_[j];

            float* x_ptr = float_particles + idx*stride/sizeof(float);
            glm::vec3 x_ij = pos - glm::vec3(*x_ptr, *(x_ptr+1), *(x_ptr+2));
            if(glm::length2(x_ij) <= effective_radius*effective_radius) {
                neighbors.push_back(idx);
            }
        }
    }
}

glm::uvec3 NearestNeighborGrid::CalcIndices(glm::vec3 pos) {
    pos -= origin_;
    glm::uvec3 indices = glm::uvec3(pos / cell_width_);
    glm::clamp(indices, glm::uvec3(0), num_cells_-1u);
}

unsigned int NearestNeighborGrid::CalcHash(glm::uvec3 indices) {
    return indices[2]*num_cells_[0]*num_cells_[1] + indices[1]*num_cells_[0] + indices[0];
}

unsigned int NearestNeighborGrid::CalcHash(glm::vec3 pos) {
    glm::uvec3 indices = CalcIndices(pos);
    return CalcHash(indices);
}