struct VertexIn {
	float2 position : POSITION;
	float4 color : COLOR;
};

struct VertexOut {
	float4 color : COLOR;
	float4 position : SV_POSITION;
};

VertexOut SDAH(VertexIn input) {
	VertexOut vso;
	vso.position = float4(input.position.x, input.position.y, 0.0f, 1.0f);
	vso.color = input.color;
	
	return vso;
}