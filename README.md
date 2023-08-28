# skiplist-cpp

 A tiny KV storage based on skiplist written in C++ language

## interface

* `InsertElement`
* `DeleteElement`
* `SearchElement`
* `DisplayList`
* `DumpFile`
* `LoadFile`
* `size`

## performance data  
测试环境：
- Intel i7-7700HQ 
- 16 G main memory

### insert

skiplist tree high: 18
insert random key

|insert element num (w) | timecost (s)  |
|---|---|
| 10 | 1.38421 |
| 50 | 7.13557 |
| 100 | 15.3014 |

qps: 7.007w

### get

|search element (w) |timecost (s) |skiplist size (w)|
|---|---| --- |
| 10 | 1.98734 | 10 |
| 50 | 10.3111 | 50 |
| 100 | 20.5816 | 100 |

qps: 4.84914w


## USAGE

Just include skip_list.hpp in your code
