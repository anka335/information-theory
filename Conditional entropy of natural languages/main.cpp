//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  this code will show all solutions to one text simultaneously. This means that it can be pretty slow - going for about 1 min. for each order
//  it does not take any input from user
//  I used plots created in python to compare entropies visually (see them in lab3/plots)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <bits/stdc++.h>
using namespace std;

void check_text(string &text, vector<string> &words, vector<string> &chars){
    cout << "text size: " << text.size() << '\n';
    cout << "words size: " << words.size() << '\n';
    cout << "first word: " << words[0] << '\n';
    cout << "last word: " << words.back() << '\n';
    cout << "chars size:" << chars.size() << '\n';
    cout << "first char: " << chars[0] << '\n';
    cout << "last char: " << chars.back() << '\n';
    //for(int i = 0; i < words.size(); ++i){
    //    cout << words[i] << '\n';
    //}
}

void check_count(map<vector<string>,int> num, int total_count){
    for(auto j : num){
        for(auto c : j.first){
            cout << c << ' ';
        }
        cout << " " << j.second << '\n';
    }
    //cout << total_count << '\n';
}

void check_probability(map<string,double> prob){
    double sum = 0.0;
    for(auto j : prob){
        sum += j.second;
    }
    cout << "SUM: " << sum << '\n';
}

void check_joint_probability(map<vector<string>, double> joint_prob){
    double sum = 0.0;
    for(auto j : joint_prob){
        /*for(auto c : j.first){
            cout << c << ' ';
        }
        cout << " " << j.second << '\n';*/
        sum += j.second;
    }
    cout << "SUM: " << sum << '\n';
}

string format_context(const vector<string>& context_vec) {
    if (context_vec.empty()) {
        return "(START)";
    }
    string result = "";
    for (size_t i = 0; i < context_vec.size(); ++i) {
        result += context_vec[i];
        if (i < context_vec.size() - 1) {
            result += " ";
        }
    }
    return result;
}

void check_cond_probability(vector<map<vector<string>, map<string, double>>> &cond_prob_all_orders) {
    for(size_t order = 0; order < cond_prob_all_orders.size(); ++order){
        cout << "\n\n\n### Order: " << order << '\n';
        
        for(const auto& a : cond_prob_all_orders[order]){
            const vector<string>& context_vec = a.first; 
            
            double sum = 0.0;
            
            string context_str = format_context(context_vec);
            
            //cout << "Context: \"" << context_str << "\"\n";
            
            for(const auto& b : a.second){
                sum += b.second;
                //cout << " P(\"" << b.first << "\" | \"" << context_str << "\") = " << b.second << '\n';
            }
            
            //cout << ">>> SUM for P(* | \"" << context_str << "\") = **" << sum << "**\n";
            
            if (abs(sum - 1.0) > 1e-2) {
                cout << ">>> SUM for P(* | \"" << context_str << "\") = **" << sum << "**\n";
                cout << "!!! WARNING: Sum deviates significantly from 1.0\n";
                //return;
            }
            //cout << "------------------------------------------\n";
        }
    }
}

void get_chars(string &text, vector<string> &chars){
    for(auto t : text){
        string temp = "";
        temp += t;
        chars.emplace_back(temp);
    }
}

void get_words(string &text, vector<string> &words){
    string word = "";
    for(auto t : text){
        if(t == ' ' && word != ""){
            words.emplace_back(word);
            word = "";
        } else if(t == ' '){
            word = "";
        } else{
            word += t;
        }
    }
    if(word != "") words.emplace_back(word);
}

void load_file(string file_name, string &text, vector<string> &words, vector<string> &chars){
    ifstream read_file(file_name);
    while (getline(read_file, text)) {}
    read_file.close();
    get_words(text, words);
    get_chars(text, chars);
}

void count_words(const vector<string> &words, map<vector<string>,int> &words_num, int rank = 1){
    vector<string> curr;
    for(int i = 0; i < rank; ++i) curr.emplace_back(words[i]);
    words_num[curr]++;

    for(int i = rank; i < words.size(); ++i){
        curr.erase(curr.begin());
        curr.emplace_back(words[i]);
        words_num[curr]++;
    }
}

int get_total_sum(const map<vector<string>,int> num = {}){
    int total_sum = 0;
    for (const auto& pair : num) {
        total_sum += pair.second;
    }
    return total_sum;
}

void get_probability(vector<string> words, map<string,double> &prob){
    map<string,int> count;
    for(auto w : words) count[w]++;
    for(const auto& pair : count)
        prob[pair.first] = double(pair.second) / double(words.size());
    //check_probability(prob);
    //for(auto p : prob) cout << p.first << " = " << p.second << "   " << count[p.first] << '\n';
}

void get_joint_probability(map<vector<string>, double> &joint_prob, vector<string> &words, int rank = 1){
    map<vector<string>,int> num;

    count_words(words, num, rank);

    int total_sum = get_total_sum(num);
    //check_count(num, total_sum);
    
    for(auto c : num){
        joint_prob[c.first] = double(c.second)/double(total_sum);
    }
}

void get_joint_entropy(vector<string> &words, int rank = 1){
    map<vector<string>, double> joint_prob;
    get_joint_probability(joint_prob, words, rank);
    check_joint_probability(joint_prob);

    double joint_entropy = 0.0;
    for(auto j : joint_prob){
        joint_entropy -= joint_prob[j.first]*log2(joint_prob[j.first]);
    }
    cout << "joint entropy: " << joint_entropy << '\n';

}

void get_cond_freq(map<vector<string>,map<string, double>> &cond_freq, vector<string> &words, int order){
    vector<string> curr;
    for(int i = 0; i < order; ++i) curr.emplace_back(words[i]);

    for(int i = order; i < words.size(); i++){
        cond_freq[curr][words[i]]++;
        curr.erase(curr.begin());
        curr.emplace_back(words[i]);
    }
}

void get_sentence_prob(map<vector<string>, double> &sentence_prob, vector<string> &words, int order){
    vector<string> curr;
    for(int i = 0; i < order; ++i) curr.emplace_back(words[i]);

    for(int i = order; i < words.size(); i++){
        sentence_prob[curr]++;
        curr.erase(curr.begin());
        curr.emplace_back(words[i]);
    }
}

void get_cond_prob(map<vector<string>,map<string, double>> &cond_prob, vector<string> &words, int order){
    map<vector<string>,map<string, double>> cond_freq;
    get_cond_freq(cond_freq, words, order);
    cond_prob = cond_freq;
    
    map<vector<string>, double> sentence_prob;
    get_sentence_prob(sentence_prob, words, order);

    for(auto &[substring, freq] : cond_prob){
        for(auto &letter : freq) {
             letter.second /= sentence_prob[substring];
        }
    }
}

void get_cond_prob_all_orders(vector<map<vector<string>,map<string, double>>> &cond_prob_all_orders, vector<string> &words, int order){
    cond_prob_all_orders.resize(order+1);
    for(int i = 1; i <= order; i++){
        get_cond_prob(cond_prob_all_orders[i], words, i);
    }
}

double get_entropy(vector<string> &words){
    map<string,double> prob;
    get_probability(words, prob);
    
    double entropy = 0.0;
    for(auto p : prob){
        entropy -= p.second*log2(p.second);
        //cout << prob[w] << " * " << log2(prob[w]) << '\n';
    }
    return entropy;
}

double get_conditional_entropy(vector<string> &words, int order, vector<map<vector<string>,map<string,double>>> &cond_prob_all_orders){
    map<vector<string>, double> joint_prob;
    get_joint_probability(joint_prob, words, order+1);

    double cond_entropy = 0.0;
    for(const auto& a : cond_prob_all_orders[order]){
        const vector<string>& y = a.first; 
        vector<string> y_x = y;             
        for(const auto& b : a.second){
            const string& x = b.first;        
            double cond_prob = b.second;      
            
            y_x.emplace_back(x);
            //cout << joint_prob[y_x] << " " << cond_prob << '\n';
            cond_entropy -= joint_prob[y_x] * log2(cond_prob);
            
            //cout << cond_entropy << '\n';
            
            y_x.pop_back(); 
        }
    }
    return cond_entropy;
    //cout << "order: " << order << " cond entropy: " << cond_entropy << '\n';
}

void get_all_orders_cond_entropy(vector<string> &words, int max_order){
    vector<map<vector<string>,map<string,double>>> cond_prob_all_orders;
    get_cond_prob_all_orders(cond_prob_all_orders, words, max_order);
    //check_cond_probability(cond_prob_all_orders);
    vector<double> entropies(max_order+1);

    entropies[0] = get_entropy(words);
    for(int i = 1; i <= max_order; ++i){
        entropies[i] = get_conditional_entropy(words, i, cond_prob_all_orders);
    }
    for(int i = 0; i <= max_order; ++i){
        cout << i << ": " << entropies[i] << '\n';
    }
}

int main(){
    const int ORDER = 3;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //                                norm_wiki_en and norm_wiki_la
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    string norm_wiki_en_text, norm_wiki_la_text;
    vector<string> norm_wiki_en_words, norm_wiki_en_chars, norm_wiki_la_words, norm_wiki_la_chars;

    load_file("./lab3/norm_wiki_en.txt", norm_wiki_en_text, norm_wiki_en_words, norm_wiki_en_chars);
    //check_text(norm_wiki_en_text, norm_wiki_en_words, norm_wiki_en_chars);

    load_file("./lab3/norm_wiki_la.txt", norm_wiki_la_text, norm_wiki_la_words, norm_wiki_la_chars);
    //check_text(norm_wiki_la_text, norm_wiki_la_words, norm_wiki_la_chars);

    cout << "ENTROPIES FOR ENGLISH LETTERS:\n";
    get_all_orders_cond_entropy(norm_wiki_en_chars, ORDER);
    cout << "ENTROPIES FOR ENGLISH WORDS:\n";
    get_all_orders_cond_entropy(norm_wiki_en_words, ORDER);
    cout << "ENTROPIES FOR LATIN LETTERS:\n";
    get_all_orders_cond_entropy(norm_wiki_la_chars, ORDER);
    cout << "ENTROPIES FOR LATIN WORDS:\n";
    get_all_orders_cond_entropy(norm_wiki_la_words, ORDER);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //                                      SAMPLES
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    /*vector<string> samples(6);
    vector<vector<string>> sample_words(6), sample_chars(6);

    for(int i = 0; i < 6; ++i){
        string file_path = "./lab3/sample";
        file_path += char(i+'0');
        file_path += ".txt";
        co  ut << file_path << '\n';
        load_file(file_path, samples[i], sample_words[i], sample_chars[i]);
        //check_text(samples[i], sample_words[i], sample_chars[i]);
        cout << "sample" << i << " words\n";
        get_all_orders_cond_entropy(sample_words[i], ORDER);
        cout << "sample" << i << " letters\n";
        get_all_orders_cond_entropy(sample_chars[i], ORDER);
    }*/
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //                                      GUESSING
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    /*
    sample0 - it is not a natural language, because order 1 and order 2 values for entropy of words are too high
    sample1 - it is a natural language, because values and plots for entropy are similar to english and latin and differ only by about 10%
    sample2 - it is not a natural language, because for entropy of words order 0 is too low and order 1 with order 2 are too high
    sample3 - it is a natural language, because values and plots for entropy are similar to english and latin and differ only by about 10%
    sample4 - it is not a natural language, because slope is completely different for entropy of letters
    sample5 - it is not a natural language, because order 1, 2, 3 and 4 are equal to 0
    */
}