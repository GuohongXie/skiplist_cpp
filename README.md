# skiplist-c++

 A tiny KV storage based on skiplist written in C++ language

# interface

* InsertElement
* DeleteElement 
* SearchElement
* DisplayList
* DumpFile 
* LoadFile
* size

# performance data  

## insert

skiplist tree high:18
insert random key

|insert element num (w) | timecost (s)  |
|---|---|
|10 |0.316763 |
|50 |1.86778 |
|100 |4.10648 |

qps: 24.39w

## get

|search element (w) |timecost (s) |skiplist size (w)|
|---|---| --- |
|10|0.47148 |10|
|50|2.56373 |50|
|100|5.43204 |100|

qps:18.41w


# USAGE

Just include skip_list.hpp in your code



# Todo 

