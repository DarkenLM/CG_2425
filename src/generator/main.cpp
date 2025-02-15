#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include "include/generator/cone.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <figure>\n";
        return 1;
    }

    std::string figure = argv[1];

    if(figure == "cone")
    {
        if(argc < 7){
            std::cerr << "Usage: " << argv[0] << " " << argv[1] << " <radius> <height> <slices> <stacks> <output_file>";
            return 1;
        }
        int radius = std::atoi(argv[2]);
        int height = std::atoi(argv[3]);
        int slices = std::atoi(argv[4]);
        int stacks = std::atoi(argv[5]);

        coneGenerator(radius,height,slices,stacks);

        std::cout << "Generated cone!!\n";
        return 0;        
    } 
    else if (figure == "box")
    {
        if(argc < 5){
            std::cerr << "Usage: " << argv[0] << " " << argv[1] << " <length> <grid_size> <output_file>";
            return 1;
        }
        int length = std::atoi(argv[2]);
        int grid_size = std::atoi(argv[3]);

        std::cout << "Generated box!!\n";
        return 0;      
    } 
    else if (figure == "plane")
    {
        if(argc < 5){
            std::cerr << "Usage: " << argv[0] << " " << argv[1] << " <length> <grid_size> <output_file>";
            return 1;
        }
        int length = std::atoi(argv[2]);
        int grid_size = std::atoi(argv[3]);

        std::cout << "Generated plane!!\n";
        return 0;    
    } 
    else if (figure == "sphere")
    {
        if(argc < 6){
            std::cerr << "Usage: " << argv[0] << " " << argv[1] << " <radius> <slices> <stacks>";
            return 1;
        }
        int radius = std::atoi(argv[2]);
        int slices = std::atoi(argv[3]);
        int stacks = std::atoi(argv[4]);

        std::cout << "Generated sphere!!\n";
        return 0;    
    } 
    else
    {
        std::cerr << "Error: Primitive not supported.\n";
        return 1;
    }    
}