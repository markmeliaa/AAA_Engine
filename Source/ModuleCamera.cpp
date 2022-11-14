#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleEditor.h"

#include <GL/glew.h>
#include "lib/MathGeoLib/Geometry/Frustum.h"

ModuleCamera::ModuleCamera()
{
	aspect = BASE_SCREEN_WIDTH / BASE_SCREEN_HEIGHT;
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	D_LOG("Create the camera");
	App->editor->log.emplace_back("Creating the camera");

	SetUpFrustum();
	proj = frustum.ProjectionMatrix();
	model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f), float4x4::RotateZ(pi / 4.0f), float3(2.0f, 1.0f, 0.0f));
	view = frustum.ViewMatrix();
	return true;
}

bool ModuleCamera::Start()
{
	return true;
}

update_status ModuleCamera::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	return true;
}

void ModuleCamera::SetUpFrustum()
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 100.0f);
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD * 90.0f, aspect); // aspect = 1.3f

	frustum.SetPos(float3(0.0f, 4.0f, 8.0f));
	frustum.SetFront(-float3::unitZ);
	frustum.SetUp(float3::unitY);

	float4x4 projectionGL = frustum.ProjectionMatrix().Transposed(); // < --Important to transpose!
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(*projectionGL.v);
}