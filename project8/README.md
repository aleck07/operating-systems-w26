# Reservations

A CLI tool to simulate seat reservations with multithreading and mutex synchronization.

## Building

Clone the repo and run `make`.

## Usage

```bash
./reservations seat_count broker_count transaction_count
```

Example:

```bash
./reservations 100 100 5000
```

## Files

- `reservations.c`: Main code for the seat reservation system
- `Makefile`: Makefile for the program

## Global Variables

- `seat_count`: Total number of seats available
- `broker_count`: Number of concurrent broker threads
- `seat_taken`: Array tracking which seats are taken (1) or free (0)
- `transaction_count`: Number of transactions each broker performs
- `seat_taken_count`: Counter for total seats currently reserved
- `seat_mutex`: Mutex to protect shared seat data from race conditions

## Functions

- `main`: Parses command line arguments, initializes seat array, launches broker threads, and waits for completion.
- `*seat_broker`: Function that performs random buy/sell transactions on random seats.
- `reserve_seat`: Reserves a seat and increments counter.
- `free_seat`: Frees a seat and decrements counter.
- `verify_seat_count`: Verifies seat count between seat_taken array and seat_taken_count.
