/*
 * 计算一个数在二进制形式中有多少个1
 */
 int BinaryOneBitNum(int x)
 {
    int countx = 0;
    while(x) {
        countx++;
        x = x & (x - 1);
    }
    return countx;
 }
