
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                            To see solutions for the main task (task 6) go to the main function
//               please compile using g++ 12.2.0 or higher and "g++ -std=c++20 -O2" command if it is possible
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <random>
#include <chrono>

using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());


void read_file(string filename, string &text){ //for all tasks
    ifstream read_file(filename);
    while (getline(read_file, text)) {
        //cout << *text;
        continue;
    }
    read_file.close();
}

vector<char> get_characters(string &text){ //for all tasks
    vector<char> characters;
    map<char,bool> is_char;
    for(auto c : text){
        is_char[c] = true;
    }
    for(auto found : is_char){
        characters.push_back(found.first);
    }
    return characters;
}

void resize_str(string &text,  string &gen_text, int &text_len){ //for tasks 2-5
    if (text_len == -1){
        text_len = (text).size();
    }
    gen_text.resize(text_len);
}

void zero_order(string &text, string &gen_text, vector<char> characters, int text_len = -1){ //for tasks 2-5
    resize_str(text, gen_text, text_len);

    for(int i = 0; i < text_len; i++){
        int randomInt = rng()%characters.size();
        gen_text[i] = characters[randomInt];
    }
}

map<char, int> frequency_char(string &text){ //for tasks 3-4
    map<char, int> freq;
    for(auto c : text){
        freq[c]++;
    }
    return freq;
}

map<char, double> probability_char(map<char, int> freq, string &text){ //for tasks 3-4
    map<char, double> prob;
    for(auto f : freq){
        double floatValue = static_cast<double>(f.second);
        prob[f.first] = (floatValue /= static_cast<double>(text.size()));
    }
    return prob;
}

char gen_letter(map<char, double> prob){ //for tasks 4-6
    vector<char> c;
    vector<double> p;
    for (auto pair : prob){
        c.push_back(pair.first);
        p.push_back(pair.second);
    }

    random_device rd;
    mt19937 engine(rd());

    discrete_distribution<> dist(p.begin(), p.end());

    return c[dist(engine)];
}

void first_order_appx(string &text, string &gen_text, int text_len = -1){ //for task 4
    resize_str(text, gen_text, text_len);
    map<char, int> freq = frequency_char(text);
    map<char, double> prob = probability_char(freq, text);

    for(int i = 0; i < text_len; i++){
        char letter = gen_letter(prob);
        gen_text[i] = letter;
    }
}

void get_cond_freq(map<string,map<char, double>> &cond_freq, string &text, int order){ //for tasks 5-6
    string curr_chars = text.substr(0, order);
    int chars_it = 0;

    for(int i = order; i < text.size(); i++){
        cond_freq[curr_chars][text[i]]++;
        chars_it++;
        curr_chars = text.substr(chars_it, order);
    }
}

void get_sentence_prob(map<string, double> &sentence_prob, string &text, int order){ //for task 6
    string curr_chars = text.substr(0, order);
    int chars_it = 0;

    for(int i = order; i < text.size()-1; i++){
        sentence_prob[curr_chars]++;
        chars_it++;
        curr_chars = text.substr(chars_it, order);
    }
}

void get_cond_prob(map<string,map<char, double>> &cond_prob, string &text, int order){ //for tasks 5-6
    map<string,map<char, double>> cond_freq;
    get_cond_freq(cond_freq, text, order);
    cond_prob = cond_freq;
    
    map<string, double> sentence_prob;
    get_sentence_prob(sentence_prob, text, order);

    for(auto &[substring, freq] : cond_prob){
        for(auto &letter : freq) {
             letter.second /= sentence_prob[substring];
        }
    }
}

void get_cond_prob_all_orders(vector<map<string,map<char, double>>> &cond_prob_all_orders, string &text, int order){
    cond_prob_all_orders.resize(order+1);
    for(int i = 1; i <= order; i++){
        get_cond_prob(cond_prob_all_orders[i], text, i);
    }
}

void calc_begin_markov(string &text, string &gen_text, vector<char> characters, int order, int &starting_point, int text_len = -1, string starting_text = ""){ //for task 6
    if(starting_text == ""){
        starting_point = order;
        zero_order(text, gen_text, characters, 1);
        vector<map<string,map<char, double>>> cond_prob_all_orders;
        get_cond_prob_all_orders(cond_prob_all_orders, text, order-1);
        resize_str(text, gen_text, text_len);

        string curr_substr = "";
        curr_substr.push_back(gen_text[0]);
        for(int i = 1; i < order; i++){
            int order_it = i;
            string temp_curr_substr = curr_substr;
            while(cond_prob_all_orders[order_it][temp_curr_substr].empty()){
                order_it--;
                temp_curr_substr.erase(temp_curr_substr.begin());
            }
            char letter = gen_letter(cond_prob_all_orders[order_it][temp_curr_substr]);
            curr_substr.push_back(letter);
            gen_text[i] = letter; 
        }

    } else {
        starting_point = starting_text.size();
        gen_text = starting_text;
        resize_str(text, gen_text, text_len);
    }
}

void calc_markov_sources(string &text, string &gen_text, vector<char> characters, int order, int text_len = -1, string starting_text = ""){ //for task 6
    int starting_point;
    calc_begin_markov(text, gen_text, characters, order, starting_point, text_len, starting_text);
    vector<map<string,map<char, double>>> cond_prob_all_orders;
    get_cond_prob_all_orders(cond_prob_all_orders, text, order);

    string curr_substr = gen_text.substr(starting_point-order, order);
    for(int i = starting_point; i < text_len; i++){
        int order_it = order;
        string temp_curr_substr = curr_substr;
        while(cond_prob_all_orders[order_it][temp_curr_substr].empty()){
            order_it--;
            temp_curr_substr.erase(temp_curr_substr.begin());
        }
        char letter = gen_letter(cond_prob_all_orders[order_it][temp_curr_substr]);
        curr_substr.push_back(letter);
        curr_substr.erase(curr_substr.begin());
        gen_text[i] = letter;
    }
}

double get_avg_length(string &text){
    double no_words = 0;
    double letters_sum = 0;
    for(int i = 0; i < text.size()-1; i++){
        if(text[i] != ' ' && text[i+1] == ' '){
            no_words++;
        }
        if (text[i] != ' '){
            letters_sum++;
        }
    }
    return letters_sum/no_words;
}

int main(){
    //task 1
    string norm_hamlet, norm_romeo_and_juliet, norm_wiki_sample;

    read_file("./lab1/norm_hamlet.txt", norm_hamlet);
    read_file("./lab1/norm_romeo_and_juliet.txt", norm_romeo_and_juliet);
    read_file("./lab1/norm_wiki_sample.txt", norm_wiki_sample);


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ choose text ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    string CHOSEN_TEXT = norm_hamlet;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


    vector<char> characters = get_characters(CHOSEN_TEXT);

    //task 2
    /*string zero_order_text;
    zero_order(CHOSEN_TEXT, zero_order_text, characters);
    cout << zero_order_text << '\n';*/

    //tasks 3-4
    /*string first_order_text;
    first_order_appx(CHOSEN_TEXT, first_order_text);
    cout << first_order_text << '\n';*/


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ task 6 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    //~~~~~~~~~~~~  Approximations based on Markov sources - first order ~~~~~~~~~~~~~
    string first_order_markov_text = "";
    calc_markov_sources(CHOSEN_TEXT, first_order_markov_text, characters, 1, 100);
    cout << "first-order Markov source: " << first_order_markov_text << '\n';

    //~~~~~~~~~~~~  Approximations based on Markov sources - third order ~~~~~~~~~~~~~
    string third_order_markov_text = "";
    calc_markov_sources(CHOSEN_TEXT, third_order_markov_text, characters, 3, 100);
    cout << "third-order Markov source: " << third_order_markov_text << '\n';

    //~~~~~~~~~~~~  Approximations based on Markov sources - fifth order ~~~~~~~~~~~~~
    string fifth_order_markov_text = "";
    calc_markov_sources(CHOSEN_TEXT, fifth_order_markov_text, characters, 5, 100, "probability");
    cout << "fifth-order Markov source: " << fifth_order_markov_text << '\n';

    //average length of words
    cout << "average length of words in first, third and fifth order: " << get_avg_length(first_order_markov_text) << ", " << get_avg_length(third_order_markov_text) << ", " << get_avg_length(fifth_order_markov_text) << '\n';
}