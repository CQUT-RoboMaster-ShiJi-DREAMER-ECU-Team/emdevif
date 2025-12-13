/**
 * @file test_main.hpp
 * @brief 测试主函数
 */

#pragma once
#ifndef EMDEVIF_TEST_MAIN_HPP
#define EMDEVIF_TEST_MAIN_HPP

extern "C" void cFeatureTest(void);

void endianTest();
void typeTraitsTest();

void initOnceTest();
void bitIntTest();
void mapTest();
void ringBufferTest();

void loggerTest();

void heapTest();
void sysQueueTest();
void threadAndMutexTest();

#endif  // !EMDEVIF_TEST_MAIN_HPP
