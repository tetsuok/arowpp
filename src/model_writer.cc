// Copyright (c) 2012, Tetsuo Kiso
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  * Redistributions of source code must retain the above
//    copyright notice, this list of conditions and the
//    following disclaimer.
//
//  * Redistributions in binary form must reproduce the above
//    copyright notice, this list of conditions and the
//    following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
//  * Neither the name of Tetsuo Kiso nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "model_writer_interface.h"

#include <fstream>
#include "logging.h"
#include "param.h"

namespace arowpp {
namespace {

ModelWriterInterface* g_model_writer = NULL;

} // namespace

class ModelWriter : public ModelWriterInterface {
 public:
  ModelWriter() {}
  ~ModelWriter() {}

  virtual bool Open(const char* filename, const Param* param) const;

 private:
  ModelWriter(const ModelWriter&);
  const ModelWriter& operator=(const ModelWriter&);
};

bool ModelWriter::Open(const char* filename, const Param* param) const {
  std::ofstream bofs;
  bofs.open(filename, std::ios::out | std::ios::binary);
  if (!bofs) {
    LOG(ERROR) << "no such file or directory: " << filename;
    return false;
  }

  bofs.write(reinterpret_cast<const char *>(&param->num_feature), sizeof(param->num_feature));
  bofs.write(reinterpret_cast<const char *>(&param->num_example), sizeof(param->num_example));
  bofs.write(reinterpret_cast<const char *>(&param->num_update), sizeof(param->num_update));
  bofs.write(reinterpret_cast<const char *>(&param->num_iter), sizeof(param->num_iter));
  bofs.write(reinterpret_cast<const char *>(&param->r), sizeof(param->r));
  bofs.write(reinterpret_cast<const char *>(&param->is_shuffled), sizeof(param->is_shuffled));

  // mean
  bofs.write(reinterpret_cast<const char *>(&param->mean[0]), param->mean.size() * sizeof(float));

  // covariance
  bofs.write(reinterpret_cast<const char *>(&param->cov[0]), param->cov.size() * sizeof(float));

  bofs.close();

  return true;
}

ModelWriterInterface* ModelWriterFactory::GetModelWriter() {
  if (g_model_writer == NULL) {
    return new ModelWriter;
  } else {
    return g_model_writer;
  }
}

void ModelWriterFactory::SetModelWriter(ModelWriterInterface* writer) {
  g_model_writer = writer;
}

} // namespace arowpp
