# Append log
A C program for UNIX systems to append logs to a memory mapped file, with locks.


Usage: `appendlog <count> <prefix> [<count> <prefix]...`

## Questions
1. If we didn't lock the header, the writers will race on the offset and count which will give us unexpected results.
2. If we didn't lock the records, then if we had multiple concurrent readers they might read the data differently if they were reading mid-write. Most likely the entire file will have some corruption while its being written and just give the record readers garbage.