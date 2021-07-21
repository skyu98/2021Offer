#include <iostream>
using namespace std;
struct ListNode {
    int val;
    ListNode* next;
    ListNode(): val(0), next(nullptr) {}
    ListNode(int value): val(value), next(nullptr) {}
};

bool isPalindrome(ListNode* head) {
    if(!head || !head->next) return true;
    ListNode* slow = head, *fast = head;
    while(fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    ListNode* cur = slow->next, *nxt = cur, *pre = nullptr;
    while(cur) {   
        nxt = cur->next;
        cur->next = pre;
        pre = cur;
        cur = nxt;
    }

    cur = pre;
    slow = head;
    
    while(cur) {
        if(slow->val != cur->val) {
            return false;
        }
        slow = slow->next;
        cur = cur->next;
    }
    return true;
}

int main() {
    int count = 0;
    cin >> count;

    ListNode* hair = new ListNode();
    ListNode* cur = hair;
    while (count--) {
        int val = 0;
        cin >> val;
        cur->next = new ListNode(val);
        cur = cur->next;
    }
    
    cout << (isPalindrome(hair->next) ? "True" : "False")<< endl;

    cur = hair->next;
    while(cur) {
        delete hair;
        hair = cur;
        cur = cur->next;
    }
    return 0;
}