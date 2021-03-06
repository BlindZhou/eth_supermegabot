/**
* Software License Agreement (BSD License)
*
* Copyright (c) 2015, C. Dario Bellicoso, Christian Gehring, Ralf Kaestner
* All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of Autonomous Systems Lab nor ETH Zurich
*     nor the names of its contributors may be used to endorse or
*     promote products derived from this software without specific
*     prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*/
/*
 * ParameterInterface.hpp
 *
 *  Created on: Sep 23, 2015
 *      Author: Christian Gehring, Ralf Kaestner
 */

#pragma once

#include <parameter_handler/ParameterValue.hpp>

#include <string>
#include <memory>
#include <typeindex>

namespace parameter_handler {

class ParameterInterface {
 protected:
  ParameterInterface(const std::type_index& type, const internal::ParameterValuePtr& value, const std::string& name = std::string()) :
    type_(type),
    value_(value),
    name_(name)
  {
  }
 public:
  ParameterInterface() :
    type_(typeid(void))
  {
  }

  ParameterInterface(const ParameterInterface& other) :
    type_(other.type_),
    value_(other.value_),
    name_(other.name_)
  {
  }
  virtual ~ParameterInterface() {}

 public:
  const std::string& getName() const {
    return name_;
  }
  void setName(const std::string& name) {
    name_ = name;
  }

  const std::type_index& getType() const {
    return type_;
  }

  template<typename ValueType_>
  const ValueType_& getValue() const
  {
    if (type_ == typeid(ValueType_)) {
      return std::static_pointer_cast<internal::ParameterValue<ValueType_> >(value_)->getValue();
    }
    else {
      throw std::runtime_error("Parameter value type mismatch");
    }
  };

  template<typename ValueType_>
  const ValueType_& getMinValue() const
  {
    if (type_ == typeid(ValueType_)) {
      return std::static_pointer_cast<internal::ParameterValue<ValueType_> >(value_)->getMinValue();
    }
    else {
      throw std::runtime_error("Parameter value type mismatch");
    }
  }
  template<typename ValueType_>
  const ValueType_& getMaxValue() const
  {
    if (type_ == typeid(ValueType_)) {
      return std::static_pointer_cast<internal::ParameterValue<ValueType_> >(value_)->getMaxValue();
    }
    else {
      throw std::runtime_error("Parameter value type mismatch");
    }
  }
  template<typename ValueType_>
  const ValueType_& getDefaultValue() const
  {
    if (type_ == typeid(ValueType_)) {
      return std::static_pointer_cast<internal::ParameterValue<ValueType_> >(value_)->getDefaultValue();
    }
    else {
      throw std::runtime_error("Parameter value type mismatch");
    }
  }

  template<typename ValueType_>
  void setValue(const ValueType_& value)
  {
    if (type_ == typeid(ValueType_)) {
      std::static_pointer_cast<internal::ParameterValue<ValueType_> >(value_)->setValue(value);
      notifyObservers();
    }
    else {
      throw std::runtime_error("Parameter value type mismatch");
    }
  }

  template<typename ValueType_>
  void setMinValue(const ValueType_& value)
  {
    if (type_ == typeid(ValueType_)) {
      std::static_pointer_cast<internal::ParameterValue<ValueType_> >(value_)->setMinValue(value);
      notifyObservers();
    }
    else {
      throw std::runtime_error("Parameter value type mismatch");
    }
  }

  template<typename ValueType_>
  void setMaxValue(const ValueType_& value)
  {
    if (type_ == typeid(ValueType_)) {
      std::static_pointer_cast<internal::ParameterValue<ValueType_> >(value_)->setMaxValue(value);
      notifyObservers();
    }
    else {
      throw std::runtime_error("Parameter value type mismatch");
    }
  }

  template<typename ValueType_>
  void setDefaultValue(const ValueType_& value)
  {
    if (type_ == typeid(ValueType_)) {
      std::static_pointer_cast<internal::ParameterValue<ValueType_> >(value_)->setDefaultValue(value);
      notifyObservers();
    }
    else {
      throw std::runtime_error("Parameter value type mismatch");
    }
  }

  ParameterInterface clone() const {
    return ParameterInterface(type_, value_->clone(), name_);
  }

  ParameterInterface& operator=(const ParameterInterface& other) {
    type_ = other.type_;
    value_ = other.value_;
    name_ = other.name_;
    return *this;
  }

  void notifyObservers() {
    for(auto observer: value_->getObservers()) { observer->parameterChanged(*this); };
  }

  void addObserver(ParameterObserverInterface * observer) {
    value_->addObserver(observer);
  }

  void removeObserver(ParameterObserverInterface * observer) {
    value_->removeObserver(observer);
  }

 protected:
  std::type_index type_;
  internal::ParameterValuePtr value_;
  std::string name_;
};


} // namespace
