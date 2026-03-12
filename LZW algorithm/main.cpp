//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// I created initial dictionary based on how many characters in the text there are and which appeared first, not based on ASCII values or alphabetical order
//                      for longer text it can take up to about a minute for compression and decompression to end
// to see how compressed/decompressed output looks like, please check out functions below and uncomment output you would like to see
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <bits/stdc++.h>
using namespace std;

void load_file(const string file_name, string &text){
    ifstream read_file(file_name);
    string temp_text = "";
    while (getline(read_file, temp_text)) {
        text += temp_text;
    }
    read_file.close();
    if(text.size() == 0) {
        cout << "File is empty or cannot be opened.\n";
        exit(1);
    }
}

string get_binary(int a, int n=0){
    string binary = "";
    while(a > 0){
        binary = char((a % 2) + '0') + binary;
        a /= 2;
    }
    while(binary.size() < n)
        binary = '0' + binary;

    return binary;
}

int get_n(string &text){
    int n;
    unordered_map<char, bool> chars;
    for(auto t : text){
        chars[t] = true;
    }
    n = get_binary(chars.size()-1).size();
    return n;
}

void print_dictionary(map<string,int> &dictionary){
    for(auto c : dictionary){
        cout << c.first << ": " << c.second << '\n';
    }
}

void print_decompress_dictionary(map<int,string> &dictionary){
    for(auto c : dictionary){
        cout << c.first << ": " << c.second << '\n';
    }
}

void create(string &text, map<string,int> &dictionary){
    int n = get_n(text);
    int i = 0;
    for(auto t : text){
        string temp = "";
        temp += t;
        if(dictionary.find(temp) == dictionary.end()){
            dictionary[temp] = i;
            ++i;
        }
    }
}

void print_output(vector<int> &output){
    for(auto o : output){
        cout << o << ' ';
    }
    cout << '\n';
}

void print_bin_output(vector<string> &bin_output){
   for(auto o : bin_output){
        cout << o;
    }
    cout << '\n';   
}

vector<string> get_bin_output(map<string,int> &dictionary, vector<int> &output){
    int n = get_binary(dictionary.size()-1).size();
    vector<string> bin_output;
    for(int i = 0; i < output.size(); ++i){
        bin_output.emplace_back(get_binary(output[i], n));
    }
    //BINARY ENCODED TEXT
    //cout << "Binary output:\n";
    //print_bin_output(bin_output);
    return bin_output;
}

vector<string> LZW_compression(map<string,int> dictionary, string &text){
    vector<int> output;
    string old_c = "";
    old_c += text[0];
    for(int i = 1; i < text.size(); ++i){
        string new_c = "";
        new_c += text[i];
        string combined = old_c + new_c;

        if(dictionary.find(combined) != dictionary.end()){
            old_c = old_c + new_c;
        } else {
            output.emplace_back(dictionary[old_c]);
            dictionary[combined] = dictionary.size();
            old_c = new_c;
        }
        if(i == text.size() - 1){
            output.emplace_back(dictionary[old_c]);
        }
    }

    //DICTIONARY FOR COMPRESSION
    //cout << "Final dictionary:\n";
    //print_dictionary(dictionary);

    //DECIMAL ENCODED TEXT
    //print_output(output);
    return get_bin_output(dictionary, output);
    
}

void get_decompress_dictionary(map<int,string> &decompress_dictionary, map<string,int> &dictionary){
    for(auto d : dictionary){
        decompress_dictionary[d.second] = d.first;
    }
}

int get_decimal(string &binary){
    int decimal = 0;
    for(int i = 0; i < binary.size(); ++i){
        decimal = decimal * 2 + (binary[i] - '0');
    }
    return decimal;
}

string LZW_decompression(map<string,int> dictionary, vector<string> &bin_output){
    map<int,string> decompress_dictionary;
    get_decompress_dictionary(decompress_dictionary, dictionary);

    string decoded_text = "";

    //DICTIONARY FOR DECOMPRESSION
    //cout << "Initial dictionary\n";
    //print_decompress_dictionary(decompress_dictionary);

    int first = get_decimal(bin_output.front());
    string old = decompress_dictionary[first];
    decoded_text += old;
    for(int i = 1; i < bin_output.size(); ++i){
        int code = get_decimal(bin_output[i]);
        string new_c;
        if(decompress_dictionary.find(code) != decompress_dictionary.end()){
            new_c = decompress_dictionary[code];
            decompress_dictionary[decompress_dictionary.size()] = old + new_c[0];
        } else {
            new_c = old + old[0];
            decompress_dictionary[decompress_dictionary.size()] = new_c;
        }
        decoded_text += new_c;
        old = new_c;
    }

    //DECODED TEXT
    //cout << "decoded text:\n" << decoded_text << '\n';
    return decoded_text;
}

int main(){
    string norm_wiki_text, wiki_text, fuji_02, test_text, original_text, decoded_text;
    vector<string> encoded_text;
    vector<char> norm_wiki_chars;
    map<string,int> dictionary;


    load_file("./lab_lzw/test.in", test_text);
    load_file("./lab_lzw/norm_wiki_sample.txt", norm_wiki_text);
    load_file("./lab_lzw/wiki_sample.txt", wiki_text);
    load_file("./lab_lzw/fuji_02.png", fuji_02);

    original_text = norm_wiki_text; //if you want to see more examples, change text in here to test_text/norm_wiki_text/wiki_text/fuji_02
    create(original_text, dictionary);


    cout << "COMPRESSION\n";
    cout.flush();
    encoded_text = LZW_compression(dictionary, original_text);

    cout << "DECOMPRESSION\n";
    cout.flush();
    decoded_text = LZW_decompression(dictionary, encoded_text);


    if(original_text == decoded_text) cout << "TEXT WAS DECODED SUCCESSFULLY\n";
    else cout << "TEXT WAS DECODED WITH ERRORS\n";
}