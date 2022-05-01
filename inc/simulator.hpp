#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <vector>
#include <memory>

#include "nearest_neighbor_grid.hpp"
#include "kernel.hpp"

struct Particle {
    glm::vec3 pos, vel, col;
    float density;
    int attr;
};

class SPHSWE {
public:

    SPHSWE(float scale, float delta_time);

    void Evolve();

    float GetDeltaTime();
    int GetNumParticles();
    Particle* GetData();

private:
    using kernel = float(float, float);
    using vkernel = glm::vec3(glm::vec3, float);
    using terrain = float(float, float);

    // Particle
    std::vector<Particle> particles_;
    float particle_radius_;
    float mass_;
    unsigned int num_particles_;
    void GenerateBoundaryParticles();

    // Nearest Neighbor Search
    std::unique_ptr<NearestNeighborGrid> nng_;
    std::vector<std::vector<int>> neighbors_;
    void RegisterNearestNeighborGrid();
    void NearestNeighborSearch();

    // Kernel
    float effective_radius_;
    int kernel_particles_;
    kernel* normal_kernel_;
    vkernel* grad_kernel_;
    kernel* laplace_kernel_;

    // Fluid
    float density_;
    float viscosity_;

    // Terrain
    terrain* ground_;

    // Simulation
    float delta_time_;
    glm::vec3 min_cord_;
    glm::vec3 max_cord_;
};