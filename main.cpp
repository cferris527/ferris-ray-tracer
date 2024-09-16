#include <iostream>
#include <fstream>

/*
    To run:
    g++ main.cpp -o example
    example > image.ppm (This writes to image.ppm vs directly to console)
*/
int main() {

    // Image

    int image_width = 256;
    int image_height = 256;

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            auto r = 0;
            auto g = double(i) / (image_width-1);
            auto b = double(j) / (image_height-1);

            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
            
        }
    }
}

