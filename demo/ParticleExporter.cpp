#include "fluidRenderD3D11.h"
#include "shadersDemoContext.h"
#include "demoContextD3D11.h"
#include "appD3D11Ctx.h"
#include <fstream>
#include <demo\d3d11\diffuseRenderD3D11.h>

struct SParticle
{
	float3 position;
	float density;
	float4 aniX;
	float4 aniY;
	float4 aniZ;
};

struct SDiffuseParticle
{
	float3 position;
};

void ExportPartices(FluidRenderBuffers* vBuffer, std::string vExportFilePath, int vFrameIndex)
{
	FluidRenderBuffersD3D11& buffer = *reinterpret_cast<FluidRenderBuffersD3D11*>(vBuffer);
	AppGraphCtxD3D11* context = reinterpret_cast<DemoContextD3D11*>(GetDemoContext())->m_appGraphCtxD3D11;
	ID3D11Buffer *posBuffer, *densityBuffer, *aniBuffer[3];
	{
		D3D11_BUFFER_DESC bfDESC;

		ZeroMemory(&bfDESC, sizeof(D3D11_BUFFER_DESC));

		buffer.m_positions.Get()->GetDesc(&bfDESC);
		bfDESC.BindFlags = 0;
		bfDESC.MiscFlags = 0;
		bfDESC.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		bfDESC.Usage = D3D11_USAGE_STAGING;
		context->m_device->CreateBuffer(&bfDESC, NULL, &posBuffer);
		context->m_deviceContext->CopyResource(posBuffer, buffer.m_positions.Get());
	}

	{
		D3D11_BUFFER_DESC bfDESC;

		ZeroMemory(&bfDESC, sizeof(D3D11_BUFFER_DESC));

		buffer.m_densities.Get()->GetDesc(&bfDESC);
		bfDESC.BindFlags = 0;
		bfDESC.MiscFlags = 0;
		bfDESC.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		bfDESC.Usage = D3D11_USAGE_STAGING;
		context->m_device->CreateBuffer(&bfDESC, NULL, &densityBuffer);
		context->m_deviceContext->CopyResource(densityBuffer, buffer.m_densities.Get());
	}

	{
		D3D11_BUFFER_DESC bfDESC;

		ZeroMemory(&bfDESC, sizeof(D3D11_BUFFER_DESC));

		buffer.m_anisotropiesArr[0].Get()->GetDesc(&bfDESC);
		bfDESC.BindFlags = 0;
		bfDESC.MiscFlags = 0;
		bfDESC.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		bfDESC.Usage = D3D11_USAGE_STAGING;
		context->m_device->CreateBuffer(&bfDESC, NULL, &aniBuffer[0]);
		context->m_deviceContext->CopyResource(aniBuffer[0], buffer.m_anisotropiesArr[0].Get());
	}

	{
		D3D11_BUFFER_DESC bfDESC;

		ZeroMemory(&bfDESC, sizeof(D3D11_BUFFER_DESC));

		buffer.m_anisotropiesArr[1].Get()->GetDesc(&bfDESC);
		bfDESC.BindFlags = 0;
		bfDESC.MiscFlags = 0;
		bfDESC.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		bfDESC.Usage = D3D11_USAGE_STAGING;
		context->m_device->CreateBuffer(&bfDESC, NULL, &aniBuffer[1]);
		context->m_deviceContext->CopyResource(aniBuffer[1], buffer.m_anisotropiesArr[1].Get());
	}

	{
		D3D11_BUFFER_DESC bfDESC;

		ZeroMemory(&bfDESC, sizeof(D3D11_BUFFER_DESC));

		buffer.m_anisotropiesArr[2].Get()->GetDesc(&bfDESC);
		bfDESC.BindFlags = 0;
		bfDESC.MiscFlags = 0;
		bfDESC.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		bfDESC.Usage = D3D11_USAGE_STAGING;
		context->m_device->CreateBuffer(&bfDESC, NULL, &aniBuffer[2]);
		context->m_deviceContext->CopyResource(aniBuffer[2], buffer.m_anisotropiesArr[2].Get());
	}



	vExportFilePath += std::to_string(vFrameIndex);
	vExportFilePath += ".bin";
	std::ofstream* outfile = new std::ofstream(vExportFilePath, std::ios::binary);
	if (!outfile->is_open())
	{
		std::cout << "Cannot open a file to save VTK particles.";
		return;
	}

	D3D11_MAPPED_SUBRESOURCE resultResources1, resultResources2, resultResources3, resultResources4, resultResources5;
	ZeroMemory(&resultResources1, sizeof(D3D11_MAPPED_SUBRESOURCE));
	ZeroMemory(&resultResources2, sizeof(D3D11_MAPPED_SUBRESOURCE));
	ZeroMemory(&resultResources3, sizeof(D3D11_MAPPED_SUBRESOURCE));
	ZeroMemory(&resultResources4, sizeof(D3D11_MAPPED_SUBRESOURCE));
	ZeroMemory(&resultResources5, sizeof(D3D11_MAPPED_SUBRESOURCE));
	
	context->m_deviceContext->Map(posBuffer, 0, D3D11_MAP_READ, 0, &resultResources1);
	float4* p1 = (float4*)resultResources1.pData;
	context->m_deviceContext->Map(densityBuffer, 0, D3D11_MAP_READ, 0, &resultResources2);
	float* p2 = (float*)resultResources2.pData;
	context->m_deviceContext->Map(aniBuffer[0], 0, D3D11_MAP_READ, 0, &resultResources3);
	float4* p3 = (float4*)resultResources3.pData;
	context->m_deviceContext->Map(aniBuffer[1], 0, D3D11_MAP_READ, 0, &resultResources4);
	float4* p4 = (float4*)resultResources4.pData;
	context->m_deviceContext->Map(aniBuffer[2], 0, D3D11_MAP_READ, 0, &resultResources5);
	float4* p5 = (float4*)resultResources5.pData;
	for (int i = 0; i < buffer.m_numParticles; i++)
	{
		SParticle data;
		data.position = float3(p1[i].x, p1[i].y, p1[i].z);
		data.density = p2[i];
		data.aniX = p3[i];
		data.aniY = p4[i];
		data.aniZ = p5[i];

		outfile->write((char*)&data, sizeof(SParticle));
	}
	context->m_deviceContext->Unmap(posBuffer, 0);
	context->m_deviceContext->Unmap(densityBuffer, 0);

	outfile->close();
	delete outfile;
}

void ExportDiffusePartices(DiffuseRenderBuffers* diffuseBuffers, std::string vExportFilePath,int vFrameIndex)
{
	DiffuseRenderBuffersD3D11& buffer = *reinterpret_cast<DiffuseRenderBuffersD3D11*>(diffuseBuffers);
	AppGraphCtxD3D11* context = reinterpret_cast<DemoContextD3D11*>(GetDemoContext())->m_appGraphCtxD3D11;
	ID3D11Buffer* posBuffer;
	{
		D3D11_BUFFER_DESC bfDESC;

		ZeroMemory(&bfDESC, sizeof(D3D11_BUFFER_DESC));

		buffer.m_positions.Get()->GetDesc(&bfDESC);
		bfDESC.BindFlags = 0;
		bfDESC.MiscFlags = 0;
		bfDESC.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		bfDESC.Usage = D3D11_USAGE_STAGING;
		context->m_device->CreateBuffer(&bfDESC, NULL, &posBuffer);
		context->m_deviceContext->CopyResource(posBuffer, buffer.m_positions.Get());
	}

	vExportFilePath += std::to_string(vFrameIndex);
	vExportFilePath += ".diffuse";
	std::ofstream* outfile = new std::ofstream(vExportFilePath, std::ios::binary);
	if (!outfile->is_open())
	{
		std::cout << "Cannot open a file to save VTK particles.";
		return;
	}

	D3D11_MAPPED_SUBRESOURCE resultResources1;
	ZeroMemory(&resultResources1, sizeof(D3D11_MAPPED_SUBRESOURCE));

	context->m_deviceContext->Map(posBuffer, 0, D3D11_MAP_READ, 0, &resultResources1);
	float4* p1 = (float4*)resultResources1.pData;
	for (int i = 0; i < buffer.m_numParticles; i++)
	{
		SDiffuseParticle data;
		data.position = float3(p1[i].x, p1[i].y, p1[i].z);

		outfile->write((char*)&data, sizeof(SDiffuseParticle));
	}
	context->m_deviceContext->Unmap(posBuffer, 0);

	outfile->close();
	delete outfile;
}