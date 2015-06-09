#include "tokenizer.h"
#include "logging.h"

namespace arowpp {

fv_t Tokenizer::Tokenize(const char* line, short* y,
                         std::size_t* maxid) {
  fv_t fv;

  *y = static_cast<short>(atoi(line));
  if (*y != 1 && *y != -1) {
    LOG(ERROR) << "Invalid label. A label must be +1 or -1.";
    return fv;
  }

  while (!isspace(*line) && *line) line++;
  while (isspace(*line) && *line) line++;
  while (1) {
    const char *begin = line;
    unsigned int index = static_cast<unsigned int>(std::atol(begin));
    while (*line != ':' && *line) line++;
    float val = static_cast<float>(std::atof(++line));

    while (*line != ' ' && *line) line++;
    fv.push_back(std::make_pair(index, val));
    if (index > *maxid) *maxid = index;
    if (*line++ == 0) break;
  }
  return fv;
}

} // namespace arowpp
