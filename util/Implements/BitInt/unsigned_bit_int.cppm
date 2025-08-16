/**
 * @file unsigned_bit_int.cppm
 * @module emdevif.util.BitInt:unsigned_partial
 * @author DuYicheng
 * @date 2025-08-16
 * @brief 无符号指定宽度整型
 */

module;

export module emdevif.util.BitInt:unsigned_partial;
import :signed_partial;

export namespace emdevif {

template<BitsType_t bits>
    requires ValidBitIntWidth<bits>
class UBitInt;

}
