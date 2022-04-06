/*
 * MeteorDetector
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include "Poco/SharedPtr.h"
#include "Define.h"

namespace uzanka {
namespace meteordetector {

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief 解析結果を保持するクラス
 *
 * 解析結果を保持する.
 */
class ResultData {
public:
  /*!
   * @brief 共享此类的指针
   */
  typedef Poco::SharedPtr<ResultData> Ptr;

  /*!
   * @brief 分析结果一览
   */
  typedef std::list<ResultData::Ptr> List;

  /*!
   * @brief 构造函数
   */
  ResultData();

  /*!
   * @brief デストラクタ
   */
  virtual ~ResultData();

public:
  Poco::Int64 frame_no_;    //!< 帧数
  double      frame_msec_;  //!< 帧毫秒
  int         mark_;        //!< 标记（0 不适用）
  double      result_[10];  //!< 检测信息（实数）
  std::string message_;     //!< 检测信息（字符串）
};

////////////////////////////////////////////////////////////////////////////////
inline ResultData::ResultData()
  : frame_no_(0),
    frame_msec_(0),
    mark_(0),
    message_("") {
  for (int i = 0; i < 10; i++) {
    result_[i] = 0;
  }
}

////////////////////////////////////////////////////////////////////////////////
inline ResultData::~ResultData() {
}

}}  // uzanka::meteordetector
