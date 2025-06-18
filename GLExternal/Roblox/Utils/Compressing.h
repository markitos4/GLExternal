#pragma once

#include "Luau/Compiler.h"
#include "Luau/BytecodeBuilder.h"
#include "Luau/BytecodeUtils.h"
#include "Zstd/xxhash.h"
#include "Zstd/zstd.h"

static class BytecodeEncoderClass : public Luau::BytecodeEncoder {
    inline void encode(uint32_t* Data, size_t Size) override {
        for (auto i = 0u; i < Size;) {
            auto& OpCode = *reinterpret_cast<uint8_t*>(Data + i);
            i += Luau::getOpLength(LuauOpcode(OpCode));
            OpCode *= 227;
        }
    }
};

namespace LuauCompress {
    static std::string ZstdCompress(const std::string_view Bytecode)
    {
        const auto MaxSize = ZSTD_compressBound(Bytecode.size());
        auto Buffer = std::vector<char>(MaxSize + 8);

        strcpy_s(&Buffer[0], Buffer.capacity(), "RSB1");

        const auto Size = Bytecode.size();

        memcpy_s(&Buffer[4], Buffer.capacity(), &Size, sizeof(Size));

        const auto compressed_size = ZSTD_compress(&Buffer[8], MaxSize, Bytecode.data(), Size, ZSTD_maxCLevel());
        if (ZSTD_isError(compressed_size))
            return "";

        const auto FinalSize = compressed_size + 8;
        const auto HashKey = XXH32(Buffer.data(), FinalSize, 42u);
        const auto Bytes = reinterpret_cast<const uint8_t*>(&HashKey);

        for (auto i = 0u; i < FinalSize; ++i)
            Buffer[i] ^= Bytes[i % 4] + i * 41u;

        return std::string(Buffer.data(), FinalSize);
    }

    static std::string Compile(const std::string& source)
    {
        BytecodeEncoderClass Encoder;

        static const char* MutableGlobals[] = {
            ("Game"),
            ("Workspace"),
            ("game"),
            ("plugin"),
            ("script"),
            ("shared"),
            ("workspace"),
            ("_G"),
            nullptr
        };

        static const char* VectorLib = ("Vector3");
        static const char* VectorFunction = ("new");

        Luau::CompileOptions Compilation;
        Compilation.optimizationLevel = 1;
        Compilation.debugLevel = 1;

        Compilation.vectorLib = VectorLib;
        Compilation.vectorCtor = VectorFunction;
        Compilation.vectorType = VectorLib;
        Compilation.mutableGlobals = MutableGlobals;

        return Luau::compile(source, Compilation, { true, true }, &Encoder);
    }
};