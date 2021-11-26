#include <stdio.h>

int recaman(int n, int *seq, int *seq_len) {
    int res;

    if (n == 0) {
        res = 0;
        goto add_to_seq;
    } else {
        int t = recaman(n - 1, seq, seq_len);
        int a = t - n;
        int b = t + n;

        if (a > 0) {
            for (int i = 0; i < *seq_len; i++) {
                if (seq[i] == a) {
                    res = b;
                    goto add_to_seq;
                }
            }
            res = a;
        } else {
            res = b;
        }
    }

add_to_seq:
    seq[*seq_len] = res;
    *seq_len += 1;
    return res;
}

int main () {
    int n = 200;
    int seq[205]={0};
    int seq_len = 0;
    scanf("%d", &n);
    recaman(n, seq, &seq_len);
    printf("%d\n", seq[n]);

    return 0;
}
