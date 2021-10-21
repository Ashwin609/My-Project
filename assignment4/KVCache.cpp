#include<bits/stdc++.h>
#include "KVFiles.cpp"
#include "pthread.h"
using namespace std;

void remove(string t, queue<string>& q)
{
    queue<string> ref;
    int s = q.size();
    int cnt = 0;

    while (q.front() != t and !q.empty()) {
        ref.push(q.front());
        q.pop();
        cnt++;
    }

    if (q.empty()) {
        cout << "Element not found in Cache!!" << endl;
        while (!ref.empty()) {
            q.push(ref.front());
            ref.pop();
        }
    }

    else {
        q.pop();
        while (!ref.empty()) {
            q.push(ref.front());
            ref.pop();
        }
        int k = s - cnt - 1;
        while (k--) {
            string p = q.front();
            q.pop();
            q.push(p);
        }
    }
}

pthread_mutex_t l;
class LRUCache {
private:
    unordered_map<string,string> mp;
    queue<string> q;
    int len;
public:
    LRUCache(int capacity) {
        len=capacity;
        pthread_mutex_init(&l,NULL);
    }
    string get(string key) {
        pthread_mutex_lock(&l);
        auto it=mp.find(key);
        if(it==mp.end())
        {
            string v = getFromFile(key);
            if(!v.compare("0")) {
                pthread_mutex_unlock(&l);
                return "key not found";
            }
            else{
                add(key,v);
                pthread_mutex_unlock(&l);
                return v;
            }
        }
        remove(key, q);
        q.push(key);
        pthread_mutex_unlock(&l);
        return it->second;
    }

    void add(string key,string value){
        auto it=mp.find(key);
        if(it!=mp.end()) {
            it->second=value;
            remove(key, q);
            q.push(key);
            return;
        }
        else
        {
            if(mp.size()<len) {
                mp[key]=value;
                q.push(key);
                return;
            }
            else
            {
                string cur=q.front();
                q.pop();
                mp.erase(cur);
                mp[key]=value;
                q.push(key);
                return;
            }

        }
    }
    void put(string key, string value) {
        pthread_mutex_lock(&l);
        auto it=mp.find(key);
        if(it!=mp.end()) {
            it->second=value;
            putInFile(key,value);
            remove(key, q);
            q.push(key);
            pthread_mutex_unlock(&l);
            return;
        }
        else
        {
            if(mp.size()<len) {
                mp[key]=value;
                q.push(key);
                putInFile(key,value);
                pthread_mutex_unlock(&l);
                return;
            }
            else
            {
                string cur=q.front();
                q.pop();
                mp.erase(cur);
                mp[key]=value;
                q.push(key);
                putInFile(key,value);
                pthread_mutex_unlock(&l);
                return;
            }

        }
    }
    int del(string key) {
        pthread_mutex_lock(&l);
        mp.erase(key);
        remove(key, q);
        int i = delfromFile(key);
        pthread_mutex_unlock(&l);
        return i;
    }
};

class LFUCache {
public:
    // m tracks the minimum frequency at an instance of time
    // s is the current s of the cache
    // c is the maximum capacity of the cache
    int m, s=0, c=0;
    // The Cache(hashmap) where key and <values,frequency> are stored
    unordered_map<string, pair<string, int>> Cache;
    // Hashmap mapped from frequency to keylist, ie, each frequency is mapped to a list of keys
    unordered_map<int, list<string>> temp1;
    // Hashmap that maps the key to its position in the list freq_keylist[freq]
    unordered_map<string, list<string>::iterator> temp2;


    //constructor for LFU cache
    LFUCache(int capacity) {
        c = capacity;
        pthread_mutex_init(&l,NULL);
    }

    string get(string key) {
        pthread_mutex_lock(&l);
        if(Cache.find(key)==Cache.end()) {
            string v = getFromFile(key);
            if(!v.compare("0")) {
                pthread_mutex_unlock(&l);
                return "key not found";
            }
            else{
                add(key,v);
                pthread_mutex_unlock(&l);
                return v;
            }
        }
        int freq = Cache[key].second;
        temp1[freq].erase(temp2[key]);

        Cache[key].second++;
        temp1[Cache[key].second].push_back(key);

        temp2[key] = --temp1[Cache[key].second].end();

        int z = temp1[m].size();
        if(z==0) m++;
        pthread_mutex_unlock(&l);
        return Cache[key].first;
    }


    void add(string key,string value){
        if(c<=0) return;

        // when the key in found in the cache
        if(Cache.find(key)!=Cache.end()){
            string abcde = get(key);
            Cache[key].first = value;
            return;
        }

        // when key is not found in the cache
        if(s==c){
            //remove the element from cache with min freq
            string remove_key = temp1[m].front();
            temp2.erase(remove_key);
            Cache.erase(remove_key);
            temp1[m].pop_front();

        }

        Cache[key] = {value, 1};
        temp1[1].push_back(key);
        temp2[key] = --temp1[1].end();
        m=1;
        if(s < c) s++;
    }
    void put(string key, string value) {
        pthread_mutex_lock(&l);
        if(c<=0){
            pthread_mutex_unlock(&l);
            return;
        }
        // when the key in found in the cache
        if(Cache.find(key)!=Cache.end()){
            string abcde = get(key);
            Cache[key].first = value;
            putInFile(key,value);
            pthread_mutex_unlock(&l);
            return;
        }

        // when key is not found in the cache
        putInFile(key,value);
        if(s==c){
            //remove the element from cache with min freq
            string remove_key = temp1[m].front();
            temp2.erase(remove_key);
            Cache.erase(remove_key);
            temp1[m].pop_front();
        }

        Cache[key] = {value, 1};
        temp1[1].push_back(key);
        temp2[key] = --temp1[1].end();
        m=1;
        if(s < c) s++;
        pthread_mutex_unlock(&l);
    }
    int del(string key) {
        pthread_mutex_lock(&l);
        int i = delfromFile(key);
        if(Cache.find(key)==Cache.end()){
            cout << "key" << " : " << key << "->" <<"Not present in cache" << "   ";
        }
        else {
            int f = Cache[key].second;
            temp2.erase(key);
            Cache.erase(key);
            temp1[f].remove(key);
            s--;
        }
        pthread_mutex_unlock(&l);
        return i;
    }
};
