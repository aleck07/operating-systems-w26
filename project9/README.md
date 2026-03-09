# Producer-Consumer

A CLI tool to simulate a producer-consumer system with multithreading and semaphores.

## Building

Clone the repo and run `make`.

## Usage

```bash
./pc num_producers num_consumers num_events max_buffer_size
```

Example:

```bash
./pc 2 5 15 3
```

## Files

- `pc.c`: Main code for the producer-consumer system
- `eventbuf.c`: Event buffer implementation used as the shared queue
- `Makefile`: Makefile for the program

## Global Variables

- `num_events`: Number of events each producer will add to the buffer
- `eb`: Pointer to the shared event buffer
- `mutex`: Semaphore to ensure mutual exclusion when accessing the event buffer
- `items`: Semaphore tracking the number of items available for consumers to consume
- `spaces`: Semaphore tracking the number of open spaces available for producers to fill

## Functions

- `main`: Parses command line arguments, initializes the event buffer and semaphores, allocates thread arrays, launches threads via `run_threads`, and cleans up.
- `run_threads`: Creates producer and consumer threads, joins all producer threads, signals consumers to exit, then joins all consumer threads.
- `*producer`: Adds `num_events` events to the shared buffer, waiting for space and acquiring the mutex before each add.
- `*consumer`: Continuously consumes events from the buffer until signaled to exit by a sentinel `sem_post` from `run_threads`.
- `sem_open_temp`: Opens a named semaphore and immediately unlinks it so it is cleaned up on process exit.
