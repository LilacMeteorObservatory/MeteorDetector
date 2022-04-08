/*
 * MeteorDetector
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include "Define.h"
#include "normalizer/Normalizer.h"
#include "video/ImageHolder.h"

namespace uzanka {
namespace meteordetector {

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief サイズを変更しマスキングする正規化エンジンのクラス
 */
class SizeNormalizer : public Normalizer {
public:
  /*!
   * @brief 本クラスを共有するポインタ
   */
  typedef Poco::SharedPtr<SizeNormalizer> Ptr;

  /*!
   * @brief コンストラクタ
   */
  SizeNormalizer();

  /*!
   * @brief デストラクタ
   */
  virtual ~SizeNormalizer();

  /*
   * @brief 自クラスを生成する.
   *
   * 自クラスを生成して返す.
   */
  //static Normalizer::Ptr Creator();

  /*!
   * @brief 自クラスの名前を取得する.
   *
   * 自クラスの名前を返す.
   */
  static std::string Name();

  /*!
   * @brief フレームを正規化する.
   * @param[in]  frame  フレーム
   *
   * 何もしない.
   */
  virtual void Run(ImageHolder::Ptr frame);

protected:
  /*!
   * @brief プロパティ情報を読み込む.
   * @param[in] name   正規化エンジン名
   * @param[in] width  幅
   * @param[in] heigth 高さ
   *
   * normalizer.正規化エンジン名.masking を読み込む.
   * カンマで区切られた数字を4ポイント毎にrectangle情報と考えてメンバ変数に格納する.
   */
  void GetProperty(const std::string& name, const int width, const int heigth);
  void CreateMask();

protected:
  int              width_;    //!< 标准化宽度
  int              height_;   //!< 标准化高度
  bool             gray_;     //!< 制作黑白图像
  std::vector<int> masking_;  //!< mask 信息（每 4 个点代表一个矩形）
  cv::Mat custom_mask;        //!< 用户自定义png mask
  bool is_genrate_mask;       // 由于读入第一帧视频之后才知道视频的尺寸，因此要在运行时更改mask的尺寸。该变量标记是否生成了mask
};

}}  // uzanka::meteordetector
