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
    
    int get_num_prefs () {
        return num_prefs;
    }
    
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
    
    void read_in_names (string sex, map<string,map<string,int>> &pref_map, int matches) {
        // for each match, read in the name
        // create a map for that person
        for (int i = 1; i <= matches; ++i) {
            string name = "";
            cout << "Name of " << sex << " #" << i << ": ";
            cin >> name;
            map<string, int> single_pref_map;
            // add the map to the pref map
            pref_map[name] = single_pref_map;
        }
        cout << endl;
    }
    
    void read_in_ranks (map<string, map<string,int>> &pref_map) {
        // read in that persons preferences
        // add the name and rank of their preferences to the map
        for (const auto &key_value : pref_map) {
            string name = key_value.first;
            string pref = "";
            for (int i = 1; i <= num_prefs; ++i) {
                cout << "Enter " << name << "\'s #" << i << " preference: ";
                cin >> pref;
                // add to that person's pref map
                pref_map[name].insert({pref, i});
            }
            if (num_prefs > 2) cout << endl;
        }
        cout << endl;
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
        read_in_names("man", m_pref, num_matches);
        read_in_names("woman", w_pref, num_matches);
        cout << "Please enter preferences" << endl;
        read_in_ranks(m_pref);
        read_in_ranks(w_pref);
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
    
protected:
    
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

class Incomplete_Pref_Matcher : public Stable_Matcher {
public:
    
    Incomplete_Pref_Matcher (int num_prefs_in, int num_matches_in)
        : Stable_Matcher(num_prefs_in, num_matches_in) {}
    
    
    // if there are 20 pairs and only 5 preferences given, fills the other
    // 15 preferences in alphabetical order.
    void store_unlisted_prefs (map<string, map<string,int>> &pref_map1,
                               map<string, map<string, int>> &pref_map2) {
        // for each man or woman
        for (const auto &key_value : pref_map1) {
            int count = get_num_prefs() + 1;
            string person = key_value.first;
            // lop through the list of opposite sex names
            for (const auto &key_value2 : pref_map2) {
                string match_name = key_value2.first;
                // if the person didnt already have that name in his prefs
                if (pref_map1[person].find(match_name) ==
                                        pref_map1[person].end()) {
                    // add the woman to their prefs
                    pref_map1[person].insert({match_name, count});
                    ++count;
                }
            }
        }
    }

    void incomplete_match () {
        store_pref();
        store_unlisted_prefs(m_pref, w_pref);
        store_unlisted_prefs(w_pref, m_pref);
        make_pairings();
    }
    
};

// NOTE: The big little program relies on big and little preferences where
// every little will get paired with someone on in their top 5.
// Unlisted preferences are NOT stored to preserve speed and space.

// NOTE: Every little will get 1 big but if there are more bigs than littles
// every big might not get a little. The program also does not allow for
// littles to share the same big

class Big_Little_Matcher : public Stable_Matcher {
public:
    
    // Bigs function as women, little function as men
    // default initialized to 0, later set to user input
    Big_Little_Matcher (int num_prefs_in, int num_matches_in)
        : Stable_Matcher(num_prefs_in, num_matches_in), num_bigs(0),
            num_littles(0) {}
    
    void set_bigs (int x) { num_bigs = x; }
    
    void set_littles (int x) { num_littles = x; }
    
    void prompt_num_bigs_and_littles () {
        int bigs_in;
        cout << "Please enter the number of bigs: ";
        cin >> bigs_in;
        set_bigs(bigs_in);
        
        int littles_in;
        cout << "Please enter the number of littles: ";
        cin >> littles_in;
        cout << endl;
        set_littles(littles_in);
    }
    
    void store_unequal_pref () {
        // read in names for every big and for every little
        cout << "Please enter participants" << endl;
        read_in_names("little", m_pref, num_littles);
        read_in_names("big", w_pref, num_bigs);
        cout << "Please enter preferences" << endl;
        read_in_ranks(m_pref);
        read_in_ranks(w_pref);
        init_single_M_and_W();
    }
    
    void big_little_match () {
        prompt_num_bigs_and_littles();
        store_unequal_pref();
        make_pairings();
    }
    
private:
    // variables to keep track of number of bigs and littles when
    // there are more bigs than littles
    int num_bigs; // W
    int num_littles; // M
};


int menu_prompt () {
    cout << "Welcome to the Stable Matching Program!" << endl;
    cout << endl;
    cout << "To test the program, please enter 1" << endl;
    cout << "To run the program, please enter 2" << endl;
    cout << "Choice: ";
    
    int choice = 0;
    cin >> choice;
    cout << endl;
    return choice;
}

int program_prompt() {
    cout << "For classic stable matching with 2 equal size groups and " << endl;
    cout << "each person ranks their preference for every member of" << endl;
    cout << "the other group," << " please enter 1" << endl << endl;
    
    cout << "For incomplete preference matching with 2 equal size " << endl;
    cout << "groups but each person only provides a few preferences, " << endl;
    cout << "please enter 2" << endl << endl;
    
    cout << "For big little matching with groups of unequal size and " << endl;
    cout << "only a few preferences, please enter 3" << endl << endl;
    
    cout << "Choice: ";
    int choice = 0;
    cin >> choice;
    cout << endl;
    return choice;
}

int main(int argc, const char * argv[]) {
    
    int prefs = 0;
    int matches = 0;
    
    int menu_choice = menu_prompt();
    // test program
    if (menu_choice == 1) {
        Stable_Matcher test(4, 4);
        test.tests_run();
    }
    // run program
    else {
        int p_choice = program_prompt();
        if (p_choice == 1) {
            cout << "Enter number of matches and preferences: ";
            cin >> matches;
            cout << endl;
            Stable_Matcher match(matches, matches);
            match.stable_match();
        }
        else if (p_choice == 2) {
            cout << "Enter number of preferences: ";
            cin >> prefs;
            cout << "Enter number of matches: ";
            cin >> matches;
            cout << endl;
            Incomplete_Pref_Matcher match(prefs, matches);
            match.incomplete_match();
        }
        else if (p_choice == 3) {
            cout << "Enter number of preferences: ";
            cin >> prefs;
            cout << "Enter number of matches: ";
            cin >> matches;
            cout << endl;
            Big_Little_Matcher match(prefs, matches);
            match.big_little_match();
        }
    }
    return 0;
}
