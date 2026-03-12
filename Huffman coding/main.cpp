#include <bits/stdc++.h>
using namespace std;

struct Huffman_Tree{
    char data;
    int freq;
    int left = -1, right = -1;
};

void load_file(const string file_name, string &text){
    ifstream read_file(file_name);
    string temp_text = "";
    while (getline(read_file, temp_text)) {
        text += temp_text;
    }
    read_file.close();
}

void get_chars_count(map<char,int> &chars_count, string &text){
    for(auto t : text){
        chars_count[t]++;
    }
}

void dfs(vector<Huffman_Tree> &hf, int v, map<char,string> &Huffman_codes, string code, int count){
    //for(int i = 0; i < count; i++){
    //    cout << ' ';
    //}
    //cout << v << " " << hf[v].data << " " << hf[v].freq <<'\n';
    if(hf[v].data != '-'){
        Huffman_codes[hf[v].data] = code;
        return;
    }
    if(hf[v].left != -1){
        dfs(hf, hf[v].left, Huffman_codes, code+"0", count+1);
    }
    if(hf[v].right != -1){
        dfs(hf, hf[v].right, Huffman_codes, code+"1", count+1);
    }
}

void get_Huffman_codes(map<char,int> chars_count, map<char,string> &Huffman_codes){
    priority_queue<pair<int,pair<char,int>>, vector<pair<int,pair<char,int>>>, greater<pair<int,pair<char,int>>>> pq;
    vector<Huffman_Tree> hf;

    for(auto c : chars_count){
        pq.push({c.second, {c.first, hf.size()}});
        Huffman_Tree new_hf;
        new_hf.data = c.first;
        new_hf.freq = c.second;
        hf.push_back(new_hf);
    }

    while(pq.size() > 1){
        pair<int,pair<char,int>> left = pq.top();
        pq.pop();

        if(pq.empty()) break;

        pair<int,pair<char,int>> right = pq.top();
        pq.pop();

        Huffman_Tree new_hf;
        new_hf.data = '-';
        new_hf.freq = left.first+right.first;
        new_hf.left = left.second.second;
        new_hf.right = right.second.second;
        hf.push_back(new_hf);

        pq.push({left.first+right.first, {'-', hf.size()-1}});
    }
    dfs(hf, hf.size()-1, Huffman_codes, "", 0);
}

void print_Huffman_codes(map<char,string> &Huffman_codes){
    for(auto c : Huffman_codes){
        cout << c.first << ": " << c.second << '\n';
    }
}

void create(string &text, map<char,string> &Huffman_codes){
    map<char,int> chars_count;
    get_chars_count(chars_count, text);
    get_Huffman_codes(chars_count, Huffman_codes);
    print_Huffman_codes(Huffman_codes);
}

void print_encoded_text(string &encoded_text){
    cout << "ENCODED TEXT LENGTH\n" << encoded_text.length() << '\n';
    //cout << "ENCODED TEXT\n" << encoded_text << '\n';
}

void encode(string &text, map<char,string> &Huffman_codes, string &encoded_text){
    encoded_text = "";
    for(auto t : text){
        encoded_text += Huffman_codes[t];
    }
    print_encoded_text(encoded_text);
}

void print_decoded_text(string &decoded_text){
    cout << "text length * 6 (number of bits of a shortest possible fixed-length encoding) = " << decoded_text.length()*6 << '\n';
}

void decode(string &encoded_text, map<char,string> &Huffman_codes, string &decoded_text){
    map<string,char> reversed_Huffman_codes;
    for(auto c : Huffman_codes){
        reversed_Huffman_codes[c.second] = c.first;
    }
    decoded_text = "";
    string temp = "";
    for(auto t : encoded_text){
        temp += t;
        if(reversed_Huffman_codes.find(temp) != reversed_Huffman_codes.end()){
            decoded_text += reversed_Huffman_codes[temp];
            temp = "";
        }
    }
    print_decoded_text(decoded_text);
}

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    string norm_wiki_text, encoded_text, decoded_text;
    vector<char> norm_wiki_chars;
    map<char,string> Huffman_codes;

    load_file("./lab_huffman/norm_wiki_sample.txt", norm_wiki_text);

    create(norm_wiki_text, Huffman_codes);
    encode(norm_wiki_text, Huffman_codes, encoded_text);
    decode(encoded_text, Huffman_codes, decoded_text);
    
    if(norm_wiki_text == decoded_text) cout << "TEXT WAS DECODED SUCCESSFULLY\n";
    else cout << "TEXT WAS DECODED WITH ERRORS\n";
}