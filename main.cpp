#include <unistd.h>
#include <string>
#include <fstream>
#include <math.h>

class Wav {
public:
  uint32_t bytes;
  uint8_t *data;
  uint32_t offset;

  Wav(std::string& path);
  ~Wav();
};

Wav::Wav(std::string& path) {
  std::ifstream inputFile(path.c_str());

  inputFile.seekg(0, inputFile.end);
  int size = static_cast<int>(inputFile.tellg());
  inputFile.seekg(0, inputFile.beg);

  this->offset = 44;
  this->bytes = size - this->offset;
  inputFile.seekg(this->offset, inputFile.beg);

  this->data = (uint8_t*)malloc(size);
  inputFile.read((char*)this->data, size - this->offset);
  inputFile.close();
}

Wav::~Wav() {
  free(this->data);
}

double distance(uint8_t* a, uint8_t* b, int size) {
  double sum = 0.0;
  for (int i = 0; i < size; i++){
    int aa = a[i];
    int bb = b[i];
    double d = sqrt((aa - bb) * (aa - bb));
    sum += d;
  }
  return sum / size;
}

int match(Wav* a, Wav* b) {
  double min = 1e+9;
  int index = -1;
  for (int i = 0; i < a->bytes - b->bytes; i++){
    double d = distance(&a->data[i], b->data, b->bytes);
    if (d < min) {
      min = d;
      index = i;
    }
  }
  return index;
}

int main(int argc, char *argv[]) {
  std::string sentencePath = "voice/yukkuri.wav";
  Wav* sentence = new Wav(sentencePath);
  printf("total: %d\n", sentence->bytes);

  std::string words[] = { "yu", "ku", "ri", "shi", "te", "i", "ne" };
  for (int i = 0; i < 7; i++){
    std::string yuPath = "voice/char/" + words[i] + ".wav";
    Wav* yu = new Wav(yuPath);
    int m = match(sentence, yu);
    printf("match(%s): %d\n", words[i].c_str(), m); 
    delete yu;
  }

  delete sentence;

  return 0;
}
