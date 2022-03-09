

class TestFluid : public Scene
{
public:

	TestFluid(const char* name) : Scene(name) {}

	virtual void Initialize()
	{
		float minSize = 0.5f;
		float maxSize = 0.7f;

		float radius = 0.1f;
		float restDistance = radius * 0.55f;
		int group = 0;

		AddRandomConvex(6, Vec3(5.0f, -0.1f, 0.6f), 1.0f, 1.0f, Vec3(1.0f, 1.0f, 0.0f), 0.0f);

		float ly = 0.5f;

		g_numSolidParticles = g_buffers->positions.size();

		float sizex = 1.76f;
		float sizey = 2.20f;
		float sizez = 3.50f;

		int x = int(sizex / restDistance);
		int y = int(sizey / restDistance);
		int z = int(sizez / restDistance);

		CreateParticleGrid(Vec3(0.0f, restDistance * 0.5f, 0.0f), x, y, z, restDistance, Vec3(0.0f), 1.0f, false, 0.0f, NvFlexMakePhase(group++, eNvFlexPhaseSelfCollide | eNvFlexPhaseFluid));
		//CreateParticleGrid(Vec3(5, restDistance*0.5f, 5), x, y * 2, z, restDistance, Vec3(0.0f), 1.0f, false, 0.0f, NvFlexMakePhase(group++, eNvFlexPhaseSelfCollide | eNvFlexPhaseFluid));

		g_params.radius = radius;
		g_params.dynamicFriction = 0.0f;
		g_params.viscosity = 15.0f;
		g_params.numIterations = 3;
		g_params.vorticityConfinement = 100.f;
		g_params.fluidRestDistance = restDistance;
		g_params.numPlanes = 5;
		//g_params.cohesion = 0.05f;

		g_maxDiffuseParticles = 128 * 1024;
		g_diffuseScale = 0.75f;

		g_waveFloorTilt = -0.025f;

		g_lightDistance *= 0.5f;

		// draw options
		g_drawDensity = true;
		g_drawDiffuse = false;
		g_drawEllipsoids = true;
		g_drawPoints = false;

		g_warmup = true;
	}
};