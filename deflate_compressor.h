#ifndef DEFLATE_COMPRESSOR_H
#define DEFLATE_COMPRESSOR_H

#include "zip_config.h"
#include "huffman.h"
#include "lz77.h"

namespace shed_zip{
    class DeflateCompressor{
        public:
            DeflateCompressor(ZipConfig cfg = ZipConfig());

            shed_std::Vvector<uint8_t> compress(const shed_std::Vvector<uint8_t>& input);
        private:
            ZipConfig config;
    };
    #include "deflate_compressor.tpp"
}

#endif // DEFLATE_COMPRESSOR_H