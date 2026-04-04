# Error Handling

These macros are defined in `carma_assert.h`.
We divide error handling in two different cases.

## Internal Errors

By internal errors we mean logical bugs that are not supposed to happen.
If we would encounter an internal error then we want to abort the program,
since it is in an invalid state.
We address internal errors by debugging the program and updating its code.
Our long term goal is to have zero internal errors.

- `CARMA_ABORT_FAILURE(fmt, ...)`
  Prints a formatted error message and aborts the program and returns the error code 1.

- `CHECK_INTERNAL(condition, fmt, ...)`
  Checks an internal condition that should always hold.
  If the condition is not valid then it prints a formatted error message
  and aborts the program and returns the error code 1.
  This is like an assert, but it is active in both debug and release builds.

## External Errors

By external errors we mean errors in the input to a program that is preventing the program from finishing successfully.
We then want to exit cleanly and report an error code.
An external error can also be an error in the environment that the program is running in,
like a missing a required file or missing internet connection if that is required.
We address external errors by updating the input to the program or the environment in which it is run.
Some external errors might be expected.

- `CARMA_EXIT_FAILURE(fmt, ...)`
  Prints a formatted error message and exits the program and returns the error code 1.

- `CHECK_EXTERNAL(condition, fmt, ...)`
  Checks an external condition that is required to hold for the program to be able to continue.
  If the condition is not valid then it prints a formatted error message
  and exits the program and returns the error code 1.
  It is active in both debug and release builds.
