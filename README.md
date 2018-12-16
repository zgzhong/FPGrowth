# FP-Growth Algorithm

This is the C++ implement of FP-Growth Algorithm.
Usage:
```cpp
alpha::FPGrowth fpGrowth(numOfRecords, support, confidence);

// This map stores the count of every field
std::map<string size_t> fieldCountMap;
// Build the Frequent Table
fpGrowth.buildFPTable(fieldCountMap);

// Build Frequent Tree
while ( some_loop ) {
    // fetch data
    fpGrowth.addRecord({data1, data2, data3, data4...});
}

```


Yon can run the example for fun. But this code requires boost library installed in your system.
```bash
$ make run
```
