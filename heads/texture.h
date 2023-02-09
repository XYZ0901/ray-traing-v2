//
// Created by 周诣鑫 on 2023/1/12.
//

#ifndef G101_TEXTURE_H
#define G101_TEXTURE_H

#include <utility>

#include "rtweekend.h"
#include "rtw_stb_image.h"
#include "perlin.h"

#include <iostream>

class texture {
public:
    virtual color value(double u, double v, const point3 &p) const = 0;
};

class solid_color : public texture {
public:
    color color_value;
public:
    solid_color() = default;

    solid_color(color c) : color_value(c) {}

    solid_color(double r, double g, double b) : solid_color(color{r, g, b}) {}

    color value(double u, double v, const point3 &p) const override {
        return color_value;
    }
};

class checker_texture : public texture {
public:
    std::shared_ptr<texture> odd;
    std::shared_ptr<texture> even;
public:
    checker_texture() = default;

    checker_texture(std::shared_ptr<texture> _even, shared_ptr<texture> _odd)
            : even(std::move(_even)), odd(std::move(_odd)) {};

    checker_texture(color c1, color c2)
            : even(make_shared<solid_color>(c1)), odd(make_shared<solid_color>(c2)) {}

    color value(double u, double v, const point3 &p) const override {
        auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        if (sines < 0) {
            return odd->value(u, v, p);
        } else {
            return even->value(u, v, p);
        }
    }
};

class noise_texture : public texture {
public:
    perlin noise;
    double scale{};
public:
    noise_texture() = default;

    explicit noise_texture(double sc) : scale(sc) {}

    color value(double u, double v, const point3 &p) const override {
        return color(1, 1, 1) * 0.5 * (1 + sin(scale * p.z() + 10 * noise.turb(p)));
    }
};

class image_texture : public texture {
private:
    unsigned char *data;
    int width, height;
    int bytes_per_scanline;
public:
    const static int bytes_per_pixel = 3;

    image_texture() : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

    image_texture(const char *filename) {
        auto components_per_pixel = bytes_per_pixel;

        data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);
        if (!data) {
            std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
            width = height = 0;
        }
        bytes_per_scanline = bytes_per_pixel * width;
    }

    ~image_texture() {
        delete data;
    }

    color value(double u, double v, const point3 &p) const override {
        // If we have no texture data, then return solid cyan as a debugging aid.
        if (data == nullptr) {
            return {0, 1, 1};
        }

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = clamp(u, 0., 1.);
        v = 1.0 - clamp(v, 0, 1);

        auto i = static_cast<int>(u * width);
        auto j = static_cast<int>(v * height);

        // Clamp integer mapping, since actual coordinates should be less than 1.
        if (i >= width) i = width - 1;
        if (j >= height) j = height - 1;

        const auto color_scale = 1.0 / 255.;
        auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;
        return {color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]};
    }

};

#endif //G101_TEXTURE_H
