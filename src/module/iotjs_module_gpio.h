/* Copyright 2015 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef IOTJS_MODULE_GPIO_H
#define IOTJS_MODULE_GPIO_H


#include "iotjs_def.h"
#include "iotjs_objectwrap.h"
#include "iotjs_reqwrap.h"


namespace iotjs {

enum GpioDirection {
  kGpioDirectionNone = 0,
  kGpioDirectionIn,
  kGpioDirectionOut,
};


enum GpioMode {
  kGpioModeNone = 0,
  kGpioModePullup,
  kGpioModePulldown,
  kGpioModeFloat,
  kGpioModePushpull,
  kGpioModeOpendrain,
};


enum GpioError {
  kGpioErrOk = 0,
  kGpioErrInitialize = -1,
  kGpioErrNotInitialized = -2,
  kGpioErrWrongUse = -98,
  kGpioErrSys = -99,
};


enum GpioOp {
  kGpioOpInitize,
  kGpioOpRelease,
  kGpioOpOpen,
  kGpioOpWrite,
  kGpioOpRead,
};


class GpioReqWrap {
 public:
  GpioReqWrap(const iotjs_jval_t* jcallback) {
    iotjs_reqwrap_initialize(&_reqwrap, jcallback, (uv_req_t*)&_req, this);
  }

  ~GpioReqWrap() {
    iotjs_reqwrap_destroy(&_reqwrap);
  }

  uint32_t pin;
  uint32_t value;
  GpioDirection dir;
  GpioMode mode; // only for set pin
  GpioError result;
  GpioOp op;

  uv_work_t* req() {
    return &_req;
  }

  const iotjs_jval_t* jcallback() {
    return iotjs_reqwrap_jcallback(&_reqwrap);
  }

 protected:
  iotjs_reqwrap_t _reqwrap;
  uv_work_t _req;
};


// This Gpio class provides interfaces for GPIO operation.
class Gpio {
 public:
  explicit Gpio(const iotjs_jval_t* jgpio);
  virtual ~Gpio();

  static Gpio* Create(const iotjs_jval_t* jgpio);
  static Gpio* GetInstance();
  static const iotjs_jval_t* GetJGpio();
  static void SetGpio();
  static void WriteGpio();
  static void ReadGpio();

  virtual int Initialize(GpioReqWrap* gpio_req) = 0;
  virtual int Release(GpioReqWrap* gpio_req) = 0;
  virtual int Open(GpioReqWrap* gpio_req) = 0;
  virtual int Write(GpioReqWrap* gpio_req) = 0;
  virtual int Read(GpioReqWrap* gpio_req) = 0;

  static void Delete(const uintptr_t data) {
    delete ((Gpio*)data);
  }

 protected:
  iotjs_jobjectwrap_t _jobjectwrap;
};


} // namespace iotjs


#endif /* IOTJS_MODULE_GPIO_H */
