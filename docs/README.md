## Introduction

CARMA is a header-only library with C ARray MAcros.

It follows the tradition of C developers implementing their own
utility functions and macros to deal with arrays,
since the C standard library is so bare bone.

Carma follows the ideas of [Generic Programming](https://en.wikipedia.org/wiki/Generic_programming#Stepanov%E2%80%93Musser_and_other_generic_programming_paradigms) and [Structural Typing](https://en.wikipedia.org/wiki/Structural_type_system)
and does not define concrete array structs on its own.
Instead, the library consists of macros that work on user defined structs,
as long as they follow certain conventions for the member variables.

## Example

CARMA provides macros like: `FOR_EACH`, `ERASE_IF`,
`CONCAT`, `CLEAR`, that can be aplied to user defined array structs like this:

```clike
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
```
