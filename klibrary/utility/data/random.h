#pragma once

#include "math/math.h"

namespace kl::random
{
bool gen_bool();
uint8_t gen_byte();
RGB gen_rgb(bool gray = false);

int gen_int(int start_inclusive, int end_exclusive);
int gen_int(int end_exclusive);

int2 gen_int2(int start_inclusive, int end_exclusive);
int2 gen_int2(int end_exclusive);

float gen_float(float start_inclusive, float end_inclusive);
float gen_float(float end_inclusive);
float gen_float();

float2 gen_float2(float start_inclusive, float end_inclusive);
float2 gen_float2(float end_inclusive);
float2 gen_float2();

float3 gen_float3(float start_inclusive, float end_inclusive);
float3 gen_float3(float end_inclusive);
float3 gen_float3();

float4 gen_float4(float start_inclusive, float end_inclusive);
float4 gen_float4(float end_inclusive);
float4 gen_float4();

float3x3 gen_float3x3(float start_inclusive, float end_inclusive);
float3x3 gen_float3x3(float end_inclusive);
float3x3 gen_float3x3();

float4x4 gen_float4x4(float start_inclusive, float end_inclusive);
float4x4 gen_float4x4(float end_inclusive);
float4x4 gen_float4x4();

char gen_char(bool upper = false);
std::string gen_string(int length, bool upper = false);
} // namespace kl::random

namespace kl
{
struct RandomInit
{
    static thread_local std::mt19937 _init;
};
} // namespace kl
