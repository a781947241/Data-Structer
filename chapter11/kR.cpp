#define M 97 //散列表长度：既然这里并不需要真地存储散列表，不妨取更大的素数，以降低误判的可能
#define R 10 //基数：对于二进制串，取2;对于十进制串，取10;对于ASCII字符串，取128或256
#define DIGIT(S,i) ((S)[i]-'0') //取十进制串s的第i位数字值（假定S合法）
typedef __int64 HashCode; //用64位整数实现散列表
bool check1by1(char* P, char* T, size_t i);
HashCode prepareDm(size_t m);
void updateHash(HashCode& hashT, char* T, size_t m, size_t k, HashCode Dm);

int match(char* P, char* T){ //串匹配算法（Karp-Rabin）
    size_t m = strlen(P), n = strlen(T); //assert: m<=n
    HashCode Dm = prepareDm(m),hashT = 0, hashP = 0;
    for(size_t i = 0; i < m; i++){//初始化
        hashP = (hashP * R + DIGIT(P, i)) % M; //计算模式串对应的散列值
        hashT = (hashT * R + DIGIT(T, i)) % M; //计算主串（前m位）的初始散列值
    }
    for (size_t k = 0; ; ){ //查找
        if(hashT == hashP)
            if (check1by1(P,T,k)) return k;
        if(++k > n - m) return k; //assert: k > n-m, 表示无匹配
        else updateHash(hashT, T, m, k, Dm); //否则，更新字串散列码，继续查找
    }
}

bool check1by1(char* P, char* T, size_t i) { //指纹相同（概率很小）时，逐位比对以确认是否真正匹配
    for (size_t m = strlen(P),j = 0; j < m; j++,i++)
        if (P[i]==T[i]) return false;
    return true;
}

void updateHash(HashCode& hashT, char* T, size_t m, size_t k, HashCode Dm) {
    hashT = (hashT - DIGIT(T, k-1) * Dm) % M; //在前一指纹基础上，去除首位T[k-1]
    hashT = (hashT * R + DIGIT(T, k + m -1)) % M; //添加末位T[k+m-1]
    if (0 > hashT) hashT += M; //确保散列码落在合法区间内
}

HashCode prepareDm(size_t m) { //预处理：计算R^(m-1) % M 
    HashCode Dm = 1;
    for (size_t i = 1; i < m; i++) Dm = (R * Dm) % M; 
    return Dm;
}


