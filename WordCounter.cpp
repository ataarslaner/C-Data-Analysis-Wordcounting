#include <iostream>     
#include <fstream>      
#include <sstream>
#include <string> 
#include<chrono>
#include <regex>

using namespace std;

struct KeyVal {
    int value;
    string key;
};

class File {
private:
    ifstream articleFile, stopFile;
    string stopwords[600] = { "" };
    //KeyVal keyVals[20500];
    string keys[20500];
    int values[20500];
    string skip, line, delims;
    int first, second, cnt1, cnt2;
public:
    File() {
        first = 0;
        second = 0;
        cnt1 = 0;
        cnt2 = 0;
        skip = "";
        line = "";
        delims = ";,:.1234567890?!'#+-*%//_(){}^<>|@~$&=[]\\` \n\t";
    }

    void sortWords() {
        int counter = 0;
        for (int i = cnt1-1; i >= 1; i--)   //  'for' loop is used for sorting the numbers in descending order
        {
            for (int j = i - 1; j >= 0; j--)
            {
                if (values[i] > values[j])
                {
                    int desc = values[i];
                    values[i] = values[j];
                    values[j] = desc;
                    string str = keys[i];
                    keys[i] = keys[j];
                    keys[j] = str;
                }
            }
        }
        for (int i = 0; i < 10; i++) {
            cout << " [ " << keys[i] << " : " << values[i] << " ] \n";
        }
    }

    bool isNumber(string str) {
        for (int i = 0; i < str.length(); i++) {
            if (str[i] < '0' || str[i]>'9') {
                return false;
            }
        }
        return true;
    }

    void readArticle(std::string path) {
        chrono::time_point<chrono::high_resolution_clock> start_point, end_point;
        start_point = chrono::high_resolution_clock::now();
        articleFile.open(path);
        if (articleFile.is_open())
        {
            string temp((istreambuf_iterator<char>(articleFile)),
                (istreambuf_iterator<char>()));
           
            size_t found = temp.find("unigramCount");
            while (found != string::npos){
                size_t found2 = temp.find('{', found);
                size_t found3 = temp.find('}', found2 + 1);
                if (found2 != string::npos && found3 != string::npos) 
                {
                    string unigram = temp.substr(found2, found3 - found2);
                    string str = "", word = "";
                    bool inside = false, numF = false, wordF = false;
                    int s = 0, e = 0, ns = 0, ne = 0;
                    /*   size_t fnd1 = unigram.find("\"");
                    size_t fnd2 = unigram.find("\"", fnd1+1);
                  while (fnd1 != string::npos && fnd2 != string::npos)
                    {
                        str = unigram.substr(fnd1+1, fnd2 - fnd1-1);
                        transform(str.begin(), str.end(), str.begin(), ::tolower);
                        size_t fnd3 = unigram.find(":", fnd2);
                        size_t fnd4 = unigram.find(",", fnd3 + 1);
                        string number = unigram.substr(fnd3 + 1, fnd4 - fnd3 - 1);
                        if (isNumber(number)) {
                            int num = stoi(unigram.substr(fnd3 + 1, fnd4 - fnd3 - 1));
                            bool stop = false;
                            int start = 0, end = cnt2;
                            int mid = (start + end) / 2;
                            while (start <= end) {
                                if (str > stopwords[mid]) {
                                    start = mid + 1;
                                    mid = (start + end) / 2;
                                }
                                else if (str < stopwords[mid]) {
                                    end = mid - 1;
                                    mid = (start + end) / 2;
                                }
                                else {
                                    stop = true;
                                    break;
                                }
                            }
                            if (stop != true) {
                                bool isFound = false;
                                for (int k = 0; k < cnt1; k++) {
                                    if (keyVals[k].key == str) {
                                        keyVals[k].value += num;
                                        isFound = true;
                                    }
                                }
                                if (!isFound && cnt1 < 20500) {
                                    keyVals[cnt1].key = str;
                                    keyVals[cnt1].value = num;
                                    cnt1++;
                                }
                            }
                        }
                        
                        fnd1 = unigram.find("\"", fnd4);
                        fnd2 = unigram.find("\"", fnd1 + 1);
                    }*/   
                    
                    for (int i = 0; i < unigram.size(); i++) {
                        
                        if(!wordF){
                            if (!inside && unigram[i] == '"') {
                                inside = true;
                                s = i + 1;
                            }
                            else if (inside && unigram[i] == '"') {
                                inside = false;
                                wordF = true;
                                e = i;
                            }
                            else if (inside == true){
                                if(unigram[i] >= 'a' && unigram[i] <= 'z')
                                    word += unigram[i];
                                else if (unigram[i] >= 'A' && unigram[i] <= 'Z')
                                    word += (unigram[i]+32);
                            }
                        }
                        else {
                            if (!inside && unigram[i] == ':'){
                                ns = i + 1;
                                inside = true;
                            }
                            else if (inside && unigram[i] == ',') {
                                ne = i;
                                inside = false;
                                numF = true;
                            }
                        }
                        if (word == "") {
                            numF = false, wordF = false;
                            continue;
                        }
                        if (wordF && numF) {
                            string number = unigram.substr(ns, ne - ns);
                            if (isNumber(number)) {
                                int num = stoi(unigram.substr(ns, ne - ns));
                                bool stop = false;
                                int start = 0, end = cnt2;
                                int mid = (start + end) / 2;
                                while (start <= end) {
                                    if (word > stopwords[mid]) {
                                        start = mid + 1;
                                        mid = (start + end) / 2;
                                    }
                                    else if (word < stopwords[mid]) {
                                        end = mid - 1;
                                        mid = (start + end) / 2;
                                    }
                                    else {
                                        stop = true;
                                        break;
                                    }
                                }
                                if (stop != true) {
                                    bool isFound = false;
                                    for (int k = 0; k < cnt1; k++) {
                                        if (keys[k] == word) {
                                            values[k] += num;
                                            isFound = true;
                                        }
                                    }
                                    //new word
                                    if (!isFound && cnt1 < 20500) {
                                        keys[cnt1] = word;
                                        values[cnt1] = num;
                                        cnt1++;
                                    }
                                }
                            }
                            wordF = false;
                            numF = false;
                            word = "";
                        }
                    }
                }
                found = temp.find("unigramCount", found3 + 1);
                end_point = chrono::high_resolution_clock::now(); //storing the ending time in end 

                auto start = time_point_cast<chrono::microseconds>(start_point).time_since_epoch().count();
                // casting the time point to microseconds and measuring the time since time epoch

                auto end = time_point_cast<chrono::microseconds>(end_point).time_since_epoch().count();
                if ((end - start) / 1000000 > 10) break;
            }
            
        }

    }

    void readStopWords(std::string path) {
        stopFile.open(path);
        if (stopFile.is_open()) {
            while (getline(stopFile, line))
            {
                std::stringstream ss(line);
                while (std::getline(ss, line, '\n')) {
                    stopwords[cnt2] = line;
                    cnt2++;
                }
            }
        }
    }
};

int main()
{
    chrono::time_point<chrono::high_resolution_clock> start_point, end_point;
    start_point = chrono::high_resolution_clock::now();
    File f;
    f.readStopWords("stopwords.txt");
    f.readArticle("PublicationsDataSet.txt");
    f.sortWords();

    end_point = chrono::high_resolution_clock::now(); //storing the ending time in end 

    auto start = time_point_cast<chrono::microseconds>(start_point).time_since_epoch().count();
    // casting the time point to microseconds and measuring the time since time epoch

    auto end = time_point_cast<chrono::microseconds>(end_point).time_since_epoch().count();

    cout << "Time taken = " << (end - start) / 1000 << " milliseconds" << endl;
    return 0;
}



