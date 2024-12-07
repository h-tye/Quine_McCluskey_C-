#ifndef _MAIN_FUNCTIONS_HPP
#define _MAIN_FUNCTIONS_HPP

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <fstream>
#include <vector>
#include <bitset>
#include <algorithm>

struct binary_term{
    std::string value;
    bool matched;
    bool permenant_matched = false;
    std::vector<int> minterms_covered;

    //overloaded operator to test equality between binary terms
    bool operator==(const binary_term &other){

        if((this->matched == other.matched) && (this->value == other.value) && (this->minterms_covered == other.minterms_covered)){
            return true;
        }

        //debugging cases
        else if(this->matched == other.matched){
            std::cout<< "Matched is equal but nothing else" << std::endl;
            return false;
        }
        else if(this->value == other.value){
            std::cout<< "Value is equal but nothing else" << std::endl;
            return false;
        }
        else if(this->minterms_covered == other.minterms_covered){
            std::cout<< "Minterms covered is equal but nothing else" << std::endl;
            return false;
        }

        return false;
    }
};

struct group{
    std::vector<binary_term> values;
};


//helper functions
bool minterms_covered(binary_term input_term, std::vector<binary_term> final_terms);
std::vector<int> getMintermsFromTerm(const std::string& term);
std::vector<binary_term> remove_null_values(std::vector<binary_term> final_terms);
std::vector<binary_term> run_iterations(std::vector<binary_term> final_terms, std::vector<group> layer_one);
std::string decToBinary(int n);
int get_num_groups(std::vector<group> groups);
bool adjascent(std::string bin1, std::string bin2);
binary_term combine_term(std::string bin1, std::string bin2);
group compare_group(group &group1, group &group2, std::vector<binary_term> &final_terms);
std::vector<group> populate_layer(std::vector<group> layer,std::vector<binary_term> &final_terms);
void generateMinterms(std::string term, int index, std::vector<int>& minterms);

#endif