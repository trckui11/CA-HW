/* 216303990 Arbel Feldman */

#include "fmm.h"

/*void fmm(int n, int* m1, int* m2, int* result) {
    register int r, i, j, k;
    register int nn = n * n;
    register int nn1 = nn - 1;
    // first iteration (k=0):
    for (i = 0; i < n; i++) {
        r = *m1;
        m1 += n;
        for (j = 0; j < n; j++)
            *result++ = r * *m2++;  // result[i][j] += m1[i][0] * m2[0][j]
        m2 -= n;
    }
    result -= nn;
    m1 -= nn1;
    m2 += n;
    // multiply (kij):
    for (k = 1; k < n; k++) {
        for (i = 0; i < n; i++) {
            r = *m1;
            m1 += n;
            for (j = 0; j < n; j++) 
                *result++ += r * *m2++; // result[i][j] += m1[i][k] * m2[k][j]
            m2 -= n;
        }
        result -= nn;
        m1 -= nn1;
        m2 += n;
    }
}*/



/*void fmm(int n, int* m1, int* m2, int* result) {
    register int r, i, j, k;
    register int in = 0, kn = 0;
    register int nn = n * n;
    // make all entries 0:
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            *result++ = 0;
        in += n;
    }
    result -= nn;
    // multiply (kij):
    for (k = 0; k < n; k++) {
        in = 0;
        for (i = 0; i < n; i++) {
            r = m1[in + k];
            for (j = 0; j < n; j++) 
                *result++ += r * m2[kn + j];  // result[i][j] += m1[i][k] * m2[k][j]
            in += n;
        }
        kn += n;
        result -= nn;
    }
}*/


/*
void fmm(int n, int* m1, int* m2, int* result) {
    register int i, j, k, s;
    register int in = 0;
    register int kn;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            s = 0;
            result[in + j] = 0;  // result[i][j] = 0
            kn = 0;
            for (k = 0; k < n; k++) {
                s += m1[in + k] * m2[kn + j];  // result[i][j] += m1[i][k] * m2[k][j]
                kn += n;
            }
            result[in + j] = s;
        }
        in += n;
    }
}*/

//GOOD
/*void fmm(int n, int* m1, int* m2, int* result) {
    register int i, j, k, r;
    register int jn = 0;
    register int kn;
    for (j = 0; j < n; j++) {
        for (i = 0; i < n; i++)
            result[jn + i] = 0;  // result[i][j] = 0
        kn = 0;
        for (k = 0; k < n; k++) {
            r = m1[jn + k];
            for (i = 0; i < n; i++) {
                result[jn + i] += m2[kn + i] * r;  // result[i][j] += m1[i][k] * m2[k][j]
            }
            kn += n;
        }
        jn += n;
    }
}*/

// good jki
/*void fmm(int n, int* m1, int* m2, int* result) {
    register int r;
    register int nn = n * n;
    register int n1 = n - 1;
    register int* resEndRow;
    register int* m1EndRow;
    register int* resEnd = result + nn;
    while (result < resEnd) {
        --result;
        resEndRow = result + n;
        while (result < resEndRow)
            *++result = 0;  // result[j][i] = 0
        result -= n1;
        --m1;
        m1EndRow = m1 + n;
        while (m1 < m1EndRow) {
            r = *++m1;
            --m2;
            --result;
            while (result < resEndRow)
                *++result += r * *++m2;  // result[j][i] += m1[j][k] * m2[k][i]
            result -= n1;
            ++m2;
        }
        ++m1;
        result += n;
        m2 -= nn;
    }
}*/

/*inline void fmm(int n, int* m1, int* m2, int* result) {
    register int r;
    register int nn = n * n;
    register int* resEndRow;
    register int* m1EndRow;
    register int* resEnd = result + nn;
    while (result < resEnd) {
        resEndRow = result + n;
        while (result < resEndRow) {// result[j][i] = 0
            *result++ = 0;  
            *result++ = 0;
            *result++ = 0;
            *result++ = 0;
            *result++ = 0;
            *result++ = 0;
            *result++ = 0;
            *result++ = 0;
            *result++ = 0;  
            *result++ = 0;
            *result++ = 0;
            *result++ = 0;
            *result++ = 0;
            *result++ = 0;
            *result++ = 0;
            *result++ = 0;
        }
        result -= n;
        m1EndRow = m1 + n;
        while (m1 < m1EndRow) {
            r = *m1++;
            while (result < resEndRow) { // result[j][i] += m1[j][k] * m2[k][i]
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
            }
            result -= n;
        }
        result += n;
        m2 -= nn;
    }
}*/


inline void fmm(int n, int* m1, int* m2, int* result) {
    register int r;
    register int nn = n * n;
    register int* resEndRow = result;
    register int* m1EndRow = m1;
    register int* resEnd = result + nn;
    while (result < resEnd) {
        m1EndRow += n;
        resEndRow += n;
        r = *m1++;
        while (result < resEndRow) { // result[j][i] = 0 + first iteration
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
            *result++ = r * *m2++;
        }
        result -= n;
        while (m1 < m1EndRow) {
            r = *m1++;
            while (result < resEndRow) { // result[j][i] += m1[j][k] * m2[k][i]
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
                *result++ += r * *m2++;
            }
            result -= n;
        }
        result += n;
        m2 -= nn;
    }
}


/*void fmm(int n, int* m1, int* m2, int* result) {
    register int i, j, k, r;
    register int jn = 0;
    register int kn = 0;
    int* m2colj = malloc(n * sizeof(int));
    for (j = 0; j < n; j++) {
        kn = 0;
        for (k = 0; k < n; k++) {
            m2colj[k] = m2[kn + j];
            kn += n;
        }
        kn = 0;
        for (k = 0; k < n; k++) {
            r = 0;
            for (i = 0; i < n; i++) {
                r += m1[kn + i] * m2colj[i];  // result[i][j] += m1[i][k] * m2[k][j]
            }
            result[kn + j] = r;
            kn += n;
        }
        jn += n;
    }
    free(m2colj);
}*/