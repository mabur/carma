#pragma once

#if defined(__cplusplus) && __cplusplus >= 201402L
    // Implement as immediately invoked lambda:
    #define CARMA_BLOCK_EXPRESSION(body, result_expression) \
        ([&](){body return (result_expression);}())
#elif defined(__GNUC__)
    // Implement as statement expression:
    #define CARMA_BLOCK_EXPRESSION(body, result_expression) \
        ({body __auto_type _carma_temp = (result_expression); _carma_temp;})
#else
    #error "CARMA_BLOCK_EXPRESSION requires C++14 or GNUC"
#endif
