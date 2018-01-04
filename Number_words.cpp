#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <map>
#include <iomanip>

#ifdef _WIN32
#define  win_type 1
#else
#define win_type 0
#endif

using namespace std;

const char *ifilename = "C:/Users/LYB/Desktop/Number_words/123.txt";
const char *ofilename = "C:/Users/LYB/Desktop/Number_words/NumberOfWords.txt";

typedef pair<string, int> PAIR;

struct rCmpByValue {
  bool operator()(const PAIR& lhs, const PAIR& rhs) {
    return lhs.second > rhs.second;
  }
};

string open(const char *filename)
{
    string out;
    ifstream ifile(filename,ios::in|ios::binary);
    if(ifile.is_open())
    {
        if(!win_type)
        {
            ifile.seekg (0, ifile.end);
            int length = ifile.tellg();
            ifile.seekg (0, ifile.beg);

            char * buffer = new char [length];
            ifile.read (buffer,length);
            cout<<buffer<<endl;
            out=buffer;
        }else{
            istreambuf_iterator<char>beg(ifile), end;
            string cache(beg, end);
            out = cache;
        }
    }
    else
        cout<<"Open the file is failed."<<endl;
    return out;
}

void write(vector<PAIR> number_words_sort)
{
    ofstream ofile(ofilename,ios::in|ios::trunc);
    if (ofile.is_open())
    {
        ofile<<setiosflags(ios::left)<<setw(32)<<"单词"<<setiosflags(ios::left)<<setw(6)<<"次数"<<"\n";
        for(int i=0;i<number_words_sort.size();i++)
        {
            ofile<<setiosflags(ios::left)<<setw(32)<<number_words_sort[i].first<<setiosflags(ios::left)<<setw(6)<<number_words_sort[i].second<<"\n";
        }
        ofile.close();
    }
}

vector<string> split(const string &source, const string &separate){
    vector<string> result;
    int vec_start = 0 ,vec_end = 0 , sep_counter = 0, vec_length = 0;
    for(int i=0; i<source.length();i++)
    {
        //cout<<"Count"<<i<<":  ";
        //cout<<source[i]<<"***"<<separate[sep_counter]<<endl;
        if(source[i] == separate[sep_counter])
            if(sep_counter == separate.length()-1)
            {
                vec_length = vec_end - vec_start;
                if (vec_length != 0)
                {
                    //cout<<"PUSH:  "<<source.substr(vec_start,vec_length)<<"**END**"<<endl;
                    result.push_back(source.substr(vec_start,vec_length));
                }
                vec_start = vec_end = i+1;
                sep_counter=0;
            }else{
                sep_counter++;
            }
        else{
            vec_end++;
        }
    }
    if ((vec_end - vec_start)!= 0)
        result.push_back(source.substr(vec_start,vec_end-vec_start));

    return result;
}

vector<string> format_words(vector<string> words_vector)
{
    //char punctuation[] = {'.',',',';','?','!',':','"'};
    string punctuation = ".,;?!:\" \n\t\r";
    for(int i = 0; i< words_vector.size(); ++i)
    {
        transform(words_vector[i].begin(),words_vector[i].end(),words_vector[i].begin(),::tolower);
        size_t found = words_vector[i].find_last_of(punctuation);
        while(found != string::npos){
            //cout<<"found: "<<found<<endl;
            words_vector[i].erase(found,1);
            found = words_vector[i].find_last_of(punctuation);
        };
    }
    return words_vector;
}

map<string,int> words_counter(vector<string> words_vector){
    map<string,int> number_words;
    map<char,int>::iterator it;
    for(int i = 0; i< words_vector.size(); ++i)
    {
        //if (words_vector[i] == "youth")
        //cout<<i<<"  "<<setiosflags(ios::showpos)<<words_vector[i]<<endl;
        if (number_words.find(words_vector[i]) == number_words.end())
            number_words[words_vector[i]] = 1;
        else
            number_words[words_vector[i]] += 1;
    }
    return number_words;
}

vector<PAIR> map_sort(map<string,int> number_words)
{
    vector<PAIR> number_words_sort(number_words.begin(), number_words.end());
    sort(number_words_sort.begin(), number_words_sort.end(), rCmpByValue());

    return number_words_sort;
}
int main()
{
    string a,b;
    const char *seq=" ";
    string out;
    out = open(ifilename);
    cout<<"The file is:"<<endl;
    cout<<"  "<<out<<endl;
    vector<string> split_vector = split(out,seq);
    //for(int i = 0; i< split_vector.size(); ++i)
     //   cout << split_vector[i] << endl;

    vector<string> format_out = format_words(split_vector);
    //for(int i = 0; i< format_out.size(); ++i)
     //   cout << format_out[i] << " *** "<<endl;

    map<string,int> number = words_counter(format_out);
    //map<string,int>::iterator iter;
    //for(iter = number.begin(); iter != number.end(); iter++)
    //    cout<<iter->first<<": "<<iter->second<<endl;
    vector<PAIR> number_sort = map_sort(number);
    //for (int i=0; i<number_sort.size();i++)
    //{
     //   cout<<number_sort[i].first<<"  **** "<<number_sort[i].second<<endl;
    //}
    write(number_sort);

    return 0;
}
