#pragma once

#define MIN2(a, b) ((a) < (b) ? (a) : (b))
#define MAX2(a, b) ((a) < (b) ? (b) : (a))
#define CLAMP(value, low, high) (MIN2(MAX2((value), (low)), (high)))
