#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <fstream>
#include <vector>
#include <bitset>
#include <algorithm>
#include <exception>

#include "main_functions.hpp"

//function to see what minterms are covered vy term
bool minterms_covered(binary_term input_term, std::vector<binary_term> final_terms){

    //if a term covers a minterm that no other term covers, keep it, else
    int minterm;
    bool covered;

    for(int i=0;i<input_term.minterms_covered.size();i++){

        //set minterm to one of the minterms covered by the term
        minterm = input_term.minterms_covered[i];
        covered = false; //set to false

        //iterate over all the terms in final_terms list
        for(int j=0;j<final_terms.size();j++){

            //iterate through all the minterms covered by each term
            for(int k=0;k<final_terms[j].minterms_covered.size();k++){

                //if input term is being tested just continue
                if(input_term == final_terms[j]){
                    continue;
                }

                //if the minterm we are testing is already covered by another term, set to true
                //if it is not covered, then set to false. if covered is ever false, that term is necessary as no other term covers it
                if(minterm == final_terms[j].minterms_covered[k]){
                    covered = true;
                }
            }

            if(!covered){ //if minterm was not covered, return 
                return covered;
            }
        }

    }

    return covered;
    
    
}
// function to get minterms from a term
std::vector<int> getMintermsFromTerm(const std::string& term) {

    //placeholder
    std::vector<int> minterms;
    generateMinterms(term, 0, minterms);

    return minterms; //returns vector of minterms that the term covers
}

std::vector<binary_term> remove_null_values(std::vector<binary_term> final_terms){

    //placeholder variable
    int num_terms = final_terms.size();

    //check any duplicates
    for (int i = 0; i < num_terms; ++i) {
        for (int j = i + 1; j < num_terms; ) { // Start j from i + 1 to avoid redundant comparisons
            if (final_terms[i].value == final_terms[j].value) {
                final_terms.erase(final_terms.begin() + j);
                --num_terms; // Adjust the size
            } else {
                ++j; // Increment j only if no deletion occurs
            }
        }
    }

    return final_terms;
}

std::vector<binary_term> run_iterations(std::vector<binary_term> final_terms, std::vector<group> layer_one){

    //declare vector of groups for all possible layers(could make a vector of vectors, 
    //but considering layers is already a vector of groups, things will get convoluted)
    //max possible layers is 6 since 6 bit

    //if there was only one group of terms 
    int test = get_num_groups(layer_one);
    if(test == 1){
        throw std::logic_error("Term cannot be simplified anymore");
    }

    std::vector<group> layer_two(get_num_groups(layer_one)-1);
    layer_two = populate_layer(layer_one,final_terms); //populate next layer with groups
    if(get_num_groups(layer_two) == 0){ //if none of the terms could be combined return 
        return final_terms;
    }

    //if we make it to next layer, declare a new vector
    std::vector<group> layer_three(get_num_groups(layer_one)-2);
    layer_three = populate_layer(layer_two,final_terms);
    if(get_num_groups(layer_three) == 0){ //if none of the terms could be combined return 
        return final_terms;
    }

    std::vector<group> layer_four(get_num_groups(layer_one)-3);
    layer_four = populate_layer(layer_three,final_terms);
    if(get_num_groups(layer_four) == 0){ //if none of the terms could be combined return 
        return final_terms;
    }

    std::vector<group> layer_five(get_num_groups(layer_one)-4);
    layer_five = populate_layer(layer_four,final_terms);
    if(get_num_groups(layer_five) == 0){ //if none of the terms could be combined return 
        return final_terms;
    }

    std::vector<group> layer_six(get_num_groups(layer_one)-5);
    layer_six = populate_layer(layer_four,final_terms);
    if(get_num_groups(layer_six) == 0){ //if none of the terms could be combined return 
        return final_terms;
    }

    std::vector<group> layer_seven(get_num_groups(layer_one)-6);
    layer_seven = populate_layer(layer_six,final_terms);
    return final_terms;

}
// Function to convert decimal to binary and return it as a vector
std::string decToBinary(int n) {
    std::string binary;

    // edge case for 0
    if (n==0) {
        binary = "000000";
        return binary;
    }

    binary = "000000";
    int i = 0;
    std::string temp;
    // convert decimal to binary
    while (n>0) {

        temp = std::to_string(n % 2);
        binary.at(i) = *temp.c_str(); // get remainder 
        n = n / 2;
        i++;                 // divide by 2
    }

    // reverse the vector to get  MSB first
    std::reverse(binary.begin(), binary.end());

    return binary;
}


int get_num_groups(std::vector<group> groups){

    //declare how many groups are actually needed
    int num_groups = 0;
    for(int i=0;i<groups.size();i++){
        if(groups[i].values.empty() == false){
            num_groups++;
        }
    }

    return num_groups;

}


//////////////////////////////////SECOND LEVEL FUNCTIONS////////////////////////////////////////////////////////////
bool adjascent(std::string bin1, std::string bin2){

    int length = bin1.size();
    int num_dif = 0;

    for(int i=0;i<length;i++){
        if(bin1.at(i) != bin2.at(i)){
            num_dif++;
        }
    }

    if(num_dif>1){
        return false;
    }

    return true;

}

binary_term combine_term(std::string bin1, std::string bin2){

    int length = bin1.size();
    int num_dif = 0;

    binary_term combined;
    combined.value = bin1;

    for(int i=0;i<length;i++){
        if(bin1.at(i) != bin2.at(i)){
            combined.value.at(i) = 'x';
        }
    }

    return combined;

}

group compare_group(group &group1, group &group2, std::vector<binary_term> &final_terms){ //pass final terms by reference so it can be altered
    
    bool adj;

    //create new group to hold combined terms
    group group1and2;

    //placeholder variable
    binary_term combined;

    //iterate through group 1 and compare it to each value of group2, nested loop
    for(int i=0;i<group1.values.size();i++){

        for(int j=0;j<group2.values.size();j++){

            //test adjascency, mark as matched if true
            group1.values[i].matched = adjascent(group1.values[i].value, group2.values[j].value); //local matched
            group2.values[j].matched = group1.values[i].matched;

            //if true, create new combined term and add to new group
            if(group1.values[i].matched){

                //logic to remove any new matched value from final terms
                for(int k=0;k<final_terms.size();k++){
                    if(group1.values[i].value == final_terms[k].value){
                        final_terms.erase(final_terms.begin() + k);
                    }
                }
                group1.values[i].permenant_matched = true; //overrall matched so only terms that were never combined are added to final terms
                group2.values[j].permenant_matched = true;
                combined = combine_term(group1.values[i].value,group2.values[j].value);
                group1and2.values.push_back(combined);

            }
    
        }
    }

    //make a list of unmatched for both groups
    for(int i=0;i<group1.values.size();i++){
        if(group1.values[i].permenant_matched != true){
            final_terms.push_back(group1.values[i]);
        }
    }
    for(int i=0;i<group2.values.size();i++){
        if(group2.values[i].permenant_matched != true){
            final_terms.push_back(group2.values[i]);
        }
    }


    //return new combined group
    return group1and2;
}

std::vector<group> populate_layer(std::vector<group> layer,std::vector<binary_term> &final_terms){

    //placeholder 
    group temp,temp2,temp3;
    //output layer
    std::vector<group> next_layer;

    int test = get_num_groups(layer)-1;
    int j = 1; //placeholder

    //if we are at the final layer, add remainding terms to final terms
    if((get_num_groups(layer)-1) == 0){
        for(int i=0;i<layer[0].values.size();i++){
            final_terms.push_back(layer[0].values[i]);
        }
    }
    //compare each group and populate layer with combined terms group
    for(int i=0;i<(get_num_groups(layer)-1);i++){ //compare groups 0 and 1, then 1 and 2, so on

        //logic for if groups are not next to each other
        //for example if we have groups 1,4, and 5
        if(j!=1){
            i+=(j-1); 
        }
        j = 1;
        while(layer[i+j].values.empty()){
            j++;
        }

        //compare groups
        temp2 = layer[i];
        temp3 = layer[i+j];
        temp = compare_group(layer[i],layer[i+j],final_terms);

        if(temp.values.empty()){ //avoid pushing back empty vectors
            continue;
        }
        next_layer.push_back(temp); //populate next layer with the now combined group
    }

    return next_layer;
}


// Recursive helper function to generate all minterms
void generateMinterms(std::string term, int index, std::vector<int>& minterms) {
    if (index == term.size()) {
        // Convert binary string to decimal and add to the minterms list
        int decimalValue = std::stoi(term, nullptr, 2);
        minterms.push_back(decimalValue);
        return;
    }

    if (term[index] == 'x') {
        // Replace 'x' with '0' and recurse
        term[index] = '0';
        generateMinterms(term, index + 1, minterms);
        
        // Replace 'x' with '1' and recurse
        term[index] = '1';
        generateMinterms(term, index + 1, minterms);
    } else {
        // If not 'x', move to the next character
        generateMinterms(term, index + 1, minterms);
    }
}