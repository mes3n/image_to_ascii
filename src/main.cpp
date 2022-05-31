#include <iostream>
#include <fstream>
#include <string>

#include <math.h>

extern "C" {

    #define STB_IMAGE_IMPLEMENTATION
    #include "stb_image.h"

    #include "ascii.h"

}

int main (int argc, char **argv) {

    std::string image_name;
    std::string output_name = "output/output.txt";

    for (int i = 1; i < argc; i++) {

        if (argv[i][0] == '-') {
            if (argv[i][1] == 'h') {

                std::cout << \

            "Converts an image to ascii art.\n" \
            "Enter the path to the image you wish to convert after the '-f' flag.\n" \
            "Enter the path of the file where the ascii is ouput after the '-o' flag.\n";

                return 0;
            }
            else if (argv[i][1] == 'f') {
                if (i + 1 < argc) {
                    image_name = argv[i + 1];
                }
                else {
                    std::cout << "missing argument after " << argv[i] << "\n";
                    return 0;
                }
            }
            else if (argv[i][1] == 'o') {
                if (i + 1 < argc) {
                    output_name = argv[i + 1];
                }
                else {
                    std::cout << "missing argument after " << argv[i] << "\n";
                    return 0;
                }
            }
            else {
                std::cout << "unknown flag\n";
                return 0;
            }
        }
    }

    int width, height;
    unsigned char * data = stbi_load(image_name.c_str(), &width, &height, nullptr, 3);

    if (data == nullptr || width < 0 || height < 0) {
        std::cout << "Failed to load the image, make sure the path is valid.\n";
        return 0;
    }

    std::ofstream output (output_name, std::ios::trunc);
    if (!output.is_open()) {
        std::cout << "output file couldn't be opened\n";
        return 0;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            int index = 3*(y*width + x);
            int r = static_cast<int>(data[index + 0]);  // red
            int g = static_cast<int>(data[index + 1]);  // green
            int b = static_cast<int>(data[index + 2]);  // blue

            int a = static_cast<int>(round(((0.299*r + 0.587*g + 0.114*b)/255)*ascii::count));  // grayscale value of the pixel

            if (a == count) {  // the char ' ' doesnt seem to render properely
                std::cout << "  ";
                output << "  ";
            }
            else {
                std::cout << ascii::characters[a] << " ";
                output << ascii::characters[a] << " ";
            }

        }
        std::cout << "\n";
        output << "\n";
    }

    stbi_image_free(data);
    output.close();
    return 0;
}
