﻿/*
 * MeteorDetector
 *
 * SPDX-License-Identifier: MIT
 */
#include "AsyncThread.h"
#include "ShutdownHelper.h"

namespace uzanka {
namespace meteordetector {

////////////////////////////////////////////////////////////////////////////////
AsyncThread::AsyncThread(VideoLoader::Ptr loader, Detector::Ptr detector, Exporter::Ptr exporter)
  : loader_(loader),
    detector_(detector),
    exporter_(exporter) {
}

////////////////////////////////////////////////////////////////////////////////
AsyncThread::~AsyncThread() {
  loader_ = nullptr;
  detector_ = nullptr;
  exporter_ = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
void AsyncThread::run() {
  // 分析框架.
  while (!ShutdownHelper::GetInstance().IsShutdown()) {
    // 获取本次要分析的帧.
    ImageHolder::Vector frame;
    if (!loader_->GetFrame(frame, detector_->RequiredFrameCount())) {
      break;
    }

    ResultData result;
    result.frame_no_   = frame[0]->frame_no_;
    result.frame_msec_ = frame[0]->frame_msec_;

    if (detector_->Run(frame, result)) {
      // 写入数据库(CSV).
      exporter_->Run(result);
    }
  }
}

}}  // uzanka::meteordetector
