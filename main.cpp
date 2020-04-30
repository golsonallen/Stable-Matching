//
//  main.cpp
//  Stable Matching
//
//  Created by Griffin Olson-Allen on 4/27/20.
//  Copyright © 2020 Griffin Olson-Allen. All rights reserved.
//

#include <iostream>
#include <map>
#include <vector>

using namespace std;


class Stable_Matcher {
public:
    
    Stable_Matcher(int num_prefs_in, int num_matches_in)
        : num_prefs(num_prefs_in), num_matches(num_matches_in) {}
    
    
    string m_highest_pref (string &man) {
        // man proposes to his highest preference
        // search thru each value in his preferences to store his highest one
        
        map<string, int> man_pref = m_pref[man];
        string highest_pref_w = man_pref.begin()->first;
        int highest_pref_num = man_pref.begin()->second;
        
        for (const auto &key_value : man_pref) {
            if (key_value.second < highest_pref_num) {
                highest_pref_w = key_value.first;
                highest_pref_num = key_value.second;
            }
        }
        return highest_pref_w;
    }
    
    void single_women (string &m_proposing, string &w_proposed_to) {
        // if women is single, she will get engaged
        // remove woman and man from single  vectors
        auto it_w = find(single_w.begin(), single_w.end(), w_proposed_to);
        auto it_m = find(single_m.begin(), single_m.end(), m_proposing);
        
        single_w.erase(it_w);
        single_m.erase(it_m);
        // add pair to engagements
        engagements[w_proposed_to] = m_proposing;
        cout << m_proposing << " proposes to " << w_proposed_to << "." << endl;
        cout << w_proposed_to << " is single so she accepts." << endl;
        cout << m_proposing << " and " << w_proposed_to << " get engaged."
                << endl;
    }
    
    void taken_women (string &woman, string &m_proposing) {
        // if women is taken, find her preferences for her current m and
        // the m proposing
        map<string, int> woman_pref = w_pref[woman];
        string current_m = engagements[woman];
        int rank_current = woman_pref[current_m];
        int rank_proposing = woman_pref[m_proposing];
        cout << m_proposing << " proposes to " << woman << "." << endl;

        // based on preferences, she either prefers current m or new m
        // if she prefers m proposing more than m current
        if (rank_proposing < rank_current) {
            woman_prefers_new_m(woman, m_proposing);
        }
        // if she prefers current m more than m proposing:
        else {
            woman_prefers_current_m(woman, m_proposing);
        }
    }
    
    void woman_prefers_current_m (string &woman, string &m_proposing) {
        // proposing m stays single and engagement remains unchanged
        // m asks next preference, woman that turned him down removed from pref
        m_pref[m_proposing].erase(woman);
        cout << woman << " turns " << m_proposing << " down." << endl;
    }
    
    void woman_prefers_new_m (string &woman, string &m_proposing) {
        // if she prefers new m
        // replace old engagement with new one
        string current_m = engagements[woman];
        engagements[woman] = m_proposing;
        cout << woman << " prefers " << m_proposing << " over " << current_m
        << "." << endl;
        
        // new m removed from single vector
        auto it = find(single_m.begin(), single_m.end(), m_proposing);
        single_m.erase(it);
        cout << m_proposing << " and " << woman << " get engaged." << endl;
        // current m added
        single_m.push_back(current_m);
        cout << current_m << " becomes single again." << endl;
        m_pref[current_m].erase(woman);
    }
    
    void make_proposals () {
        // make each mans proposal
        string man = *single_m.begin();
        
        // man will propose to first women on his list
        string first_pref = m_highest_pref(man);
        auto it = find(single_w.begin(), single_w.end(), first_pref);
        // if it != to end iterator, w is single
        if (it != single_w.end()) {
            // if she is single, they get engaged
            single_women(man, first_pref);
        }
        // if not: either prefers current m or m proposing
        else {
            taken_women(first_pref, man);
        }
    }
    
    void print_proposals () {
        cout << "-----------------------------------" << endl;
        cout << "Final Engagements:" << endl << endl;
        for (const auto &key_value : engagements) {
            cout << key_value.first << " and " << key_value.second << endl;
        }
        cout << "-----------------------------------" << endl;
    }
    
    void store_pref () {
        cout << "Please enter participants" << endl;
        read_in_names("man");
        read_in_names("woman");
        cout << "Please enter preferences" << endl;
        read_in_M_ranks();
        read_in_W_ranks();
        init_single_M_and_W();
    }
    
    void make_pairings () {

        cout << "Proposals: " << endl;
        while (!single_m.empty()) {
            make_proposals();
        }
        print_proposals();
    }
    
    void stable_match () {
        store_pref();
        make_pairings();
    }
    
    void unequal_match () {
        store_pref();
        store_unlisted_prefs();
        make_pairings();
    }
    
    void read_in_names (string sex) {
        // for each match, read in the name
        // create a map for that person
        for (int i = 1; i <= num_matches; ++i) {
            string name = "";
            cout << "Name of " << sex << " #" << i << ": ";
            cin >> name;
            map<string, int> single_pref_map;
            // add the map to the pref map
            if (sex == "man") {
                m_pref[name] = single_pref_map;
            }
            else {
                w_pref[name] = single_pref_map;
            }
        }
        cout << endl;
    }
    
    void read_in_M_ranks () {
        // read in that persons preferences
        // add the name and rank of their preferences to the map
        for (const auto &key_value : m_pref) {
            string name = key_value.first;
            string pref = "";
            for (int i = 1; i <= num_prefs; ++i) {
                cout << "Enter " << name << "\'s #" << i << " preference: ";
                cin >> pref;
                // add to that person's pref map
                m_pref[name].insert({pref, i});
            }
            cout << endl;
        }
    }
    
    void read_in_W_ranks () {
        // read in that persons preferences
        // add the name and rank of their preferences to the map
        for (const auto &key_value : w_pref) {
            string name = key_value.first;
            string pref = "";
            for (int i = 1; i <= num_prefs; ++i) {
                cout << "Enter " << name << "\'s #" << i << " preference: ";
                cin >> pref;
                // add to that person's pref map
                w_pref[name].insert({pref, i});
            }
            cout << endl;
        }
        cout << endl;
    }

    void store_unlisted_M_prefs () {
        // for each man
        for (const auto &key_value : m_pref) {
            int count = num_prefs + 1;
            string man = key_value.first;
            // lop through the list of woman's names
            for (const auto &key_value2 : w_pref) {
                string woman_name = key_value2.first;
                // if the man didnt already have that woman in his prefs
                if (m_pref[man].find(woman_name) == m_pref[man].end()) {
                    // add the woman to their prefs
                    m_pref[man].insert({woman_name, count});
                    ++count;
                }
            }
        }
    }
    
    void store_unlisted_W_prefs() {
        // for each woman
        for (const auto &key_value : w_pref) {
            int count = num_prefs + 1;
            string woman = key_value.first;
            // loop through the list of men's names
            for (const auto &key_value2 : m_pref) {
                string man_name = key_value2.first;
                // if the woman didnt already have that man in her prefs
                if (w_pref[woman].find(man_name) == w_pref[woman].end()) {
                    // add the man to their prefs
                    w_pref[woman].insert({man_name, count});
                    ++count;
                }
            }
        }
    }
    
    // if there are 20 pairs and only 5 preferences given, fills the other
    // 15 preferences in alphabetical order.
    void store_unlisted_prefs() {
        store_unlisted_M_prefs();
        store_unlisted_W_prefs();
    }
    
    void init_single_M_and_W () {
        // loop through the pref maps to get the name of each man and woman
        // push back into single vector
        for (const auto &key_value : m_pref) {
            single_m.push_back(key_value.first);
        }
        for (const auto &key_value : w_pref) {
            single_w.push_back(key_value.first);
        }
    }
    
    void tests_run () {
        cout << "TEST DATA" << endl << endl;
        
        map<string, int> Griffin;
        Griffin["Caroline"] = 1;
        Griffin["Katie"] = 2;
        Griffin["Ronnie"] = 3;
        Griffin["Siobhan"] = 4;
        m_pref["Griffin"] = Griffin;
        
        map<string, int> Joey;
        Joey["Katie"] = 1;
        Joey["Caroline"] = 2;
        Joey["Ronnie"] = 3;
        Joey["Siobhan"] = 4;
        m_pref["Joey"] = Joey;
        
        map<string, int> Scott;
        Scott["Katie"] = 1;
        Scott["Caroline"] = 2;
        Scott["Siobhan"] = 3;
        Scott["Ronnie"] = 4;
        m_pref["Scott"] = Scott;
        
        map<string, int> Dean;
        Dean["Caroline"] = 1;
        Dean["Katie"] = 2;
        Dean["Siobhan"] = 3;
        Dean["Ronnie"] = 4;
        m_pref["Dean"] = Dean;
                
        map<string, int> Caroline;
        Caroline["Joey"] = 1;
        Caroline["Griffin"] = 2;
        Caroline["Dean"] = 3;
        Caroline["Scott"] = 4;
        w_pref["Caroline"] = Caroline;
        
        map<string, int> Katie;
        Katie["Griffin"] = 1;
        Katie["Scott"] = 2;
        Katie["Joey"] = 3;
        Katie["Dean"] = 4;
        w_pref["Katie"] = Katie;
        
        map<string, int> Ronnie;
        Ronnie["Scott"] = 1;
        Ronnie["Griffin"] = 2;
        Ronnie["Dean"] = 3;
        Ronnie["Joey"] = 4;
        w_pref["Ronnie"] = Ronnie;
        
        map<string, int> Siobhan;
        Siobhan["Scott"] = 1;
        Siobhan["Dean"] = 2;
        Siobhan["Griffin"] = 3;
        Siobhan["Joey"] = 4;
        w_pref["Siobhan"] = Siobhan;
        
        init_single_M_and_W();
        make_pairings();
    }
    
private:
    
    int num_prefs;
    int num_matches;
    // pref maps contain the name of the m or w and then a map of their prefs
    // string is name of m/w and the int is the pref number
    map<string, map<string, int> > m_pref;
    map<string, map<string, int> > w_pref;
    
    vector<string> single_m;
    vector<string> single_w;
    // woman, man
    map<string, string> engagements;
    
};


void print_error_message() {
    cout << "Usage: main.exe NUM_MATCHES NUM_PREFS [test]" << endl;
    cout << endl;
    cout << "NUM_MATCHES and NUM_PREFS must be at least 1" << endl;
    cout << "If testing, NUM_MATCHES and NUM_PREFS must both be 4" << endl;
}

// command line arguments:
// executable, number of preferences, number of matches, test
// if test is provided, use testing data
int main(int argc, const char * argv[]) {

    // incorrect number of arguments or invalid number of matches/prefs
    if ((argc != 3 && argc != 4) || atoi(argv[1]) < 1 || atoi(argv[2]) < 1) {
        print_error_message();
        return 1;
    }
    // incorrect test argument provided
    if (argc == 4 && (string(argv[3]) != "test")) {
        print_error_message();
        return 1;
    }
    // testing with an incorrect number of matches/prefs
    if (argc == 4 && (string(argv[1]) != "4" || string(argv[2]) != "4")) {
        print_error_message();
        return 1;
    }
    
    int prefs = atoi(argv[1]);
    int matches = atoi(argv[2]);
    Stable_Matcher match(prefs, matches);
    
    // if test provided, use tests data
    if (argc == 4) {
        match.tests_run();
    }
    // otherwise use user input
    else {
        // normal stable matching algorithm
        if (prefs == matches) {
            match.stable_match();
        }
        // unequal number of men and women, add exta prefs
        else {
            match.unequal_match();
        }
    }
    return 0;
}
