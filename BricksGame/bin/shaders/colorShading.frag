#version 330 core
//The fragment shader operates on each pixel in a given polygon
in vec4 fragmentColor; //color interpolated, mix together 
//to avoid the interpolation we can use "flat" before out
//3 component float vector that gets outputted to the screen for each pixel
out vec4 color; //mix color and take into account the alpha (vec4)

in vec2 fragmentPosition;
in vec2 fragmentUV;

//To apply a var in the whole mesh we use uniform
//it's a global variable
//For example a time variable or a var for colors.
//if time == 0 the color animation will not be used
uniform float time;

//Sampler texture 
uniform sampler2D tSampler;

void main()
{
	//fragmentColor comes from vertex shader
	//Get RGBA color from the texture
	vec4 textureColor = texture( 
		tSampler, /* UV coordinates */
		fragmentUV
	);
	if (time>0.0){
		//arcade mode
		color = textureColor *  //color blending equals to multiplication
		vec4(
			fragmentColor.r * (cos(fragmentPosition.x *4.0 + time) + 1.0)*0.5,
				fragmentColor.g * (cos(fragmentPosition.y * 8.0 + time) + 1.0)*0.5,
			fragmentColor.b * (cos(3.14 * time) + 1.0)*0.5,
				fragmentColor.a
			);
	} else {
		//normal mode
		vec4 textureColor = texture( 
			tSampler, 
			fragmentUV
		);
		color = textureColor * fragmentColor;
	}
}