#include <carma/carma.h>

typedef struct Particle {
    int x;
    int y;
    int vx;
    int vy;
    int age;
    int mass;
} Particle;

typedef struct Particles {
    Particle* data;
    int count;
    int capacity;
} Particles;

Particle updateParticle(Particle p) {
    return (Particle){p.x + p.vx, p.y + p.vy, p.vx, p.vy, p.age, p.mass};
}

bool shouldDie(Particle p) {
    return p.age > 500;
}

Particles explode(Particles new_particles, Particle p) {
    auto m = p.mass / 2;
    if (m > 0) {
        auto a = (Particle){p.x, p.y, p.vy, -p.vx, 0, m};
        auto b = (Particle){p.x, p.y, -p.vy, p.vx, 0, m};
        APPEND(new_particles, a);
        APPEND(new_particles, b);
    }
    return new_particles;
}

Particles update(Particles particles) {
    static Particles new_particles = {};

    FOR_EACH(p, particles) {
        *p = updateParticle(*p);
        if (shouldDie(*p)) {
            new_particles = explode(new_particles, *p);
        }
    }
    ERASE_IF(particles, shouldDie);
    CONCAT(particles, new_particles);
    CLEAR(new_particles);
    return particles;
}

int main() {
    auto particles = (Particles){};
    for (;;) {
        particles = update(particles);
    }
}
