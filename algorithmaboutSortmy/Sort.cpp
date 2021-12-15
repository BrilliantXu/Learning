#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <math.h>
#include <windows.h>
#include <time.h>

using namespace std;

template <typename T>
class Sort
{
private:
    vector<T> arr1;
public:
    Sort(){}
    Sort(vector<T> &num){arr1 = num;}
    
    void println(vector<T> &num) {
        print(num);
        cout << '\n';
    }

    //-------------------****-------------------//    
    //------------------QuickSort---------------//
    int QuickSortAdjust(vector<T> &arr, int left, int right)
    {
        int gate = arr[left];
        while (left < right) {
            while (arr[right] >= gate && left < right) right--;
            if (left < right) arr[left++] = arr[right];         //        这里如果元素是顺序的话，两个相同的元素就不会交换位置，但是如果不是顺序的话。就会变换顺序
            while (arr[left] < gate && left < right) left++;   //------->这也是为什么说快速排序是不稳定的。   
            if (left < right) arr[right--] = arr[left];  
        }
        arr[right] = gate;
        return right;
    }

    void quick_sort(vector<T> &arr, int left, int right) {
        if(left < right) {
            int temp = QuickSortAdjust(arr, left, right);
            quick_sort(arr, left, temp - 1);                    //这里要注意不要写错成调整位置函数的递归，由机理决定关系。
            quick_sort(arr, temp + 1, right);            
        }
        return;
    } 

    void myQuickSort(vector<T> &arr)
    {
        int left = 0, right = arr.size() - 1;
        quick_sort(arr, left, right);
        return;
    }
    //-------------------****-------------------//
    //------------------Heap Sort---------------//
    void heap_sortAdjust(vector<T> &arr, int start, int end) {
        int dad = start;
        int son = dad * 2 + 1;
        while(son <= end) {
            if(son + 1 <= end && arr[son] < arr[son + 1]) son++;
            if(arr[dad] > arr[son] )return ;
            else {
                swap(arr[dad], arr[son]);
                dad = son;
                son = dad * 2 + 1;
            }
        }
    }
    
    void heap_sort(vector<T> &arr, int lenth) {
        int dad = lenth / 2 - 1;
        for(int i = dad; i >= 0; i--) {
            heap_sortAdjust(arr, i, lenth - 1);
        }        
        for(int i = lenth - 1; i > 0; i--) {
            swap(arr[0], arr[i]);
            heap_sortAdjust(arr, 0, i - 1);
        }
    }
    
    void myHeapSort(vector<T> &arr) {
        int lenth = arr.size();
        heap_sort(arr, lenth);
    }

    //-------------------****-------------------//
    //----------------BucketSort-----------=----//      还有点问题OXO

    struct ListNode{
        ListNode(){}
        ListNode(T i):mData(i){}
        T mData;
        ListNode* nest;
    };
    
    ListNode* Bucket_insert(ListNode *head, T val) {
        ListNode dummyNode;
        ListNode *newNode = new ListNode(val);
        ListNode *pre,*curr;
        dummyNode.nest = head;
        pre = &dummyNode;
        curr = head;
        while(NULL!=curr && curr->mData<=val){
                pre = curr;
                curr = curr->nest;
        }
        newNode->nest = curr;
        pre->nest = newNode;
        return dummyNode.nest;
    }

    ListNode* Bucket_merge(ListNode *head1, ListNode *head2) { 
        ListNode dummyNode;
        ListNode *dummy = &dummyNode;
        while(NULL!=head1 && NULL!=head2){
                if(head1->mData <= head2->mData){
                        dummy->nest = head1;
                        head1 = head1->nest;
                }else{
                        dummy->nest = head2;
                        head2 = head2->nest;
                }
                dummy = dummy->nest;
        }
        if(NULL!=head1) dummy->nest = head1;
        if(NULL!=head2) dummy->nest = head2;
       
        return dummyNode.nest;        
    }

    void myBucketSort(vector<T> & arr) {
        int lenth = arr.size(), BUCKET_NUM = lenth / 100;
        vector<ListNode*> buckets(BUCKET_NUM,(ListNode*)(0));
        for(int i = 0; i < lenth; ++i){
            int index = arr[i]/BUCKET_NUM;
            ListNode *head = buckets.at(index);
            buckets.at(index) = Bucket_insert(head,arr[i]);
        }
        ListNode *head = buckets.at(0);
        for(int i=1; i < BUCKET_NUM; ++i){
                head = Bucket_merge(head,buckets.at(i));
        }
        for(int i=0; i<lenth; ++i){
                arr[i] = head->mData;
                head = head->nest;
        }
    }

    //-------------------****-------------------//
    //-----------------InsertSort---------------//

    void myInsertSort(vector<T> &arr) {                       //类似划分已排序区域以及未排序区域，逐步添加。
        for(int i = 1; i < arr.size(); i++) {               //在思想上和快排有一定类似，只不过快排的划分范围是全体，不像插入是逐步添加的
            int key = arr[i];
            int j = i - 1;
            while(j >= 0 && arr[j] > key) {
                arr[j] = arr[j + 1];
                j--;
            }
            arr[j] = key;
        }
    }

    //-------------------****-------------------//      
    //-----------------CountSort----------------//
 


    void myCountSort(vector<int> &arr) {                    
        vector<int>cnt(arr.size());              
        //vecotr<T>::iterator it = arr.begin();         
        for(int val : arr) {
            cnt[val]++;
        }
        for(int i = 1; i < cnt.size(); i++) {
            cnt[i] = cnt[i] + cnt[i - 1];
        }
        auto it = arr.begin();
        if(cnt[0]) {
            int temp = 0;
            generate(it, it + cnt[0], [temp]{
                return temp;
            });
            it = (it + cnt[0]);
        }
        for(int i = 1; i < cnt.size(); i++) {
            if(cnt[i] - cnt[i - 1] > 0) {
                int count = cnt[i] - cnt[i - 1];
                int temp = i;
                generate(it, it + count, [temp]{
                    return temp;
                });
                it = it + count;
            }
        }
    }


    //-------------------****-----------------------//
    //-----------------RadixSort_test---------------//   

    int SearchMaxBit(vector<T> &arr) {
        int max1 = INT_MIN;
        for(int i = 0; i < arr.size(); i++) {
            max1 = max(max1, arr[i]); 
        }
        int Bits = 0;
        while(max1 > 0 ) {
            max1 /= 10;
            Bits++;
        }
        return Bits;
    }

    void myRadixSort(vector<T> &arr) {
        int lenth = arr.size();
        int MaxBit = SearchMaxBit(arr);
        vector<T> temp(lenth);
        vector<int> cnt(10);
        int index = 1, k;
        for(int i = 0; i < MaxBit; i++) {
            cnt.assign(10,0);
            for(int j = 0; j < lenth; j++) {
                k = (arr[j] / index) % 10;
                cnt[k]++;
            }
            for(int i = 1; i < 10; i++) {
                cnt[i] = cnt[i] + cnt[i - 1];
            }
            for(int j = lenth - 1; j >=0; j--) {
                k = (arr[j] / index) % 10;
                temp[cnt[k] - 1] = arr[j];
                cnt[k]--;
            }
            arr.assign(temp.begin(),temp.end());
            index = index * 10;
        }
        return;
    }
    
    //-------------------****-------------------//
    //-----------------MegerSort---------------//    
    void Merge(vector<T> &arr, int begin, int mid, int end) {
        vector<T>LeftSide(arr.begin() + begin , arr.begin() + mid + 1);
        vector<T>RightSide(arr.begin() + mid + 1, arr.begin() + end + 1);   //创建是否正确？ 错误理解了其构造曾经产生了一些构造问题
        int LeftIndex = 0, RightIndex = 0;
        LeftSide.push_back(INT16_MAX); RightSide.push_back(INT16_MAX);
        for(int i = 0; i <= end - begin; i++) {
            if(LeftSide[LeftIndex] < RightSide[RightIndex]) {
                arr[begin + i] = LeftSide[LeftIndex++];
            }else {
                arr[begin + i] = RightSide[RightIndex++];
            }
        }
    }

    void Merge_Sort(vector<T> &arr, int begin, int end) {
        if(begin >= end) return;
        int mid = (begin + end) / 2;
        Merge_Sort(arr, begin, mid);
        Merge_Sort(arr, mid + 1, end);
        Merge(arr, begin, mid, end);
    } 

    void myMergeSort(vector<T> &arr) {
        int lenth = arr.size();
        Merge_Sort(arr, 0, lenth - 1);
        return;
    }


    //-------------------****-------------------//
    //-------------------****-------------------//
};

int main() {
    Sort<int> S1;
    clock_t tStart, tEnd;
    double total_t;
    vector<int> int1(1000000);
    for(int j = 0; j < 7; j++) {
        srand(time(0));
        for(int i = 0; i < 1000000; i++) {
            int1[i] = (rand() % INT_MAX);
        }
        tStart = clock();
        switch (j) {
            case 0: S1.myCountSort(int1);break;
            case 1: S1.myHeapSort(int1);break;
            case 2: S1.myQuickSort(int1);break;
            case 3: S1.myRadixSort(int1);break;
            case 4: S1.myMergeSort(int1);break;
            case 5: S1.myInsertSort(int1);break;
            //case 6: S1.myBucketSort(int1);]break;
            default:break;
        }
        tEnd = clock();
        total_t = (double)(tEnd - tStart) / CLOCKS_PER_SEC;
        switch (j) {
            case 0:cout << "myCountSort time = " << total_t << "s" << endl; break;  
            case 1:cout << "myHeapSort time = " << total_t << "s" << endl;  break; 
            case 2:cout << "myQuickSort time = " << total_t << "s" << endl; break;  
            case 3:cout << "myRadixSort time = " << total_t << "s" << endl; break;  
            case 4:cout << "myMergeSort time = " << total_t << "s" << endl; break;  
            case 5:cout << "myInsertSort time = " << total_t << "s" << endl;break;       
            case 6:cout << "myBucketSort time = " << total_t << "s" << endl; break;         
            default:break;
        }
    }
    return 0;
}
