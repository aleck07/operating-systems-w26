# Grayscaler

A CLI tool to grayscale PPM images with multithreading.

## Building

Clone the repo and run `make`.

## Files

- `graysaler.c`: Main code for the grayscaler
- `goat.ppm`: Test file to
- `ppm.c`: PPM source code
- `ppm.h`: PPM header file
- `Makefile`: Makefile for the program

## Data

### `struct thread_data`
Structure passed to each worker thread containing:
- `thread_id`: Thread identifier
- `image`: Pointer to the PPM image being processed
- `start_y`: Starting row for this thread's slice
- `slice_height`: Number of rows to process
- `sum_r`, `sum_g`, `sum_b`: Accumulated RGB values for calculating averages

## Functions

- `main`: Validates arguments, calculates image height and launch threads to calculate pixel threads.
- `*grayscale_slice`: Calculates gray pixel value based on the RGB values of the slice.
