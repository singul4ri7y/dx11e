struct VSOut {
	float4 color : COLOR;
	float4 position : SV_POSITION;
};

VSOut SDAH(float2 position : POSITION, float4 color : COLOR) {
	VSOut vso;
	vso.position = float4(position.x, position.y, 0.0f, 1.0f);
	vso.color = color;
	
	return vso;
}