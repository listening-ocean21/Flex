class Buoyancy : public Scene
{
public:

	Buoyancy(const char* name) : Scene(name) {}

	virtual void Initialize()
	{	
		float radius = 0.1f;
		float restDistance = radius*0.5f;
		int group = 0;

		int n = 1;
		float spacing = 64*restDistance*0.9f/(2.0f*n);
		float sampling = restDistance*0.8f;
		Vec3 size = sampling*12.0f;

		//TODO:Mass
		const float mass[] = {5.0f, 2.5f, 5.f, 5.f };

		for (int j=0; j < 1; ++j)
			for (int i=0; i < n; ++i)
				CreateParticleShape(GetFilePathByPlatform("../../data/sphere.ply").c_str(), Vec3(spacing - 0.5f*size.x + i*spacing * 2, 2.0f + j*size.y*1.2f, 0.6f), size, 0.0f, sampling, Vec3(0.0f, 0.0f, 0.0f), mass[i], true, 1.0f, NvFlexMakePhase(group++, 0), true, 0.0001f);
			

		//CreateParticleShape(GetFilePathByPlatform("../../data/dragon.obj").c_str(), Vec3(spacing - 0.5f * size.x + 3 * spacing * 2, 2.0f + 0 * size.y * 1.2f, 0.6f), size, 0.0f, sampling, Vec3(0.0f, 0.0f, 0.0f), mass[3], true, 1.0f, NvFlexMakePhase(group++, 0), true, 0.01f);

		g_numSolidParticles = g_buffers->positions.size();	

		int x = 64;
		int y = 20;
		int z = 32;

		CreateParticleGrid(Vec3(0.0f), x, y, z, restDistance*0.9f, Vec3(0.0f), 1.0f, false, 0.0f, NvFlexMakePhase(group++, eNvFlexPhaseSelfCollide | eNvFlexPhaseFluid));
		//CreateParticleGrid(Vec3(0.0f, restDistance, 0.0f), x, 2*y, z, restDistance*0.9f, Vec3(0.0f), 1.0f, false, 0.0f, NvFlexMakePhase(group++, eNvFlexPhaseSelfCollide | eNvFlexPhaseFluid));
		//CreateParticleGrid(Vec3(5, restDistance, 5), x, 2*y, z, restDistance*0.9f, Vec3(0.0f), 1.0f, false, 0.0f, NvFlexMakePhase(group++, eNvFlexPhaseSelfCollide | eNvFlexPhaseFluid));

		g_params.radius = radius;
		g_params.dynamicFriction = .1f;
		g_params.viscosity = 2.0f;
		g_params.numIterations = 4;
		g_params.vorticityConfinement = 180.f;
		g_params.fluidRestDistance = restDistance;
		g_params.numPlanes = 5;
		g_params.cohesion = 0.001625f;
		g_params.collisionDistance = restDistance;
		g_params.solidPressure = 0.1f;
		g_params.restitution = 0.0f;
		g_params.relaxationFactor = 1.0f;
		g_params.relaxationMode = eNvFlexRelaxationLocal;

		g_numSubsteps = 2;

		g_maxDiffuseParticles = 64*1024;
		g_diffuseScale = 0.25f;		
		g_diffuseShadow = false;
		g_diffuseColor = 1.5f;
		g_diffuseMotionScale = 1.5f;
		g_params.diffuseBallistic = 35;
		g_params.diffuseThreshold *= 0.1f;
		
		g_drawPlaneBias = g_params.collisionDistance*1.5f;

		g_lightDistance *= 0.65f;
		
		g_fluidColor = Vec4(0.2f, 0.6f, 0.9f, 1.0f);

		// draw options
		g_drawDensity = true;
		g_drawDiffuse = true;
		g_drawEllipsoids = true;
		g_drawPoints = false;

		g_warmup = true;
	}

};
