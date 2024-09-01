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
    int count;
    int capacity;
    Particle* data;
} Particles;

Particle updateParticle(Particle p) {
    return (Particle){
        .x = p.x,
        .y = p.y,
        .vx = p.vx,
        .vy = p.vy,
        .age = p.age,
        .mass = p.mass,
    };
}

bool shouldDie(Particle p) {
    return p.age > 500;
}

Particles explode(Particles new_particles, Particle p) {
    while (p.mass > 0) {
        auto a = (Particle){p.x, p.y, p.vx, p.vy, 0, p.mass / 2};
        auto b = (Particle){p.x, p.y, p.vx, p.vy, 0, p.mass / 2};
        APPEND(new_particles, a);
        APPEND(new_particles, b);
        p.mass /= 2;
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
