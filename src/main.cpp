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

    std::string image_name = "";
    std::string output_name = "output/output.txt";

    int sizex = 0;
    int sizey = 0;

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
            else if (argv[i][1] == 's') {
                if (i + 2 < argc) {
                    try {
                        sizex = std::stoi (argv[i + 1]);
                        sizey = std::stoi (argv[i + 2]);
                    }
                    catch (std::invalid_argument &) {
                        std::cout << "faulty arguments after " << argv[i] << "\n" \
                        << "enter sizes as '-s 50 50'\n";
                        return 0;
                    }
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

    if (image_name == "") {
        std::cout << "missing image file to convert\n";
        return 0;
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

    if (sizex > width || sizey > height) {
        std::cout << "ascii art can not be larger than image\n";
        return 0;
    }

    sizex = sizex ? sizex : width;
    sizey = sizey ? sizey : height;

    int xpixel_per_ascii = static_cast<int>(round(width / sizex));
    int ypixel_per_ascii = static_cast<int>(round(height / sizey));

    for (int y = 0; y < sizey; y++) {
        for (int x = 0; x < sizex; x++) {

            int r = 0;
            int g = 0;
            int b = 0;

            for (int px = 0; px < xpixel_per_ascii; px++) {
                for (int py = 0; py < ypixel_per_ascii; py++) {

                    int index = 3*((y*ypixel_per_ascii + py)*width + (x*xpixel_per_ascii + px));

                    r += static_cast<int>(data[index + 0]);  // red
                    g += static_cast<int>(data[index + 1]);  // green
                    b += static_cast<int>(data[index + 2]);  // blue

                }
            }

            // 0.299*r + 0.587*g + 0.114*b
            int pixel_count = xpixel_per_ascii*ypixel_per_ascii;
            int a = static_cast<int>(round(((0.333*r + 0.333*g + 0.333*b)/(255*pixel_count))*ascii::count));  // grayscale value of the pixel

            if (a == ascii::count) {  // the char ' ' doesnt seem to render properely
                output << "  ";
            }
            else {
                output << ascii::characters[a] << " ";
            }
        }
        output << "\n";
    }

    stbi_image_free(data);
    output.close();
    return 0;
}
