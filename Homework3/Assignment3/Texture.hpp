//
// Created by LEI XU on 4/27/19.
//

#ifndef RASTERIZER_TEXTURE_H
#define RASTERIZER_TEXTURE_H
#include "global.hpp"
#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>
class Texture{
private:
    cv::Mat image_data;

public:
    Texture(const std::string& name)
    {
        image_data = cv::imread(name);
        cv::cvtColor(image_data, image_data, cv::COLOR_RGB2BGR);
        width = image_data.cols;
        height = image_data.rows;
    }

    int width, height;

    Eigen::Vector3f getColor(float u, float v)
    {
        if (u < 0) u = 0;
        if (v < 0) v = 0;
        if (u > 1) u = 1;
        if (v > 1) v = 1;

        auto u_img = u * width;
        auto v_img = (1 - v) * height;
        auto color = image_data.at<cv::Vec3b>(v_img, u_img);
        return Eigen::Vector3f(color[0], color[1], color[2]);
    }
    Eigen::Vector3f getColorBilinear(float u, float v)
    {
        if (u < 0) u = 0;
        if (v < 0) v = 0;
        if (u > 1) u = 1;
        if (v > 1) v = 1;
        auto u_img = u * width;
        auto v_img = (1 - v) * height;
        int x = floor(u_img);
        int y = floor(v_img);
        float u_ratio = u_img - x;
        float v_ratio = v_img - y;
        float u_opposite = 1 - u_ratio;
        float v_opposite = 1 - v_ratio;
        auto color00 = image_data.at<cv::Vec3b>(y, x);
		Eigen::Vector3f color00_vec(color00[0], color00[1], color00[2]);
        auto color01 = image_data.at<cv::Vec3b>(y, std::min(x + 1, width - 1));
		Eigen::Vector3f color01_vec(color01[0], color01[1], color01[2]);
        auto color10 = image_data.at<cv::Vec3b>(std::min(y + 1, height - 1), x);
		Eigen::Vector3f color10_vec(color10[0], color10[1], color10[2]);
        auto color11 = image_data.at<cv::Vec3b>(std::min(y + 1, height - 1), std::min(x + 1, width - 1));
		Eigen::Vector3f color11_vec(color11[0], color11[1], color11[2]);
        
        Eigen::Vector3f result_color = color00_vec * u_opposite * v_opposite +
            color10_vec * u_ratio * v_opposite +
            color01_vec * u_opposite * v_ratio +
            color11_vec * u_ratio * v_ratio;
		return result_color;
    }

};
#endif //RASTERIZER_TEXTURE_H
