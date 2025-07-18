// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2019-2025, The OpenROAD Authors

#pragma once

#include <algorithm>
#include <memory>

#include "db/obj/frRef.h"
#include "db/obj/frShape.h"
#include "db/tech/frViaDef.h"

namespace drt {
class frNet;
class drVia;
class frVia : public frRef
{
 public:
  // constructors
  frVia() = default;
  frVia(const frViaDef* in) : viaDef_(in) {}
  frVia(const frViaDef* def_in, const Point& pt_in)
      : origin_(pt_in), viaDef_(def_in)
  {
  }
  frVia(const frVia& in)
      : frRef(in),
        origin_(in.origin_),
        viaDef_(in.viaDef_),
        owner_(in.owner_),
        tapered_(in.tapered_),
        bottomConnected_(in.bottomConnected_),
        topConnected_(in.topConnected_),
        isLonely_(in.isLonely_)
  {
  }
  frVia(const drVia& in);
  // getters
  const frViaDef* getViaDef() const { return viaDef_; }
  Rect getLayer1BBox() const
  {
    Rect box;
    box.mergeInit();
    for (auto& fig : viaDef_->getLayer1Figs()) {
      box.merge(fig->getBBox());
    }
    getTransform().apply(box);
    return box;
  }
  Rect getCutBBox() const
  {
    Rect box;
    box.mergeInit();
    for (auto& fig : viaDef_->getCutFigs()) {
      box.merge(fig->getBBox());
    }
    getTransform().apply(box);
    return box;
  }
  Rect getLayer2BBox() const
  {
    Rect box;
    box.mergeInit();
    for (auto& fig : viaDef_->getLayer2Figs()) {
      box.merge(fig->getBBox());
    }
    getTransform().apply(box);
    return box;
  }
  // setters
  void setViaDef(const frViaDef* in) { viaDef_ = in; }
  // others
  frBlockObjectEnum typeId() const override { return frcVia; }

  /* from frRef
   * getOrient
   * setOrient
   * getOrigin
   * setOrigin
   * getTransform
   * setTransform
   */

  dbOrientType getOrient() const override { return dbOrientType(); }
  void setOrient(const dbOrientType& tmpOrient) override { ; }
  Point getOrigin() const override { return origin_; }
  void setOrigin(const Point& tmpPoint) override { origin_ = tmpPoint; }
  dbTransform getTransform() const override { return dbTransform(origin_); }
  void setTransform(const dbTransform& xformIn) override {}

  /* from frPinFig
   * hasPin
   * getPin
   * addToPin
   * removeFromPin
   */
  bool hasPin() const override
  {
    return (owner_)
           && ((owner_->typeId() == frcBPin) || owner_->typeId() == frcMPin);
  }
  frPin* getPin() const override { return reinterpret_cast<frPin*>(owner_); }
  void addToPin(frPin* in) override
  {
    owner_ = reinterpret_cast<frBlockObject*>(in);
  }
  void addToPin(frBPin* in) override
  {
    owner_ = reinterpret_cast<frBlockObject*>(in);
  }
  void removeFromPin() override { owner_ = nullptr; }

  /* from frConnFig
   * hasNet
   * getNet
   * addToNet
   * removeFromNet
   */
  bool hasNet() const override
  {
    return (owner_) && (owner_->typeId() == frcNet);
  }
  frNet* getNet() const override { return reinterpret_cast<frNet*>(owner_); }
  void addToNet(frNet* in) override
  {
    owner_ = reinterpret_cast<frBlockObject*>(in);
  }
  void removeFromNet() override { owner_ = nullptr; }

  /* from frFig
   * getBBox
   * move
   * intersects
   */

  Rect getBBox() const override
  {
    auto& layer1Figs = viaDef_->getLayer1Figs();
    auto& layer2Figs = viaDef_->getLayer2Figs();
    auto& cutFigs = viaDef_->getCutFigs();
    bool isFirst = true;
    frCoord xl = 0;
    frCoord yl = 0;
    frCoord xh = 0;
    frCoord yh = 0;
    for (auto& fig : layer1Figs) {
      Rect box = fig->getBBox();
      if (isFirst) {
        xl = box.xMin();
        yl = box.yMin();
        xh = box.xMax();
        yh = box.yMax();
        isFirst = false;
      } else {
        xl = std::min(xl, box.xMin());
        yl = std::min(yl, box.yMin());
        xh = std::max(xh, box.xMax());
        yh = std::max(yh, box.yMax());
      }
    }
    for (auto& fig : layer2Figs) {
      Rect box = fig->getBBox();
      if (isFirst) {
        xl = box.xMin();
        yl = box.yMin();
        xh = box.xMax();
        yh = box.yMax();
        isFirst = false;
      } else {
        xl = std::min(xl, box.xMin());
        yl = std::min(yl, box.yMin());
        xh = std::max(xh, box.xMax());
        yh = std::max(yh, box.yMax());
      }
    }
    for (auto& fig : cutFigs) {
      Rect box = fig->getBBox();
      if (isFirst) {
        xl = box.xMin();
        yl = box.yMin();
        xh = box.xMax();
        yh = box.yMax();
        isFirst = false;
      } else {
        xl = std::min(xl, box.xMin());
        yl = std::min(yl, box.yMin());
        xh = std::max(xh, box.xMax());
        yh = std::max(yh, box.yMax());
      }
    }
    Rect box(xl, yl, xh, yh);
    getTransform().apply(box);
    return box;
  }
  void move(const dbTransform& xform) override { ; }
  bool intersects(const Rect& box) const override { return false; }

  void setIter(frListIter<std::unique_ptr<frVia>>& in) { iter_ = in; }
  frListIter<std::unique_ptr<frVia>> getIter() const { return iter_; }
  void setTapered(bool t) { tapered_ = t; }

  bool isTapered() const { return tapered_; }

  bool isBottomConnected() const { return bottomConnected_; }
  bool isTopConnected() const { return topConnected_; }
  void setBottomConnected(bool c) { bottomConnected_ = c; }
  void setTopConnected(bool c) { topConnected_ = c; }
  void setIndexInOwner(int idx) { index_in_owner_ = idx; }
  int getIndexInOwner() const { return index_in_owner_; }
  void setIsLonely(bool in) { isLonely_ = in; }
  bool isLonely() const { return isLonely_; }

 private:
  Point origin_;
  const frViaDef* viaDef_{nullptr};
  frBlockObject* owner_{nullptr};
  frListIter<std::unique_ptr<frVia>> iter_;
  int index_in_owner_{0};
  bool tapered_{false};
  bool bottomConnected_{false};
  bool topConnected_{false};
  bool isLonely_{false};

  template <class Archive>
  void serialize(Archive& ar, unsigned int version);

  friend class boost::serialization::access;
};
}  // namespace drt
