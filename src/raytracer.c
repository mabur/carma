#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "carma.h"

typedef struct {
    double x;
    double y;
    double z;
} Vec3d;

Vec3d add(Vec3d a, Vec3d b) {
    return (Vec3d){a.x + b.x, a.y + b.y, a.z + b.z};
}

Vec3d sub(Vec3d a, Vec3d b) {
    return (Vec3d){a.x - b.x, a.y - b.y, a.z - b.z};
}

Vec3d mul(Vec3d a, Vec3d b) {
    return (Vec3d){a.x * b.x, a.y * b.y, a.z * b.z};
}

Vec3d muls(double a, Vec3d b) {
    return (Vec3d){a * b.x, a * b.y, a * b.z};
}

double dot(Vec3d a, Vec3d b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

double squaredNorm(Vec3d v) {
    return dot(v, v);
}

double norm(Vec3d v) {
    return sqrt(squaredNorm(v));
}

Vec3d normalize(Vec3d v) {
    return muls(1.0 / norm(v), v);
}

typedef struct {
    Vec3d position;
    double squaredRadius;
    Vec3d color;
} Sphere;

typedef struct {
    Vec3d direction;
    Vec3d color;
} Light;

typedef RANGE(Sphere) Spheres;
typedef RANGE(Light) Lights;

typedef struct {
    Spheres spheres;
    Lights lights;
    Vec3d atmosphere_color;
} World;

typedef struct {
    Vec3d position;
    Vec3d normal;
    double distance;
    Vec3d color;
} Intersection;

Intersection makeIntersection() {
    return (Intersection){.distance = INFINITY};
}

World makeWorld() {
    auto R = 100000.0;
    auto MAX_C = 1.0;
    auto MIN_C = 0.1;
    auto world = (World){};
    
    INIT_RANGE_ELEMENTS(
        world.spheres, 
        (Sphere){(Vec3d){-2, 0, 6}, 1, (Vec3d){MAX_C, MAX_C, MIN_C}},
        (Sphere){(Vec3d){0, 0, 5}, 1, (Vec3d){MAX_C, MIN_C, MIN_C}},
        (Sphere){(Vec3d){2, 0, 4}, 1, (Vec3d){2 * MIN_C, 4 * MIN_C, MAX_C}},
        (Sphere){(Vec3d){0, 1 + R, 0}, R * R, (Vec3d){MIN_C, MAX_C, MIN_C}},
        (Sphere){(Vec3d){0, -1 - R, 0}, R * R, (Vec3d){MAX_C, MAX_C, MAX_C}},
    );
    INIT_RANGE_ELEMENTS(
        world.lights,
        (Light){(Vec3d){+1, +1, +2}, muls(0.4, (Vec3d){1, 0.8, 0.5})},
        (Light){(Vec3d){-1, -1, -2}, muls(0.4, (Vec3d){0.5, 0.5, 1})},
    );
    world.atmosphere_color = muls(0.3, (Vec3d){0.5, 0.5, 1});
    return world;
}

void freeWorld(World world) {
    FREE_RANGE(world.spheres);
    FREE_RANGE(world.lights);
}

Intersection findSingleIntersection(
    Vec3d start, Vec3d direction, Sphere sphere
) {
    auto intersection = makeIntersection();
    auto offset = sub(sphere.position, start);
    auto c = dot(direction, offset);
    if (c < 0.0) {
        return intersection;
    }
    auto discriminant = c * c - squaredNorm(offset) + sphere.squaredRadius;
    if (discriminant < 0.0) {
        return intersection;
    }
    intersection.distance = c - sqrt(discriminant);
    intersection.position = add(start, muls(intersection.distance, direction));
    intersection.normal = normalize(sub(intersection.position, sphere.position));
    intersection.color = sphere.color;
    return intersection;
}

Intersection findIntersection(Vec3d start, Vec3d direction, Spheres spheres) {
    auto i1 = makeIntersection();
    FOR_EACH(sphere, spheres) {
        auto i2 = findSingleIntersection(start, direction, *sphere);
        if (i2.distance < i1.distance) {
            i1 = i2;
        }
    }
    return i1;
}

Vec3d shadeSingleLight(Intersection intersection, Light light) {
    auto geometry = fmax(-dot(light.direction, intersection.normal), 0.0);
    return muls(geometry, mul(intersection.color, light.color));
}

Vec3d shadeAtmosphere(Intersection intersection, Vec3d atmosphere_color) {
    return muls(sqrt(intersection.position.z), atmosphere_color);
}

Vec3d shade(Intersection intersection, World world) {
    if (isinf(intersection.distance)) {
        return (Vec3d){ 1, 1, 1 };
    }
    auto color = shadeAtmosphere(intersection, world.atmosphere_color);
    FOR_EACH(light, world.lights) {
        color = add(color, shadeSingleLight(intersection, *light));
    }
    return color;
}

int colorU8fromF64(double c) {
    return (int)(fmin(255.0 * c, 255.0));
}

void writePixel(
    FILE* file,
    int x,
    int y,
    int width,
    int height,
    World world
) {
    auto start = (Vec3d){0, 0, 0};
    auto xd = (double)(x - width / 2);
    auto yd = (double)(y - height / 2);
    auto zd = (double)(height / 2);
    auto direction = normalize((Vec3d){xd, yd, zd});
    auto intersection = findIntersection(start, direction, world.spheres);
    auto color = shade(intersection, world);
    auto r = colorU8fromF64(color.x);
    auto g = colorU8fromF64(color.y);
    auto b = colorU8fromF64(color.z);
    fprintf(file, "%d %d %d ", r, g, b);
}

void writeImage(const char* file_path, World world) {
    auto file = fopen(file_path, "w");
    if (file == NULL) {
        fprintf(stderr, "error opening file\n");
        exit(EXIT_FAILURE);
    }
    auto WIDTH = 800;
    auto HEIGHT = 600;
    fprintf(file, "%s\n%d\n%d\n%d\n", "P3", WIDTH, HEIGHT, 255);
    for (auto y = 0; y < HEIGHT; ++y) {
        for (auto x = 0; x < WIDTH; ++x) {
            writePixel(file, x, y, WIDTH, HEIGHT, world);
        }
    }
    fclose(file);
}

int main() {
    printf("Saving image\n");
    auto world = makeWorld();
    writeImage("image.ppm", world);
    freeWorld(world);
    return 0;
}
