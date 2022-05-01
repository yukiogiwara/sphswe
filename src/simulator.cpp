#include "simulator.hpp"

SPHSWE::SPHSWE(float scale, float delta_time) {
    // Set cordinates
    min_cord_ = glm::vec3(-scale/2.0f, 0.0f, -scale/2.0f);
    max_cord_ = glm::vec3( scale/2.0f, 0.0f,  scale/2.0f);

    // Set parameters
    delta_time_ = delta_time;
    mass_ = 2.0;
    density_ = 998.29;
    viscosity_ = 30.0;
    kernel_particles_ = 20;

    // Calculate effective radius
    effective_radius_ = sqrt(mass_*kernel_particles_/(glm::pi<float>()*density_));
    particle_radius_ = 0.5*effective_radius_*sqrt(glm::pi<float>()/kernel_particles_);

    // Set kernels
    normal_kernel_ = Poly6;    
    grad_kernel_ = GradSpiky;
    laplace_kernel_ = LaplaceVisc;

    // Set ground
    ground_ = [](float x, float z) -> float { return 0; };

    // Generate particles
    GenerateBoundaryParticles();    
    // AddParticlesRectangle();
    num_particles_ = particles_.size();
}

void SPHSWE::Evolve() {

}

float SPHSWE::GetDeltaTime() {
    return delta_time_;
}

int SPHSWE::GetNumParticles() {
    return num_particles_;
}

Particle* SPHSWE::GetData() {
    return particles_.data();
}

void SPHSWE::RegisterNearestNeighborGrid() {
    nng_->Register(particles_.data(), num_particles_, sizeof(Particle));
}

void SPHSWE::NearestNeighborSearch() {
    for(int i = 0; i < num_particles_; i++) {
        nng_->Search(particles_[i].pos, particles_.data(), sizeof(Particle), neighbors_[i], effective_radius_);
    }
}

void SPHSWE::GenerateBoundaryParticles() {
    for(float x = min_cord_[0]; x <= max_cord_[0]; x += 2*particle_radius_) {
        for(float z = min_cord_[2]; z <= max_cord_[2]; z += 2*particle_radius_) {
            Particle p{glm::vec3(x, 0.0f, z), glm::vec3(0.0f), glm::vec3(0.5f), 0.0f, 1};
            particles_.push_back(p);
        }
    }
}