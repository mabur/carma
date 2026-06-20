#pragma once

#ifdef __cplusplus
#define CARMA_BEGIN_EXTERN_C extern "C" {
#define CARMA_END_EXTERN_C }
#else
#define CARMA_BEGIN_EXTERN_C
#define CARMA_END_EXTERN_C
#endif
