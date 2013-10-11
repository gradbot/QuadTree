#include "stdafx.h"

struct MyShaders 
{
	ID3D11VertexShader *pVS;
	ID3D11PixelShader *pPS;
	ID3D11InputLayout *pLayout;

	MyShaders(ID3D11Device *dev, ID3D11DeviceContext *devcon)
	{
		const char* shaders = "\
		struct VOut\
		{\
			float4 position : SV_POSITION;\
			float4 color : COLOR;\
		};\
		\
		VOut VShader(float4 position : POSITION, float4 color : COLOR)\
		{\
			VOut output;\
			\
			output.position = float4((position.xy - float2(512, 512)) / 512, position.zw);\
			output.color = color;\
			\
			return output;\
		}\
		\
		float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET\
		{\
			return color;\
		}";

		// compile the two shaders
		ID3D10Blob *VS, *PS;
		D3DCompile(shaders, strlen(shaders), 0, 0, 0, "VShader", "vs_4_0", 0, 0, &VS, 0);
		D3DCompile(shaders, strlen(shaders), 0, 0, 0, "PShader", "ps_4_0", 0, 0, &PS, 0);

		// encapsulate both shaders into shader objects
		dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
		dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

		// set the shader objects
		devcon->VSSetShader(pVS, 0, 0);
		devcon->PSSetShader(pPS, 0, 0);

		// create the input layout object
		D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
		devcon->IASetInputLayout(pLayout);
	}

	~MyShaders()
	{
		pVS->Release();
		pPS->Release();
		pLayout->Release();
	}

};
