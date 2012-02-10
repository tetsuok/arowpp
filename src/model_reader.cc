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

#include "model_reader_interface.h"

#include <fstream>
#include "logging.h"
#include "param.h"

namespace arowpp {
namespace {

ModelReaderInterface* g_model_reader = NULL;

} // namespace

class ModelReader : public ModelReaderInterface {
 public:
  ModelReader() {}
  ~ModelReader() {}

  virtual bool Open(const char* filename, Param* param) const;

 private:
  ModelReader(const ModelReader&);
  const ModelReader& operator=(const ModelReader&);
};

bool ModelReader::Open(const char* filename, Param* param) const {
  std::ifstream bifs(filename, std::ios::in | std::ios::binary);
  if (!bifs) {
    LOG(ERROR) << "Failed to load " << filename;
    return false;
  }

  // Get number of feature, number of examples and number of update
  bifs.read(reinterpret_cast<char *>(&param->num_feature), sizeof(param->num_feature));
  bifs.read(reinterpret_cast<char *>(&param->num_example), sizeof(param->num_example));
  bifs.read(reinterpret_cast<char *>(&param->num_update), sizeof(param->num_update));
  bifs.read(reinterpret_cast<char *>(&param->num_iter), sizeof(param->num_iter));
  bifs.read(reinterpret_cast<char *>(&param->r), sizeof(param->r));
  bifs.read(reinterpret_cast<char *>(&param->is_shuffled), sizeof(param->is_shuffled));

  // Initialize
  param->Reset();

  bifs.read(reinterpret_cast<char *>(&param->mean[0]),
            static_cast<std::streamsize>(param->mean.size() * sizeof(float)));
  bifs.read(reinterpret_cast<char *>(&param->cov[0]),
            static_cast<std::streamsize>(param->cov.size() * sizeof(float)));

  bifs.close();

  return true;
}

ModelReaderInterface* ModelReaderFactory::GetModelReader() {
  if (g_model_reader == NULL) {
    return new ModelReader;
  } else {
    return g_model_reader;
  }
}

void ModelReaderFactory::SetModelReader(ModelReaderInterface* reader) {
  g_model_reader = reader;
}

} // namespace arowpp
