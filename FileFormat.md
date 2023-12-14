D2 Dictionary File Format
=========================

Words are stored in one file per word-length.
Each such file partitions words into buckets keyed by the first two characters (head)
and containing the remaining characters (tail).

Little-endian (or host-endian)

## Header

- magic "d2" (2 bytes)
- word length (uint16) (or: uint8 *zero_reserved*; uint8 wordlength)
- number of heads (uint32) (or: uint16 *zero_reserved*; uint16 num_heads)

## Per-Head Bucket Info

- head (char[2])
- zero_reserved (char[2])
- tail_end (uint32) i.e. global index just past last for this head

## Bucket Contents

Tails for each head, in order

## Example

For a simple dictionary containing the following words:
- ABBEY
- ABIDE
- ABODE
- ANNEX
- ANVIL
- BONUS

the dictionary contains the following items
- magic: "d2"
- wordlength: 5
- num_head: 3
- head: "AB"
- tail_end: 3
- head: "AN"
- tail_end: 5
- head: "BO"
- tail_end: 6
- tails: "BEYIDEODENEXVILNUS"

Or as a hex dump:
```
00000000: 64 32 05 00 03 00 00 00 41 42 00 00 03 00 00 00  d2......AB......
00000010: 41 4e 00 00 05 00 00 00 42 4f 00 00 06 00 00 00  AN......BO......
00000020: 42 45 59 49 44 45 4f 44 45 4e 45 58 56 49 4c 4e  BEYIDEODENEXVILN
00000030: 55 53                                            US
```
