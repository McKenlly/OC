#include<bits/stdc++.h>
#include <algorithm>

using namespace std;


struct segment {
    int beg; // индекс первого элемента
    int len; // длина интервала
    segment(){}
    segment(int b, int l) : beg(b), len(l){}
};
// ответ должен быть в диапазоне (32,64]
inline int get_min_size(int n) {
    int r = 0;
    while (n >= 64) {
        n >>= 1;
        r |= n &1;
    }
    return n + r;
}
// Да простит меня Тим Петерсон, но вместо сортировки вставками
// я использую сортировку выбором
void selection_sort(vector<int> &mas, int beg, int last) {
    for (int i=beg; i<last;++i) {
        int min_pos = i;
        for (int j=i+1; j<last; ++j) {
            if (mas[j] < mas[min_pos])
                min_pos = j;
        }
        swap(mas[i],mas[min_pos]);
    }
}
const int dx = 1, dy = 2, dz = 3;
void merge(vector<int> &mas, vector<segment> &seg, bool isXY, vector<int> &tmp) {
    segment f = seg[seg.size() - dy];
    segment s = isXY ? seg[seg.size() - dx] : seg[seg.size() - dz];
    if (f.beg > s.beg) swap(f,s);
    int posF = 0, posS = s.beg, pos = f.beg-1;
    int lstF = f.len, lstS = s.beg + s.len;
    copy(mas.begin() + f.beg + posF, mas.begin() + f.beg + lstF, tmp.begin());
    int fAmount = 0, sAmount = 0;
    while (posF < lstF && posS < lstS) {
        if (tmp[posF] < mas[posS]) {
            mas[++pos] = tmp[posF++];
            ++fAmount; sAmount=0;
            if (fAmount == 7) {
                vector<int>::iterator it = upper_bound(tmp.begin() + posF, tmp.begin() + lstF, mas[posS]);
                copy(tmp.begin() + posF, it, mas.begin() + pos + 1);
                pos += it - (tmp.begin() + posF);
                posF += it - (tmp.begin() + posF);
            }
        }
        else {
            mas[++pos] = mas[posS++];
            fAmount=0; ++sAmount;
            if (sAmount == 7) {
                vector<int>::iterator it = upper_bound(mas.begin() + posS, mas.begin() + lstS, tmp[posF]);
                copy(mas.begin() + posS, it, mas.begin() + pos + 1);
                pos += it - (mas.begin() + posS);
                posS += it - (mas.begin() + posS);
            }
        }
    }
    copy(tmp.begin() + posF, tmp.begin() + lstF, mas.begin() + pos + 1);
}
void try_merge(vector<int> &mas, vector<segment> &seg, vector<int> &tmp, bool is_merge = false) {
    while (seg.size() > 1) {
        int x = seg[seg.size() - dx].len;
        int y = seg.size() < 2 ? 0 : seg[seg.size() - dy].len;
        int z = seg.size() < 3 ? 0 : seg[seg.size() - dz].len;
        if (seg.size() >= 3 && z <= x + y) {
            if (z < x) {
                merge(mas,seg,false,tmp);
                seg[seg.size() - dz].len += seg[seg.size() - dy].len;
                seg[seg.size()- dy] = seg[seg.size()- dx];
                goto POP_BACK;
            }
            else {
                merge(mas,seg,true,tmp);
                seg[seg.size() - dy].len += seg[seg.size() - dx].len;
                goto POP_BACK;
            }
        }
        else if (is_merge || y <= x) {
            merge(mas,seg,true,tmp);
            seg[seg.size() - dy].len += seg[seg.size() - dx].len;
            goto POP_BACK;
        }
        else
            break;
        POP_BACK: seg.pop_back();
    }
}
void tim_sort(vector<int> &mas) {
    int n = mas.size();
    vector<int> tmp(n);
    int min_size = get_min_size(n);
    int beg = 0, size = min_size;
    vector<segment> seg;
    seg.reserve((n-1)/min_size + 1);

    for (int i=0;i<n;i+=min_size) {
        size = min(min_size,n-i);
        selection_sort(mas,i,i+size);
        seg.push_back(segment(i,size));
        try_merge(mas, seg, tmp);
    }
    while (seg.size() != 1) {
        try_merge(mas, seg, tmp, true);
    }
}
int main() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    tim_sort(a);
    for (int i = 0; i < n; i++) {
        cout << a[i];
    }
}