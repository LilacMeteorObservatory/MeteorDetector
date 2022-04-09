/*
 * MeteorDetector
 *
 * SPDX-License-Identifier: MIT
 */
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>  // videoio
#include <opencv2/highgui.hpp>  // highgui
#include <opencv2/imgproc/imgproc.hpp>
#include "Poco/StringTokenizer.h"
#include "Poco/Dynamic/Var.h"
#include "normalizer/SizeNormalizer.h"
#include "Property.h"
#include "GlobalConfig.h"


namespace uzanka {
namespace meteordetector {

////////////////////////////////////////////////////////////////////////////////
SizeNormalizer::SizeNormalizer()
  : width_(0),
    height_(0),
    gray_(false),
    is_genrate_mask(false){
}

////////////////////////////////////////////////////////////////////////////////
SizeNormalizer::~SizeNormalizer() {
}

////////////////////////////////////////////////////////////////////////////////
//Normalizer::Ptr SizeNormalizer::Creator() {
//  return new SizeNormalizer;
//}

////////////////////////////////////////////////////////////////////////////////
std::string SizeNormalizer::Name() {
  return "SizeNormalizer";
}

////////////////////////////////////////////////////////////////////////////////
void SizeNormalizer::Run(ImageHolder::Ptr frame) {
  if ((frame->src_width_ != width_) || (frame->src_height_ != height_)) {
    cv::resize(frame->original_, frame->frame_, cv::Size(), ((double)width_ / frame->src_width_), ((double)height_ / frame->src_height_));
    frame->width_ = frame->frame_.cols;
    frame->height_ = frame->frame_.rows;
  } else {
    Normalizer::Run(frame);
  }

  // delete time counter area
  for (std::size_t i = 0; i < masking_.size(); i += 4) {
    cv::rectangle(frame->frame_, 
      cv::Point(masking_[i + 0], masking_[i + 1]),
      cv::Point(masking_[i + 2], masking_[i + 3]),
      cv::Scalar(0, 0, 0), -1);
  }


  if (is_genrate_mask) {
      // enable mask
      cv::Mat and_;
	  cv::bitwise_and(frame->frame_, frame->frame_, and_, custom_mask);
      frame->frame_ = and_;
#if _DEBUG
	  // std::cout << frame->frame_.size() << " " << custom_mask.size() << std::endl;
	  cv::imshow("Mask to Image", frame->frame_);
	  cv::waitKey(1);
#endif
      
  }

  if (gray_) {
	  cv::cvtColor(frame->frame_, frame->frame_, cv::COLOR_RGB2GRAY);
  }
}

////////////////////////////////////////////////////////////////////////////////
void SizeNormalizer::GetProperty(const std::string& name, const int width, const int height) {
  width_ = width;
  height_ = height;

  try {
    std::string value = Property::GetInstance().GetString(Poco::format("normalizer.%s.masking", name), "");
    Poco::StringTokenizer tokenizer(value, ",", Poco::StringTokenizer::TOK_TRIM);
    if (tokenizer.count() >= 4) {
      for (std::size_t i = 0; i < tokenizer.count(); i += 4) {
        masking_.push_back(Poco::Dynamic::Var(tokenizer[i + 0]).convert<int>());
        masking_.push_back(Poco::Dynamic::Var(tokenizer[i + 1]).convert<int>());
        masking_.push_back(Poco::Dynamic::Var(tokenizer[i + 2]).convert<int>());
        masking_.push_back(Poco::Dynamic::Var(tokenizer[i + 3]).convert<int>());
      }
    }

    gray_ = Property::GetInstance().GetInt(Poco::format("normalizer.%s.gray", name), Property::GetInstance().GetInt("normalizer.gray", false)) != 0;
  } catch (Poco::Exception& exc) {
    std::cout << Poco::format("Failed to get property. (exc=%s)", exc.displayText()) << std::endl;
    masking_.clear();
  }
}

void SizeNormalizer::CreateMask() {
    if (width_ == 0) {
        std::cout << "ERROR call SizeNormalizer::CreateMask after SizeNormalizer::GetProperty!" << std::endl;
        return;
    }
    if (mask_path.length() == 0) {
        return;
    }
    custom_mask = cv::imread(mask_path, cv::IMREAD_UNCHANGED);
    cv::Mat rgba[4];
    cv::split(custom_mask, rgba);
    custom_mask = rgba[3]; // read alpha channel.

    cv::resize(custom_mask, custom_mask, cv::Size(), ((double)width_ / custom_mask.size().width), ((double)height_ / custom_mask.size().height));
    // 二值化mask
    cv::threshold(custom_mask, custom_mask, 254, 255, cv::THRESH_BINARY);
    cv::bitwise_not(custom_mask, custom_mask);
    is_genrate_mask = true;
#if _DEBUG
    std::cout << custom_mask.size().width << " " << custom_mask.size().height << std::endl;
    cv::namedWindow("show mask", cv::WINDOW_NORMAL);
	cv::imshow("show mask", custom_mask);
	cv::waitKey(-1);
#endif
}

}}  // uzanka::meteordetector
