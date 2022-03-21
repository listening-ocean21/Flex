#include "fluidRenderD3D11.h"
#include "shadersDemoContext.h"
#include "demoContextD3D11.h"
#include "appD3D11Ctx.h"
#include <fstream>
#include <demo\d3d11\diffuseRenderD3D11.h>
#include <direct.h>
#include <io.h>

struct SParticle
{
	float3 position;
	float density;
	float4 aniX;
	float4 aniY;
	float4 aniZ;
	float speed;
};

struct SDiffuseParticle
{
	float4 position; // lifetime in w
	float3 velocity;
};

void ExportPartices(FluidRenderBuffers* vBuffer, std::string vExportFilePath, int vFrameIndex, int vSolidParticleNum)
{
	FluidRenderBuffersD3D11& buffer = *reinterpret_cast<FluidRenderBuffersD3D11*>(vBuffer);
	AppGraphCtxD3D11* context = reinterpret_cast<DemoContextD3D11*>(GetDemoContext())->m_appGraphCtxD3D11;
	ID3D11Buffer* posBuffer, * densityBuffer, * aniBuffer[3], * velocityBuffer;
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

	{
		D3D11_BUFFER_DESC bfDESC;

		ZeroMemory(&bfDESC, sizeof(D3D11_BUFFER_DESC));

		buffer.m_velocities.Get()->GetDesc(&bfDESC);
		bfDESC.BindFlags = 0;
		bfDESC.MiscFlags = 0;
		bfDESC.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		bfDESC.Usage = D3D11_USAGE_STAGING;
		context->m_device->CreateBuffer(&bfDESC, NULL, &velocityBuffer);
		context->m_deviceContext->CopyResource(velocityBuffer, buffer.m_velocities.Get());
	}

	vExportFilePath += std::to_string(vFrameIndex / 130);
	CreateDirectory(vExportFilePath.c_str(), NULL);
	vExportFilePath += "/";
	vExportFilePath += std::to_string(vFrameIndex);
	vExportFilePath += ".particle";
	std::ofstream* outfile = new std::ofstream(vExportFilePath, std::ios::binary);
	if (!outfile->is_open())
	{
		std::cout << "Cannot open a file to save particles.";
		return;
	}

	D3D11_MAPPED_SUBRESOURCE resultResources1, resultResources2, resultResources3, resultResources4, resultResources5, resultResources6;
	ZeroMemory(&resultResources1, sizeof(D3D11_MAPPED_SUBRESOURCE));
	ZeroMemory(&resultResources2, sizeof(D3D11_MAPPED_SUBRESOURCE));
	ZeroMemory(&resultResources3, sizeof(D3D11_MAPPED_SUBRESOURCE));
	ZeroMemory(&resultResources4, sizeof(D3D11_MAPPED_SUBRESOURCE));
	ZeroMemory(&resultResources5, sizeof(D3D11_MAPPED_SUBRESOURCE));
	ZeroMemory(&resultResources6, sizeof(D3D11_MAPPED_SUBRESOURCE));

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
	context->m_deviceContext->Map(velocityBuffer, 0, D3D11_MAP_READ, 0, &resultResources6);
	float3* p6 = (float3*)resultResources6.pData;
	for (int i = vSolidParticleNum; i < buffer.m_numParticles - vSolidParticleNum; i++)
	{
		SParticle data;
		data.position = float3(p1[i].x, p1[i].y, p1[i].z);
		data.density = p2[i];
		data.aniX = p3[i];
		data.aniY = p4[i];
		data.aniZ = p5[i];
		data.speed = p6[i].x * p6[i].x + p6[i].y * p6[i].y + p6[i].z * p6[i].z;

		outfile->write((char*)&data, sizeof(SParticle));
	}
	context->m_deviceContext->Unmap(posBuffer, 0);
	context->m_deviceContext->Unmap(densityBuffer, 0);
	context->m_deviceContext->Unmap(aniBuffer[0], 0);
	context->m_deviceContext->Unmap(aniBuffer[1], 0);
	context->m_deviceContext->Unmap(aniBuffer[2], 0);
	context->m_deviceContext->Unmap(velocityBuffer, 0);

	outfile->close();
	delete outfile;

	posBuffer->Release();
	densityBuffer->Release();
	aniBuffer[0]->Release();
	aniBuffer[1]->Release();
	aniBuffer[2]->Release();
	velocityBuffer->Release();
}

void ExportDiffusePartices(DiffuseRenderBuffers* diffuseBuffers, std::string vExportFilePath, int vFrameIndex, int vDiffuseCount)
{
	DiffuseRenderBuffersD3D11& buffer = *reinterpret_cast<DiffuseRenderBuffersD3D11*>(diffuseBuffers);
	AppGraphCtxD3D11* context = reinterpret_cast<DemoContextD3D11*>(GetDemoContext())->m_appGraphCtxD3D11;
	ID3D11Buffer* posBuffer, * velocityBuffer;
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

		buffer.m_velocities.Get()->GetDesc(&bfDESC);
		bfDESC.BindFlags = 0;
		bfDESC.MiscFlags = 0;
		bfDESC.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		bfDESC.Usage = D3D11_USAGE_STAGING;
		context->m_device->CreateBuffer(&bfDESC, NULL, &velocityBuffer);
		context->m_deviceContext->CopyResource(velocityBuffer, buffer.m_velocities.Get());
	}
	vExportFilePath += std::to_string(vFrameIndex / 130);
	vExportFilePath += "/";
	vExportFilePath += std::to_string(vFrameIndex);
	vExportFilePath += ".diffuse";
	std::ofstream* outfile = new std::ofstream(vExportFilePath, std::ios::binary);
	if (!outfile->is_open())
	{
		std::cout << "Cannot open a file to save diffuse particles.";
		return;
	}

	D3D11_MAPPED_SUBRESOURCE resultResources1, resultResources2;
	ZeroMemory(&resultResources1, sizeof(D3D11_MAPPED_SUBRESOURCE));
	ZeroMemory(&resultResources2, sizeof(D3D11_MAPPED_SUBRESOURCE));

	context->m_deviceContext->Map(posBuffer, 0, D3D11_MAP_READ, 0, &resultResources1);
	context->m_deviceContext->Map(velocityBuffer, 0, D3D11_MAP_READ, 0, &resultResources2);
	float4* p1 = (float4*)resultResources1.pData;
	float4* p2 = (float4*)resultResources2.pData;
	for (int i = 0; i < vDiffuseCount; i++)
	{
		SDiffuseParticle data;
		data.position = float4(p1[i].x, p1[i].y, p1[i].z, p1[i].w);
		data.velocity = float3(p2[i].x, p2[i].y, p2[i].z);
		outfile->write((char*)&data, sizeof(SDiffuseParticle));
	}
	context->m_deviceContext->Unmap(posBuffer, 0);
	context->m_deviceContext->Unmap(velocityBuffer, 0);

	outfile->close();
	delete outfile;

	posBuffer->Release();
	velocityBuffer->Release();
}

void ExportMeshIndex(Mesh* mesh, std::string vExportFilePath)
{
	std::ofstream* outfile = new std::ofstream(vExportFilePath + ".index", std::ios::binary);
	if (!outfile->is_open())
	{
		std::cout << "Cannot open a file to save diffuse particles.";
		return;
	}

	if (mesh) outfile->write((char*)mesh->m_indices.data(), sizeof(uint32_t) * mesh->m_indices.size());
	outfile->close();
	delete outfile;
}

void ExportMeshPos(Mesh* mesh, std::string vExportFilePath, int vFrameIndex)
{
	vExportFilePath += std::to_string(vFrameIndex / 130);
	vExportFilePath += "/";
	vExportFilePath += std::to_string(vFrameIndex);
	vExportFilePath += ".pos";
	std::ofstream* outfile = new std::ofstream(vExportFilePath, std::ios::binary);

	if (mesh) outfile->write((char*)mesh->m_positions.data(), sizeof(Point3) * mesh->m_positions.size());
	outfile->close();
	delete outfile;
}



//判断是否是".."目录和"."目录
bool is_special_dir(const char* path)
{
	return strcmp(path, "..") == 0 || strcmp(path, ".") == 0;
}

//判断文件属性是目录还是文件
bool is_dir(int attrib)
{
	return attrib == 16 || attrib == 18 || attrib == 20;
}

void get_file_path(const char* path, const char* file_name, char* file_path)
{
	strcpy_s(file_path, sizeof(char) * _MAX_PATH, path);
	file_path[strlen(file_path) - 1] = '\0';
	strcat_s(file_path, sizeof(char) * _MAX_PATH, file_name);
	strcat_s(file_path, sizeof(char) * _MAX_PATH, "\\*");
}

void show_error(const char* file_name)
{
	errno_t err;
	_get_errno(&err);
	switch (err)
	{
	case ENOTEMPTY:
		printf("Given path is not a directory, the directory is not empty, or the directory is either the current working directory or the root directory.\n");
		break;
	case ENOENT:
		printf("Path is invalid.\n");
		break;
	case EACCES:
		printf("%s had been opend by some application, can't delete.\n", file_name);
		break;
	}
}

void DeleteAllFiles(std::string strPath)
{
	_finddata_t dir_info;  // 文件夹信息
	_finddata_t file_info;  // 文件信息
	intptr_t f_handle;

	char tmp_path[_MAX_PATH];

	if ((f_handle = _findfirst(strPath.c_str(), &dir_info)) != -1)
	{
		while ((_findnext(f_handle, &file_info)) == 0)
		{
			if (is_special_dir(file_info.name))
				continue;
			if (is_dir(file_info.attrib))   //如果是目录，生成完整的路径
			{
				get_file_path(strPath.c_str(), file_info.name, tmp_path);
				DeleteAllFiles(tmp_path);    //开始递归删除目录中的内容
				tmp_path[strlen(tmp_path) - 2] = '\0';
				if (file_info.attrib == 20)
					printf("This is system file, can't delete!\n");
				else
				{
					// 删除空目录,必须在递归返回前调用 _findclose, 否则无法删除目录
					if (_rmdir(tmp_path) == -1)
						show_error(file_info.name);//目录非空则会显示出错原因
				}
			}
			else
			{
				strcpy_s(tmp_path, strPath.c_str());
				tmp_path[strlen(tmp_path) - 1] = '\0';
				strcat_s(tmp_path, file_info.name);  // 生成完整文件路径

				if (remove(tmp_path) == -1)
				{
					show_error(file_info.name);
				}
			}
		}
		_findclose(f_handle);//关闭打开的文件句柄，并释放关联资源，否则无法删除空目录
	}
	else
	{
		show_error(file_info.name);//若路径不存在，显示错误信息
	}
}

void DeleteAllFile(std::string strPath)
{
	strPath += "\\*";
	DeleteAllFiles(strPath);
}