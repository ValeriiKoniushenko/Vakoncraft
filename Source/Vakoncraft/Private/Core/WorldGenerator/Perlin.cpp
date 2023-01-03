﻿#include "Core/WorldGenerator/Perlin.h"

namespace db
{
	// Permutation table, the second half is a mirror of the first half.
	static const unsigned char p[512] = {
		151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142,
		8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203,
		117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74,
		165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220,
		105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132,
		187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186,
		3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59,
		227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70,
		221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178,
		185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241,
		81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176,
		115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195,
		78, 66, 215, 61, 156, 180,

		151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142,
		8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203,
		117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74,
		165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220,
		105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132,
		187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186,
		3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59,
		227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70,
		221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178,
		185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241,
		81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176,
		115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195,
		78, 66, 215, 61, 156, 180,
	};

	template <typename T>
	static T lerp(T a, T b, T t)
	{
		return a + t * (b - a);
	}

	template <typename T>
	static int floor(T x)
	{
		const auto xi = static_cast<int>(x);
		return (x < T(xi)) ? xi - 1 : xi;
	}

	template <typename T>
	static T fade(T t)
	{
		return t * t * t * (t * (t * T(6.0) - T(15.0)) + T(10.0));
	}

	template <typename T>
	static T dot_grad(int hash, T xf)
	{
		// In 1D case, the gradient may be either 1 or -1
		// The distance vector is the input offset (relative to the smallest bound)
		return (hash & 0x1) ? xf : -xf;
	}

	template <typename T>
	static T dot_grad(int hash, T xf, T yf)
	{
		// In 2D case, the gradient may be any of 8 direction vectors pointing to the
		// edges of a unit-square. The distance vector is the input offset (relative to
		// the smallest bound)
		switch (hash & 0x7)
		{
		case 0x0: return xf + yf;
		case 0x1: return xf;
		case 0x2: return xf - yf;
		case 0x3: return -yf;
		case 0x4: return -xf - yf;
		case 0x5: return -xf;
		case 0x6: return -xf + yf;
		case 0x7: return yf;
		default: return T(0.0);
		}
	}

	template <typename T>
	static T dot_grad(int hash, T xf, T yf, T zf)
	{
		// In 3D case, the gradient may be any of 12 direction vectors pointing to the edges
		// of a unit-cube (rounded to 16 with duplications). The distance vector is the input
		// offset (relative to the smallest bound)
		switch (hash & 0xF)
		{
		case 0x0: return xf + yf;
		case 0x1: return -xf + yf;
		case 0x2: return xf - yf;
		case 0x3: return -xf - yf;
		case 0x4: return xf + zf;
		case 0x5: return -xf + zf;
		case 0x6: return xf - zf;
		case 0x7: return -xf - zf;
		case 0x8: return yf + zf;
		case 0x9: return -yf + zf;
		case 0xA: return yf - zf;
		case 0xB: return -yf - zf;
		case 0xC: return yf + xf;
		case 0xD: return -yf + zf;
		case 0xE: return yf - xf;
		case 0xF: return -yf - zf;
		default: return T(0.0);
		}
	}

	template <typename T>
	T perlin(T x)
	{
		// Left coordinate of the unit-line that contains the input
		const int xi0 = floor(x);

		// Input location in the unit-line
		const T xf0 = x - T(xi0);
		const T xf1 = xf0 - T(1.0);

		// Wrap to range 0-255
		const int xi = xi0 & 0xFF;

		// Apply the fade function to the location
		const T u = fade(xf0);

		// Generate hash values for each point of the unit-line
		const int h0 = p[xi + 0];
		const int h1 = p[xi + 1];

		// Linearly interpolate between dot products of each gradient with its distance to the input location
		return lerp(dot_grad(h0, xf0), dot_grad(h1, xf1), u);
	}

	template <typename T>
	T perlin(T x, T y)
	{
		// Top-left coordinates of the unit-square
		const int xi0 = floor(x) & 0xFF;
		const int yi0 = floor(y) & 0xFF;

		// Input location in the unit-square
		const T xf0 = x - T(xi0);
		const T yf0 = y - T(yi0);
		const T xf1 = xf0 - T(1.0);
		const T yf1 = yf0 - T(1.0);

		// Wrap to range 0-255
		const int xi = xi0 & 0xFF;
		const int yi = yi0 & 0xFF;

		// Apply the fade function to the location
		const T u = fade(xf0);
		const T v = fade(yf0);

		// Generate hash values for each point of the unit-square
		const int h00 = p[p[xi + 0] + yi + 0];
		const int h01 = p[p[xi + 0] + yi + 1];
		const int h10 = p[p[xi + 1] + yi + 0];
		const int h11 = p[p[xi + 1] + yi + 1];

		// Linearly interpolate between dot products of each gradient with its distance to the input location
		const T x1 = lerp(dot_grad(h00, xf0, yf0), dot_grad(h10, xf1, yf0), u);
		const T x2 = lerp(dot_grad(h01, xf0, yf1), dot_grad(h11, xf1, yf1), u);
		return lerp(x1, x2, v);
	}

	template <typename T>
	T perlin(T x, T y, T z)
	{
		// Top-left coordinates of the unit-cube
		const int xi0 = floor(x);
		const int yi0 = floor(y);
		const int zi0 = floor(z);

		// Input location in the unit-cube
		const T xf0 = x - T(xi0);
		const T yf0 = y - T(yi0);
		const T zf0 = z - T(zi0);
		const T xf1 = xf0 - T(1.0);
		const T yf1 = yf0 - T(1.0);
		const T zf1 = zf0 - T(1.0);

		// Wrap to range 0-255
		const int xi = xi0 & 0xFF;
		const int yi = yi0 & 0xFF;
		const int zi = zi0 & 0xFF;

		// Apply the fade function to the location
		const T u = fade(xf0);
		const T v = fade(yf0);
		const T w = fade(zf0);

		// Generate hash values for each point of the unit-cube
		const int h000 = p[p[p[xi + 0] + yi + 0] + zi + 0];
		const int h001 = p[p[p[xi + 0] + yi + 0] + zi + 1];
		const int h010 = p[p[p[xi + 0] + yi + 1] + zi + 0];
		const int h011 = p[p[p[xi + 0] + yi + 1] + zi + 1];
		const int h100 = p[p[p[xi + 1] + yi + 0] + zi + 0];
		const int h101 = p[p[p[xi + 1] + yi + 0] + zi + 1];
		const int h110 = p[p[p[xi + 1] + yi + 1] + zi + 0];
		const int h111 = p[p[p[xi + 1] + yi + 1] + zi + 1];

		// Linearly interpolate between dot products of each gradient with its distance to the input location
		const T x11 = lerp(dot_grad(h000, xf0, yf0, zf0), dot_grad(h100, xf1, yf0, zf0), u);
		const T x12 = lerp(dot_grad(h010, xf0, yf1, zf0), dot_grad(h110, xf1, yf1, zf0), u);
		const T x21 = lerp(dot_grad(h001, xf0, yf0, zf1), dot_grad(h101, xf1, yf0, zf1), u);
		const T x22 = lerp(dot_grad(h011, xf0, yf1, zf1), dot_grad(h111, xf1, yf1, zf1), u);

		const T y1 = lerp(x11, x12, v);
		const T y2 = lerp(x21, x22, v);

		return lerp(y1, y2, w);
	}
}

template float db::perlin<float>(float x);
template float db::perlin<float>(float x, float y);
template float db::perlin<float>(float x, float y, float z);

template double db::perlin<double>(double x);
template double db::perlin<double>(double x, double y);
template double db::perlin<double>(double x, double y, double z);
