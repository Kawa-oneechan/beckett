//XclSR2 by artcreativecode
in vec2 TexCoords;
out vec4 fragColor;

#include "common.fs"

// 3D rectangular tunnel effect with movement and diagonal wall lines
precision highp float;

// Define line color and background color as constants
const vec3 color1 = vec3(0.03137, 0.17647, 0.25882);
const vec3 color2 = vec3(0.72157, 0.58431, 0.77255);
const vec3 color3 = vec3(0.0, 0.64314, 0.69412);

const vec3 uLineColor = color2; 
const vec3 uBackgroundColor = color1;
const float uSpeed = 2.5;

// Function to draw a line given two endpoints, line width, and uv coordinates
void drawLine(vec2 uv, vec2 start, vec2 end, float lineWidth, inout vec3 color) {
	float lineDist;

	if (start.x == end.x) {
		// Handle vertical line
		lineDist = abs(uv.x - start.x);
	} else {
		// Non-vertical line
		float m = (end.y - start.y) / (end.x - start.x);
		float b = end.y - m * end.x;
		lineDist = abs(uv.y - m * uv.x - b) / sqrt(m * m + 1.0);
	}

	// Anti-aliasing factor
	float aaWidth = 1.0 / ScreenRes.y; // Adjust for screen resolution

	// Calculate line intensity based on distance from the line and apply anti-aliasing
	float alpha = smoothstep(lineWidth, lineWidth + aaWidth, lineDist);
	// Blend the line color with the background
	color = mix(color, uLineColor, 1.0 - alpha);
}


void main()
{
	// Normalized pixel coordinates (from -1 to 1)
	vec2 uv = (gl_FragCoord.xy - 0.5 * ScreenRes.xy) / ScreenRes.y;

	float time = -TotalTime * 2.5;

	// Tunnel parameters
	float depth = 10.0; // Total depth of the tunnel
	float numRects = 10.0; // Number of rectangles in the tunnel
	float spacing = depth / numRects; // Spacing between rectangles

	// Calculate the depth of the nearest and farthest rectangles
	float nearestDepth = mod(time, spacing);
	float farthestDepth = depth;

	// Initialize color
	vec3 color = uBackgroundColor;

	// Aspect ratio for the rectangles (making them wider)
	float aspectRatio = 1.5; // Width is 1.5 times the height

	// Bounds of the nearest and farthest rectangles
	vec2 nearestLowerBound = -vec2(0.8 * aspectRatio, 0.8) * (1.0 / nearestDepth);
	vec2 nearestUpperBound = vec2(0.8 * aspectRatio, 0.8) * (1.0 / nearestDepth);
	vec2 farthestLowerBound = -vec2(0.8 * aspectRatio, 0.8) * (1.0 / farthestDepth);
	vec2 farthestUpperBound = vec2(0.8 * aspectRatio, 0.8) * (1.0 / farthestDepth);

	// Fixed and farthest points for the rectangle corners
	vec2 nearestFixedUpperRight = vec2(0.8 * aspectRatio, 0.8);
	vec2 nearestFixedUpperLeft = vec2(-0.8 * aspectRatio, 0.8);
	vec2 farthestUpperLeft = vec2(-0.8 * aspectRatio, 0.8) * (1.0 / farthestDepth);
	vec2 nearestFixedLowerRight = vec2(0.8 * aspectRatio, -0.8);
	vec2 farthestLowerRight = vec2(0.8 * aspectRatio, -0.8) * (1.0 / farthestDepth);
	vec2 farthestUpperRight = vec2(0.8 * aspectRatio, 0.8) * (1.0 / farthestDepth);

	float lineWidth = .2 / ScreenRes.y; // Width of the lines
	
	// Draw the main diagonal lines
	drawLine(uv, farthestUpperBound, nearestFixedUpperRight, lineWidth, color);
	drawLine(uv, farthestUpperLeft, nearestFixedUpperLeft, lineWidth, color);
	
	// Draw the additional lines between the main diagonal lines
	for (int i = 0; i < 8; i++) {
		float t = float(i) / 8.0; // Normalized position for each line
		vec2 start = mix(farthestUpperLeft, farthestUpperBound, t);
		vec2 end = mix(nearestFixedUpperLeft, nearestFixedUpperRight, t);
		drawLine(uv, start, end, lineWidth, color);
	}

	// Draw the diagonal lines on the right side
	for (int i = 0; i < 8; i++) {
		float t = float(i) / 8.0; // Normalized position for each line
		vec2 startRight = mix(farthestLowerRight, farthestUpperRight, t);
		vec2 endRight = mix(nearestFixedLowerRight, nearestFixedUpperRight, t);
		drawLine(uv, startRight, endRight, lineWidth, color);
	}
	
	if (uv.x > farthestLowerBound.x && uv.x < farthestUpperBound.x &&
		uv.y > farthestLowerBound.y && uv.y < farthestUpperBound.y) {
		color = uBackgroundColor; 
	}
	
	// Check for border (1 pixel wide)
	  float borderWidth = 1.0 / ScreenRes.y;

	// Draw each moving rectangle and the constant farthest rectangle
	for (float i = 0.0; i <= numRects; ++i) {
		float currentDepth = (i < numRects) ? (nearestDepth + i * spacing) : farthestDepth;
		float scale = 1.0 / currentDepth;

		// Rectangle bounds
		vec2 lowerBound = -vec2(0.8 * aspectRatio, 0.8) * scale;
		vec2 upperBound = vec2(0.8 * aspectRatio, 0.8) * scale;
			// Check if the current pixel is within the rectangle bounds
		if (uv.x > lowerBound.x && uv.x < upperBound.x &&
		uv.y > lowerBound.y && uv.y < upperBound.y) {

		  
		  if (abs(uv.x - lowerBound.x) < borderWidth || abs(uv.x - upperBound.x) < borderWidth ||
			  abs(uv.y - lowerBound.y) < borderWidth || abs(uv.y - upperBound.y) < borderWidth) {
			  color = uLineColor; 
		  }
	  }
	}


  fragColor = vec4(color, 1.0);
}
