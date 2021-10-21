#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;

string getFromFile(string key)
{

    fstream file;
    file.open("../../storage/a.txt",ios::in);
    if(file.is_open())
    {
        string line,k,v;
        v = "0";
        while (getline(file, line))
        {
            stringstream stream(line);
            getline(stream, k,' ');
            if(k == key)
            {
                getline(stream,v);
                break;
            }
        }
        file.close();
        return v;
    }
    return "0";
}
void putInFile(string key, string value)
{
    string line,k,v,add;;
    ifstream in("../../storage/a.txt");
    if( !in.is_open())
    {
        in.close();
        ofstream out("../../storage/a.txt");
        add = key+' '+value;
        out << add << "\n";
        out.close();
        return;
    }
    ofstream out("../../storage/o.txt");
    while( getline(in,line) )
    {
        stringstream stream(line);
        getline(stream, k,' ');
        if(k != key)
        {
            getline(stream, v);
            add = k+' '+v;
            out << add << "\n";
        }
    }
    add = key+' '+value;
    out << add << "\n";
    in.close();
    out.close();
    remove("../../storage/a.txt");
    rename("../../storage/o.txt","../../storage/a.txt");
    return;
}

int delfromFile(string key)
{
    string line,k,v,add;
    int flag=0;
    ifstream in("../../storage/a.txt");
    if( !in.is_open())
    {
        return -1;
    }
    ofstream out("../../storage/o.txt");
    while( getline(in,line) )
    {
        stringstream stream(line);
        getline(stream, k,' ');
        if(k != key)
        {
            getline(stream, v);
            add = k+' '+v;
            out << add << "\n";
        }
        else
        {
            flag=1;
        }
    }
    in.close();
    out.close();
    remove("../../storage/a.txt");
    rename("../../storage/o.txt","../../storage/a.txt");
    if (flag==0)
    {
        return -2;
    }
    return 0;
}