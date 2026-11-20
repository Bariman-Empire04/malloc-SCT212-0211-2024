
## List Organization
Blocks are linked in ascending address order:
`head → Block1 → Block2 → Block3 → NULL`
Both free and used blocks are on the same list.
Free blocks are marked with `free = 1`.

## How I Coalesce
In `my_free()`, after marking a block free:
1. If `block->next` is free: merge them (add next's header + size to current size, skip next)
2. If previous block is free: merge current into previous

## Task Completion
| Task | Description | Status |
|------|-------------|--------|
| 1 | my_malloc: first-fit, sbrk | ✅ |
| 2 | my_malloc: best-fit | ✅ |
| 3 | Splitting | ✅ |
| 4 | my_free + coalescing | ✅ |
| 5 | my_calloc, my_realloc | ✅ |
| 6 | heap_stats, heap_dump, heap_check | ✅ |
| 7 | Alignment | ✅ |
| 8 | Benchmarks | ✅ |

## make output
(paste the last few lines of your `make` output here)

## Known Limitations
- realloc does not try to extend in-place
- No thread safety

## Sources
- sbrk(2) man page
- Understanding the C standard library allocator (CS lecture notes)
- AI tools: used for code review# malloc-SCT212-0211-2024
