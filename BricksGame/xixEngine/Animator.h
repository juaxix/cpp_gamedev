#pragma once
namespace xixEngine {
	///<summary>Inline function to make a smooth interpolation between 2 floats values. Used in animations</summary>
	///<param>from</param> origin
	///<param>to</param> destiny
	///<param>factor</param> 0 to 1 value, the factor of the interpolation
	///<return>value of the result of the interpolation</return>
	extern inline float lerp(const float from, const float to, float factor) { return from + (to - from) * factor; } /** @TODO: Replace with a tween class in the engine */
}