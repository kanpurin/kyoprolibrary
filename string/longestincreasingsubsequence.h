#ifndef _LIS_H_
#define _LIS_H_

#include <vector>
#include <algorithm>
#include <limits>

template<typename T>
T LIS(const std::vector<T> &A) {
    std::vector<T> lis(A.size(), std::numeric_limits<T>::max());
    for (int i = 0; i < (int)A.size(); i++)
    {
        lis[std::lower_bound(lis.begin(), lis.end(), A[i]) - lis.begin()] = A[i];
        // 非減少時
        // lis[std::upper_bound(lis.begin(), lis.end(), A[i]) - lis.begin()] = A[i]; 
    }
    return std::lower_bound(lis.begin(), lis.end(), std::numeric_limits<T>::max()) - lis.begin();
}

#endif
